#include "constants.h"
#include <iostream>
#include <algorithm>

using namespace std;

vector <unsigned char> X(vector <unsigned char> &a, vector <unsigned char> &b) {
    vector <unsigned char> c(block_size);
    for (int i = 0; i < block_size; i++)
        c[i] = char(a[i] ^ b[i]);
    return c;
}

void S(vector <unsigned char> &input_data) {
    for (int i = 0; i < block_size; i++) {
        input_data[i] = pi[input_data[i]];
    }
}

static unsigned char GField_mul(unsigned char a, unsigned char b) {
    unsigned char c = 0;
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

static vector <unsigned char> R(vector <unsigned char> &state) {
    unsigned char a_15 = 0;
    vector <unsigned char> internal(16);
    for (int i = 15; i >= 0; i--) {
    	if (i == 0)
    		internal[15] = state[i];
    	else
    		internal[i - 1] = state[i];

	    a_15 ^= GField_mul(state[i], lin_vec[i]);
    }
    internal[15] = a_15;
    return internal;
}

vector <unsigned char> L(vector <unsigned char> &input_data) {
    vector <unsigned char> res(16);
    for (int i = 0; i < 16; i++) {
        res = X(res, precalc[input_data[i]][15 - i]);
    }
    return res;
}

vector <unsigned char> LS(vector <unsigned char> &input_data) {
    vector <unsigned char> res(16);
    for (int i = 0; i < 16; i++) {
        res = X(res, precalc[pi[input_data[i]]][15 - i]);
    }
    return res;
}

static void get_constants() {
    vector <vector <unsigned char>> iter_num(32, vector <unsigned char>(16));
    for (int i = 0; i < 32; i++) {
    	for (int j = 0; j < block_size; j++)
    		iter_num[i][j] = 0;
        iter_num[i][0] = i + 1;
    }
    for (int i = 0; i < 32; i++) {
        iter_C[i] = L(iter_num[i]);
    }
}

vector <vector <unsigned char>> F(vector <unsigned char> &in_key_1, vector <unsigned char> &in_key_2, vector <unsigned char> &iter_const) {
    vector <unsigned char> internal;
    internal = X(in_key_1, iter_const);
    internal = LS(internal);
    vector <unsigned char> out_key_1 = X(internal, in_key_2);
    vector <vector <unsigned char>> key = {out_key_1, in_key_1};
    return key;
}

void Expand_Key(vector <unsigned char> &key_1, vector <unsigned char> &key_2) {
    get_constants();
    iter_key[0] = key_1;
    iter_key[1] = key_2;
    vector <vector <unsigned char>> iter12 = {key_1, key_2}, iter34(2);
    iter12[0] = key_1;
    iter12[1] = key_2;
    for (int i = 0; i < 4; i++)
    {
        iter34 = F(iter12[0], iter12[1], iter_C[0 + 8 * i]);
        iter12 = F(iter34[0], iter34[1], iter_C[1 + 8 * i]);
        iter34 = F(iter12[0], iter12[1], iter_C[2 + 8 * i]);
        iter12 = F(iter34[0], iter34[1], iter_C[3 + 8 * i]);
        iter34 = F(iter12[0], iter12[1], iter_C[4 + 8 * i]);
        iter12 = F(iter34[0], iter34[1], iter_C[5 + 8 * i]);
        iter34 = F(iter12[0], iter12[1], iter_C[6 + 8 * i]);
        iter12 = F(iter34[0], iter34[1], iter_C[7 + 8 * i]);
        
        iter_key[2 * i + 2] = iter12[0];
        iter_key[2 * i + 3] = iter12[1];
    }
}

vector <unsigned char> Encrypt(vector <unsigned char> &block) {
    vector <unsigned char> out_block = block;
    for (int i = 0; i < 9; i++) {
    	out_block = X(iter_key[i], out_block);
    	out_block = LS(out_block);
    }
    out_block = X(out_block, iter_key[9]);

    return out_block;
}

void fill_precalc() {
    for (int n = 0; n < 256; n++) {
        for (int i = 0; i < 16; i++) {
            for (int j = 0; j < 16; j++) {
                precalc[n][i][j] = GField_mul(n, LMatrix[i][j]);
            }
            reverse(precalc[n][i].begin(), precalc[n][i].end());
        }
    }
}
