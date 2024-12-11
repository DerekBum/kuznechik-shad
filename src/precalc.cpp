#include "precalc.hpp"
#include <algorithm>

using namespace std;

Block iter_C[32];
Block iter_key[10];
std::vector <std::vector <std::vector <unsigned char>>> precalc_vector(256, std::vector <std::vector <unsigned char>>(16, std::vector <unsigned char>(16)));
Block precalc[256][16];

uint8_t GField_mul(uint8_t a, uint8_t b) {
    uint8_t c = 0;
    for (int i = 0; i < 8; i++) {
        if ((b & 1) == 1)
        	c ^= a;
        int hi_bit = (a & 0x80);
        a <<= 1;
        if (hi_bit > 0)
        	a ^= 0xc3;
        b >>= 1;
    }	    
	return c;
}

void get_constants() {
    ByteArray iter_num[32];
    for (int i = 0; i < 32; i++) {
    	for (int j = 0; j < block_size; j++)
    		iter_num[i].data[j] = 0;
        iter_num[i].data[0] = i + 1;
    }
    for (int i = 0; i < 32; i++) {
        iter_C[i] = L(fromByteArray(iter_num[i]));
    }
}

Key F(Block in_key_1, Block in_key_2, Block iter_const) {
    Block internal = in_key_1 ^ iter_const;
    internal = LS(internal);
    Block out_key_1 = internal ^ in_key_2;
    return {out_key_1, in_key_1};
}

void expand_key(Block key_1, Block key_2) {
    get_constants();
    iter_key[0] = key_1;
    iter_key[1] = key_2;
    Key iter12 = {key_1, key_2}, iter34;
    for (int i = 0; i < 4; i++)
    {
        iter34 = F(iter12.first, iter12.second, iter_C[0 + 8 * i]);
        iter12 = F(iter34.first, iter34.second, iter_C[1 + 8 * i]);
        iter34 = F(iter12.first, iter12.second, iter_C[2 + 8 * i]);
        iter12 = F(iter34.first, iter34.second, iter_C[3 + 8 * i]);
        iter34 = F(iter12.first, iter12.second, iter_C[4 + 8 * i]);
        iter12 = F(iter34.first, iter34.second, iter_C[5 + 8 * i]);
        iter34 = F(iter12.first, iter12.second, iter_C[6 + 8 * i]);
        iter12 = F(iter34.first, iter34.second, iter_C[7 + 8 * i]);
        
        iter_key[2 * i + 2] = iter12.first;
        iter_key[2 * i + 3] = iter12.second;
    }
}

void fill_precalc() {
    for (int n = 0; n < 256; n++) {
        for (int i = 0; i < 16; i++) {
            for (int j = 0; j < 16; j++) {
                precalc_vector[n][i][j] = GField_mul(n, LMatrix[i][j]);
            }
            reverse(precalc_vector[n][i].begin(), precalc_vector[n][i].end());
            
            ByteArray precalc_array;
            for (int j = 0; j < 16; j++) {
                precalc_array.data[j] = precalc_vector[n][i][j];
            }
            precalc[n][i] = fromByteArray(precalc_array);
        }
    }
}
