#include "hidden_singles.h"

bool check_unique(Cell **p_cells, int value)
{
    int count = 0;
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (is_candidate(p_cells[i], value)) {
            count++;
        }
    }
    return (count == 1);
}

int find_hidden_single_values(SudokuBoard *p_board, Cell *p_cell) {
    for (int candy = 1; candy <= 9; candy++) {
        if (is_candidate(p_cell, candy)) {  // candy is a candidate.
            bool is_hidden_value = false;
            is_hidden_value |= check_unique(p_board->p_rows[p_cell->row_index], candy);
            is_hidden_value |= check_unique(p_board->p_cols[p_cell->col_index], candy);
            is_hidden_value |= check_unique(p_board->p_boxes[p_cell->box_index], candy);
            if (is_hidden_value) {
                return candy;
            }
        }
    }
    return 0;
}

// find hidden single cells in a row, in a collumn or in a box
void find_hidden_single(SudokuBoard *p_board, Cell *p_cell, HiddenSingle *p_hidden_singles, int *p_counter) {
    int hidden_singles_value = find_hidden_single_values(p_board, p_cell);
    if (hidden_singles_value) {
        p_hidden_singles[*p_counter].p_cell = p_cell;
        p_hidden_singles[*p_counter].value = hidden_singles_value;
        (*p_counter) += 1;
    }
}

// find hidden single cells in a board, return the number of hidden single cells
int hidden_singles(SudokuBoard *p_board) {
    int counter = 0;
    HiddenSingle hidden_singles[BOARD_SIZE * BOARD_SIZE];  // Max size

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (p_board->data[i][j].num_candidates <= 1) continue;
            find_hidden_single(p_board, &p_board->data[i][j], hidden_singles, &counter);
        }
    }

    for (int i = 0; i < counter; i++) {
        // unset all other candidates of hidden single.
        for(int other_candy = 1; other_candy <= 9; other_candy++) {
            if (is_candidate(hidden_singles[i].p_cell, other_candy)) {
                if (other_candy == hidden_singles[i].value) continue;
                unset_candidate(hidden_singles[i].p_cell, other_candy);
            }
        }
    }
    return counter;
}