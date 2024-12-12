#include <iostream>
#include <iomanip>
#include <cstdint>
#include <chrono>
#include "precalc.hpp"
#include "types.hpp"
#include "encrypt.hpp"
#include "decrypt.hpp"

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
    expand_key(key_1, key_2);

    auto encrypted = encrypt(block);
    for (auto el : toByteArray(encrypted).data) {
        cout << hex << int(el) << " ";
    }
    cout << endl;

    if (block != decrypt(encrypted)) {
        return 1;
    }

    long long volume_sum = 0, time_sum = 0;

    for (int t = 0; t < 100; t++) {
        int vol = 0;

        auto start = chrono::high_resolution_clock::now();
        for (vol = 0; vol * 16 < 100 * 1024 * 1024; vol++) {
            auto encrypted2 = encrypt(block);

            if (encrypted != encrypted2) {
                return 1;
            }
        }
        auto stop = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);

        cout << "Time of encoding: " << dec << duration.count() << "ms" << endl;

        volume_sum += vol * 16;
        time_sum += duration.count();
    }

    cout << fixed;
    cout << "Average speed of encoding: " << setprecision(5) << volume_sum * 1000.0 / 1024.0 / 1024.0 / time_sum << " Mb/s" << endl;
    return 0;
}
