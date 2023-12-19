#pragma once

#include "sudoku.h"
#include <stdio.h>

typedef struct HiidenTripleCells {
    Cell *p_cells[3];
    int values[3];
} HiddenTriple;


bool check_value_hidden_triple(Cell *p_cell, int x, int y, int z);
bool valid_hidden_triple(Cell **p_cells, HiddenTriple triple);
bool is_in_hidden_triples_list(HiddenTriple *p_array, int size, Cell *p_a, Cell *p_b, Cell *p_c, int x, int y, int z);
void form_hidden_triple(Cell **p_cells, Cell *p_a, Cell *p_b, Cell *p_c, int x, int y, int z, HiddenTriple *triples, int *counter);
void run_hidden_triple_values(Cell **p_cells, int x, int y, int z, HiddenTriple *triples, int *counter);
void eliminate_candidates_hidden_triples(HiddenTriple triple);

int hidden_triples(SudokuBoard *p_board);