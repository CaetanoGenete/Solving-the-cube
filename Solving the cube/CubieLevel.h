#pragma once

#include "CUtils.h"

#define NO_CENTERS 6
#define NO_CORNERS 8
#define NO_EDGES 12

#define NO_SLICE_EDGES 4
#define NO_UD_EDGES 8

#define NO_FACE_EDGES 4

#define MAX_CORNER_ORI 3
#define MAX_EDGE_ORI 2

#define NO_UNIQUE_MOVES 6
#define NO_MOVES 18
#define REPEATS_TO_IDENTITY 4
#define NO_UNIQUE_REPEATS 3

#define NO_SYMMETRIES 4

#define NO_SYMMETRY_MOVES 16
#define ALL_SYMMETRIES 48

#define NO_FACES 6

enum FaceTurn: unsigned char {
	Ux1, Rx1, Fx1, Dx1, Lx1, Bx1,
	Ux2, Rx2, Fx2, Dx2, Lx2, Bx2,
	Ux3, Rx3, Fx3, Dx3, Lx3, Bx3
};

enum SymTurn : unsigned char {
	S_URF_DLB_120,
	S_F_B_180,
	S_U_D_90,
	S_U_F_D_B
};

enum Corner : unsigned char {
	URF, UFL, ULB, UBR, // U face corners
	DFR, DLF, DBL, DRB  // D face corners
};

enum Edge : unsigned char {
	UR, UF, UL, UB, // U face edges
	DR, DF, DL, DB, // D face edges
	FR, FL, BL, BR  // UD slice edges
};

enum Face : unsigned char{
	U, R, F, D, L, B
};

const Edge UD_slice_pieces[NO_SLICE_EDGES] = { FR, FL, BL, BR };
const Edge FB_slice_pieces[NO_SLICE_EDGES] = { UR, UL, DR, DL };
const Edge RL_slice_pieces[NO_SLICE_EDGES] = { UF, UB, DF, DB };

const Edge U_edge_pieces[NO_FACE_EDGES] = { UR, UF, UL, UB };
const Edge D_edge_pieces[NO_FACE_EDGES] = { DR, DF, DL, DB };

extern const Face pristine_center_perm_config[NO_FACES];

class CubieLevel {
	
private:

	Corner corner_perm_[NO_CORNERS]; // permutation of the corners
	Edge edge_perm_[NO_EDGES];		 // permutation of the edges

	unsigned char corner_ori_[NO_CORNERS]; // orientation of the corners
	unsigned char edge_ori_[NO_EDGES];	   // orientation of the edges

public:

	CubieLevel();

	CubieLevel(
		const Corner corner_perm[NO_CORNERS],
		const Edge   edge_perm[NO_EDGES],
		const unsigned char corner_ori[NO_CORNERS],
		const unsigned char edge_ori[NO_EDGES]
	);

	bool operator==(const CubieLevel& rhs);

	void multiplyEdgeOri(const CubieLevel& lhs);
	void multiplyCornerOri(const CubieLevel& lhs);

	void setCornerCombinationOri(unsigned short orientation, const Corner* pieces, unsigned char pieces_count, bool independant);
	void setEdgeCombinationOri(unsigned short orientation, const Edge* pieces, unsigned char pieces_count, bool independant);

	void multiplyCornerPerm(const CubieLevel& lhs);
	void multiplyEdgePerm(const CubieLevel& lhs);

	void multiplyAllCoordinates(const CubieLevel& lhs);

	void turnFace(const FaceTurn& face);

	void setCornerOriCoord(unsigned short orientation);
	void setEdgeOriCoord(unsigned short orientation);

	void setCornerPermCoord(unsigned short perm);
	void setEdgePermCoord(unsigned int perm);

	void setCornerCombination(unsigned short comb, const Corner* pieces, unsigned short count);
	void setEdgeCombination(unsigned short comb, const Edge* pieces, unsigned short count);

	void setUDEdgePerm(unsigned short perm);

	void setArrangedCornerCoord(unsigned short arranged, const Corner* pieces, unsigned short count);
	void setArrangedEdgeCoord(unsigned short arranged, const Edge* pieces, unsigned short count);

	bool isPristine();

	inline const Corner* corner_perm() const { // const getter
		return corner_perm_;
	}

	inline const Edge* edge_perm() const { // const getter
		return edge_perm_;
	}

	inline const unsigned char* corner_ori() const { // const getter
		return corner_ori_;
	}
	
	inline const unsigned char* edge_ori() const { // const getter
		return edge_ori_;
	}

};

extern const CubieLevel turn_cubes[NO_UNIQUE_MOVES];

extern CubieLevel sym_move_cubes[ALL_SYMMETRIES];
extern unsigned char inv_sym_index[ALL_SYMMETRIES];

extern unsigned char apply_sym_move[ALL_SYMMETRIES][NO_MOVES];
extern unsigned char sym_multiply[ALL_SYMMETRIES][ALL_SYMMETRIES];

void loadSymmetries();

std::ostream& operator<<(std::ostream& stream, const CubieLevel& level);



