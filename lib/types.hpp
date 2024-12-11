#pragma once

#include <iostream>
#include <bit>
#include <cstdint>

typedef __int128_t Block;
typedef std::pair <Block, Block> Key;

struct ByteArray {
    uint8_t data[16];
};

inline ByteArray toByteArray(Block input) {
    auto result = std::bit_cast<ByteArray>(input);
    return result;
}

inline Block fromByteArray(ByteArray input) {
    return std::bit_cast<Block>(input);
}
