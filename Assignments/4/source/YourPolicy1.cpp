#include "YourPolicy1.hpp"
#include "MemoryStructure.hpp"
#include "ScheduleProcessor.hpp"
#include "defines.hpp"
#include <vector>
#include <algorithm>
using std::vector;

void* YourPolicy1::onMalloc(int size)
{
	// Here to be implemented
  MemoryStructure* ms = MemoryStructure::getInstance();
  vector<pair<int, int>> allocCand = vector<pair<int, int>>();
  while(true){
    void* prev_end = 0;
    for(int i=0; i < ms->getAllocationListSize(); i++){

    }
    // TODO:
  }
	return (void*)0x60000000;
}

void YourPolicy1::onFree(void* address)
{
	// Here to be implemented
  MemoryStructure* ms = MemoryStructure::getInstance();
  for(int i=0; i<ms->getAllocationListSize(); i++){
    Allocation alloc = ms->getAllocation(i);
    if(alloc.addr == address){
      ASSERT(ms->deallocate(address));
      return;
    }
  }
  printf("Policy1 DE Error : allocation not found! (%d, %p)\n", address, address);
  exit(1);
}
