#pragma once

#include "defines.hpp"
#include <cmath>
#ifdef RVVMF_RVV_0P7
	#include <riscv-vector.h>
	float l2Norm_mN(float* v, size_t n);
#elif defined RVVMF_RVV_1P0
	#include <riscv_vector.h>
	float l2Norm_mN(float* v, size_t n);
#else
	float l2Norm(float* v, std::size_t n);
#endif




