#pragma once

#include "genetics.h"
#include "game.h"

int choose_move(Individual* individual, int board[HEIGHT][LENGTH], int color);
void simulate_move(int board[HEIGHT][LENGTH], int col, int player);

int play_game(Individual* player1, Individual* player2);