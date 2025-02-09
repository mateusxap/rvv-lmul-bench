#pragma once

#define JOIN2_X(x, y) x ## y
#define JOIN2(x, y) JOIN2_X(x, y)

#ifdef RVV_MLEN
	#define N RVV_MLEN
#endif

#ifdef RVVMF_RVV_0P7
	#define l2Norm_mN JOIN2(l2Norm_m, N)
	#define vfloat32mN_t JOIN2(JOIN2(vfloat32m, N), _t)
	#define vsetvlmax_e32mN JOIN2(vsetvlmax_e32m, N)
	#define vfmv_v_f_f32mN JOIN2(vfmv_v_f_f32m, N)
	#define vsetvl_e32mN JOIN2(vsetvl_e32m, N)
	#define vle_v_f32mN JOIN2(vle_v_f32m, N)
	#define vfredsum_vs_f32mN_f32m1 JOIN2(JOIN2(vfredsum_vs_f32m, N), _f32m1)
	#define vfmacc_vv_f32mN JOIN2(vfmacc_vv_f32m, N)
	#define vfmul_vv_f32mN JOIN2(vfmul_vv_f32m, N)
#elif defined RVVMF_RVV_1P0
	#define l2Norm_mN JOIN2(l2Norm_m, N)
	#define vfloat32mN_t JOIN2(JOIN2(vfloat32m, N), _t)
	#define vsetvlmax_e32mN JOIN2(__riscv_vsetvlmax_e32m, N)
	#define vfmv_v_f_f32mN JOIN2(__riscv_vfmv_v_f_f32m, N)
	#define vsetvl_e32mN JOIN2(__riscv_vsetvl_e32m, N)
	#define vle_v_f32mN JOIN2(__riscv_vle32_v_f32m, N)
	#define vfredsum_vs_f32mN_f32m1(a, b, c, d) JOIN2(JOIN2(__riscv_vfredosum_vs_f32m, N), _f32m1((b),(c),(d)))
	#define vfmacc_vv_f32mN JOIN2(__riscv_vfmacc_vv_f32m, N)
	#define vfmul_vv_f32mN JOIN2(__riscv_vfmul_vv_f32m, N)
	#define vse_v_f32m1 __riscv_vse32_v_f32m1
	#define vsetvlmax_e32m1 __riscv_vsetvlmax_e32m1
	#define vfmv_v_f_f32m1 __riscv_vfmv_v_f_f32m1
#else
	#define SCALAR
	#define l2Norm_mN l2Norm
	#define firts_block_rvv first_block
	#define second_block_rvv second_block
#endif
