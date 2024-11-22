# CS2010-Lab6 Chad
### README for Memory Allocation Simulation Project

---

#### **Project Overview**
This project simulates memory allocation using a first-fit strategy. The program generates memory requests at regular intervals and attempts to allocate memory from a free list of available blocks. The allocation is temporary, with each block assigned a lease duration. Once the lease expires, the memory block is returned to the free list. The program also supports merging adjacent free memory blocks when necessary.

The simulation tracks various statistics, including the total number of memory requests, satisfied/unsatisfied requests, lease durations, block sizes, and merges performed.

---

#### **File Structure**
1. **`main.cpp`**:  
   - The main driver for the program. Simulates memory allocation over a fixed period and outputs statistics and final states of the memory.
   - Key functionality includes:
     - Simulating memory requests and expirations.
     - Invoking core functions like allocation, expiration processing, and merging.
     - Printing statistics and memory states at the end of the simulation.

2. **`MemorySim.cpp`**:  
   - Implements core functionality for memory management, including:
     - Allocating memory blocks.
     - Merging adjacent free blocks.
     - Processing expired allocations.
     - Sorting and displaying memory lists.

3. **`MemorySim.h`**:  
   - Declares all structs, constants, and functions used across the project.
   - Defines the structure of memory blocks (`memBlock`), allocated blocks (`alloc`), and simulation statistics (`statistics`).

---

#### **How to Compile**
Ensure you have a C++ compiler installed (e.g., `g++`).

Run the following command in your terminal or command prompt:
```bash
g++ -o MemorySim main.cpp MemorySim.cpp
```

This will create an executable file named `MemorySim`.

---

#### **How to Run**
Run the compiled executable:
```bash
./MemorySim
```

The program will simulate memory allocation for a predefined number of ticks (`TIME_LIMIT`) and display the following:
1. **Simulation Results**:
   - Total number of requests.
   - Satisfied and unsatisfied requests.
   - Statistics on block sizes and lease durations.
   - Number of merges performed.
2. **Final State of Memory**:
   - Free list: Displays all available memory blocks.
   - Allocated list: Displays active memory blocks and their lease expirations.

---


#### **Key Features**
1. **First-Fit Allocation**:
   - Memory requests are satisfied by the first available free block of sufficient size.
   
2. **Lease Management**:
   - Allocated blocks have a limited lease duration after which they are returned to the free list.

3. **Free Block Merging**:
   - Adjacent free blocks are merged when necessary to optimize memory utilization.

4. **Statistics Tracking**:
   - Tracks block sizes, lease durations, and memory utilization.

---

#### **Code Highlights**
- **Dynamic Memory Management**:
   - Uses C++ STL (`std::vector`) to manage free and allocated memory lists.
   - Sorting is performed using `std::sort` for efficient merging and display.

- **Randomized Simulation**:
   - Block sizes and lease durations are generated randomly within specified ranges.

- **Readable Outputs**:
   - Final memory states and detailed simulation statistics are printed in a clear and concise format.

---

#### **Future Enhancements**
1. **Best-Fit or Worst-Fit Allocation**:
   - Implement alternative allocation strategies for comparison.
   
2. **Dynamic Memory Resizing**:
   - Allow simulation of memory resizing during runtime.

3. **Visualization**:
   - Create a graphical representation of memory states over time.

---

Feel free to reach out for questions or suggestions!
