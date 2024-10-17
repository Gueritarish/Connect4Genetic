#pragma once

#include "genetics.h"

#define HEIGHT 6
#define LENGTH 7

#define SUCCESS 1
#define FAIL 0

#define DRAW 20
#define RUNNING 10

#define NO_PIECE 0
#define YELLOW_PIECE 1
#define RED_PIECE -1
#define IA_COLOR RED_PIECE

#define WHITE "\033[0m"
#define RED "\033[0;31m"
#define YELLOW "\033[0;33m"

void display(int frame[HEIGHT][LENGTH], int color_turn);
int check_game_state(int frame[HEIGHT][LENGTH]);
int run_play_game(Individual* Individual);