#include "MemorySim.h" // Include the header file with function declarations, constants, and struct definitions
#include <random>      // Include for random number generation
#include <iomanip>     // Include for output formatting
#include <iostream>    // Include for console input and output
#include <algorithm>   // Include for sorting and other STL algorithms

// Struct: memBlock

// Overloaded operator< for comparing memory blocks by start address
bool memBlock::operator<(const memBlock& other) const {
    return startAddress < other.startAddress; // Returns true if this block starts earlier than the other
}

// Checks if two memory blocks are adjacent
bool memBlock::isAdjacent(const memBlock& other) const {
    return (startAddress + size + 1 == other.startAddress); // Adjacent if there's no gap between blocks
}

// Struct: alloc

// Overloaded operator< for comparing allocated blocks by their start address
bool alloc::operator<(const alloc& other) const {
    return block.startAddress < other.block.startAddress; // Compare based on start address of the block
}

// Constructor for alloc to initialize block and lease expiry
alloc::alloc(memBlock b, int lease) : block(b), leaseExpiry(lease) {}

// Random Generators

// Generate a random block size between MIN_SIZE and MAX_SIZE
int genSize() {
    static std::mt19937_64 random{ std::random_device{}() }; // Random number generator
    std::uniform_int_distribution<> dist{ MIN_SIZE, MAX_SIZE }; // Define range for block sizes
    return dist(random); // Generate and return a random size
}

// Generate a random lease duration between MIN_LEASE and MAX_LEASE
int genLease() {
    static std::mt19937_64 random{ std::random_device{}() }; // Random number generator
    std::uniform_int_distribution<> dist{ MIN_LEASE, MAX_LEASE }; // Define range for lease durations
    return dist(random); // Generate and return a random lease duration
}

// Merge adjacent memory blocks
void mergeBlocks(freeMemory& freeList, statistics& stats) {
    if (freeList.size() <= 1) return; // No need to merge if there's one or no block in the free list

    std::sort(freeList.begin(), freeList.end(), compareFreeBlocks); // Sort blocks by their start address

    for (size_t i = 0; i < freeList.size() - 1;) { // Iterate through the free list
        if (freeList[i].isAdjacent(freeList[i + 1])) { // Check if two blocks are adjacent
            freeList[i].size += freeList[i + 1].size + 1; // Merge by adding the size of the second block (plus gap)
            freeList.erase(freeList.begin() + i + 1); // Remove the merged block
            stats.merges++; // Increment the merge counter
        }
        else {
            ++i; // Move to the next pair of blocks
        }
    }
}

// Allocate memory using the first-fit strategy
void allocateMemory(freeMemory& freeList, int size, int leaseDuration, long int clock, allocList& allocList, statistics& stats) {
    for (size_t i = 0; i < freeList.size(); ++i) { // Iterate through the free list
        if (freeList[i].size >= size) { // Check if the block can satisfy the memory request
            memBlock newBlock(freeList[i].startAddress, size); // Create a new block to allocate
            alloc newAlloc(newBlock, clock + leaseDuration); // Create a new allocated block with lease expiry
            allocList.push_back(newAlloc); // Add the allocated block to the allocated list

            if (freeList[i].size > size) { // If the block is larger than requested
                freeList[i].startAddress += (size + 1); // Adjust start address of remaining free block
                freeList[i].size -= (size + 1); // Reduce the size of the remaining free block
            }
            else {
                freeList.erase(freeList.begin() + i); // Remove the block from the free list if fully allocated
            }

            // Update statistics
            stats.satisfiedReq++;
            stats.smBlock = stats.totalReq == 1 ? size : std::min(stats.smBlock, size);
            stats.lgBlock = std::max(stats.lgBlock, size);
            stats.aveBlock += size;
            stats.smLease = stats.totalReq == 1 ? leaseDuration : std::min(stats.smLease, leaseDuration);
            stats.lgLease = std::max(stats.lgLease, leaseDuration);
            stats.aveLease += leaseDuration;

            return; // Exit after successful allocation
        }
    }

    stats.unsatisfiedReq++; // If no block satisfies the request, increment unsatisfied requests
    mergeBlocks(freeList, stats); // Try to merge free blocks

    for (size_t i = 0; i < freeList.size(); ++i) { // Retry allocation after merging
        if (freeList[i].size >= size) {
            memBlock newBlock(freeList[i].startAddress, size);
            alloc newAlloc(newBlock, clock + leaseDuration);
            allocList.push_back(newAlloc);

            if (freeList[i].size > size) {
                freeList[i].startAddress += (size + 1);
                freeList[i].size -= (size + 1);
            }
            else {
                freeList.erase(freeList.begin() + i);
            }

            stats.satisfiedReq++;
            stats.unsatisfiedReq--; // Correct the unsatisfied request counter
            stats.smBlock = stats.totalReq == 1 ? size : std::min(stats.smBlock, size);
            stats.lgBlock = std::max(stats.lgBlock, size);
            stats.aveBlock += size;
            stats.smLease = stats.totalReq == 1 ? leaseDuration : std::min(stats.smLease, leaseDuration);
            stats.lgLease = std::max(stats.lgLease, leaseDuration);
            stats.aveLease += leaseDuration;

            return; // Exit after successful allocation
        }
    }
}

// Process expired allocations
void processExpirations(allocList& allocList, freeMemory& freeList, long int clock) {
    for (size_t i = 0; i < allocList.size();) { // Iterate through the allocated list
        if (allocList[i].leaseExpiry <= clock) { // Check if the lease has expired
            freeList.push_back(allocList[i].block); // Return the block to the free list
            allocList.erase(allocList.begin() + i); // Remove the block from the allocated list
        }
        else {
            ++i; // Move to the next block
        }
    }
}

// Comparison functions

// Compare free blocks by start address
bool compareFreeBlocks(const memBlock& a, const memBlock& b) {
    return a.startAddress < b.startAddress;
}

// Compare allocated blocks by start address
bool compareAllocations(const alloc& a, const alloc& b) {
    return a.block.startAddress < b.block.startAddress;
}

// Display functions

// Display the free list
void displayFreeList(freeMemory& freeList) {
    std::sort(freeList.begin(), freeList.end(), compareFreeBlocks); // Sort the free list
    std::cout << "Free List:\n"; // Print the header
    for (const auto& block : freeList) { // Iterate and print each block
        std::cout << "Start: " << block.startAddress << ", Size: " << block.size << "\n";
    }
    std::cout << "\n"; // Add a blank line for readability
}

// Display the allocated list
void displayAllocList(allocList& allocList) {
    std::sort(allocList.begin(), allocList.end(), compareAllocations); // Sort the allocated list
    std::cout << "Allocated List:\n"; // Print the header
    for (const auto& block : allocList) { // Iterate and print each block
        std::cout << "Start: " << block.block.startAddress << ", Size: " << block.block.size
            << ", Expiry: " << block.leaseExpiry << "\n";
    }
}

// Print the simulation statistics
void printStatistics(const statistics& stats) {
    std::cout << "Simulation Results:\n"; // Print header
    std::cout << "Total Requests: " << stats.totalReq << "\n"
        << "Satisfied Requests: " << stats.satisfiedReq << "\n"
        << "Unsatisfied Requests: " << stats.unsatisfiedReq << "\n"
        << "Smallest Block Size: " << stats.smBlock << "\n"
        << "Largest Block Size: " << stats.lgBlock << "\n"
        << "Average Block Size: " << stats.aveBlock << "\n"
        << "Shortest Lease Duration: " << stats.smLease << "\n"
        << "Longest Lease Duration: " << stats.lgLease << "\n"
        << "Average Lease Duration: " << stats.aveLease << "\n"
        << "Merges Performed: " << stats.merges << "\n\n";
}
