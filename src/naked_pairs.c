#include "naked_pairs.h"
#include "naked_triples.h"

bool is_naked_pairs(Cell *p_fixed_cell, Cell *p_check_cell) {
    if (p_check_cell->num_candidates != 2) return false;  // Pair of 2
    
    for (int value = 1; value <= BOARD_SIZE; value++) {
        if (is_candidate(p_fixed_cell, value)) {
            if (!is_candidate(p_check_cell, value)) {
                return false;
            }
        }
    }
    return true;
}

void find_cell_pair_with(Cell **p_cells, int size, Cell *p_fixed_cell, NakedPair *pairs, int *counter) {
    for (int i = 0; i < size; i++) {
        if (is_naked_pairs(p_fixed_cell, p_cells[i])) {
            pairs[*counter].p_cells[0] = p_fixed_cell;
            pairs[*counter].p_cells[1] = p_cells[i];
            pairs[*counter].values = get_candidates(p_fixed_cell);
            (*counter) += 1;
        }
    }
}

void get_potential_naked_cells(Cell **p_cells, Cell *p_fixed_cell, Cell **potential_naked_cells, int *potential_count) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (p_cells[i] == p_fixed_cell) continue;
        if (p_cells[i]->row_index < p_fixed_cell->row_index) continue;  // Only consider rows >= x
        if (p_cells[i]->col_index < p_fixed_cell->col_index) continue;  // and columns >= y

        if (is_in_list(potential_naked_cells, *potential_count, p_cells[i])) continue;  // already in the list

        potential_naked_cells[*potential_count] = p_cells[i];
        (*potential_count) += 1;

    }
}

void eliminate_candidates(Cell **p_cells, NakedPair pair, int *number_eliminate) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (p_cells[i] == pair.p_cells[0]) continue;
        if (p_cells[i] == pair.p_cells[1]) continue;
        if (is_candidate(p_cells[i], pair.values[0])) {
            unset_candidate(p_cells[i], pair.values[0]);
            (*number_eliminate) += 1;
        }
        if (is_candidate(p_cells[i], pair.values[1])) {
            unset_candidate(p_cells[i], pair.values[1]);
            (*number_eliminate) += 1;
        }
    }
}

int naked_pairs(SudokuBoard *p_board) {
    int counter = 0;
    NakedPair pairs[BOARD_SIZE * BOARD_SIZE * 3];

    int potential_count = 0;
    Cell *potential_naked_cells[BOARD_SIZE * 3];
    
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (p_board->data[i][j].num_candidates != 2) continue;  // Pairs
            potential_count = 0;  // reset potential array.

            get_potential_naked_cells(p_board->p_rows[p_board->data[i][j].row_index], &p_board->data[i][j], 
                                                                        potential_naked_cells, &potential_count);
            get_potential_naked_cells(p_board->p_cols[p_board->data[i][j].col_index], &p_board->data[i][j], 
                                                                        potential_naked_cells, &potential_count);
            get_potential_naked_cells(p_board->p_boxes[p_board->data[i][j].box_index], &p_board->data[i][j], 
                                                                        potential_naked_cells, &potential_count);

            find_cell_pair_with(potential_naked_cells, potential_count, &p_board->data[i][j], pairs, &counter);   
        }
    }

    int eliminate_count = 0;
    for (int i = 0; i < counter; i++) {
        // printf("%d %d %d %d\n", pairs[i].p_cells[0]->row_index, pairs[i].p_cells[0]->col_index,
        //                         pairs[i].p_cells[1]->row_index, pairs[i].p_cells[1]->col_index);

        if (pairs[i].p_cells[0]->row_index == pairs[i].p_cells[1]->row_index)
            eliminate_candidates(p_board->p_rows[pairs[i].p_cells[0]->row_index], pairs[i], &eliminate_count);
        if (pairs[i].p_cells[0]->col_index == pairs[i].p_cells[1]->col_index)
            eliminate_candidates(p_board->p_cols[pairs[i].p_cells[0]->col_index], pairs[i], &eliminate_count);
        if (pairs[i].p_cells[0]->box_index == pairs[i].p_cells[1]->box_index)
            eliminate_candidates(p_board->p_boxes[pairs[i].p_cells[0]->box_index], pairs[i], &eliminate_count);

        free(pairs[i].values);
    }
    // printf("THIS IS NAKED PAIRS\n");

    // printf("###############\n");
    // for (int i = 0; i < BOARD_SIZE; i++) {
    //     for (int j = 0; j < BOARD_SIZE; j++) {
    //         for (int k = 1; k <= 9; k++) {
    //             if (is_candidate(&p_board->data[i][j], k)) {
    //                 printf("%d ", k);
    //             }
    //         }
    //         printf("\n");
    //     }
    // }
    // printf("###############\n");

    if (!eliminate_count) return 0;
    return counter;
}