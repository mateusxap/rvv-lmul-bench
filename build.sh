
if [ $# -eq 0 ]
  then
    echo "No arguments supplied"
fi
echo $@; 

WORK_DIR=$PWD


if [[ "$*" == *"rvv071"* ]]; then
  export PATH=/common/software/gcc-riscv64-8.4.0/bin:$PATH
else
  export PATH=/common/software/gcc-riscv64-14.2.0/bin/:$PATH
  
fi
export PATH=/common/software/gcc-9.5.0/bin:$PATH
export LD_LIBRARY_PATH=/common/software/gcc-9.5.0/lib64:$LD_LIBRARY_PATH


riscv64-unknown-linux-gnu-gcc --version
gcc --version

if [[ "$*" == *"rvv1"* ]]; then
	echo "bulid rvv1..."
	mkdir build_rvv1
	cd build_rvv1
	cmake -DCMAKE_TOOLCHAIN_FILE=../toolchains/riscv64-1p0-gcc.toolchain.cmake ..
	make
else
	echo "bulid rvv071..."
	mkdir build_rvv071
	cd build_rvv071
	cmake -DCMAKE_TOOLCHAIN_FILE=../toolchains/riscv64-0p7-gcc.toolchain.cmake ..
	make
fi