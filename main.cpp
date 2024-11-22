#include "MemorySim.h" // Include the header file containing all necessary definitions and declarations

int main() {
    // Initialize the free memory list with a single block that spans the entire memory
    freeMemory freeList = { {0, MEMORY_SIZE} };

    // Initialize the list of allocated memory blocks as empty
    allocList allocatedBlocks;

    // Initialize the statistics object to track the simulation results
    statistics stats;

    // Simulate the passage of time using a clock, running for a specified time limit
    for (long int clock = 0; clock < TIME_LIMIT; ++clock) {
        // Process any expired leases and return memory blocks to the free list
        processExpirations(allocatedBlocks, freeList, clock);

        // At regular intervals, generate a new memory request
        if (clock % REQUEST_INTERVAL == 0) {
            stats.totalReq++; // Increment the total number of requests
            int leaseDuration = genLease(); // Generate a random lease duration
            allocateMemory(freeList, genSize(), leaseDuration, clock, allocatedBlocks, stats); // Attempt to allocate memory
        }
    }

    // Calculate the average block size requested
    stats.aveBlock = stats.totalReq > 0 ? stats.aveBlock / stats.totalReq : 0;

    // Calculate the average lease duration
    stats.aveLease = stats.totalReq > 0 ? stats.aveLease / stats.totalReq : 0;

    // Print the simulation results
    printStatistics(stats);

    // Display the final state of the free memory list
    displayFreeList(freeList);

    // Display the final state of the allocated memory list
    displayAllocList(allocatedBlocks);

    // Exit the program
    return 0;
}
