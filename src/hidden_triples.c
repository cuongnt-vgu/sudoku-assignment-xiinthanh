#include "hidden_triples.h"


bool check_value_hidden_triple(Cell *p_cell, int x, int y, int z) {
    int count = 0;
    count += is_candidate(p_cell, x);
    count += is_candidate(p_cell, y);
    count += is_candidate(p_cell, z);
    return (count >= 2);
}

bool valid_hidden_triple(Cell **p_cells, HiddenTriple triple) {
    // check candidates of the Triple can form the triple Values.
    for (int value_id = 0; value_id < 3; value_id++) {
        bool have_value_candidate = false;
        for (int cell_id = 0; cell_id < 3; cell_id++) {
            if (is_candidate(triple.p_cells[cell_id], triple.values[value_id])) {
                have_value_candidate = true;
                break;
            }
        }
        if (!have_value_candidate) return false;
    }

    // check other cells in row (or col or box), they should have any value in triple values as a candidate
    for (int i = 0; i < BOARD_SIZE; i++) {
        bool is_in_triple = false;
        for (int j = 0; j < 3; j++) {
            if (p_cells[i] == triple.p_cells[j]) {
                is_in_triple = true;
                break;
            }
        }
        if (is_in_triple) continue;
        for (int j = 0; j < 3; j++) {
            if (is_candidate(p_cells[i], triple.values[j])) {
                return false;
            }
        }
    }

    // check can eliminate candidates
    for (int cell_id = 0; cell_id < 3; cell_id++) {
        for (int value = 1; value <= BOARD_SIZE; value++) {
            bool is_triple_value = false;
            for (int value_id = 0; value_id < 3; value_id++) {
                if (value == triple.values[value_id]) {
                    is_triple_value = true;
                    break;
                }
            }
            if (is_triple_value) continue;

            if (is_candidate(triple.p_cells[cell_id], value)) {
                // printf("%d %d, %d - eliminate\n", triple.p_cells[cell_id]->row_index, triple.p_cells[cell_id]->col_index, value);
                return true;  // can eliminate value from triple.p_cells[cell_id]
            }
        }
    }

    return false;
}

bool is_in_hidden_triples_list(HiddenTriple *p_array, int size, Cell *p_a, Cell *p_b, Cell *p_c, int x, int y, int z) {
    int count = 0;
    for (int i = 0; i < size; i++) {
        count = 0;
        count += (p_array[i].p_cells[0] == p_a);
        count += (p_array[i].p_cells[1] == p_b);
        count += (p_array[i].p_cells[2] == p_c);
        count += (p_array[i].values[0] == x);
        count += (p_array[i].values[1] == y);
        count += (p_array[i].values[2] == z);
        if (count == 6) return true;
    }
    return false;
}

void form_hidden_triple(Cell **p_cells, Cell *p_a, Cell *p_b, Cell *p_c, int x, int y, int z, HiddenTriple *triples, int *counter) {
    // assign to check
    triples[*counter].p_cells[0] = p_a;
    triples[*counter].p_cells[1] = p_b;
    triples[*counter].p_cells[2] = p_c;
    triples[*counter].values[0] = x;
    triples[*counter].values[1] = y;
    triples[*counter].values[2] = z;

    if (valid_hidden_triple(p_cells, triples[*counter]) && !is_in_hidden_triples_list(triples, *counter, p_a, p_b, p_c, x, y, z)) {
        
        (*counter) += 1;
    }
}

void run_hidden_triple_values(Cell **p_cells, int x, int y, int z, HiddenTriple *triples, int *counter) {
        
    for (int i = 0; i < BOARD_SIZE - 2; i++) if (check_value_hidden_triple(p_cells[i], x, y, z)) {
        for (int j = i + 1; j < BOARD_SIZE - 1; j++) if (check_value_hidden_triple(p_cells[j], x, y, z)) {
            for (int k = j + 1; k < BOARD_SIZE; k++) if (check_value_hidden_triple(p_cells[k], x, y, z)) {
                // (i, j, k) will be a Triple Cells.

                form_hidden_triple(p_cells, p_cells[i], p_cells[j], p_cells[k], x, y, z, triples, counter);
            }
        }
    }
}

void eliminate_candidates_hidden_triples(HiddenTriple triple) {
    for (int value = 1; value <= BOARD_SIZE; value++) {
        bool cont = false;
        for (int i = 0; i < 3; i++) {
            if (value == triple.values[i]) {
                cont = true;
                break;
            }
        }
        if (cont) continue;

        for (int i = 0; i < 3; i++) {
            if (is_candidate(triple.p_cells[i], value)) {
                unset_candidate(triple.p_cells[i], value);
            }
        }
    }
}


int hidden_triples(SudokuBoard *p_board)
{

    int counter = 0;
    HiddenTriple triples[200000];  // <= C(3, 9) * 3 * 9 * C(3, 9) = 190512 (9rows + 9cols + 9boxes)
    for (int x = 1; x <= BOARD_SIZE - 2; x++) {
        for (int y = x + 1; y <= BOARD_SIZE - 1; y++) {
            for (int z = y + 1; z <= BOARD_SIZE; z++) {
                /// (x, y, z) will be the values of the Triples.
                for (int id = 0; id < BOARD_SIZE; id++) {

                    run_hidden_triple_values(p_board->p_rows[id], x, y, z, triples, &counter);
                    run_hidden_triple_values(p_board->p_cols[id], x, y, z, triples, &counter);
                    run_hidden_triple_values(p_board->p_boxes[id], x, y, z, triples, &counter);
                }
            }
        }
    }

    for (int i = 0; i < counter; i++) {
        eliminate_candidates_hidden_triples(triples[i]);
        // printf("%d %d, %d %d, %d %d ", triples[i].p_cells[0]->row_index, triples[i].p_cells[0]->col_index,
        //                                 triples[i].p_cells[1]->row_index, triples[i].p_cells[1]->col_index,
        //                                 triples[i].p_cells[2]->row_index, triples[i].p_cells[2]->col_index);
        // printf("- %d %d %d\n", triples[i].values[0], triples[i].values[1], triples[i].values[2]);
    }
    // printf("|||%d|||\n", eliminate_count);
    // printf("THIS IS NAKED TRIPLE\n");
    return counter;
}