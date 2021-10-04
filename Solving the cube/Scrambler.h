#pragma once

#include "CubieLevel.h"
#include "Solver.h"

#include "CoordinateLevel.h"

void scrambleCube(CubieLevel& cube);

utils::ArrayList<unsigned char> getScrambleString(CubieLevel& cube);
utils::String scrambleStringToString(const utils::ArrayList<unsigned char>& scrambleString);