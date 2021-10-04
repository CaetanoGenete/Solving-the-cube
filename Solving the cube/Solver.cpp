#include "Solver.h"

#include "CubieLevel.h"
#include "CoordinateLevel.h"

const utils::String move_to_string[NO_MOVES] = {
	"U " , "R " , "F " , "D " , "L " , "B " ,
	"U2 ", "R2 ", "F2 ", "D2 ", "L2 ", "B2 ",
	"U' ", "R' ", "F' ", "D' ", "L' ", "B' "
};

const FaceTurn inv_face_turn[NO_MOVES] = {
	Ux3, Rx3, Fx3, Dx3, Lx3, Bx3, Ux2, Rx2, Fx2, Dx2, Lx2, Bx2, Ux1, Rx1, Fx1, Dx1, Lx1, Bx1
};

#define MAX_PHASE_1_DEPTH 12
#define MAX_PHASE_2_DEPTH 18

#define SOLUTION_NOT_FOUND -1
#define START 0

unsigned char getInverseMove(unsigned char move) {
	return inv_face_turn[move];
}

bool phase2Search(
	unsigned char* moves,

	unsigned int ud_slice_arranged,
	unsigned int ud_edge_perm,
	unsigned int sym_corner_perm,

	unsigned int current_depth,
	unsigned int remaining_depth
) {
	unsigned int class_index = sym_corner_perm / NO_SYMMETRY_MOVES;

	//If the coordinates have reached the goal state, return search successfull

	//If maximal depth has been reached, exit
	if (remaining_depth == 0) {
		if (class_index      == phase2_corner_perm_goal_state 
		&& ud_edge_perm      == phase2_ud_edge_perm_goal_state
		&& ud_slice_arranged == phase2_ud_slice_arranged_goal_state)
			return true;
		else
			return false;
	}

	unsigned int symmetry = sym_corner_perm % NO_SYMMETRY_MOVES;
	unsigned int ud_edge_perm_conj = ud_edge_perm_conjugate[ud_edge_perm][symmetry];

	//If pruning value is greater than remaining depth, then solution is impossible. Exit this branch
	if (getPruning(ud_edge_perm_corner_perm_prune_table, (ud_edge_perm_conj * no_sym_corner_permutations + class_index)) > remaining_depth)
		return false;

	//Apply all moves 
	for (unsigned char phase2_move = 0; phase2_move < phase2_move_set.getSize(); phase2_move++) {
		unsigned char move = phase2_move_set[phase2_move];

		if(current_depth != 0)
		if (move == getInverseMove(moves[current_depth - 1]) || move == moves[current_depth - 1])
			continue;

		unsigned int new_sym_corner_perm = sym_table_move(sym_corner_perm, (unsigned int*)corner_perm_move_table, move);

		unsigned int new_ud_slice_arranged = ud_slice_arranged_move_table[ud_slice_arranged][move];
		unsigned int new_ud_edge_perm = ud_edge_perm_move_table[ud_edge_perm][move];

		moves[current_depth] = move;

		//After every move, continue searching down
		if (phase2Search(moves, new_ud_slice_arranged, new_ud_edge_perm, new_sym_corner_perm, current_depth + 1, remaining_depth - 1))
			return true;
	}

	return false;
}

int phase2SearchStart(
	unsigned char* moves,

	unsigned int ud_slice_arranged,
	unsigned int rl_slice_arranged,
	unsigned int fb_slice_arranged,
	unsigned int sym_corner_perm,

	unsigned int current_depth,
	unsigned int remaining_depth
) {
	
	if (current_depth != 0) {
		unsigned char prev_move = moves[current_depth - 1];

		for (unsigned char move = 0; move < phase2_move_set.getSize(); move++)
			if (phase2_move_set[move] == prev_move)  return SOLUTION_NOT_FOUND;
	}

	//apply phase1 solution to phase2 coordinates
	for (unsigned char prev_move = 0; prev_move < current_depth; prev_move++) {
		unsigned char move = moves[prev_move];

		ud_slice_arranged = ud_slice_arranged_move_table[ud_slice_arranged][move];
		rl_slice_arranged = rl_slice_arranged_move_table[rl_slice_arranged][move];
		fb_slice_arranged = fb_slice_arranged_move_table[fb_slice_arranged][move];

		sym_corner_perm = sym_table_move(sym_corner_perm, (unsigned int*)corner_perm_move_table, move);
	}

	unsigned int ud_edge_perm = get_ud_edge_perm[rl_slice_arranged][fb_slice_arranged % no_ud_slice_permutations];

	//start phase2
	for (unsigned int depth = 0; depth <= remaining_depth && depth <= MAX_PHASE_2_DEPTH; depth++) {
		if (phase2Search(moves, ud_slice_arranged, ud_edge_perm, sym_corner_perm, current_depth, depth))
			return depth;
	}

	return SOLUTION_NOT_FOUND;
}

int phase1Search(
	unsigned char* moves,

	unsigned int flip_ud_slice, 
	unsigned int corner_ori, 
	unsigned int ud_slice_arranged, 
	unsigned int rl_slice_arranged, 
	unsigned int fb_slice_arranged, 
	unsigned int sym_corner_perm, 
	
	unsigned int current_depth,
	unsigned int remaining_depth,
	unsigned int max_depth
) {

	unsigned int class_index = flip_ud_slice / NO_SYMMETRY_MOVES;
	
	//If the coordinates have reached the goal state, return search successfull

	//If maximal depth has been reached, exit
	if (remaining_depth == 0) {
		if (class_index == phase1_flip_UD_slice_goal_state && corner_ori == phase1_corner_ori_goal_state) {
			int phase_2_depth = phase2SearchStart(moves, ud_slice_arranged, rl_slice_arranged, fb_slice_arranged, sym_corner_perm, current_depth, max_depth - current_depth);

			if (phase_2_depth != SOLUTION_NOT_FOUND)
				return current_depth + phase_2_depth;
			else
				return SOLUTION_NOT_FOUND;
			
		}
		else
			return SOLUTION_NOT_FOUND;
	}

	unsigned int symmetry = flip_ud_slice % NO_SYMMETRY_MOVES;
	unsigned int conjugate_ori = corner_ori_conjugate[corner_ori][symmetry];

	//If pruning value is greater than remaining depth, then solution is impossible. Exit this branch
	if (getPruning(corner_ori_flip_ud_slice_prune_table, (class_index * no_corner_orientations + conjugate_ori)) > remaining_depth) 
		return SOLUTION_NOT_FOUND;

	//Apply all moves 
	for (unsigned char move = 0; move < NO_MOVES; move++) {
		if (current_depth != 0)
			if (move == getInverseMove(moves[current_depth - 1]) || move == moves[current_depth - 1])
				continue;

		unsigned int new_flip_ud_slice = sym_table_move(flip_ud_slice, (unsigned int*)flip_ud_slice_move_table, move);		
		unsigned int new_corner_ori = corner_ori_move_table[corner_ori][move];

		moves[current_depth] = move;

		int phase_1_depth = phase1Search(moves, new_flip_ud_slice, new_corner_ori, ud_slice_arranged, rl_slice_arranged, fb_slice_arranged, sym_corner_perm, current_depth + 1, remaining_depth - 1, max_depth);

		//After every move, continue searching down
		if (phase_1_depth != SOLUTION_NOT_FOUND)
			return phase_1_depth;
	}

	return SOLUTION_NOT_FOUND;
}

utils::ArrayList<unsigned char> search(const CubieLevel & level, unsigned int max_depth) {

	unsigned int flip_ud_slice_coord = getFlipUDSliceCoord(level);
	unsigned int corner_ori_coord = getCornerOriCoord(level);

	unsigned int sym_corner_perm = getSymCornerPermCoord(level);

	unsigned int ud_slice_arranged = getEdgeArranged(level, UD_slice_pieces, NO_SLICE_EDGES);
	unsigned int rl_slice_arranged = getEdgeArranged(level, RL_slice_pieces, NO_SLICE_EDGES);
	unsigned int fb_slice_arranged = getEdgeArranged(level, FB_slice_pieces, NO_SLICE_EDGES);

	unsigned char* moves = new unsigned char[max_depth];
	
	//IDA* algorithm
	int result_depth = SOLUTION_NOT_FOUND;
	for (unsigned int depth = 0; depth <= MAX_PHASE_1_DEPTH && depth <= max_depth; depth++) {
		result_depth = phase1Search(moves, flip_ud_slice_coord, corner_ori_coord, ud_slice_arranged, rl_slice_arranged, fb_slice_arranged, sym_corner_perm, START, depth, max_depth);

		if (result_depth != SOLUTION_NOT_FOUND)
			break;
	}

	utils::ArrayList<unsigned char> result;

	if (result_depth != SOLUTION_NOT_FOUND)
		for (unsigned char i = 0; i < result_depth; i++)
			result.add(moves[i]);

	delete[] moves;

	return result;
}
