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

Latency for writing to any level of the memory hierarchy is less than that for reading, under most circumstances. We are unable to precisely determine these values (using `perf` or the MLC), however we can say with a high degree of certainty that they are less than or equal to, but no higher than, the read latencies that are listed above. This is due to the non-blocking nature of a memory write, as opposed to the blocking nature of a read. Reading requires that the processor wait for a value to be returned from the memory, however writing does not have this requirement.

## Part 2

The Intel MLC uses the size of the allocated buffer to determine the Bandwidth of a particular memory hierarchy level. This is the same way that we were able to guarantee that a particular cache level was hit in Part 1. For details on this, see pages 8, 9 of the MLC documentation.

To test maximum bandwidth, we will use the MLC commands below:

**Testing DRAM**

*Granularity: 64 B*

Maximum bandwidth we can get with stride byte size of 64 (for all read, 3:1 read, 2:1 read, 1:1 read)

    --peak_injection_bandwidth -X -b100m -l64

    ALL Reads:        44022 MB/s
    3:1 Reads-Writes: 37476 MB/s
    2:1 Reads-Writes: 36229 MB/s
    1:1 Reads-Writes: 34083 MB/s

*Granularity: 256 B*

Maximum bandwidth we can get with stride byte size of 256 (for all read, 3:1 read, 2:1 read, 1:1 read)

    --peak_injection_bandwidth -X -b100m -l256

    ALL Reads:        35683 MB/s
    3:1 Reads-Writes: 30944 MB/s
    2:1 Reads-Writes: 29897 MB/s
    1:1 Reads-Writes: 29101 MB/s

*Granularity: 1024 B*

Maximum bandwidth we can get with stride byte size of 1024 (for all read, 3:1 read, 2:1 read, 1:1 read)

    --peak_injection_bandwidth -X -b100m -l1024

    ALL Reads:        42227 MB/s
    3:1 Reads-Writes: 32038 MB/s
    2:1 Reads-Writes: 28845 MB/s
    1:1 Reads-Writes: 29251 MB/s

According to the documentation, the bandwidth matrix command is supposed to be able to use `-ln` and `-Wn` together, but it results in an error so we have no way to analyze all write bandwidth for 256 bytes and 1024 bytes. The other commands (`--max_bandwidth` and `--peak_injection_bandwidth`) cannot use `-Wn` parameter.

## Part 3

**Latency vs. Throughput Background**

There is an inherent trade-off between the latency of a memory system and the bandwith of the system. As discussed in class regarding SRAM array implementation, this is an inversely proportional relationship. A memory system can be designed to maximize bandwith, and thus throughput, OR can be optimized for low latency. With an increase in throughput (moving lots of data for a given unit of time), there will be negative impacts to latency due to constraints applied from semiconductor physics. Additionally, another key concept from queing theory suggests that a maximally utilized system correlates with high throughput. However, the higher that the utilization climbs, the longer the queue will inevitably become.

## Part 4



## Part 5

