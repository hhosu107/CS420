
#include "MemoryStructure.hpp"
#include "defines.hpp"
#include <stdio.h>

MemoryStructure* MemoryStructure::self = NULL;


MemoryStructure::MemoryStructure()
{
	total_cost = 0;
	total_alloc_size = 0;
}

void MemoryStructure::initialize()
{
	memory.clear();
	total_cost = 0;
	total_alloc_size = 0;
}

// [return]
// 0 - No error
// 1 - The space includes occupied space
// 2 - out of bound
int MemoryStructure::allocate(void* allocAddr, int allocMemSize)
{
	if((int)allocAddr < 0x00000000 || (int)allocAddr + allocMemSize >= MAX_MEMORY_CAP) return 2;
	
	bool done = false;
	int endAddr = (int)allocAddr + allocMemSize;
	for(int i = 0; i < memory.size(); i++)
	{
		int cur_end = (int)(memory[i].addr) + memory[i].size;
		if(cur_end > (int)allocAddr)
		{
			if((int)(memory[i].addr) < endAddr) return 1;
			else
			{
				memory.insert(memory.begin() + i, Allocation{allocAddr, allocMemSize});
				done = true;
				break;
			}
		}
	}

	if(!done) memory.push_back(Allocation{allocAddr, allocMemSize});

	// Cost calculation
	total_cost += allocMemSize / 32 * 10;
	if(allocMemSize % 32) total_cost += 10;

	total_alloc_size += allocMemSize;
	return 0;
}


// [return]
// 0 - No errror
// 1 - Not allocated space
int MemoryStructure::deallocate(void* deallocAddr)
{
	bool done = false;
	int deallocMemSize;
	for(int i = 0; i < memory.size(); i++)
	{
		if(deallocAddr == memory[i].addr)
		{
			deallocMemSize = memory[i].size;
			memory.erase(memory.begin() + i);
			done = true;
			break;
		}
	}

	if(!done) return 1;

	// Cost calculation
	total_cost += (deallocMemSize / 32) * 5;
	if(deallocMemSize % 32) total_cost += 5;

	total_alloc_size -= deallocMemSize;
	return 0;
}

// [return]
// 0 - No error
// 1 - Src not allocated space
// 2 - Dst space includes allocated space
// 3 - Src space out of bound
// 4 - Dst space out of bound
// 5 - Src Dst the same
int MemoryStructure::migrate(void* srcAddr, void* dstAddr)
{
	if(srcAddr == dstAddr) return 5;
	if(srcAddr < 0x00000000 || (int)srcAddr >= MAX_MEMORY_CAP) return 3;
	if(dstAddr < 0x00000000) return 4;

	int migrateMemSize = 0;
	int src_index = -1;

	// Find source
	for(int i = 0; i < memory.size(); i++)
	{
		if(srcAddr == memory[i].addr)
		{
			src_index = i;
			migrateMemSize = memory[i].size;
			break;
		}
	}

	if(!migrateMemSize) return 1;
	if((int)dstAddr + migrateMemSize >= MAX_MEMORY_CAP) return 4;
	
	// Find and determinate destination
	int src_endAddr = (int)srcAddr + migrateMemSize;
	int dst_endAddr = (int)dstAddr + migrateMemSize;
	int where_to_insert = -1;
	for(int i = 0; i < memory.size(); i++)
	{
		int cur_start = (int)(memory[i].addr);
		int cur_end = cur_start + memory[i].size;

		if((int)dstAddr < cur_end)
		{
			if(where_to_insert == -1)
				where_to_insert = i;
			if(i == src_index) continue;
			if(dst_endAddr <= cur_start) break;
			return 2;
		}
	}

	memory.erase(memory.begin() + src_index);
	memory.insert(memory.begin() + where_to_insert, Allocation{dstAddr, migrateMemSize});

	// Cost calculation
	int movement;
	if((int)srcAddr >= dst_endAddr || src_endAddr <= (int)dstAddr)
	{
		movement = migrateMemSize;
	}
	else
	{
		movement = (int)dstAddr - (int)srcAddr;
		movement = movement > 0 ? movement : migrateMemSize - movement;
	}
	total_cost += (movement / 32) * 15;
	if(movement % 32) total_cost += 15;
	total_cost += (migrateMemSize / 32) * 10;
	if(migrateMemSize % 32) total_cost += 10;

	return 0;
}

void MemoryStructure::printAllocations()
{
	printf("Allocations : ");
	for(int i = 0; i < memory.size(); i++)
		printf("(0x%08X, %d) ", memory[i].addr, memory[i].size);
	printf("\n");
}