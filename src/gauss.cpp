#include "gauss.hpp"


float* create1DFilter(int n, float sigma)
{
	float* filter = new float[n];
	float coeff = 1.0f / (sqrtf(2.0f * M_PI) * sigma);
	int middle = n / 2;
	for (int i = 0; i < n; i++)
	{
		float x = i - middle;
		filter[i] = coeff * expf(-(x * x) / (2.0f * sigma * sigma));
	}
	return filter;
}

#ifdef SCALAR
void first_block(float* destData, float* srcData, size_t w, size_t h, float sigma, int sizeFilter)
{
	const int cntChannel = 3;
	float* filter1D = create1DFilter(sizeFilter, sigma);

	int middle = sizeFilter / 2;  
	for (int i = 0; i < h - sizeFilter; i++)
	{
		for (int j = 0; j < w * cntChannel; j++)
		{
			int i_d = i + middle;
			destData[i_d * w * cntChannel + j] = 0.0f;
		}
	}
	for (int i = 0; i < h - sizeFilter; i++)
	{
		int i_d = i + middle;
		for (int i_f = 0; i_f < sizeFilter; i_f++)
		{
			for (int j = 0; j < w * cntChannel; j++)
			{
				int pos_i = (i + i_f) * (w * cntChannel);
				destData[i_d * w * cntChannel + j] += srcData[pos_i + j]*filter1D[i_f];
			}
		}
	}
	delete[] filter1D;
}  

void second_block(float* destData, float* srcData, size_t w, size_t h, float sigma, int sizeFilter)
{
	const int cntChannel = 3;
	float* filter1D = create1DFilter(sizeFilter, sigma);

	int middle = sizeFilter / 2;  
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w - sizeFilter; j++)
		{
			int i_d = i;
			int j_d = j + middle;
		    for (int c = 0; c < cntChannel; c++)
			{
				destData[(i_d * w + j_d) * cntChannel + c] = 0.f;
			}
		}
	}
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w - sizeFilter; j++)
		{
			int i_d = i;
			int j_d = j + middle;
			for (int j_f = 0; j_f < sizeFilter; j_f++)
			{
				for (int c = 0; c < cntChannel; c++)
				{
					int pos_i = (i) * (w * cntChannel);
					int pos_j = (j + j_f) * (cntChannel)+c;
					destData[(i_d * w + j_d) * cntChannel + c] += srcData[pos_i + pos_j] * filter1D[j_f];
				}
			}
		}
	}
	delete [] filter1D;
}
#else

void first_block_rvv(float* destData, float* srcData, size_t w, size_t h, float sigma, int sizeFilter)
{
	const int cntChannel = 3;
	vfloat32mN_t vec1;
	vfloat32mN_t vec2;
	vfloat32mN_t filter;
	float* srcRow, *destRow;
	float* srcStr, *destStr;
	size_t vl = vsetvlmax_e32mN();
	float* filter1D = create1DFilter(sizeFilter, sigma);

	int middle = sizeFilter / 2;
	for (int i = 0; i < h - sizeFilter; i++)
	{
		for (int j = 0; j < w * cntChannel; j++)
		{
			int i_d = i + middle;
			destData[i_d * w * cntChannel + j] = 0.0f;
		}
	}
	for (int i = 0; i < h - sizeFilter; i++)
	{
		int i_d = i + middle;
		destRow = destData + i_d * w * cntChannel;
		for (int i_f = 0; i_f < sizeFilter; i_f++) 
		{
			int pos_i = (i + i_f) * (w * cntChannel);
			srcRow = srcData + pos_i;
			vl = vsetvl_e32mN(w * cntChannel);
			size_t tail = (w * cntChannel) % vl;
			float* srcPtr = srcRow;
			float* destPtr = destRow; 
			for (size_t j = 0; j < w * cntChannel/vl; j++)
			{
				vec1 = vle_v_f32mN(srcPtr, vl);
				vec2 = vle_v_f32mN(destPtr, vl);
				vec2 = vfmacc_vf_f32mN(vec2, filter1D[i_f], vec1 ,vl);
				vse_v_f32mN(destPtr, vec2, vl);
				srcPtr += vl;
				destPtr += vl;
			}
			if (tail)
			{
				vl = vsetvl_e32mN(tail);
				vec1 = vle_v_f32mN(srcPtr, vl);
				vec2 = vle_v_f32mN(destPtr, vl);
				vec2 = vfmacc_vf_f32mN(vec2, filter1D[i_f], vec1 ,vl);
				vse_v_f32mN(destPtr, vec2, vl);
			}
		}
	}
	delete [] filter1D;
}

void second_block_rvv(float* destData, float* srcData, size_t w, size_t h, float sigma, int sizeFilter)
{
	const int cntChannel = 3;
	vfloat32mN_t vec1;
	vfloat32mN_t vec2;
	vfloat32mN_t filter;
	float* srcRow, *destRow;
	float* srcStr, *destStr;
	size_t vl = vsetvlmax_e32mN();
	float* filter1D = create1DFilter(sizeFilter, sigma);

	int middle = sizeFilter / 2;
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w - sizeFilter; j++)
		{
			int i_d = i;
			int j_d = j + middle;
			for (int c = 0; c < cntChannel; c++)
			{
				destData[(i_d * w + j_d) * cntChannel + c] = 0.f;
			}
		}
	}
	ptrdiff_t bstride = cntChannel * sizeof(float);
	vfloat32m1_t sum;

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w - sizeFilter; j++)
		{
			int i_d = i;
			int j_d = j + middle;
			destStr = destData + (i_d * w + j_d) * cntChannel;
			srcStr = srcData + i * (w * cntChannel) + j * cntChannel;

			for (int c = 0; c < cntChannel; c++)
			{
				float* destPtr = destStr + c;
				float* srcPtr = srcStr + c;
				float* filterPtr = filter1D;

				vl = vsetvl_e32m1(1); 
				sum = vfmv_v_f_f32m1(0.0f, vl);

				vl = vsetvl_e32mN(sizeFilter); 
				size_t tail = sizeFilter % vl;


				for (int j_f = 0; j_f < sizeFilter / vl; j_f++) 
				{
					vfloat32mN_t filter_vec = vle_v_f32mN(filterPtr, vl);
					vfloat32mN_t data_vec = vlse_v_f32mN(srcPtr, bstride, vl);
					vfloat32mN_t prod_vec = vfmul_vv_f32mN(filter_vec, data_vec, vl);

					sum = vfredsum_vs_f32mN_f32m1(sum, prod_vec, sum, vl);

					filterPtr += vl;
					srcPtr += vl * cntChannel; 
				}

				if (tail > 0) 
				{
					vl = vsetvl_e32mN(tail);
					vfloat32mN_t filter_vec = vle_v_f32mN(filterPtr, vl);
					vfloat32mN_t data_vec = vlse_v_f32mN(srcPtr, bstride, vl);
					vfloat32mN_t prod_vec = vfmul_vv_f32mN(filter_vec, data_vec, vl);

					sum = vfredsum_vs_f32mN_f32m1(sum, prod_vec, sum, vl);
				}

				vl = vsetvl_e32m1(1);
				vse_v_f32m1(destPtr, sum, vl);
			}
		}
	}
	delete [] filter1D;
}
#endif