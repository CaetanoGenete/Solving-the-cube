#include "Scrambler.h"
#include <random>

#define MAX_SCRAMBLE_DEPTH 21

std::random_device random_engine;

const std::uniform_int_distribution<int> rand_edge_perm_gen(0, no_edge_permutations);
const std::uniform_int_distribution<int> rand_edge_ori_gen(0, no_edge_orientations);
const std::uniform_int_distribution<int> rand_corner_perm_gen(0, no_raw_corner_permutations);
const std::uniform_int_distribution<int> rand_corner_ori_gen(0, no_corner_orientations);

void scrambleCube( CubieLevel & cube) {

	cube.setCornerOriCoord(rand_corner_ori_gen(random_engine));
	cube.setEdgeOriCoord(rand_edge_ori_gen(random_engine));

	cube.setEdgePermCoord(rand_edge_perm_gen(random_engine));
	do cube.setCornerPermCoord(rand_corner_perm_gen(random_engine));
	while (getEdgeParity(cube) != getCornerParity(cube));
}

utils::ArrayList<unsigned char> getScrambleString(CubieLevel & cube)
{
	//Solve the cube;
	utils::ArrayList<unsigned char> result;
	result = search(cube, MAX_SCRAMBLE_DEPTH);

	utils::ArrayList<unsigned char> scramble;

	//flip the result and invert each move;
	for (int move = result.getSize() - 1; move >= 0; move--)
		scramble.add(getInverseMove(result[move]));

	return scramble;
}

utils::String scrambleStringToString(const utils::ArrayList<unsigned char>& scrambleString){

	utils::String result;

	//Convert every move to a string
	for (unsigned int move = 0; move < scrambleString.getSize(); move++)
		result += move_to_string[scrambleString[move]];

	return result;
}
