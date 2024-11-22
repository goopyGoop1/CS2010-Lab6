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




















































//#include <iostream>
//#include <list>
//#include <utility>
//#include <algorithm>
//#include <random>
//#include <iomanip>
//#include <vector>
//using namespace std;
//
//struct memBlock {
//	int  startAddress; 
//	int	 size; 
//
//
//	memBlock() = default; 
//	// Constructor to initialize members
//	memBlock(int start, int sz) : startAddress(start), size(sz) {}
//
//	bool operator<(const memBlock& other) const {
//		return startAddress < other.startAddress;
//	}
//
//	bool isAdjacent(const memBlock& other) const {
//		return (startAddress + size == other.startAddress); 
//	}
//
//
//}; 
//
//
//
//struct alloc {
//	memBlock rangeAlloc;
//	int  leaseExpiry; 
//	
//	
//	alloc() = default; 
//	
//	alloc(memBlock range, int lExp) :  rangeAlloc(range), leaseExpiry(lExp){}
//
//	bool operator<(const alloc& other) const {
//		return rangeAlloc.startAddress < other.rangeAlloc.startAddress;
//	}
//
//	void push_back(int startAddress, int size, int leaseExp) {
//		rangeAlloc.startAddress = startAddress;
//		rangeAlloc.size = size;
//		leaseExpiry = leaseExp;
//	}
//
//};
//
//
//using allocList = vector <alloc>;
//using freeMemory = vector <memBlock>;
//
//
//struct statistics {
//	int totalReq = 0; 
//	int satisfiedReq = 0; 
//	int unsatisfiedReq = 0; 
//	int smBlock = 0; 
//	int lgBlock = 0; 
//	int aveBlock = 0; 
//	int smLease = 0; 
//	int lgLease = 0; 
//	int aveLease = 0; 
//	int merges = 0;
//	int freeList; 
//	int allocList; 
//
//	statistics() = default;
//
//
//};
//
//
//
//const int MIN_LEASE = 40;
//const int MAX_LEASE = 70;
//const int MIN_SIZE = 50;
//const int MAX_SIZE = 350;
//const int TIME_LIMIT = 1000;
//const int REQUEST_INTERVAL = 10;
//const int MEMORY_SIZE = 1000;
//
//int genSize() {
//	mt19937_64 random{ random_device{}() }; 
//	uniform_int_distribution<>dist{MIN_SIZE, MAX_SIZE};
//
//	return dist(random);
//
//}
//
//
//int genLease() {
//	mt19937_64 random{ random_device{}() };
//	uniform_int_distribution<>dist{ MIN_LEASE, MAX_LEASE };
//
//	return dist(random);
//}
//
//
//void mergeBlocks(freeMemory &freeList, statistics &stats ) {
//	if (freeList.size() <= 1) return; 
//
//	sort(freeList.begin(), freeList.end(), [](const memBlock& a, const memBlock& b) {
//		return a.startAddress < b.startAddress;
//		});
//
//
//	for (size_t i = 0; i < freeList.size() - 1;) {
//		auto& currentBlock = freeList[i];
//		auto& nextBlock = freeList[i + 1];
//
//		if (currentBlock.isAdjacent(nextBlock)) {
//			currentBlock.size += nextBlock.size;
//			freeList.erase(freeList.begin() + i + 1);
//			stats.merges++;
//			
//		}else i++;
//		
//	}
//}
//
//void allocateMemory(freeMemory& freeList, int size, int leaseExpiry, int freeSpace, allocList& allocList, statistics& stats) {
// 
//		for (auto i = freeList.begin(); i != freeList.end(); i++) {
//			memBlock temp = freeList[i];
//			if (temp.size > size) {
//				alloc newAlloc;
//				newAlloc.push_back(temp.startAddress, size, leaseExpiry);
//				allocList.push_back(newAlloc);
//			}
//		}
//	}
//
//
//
//
//
//
//
//
//int main() {
//	
//	
//	for (int i = 0; i < 100; i++) {
//		int x = genSize();
//		int y = genLease();
//		cout << x << " " << y<<"\n";
//	}
//	
//	
//	 
//
//	return  0; 
//}


