#include "CoordinateLevel.h"
#include "CUtils.h"

#include "FileSystem.h"

#include "Math.h"

#include <assert.h>

#define PRUNING_EMPTY 0xFF

//PHASE 1 COORDINATE TABLES

unsigned int flip_ud_slice_class_index_to_representative[no_flip_ud_slice];

unsigned int flip_ud_slice_move_table[no_flip_ud_slice][NO_MOVES];
unsigned short edge_ori_move_table[no_edge_orientations][NO_MOVES];
unsigned short corner_ori_move_table[no_corner_orientations][NO_MOVES];

unsigned short corner_ori_conjugate[no_corner_orientations][NO_SYMMETRY_MOVES];

//PHASE 2 COORDINATE TABLES

unsigned int corner_perm_class_index_to_representative[no_sym_corner_permutations];

unsigned short ud_edge_perm_move_table[no_ud_edge_permutations][NO_MOVES];
unsigned short ud_slice_arranged_move_table[no_ud_slice_arranged][NO_MOVES];
unsigned short rl_slice_arranged_move_table[no_rl_slice_arranged][NO_MOVES];
unsigned short fb_slice_arranged_move_table[no_fb_slice_arranged][NO_MOVES];
unsigned int corner_perm_move_table[no_sym_corner_permutations][NO_MOVES];

unsigned int get_ud_edge_perm[no_fb_slice_arranged][no_ud_slice_permutations];
unsigned short ud_edge_perm_conjugate[no_ud_edge_permutations][NO_SYMMETRY_MOVES];

//Ph

unsigned char* corner_ori_flip_ud_slice_prune_table = new unsigned char[no_flip_ud_slice * no_corner_orientations];
unsigned char* ud_edge_perm_corner_perm_prune_table = new unsigned char[no_sym_corner_permutations * no_ud_edge_permutations];


utils::ArrayList<FaceTurn> phase2_move_set = {
	Ux1, Ux2, Ux3, Dx1, Dx2, Dx3 ,Rx2, Lx2, Fx2, Bx2
};

inline unsigned short getOrientation(const unsigned char* ori, unsigned int count, unsigned int max_ori) {
	unsigned int result = 0;

	//Go through every piece
	for (unsigned char cubelet = 0; cubelet < count - 1; cubelet++) 
		result = result * max_ori + ori[cubelet];

	return result;
}

unsigned short getCornerOriCoord(const CubieLevel & level) {
	return getOrientation(level.corner_ori(), NO_CORNERS, MAX_CORNER_ORI);
}

unsigned short getEdgeOriCoord(const CubieLevel & level) {
	return getOrientation(level.edge_ori(), NO_EDGES, MAX_EDGE_ORI);
}

inline unsigned int getPerm(const unsigned char* permutations, unsigned char count) {
	unsigned int result = 0;

	//Go through every piece
	for (unsigned char curr_piece = 0, inv = count; curr_piece < count - 1; curr_piece++, inv--) {
		result *= inv;

		//Looping over every piece after the current one
		for (unsigned char next_piece = curr_piece + 1; next_piece < count; next_piece++)
			if (permutations[curr_piece] > permutations[next_piece]) result++;
	}

	return result;
}

unsigned short getCornerPermCoord(const CubieLevel & level) {
	return getPerm((const unsigned char*)level.corner_perm(), NO_CORNERS);
}

unsigned int getEdgePermCoord(const CubieLevel & level) {
	return getPerm((const unsigned char*)level.edge_perm(), NO_EDGES);
}

unsigned short getCombination(const unsigned char* perm, unsigned char count, const unsigned char* pieces, unsigned char pieces_count) {

	unsigned int result = 0;

	char r = pieces_count;
	for (char curr_piece = 0; curr_piece < count && r >= 0; curr_piece++) {

		for (char check_piece = 0; check_piece < pieces_count; check_piece++)
			if (perm[curr_piece] == pieces[check_piece]) 
				result += nCk_table[count - curr_piece - 1][r--];

	}

	return result;
}

unsigned short getCornerCombination(const CubieLevel& level, const Corner* pieces, unsigned char pieces_count) {
	return getCombination((const unsigned char*)level.corner_perm(), NO_CORNERS, (const unsigned char*)pieces, pieces_count);
}

unsigned short getEdgeCombination(const CubieLevel& level, const Edge* pieces, unsigned char pieces_count) {
	return getCombination((const unsigned char*)level.edge_perm(), NO_EDGES, (const unsigned char*)pieces, pieces_count);
}

unsigned short getArranged(const unsigned char* perm, unsigned char count, const unsigned char* pieces, unsigned char pieces_count) {
	utils::ArrayList<unsigned char> pieces_perm(pieces_count);
	utils::ArrayList<unsigned char> temp_perm(pieces_count);

	unsigned int combination = 0;
	unsigned int factorial = 1;

	char r = pieces_count;
	for (char curr_piece = 0; curr_piece < count && r >= 0; curr_piece++) {
		for (char check_piece = 0; check_piece < pieces_count; check_piece++)
			if (perm[curr_piece] == pieces[check_piece]) {
				factorial *= r;

				combination += nCk_table[count - curr_piece - 1][r--];
				unsigned char curr_perm = perm[curr_piece];

				unsigned short greater_than = 0;
				for (unsigned int piece = 0; piece < temp_perm.getSize(); piece++) {
					if (curr_perm > temp_perm[piece]) greater_than++;
					else pieces_perm[piece]++;
				}

				temp_perm.add(curr_perm);
				pieces_perm.add(greater_than);
			}
	}

	unsigned short result = combination * factorial + getPerm(pieces_perm.getData(), pieces_count);

	return result;
}

unsigned short getCornerArranged(const CubieLevel & level, const Corner * pieces, unsigned char pieces_count)
{
	return getArranged((const unsigned char*)level.corner_perm(), NO_CORNERS, (const unsigned char*)pieces, pieces_count);
}

unsigned short getEdgeArranged(const CubieLevel & level, const Edge * pieces, unsigned char pieces_count) {
	return getArranged((const unsigned char*)level.edge_perm(), NO_EDGES, (const unsigned char*)pieces, pieces_count);
}

unsigned short getUDEdgePerm(const CubieLevel & level) {
	return getPerm((unsigned char*)level.edge_perm(), NO_UD_EDGES);
}

inline unsigned int getRawFlipUDSlice(const CubieLevel& cube) {
	return getEdgeCombination(cube, UD_slice_pieces, NO_SLICE_EDGES) * no_edge_orientations + getEdgeOriCoord(cube);
}

inline bool getParity(const unsigned char* perm, unsigned int count) {

	unsigned char total = 0;

	//sum all of the less than values
	for (unsigned char curr_piece = 0; curr_piece < count - 1; curr_piece++) {
		for (unsigned char next_piece = curr_piece + 1; next_piece < count; next_piece++)
			if (perm[curr_piece] > perm[next_piece]) total++;
	}

	//if odd return true else return false
	return total & 1;
}

bool getEdgeParity(const CubieLevel & level)
{
	return getParity((const unsigned char*)level.edge_perm(), NO_EDGES);
}

bool getCornerParity(const CubieLevel & level)
{
	return getParity((const unsigned char*)level.corner_perm(), NO_CORNERS);
}

bool isFlipUDSliceRep(unsigned int coord) {
	int class_index = utils::binarySearch(flip_ud_slice_class_index_to_representative, coord, no_flip_ud_slice);

	return (class_index != -1);
}

/////////////////////////////GET SYM COORDINATES /////////////////////////////////////////////////////////////////////////////

template<class T>
inline unsigned int getSymCoord(const CubieLevel& level, unsigned int* class_to_representative, T (*getCoord)(const CubieLevel&), unsigned int no_coordinates) {
	
	//Check all symmetries
	for (unsigned char symmetry = 0; symmetry < NO_SYMMETRY_MOVES; symmetry++) {
		CubieLevel sym_cube = sym_move_cubes[symmetry];
		sym_cube.multiplyAllCoordinates(level);
		sym_cube.multiplyAllCoordinates(sym_move_cubes[inv_sym_index[symmetry]]);

		int class_index = utils::binarySearch(class_to_representative, (unsigned int)getCoord(sym_cube), no_coordinates);

		//If raw coordinate found to exist in array, it must be a representative
		if (class_index != BINARY_SEARCH_NOT_FOUND)
			return class_index * NO_SYMMETRY_MOVES + symmetry;
	}

	assert(false);
	return NULL;
}

unsigned int getFlipUDSliceCoord(const CubieLevel & level) {
	return getSymCoord(level, flip_ud_slice_class_index_to_representative, getRawFlipUDSlice, no_flip_ud_slice);
}

unsigned int getSymCornerPermCoord(const CubieLevel& cube) {
	return getSymCoord(cube, corner_perm_class_index_to_representative, getCornerPermCoord, no_sym_corner_permutations);
}

unsigned int sym_table_move(unsigned int coord, unsigned int * move_table, unsigned char move)
{
	unsigned char symmetry = coord % NO_SYMMETRY_MOVES;
	unsigned int coord_class = coord / NO_SYMMETRY_MOVES;

	unsigned int conjugate_move = apply_sym_move[symmetry][move];

	unsigned int moved_coord = move_table[coord_class * NO_MOVES + conjugate_move];
	unsigned char move_symmetry = moved_coord % NO_SYMMETRY_MOVES;
	unsigned int move_coord_class = moved_coord / NO_SYMMETRY_MOVES;

	unsigned char new_symmetry = sym_multiply[move_symmetry][symmetry];

	return move_coord_class * NO_SYMMETRY_MOVES + new_symmetry;
}

inline void setRawFlipUDSlice(CubieLevel& cube, unsigned int coord) {
	cube.setEdgeCombination(coord / no_edge_orientations, UD_slice_pieces, NO_SLICE_EDGES);
	cube.setEdgeOriCoord(coord % no_edge_orientations);
}

void generateFlipUDSliceClassIndexToRepresentative() {
	if (doesFileExist("res/flipUDClassIndexToRep.dat")) {
		utils::String data = readFromFile("res/flipUDClassIndexToRep.dat");
		unsigned int* data_stream = (unsigned int*)((unsigned char*)data.getData());

		for (unsigned int coord = 0; coord < no_flip_ud_slice; coord++)
			flip_ud_slice_class_index_to_representative[coord] = data_stream[coord];

	} else {
		bool* occupied = new bool[no_raw_flip_ud_slice];

		for (unsigned int i = 0; i < no_raw_flip_ud_slice; i++)
			occupied[i] = 0;

		unsigned short class_index = 0;
		for (unsigned int coord = 0; coord < no_raw_flip_ud_slice; coord++) {
			if (!occupied[coord]) {
				CubieLevel cube;

				setRawFlipUDSlice(cube, coord);

				flip_ud_slice_class_index_to_representative[class_index++] = coord;

				for (unsigned char symmetry = 0; symmetry < NO_SYMMETRY_MOVES; symmetry++) {
					CubieLevel inv = sym_move_cubes[inv_sym_index[symmetry]];
					inv.multiplyAllCoordinates(cube);
					inv.multiplyAllCoordinates(sym_move_cubes[symmetry]);

					occupied[getRawFlipUDSlice(inv)] = true;
				}
			}
		}

		delete[] occupied;

		dumpToFile("res/flipUDClassIndexToRep.dat", (char*)flip_ud_slice_class_index_to_representative, sizeof(flip_ud_slice_class_index_to_representative));
	}
}

void generateFlipUDSliceMoveTable() {
	if (doesFileExist("res/flipUDMoveTable.dat")) {
		utils::String data = readFromFile("res/flipUDMoveTable.dat");
		unsigned int* data_stream = (unsigned int*)((unsigned char*)data.getData());

		unsigned int data_index = 0;
		for (unsigned int coord = 0; coord < no_flip_ud_slice; coord++)
			for (unsigned int move = 0; move < NO_MOVES; move++, data_index++)
				flip_ud_slice_move_table[coord][move] = data_stream[data_index];


	}
	else {
		for (unsigned int class_index = 0; class_index < no_flip_ud_slice; class_index++) {
			CubieLevel cube;

			unsigned int raw_coord = flip_ud_slice_class_index_to_representative[class_index];

			setRawFlipUDSlice(cube, raw_coord);

			for (unsigned char move = 0; move < NO_UNIQUE_MOVES; move++) {

				for (unsigned char repeat = 0; repeat < REPEATS_TO_IDENTITY; repeat++) {
					cube.multiplyAllCoordinates(turn_cubes[move]);

					if (repeat < REPEATS_TO_IDENTITY - 1)
						flip_ud_slice_move_table[class_index][repeat * NO_UNIQUE_MOVES + move] = getFlipUDSliceCoord(cube);
				}

			}
		}

		dumpToFile("res/flipUDMoveTable.dat", (char*)flip_ud_slice_move_table, sizeof(flip_ud_slice_move_table));
	}

}

void generateCornerPermClassIndexToRepresentative() {
	if (doesFileExist("res/cornerPermClassIndexToRep.dat")) {
		utils::String data = readFromFile("res/cornerPermClassIndexToRep.dat");
		unsigned int* data_stream = (unsigned int*)((unsigned char*)data.getData());

		for (unsigned int coord = 0; coord < no_sym_corner_permutations; coord++)
			corner_perm_class_index_to_representative[coord] = data_stream[coord];

	} else {

		bool* occupied = new bool[no_raw_corner_permutations];

		for (unsigned int i = 0; i < no_raw_corner_permutations; i++)
			occupied[i] = 0;

		unsigned short class_index = 0;
		for (unsigned short coord = 0; coord < no_raw_corner_permutations; coord++) {
			if (!occupied[coord]) {
				CubieLevel cube;

				cube.setCornerPermCoord(coord);

				corner_perm_class_index_to_representative[class_index++] = coord;

				for (unsigned char symmetry = 0; symmetry < NO_SYMMETRY_MOVES; symmetry++) {
					CubieLevel inv = sym_move_cubes[inv_sym_index[symmetry]];
					inv.multiplyAllCoordinates(cube);
					inv.multiplyAllCoordinates(sym_move_cubes[symmetry]);

					occupied[getCornerPermCoord(inv)] = true;
				}
			}
		}

		delete[] occupied;

		dumpToFile("res/cornerPermClassIndexToRep.dat", (char*)corner_perm_class_index_to_representative, sizeof(corner_perm_class_index_to_representative));
	}
}

void generateCornerPermMoveTable() {
	if (doesFileExist("res/cornerPermMoveTable.dat")) {
		utils::String data = readFromFile("res/cornerPermMoveTable.dat");
		unsigned int* data_stream = (unsigned int*)((unsigned char*)data.getData());

		unsigned int data_index = 0;
		for (unsigned short coord = 0; coord < no_sym_corner_permutations; coord++)
			for (unsigned int move = 0; move < NO_MOVES; move++, data_index++)
				corner_perm_move_table[coord][move] = data_stream[data_index];


	} else {
		//For every class index
		for (unsigned short class_index = 0; class_index < no_sym_corner_permutations; class_index++) {
			CubieLevel cube;

			//calculate the raw coordinate
			unsigned short raw_coord = corner_perm_class_index_to_representative[class_index];
			
			cube.setCornerPermCoord(raw_coord);

			//Apply all the moves
			for (unsigned char move = 0; move < NO_UNIQUE_MOVES; move++) {

				for (unsigned char repeat = 0; repeat < REPEATS_TO_IDENTITY; repeat++) {
					cube.multiplyAllCoordinates(turn_cubes[move]);

					//Store new coordinate in table
					if (repeat < REPEATS_TO_IDENTITY - 1)
						corner_perm_move_table[class_index][repeat * NO_UNIQUE_MOVES + move] = getSymCornerPermCoord(cube);
				}

			}
		}

		dumpToFile("res/cornerPermMoveTable.dat", (char*)corner_perm_move_table, sizeof(corner_perm_move_table));
	}

}

void generateCornerOriMoveTable() {

	//go through every corner orientation coordinate
	for (unsigned short coord = 0; coord < no_corner_orientations; coord++) {
		CubieLevel cube;
		
		cube.setCornerOriCoord(coord);
			
		//Go through every move
		for (unsigned char move = 0; move < NO_UNIQUE_MOVES; move++) {

			for (unsigned char repeat = 0; repeat < REPEATS_TO_IDENTITY; repeat++) {
				cube.multiplyCornerOri(turn_cubes[move]);

				if (repeat < REPEATS_TO_IDENTITY - 1)
					corner_ori_move_table[coord][repeat * NO_UNIQUE_MOVES + move] = getCornerOriCoord(cube);
			}

		}
	}
}

void generateUDEdgePermMoveTable() {

	//go through every corner orientation coordinate
	for (unsigned short coord = 0; coord < no_ud_edge_permutations; coord++) {
		//Go through every move
		for (FaceTurn& move : phase2_move_set) {
			CubieLevel cube;
			cube.setUDEdgePerm(coord);

			cube.turnFace(move);

			ud_edge_perm_move_table[coord][move] = getUDEdgePerm(cube);
		}
	}
}

void generateCornerOriConjugate() {
	for (unsigned int coord = 0; coord < no_corner_orientations; coord++) {
		CubieLevel cube;

		cube.setCornerOriCoord(coord);

		for (unsigned char symmetry = 0; symmetry < NO_SYMMETRY_MOVES; symmetry++) {
			CubieLevel sym_cube = sym_move_cubes[symmetry];
			sym_cube.multiplyAllCoordinates(cube);
			sym_cube.multiplyAllCoordinates(sym_move_cubes[inv_sym_index[symmetry]]);

			corner_ori_conjugate[coord][symmetry] = getCornerOriCoord(sym_cube);
		}
	}
}

void generateUDEdgePermConjugate() {
	for (unsigned int coord = 0; coord < no_ud_edge_permutations; coord++) {
		CubieLevel cube;

		cube.setUDEdgePerm(coord);

		for (unsigned char symmetry = 0; symmetry < NO_SYMMETRY_MOVES; symmetry++) {
			CubieLevel sym_cube = sym_move_cubes[symmetry];
			sym_cube.multiplyAllCoordinates(cube);
			sym_cube.multiplyAllCoordinates(sym_move_cubes[inv_sym_index[symmetry]]);

			ud_edge_perm_conjugate[coord][symmetry] = getUDEdgePerm(sym_cube);
		}
	}
}

inline void generateEdgeSliceArrangedMoveTable(unsigned short* result, const Edge slice_pieces[NO_SLICE_EDGES], unsigned int no_coordinates) {
	for (unsigned short coord = 0; coord < no_coordinates; coord++) {

		CubieLevel cube;

		cube.setArrangedEdgeCoord(coord, slice_pieces, NO_SLICE_EDGES);

		for (unsigned char move = 0; move < NO_UNIQUE_MOVES; move++) {

			for (unsigned char repeat = 0; repeat < REPEATS_TO_IDENTITY; repeat++) {
				cube.multiplyEdgePerm(turn_cubes[move]);

				if (repeat < REPEATS_TO_IDENTITY - 1)
					result[coord * NO_MOVES + (repeat * NO_UNIQUE_MOVES + move)] = getEdgeArranged(cube, slice_pieces, NO_SLICE_EDGES);
			}

		}
	}
}

void generateUDSliceArrangedMoveTable() {
	generateEdgeSliceArrangedMoveTable((unsigned short*)ud_slice_arranged_move_table, UD_slice_pieces, no_ud_slice_arranged);
}

void generateFBSliceArrangedMoveTable() {
	generateEdgeSliceArrangedMoveTable((unsigned short*)fb_slice_arranged_move_table, FB_slice_pieces, no_fb_slice_arranged);
}

void generateRLSliceArrangedMoveTable() {
	generateEdgeSliceArrangedMoveTable((unsigned short*)rl_slice_arranged_move_table, RL_slice_pieces, no_rl_slice_arranged);
}

void generateGetUDEdgePermTable() {
	if (doesFileExist("res/getUDEdgePermTable.dat")) {
		utils::String data = readFromFile("res/getUDEdgePermTable.dat");
		unsigned int* data_stream = (unsigned int*)((unsigned char*)data.getData());

		unsigned int index = 0;
		for (unsigned int rl_slice_arranged = 0; rl_slice_arranged < no_rl_slice_arranged; rl_slice_arranged++)
			for (unsigned char ud_slice_perm = 0; ud_slice_perm < no_ud_slice_permutations; ud_slice_perm++)
				get_ud_edge_perm[rl_slice_arranged][ud_slice_perm] = data_stream[index++];
	}
	else {

		CubieLevel cube;

		//Go through every cooridinate
		for (unsigned short ud_edge_perm = 0; ud_edge_perm < no_ud_edge_permutations; ud_edge_perm++) {
			cube.setUDEdgePerm(ud_edge_perm);

			//Find the respective slice coordinates at this ud_edge_perm coordinate
			unsigned int rl_slice_coord = getEdgeArranged(cube, RL_slice_pieces, NO_SLICE_EDGES);
			unsigned int fb_slice_coord = getEdgeArranged(cube, FB_slice_pieces, NO_SLICE_EDGES);

			//only the permutation of the fb_slice_coord is needed.
			get_ud_edge_perm[rl_slice_coord][fb_slice_coord % no_ud_slice_permutations] = ud_edge_perm;
		}

		dumpToFile("res/getUDEdgePermTable.dat", (char*)get_ud_edge_perm, sizeof(get_ud_edge_perm));
	}

}

void setPruning(unsigned char* table, unsigned int index, unsigned char value) {
	table[index] = value;
}

unsigned char getPruning(unsigned char* table, unsigned int index) {
	return table[index];
}

void generateCornerOriFlipUDSlicePruningTable() {

	unsigned int no_coordinates = no_flip_ud_slice * no_corner_orientations;

	if (doesFileExist("res/CornerOriflipUDPruningTable.dat")) {
		utils::String data = readFromFile("res/CornerOriflipUDPruningTable.dat");
		unsigned char* data_stream = (unsigned char*)data.getData();

		for (unsigned int coord = 0; coord < no_coordinates; coord++)
			corner_ori_flip_ud_slice_prune_table[coord] = data_stream[coord];

	} else {

		for (unsigned int coord = 0; coord < no_coordinates; coord++)
			corner_ori_flip_ud_slice_prune_table[coord] = PRUNING_EMPTY;

		unsigned char depth = 0;
		setPruning(corner_ori_flip_ud_slice_prune_table, 0, depth);
		unsigned int values_filled = 1;

		unsigned short* sym_state = new unsigned short[no_flip_ud_slice];

		for (unsigned int i = 0; i < no_flip_ud_slice; i++)
			sym_state[i] = 0;

		for (unsigned int i = 0; i < no_flip_ud_slice; i++) {
			CubieLevel cube;
			setRawFlipUDSlice(cube, flip_ud_slice_class_index_to_representative[i]);

			for (unsigned int j = 0; j < NO_SYMMETRY_MOVES; j++) {
				CubieLevel inv = sym_move_cubes[inv_sym_index[j]];
				inv.multiplyAllCoordinates(cube);
				inv.multiplyAllCoordinates(sym_move_cubes[j]);

				int class_index = utils::binarySearch(flip_ud_slice_class_index_to_representative, getRawFlipUDSlice(inv), no_flip_ud_slice);

				if (class_index != -1)
					sym_state[i] = sym_state[i] | (1 << j);

			}
		}

		unsigned int previous_filled = 0;

		while (values_filled != no_coordinates) {

			std::cout << "depth: " << (int)depth << ", size: " << (values_filled - previous_filled) << std::endl;
			previous_filled = values_filled;

			for (unsigned int coord = 0; coord < no_coordinates; coord++) {
				if (getPruning(corner_ori_flip_ud_slice_prune_table, coord) == depth) {
					unsigned int flip_ud_slice = coord / no_corner_orientations;
					unsigned int corner_ori = coord % no_corner_orientations;

					for (unsigned char move = 0; move < NO_MOVES; move++) {
						unsigned int new_flip_ud_slice = flip_ud_slice_move_table[flip_ud_slice][move];

						unsigned int new_corner_ori = corner_ori_move_table[corner_ori][move];

						unsigned int class_index = new_flip_ud_slice / NO_SYMMETRY_MOVES;
						unsigned int test_sym = new_flip_ud_slice % NO_SYMMETRY_MOVES;

						unsigned int corner_conjugate = corner_ori_conjugate[new_corner_ori][test_sym];

						unsigned int sym = sym_state[class_index];

						for (unsigned char symmetry = 0; symmetry < NO_SYMMETRY_MOVES; symmetry++) {
							if ((sym >> symmetry) & 1) {
								unsigned int new_coord = class_index * no_corner_orientations + corner_ori_conjugate[corner_conjugate][symmetry];

								if (getPruning(corner_ori_flip_ud_slice_prune_table, new_coord) == PRUNING_EMPTY) {
									setPruning(corner_ori_flip_ud_slice_prune_table, new_coord, depth + 1);
									values_filled++;
								}
							}
						}
					}
				}
			}
			depth++;
		}

		delete[] sym_state;

		dumpToFile("res/CornerOriflipUDPruningTable.dat", (char*)corner_ori_flip_ud_slice_prune_table, sizeof(corner_ori_flip_ud_slice_prune_table));
	}
}

void generateUDEdgePermCornerPermPruningTable() {

	unsigned int no_coordinates = no_ud_edge_permutations * no_sym_corner_permutations;

	if (doesFileExist("res/UDEdgePermCornerPermPruningTable.dat")) {
		utils::String data = readFromFile("res/UDEdgePermCornerPermPruningTable.dat");
		unsigned char* data_stream = (unsigned char*)data.getData();

		for (unsigned int coord = 0; coord < no_coordinates; coord++)
			ud_edge_perm_corner_perm_prune_table[coord] = data_stream[coord];
	}
	else {

		//Set all pruning values to empty 
		for (unsigned int coord = 0; coord < no_coordinates; coord++)
			ud_edge_perm_corner_perm_prune_table[coord] = 0xFF;


		// solved state of the cube has depth 0, set that to the pruning table
		unsigned char depth = 0;
		setPruning(ud_edge_perm_corner_perm_prune_table, 0, depth);
		unsigned int values_filled = 1;

		unsigned short sym_state[no_sym_corner_permutations];

		//Calculate multiple symmetry representations
		for (unsigned int i = 0; i < no_sym_corner_permutations; i++)
			sym_state[i] = 0;

		for (unsigned int i = 0; i < no_sym_corner_permutations; i++) {
			CubieLevel cube;
			cube.setCornerPermCoord(corner_perm_class_index_to_representative[i]);

			for (unsigned int j = 0; j < NO_SYMMETRY_MOVES; j++) {
				CubieLevel inv = sym_move_cubes[inv_sym_index[j]];
				inv.multiplyAllCoordinates(cube);
				inv.multiplyAllCoordinates(sym_move_cubes[j]);

				int class_index = utils::binarySearch(corner_perm_class_index_to_representative, (unsigned int)getCornerPermCoord(inv), no_sym_corner_permutations);

				if (class_index != -1)
					sym_state[i] = sym_state[i] | (1 << j);

			}
		}

		unsigned int previous_filled = 0;
		
		//until table has been filled out, continue looping
		while (values_filled != no_coordinates) {

			std::cout << "depth: " << (int)depth << ", size: " << (values_filled - previous_filled) << std::endl;
			previous_filled = values_filled;

			for (unsigned int coord = 0; coord < no_coordinates; coord++) {
				if (getPruning(ud_edge_perm_corner_perm_prune_table, coord) == depth) {
					unsigned int corner_perm = coord % no_sym_corner_permutations;
					unsigned int ud_edge_perm = coord / no_sym_corner_permutations;

					//Apply all phase 2 turns
					for(FaceTurn& move : phase2_move_set) {
						unsigned int new_corner_perm = corner_perm_move_table[corner_perm][move];

						unsigned int new_ud_edge_perm = ud_edge_perm_move_table[ud_edge_perm][move];

						unsigned int class_index = new_corner_perm / NO_SYMMETRY_MOVES;
						unsigned int test_sym = new_corner_perm % NO_SYMMETRY_MOVES;

						unsigned int ud_edge_conjugate = ud_edge_perm_conjugate[new_ud_edge_perm][test_sym];

						//Apply symmetries and caluclate their positions
						unsigned int sym = sym_state[class_index];
						for (unsigned char symmetry = 0; symmetry < NO_SYMMETRY_MOVES; symmetry++) {
							if ((sym >> symmetry) & 1) {
								unsigned int new_coord = class_index + no_sym_corner_permutations * ud_edge_perm_conjugate[ud_edge_conjugate][symmetry];

								//If the values has not already been filled, set it to the current depth
								if (getPruning(ud_edge_perm_corner_perm_prune_table, new_coord) == PRUNING_EMPTY) {
									setPruning(ud_edge_perm_corner_perm_prune_table, new_coord, depth + 1);
									values_filled++;
								}
							}
						}
					}
				}
			}
			depth++;
		}

		dumpToFile("res/UDEdgePermCornerPermPruningTable.dat", (char*)ud_edge_perm_corner_perm_prune_table, sizeof(ud_edge_perm_corner_perm_prune_table));
	}
}

void loadTransitionTables()
{
	generateCornerOriMoveTable();
	
	generateUDSliceArrangedMoveTable();

	generateFlipUDSliceClassIndexToRepresentative();
	generateFlipUDSliceMoveTable();

	generateCornerOriConjugate();
	generateCornerOriFlipUDSlicePruningTable();

	generateCornerPermClassIndexToRepresentative();
	
	generateCornerPermMoveTable();
	generateUDEdgePermConjugate();
	generateUDEdgePermMoveTable();

	generateUDEdgePermCornerPermPruningTable();

	generateUDSliceArrangedMoveTable();
	generateRLSliceArrangedMoveTable();
	generateFBSliceArrangedMoveTable();

	generateGetUDEdgePermTable();

}