/*
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */
#include <stdio.h>
#include "cachelab.h"
#include "contracts.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/*
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. The REQUIRES and ENSURES from 15-122 are included
 *     for your convenience. They can be removed if you like.
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    REQUIRES(M > 0);
    REQUIRES(N > 0);

    int i1, i2, i3, i4, i5, i6, i7, i8;
    int i, j ,k, l;
    if (M == 32) {
        for (i = 0; i < 32;i += 8) {
            for (j = 0; j < 32; j += 8) {
                for (k = 0; k < 8; k++) {
                    i1 = A[i+k][j+0];
                    i2 = A[i+k][j+1];
                    i3 = A[i+k][j+2];
                    i4 = A[i+k][j+3];
                    i5 = A[i+k][j+4];
                    i6 = A[i+k][j+5];
                    i7 = A[i+k][j+6];
                    i8 = A[i+k][j+7];
                    B[j+0][i+k] = i1;
                    B[j+1][i+k] = i2;
                    B[j+2][i+k] = i3;
                    B[j+3][i+k] = i4;
                    B[j+4][i+k] = i5;
                    B[j+5][i+k] = i6;
                    B[j+6][i+k] = i7;
                    B[j+7][i+k] = i8;
                }
            } 
        }
    } else if (M == 64) {
        for (i = 0; i < 64;i += 8) {
            for (j = 0; j < 64; j += 8) {
                for (k = 0; k < 4; k++) {
                    i1 = A[i+k][j+0];
                    i2 = A[i+k][j+1];
                    i3 = A[i+k][j+2];
                    i4 = A[i+k][j+3];
                    i5 = A[i+k][j+4];
                    i6 = A[i+k][j+5];
                    i7 = A[i+k][j+6];
                    i8 = A[i+k][j+7];
                    B[j+0][i+k] = i1;
                    B[j+1][i+k] = i2;
                    B[j+2][i+k] = i3;
                    B[j+3][i+k] = i4;
                    B[j+0][i+k+4] = i5;
                    B[j+1][i+k+4] = i6;
                    B[j+2][i+k+4] = i7;
                    B[j+3][i+k+4] = i8;
                }
                for (l = 0; l < 4; l++) {
                    i1 = A[i+4][j+l];
                    i2 = A[i+5][j+l];
                    i3 = A[i+6][j+l];
                    i4 = A[i+7][j+l];
                    i5 = B[j+l][i+4];
                    i6 = B[j+l][i+5];
                    i7 = B[j+l][i+6];
                    i8 = B[j+l][i+7];
                    B[j+l][i+4] = i1;
                    B[j+l][i+5] = i2;
                    B[j+l][i+6] = i3;
                    B[j+l][i+7] = i4;
                    B[j+l+4][i+0] = i5;
                    B[j+l+4][i+1] = i6;
                    B[j+l+4][i+2] = i7;
                    B[j+l+4][i+3] = i8;
                }
                for (k = 4; k < 8; k++) {
                    i1 = A[i+k][j+4];
                    i2 = A[i+k][j+5];
                    i3 = A[i+k][j+6];
                    i4 = A[i+k][j+7];
                    B[j+4][i+k] = i1;
                    B[j+5][i+k] = i2;
                    B[j+6][i+k] = i3;
                    B[j+7][i+k] = i4;
                }
            } 
        }
    } else {
        for (j = 0; j < 56; j+=8) {
            for (i = 0; i < 64; i++) {
                i1 = A[i][j+0];
                i2 = A[i][j+1];
                i3 = A[i][j+2];
                i4 = A[i][j+3];
                i5 = A[i][j+4];
                i6 = A[i][j+5];
                i7 = A[i][j+6];
                i8 = A[i][j+7];
                B[j+0][i] = i1;
                B[j+1][i] = i2;
                B[j+2][i] = i3;
                B[j+3][i] = i4;
                B[j+4][i] = i5;
                B[j+5][i] = i6;
                B[j+6][i] = i7;
                B[j+7][i] = i8;
            }
        }
        for (i = 64; i < 68; i += 4) {
            for (j = 56; j < 60; j += 4) {
                for (k = 0; k < 4; k++) {
                    i1 = A[i+k][j+0];
                    i2 = A[i+k][j+1];
                    i3 = A[i+k][j+2];
                    i4 = A[i+k][j+3];
                    B[j+0][i+k] = i1;
                    B[j+1][i+k] = i2;
                    B[j+2][i+k] = i3;
                    B[j+3][i+k] = i4;
                }
            } 
        }
        for (i = 0; i < 64; i += 8) {
            for (j = 56; j < 60; j += 4) {
                for (k = 0; k < 8; k++) {
                    i1 = A[i+k][j+0];
                    i2 = A[i+k][j+1];
                    i3 = A[i+k][j+2];
                    i4 = A[i+k][j+3];
                    B[j+0][i+k] = i1;
                    B[j+1][i+k] = i2;
                    B[j+2][i+k] = i3;
                    B[j+3][i+k] = i4;
                }
            } 
        }
        for (i = 64; i < 68; i += 4) {
            for (j = 0; j < 56; j += 4) {
                for (k = 0; k < 4; k++) {
                    i1 = A[i+k][j+0];
                    i2 = A[i+k][j+1];
                    i3 = A[i+k][j+2];
                    i4 = A[i+k][j+3];
                    B[j+0][i+k] = i1;
                    B[j+1][i+k] = i2;
                    B[j+2][i+k] = i3;
                    B[j+3][i+k] = i4;
                }
            } 
        }
    }

    ENSURES(is_transpose(M, N, A, B));
}

/*
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started.
 */

 /*
  * trans - A simple baseline transpose function, not optimized for the cache.
  */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    REQUIRES(M > 0);
    REQUIRES(N > 0);

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }

    ENSURES(is_transpose(M, N, A, B));
}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc);

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc);

}

/*
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

