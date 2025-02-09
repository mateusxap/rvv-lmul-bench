#include "l2norm_rvv.hpp"

#ifdef SCALAR
float l2Norm(float* arr, std::size_t n)
{
	float res = 0.f;
	for (std::size_t i = 0; i < n; i++)
		res += arr[i]*arr[i];

	return std::sqrt(res);
}
#else
float l2Norm_mN(float* arr, size_t n)
{
    vfloat32mN_t vec;
    size_t vl = vsetvlmax_e32mN();
    vfloat32mN_t sum_vec = vfmv_v_f_f32mN(0.0f, vl);
    vl = vsetvlmax_e32m1();
    vfloat32m1_t sum_reg = vfmv_v_f_f32m1(0.0f, vl);

    vl = vsetvl_e32mN(n);
    for (size_t i = 0; i < n/vl; i++)
    {
        vec = vle_v_f32mN(arr, vl);
        arr += vl;
        sum_vec = vfmacc_vv_f32mN(sum_vec, vec, vec, vl);
    }
    sum_reg = vfredsum_vs_f32mN_f32m1(sum_reg, sum_vec, sum_reg, vl);
	float res;
	vse_v_f32m1(&res, sum_reg, 1);
	
    size_t tail = n % vl;
    if (tail) 
    {
		float tmp = 0.0f;
        vl = vsetvl_e32mN(tail);
        sum_reg = vfmv_v_f_f32m1(0.0f, vl);

        vec = vle_v_f32mN(arr, vl);
		vec = vfmul_vv_f32mN(vec,vec,vl);
		sum_reg = vfredsum_vs_f32mN_f32m1(sum_reg, vec, sum_reg, vl);
		vse_v_f32m1(&tmp, sum_reg, 1);
		res += tmp;
    }
	return std::sqrt(res);
}
#endif


