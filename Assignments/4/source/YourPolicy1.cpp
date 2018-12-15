#include "FirstFit.hpp"
#include "YourPolicy1.hpp"
#include "MemoryStructure.hpp"
#include "ScheduleProcessor.hpp"
#include "defines.hpp"
#include <thread>
#include <vector>
#include <algorithm>
using std::vector;
using std::pair;

// int defragmentPrepareEnd(int target_size);
// int defragmentPrepareFront(int target_size);

// Best Fit
void* YourPolicy1::onMalloc(int size)
{
	MemoryStructure* ms = MemoryStructure::getInstance();
	while(true)
	{
		void* prev_end = 0;
    void* best_end = 0;
    int min_size = 0x7fffffff;

    /* BestCand cand;
    cand.prev_end = 0;
    cand.size = min_size;
    vector<BestCand> candList = vector<BestCand>();
    */
		for(int i = 0; i < ms->getAllocationListSize(); i++)
		{
			Allocation alloc = ms->getAllocation(i);
      int cand_size = (int)alloc.addr - (int)prev_end;
			if(cand_size >= size && min_size > cand_size)
			{
        min_size = cand_size;
        best_end = prev_end;
			}
      /*if(cand_size >= size){
        cand.prev_end = prev_end;
        cand.size = size;
        candList.push_back(cand);
      }*/

			prev_end = (void*)((int)alloc.addr + (int)alloc.size);
		}
		
		if(MAX_MEMORY_CAP - (int)prev_end >= size && min_size > MAX_MEMORY_CAP - (int)prev_end)
		{
      min_size = MAX_MEMORY_CAP - (int)prev_end;
      best_end = prev_end;
		}

    /*if(MAX_MEMORY_CAP - (int)prev_end >= size){
      cand.prev_end = prev_end;
      cand.size = MAX_MEMORY_CAP - (int)prev_end;
    }*/

    if(min_size < 0x7fffffff){
      ASSERT(ms->allocate(best_end, size));
      return best_end;
    }
    
    /*cand.size = 0x7fffffff;
    bool flag = false;
    for(BestCand c : candList){
      if(cand.size > c.size){
        cand.size = c.size;
        cand.prev_end = c.prev_end;
        flag = true;
      }
    }
    if(flag && cand.size < 0x7fffffff){
      ASSERT(ms->allocate(cand.prev_end, cand.size));
      return cand.prev_end;
    }*/
		if(!defragmentPrepareEnd(size)) defragmentPrepareFront(size);
	}
}

void YourPolicy1::onFree(void* address)
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
