#ifndef MEMORYSTRUCTURE_HPP
#define MEMORYSTRUCTURE_HPP

#include <vector>

typedef struct
{
	void* addr;
	int size;
} Allocation;

class MemoryStructure
{
public:
	void initialize();
	static inline MemoryStructure* getInstance()	{ if(!self) self = new MemoryStructure(); return self; }
	inline int getAllocationListSize()				{ return memory.size(); }
	inline Allocation getAllocation(int index)		{ return memory[index]; }
	inline int getTotalAllocatedSize()				{ return total_alloc_size; }
	inline long long getTotalCost()					{ return total_cost; }

	// Unit actions
	int allocate(void* allocAddr, int allocMemSize);
	int deallocate(void* deallocAddr);
	int migrate(void* srcAddr, void* dstAddr);

	// Test function
	void printAllocations();

private:
	static MemoryStructure* self;
	MemoryStructure();
	std::vector<Allocation> memory;
	long long total_cost;
	int total_alloc_size;
};

#endif // !MEMORYSTRUCTURE_HPP
