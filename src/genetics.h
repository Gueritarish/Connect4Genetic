#pragma once

#define EVALUATE_NUMBER_GAME 20
#define GENERATIONS 500
#define POPULATION_SIZE 20
#define WEIGHTS_NUMBER 200

#define MUTATION_RATE 0.05

#define DISPLAY_GAME 0

typedef struct Individual {
    double weights[WEIGHTS_NUMBER]; // Tableau de poids pour le réseau neuronal
    double fitness;  // Score de performance (évalué après des parties)
} Individual;

typedef struct Population {
    Individual individuals[POPULATION_SIZE];
} Population;


void initialize_population(Population* population);
void evaluate_fitness(Individual* individual, Individual* best_previous);
Individual* select_parent(Population* population);
void crossover(Individual* parent1, Individual* parent2, Individual* child1, Individual* child2);
void mutate(Individual* individual);
void evolve_population(Population* population);
void run_genetic_algorithm();
void save_best_individual(Individual* best, const char* filename);
void load_best_individual(Individual* best, const char* filename);