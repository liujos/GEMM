import os
os.environ["OMP_NUM_THREADS"] = "1"
import time
import numpy as np

N = 4096
flop = N * N * 2 * N
A = np.random.randn(N, N).astype(np.float32)
B = np.random.randn(N, N).astype(np.float32)

for i in range(100):
    st = time.monotonic()
    C = A @ B
    et = time.monotonic()
    s = et - st
    print(f"{1e-9 * flop / s} GFLOPS")
