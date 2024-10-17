#define _POSIX_C_SOURCE 199506L

#include "game.h"
#include "genetics.h"
#include "utils.h"
#include "input.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void display(int frame[HEIGHT][LENGTH], int color_turn)
{
    printf("\n\n\n\n");
    for (int i = 0; i < HEIGHT; i++)
    {
        printf("%s|",WHITE);
        for (int j = 0; j < LENGTH; j++)
        {
            if (j != 0)
                printf(" ");
            if (frame[i][j] == NO_PIECE)
                printf("%s.",WHITE);
            else
            {
                if (frame[i][j] == YELLOW_PIECE)
                    printf("%sO",YELLOW);
                else
                    printf("%sX",RED);
            }
        }
        printf("%s|\n",WHITE);
    }
    printf("|");
    for (int i = 0; i < LENGTH - 1; i++)
        printf("__");
    printf("_|\n");
    printf("|");
    if (check_game_state(frame) == DRAW)
        printf("    DRAW    ");
    else
    {
        if (color_turn == YELLOW_PIECE)
        {
            if (check_game_state(frame) == RED_PIECE)
                printf("  RED  WIN!");
            else
                printf(" YELLOW TURN");
        }
        else
        {
            if (check_game_state(frame) == YELLOW_PIECE)
                printf(" YELLOW WIN!");
            else
                printf("  RED  TURN ");
        }
    }
    printf(" |\n");
}

void display_help()
{
    printf("\
 ----- Connect4 Help Menu  ----- \n\
 Args : \n\
 First argument is an int, 0 1 or 2, depending on the number of IA you want to make play.\n\
 Second and Third agurments are file paths to AI saved files to make an already trained AI.\n");
    return;
}

int place_piece(int frame[HEIGHT][LENGTH], int piece, int row)
{
    for (int i = HEIGHT - 1; i >= 0; i--)
    {
        if (frame[i][row] == NO_PIECE)
        {
            frame[i][row] = piece;
            return SUCCESS;
        }
    }
    return FAIL;
}

int run_play_game(Individual* individual)
{
    int frame[HEIGHT][LENGTH];
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < LENGTH; j++)
        {
            frame[i][j] = NO_PIECE;
        }
    }

    int turn = YELLOW_PIECE;
    printf("Each turn, please enter a row between 1 and 7 then press enter.\n");

    while (check_game_state(frame) == RUNNING)
    {
        display(frame, turn);
        
        int column = -1; // Initialize to an invalid value

        if ((individual != NULL && turn == IA_COLOR)) {
            // AI
            int column = choose_move(individual, frame, turn);
            simulate_move(frame, column, turn);
        } else {
            // Human
            column = gather_input();
            while ((column < 1 || column > 7) || place_piece(frame, turn, column - 1) == FAIL)
            {
                printf("Please provide a row number, not full, between 1 and 7.\n");
                column = gather_input();
            }
        }
        turn = (turn == YELLOW_PIECE) ? RED_PIECE : YELLOW_PIECE;
    }

    // Display the final game result
    display(frame, turn);
    int finished = check_game_state(frame);
    if (finished == DRAW)
    {
        printf("\n\nDRAW. Nobody won.\n");
        return DRAW;
    }
    else if (finished == YELLOW_PIECE)
    {
        printf("\n\nYellow won.\n");
        return YELLOW_PIECE;
    }
    printf("\n\nRed won.\n");
    return RED_PIECE;
}

int main(int argc, char* argv[])
{
    if (argc < 2)
        display_help();
    else if (char_to_int(argv[1][0]) == 0)
    {
        run_play_game(NULL);
    }
    else if (char_to_int(argv[1][0]) == 1)
    {
        Individual individual;
        load_best_individual(&individual, "best_individual.log");
        individual.fitness = 0;
        run_play_game(&individual);
    }
    else if (char_to_int(argv[1][0]) == 2)
    {
        run_genetic_algorithm();
    }
    else
    {
        display_help();
    }
    return 0;
}