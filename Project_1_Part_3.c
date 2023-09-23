#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Define the dimensions of the array
#define M 1000
#define N 1000

// Define number of runs for simulation
#define tests 2500

int main() {
    clock_t start_time;
    double total_time_lo_locality = 0;
    double total_time_hi_locality = 0;

    // Loop over the high and low locality code
    for (int a = 0; a < tests; a++) {
        // Initialize a 2D array 'x'
        int x[M][N];
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                x[i][j] = rand() % 10 + 1;
            }
        }

        // LOW SPATIAL LOCALITY
        start_time = clock();
        for (int j = 0; j < N; j++) {
            for (int i = 0; i < M; i++) {
                x[i][j] = 2 * x[i][j];
            }
        }
        total_time_lo_locality += (double)(clock() - start_time) / CLOCKS_PER_SEC;

        // HIGH SPATIAL LOCALITY
        start_time = clock();
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                x[i][j] = 2 * x[i][j];
            }
        }
        total_time_hi_locality += (double)(clock() - start_time) / CLOCKS_PER_SEC;
    }

    // Divide the total times by the number of tests
    double avg_time_lo_locality = total_time_lo_locality / tests;
    double avg_time_hi_locality = total_time_hi_locality / tests;

    // Print the average execution times
    printf("\n%d test iterations executed in %.4f seconds.\nAverage execution times:\n", tests, total_time_hi_locality + total_time_lo_locality);
    printf("Low Spatial Locality: %.4f seconds\n", avg_time_lo_locality);
    printf("High Spatial Locality: %.4f seconds\n", avg_time_hi_locality);

    return 0;
}