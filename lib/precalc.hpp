#pragma once

#include "types.hpp"
#include "constants.hpp"
#include "lsx.hpp"

uint8_t GField_mul(uint8_t a, uint8_t b);

void get_constants();

Key F(Block in_key_1, Block in_key_2, Block iter_const);

void expand_key(Block key_1, Block key_2);

void fill_precalc();
