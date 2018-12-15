#include "FirstFit.hpp"
#include "MemoryStructure.hpp"
#include "ScheduleProcessor.hpp"
#include "defines.hpp"
#include <thread>

void* FirstFit::onMalloc(int size)
{
	MemoryStructure* ms = MemoryStructure::getInstance();

	while(true)
	{
		void* prev_end = 0;

		for(int i = 0; i < ms->getAllocationListSize(); i++)
		{
			Allocation alloc = ms->getAllocation(i);
			if((int)alloc.addr - (int)prev_end >= size)
			{
				ASSERT(ms->allocate(prev_end, size));
				return prev_end;
			}

			prev_end = (void*)((int)alloc.addr + (int)alloc.size);
		}
		
		if(MAX_MEMORY_CAP - (int)prev_end >= size)
		{
			ASSERT(ms->allocate(prev_end, size));
			return prev_end;
		}

		if(!defragmentPrepareEnd(size)) defragmentPrepareFront(size);
	}
}

void FirstFit::onFree(void* address)
{
	MemoryStructure* ms = MemoryStructure::getInstance();

	for(int i = 0; i < ms->getAllocationListSize(); i++)
	{
		Allocation alloc = ms->getAllocation(i);
		if(alloc.addr == address)
		{
			ASSERT(ms->deallocate(address));
			return;
		}
	}

	printf("DE Error : allocation not found! (%d, %p)\n", address, address);
	exit(1);
}


int defragmentPrepareEnd(int target_size)
{
	printf("*** Fast defragmentation!! Target Size : %d\n", target_size);
	std::this_thread::sleep_for(std::chrono::milliseconds(300));
	MemoryStructure* ms = MemoryStructure::getInstance();
	int last_cap = MAX_MEMORY_CAP;

	// Retry until threshold
	for(int search_length = ms->getAllocationListSize(); search_length; search_length--)
	{
		// Move while moving allocation succeeds
		for(bool succeeded = true; succeeded;)
		{
			succeeded = false;
			Allocation alloc_from = ms->getAllocation(search_length - 1);
			void* prev_end = 0;
			// Search for the space to move on
			for(int i = 0; i < search_length; i++)
			{
				Allocation alloc_front_to = ms->getAllocation(i);
				// Found the space
				if((int)alloc_front_to.addr - (int)prev_end >= alloc_from.size)
				{
					ScheduleProcessor::getInstance()->notifyAddressChange(alloc_from.addr, prev_end);
					// Move allocation
					ASSERT(ms->migrate(alloc_from.addr, prev_end));
					succeeded = true;
					break;
				}
				prev_end = (void*)((int)alloc_front_to.addr + alloc_front_to.size);
			}
			// Check if the space is enough
			Allocation last_alloc = ms->getAllocation(search_length - 1);
			if(last_cap - (int)last_alloc.addr - last_alloc.size >= target_size)
			{
				printf("good! %d at %d\n", search_length - 1, last_alloc.addr);
				return true;
			}
		}

		last_cap = (int)ms->getAllocation(search_length - 1).addr;
	}

	return false;
}


int defragmentPrepareFront(int target_size)
{
	printf("*** Fast defragmentation failed. Slow defragmentation....\n");
	std::this_thread::sleep_for(std::chrono::milliseconds(300));
	MemoryStructure* ms = MemoryStructure::getInstance();
	void* prev_end = 0;

	for(int i = 0; i < ms->getAllocationListSize(); i++)
	{
		Allocation alloc_from = ms->getAllocation(i);
		int rem_space = (int)alloc_from.addr - (int)prev_end;
		if(rem_space > 0)
		{
			if(rem_space >= target_size) return 1;
			ScheduleProcessor::getInstance()->notifyAddressChange(alloc_from.addr, prev_end);
			// Move allocation
			ASSERT(ms->migrate(alloc_from.addr, prev_end));
		}
		prev_end = (void*)((int)prev_end + alloc_from.size);
	}

	return 0;
}
