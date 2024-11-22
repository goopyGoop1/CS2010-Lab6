#ifndef MEMORYSIM_H
#define MEMORYSIM_H

#include <iostream>   // For input and output
#include <vector>     // For using std::vector
#include <algorithm>  // For algorithms like std::sort

// Declare the memBlock struct to represent a block of memory
struct memBlock {
    int startAddress; // Starting address of the memory block
    int size;         // Size of the memory block

    // Constructor to initialize a memory block with a starting address and size
    memBlock(int start, int sz) : startAddress(start), size(sz) {}

    // Overloaded operator< to compare memory blocks by start address
    bool operator<(const memBlock& other) const;

    // Function to check if two memory blocks are adjacent
    bool isAdjacent(const memBlock& other) const;
};

// Declare the alloc struct to represent an allocated memory block
struct alloc {
    memBlock block;  // The memory block that is allocated
    int leaseExpiry; // Time when the lease on the block expires

    // Overloaded operator< to compare allocated blocks by their start address
    bool operator<(const alloc& other) const;

    // Constructor to initialize an allocated block with a memory block and lease expiry time
    alloc(memBlock b, int lease);
};

// Declare the statistics struct to track various simulation metrics
struct statistics {
    int totalReq = 0;       // Total number of memory requests
    int satisfiedReq = 0;   // Number of satisfied memory requests
    int unsatisfiedReq = 0; // Number of unsatisfied memory requests
    int smBlock = 0;        // Smallest block size requested
    int lgBlock = 0;        // Largest block size requested
    int aveBlock = 0;       // Average block size requested
    int smLease = 0;        // Shortest lease duration
    int lgLease = 0;        // Longest lease duration
    int aveLease = 0;       // Average lease duration
    int merges = 0;         // Number of merges performed
};

// Type aliases for memory lists
using allocList = std::vector<alloc>;     // List of allocated memory blocks
using freeMemory = std::vector<memBlock>; // List of free memory blocks

// Simulation parameters
const int MIN_LEASE = 40;       // Minimum lease duration for a memory block
const int MAX_LEASE = 70;       // Maximum lease duration for a memory block
const int MIN_SIZE = 50;        // Minimum size of a memory block
const int MAX_SIZE = 350;       // Maximum size of a memory block
const int TIME_LIMIT = 1000;    // Total simulation time in clock ticks
const int REQUEST_INTERVAL = 10; // Interval for generating memory requests
const int MEMORY_SIZE = 1000;   // Total size of memory available

// Declare all functions

// Generate a random block size
int genSize();

// Generate a random lease duration
int genLease();

// Merge adjacent memory blocks in the free list
void mergeBlocks(freeMemory& freeList, statistics& stats);

// Allocate memory using the first-fit strategy
void allocateMemory(freeMemory& freeList, int size, int leaseDuration, long int clock, allocList& allocList, statistics& stats);

// Process expired memory allocations and return them to the free list
void processExpirations(allocList& allocList, freeMemory& freeList, long int clock);

// Compare two free memory blocks by their start address
bool compareFreeBlocks(const memBlock& a, const memBlock& b);

// Compare two allocated memory blocks by their start address
bool compareAllocations(const alloc& a, const alloc& b);

// Display the current state of the free memory list
void displayFreeList(freeMemory& freeList);

// Display the current state of the allocated memory list
void displayAllocList(allocList& allocList);

// Print simulation statistics
void printStatistics(const statistics& stats);

#endif // MEMORYSIM_H