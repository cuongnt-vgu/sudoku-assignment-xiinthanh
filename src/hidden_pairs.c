#include "hidden_pairs.h"



// contain 2 candidates x, y
bool check_value_hidden(Cell *p_cell, int x, int y) {
    return (is_candidate(p_cell, x) && is_candidate(p_cell, y));
}

int number_of_apperance(Cell **p_cells, int x) {
    int count = 0;
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (is_candidate(p_cells[i], x)) {
            count += 1;
        }
    }
    return count;
}

bool can_form_hidden_pair(Cell **p_cells, int x, int y) {
    return (number_of_apperance(p_cells, x) == 2 
            && number_of_apperance(p_cells, y) == 2);
}

bool is_in_pairs_list(HiddenPair *p_array, int size, Cell *p_a, Cell *p_b, int x, int y) {
    int count = 0;
    for (int i = 0; i < size; i++) {
        count = 0;
        count += (p_array[i].p_cells[0] == p_a);
        count += (p_array[i].p_cells[1] == p_b);
        count += (p_array[i].values[0] == x);
        count += (p_array[i].values[1] == y);
        if (count == 4) return true;
    }
    return false;
}

void add_hidden_pair(Cell *p_a, Cell *p_b, int x, int y, HiddenPair *pairs, int *counter) {
    if (!is_in_pairs_list(pairs, *counter, p_a, p_b, x, y)) {
        pairs[*counter].p_cells[0] = p_a;
        pairs[*counter].p_cells[1] = p_b;
        pairs[*counter].values[0] = x;
        pairs[*counter].values[1] = y;
        (*counter) += 1; 
    }
}

void run_pair_values(Cell **p_cells, int x, int y, HiddenPair *pairs, int *counter) {
    if (!can_form_hidden_pair(p_cells, x, y)) return;

    for (int i = 0; i < BOARD_SIZE - 1; i++) if (check_value_hidden(p_cells[i], x, y)) {
        for (int j = i + 1; j < BOARD_SIZE; j++) if (check_value_hidden(p_cells[j], x, y)) {
            // (i, j) will be a Pair Cells.

            if (p_cells[i]->num_candidates == 2 && p_cells[j]->num_candidates == 2) continue;  // no candidate to eliminate

            add_hidden_pair(p_cells[i], p_cells[j], x, y, pairs, counter);
        }
    }
}


void eliminate_candidates_hidden_pairs(HiddenPair pair) {
    for (int value = 1; value <= BOARD_SIZE; value++) {
        if (value == pair.values[0] || value == pair.values[1]) continue;

        if (is_candidate(pair.p_cells[0], value)) {
            unset_candidate(pair.p_cells[0], value);
        }
        if (is_candidate(pair.p_cells[1], value)) {
            unset_candidate(pair.p_cells[1], value);
        }
    }
}


int hidden_pairs(SudokuBoard *p_board)
{
    int counter = 0;
    HiddenPair pairs[34992 + 12];  // max size: 3 * 9 * C(2, 9) * C(2, 9) = 3 * 9 * (9*8/2) * (9*8/2) = 34992
    for (int x = 1; x <= BOARD_SIZE - 1; x++) {
        for (int y = x + 1; y <= BOARD_SIZE; y++) {
            // (x, y) will be a Pair Values
            for (int id = 0; id < BOARD_SIZE; id++) {
                run_pair_values(p_board->p_rows[id], x, y, pairs, &counter);
                run_pair_values(p_board->p_cols[id], x, y, pairs, &counter);
                run_pair_values(p_board->p_boxes[id], x, y, pairs, &counter);
            }
        }
    }

    for (int i = 0; i < counter; i++) {
        eliminate_candidates_hidden_pairs(pairs[i]);
    }

    return counter;
}