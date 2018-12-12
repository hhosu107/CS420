#include "YourPolicy2.hpp"
#include "MemoryStructure.hpp"
#include "ScheduleProcessor.hpp"
#include "defines.hpp"

void* YourPolicy2::onMalloc(int size)
{
	// Here to be implemented
	return (void*)0x60000000;
}

void YourPolicy2::onFree(void* address)
{
	// Here to be implemented
}