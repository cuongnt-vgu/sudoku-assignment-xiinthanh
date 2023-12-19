#pragma once

#include "sudoku.h"
#include <stdio.h>

typedef struct PairCells {
    Cell *p_cells[2];
    int values[2];
} HiddenPair;


bool check_value_hidden(Cell *p_cell, int x, int y);
int number_of_apperance(Cell **p_cells, int x);
bool can_form_hidden_pair(Cell **p_cells, int x, int y);
bool is_in_pairs_list(HiddenPair *p_array, int size, Cell *p_a, Cell *p_b, int x, int y);
void add_hidden_pair(Cell *p_a, Cell *p_b, int x, int y, HiddenPair *pairs, int *counter);
void run_pair_values(Cell **p_cells, int x, int y, HiddenPair *pairs, int *counter);

void eliminate_candidates_hidden_pairs(HiddenPair pair, int *eliminate_count);

int hidden_pairs(SudokuBoard *p_board);
