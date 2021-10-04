#pragma once

#include "CubieLevel.h"

void loadTransitionTables();

const unsigned short no_edge_orientations = 2048;
const unsigned short no_corner_orientations = 2187;

const unsigned long int no_edge_permutations = 479001600;
const unsigned short no_raw_corner_permutations = 40320;
const unsigned short no_sym_corner_permutations = 2768;

const unsigned short no_ud_edge_permutations = 40320;

const unsigned short no_ud_slice_permutations = 24;
const unsigned short no_ud_slice_combinations = 495;

const unsigned short no_ud_slice_arranged = 11880;
const unsigned short no_rl_slice_arranged = 11880;
const unsigned short no_fb_slice_arranged = 11880;

const unsigned short no_u_edge_arranged = 11880;
const unsigned short no_u_edge_orientations = 16;

const unsigned int no_ud_edge_arranged_ori = 190080;

const unsigned short no_ud_edge_perm = 40320;

const unsigned short no_flip_ud_slice = 64430;
const unsigned int no_raw_flip_ud_slice = 1013760;

//PHASE 1 MOVE TABLES

extern unsigned int flip_ud_slice_move_table[no_flip_ud_slice][NO_MOVES];
extern unsigned short corner_ori_move_table[no_corner_orientations][NO_MOVES];

extern unsigned short corner_ori_conjugate[no_corner_orientations][NO_SYMMETRY_MOVES];

//PHASE 2 MOVE TABLES

extern unsigned short ud_edge_perm_move_table[no_ud_edge_permutations][NO_MOVES];
extern unsigned short ud_slice_arranged_move_table[no_ud_slice_arranged][NO_MOVES];
extern unsigned short rl_slice_arranged_move_table[no_rl_slice_arranged][NO_MOVES];
extern unsigned short fb_slice_arranged_move_table[no_fb_slice_arranged][NO_MOVES];

extern unsigned int get_ud_edge_perm[no_fb_slice_arranged][no_ud_slice_permutations];


extern unsigned short ud_edge_perm_conjugate[no_ud_edge_permutations][NO_SYMMETRY_MOVES];

extern unsigned int corner_perm_move_table[no_sym_corner_permutations][NO_MOVES];

//PHASE 1 PRUNINING

const unsigned int phase1_flip_UD_slice_goal_state = 0;
const unsigned int phase1_corner_ori_goal_state = 0;

extern unsigned char* corner_ori_flip_ud_slice_prune_table;

//PHASE 2 PRUNING

const unsigned int phase2_corner_perm_goal_state = 0;
const unsigned int phase2_ud_slice_arranged_goal_state = 0;
const unsigned int phase2_ud_edge_perm_goal_state = 0;

extern unsigned char* ud_edge_perm_corner_perm_prune_table;

extern utils::ArrayList<FaceTurn> phase2_move_set;

class CoordinateLevel {
private:

	//Orientation coordianates
	unsigned short corner_ori_;
	unsigned short edge_ori_;

	//Permutation coordinates
	unsigned int corner_perm_;
	unsigned int edge_perm_;

public:

	inline const unsigned short corner_ori() const {
		return corner_ori_;
	}

	inline const unsigned short edge_ori() const {
		return edge_ori_;
	}

	inline const unsigned int corner_perm() const {
		return corner_perm_;
	}

	inline const unsigned int edge_perm() const {
		return edge_perm_;
	}

};

unsigned short getCornerOriCoord(const CubieLevel& level);
unsigned short getEdgeOriCoord(const CubieLevel& level);

unsigned short getCornerCombinationOri(const CubieLevel & level, const Corner* pieces, unsigned char pieces_count, bool independant);
unsigned short getEdgeCombinationOri(const CubieLevel & level, const Edge* pieces, unsigned char pieces_count, bool independant);

unsigned short getCornerPermCoord(const CubieLevel& level);
unsigned int getEdgePermCoord(const CubieLevel& level);

unsigned short getCornerCombination(const CubieLevel& level, const Corner* pieces, unsigned char pieces_count);
unsigned short getEdgeCombination(const CubieLevel& level,   const Edge* pieces,   unsigned char pieces_count);

unsigned short getCornerArranged(const CubieLevel& level, const Corner* pieces, unsigned char pieces_count);
unsigned short getEdgeArranged(const CubieLevel& level, const Edge* pieces, unsigned char pieces_count);

unsigned short getUDEdgePerm(const CubieLevel& level);

unsigned int getFlipUDSliceCoord(const CubieLevel& level);
unsigned int getSymCornerPermCoord(const CubieLevel& level);

unsigned char getPruning(unsigned char* table, unsigned int index);

bool getEdgeParity(const CubieLevel& level);
bool getCornerParity(const CubieLevel& level);

unsigned int sym_table_move(unsigned int coord, unsigned int* move_table, unsigned char move);
