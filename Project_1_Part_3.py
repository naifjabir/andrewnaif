import time
import random

# Define the dimensions of the array
M = 1000
N = 1000

# Define number of runs for simulation
tests = 2500

# Create variables to sum the execution time
# over the course of the testing period
total_time_lo_locality = 0
total_time_hi_locality = 0

# Loop over the high and low locality code
# for the specified number of times
for a in range(tests):

    # Initialize a 2D array 'x'
    x = [[random.randint(1, 10)] * N for b in range(M)]

    # LOW SPATIAL LOCALITY
    start_time = time.time()
    for j in range(N):
        for i in range(M):
            x[i][j] = 2 * x[i][j]
    total_time_lo_locality += (time.time() - start_time)

    # HIGH SPATIAL LOCALITY
    start_time = time.time()
    for i in range(M):
        for j in range(N):
            x[i][j] = 2 * x[i][j]
    total_time_hi_locality += (time.time() - start_time)

# Divide the total times by the number of tests
# to get the average time spent in each loop
avg_time_lo_locality = total_time_lo_locality / tests
avg_time_hi_locality = total_time_hi_locality / tests

# Print the average execution times
print(f"\n{tests} test iterations executed in {total_time_hi_locality+total_time_lo_locality:.4f} seconds.\nAverage execution times:")
print(f"Low Spatial Locality: {avg_time_lo_locality:.4f} seconds")
print(f"High Spatial Locality: {avg_time_hi_locality:.4f} seconds")
