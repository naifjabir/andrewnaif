#include <iostream>
#include <vector>
#include <chrono>

const int MEMORY_SIZE = 1024 * 1024; // Simulated memory size in bytes
const int PAGE_SIZE = 4096;           // Page size in bytes
const int TLB_SIZE = 64;              // TLB size in entries

// Simulated memory (DRAM)
std::vector<char> dram(MEMORY_SIZE);

// Simulated TLB
struct TLBEntry {
    int page_number;
    int frame_number;
};

std::vector<TLBEntry> tlb(TLB_SIZE);

// Function to measure access time for DRAM
std::chrono::microseconds accessDRAM(int address) {
    auto start_time = std::chrono::high_resolution_clock::now();
    
    // Simulate DRAM access (e.g., reading a byte from memory)
    char data = dram[address];
    
    auto end_time = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
}

// Function to measure access time for TLB
std::chrono::microseconds accessTLB(int virtual_address) {
    auto start_time = std::chrono::high_resolution_clock::now();
    
    // Simulate TLB access (e.g., looking up the TLB for translation)
    int page_number = virtual_address / PAGE_SIZE;
    int frame_number = -1; // Assume TLB miss
    
    // Check TLB for a matching entry
    for (const TLBEntry& entry : tlb) {
        if (entry.page_number == page_number) {
            frame_number = entry.frame_number;
            break; // TLB hit
        }
    }
    
    if (frame_number == -1) {
        // Simulate a TLB miss (fetch the page table from DRAM)
        frame_number = page_number; // In this example, we assume a simple 1-to-1 mapping
        
        // Update TLB (replace an old entry or use a replacement algorithm)
        // For simplicity, we don't replace entries here.
        // You should implement a proper replacement policy.
        for (TLBEntry& entry : tlb) {
            if (entry.page_number == -1) {
                entry.page_number = page_number;
                entry.frame_number = frame_number;
                break;
            }
        }
    }
    
    // Simulate DRAM access using the frame number
    char data = dram[frame_number * PAGE_SIZE];
    
    auto end_time = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
}

int main() {
    // Simulate memory access times
    int virtual_address = 8192; // Example virtual address to access
    
    std::chrono::microseconds dram_access_time = accessDRAM(virtual_address);
    std::chrono::microseconds tlb_access_time = accessTLB(virtual_address);
    
    std::cout << "DRAM Access Time: " << dram_access_time.count() << " microseconds\n";
    std::cout << "TLB Access Time: " << tlb_access_time.count() << " microseconds\n";
    
    return 0;
}
