#include "naked_triples.h"
#include <stdio.h>


bool check_value_triple(Cell *p_cell, int x, int y, int z) {
    if (p_cell->num_candidates < 2 || 3 < p_cell->num_candidates) return false;
    int count = 0;
    count += is_candidate(p_cell, x);
    count += is_candidate(p_cell, y);
    count += is_candidate(p_cell, z);
    return (count == p_cell->num_candidates);
}

bool is_in_triples_list(Triple *p_array, int size, Cell *p_a, Cell *p_b, Cell *p_c) {
    int count = 0;
    for (int i = 0; i < size; i++) {
        count = 0;
        count += (p_array[i].p_cells[0] == p_a);
        count += (p_array[i].p_cells[1] == p_b);
        count += (p_array[i].p_cells[2] == p_c);
        if (count == 3) return true;
    }
    return false;
}

void form_naked_triple(Cell *p_a, Cell *p_b, Cell *p_c, int x, int y, int z, Triple *triples, int *counter) {
    if (!is_in_triples_list(triples, *counter, p_a, p_b, p_c)) {
        triples[*counter].p_cells[0] = p_a;
        triples[*counter].p_cells[1] = p_b;
        triples[*counter].p_cells[2] = p_c;
        triples[*counter].values[0] = x;
        triples[*counter].values[1] = y;
        triples[*counter].values[2] = z;
        (*counter) += 1; 
    }
}

void eliminate_candidates_triples(Cell **p_cells, Triple triple, int *eliminate_count) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        bool cont = false;
        for (int cell_id = 0; cell_id < 3; cell_id++) {
            if (p_cells[i] == triple.p_cells[cell_id]) cont = true;
        }
        if (cont) continue;
        for (int value_id = 0; value_id < 3; value_id++) {
            if (is_candidate(p_cells[i], triple.values[value_id])) {
                unset_candidate(p_cells[i], triple.values[value_id]);
                // printf("%d %d %d\n", p_cells[i]->row_index, p_cells[i]->col_index, triple.values[value_id]);
                (*eliminate_count) += 1;
            }
        }
    }
}

void run_triple_values(Cell **p_cells, int x, int y, int z, Triple *triples, int *counter) {
        
    for (int i = 0; i < BOARD_SIZE - 2; i++) if (check_value_triple(p_cells[i], x, y, z)) {
        for (int j = i + 1; j < BOARD_SIZE - 1; j++) if (check_value_triple(p_cells[j], x, y, z)) {
            for (int k = j + 1; k < BOARD_SIZE; k++) if (check_value_triple(p_cells[k], x, y, z)) {
                // (i, j, k) will be a Triple Cells.

                form_naked_triple(p_cells[i], p_cells[j], p_cells[k], x, y, z, triples, counter);

            }
        }
    }
}

int naked_triples(SudokuBoard *p_board)
{
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


    int counter = 0;
    Triple triples[2300];  // <= C(3, 9) * 3 * 9 = 2268 (9rows + 9cols + 9boxes)
    for (int x = 1; x <= BOARD_SIZE - 2; x++) {
        for (int y = x + 1; y <= BOARD_SIZE - 1; y++) {
            for (int z = y + 1; z <= BOARD_SIZE; z++) {
                /// (x, y, z) will be the values of the Triples.
                for (int id = 0; id < BOARD_SIZE; id++) {

                    run_triple_values(p_board->p_rows[id], x, y, z, triples, &counter);
                    run_triple_values(p_board->p_cols[id], x, y, z, triples, &counter);
                    run_triple_values(p_board->p_boxes[id], x, y, z, triples, &counter);
                }
            }
        }
    }

    int eliminate_count = 0;
    for (int i = 0; i < counter; i++) {
        int row_index = triples[i].p_cells[0]->row_index;
        if (triples[i].p_cells[1]->row_index == row_index && triples[i].p_cells[2]->row_index == row_index) {  // row
            eliminate_candidates_triples(p_board->p_rows[row_index], triples[i], &eliminate_count);
        }
        int col_index = triples[i].p_cells[0]->col_index;
        if (triples[i].p_cells[1]->col_index == col_index && triples[i].p_cells[2]->col_index == col_index) {  // row
            eliminate_candidates_triples(p_board->p_cols[col_index], triples[i], &eliminate_count);
        }
        int box_index = triples[i].p_cells[0]->box_index;
        if (triples[i].p_cells[1]->box_index == box_index && triples[i].p_cells[2]->box_index == box_index) {  // row
            eliminate_candidates_triples(p_board->p_boxes[box_index], triples[i], &eliminate_count);
        }
        // printf("%d %d, %d %d, %d %d ", triples[i].p_cells[0]->row_index, triples[i].p_cells[0]->col_index,
        //                                 triples[i].p_cells[1]->row_index, triples[i].p_cells[1]->col_index,
        //                                 triples[i].p_cells[2]->row_index, triples[i].p_cells[2]->col_index);
        // printf("- %d %d %d\n", triples[i].values[0], triples[i].values[1], triples[i].values[2]);
    }
    // printf("|||%d|||\n", eliminate_count);
    // printf("THIS IS NAKED TRIPLE\n");
    if (!eliminate_count) return 0;
    return counter;
}