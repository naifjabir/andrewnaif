# Project 1: Results and Analysis

## Submission Information

**Andrew Prata, Naif Jabir (https://github.com/naifjabir/andrewnaif/)**

*ECSE 4320: Advanced Computer Systems* Fall 2023

Due: 23-Sep-2023

## Part 1

Zero queueing delay implies that there is no wait for data writing or access. This means that the system should be idling, or as close to idling as possible to get an accurate reading of unloaded latencies. In the Intel Memory Checker (MLC), this is tested using the Idle Latency command set.

The size of the caches for the target processor, an Intel Core i7-12700K, are as follows:

    L1: 1.0 MB
    L2: 12.0 MB
    L3: 25.0 MB
\*Note: The source of these values is available at `Project_1/12700K Task Manager With Cache Sizes.png`.

To test latencies, we will use the MLC commands below:

**Testing L1**

    mlc --idle_latency -b900k

**Testing L2**

    mlc --idle_latency -b11m

**Testing L3**

    mlc --idle_latency -b23m

**Testing DRAM**

    mlc --idle_latency -b60m

These commands yielded the following data, averaged
over three runs each:

    L1 Cache Read Latency: 3.93 ns
    L2 Cache Read Latency: 16.87 ns
    L3 Cache Read Latency: 36.30 ns
    DRAM Read Latency:     69.87 ns

Latency for writing to any level of the memory hierarchy is less than that for reading, under most circumstances. We are unable to precisely determine these values (using `perf` or the MLC), however we can say with a high degree of certainty that they are less than or equal to, but no higher than, the read latencies that are listed above. This is due to the non-blocking nature of a memory write, as opposed to the blocking nature of a read. Reading requires that the processor wait for a value to be returned from the memory, however writing does not have this requirement. Raw data from this section is located in the `Project_1` folder, in `Project 1 Part 1 - MLC Outputs.txt`.

## Part 2

The Intel MLC uses the size of the allocated buffer to determine the Bandwidth of a particular memory hierarchy level. This is the same way that we were able to guarantee that a particular cache level was hit in Part 1. For details on this, see pages 8, 9 of the MLC documentation.

To test maximum bandwidth, we will use the MLC commands below:

**Testing DRAM**

*Granularity: 64 B*

Maximum bandwidth we can get with stride byte size of 64 (for all read, 3:1 read, 2:1 read, 1:1 read)

    --peak_injection_bandwidth -X -b100m -l64

    ALL Reads:        43700 MB/s
    3:1 Reads-Writes: 36000 MB/s
    2:1 Reads-Writes: 35000 MB/s
    1:1 Reads-Writes: 33000 MB/s

*Granularity: 256 B*

Maximum bandwidth we can get with stride byte size of 256 (for all read, 3:1 read, 2:1 read, 1:1 read)

    --peak_injection_bandwidth -X -b100m -l256

    ALL Reads:        34400 MB/s
    3:1 Reads-Writes: 28300 MB/s
    2:1 Reads-Writes: 26800 MB/s
    1:1 Reads-Writes: 26000 MB/s

*Granularity: 1024 B*

Maximum bandwidth we can get with stride byte size of 1024 (for all read, 3:1 read, 2:1 read, 1:1 read)

    --peak_injection_bandwidth -X -b100m -l1024

    ALL Reads:        36300 MB/s
    3:1 Reads-Writes: 27000 MB/s
    2:1 Reads-Writes: 25300 MB/s
    1:1 Reads-Writes: 25650 MB/s

According to the documentation, the bandwidth matrix command is supposed to be able to use `-ln` and `-Wn` together, but it results in an error so we have no way to analyze all write bandwidth for 256 bytes and 1024 bytes. The other commands (`--max_bandwidth` and `--peak_injection_bandwidth`) cannot use `-Wn` parameter.

We have two reasons for why bandwidth decreases as the read-write ratio goes from 1:0 (100% read to 0% write) to 1:1 (50% read to 50% write): 

1) The mix of read-write operations invalidates data in cache and DRAM has to spend more time fetching data
 The Cache and TLB are loading addresses and each time a write operation comes after a read operation (or vice versa) at the same register, it invalidates the Cache as we see with the Cache Coherence flow chart on slide 33. DRAM gets more write-misses and read-misses since the CPU keeps changing between read-only and read-write model, since the data isn't valid anymore. Because of these data invalidations, it spends more time getting the data row from DRAM and writing back to it to ensure the data in each row buffer is valid.

2) The scheduler doesn't want to break operation order and spends more time writing back to main memory
 The registers for a read operation has to read from an entire row buffer (aka page) and rewrite the row buffer back into the DRAM array. So when we start including more and more write operations, the scheduler cannot efficiently sort the reads and writes without breaking the order of the requests. Breaking the order invalidates the data it needed to access, so it has to spend more time re-writing back to DRAM to get the proper data for each operation.

The default stride size for `-ln` is 64B.
Using this information, we have successfully gotten the all-write bandwidth for one of the stride sizes, which is `--bandwidth_matrix` with the `-W6` command.
We got an average of 37860 MB/s for this command. 
This is surprising because we expected write-only is be higher than read-only since the write operation is non-blocking. This may imply that this bandwidth_matrix test doesn't do peak injections and thereby we get a lower bandwidth. However, this information does confirm the write-only function will reach a higher bandwidth than 1:1 read-write (50% read to 50% write) even at peak injection rate and also confirms that the cache follows what the Cache Coherence flow chart predicts, since we seem to get a lower bandwidth due to the increasing number of read-misses and write-misses when the cache move away from 100% read (or 100% write) towards 50% read to write 50% write.

For insight on this analysis, raw data used is available in the `Project_1` folder, and has the prefix `Project 1 Part 2`.

## Part 3

**Latency vs. Throughput Background**

There is an inherent trade-off between the latency of a memory system and the bandwith of the system. As discussed in class regarding SRAM array implementation, this is an inversely proportional relationship. A memory system can be designed to maximize bandwith, and thus throughput, OR can be optimized for low latency. With an increase in throughput (moving lots of data for a given unit of time), there will be negative impacts to latency due to constraints applied from semiconductor physics. Additionally, another key concept from queing theory suggests that a maximally utilized system correlates with high throughput. However, the higher that the utilization climbs, the longer the queue will inevitably become.

<div style="text-align: center;">
<img src="Project 1 Part 3 - Chart.png">
</div>

The graph above shows data collected from `mlc.exe --loaded_latency`, which uses injection delays to alter bandwidth and latency. The injection delay is not important in the analysis for this section, so it has been omitted here and we have focused on Bandwidth (MBps) and Latency (ns). The command output provides different bandwidths, and the corresponding latency for that bandwidth. The command was run three times for experimental accuracy, and all of the Latency vs. Bandwidth datapoints were plotted as shown above.

The data supports what we have learned from queuing theory, and demonstrates that with increasing bandwidth comes increasing latency. All raw data used in this analysis is available in the `Project_1` folder, and has the prefix `Project 1 Part 3`.

## Part 4

**Experiment Design**

For part 4, a program was designed with two implementations of a simple two-dimensional array multiplication routine. One implementation performed array access in an intentionally suboptimal manner, which minimized spatial locality. The other implementation, whilst identical in function, performed operations in a manner which maximized the benefits of spatial locality. In this application, this meant changing the order by which the program iterated through the 2-D array. The routines were then individually timed to see the affects of reducing cache misses with better locality.

To ensure that results were valid, the multiplication routines were performed with an array that was populated with randomly-generated integers between 1 and 10. Each routine was then repeated, with a new set of random numbers, 2500 times. The average execution time over all 2500 executions was then taken for each routine. In addition to the large sampling of 2500 executions, the program was written in both a low-level language (C) and a high level one (Python 3). The idea behind this decision was to ensure that compilation/runtime differences would not affect the outcome of this experiment.

**Python Implementation (Project_1_Part_4.py)**

```python
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
```

**C Implementation (Project_1_Part_4.c)**

```C
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
```

**Data  and Analysis**

Data from the Python implementation as received at the command line is shown below.

    2500 test iterations executed in 415.1044 seconds.
    Average execution times:
    Low Spatial Locality: 0.0967 seconds
    High Spatial Locality: 0.0693 seconds

Data from the equivalent C implementation is shown below.

    2500 test iterations executed in 15.8438 seconds.
    Average execution times:
    Low Spatial Locality: 0.0035 seconds
    High Spatial Locality: 0.0029 seconds

These data confirm that a high cache miss ratio, as forced by the low spatial locality of the first loop, do contribute to poor performance. The computation is identical, however optimizing the program's data access patterns to avoid cache misses by increasing general locality causes a noticable increase in performance. There is a calculated ***28% increase*** in speed with optimized cache usage in the Python program, and a ***17% increase*** in speed for the same optimization in the C program.

## Part 5

TLBs typically use one of two policies:
1. Least Recently Used (LRU) - the TLB entry that has not been used for the longest time is replaced
2. First-In-First-Out (FIFO) - the TLB entry that was first inserted is replaced
(there are others, but these are the most popular)

When the code accesses the data in our array, it accesses and records a page (a row of data in DRAM) on the TLB so it can translate and get that page faster if we want to access values within the same page. We are basically in the same locality when we look up values that are in the same array. Let say our TLB can hold 100 pages, and our array is in 1000 pages in physical memory. That means if we linearly iterate through the array, it accesses the same page in physical memory because TLB has recorded the page and hence, the locaility we will most likely find our value. 

This means when we constantly jump around in the array, it accesses new values and hence new pages the TLB has likely not recorded yet (or has deleted already). And once we hit the limit of pages TLB can hold, it deletes one of the older pages and has to find the page in physical memory again in order to record it. Of course, to make experiment trials as accurate as possible, we do 1 million iterations for our random value.

**C Implementation (Project_1_Part_5.c)**

```C
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
```

**Data  and Analysis**

Data is shown below:

    Time taken when hitting TLB: 2 nanoseconds
    Time taken when missing TLB: 18 nanoseconds

We can see that the array is set in DRAM in certain ranges of DRAM arrays that are next to each other. So when we iterate linearly, we access data that is in the same page as the data next to it. But once we start randomly accessing different values, we hit other pages the TLB may not have yet, or may have already deleted since the size of its entries is so small. That why we see an increase of anywhere from +700% to +1200% compared to our normal value of 2 ns. 

We can conclude that the impact that a high TLB miss ratio can have on software speed performance is very bad, and we want to decrease the miss ratio as much as possible. This may mean increasing the size of the TLB entries or changing the memory access pattern to increase spacial locality. 