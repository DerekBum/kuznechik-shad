#include <iostream>
#include <cstdint>
#include <chrono>
#include "lsx.cpp"

using namespace std;

ByteArray key_1_vector = {
    0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11, 0x00,
    0xff, 0xee, 0xdd, 0xcc, 0xbb, 0xaa, 0x99, 0x88,
};
ByteArray key_2_vector = {
    0xef, 0xcd, 0xab, 0x89, 0x67, 0x45, 0x23, 0x01,
    0x10, 0x32, 0x54, 0x76, 0x98, 0xba, 0xdc, 0xfe,
};
ByteArray block_vector = {
    0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF,
    0x00, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11,
};

auto key_1 = fromByteArray(key_1_vector);
auto key_2 = fromByteArray(key_2_vector);
auto block = fromByteArray(block_vector);

int main() {
    fill_precalc();
    Expand_Key(key_1, key_2);

    auto encrypted = Encrypt(block);
    for (auto el : toByteArray(encrypted).data) {
        cout << hex << int(el) << " ";
    }
    cout << endl;

    auto start = chrono::high_resolution_clock::now();
    for (int _ = 0; _ * 16 < 100 * 1024 * 1024; _++) {
        auto encrypted2 = Encrypt(block);

        if (encrypted != encrypted2) {
            return 1;
        }
    }
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);

    cout << "Time of encoding: " << dec << duration.count() << "ms" << endl;
    return 0;
}
