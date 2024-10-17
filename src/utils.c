#include "utils.h"

#include "game.h"

#include <stdlib.h>
#include <stdio.h>

// Fonction pour vérifier si une colonne est jouable
int is_column_playable(int board[HEIGHT][LENGTH], int col) {
    return board[0][col] == 0;
}

// Fonction qui vérifie si un coup dans une colonne donnée mène à une victoire
int is_winning_move(int board[HEIGHT][LENGTH], int player, int col) {
    // Simuler un coup dans la colonne
    for (int row = HEIGHT - 1; row >= 0; row--) {
        if (board[row][col] == 0) {
            board[row][col] = player;  // Simuler le coup du joueur
            int win = check_game_state(board) == player;  // Vérifier si cela mène à une victoire
            board[row][col] = 0;  // Annuler la simulation
            return win;
        }
    }
    return 0;  // Pas de place dans la colonne
}

// Fonction qui simule un coup dans une colonne
void simulate_move(int board[HEIGHT][LENGTH], int col, int player) {
    for (int row = HEIGHT - 1; row >= 0; row--) {
        if (board[row][col] == 0) {
            board[row][col] = player;
            break;
        }
    }
}

int choose_move(Individual* individual, int board[HEIGHT][LENGTH], int color) {
    double max_score = -RAND_MAX;        // Stocker le score maximal
    int best_column = 0;                 // Colonne avec le score le plus élevé
    
    // Parcourir chaque colonne pour évaluer les mouvements possibles
    for (int col = 0; col < LENGTH; col++) {
        // Vérifier si la colonne est jouable
        if (!is_column_playable(board, col)) {
            continue;  // La colonne est pleine, on la saute
        }

        // Vérifier si l'individu peut gagner en jouant dans cette colonne
        if (is_winning_move(board, color, col)) {  // '1' représente l'individu
            return col;  // Si oui, jouer immédiatement ici
        }

        // Vérifier si l'adversaire peut gagner au prochain tour et bloquer
        if (is_winning_move(board, color, col)) {  // '2' représente l'adversaire
            return col;  // Bloquer cette colonne
        }

        // Sinon, évaluer la colonne comme d'habitude avec les poids
        double score = 0.0;
        
        // Utiliser les poids pour évaluer chaque colonne
        for (int row = 0; row < HEIGHT; row++) {
            if (board[row][col] == 0) {
                // Calculer un score basé sur la position vide (ajuster selon ta stratégie)
                int weight_index = col * HEIGHT + row;
                score += individual->weights[weight_index];
            } else if (board[row][col] == color) {
                // Bonus pour les pions alliés
                score += individual->weights[col] * 0.5;
            } else if (board[row][col] == -color) {
                // Pénalité pour les pions adverses
                score -= individual->weights[col] * 0.5;
            }
        }

        if (score > max_score) {
            max_score = score;
            best_column = col;
        }
    }

    // Retourner la meilleure colonne trouvée
    return best_column;
}

int check_line(int frame[HEIGHT][LENGTH], int checking_val, int row, int col, int d_row, int d_col) {
    int count = 0;

    while (row >= 0 && row < HEIGHT && col >= 0 && col < LENGTH && frame[row][col] == checking_val) {
        count++;
        row += d_row;
        col += d_col;
    }

    return count;
}

int check_game_state(int board[HEIGHT][LENGTH]) {
    int zero_count = 0;
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < LENGTH; j++) {
            if (board[i][j] != NO_PIECE) {
                int checking_val = board[i][j];

                int horizontal = check_line(board, checking_val, i, j, 0, 1);
                if (horizontal >= 4)
                    return checking_val;

                int vertical = check_line(board, checking_val, i, j, 1, 0);
                if (vertical >= 4)
                    return checking_val;

                int diag_down = check_line(board, checking_val, i, j, 1, 1);
                if (diag_down >= 4)
                    return checking_val;

                int diag_up = check_line(board, checking_val, i, j, -1, 1);
                if (diag_up >= 4)
                    return checking_val;
            }
            else
            {
                zero_count++;
            }
        }
    }
    if (zero_count > 0)
        return RUNNING;
    return DRAW;
}

int play_game(Individual* player1, Individual* player2) {
    int board[HEIGHT][LENGTH] = { {0} }; // Plateau de jeu initialisé à vide
    int current_player = YELLOW_PIECE;   // Le joueur 1 commence (jeton jaune)
    int column;
    while (1) {
        // Le joueur 1 (YELLOW_PIECE) joue
        if (DISPLAY_GAME)
            display(board, current_player);
        if (current_player == YELLOW_PIECE)
            column = choose_move(player1, board, current_player);
        
        else
            column = choose_move(player2, board, current_player);

        simulate_move(board, column, current_player);

        // Vérifier s'il y a un gagnant après chaque coup
        int game_status = check_game_state(board);
        
        if (game_status != RUNNING && DISPLAY_GAME)
            display(board, current_player);
        
        if (game_status == YELLOW_PIECE) {
            return 1;  // Player 1 gagne
        }
        else if (game_status == RED_PIECE) {
            return -1; // Player 2 gagne
        }
        else if (game_status == DRAW) {
            return 0;  // Match nul
        }

        // Changer de joueur
        current_player = (current_player == YELLOW_PIECE) ? RED_PIECE : YELLOW_PIECE;
    }
}



