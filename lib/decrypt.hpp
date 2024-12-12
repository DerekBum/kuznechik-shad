#pragma once

#include "types.hpp"
#include "lsx_reverse.hpp"
#include "constants.hpp"

Block decrypt(Block block) {
    block ^= iter_key[9];
    for (int i = 8; i >= 0; i--) {
        block = reverse_L(block);
        block = reverse_S(block);
        block ^= iter_key[i];
    }
    return block;
}
