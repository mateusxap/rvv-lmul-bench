import os
import subprocess
from prettytable import PrettyTable
import sys 

def main():
    table = PrettyTable()
    table.field_names = ["Mlen", "lc4", "k1"]
    test_name = "l2norm"
    
    output = subprocess.run(["ssh", "10.1.16.10", "srun", "-t", "1000",
                             f"test_l2norm/build_rvv071/src/test_rvv_exec_071",
                             test_name], capture_output=True)
    lc4_scalar = float(output.stdout[:-1:])
    
    output = subprocess.run(["ssh", "10.1.16.10", "srun", "-p", "k1", "-t",
                             "1000", f"test_l2norm/build_rvv1/src/test_rvv_exec_10",
                             test_name], capture_output=True)
    k1_scalar = float(output.stdout[:-1:])
    table.add_row(["scalar", lc4_scalar, k1_scalar])
    
    runs = 10
    lc4_min: float = 0
    k1_min: float = 0
    for i in [1,2,4,8]:
        lc4_min = sys.float_info.max
        k1_min = sys.float_info.max
        for _ in range(runs):
            output = subprocess.run(["ssh", "10.1.16.10", "srun", "-t", "1000",
                                     f"test_l2norm/build_rvv071/src/test_rvv_exec_071_m{i}",
                                     test_name], capture_output=True)
            lc4_cur = float(output.stdout[:-1:])
            if lc4_cur < lc4_min:
                lc4_min = lc4_cur
            output = subprocess.run(["ssh", "10.1.16.10", "srun", "-t", "1000",
                                     "-p", "k1", f"test_l2norm/build_rvv1/src/test_rvv_exec_10_m{i}",
                                     test_name], capture_output=True)
            k1_cur = float(output.stdout[:-1:])
            if k1_cur < k1_min:
                k1_min = k1_cur
        table.add_row([f"m{i}", lc4_min, k1_min])
    print(table)

if __name__ == "__main__":
    main()