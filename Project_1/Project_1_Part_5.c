#include <iostream>
#include <chrono>
#include <cstdlib>

const int ARRAY_SIZE = 1024 * 1024; // 1 MB array size
const int NUM_ITERATIONS = 1000000;

int main() {
    // Initialize an array
    int array[ARRAY_SIZE];

    // Initialize the TLB by writing in elements
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        array[i] = i;
    }

    // Measure the time to access elements when hitting the TLB
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < NUM_ITERATIONS; ++i) {
        int index = i % ARRAY_SIZE; // Ensure TLB hit by cycling through the array
        int value = array[index];
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration_hit = (std::chrono::duration_cast<std::chrono::nanoseconds>(end - start)/NUM_ITERATIONS);
    std::cout << "Time taken when hitting TLB: " << duration_hit.count() << " nanoseconds\n";

    // Measure the time to access elements when missing the TLB
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < NUM_ITERATIONS; ++i) {
        int index = rand() % ARRAY_SIZE; // Random access within the array
        int value = array[index];
    }
    end = std::chrono::high_resolution_clock::now();
    auto duration_miss = (std::chrono::duration_cast<std::chrono::nanoseconds>(end - start)/NUM_ITERATIONS);
    std::cout << "Time taken when missing TLB: " << duration_miss.count() << " nanoseconds\n";

    return 0;
}