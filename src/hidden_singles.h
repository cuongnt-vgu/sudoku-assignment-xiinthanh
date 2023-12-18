#pragma once

#include "sudoku.h"
#include <stdio.h>

typedef struct HiddenSingle_impl
{
    Cell *p_cell;
    int value;
} HiddenSingle;

bool check_unique(Cell **p_cells, int value);

// find hidden single values (the value that is unique among all the candidates for a row, a collumn, or a box)
int find_hidden_single_values(SudokuBoard *p_board, Cell *p_cell);


// find hidden single cells in a row, in a collumn or in a box
void find_hidden_single(SudokuBoard *p_board, Cell *p_cell, HiddenSingle *p_hidden_singles, int *p_counter);


// find hidden single cells in a board, return the number of hidden single cells
int hidden_singles(SudokuBoard *p_board);