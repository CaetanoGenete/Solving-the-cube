#include "CubieLevel.h"
#include "Math.h"

#include <iostream>

#define NONE 0
#define CLOK 1
#define ANTI 2
#define SYMM 3

const unsigned char no_S_URF_DLB_120 = 3;
const unsigned char no_S_F_B_180     = 2;
const unsigned char no_S_U_D_90      = 4;
const unsigned char no_S_U_F_D_B     = 2;

const Corner pristine_corner_perm_config[NO_CORNERS] = {
	URF, UFL, ULB, UBR, DFR, DLF, DBL, DRB
};

const Edge pristine_edge_perm_config[NO_EDGES] = {
	UR, UF, UL, UB, DR, DF, DL, DB, FR, FL, BL, BR
};

const Face pristine_center_perm_config[NO_FACES] = {
	U, R, F, D, L, B
};

const unsigned char pristine_corner_ori_config[NO_CORNERS] = {
	0, 0, 0, 0, 0, 0, 0, 0
};

const unsigned char pristine_edge_ori_config[NO_EDGES] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

//is replaced by table
const Corner corner_turn_perm[NO_UNIQUE_MOVES][NO_CORNERS] = {
	{ UBR, URF, UFL, ULB, DFR, DLF, DBL, DRB }, //U 
	{ DFR, UFL, ULB, URF, DRB, DLF, DBL, UBR }, //R
	{ UFL, DLF, ULB, UBR, URF, DFR, DBL, DRB }, //F
	{ URF, UFL, ULB, UBR, DLF, DBL, DRB, DFR }, //D
	{ URF, ULB, DBL, UBR, DFR, UFL, DLF, DRB }, //L
	{ URF, UFL, UBR, DRB, DFR, DLF, ULB, DBL }, //B
};

//is replaced by table
const Edge edge_turn_perm[NO_UNIQUE_MOVES][NO_EDGES] = {
	{ UB, UR, UF, UL, DR, DF, DL, DB, FR, FL, BL, BR }, //U
	{ FR, UF, UL, UB, BR, DF, DL, DB, DR, FL, BL, UR }, //R
	{ UR, FL, UL, UB, DR, FR, DL, DB, UF, DF, BL, BR }, //F
	{ UR, UF, UL, UB, DF, DL, DB, DR, FR, FL, BL, BR }, //D
	{ UR, UF, BL, UB, DR, DF, FL, DB, FR, UL, DL, BR }, //L 
	{ UR, UF, UL, BR, DR, DF, DL, BL, FR, FL, UB, DB }  //B
};

const unsigned char corner_turn_ori[NO_UNIQUE_MOVES][NO_CORNERS] = {
	{ NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE }, //U
	{ ANTI, NONE, NONE, CLOK, CLOK, NONE, NONE, ANTI }, //R
	{ CLOK ,ANTI, NONE, NONE, ANTI, CLOK, NONE, NONE }, //F
	{ NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE }, //D
	{ NONE, CLOK, ANTI, NONE, NONE, ANTI, CLOK, NONE }, //L
	{ NONE, NONE, CLOK, ANTI, NONE, NONE, ANTI, CLOK }  //B
};

const unsigned char edge_turn_ori[NO_UNIQUE_MOVES][NO_EDGES] = {
	{ NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE }, //U
	{ NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE }, //R
	{ NONE, CLOK, NONE, NONE, NONE, CLOK, NONE, NONE, CLOK, CLOK, NONE, NONE }, //F
	{ NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE }, //D
	{ NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE }, //L
	{ NONE, NONE, NONE, CLOK, NONE, NONE, NONE, CLOK, NONE, NONE, CLOK, CLOK }  //B
};

const CubieLevel turn_cubes[NO_UNIQUE_MOVES] = {
	{ corner_turn_perm[Ux1], edge_turn_perm[Ux1], corner_turn_ori[Ux1], edge_turn_ori[Ux1]}, //U
	{ corner_turn_perm[Rx1], edge_turn_perm[Rx1], corner_turn_ori[Rx1], edge_turn_ori[Rx1]}, //R
	{ corner_turn_perm[Fx1], edge_turn_perm[Fx1], corner_turn_ori[Fx1], edge_turn_ori[Fx1]}, //F
	{ corner_turn_perm[Dx1], edge_turn_perm[Dx1], corner_turn_ori[Dx1], edge_turn_ori[Dx1]}, //D
	{ corner_turn_perm[Lx1], edge_turn_perm[Lx1], corner_turn_ori[Lx1], edge_turn_ori[Lx1]}, //L
	{ corner_turn_perm[Bx1], edge_turn_perm[Bx1], corner_turn_ori[Bx1], edge_turn_ori[Bx1]}  //B
};

const Corner corner_sym_perm[NO_SYMMETRIES][NO_CORNERS] = {
	{URF, DFR, DLF, UFL, UBR, DRB, DBL, ULB}, 
	{DLF, DFR, DRB, DBL, UFL, URF, UBR, ULB},
	{UBR, URF, UFL, ULB, DRB, DFR, DLF, DBL},
	{UFL, URF, UBR, ULB, DLF, DFR, DRB, DBL}
};

const Edge edge_sym_perm[NO_SYMMETRIES][NO_EDGES] = {
	{UF, FR, DF, FL, UB, BR, DB, BL, UR, DR, DL, UL},
	{DL, DF, DR, DB, UL, UF, UR, UB, FL, FR, BR, BL},
	{UB, UR, UF, UL, DB, DR, DF, DL, BR, FR, FL, BL},
	{UL, UF, UR, UB, DL, DF, DR, DB, FL, FR, BR, BL}
};

const unsigned char corner_sym_ori[NO_SYMMETRIES][NO_CORNERS] = {
	{CLOK, ANTI, CLOK, ANTI, ANTI, CLOK, ANTI, CLOK},
	{NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE},
	{NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE},
	{SYMM, SYMM, SYMM, SYMM, SYMM, SYMM, SYMM, SYMM}
};

const unsigned char edge_sym_ori[NO_SYMMETRIES][NO_EDGES] = {
	{CLOK, NONE, CLOK, NONE, CLOK, NONE, CLOK, NONE, CLOK, CLOK, CLOK, CLOK},
	{NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE},
	{NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, CLOK, CLOK, CLOK, CLOK},
	{NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE}
};

const CubieLevel unique_sym_cubes[NO_SYMMETRIES] = {
	{ corner_sym_perm[S_URF_DLB_120], edge_sym_perm[S_URF_DLB_120], corner_sym_ori[S_URF_DLB_120], edge_sym_ori[S_URF_DLB_120]}, 
	{ corner_sym_perm[S_F_B_180],	  edge_sym_perm[S_F_B_180],     corner_sym_ori[S_F_B_180],     edge_sym_ori[S_F_B_180]    }, 
	{ corner_sym_perm[S_U_D_90],      edge_sym_perm[S_U_D_90],      corner_sym_ori[S_U_D_90],      edge_sym_ori[S_U_D_90]     }, 
	{ corner_sym_perm[S_U_F_D_B],     edge_sym_perm[S_U_F_D_B],     corner_sym_ori[S_U_F_D_B],     edge_sym_ori[S_U_F_D_B]    }, 

};

CubieLevel sym_move_cubes[ALL_SYMMETRIES];
unsigned char inv_sym_index[ALL_SYMMETRIES];

unsigned char apply_sym_move[ALL_SYMMETRIES][NO_MOVES];
unsigned char sym_multiply[ALL_SYMMETRIES][ALL_SYMMETRIES];

CubieLevel::CubieLevel() : CubieLevel(pristine_corner_perm_config, pristine_edge_perm_config, pristine_corner_ori_config, pristine_edge_ori_config) {}

CubieLevel::CubieLevel(const Corner corner_perm[NO_CORNERS], const Edge edge_perm[NO_EDGES], const unsigned char corner_ori[NO_CORNERS], const unsigned char edge_ori[NO_EDGES])
{
	for (unsigned int corner = URF; corner <= DRB; corner++) {
		corner_perm_[corner] = corner_perm[corner];
		corner_ori_[corner]  = corner_ori[corner];
	}

	for (unsigned int edge = UR; edge <= BR; edge++) {
		edge_perm_[edge] = edge_perm[edge];
		edge_ori_[edge]  = edge_ori[edge];
	}
}

bool CubieLevel::operator==(const CubieLevel & rhs)
{
	for (unsigned char edge = 0; edge < NO_EDGES; edge++) {
		if (edge_perm_[edge] != rhs.edge_perm_[edge]) return false;
		if (edge_ori_[edge] != rhs.edge_ori_[edge]) return false;
	}

	for (unsigned char corner = 0; corner < NO_CORNERS; corner++) {
		if (corner_perm_[corner] != rhs.corner_perm_[corner]) return false;
		if (corner_ori_[corner] != rhs.corner_ori_[corner]) return false;
	}

	return true;
}

void CubieLevel::multiplyCornerOri(const CubieLevel& lhs) {

	//stores corner_ori values so that there is no interferance
	unsigned char temp_corner_ori[NO_CORNERS];

	for (unsigned int i = 0; i < NO_CORNERS; i++)
		temp_corner_ori[i] = corner_ori_[i];

	for (unsigned int i = 0; i < NO_CORNERS; i++) {

		//orientation of the piece that is going to be moved 
		unsigned short oriA = temp_corner_ori[lhs.corner_perm_[i]];

		//orientation of the current piece
		unsigned short oriB = lhs.corner_ori_[i];

		unsigned short resultOri = 0;

		//Case 1: Neiter reflected
		if (oriA < MAX_CORNER_ORI && oriB < MAX_CORNER_ORI) {
			resultOri = oriA + oriB;

			//MOD 3
			if (resultOri >= MAX_CORNER_ORI)
				resultOri -= MAX_CORNER_ORI;
		}

		//Case 2: rhs is reflected
		else if (oriA < MAX_CORNER_ORI && oriB >= MAX_CORNER_ORI) {
			resultOri = oriA + oriB;

			if (resultOri >= 6)
				resultOri -= MAX_CORNER_ORI;
		}

		//Case 3: lhs is reflected
		else if (oriA >= MAX_CORNER_ORI && oriB < MAX_CORNER_ORI) {
			resultOri = oriA - oriB;

			if (resultOri < MAX_CORNER_ORI)
				resultOri += MAX_CORNER_ORI;
		}

		//Case 4: both are reflected
		else if (oriA >= MAX_CORNER_ORI && oriB >= MAX_CORNER_ORI) {
			resultOri = oriA - oriB;

			if (resultOri < 0)
				resultOri += MAX_CORNER_ORI;
		}

		corner_ori_[i] = resultOri;
	}
}

void CubieLevel::multiplyEdgeOri(const CubieLevel& lhs) {
	//stores edge_ori values so that there is no interferance
	unsigned char temp_edge_ori[NO_EDGES];

	//copy ori into temp_edge_ori
	for (unsigned int i = 0; i < NO_EDGES; i++)
		temp_edge_ori[i] = edge_ori_[i];

	//apply turn
	for (unsigned int i = 0; i < NO_EDGES; i++)
		edge_ori_[i] = (temp_edge_ori[lhs.edge_perm_[i]] + lhs.edge_ori_[i]) % MAX_EDGE_ORI;
}

inline void multiplyCubeletPerm(unsigned int count, unsigned char* result, const unsigned char* lhs) {
	//stores perm values so that there is no interferance
	unsigned char* temp = new unsigned char[count];

	//apply turn
	for (unsigned char i = 0; i < count; i++)
		temp[i] = result[lhs[i]];

	//set result
	for (unsigned char i = 0; i < count; i++)
		result[i] = temp[i];

	delete[] temp;
}

void CubieLevel::multiplyCornerPerm(const CubieLevel & lhs) {
	multiplyCubeletPerm(NO_CORNERS, (unsigned char*)corner_perm_, (const unsigned char*)lhs.corner_perm_);
}

void CubieLevel::multiplyAllCoordinates(const CubieLevel & lhs)
{
	multiplyCornerPerm(lhs);
	multiplyEdgePerm(lhs);

	multiplyCornerOri(lhs);
	multiplyEdgeOri(lhs);
}

void CubieLevel::multiplyEdgePerm(const CubieLevel & lhs) {
	multiplyCubeletPerm(NO_EDGES, (unsigned char*)edge_perm_, (const unsigned char*)lhs.edge_perm_);
}

void CubieLevel::turnFace(const FaceTurn& face) {
	unsigned char unique_move = face % NO_UNIQUE_MOVES;
	unsigned char repeats = face / NO_UNIQUE_MOVES;

	for(unsigned char repeat = 0; repeat <= repeats; repeat++)
		multiplyAllCoordinates(turn_cubes[unique_move]);
}

inline void setOri(unsigned char count, unsigned char maxOri, unsigned short ori, unsigned char* result) {

	//sum of orientations
	unsigned short total = 0;
	for (char current_piece = count - 2; current_piece >= 0; current_piece--) {

		total += (result[current_piece] = ori % maxOri);

		ori /= maxOri;
	}
	
	//Calculate orientation of final piece
	result[count - 1] = (maxOri - (total % maxOri)) % maxOri;
}

void CubieLevel::setCornerOriCoord(unsigned short orientation) {
	setOri(NO_CORNERS, MAX_CORNER_ORI, orientation, corner_ori_);
}

void CubieLevel::setEdgeOriCoord(unsigned short orientation) {
	setOri(NO_EDGES, MAX_EDGE_ORI, orientation, edge_ori_);
}

inline void setPermCoord(const unsigned char count, unsigned int perm, unsigned char* permutation) {

	//set all permutations to 0
	for (unsigned int current_piece = 0; current_piece < count; current_piece++)
		permutation[current_piece] = 0;

	// Go through every piece execpt for the last
	unsigned int ctr = 0;
	for (char current_cubelet = count - 2, inv = 2; current_cubelet >= 0; current_cubelet--, inv++) {
		permutation[current_cubelet] = perm % inv;

		// move to next digit
		perm /= inv;

		for (unsigned char next_cubelet = current_cubelet + 1; next_cubelet < count; next_cubelet++)
			if (permutation[next_cubelet] >= permutation[current_cubelet]) permutation[next_cubelet] += 1;
	}
}

void CubieLevel::setCornerPermCoord(unsigned short perm) {
	setPermCoord(NO_CORNERS, perm, (unsigned char*)corner_perm_);
}

void CubieLevel::setEdgePermCoord(unsigned int perm) {
	setPermCoord(NO_EDGES, perm, (unsigned char*)edge_perm_);
}

inline void setCombination(unsigned short count, unsigned char* perm, unsigned short comb, const unsigned char* pieces, unsigned short pieces_count) {
	utils::ArrayList<unsigned char> remaining_pieces;

	for (unsigned int current_cubelet = 0; current_cubelet < count; current_cubelet++) {
		bool contained = false;

		for (unsigned int current_piece = 0; current_piece < pieces_count; current_piece++)
			if (current_cubelet == pieces[current_piece]) {
				contained = true;
				break;
			}

		if(!contained)
			remaining_pieces.add(current_cubelet);
	}

	char r = pieces_count;

	//loop over all pieces backwards or until no more pieces are left to place
	for (unsigned char current_cubelet = 0, remaining = 0; current_cubelet < count && r >= 0; current_cubelet++) {

		unsigned int possibilities = nCk_table[count - current_cubelet - 1][r];

		//if true, there must be a piece here
		if (possibilities <= comb) {
			perm[current_cubelet] = pieces[pieces_count - r--];

			comb -= possibilities;
		}
		else
			perm[current_cubelet] = remaining_pieces[remaining++];
		

	}

}

void CubieLevel::setCornerCombination(unsigned short comb, const Corner * pieces, unsigned short count) {
	setCombination(NO_CORNERS, (unsigned char*)corner_perm_, comb, (const unsigned char*)pieces, count);
}

void CubieLevel::setEdgeCombination(unsigned short comb, const Edge * pieces, unsigned short count) {
	setCombination(NO_EDGES, (unsigned char*)edge_perm_, comb, (const unsigned char*)pieces, count);
}

void CubieLevel::setUDEdgePerm(unsigned short perm) {
	setPermCoord(NO_UD_EDGES, perm, (unsigned char*)edge_perm_);
}

inline void setArranged(unsigned short count, unsigned char* perm, unsigned short arranged, const unsigned char* pieces, unsigned short pieces_count) {
	unsigned char* pieces_copy = new unsigned char[pieces_count];
	unsigned char* perm_data = new unsigned char[pieces_count];

	for (unsigned int piece = 0; piece < pieces_count; piece++)
		perm_data[piece] = piece;
	
	unsigned short perm_coord = arranged % factorial(pieces_count);
	unsigned short comb_coord = arranged / factorial(pieces_count);

	setPermCoord(pieces_count, perm_coord, perm_data);

	for (unsigned int piece = 0; piece < pieces_count; piece++)
		pieces_copy[piece] = pieces[perm_data[piece]];

	setCombination(count, perm, comb_coord, pieces_copy, pieces_count);

	delete[] pieces_copy;
	delete[] perm_data;
 }

void CubieLevel::setArrangedCornerCoord(unsigned short arranged, const Corner * pieces, unsigned short count)
{
	setArranged(NO_CORNERS, (unsigned char*)corner_perm_, arranged, (unsigned char*)pieces, count);
}

void CubieLevel::setArrangedEdgeCoord(unsigned short arranged, const Edge * pieces, unsigned short count)
{
	setArranged(NO_EDGES, (unsigned char*)edge_perm_, arranged, (unsigned char*)pieces, count);
}

bool CubieLevel::isPristine() {
	for (unsigned char edge = 0; edge < NO_EDGES; edge++) {
		if (edge_perm_[edge] != pristine_edge_perm_config[edge]) return false;
		if (edge_ori_[edge] != NONE) return false;
	}

	for (unsigned char corner = 0; corner < NO_CORNERS; corner++) {
		if (corner_perm_[corner] != pristine_corner_perm_config[corner]) return false;
		if (corner_ori_[corner] != NONE) return false;
	}

	return true;
}

void generateSymmetryMoveMult() {

	CubieLevel cube;
	for (unsigned char move = 0; move < NO_UNIQUE_MOVES; move++) {
		for (unsigned char repeats = 0; repeats < REPEATS_TO_IDENTITY; repeats++) {
			cube.turnFace((FaceTurn)move);

			if(repeats < REPEATS_TO_IDENTITY - 1)
			for (unsigned char symmetry = 0; symmetry < ALL_SYMMETRIES; symmetry++) {
				CubieLevel sym_cube = sym_move_cubes[symmetry];
				sym_cube.multiplyAllCoordinates(cube);
				sym_cube.multiplyAllCoordinates(sym_move_cubes[inv_sym_index[symmetry]]);

				for (unsigned char check_move = 0; check_move < NO_UNIQUE_MOVES; check_move++) {
					for (unsigned char check_repeats = 0; check_repeats < REPEATS_TO_IDENTITY; check_repeats++) {
						sym_cube.turnFace((FaceTurn)check_move);

						if (check_repeats < REPEATS_TO_IDENTITY - 1)
							if (sym_cube.isPristine())
								apply_sym_move[symmetry][repeats * NO_UNIQUE_MOVES + move] = (NO_UNIQUE_REPEATS - check_repeats - 1) * NO_UNIQUE_MOVES + check_move;
					}
				}
			}
		}
	}
}

void generateSymMult() {
	for (unsigned char lhs_symmetry = 0; lhs_symmetry < ALL_SYMMETRIES; lhs_symmetry++) {
		for (unsigned char rhs_symmetry = 0; rhs_symmetry < ALL_SYMMETRIES; rhs_symmetry++) {
			CubieLevel cube = sym_move_cubes[lhs_symmetry];
			cube.multiplyAllCoordinates(sym_move_cubes[rhs_symmetry]);

			for (unsigned char check_symmetry = 0; check_symmetry < ALL_SYMMETRIES; check_symmetry++)
				if (sym_move_cubes[check_symmetry] == cube)
					sym_multiply[lhs_symmetry][rhs_symmetry] = check_symmetry;
		}
	}
}

void generateSymmetryMoves() {

	CubieLevel cube;

	unsigned char index = 0;
	for (unsigned char s_urf_dlb_120 = 0; s_urf_dlb_120 < no_S_URF_DLB_120; s_urf_dlb_120++) {
		for (unsigned char s_u_d_90 = 0; s_u_d_90 < no_S_U_D_90; s_u_d_90++) {
			for (unsigned char s_f_b_180 = 0; s_f_b_180 < no_S_F_B_180; s_f_b_180++) {
				for (unsigned char s_u_f_d_b = 0; s_u_f_d_b < no_S_U_F_D_B; s_u_f_d_b++) {

					sym_move_cubes[index++] = cube;

					cube.multiplyAllCoordinates(unique_sym_cubes[S_U_F_D_B]);
				}
				cube.multiplyAllCoordinates(unique_sym_cubes[S_F_B_180]);
			}
			cube.multiplyAllCoordinates(unique_sym_cubes[S_U_D_90]);
		}
		cube.multiplyAllCoordinates(unique_sym_cubes[S_URF_DLB_120]);
	}

	for (unsigned int symmetry = 0; symmetry < ALL_SYMMETRIES; symmetry++) {
		for (unsigned int check_symmetry = 0; check_symmetry < ALL_SYMMETRIES; check_symmetry++) {
			CubieLevel sym_cube = sym_move_cubes[symmetry];
			sym_cube.multiplyAllCoordinates(sym_move_cubes[check_symmetry]);

			if (sym_cube.isPristine())
				inv_sym_index[symmetry] = check_symmetry;
			
			
		}
	}

}

void loadSymmetries()
{
	generateSymmetryMoves();
	generateSymmetryMoveMult();
	generateSymMult();
}

static const char* edge_to_string[NO_EDGES] = {
	"UR", "UF", "UL", "UB", "DR", "DF", "DL", "DB", "FR", "FL", "BL", "BR"
};

static const char* corner_to_string[NO_CORNERS] = {
	"URF", "UFL", "ULB", "UBR", "DFR", "DLF", "DBL", "DRB"
};

std::ostream& operator<<(std::ostream& stream, const CubieLevel& level) {

	std::cout << "corners: ";
	for (unsigned int i = 0; i < NO_CORNERS; i++) {
		std::cout << corner_to_string[level.corner_perm()[i]] << " | ";
		std::cout << (int)level.corner_ori()[i] << ((i == NO_CORNERS - 1) ? "" : ", ");

	}

	std::cout << std::endl << "edges: ";

	for (unsigned int i = 0; i < NO_EDGES; i++) {
		std::cout << edge_to_string[level.edge_perm()[i]] << " | ";
		std::cout << (int)level.edge_ori()[i] << ((i == NO_EDGES - 1) ? "" : ", ");
	}

	return stream;

}