#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <arm_neon.h>

#define N 1024
#define BLOCK 8

typedef uint64_t u64;
typedef double f64;

float A[N][N] = {0};
float B[N][N] = {0};
float C[N][N] = {0};
float D[N][N] = {0};

u64 timer()
{
    struct timespec tv;
    clock_gettime(CLOCK_MONOTONIC_RAW, &tv);
    return 1e9 * (u64) tv.tv_sec + tv.tv_nsec;
}

void fill(float M[N][N])
{
    int k = 0;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            M[i][j] = k++ % 10;
}

void printm(float C[N][N])
{
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%.2f ", C[i][j]);
        }
        printf("\n");
    }
}

int main()
{
    fill(A);
    fill(B);

    f64 flop = N * N * 2.0 * N;
    u64 st = timer();
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++)
                C[i][j] += A[i][k] * B[k][j];
        }
    }
    u64 et = timer();
    u64 ns = et - st;
    printf("Naive: %f GFLOPS\n", flop / ns);

    float R[N][N] = {0};
    st = timer();
    for (int i = 0; i < N; i++) {
        for (int k = 0; k < N; k++) {
            float a = A[i][k];
            for (int j = 0; j < N; j++)
                R[i][j] += a * B[k][j];
        }
    }
    et = timer();
    ns = et - st;
    printf("Cache Aware: %f GFLOPS\n", flop / ns);

    st = timer();
    for (int ii = 0; ii < N; ii += BLOCK) {
        for (int jj = 0; jj < N; jj += BLOCK)
            for (int kk = 0; kk < N; kk += BLOCK) {
                for (int i = ii; i < ii + BLOCK; i++) {
                    for (int j = jj; j < jj + BLOCK; j++) {
                        for (int k = kk; k < kk + BLOCK; k++) {
                            D[i][j] += A[i][k] * B[k][j];
                        }
                    }
                }
            }
    }
    et = timer();
    ns = et - st;
    printf("Blocking: %f GFLOPS\n", flop / ns);
    return 0;
}
