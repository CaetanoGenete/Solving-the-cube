#pragma once

#include "CUtils.h"
#include "CubieLevel.h"

extern const utils::String move_to_string[NO_MOVES];

unsigned char getInverseMove(unsigned char move);

utils::ArrayList<unsigned char> search(const CubieLevel & level, unsigned int max_depth);
