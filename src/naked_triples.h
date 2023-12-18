#pragma once

#include "sudoku.h"
#include <stdlib.h>

typedef struct TripleCells {
    Cell *p_cells[3];
    int values[3];
} Triple;

bool check_value_triple(Cell *p_cell, int x, int y, int z);
bool is_in_triples_list(Triple *p_array, int size, Cell *p_a, Cell *p_b, Cell *p_c);
void form_naked_triple(Cell *p_a, Cell *p_b, Cell *p_c, int x, int y, int z, Triple *triples, int *counter);
void eliminate_candidates_triples(Cell **p_cells, Triple triple, int *eliminate_count);

void run_triple_values(Cell **p_cells, int x, int y, int z, Triple *triples, int *counter);


int naked_triples(SudokuBoard *p_board);