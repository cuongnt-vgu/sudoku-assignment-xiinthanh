#pragma once

#include "sudoku.h"

typedef struct HiddenSingle_impl
{
    Cell *p_cell;
    int value;
} HiddenSingle;

// check if value is a unique candidate in cells array.
bool check_unique(Cell **p_cells, int value);

// find hidden value for hidden singles, if possible.
int find_hidden_single_values(SudokuBoard *p_board, Cell *p_cell);

// if a cell is a hidden single, add to the hidden_cells array.
void find_hidden_single(SudokuBoard *p_board, Cell *p_cell, HiddenSingle *p_hidden_singles, int *p_counter);


// find hidden single cells in a board, return the number of hidden single cells
int hidden_singles(SudokuBoard *p_board);