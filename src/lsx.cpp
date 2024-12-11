#include "lsx.hpp"
#include "constants.hpp"
#include <iostream>

using namespace std;

Block L(Block input_data) {
    Block res;
    auto bytes_data = toByteArray(input_data);
    for (int i = 0; i < 16; i++) {
        res ^= precalc[bytes_data.data[i]][15 - i];
    }

    return res;
}

Block LS(Block input_data) {
    Block res;
    auto bytes_data = toByteArray(input_data);
    for (int i = 0; i < 16; i++) {
        res ^= precalc[pi[bytes_data.data[i]]][15 - i];
    }

    return res;
}
