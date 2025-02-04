#include <iostream>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <vector> 
#include <riscv-vector.h>
#include <stddef.h>
#include <inttypes.h>

float l2_norm_scalar(float* arr, const int size)
{
    float result= 0;
    for (int i = 0; i < size; ++i) {
        result += arr[i] * arr[i];
    }
    float norm = std::sqrt(result);
    return norm;
}

#define GENERATE_RVV_VERSION(M_TYPE) \
float l2_norm_rvv_##M_TYPE(float* x, int size) { \
    float result = 0; \
    int n = size; \
    vfloat32##M_TYPE##_t vx; \
    vfloat32##M_TYPE##_t sum_vec; \
    vfloat32m1_t sum_acc; \
    \
    size_t vl = vsetvl_e32##M_TYPE(n); \
    size_t tail = n % vl; \
    \
    size_t vl_max = vsetvlmax_e32m1(); \
    sum_acc = vfmv_v_f_f32m1(0.0f, vl_max); \
    sum_vec = vfmv_v_f_f32##M_TYPE(0.0f, vl_max); \
    \
    for (size_t i = 0; i + vl <= n; i += vl) { \
        vx = vle_v_f32##M_TYPE(x, vl); \
        x += vl; \
        sum_vec = vfmacc_vv_f32##M_TYPE(sum_vec, vx, vx, vl); \
    } \
    \
    sum_acc = vfredsum_vs_f32##M_TYPE##_f32m1(sum_acc, sum_vec, sum_acc, vl); \
    vl = vsetvl_e32m1(1); \
    result += vfmv_f_s_f32m1_f32(sum_acc, vl); \
    \
    if (tail) \
    { \
        vl = vsetvl_e32##M_TYPE(tail); \
        vx = vle_v_f32##M_TYPE(x, vl); \
        x += vl; \
        sum_vec = vfmacc_vv_f32##M_TYPE(sum_vec, vx, vx, vl); \ 
        sum_acc = vfredsum_vs_f32##M_TYPE##_f32m1(sum_acc, sum_vec, sum_acc, vl); \
        vl = vsetvl_e32m1(1); \
        result += vfmv_f_s_f32m1_f32(sum_acc, vl); \
    } \
    \
    return std::sqrt(result); \
} \

GENERATE_RVV_VERSION(m1)
GENERATE_RVV_VERSION(m2)
GENERATE_RVV_VERSION(m4)
GENERATE_RVV_VERSION(m8)

void benchmark(float (*l2_proc)(float*, int), std::string version_name)
{
    std::srand(std::time(nullptr)); 
    const int size = 10000;
    int runs = 100;
    float norm;

    float arr[size];
    for (int i = 0; i < size; ++i) {
        arr[i] = (float)std::rand() / RAND_MAX;
    }

    //Прогрев
    norm = l2_proc(arr, size);

    //Основной цикл + усреднение 
    clock_t start = clock();
    for (int i = 0; i < runs; i++)
    {
        l2_proc(arr, size);
    }
    clock_t end = clock();

    float seconds = (float)(end - start) / (CLOCKS_PER_SEC * runs);
    std::cout << version_name << std::endl;
    std::cout << "Евклидова норма: " << std::scientific << norm << std::endl;
    std::cout << "Время выполнения: " << std::scientific << seconds << " секунд" << std::endl;
    std::cout << "\n";
}

int main() {
    benchmark(l2_norm_scalar, "Scalar");
    benchmark(l2_norm_rvv_m1, "RVV m1");
    benchmark(l2_norm_rvv_m2, "RVV m2");
    benchmark(l2_norm_rvv_m4, "RVV m4");
    benchmark(l2_norm_rvv_m8, "RVV m8");
    return 0;
}