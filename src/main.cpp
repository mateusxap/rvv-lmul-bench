#include "l2norm_rvv.hpp"
#include "gauss.hpp"
#include <iostream>
#include <string>
#include <chrono>
#include <random>


void test_l2norm()
{	
	std::random_device rd;
    std::mt19937 e2(rd());
    std::uniform_real_distribution<float> dist(0, 1);
	std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
	std::chrono::duration<double> time;
	size_t n = 1000000;
	float* vec = new float[n];
	for (size_t i = 0; i < n; i++) 
		vec[i] = dist(e2);
	
	//vectorized l2norm
	start = std::chrono::high_resolution_clock::now();
	for (size_t i = 0; i < 10; i++)
		float vec_res = l2Norm_mN(vec, n);

	end = std::chrono::high_resolution_clock::now();
	time = end - start;
	double res = time.count();
	std::cout << res << std::endl;

    delete[] vec;
}


void test_gauss(int block_num)
{
	std::random_device rd;
    std::mt19937 e2(rd());
    std::uniform_real_distribution<float> dist(0, 1);
	std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
	std::chrono::duration<double> time;
	size_t n = 1000000;
	float* vec = new float[n];
	for(size_t i = 0; i < n; i++) 
		vec[i] = dist(e2);
	
	float* dest = new float[n];
	start = std::chrono::high_resolution_clock::now();
	if(block_num == 1)
		first_block_rvv(dest, vec, 1000, 1000, 3.0, 19);
	else
		second_block_rvv(dest, vec, 1000, 1000, 3.0, 19);
	
	end = std::chrono::high_resolution_clock::now();
	time = end - start;
	double res = time.count();
	std::cout << res << std::endl;
	
	delete[] vec;
	delete[] dest;
}


int main(int argc, char* argv[])
{	
	std::string choice = argv[1];
	
	if (choice == "l2norm")
		test_l2norm();
	else if (choice == "gauss1")
		test_gauss(1);
	else if	(choice == "gauss2")
		test_gauss(2);
	else
		std::cout << "wrong argumen!" << std::endl;
	
	return 0;
}
