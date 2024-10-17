#include "genetics.h"

#include "utils.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

void initialize_population(Population* population) {
    srand(time(NULL));

    for (int i = 0; i < POPULATION_SIZE; i++)
    {
        for (int j = 0; j < WEIGHTS_NUMBER; j++) {
            population->individuals[i].weights[j] = ((double)rand() / RAND_MAX) * 2.0 - 1.0;
        }

        population->individuals[i].fitness = 0.0;
    }
}

void evaluate_fitness(Individual* individual, Individual* best_previous)
{
    int games_to_play = EVALUATE_NUMBER_GAME;  // Nombre de parties à jouer
    int wins = 0;
    int losses = 0;
    int draws = 0;

    // Simuler des jeux contre le meilleur individu
    for (int i = 0; i < games_to_play; i++) {
        // Chaque jeu est joué avec `individual` comme joueur 1 et `best_previous` comme joueur 2
        int result = play_game(individual, best_previous);

        // Analyser le résultat : 1 = victoire, -1 = défaite, 0 = match nul
        if (result == 1) {
            wins++;
        } else if (result == -1) {
            losses++;
        } else {
            draws++;
        }
    }

    // Calculer la fitness
    individual->fitness = (double)wins * 1.0 + (double)draws * 0.5 - (double)losses * 1.0;
}

void sort_population_by_fitness(Population* population) {
    for (int i = 0; i < POPULATION_SIZE - 1; i++) {
        // Trouver l'individu avec la meilleure fitness dans le sous-ensemble non trié
        int max_index = i;
        for (int j = i + 1; j < POPULATION_SIZE; j++) {
            if (population->individuals[j].fitness > population->individuals[max_index].fitness) {
                max_index = j;
            }
        }

        // Échanger l'individu avec la meilleure fitness avec l'individu à l'indice 'i'
        if (max_index != i) {
            Individual temp = population->individuals[i];
            population->individuals[i] = population->individuals[max_index];
            population->individuals[max_index] = temp;
        }
    }
}


void crossover(Individual* parent1, Individual* parent2, Individual* child1, Individual* child2) {
    // Choisir un point de croisement aléatoire
    int crossover_point = rand() % WEIGHTS_NUMBER;

    // Créer le premier enfant
    for (int i = 0; i < WEIGHTS_NUMBER; i++) {
        if (i < crossover_point) {
            child1->weights[i] = parent1->weights[i]; // Prendre du parent 1
            child2->weights[i] = parent2->weights[i]; // Prendre du parent 2
        } else {
            child1->weights[i] = parent2->weights[i]; // Prendre du parent 2
            child2->weights[i] = parent1->weights[i]; // Prendre du parent 1
        }
    }

    mutate(child1);
    mutate(child2);
}

void mutate(Individual* individual) {
    for (int i = 0; i < WEIGHTS_NUMBER; i++) {
        // Générer un nombre aléatoire et comparer à la taux de mutation
        double random_value = (double)rand() / RAND_MAX; // Valeur entre 0 et 1
        if (random_value < MUTATION_RATE) {
            // Appliquer une mutation : par exemple, ajouter un bruit aléatoire
            double mutation_amount = ((double)rand() / RAND_MAX) * 2.0 - 1.0; // Valeur entre -1 et 1
            individual->weights[i] += mutation_amount; // Ajouter le bruit au poids
        }
    }
}

Individual get_best(Population* population)
{
    int number_contestant = POPULATION_SIZE;
    int list_winner[POPULATION_SIZE];
    for (int i = 0; i < POPULATION_SIZE; i++)
    {
        list_winner[i] = i;
    }
    int winp1 = 0, winp2 = 0;
    while(number_contestant != 1)
    {
        for (int n = 0; n < number_contestant - 1; n += 2)
        {
            winp1 = 0, winp2 = 0;
            for (int i = 0; i < 10; i++)
            {
                int res = play_game(&population->individuals[list_winner[n]],&population->individuals[list_winner[n + 1]]);
                if (res == YELLOW_PIECE)
                    winp1++;
                if (res == RED_PIECE)
                    winp2++;
            }
            if (winp1 >= winp2)
            {
                list_winner[n / 2] = list_winner[n];
            }
            else
                list_winner[n / 2] = list_winner[n + 1];
        }
        number_contestant /= 2;
    }
    return population->individuals[list_winner[0]];
}

void evolve_population(Population* population) {
    Population new_population;
    Individual best_former = get_best(population);

    sort_population_by_fitness(population);
    // Boucle pour générer la nouvelle population
    for (int i = 0; i < POPULATION_SIZE / 2; i += 2) {
        // Sélectionner deux parents avec la sélection par roulette
        Individual* parent1 = &population->individuals[i];
        Individual* parent2 = &population->individuals[i + 1];

        Individual child1, child2;

        // Effectuer le croisement pour créer deux enfants
        crossover(parent1, parent2, &child1, &child2);

        // Appliquer la mutation sur les enfants
        mutate(&child1);
        mutate(&child2);

        // Évaluer la fitness des enfants
        evaluate_fitness(&child1, &best_former);
        evaluate_fitness(&child2, &best_former);

        // Ajouter les enfants à la nouvelle population
        new_population.individuals[i] = child1;
        new_population.individuals[i + 1] = child2;
        new_population.individuals[2 * i] = *parent1;
        new_population.individuals[2 * i + 1] = *parent2;
    }

    // Remplacer l'ancienne population par la nouvelle population
    *population = new_population;
}

void run_genetic_algorithm() {
    // Initialiser la population
    Population population;
    initialize_population(&population); // Assurez-vous d'avoir une fonction pour cela

    // Boucle sur le nombre de générations
    for (int generation = 0; generation < GENERATIONS; generation++) {
        // Par exemple, afficher la meilleure fitness de la génération actuelle
        double best_fitness = population.individuals[0].fitness; // À ajuster pour trouver le meilleur
        for (int i = 1; i < POPULATION_SIZE; i++) {
            if (population.individuals[i].fitness > best_fitness) {
                best_fitness = population.individuals[i].fitness;
            }
        }
        printf("Génération %d : Meilleure fitness = %.2f\n", generation, best_fitness);

        // Évoluer la population
        evolve_population(&population);
    }

    // Optionnel : Afficher le meilleur individu après toutes les générations
    Individual best_individual = population.individuals[0]; // À ajuster pour trouver le meilleur
    for (int i = 1; i < POPULATION_SIZE; i++) {
        if (population.individuals[i].fitness > best_individual.fitness) {
            best_individual = population.individuals[i];
        }
    }
    save_best_individual(&best_individual, "best_individual.log");
    printf("Meilleur individu final avec fitness = %.2f\n", best_individual.fitness);
}

void save_best_individual(Individual* best, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    // Écrire les poids de l'individu dans le fichier
    for (int i = 0; i < WEIGHTS_NUMBER; i++) {
        fprintf(file, "%f\n", best->weights[i]); // Utiliser %f pour les flottants
    }

    fclose(file);
    printf("Meilleur individu sauvegardé dans %s\n", filename);
}

void load_best_individual(Individual* best, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    // Lire les poids de l'individu à partir du fichier
    for (int i = 0; i < WEIGHTS_NUMBER; i++) {
        fscanf(file, "%lf\n", &best->weights[i]); // Utiliser %lf pour les doubles
    }

    fclose(file);
    printf("Meilleur individu chargé depuis %s\n", filename);
}