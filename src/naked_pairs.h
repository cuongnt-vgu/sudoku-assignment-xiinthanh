#pragma once

#include "sudoku.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct NakedPair_impl
{
    Cell *p_cells[2];
    int *values;
} NakedPair;


bool is_naked_pairs(Cell *p_fixed_cell, Cell *p_check_cell);
void find_cell_pair_with(Cell **p_cells, int size, Cell *p_fixed_cell, NakedPair *pairs, int *counter);
void get_potential_naked_cells(Cell **p_cells, Cell *p_fixed_cell, Cell **potential_naked_cells, int *potential_count);

int naked_pairs(SudokuBoard *p_board);