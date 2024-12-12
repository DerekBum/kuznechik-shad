#include "lsx_reverse.hpp"
#include "precalc.hpp"

using namespace std;

Block reverse_S(Block input_data) {
    auto data = toByteArray(input_data);
    for (int i = 0; i < 16; i++)
        data.data[i] = reverse_pi[data.data[i]];
    return fromByteArray(data);
}

Block reverse_R(Block input_data) {
    ByteArray res, data = toByteArray(input_data);
    uint8_t a_0 = data.data[15];
    for (int i = 1; i < 16; i++) {
        res.data[i] = data.data[i - 1];
        a_0 ^= GField_mul(res.data[i], lin_vec[i]);
    }
    res.data[0] = a_0;
    return fromByteArray(res);
}

Block reverse_L(Block input_data) {
    for (int i = 0; i < 16; i++)
        input_data = reverse_R(input_data);
    return input_data;
}
