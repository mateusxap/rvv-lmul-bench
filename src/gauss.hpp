#pragma once

#include "defines.hpp"
#include <cmath>

float* create1DFilter(int n, float sigma);
#ifdef RVVMF_RVV_0P7
	#include <riscv-vector.h>
	void first_block_rvv(float* destData, float* srcData, size_t w, size_t h, float sigma, int sizeFilter);
	void second_block_rvv(float* destData, float* srcData, size_t w, size_t h, float sigma, int sizeFilter);
#elif defined RVVMF_RVV_1P0
	#include <riscv_vector.h>
	void first_block_rvv(float* destData, float* srcData, size_t w, size_t h, float sigma, int sizeFilter);
	void second_block_rvv(float* destData, float* srcData, size_t w, size_t h, float sigma, int sizeFilter);
#else
	void first_block(float* destData, float* srcData, size_t w, size_t h, float sigma, int sizeFilter);
	void second_block(float* destData, float* srcData, size_t w, size_t h, float sigma, int sizeFilter);
#endif