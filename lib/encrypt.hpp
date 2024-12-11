#pragma once

#include "types.hpp"
#include "lsx.hpp"
#include "constants.hpp"

Block encrypt(Block block) {
    for (int i = 0; i < 9; i++) {
    	block ^= iter_key[i];
    	block = LS(block);
    }
    block ^= iter_key[9];

    return block;
}
