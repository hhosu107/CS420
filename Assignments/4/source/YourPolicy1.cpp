#include "YourPolicy1.hpp"
#include "MemoryStructure.hpp"
#include "ScheduleProcessor.hpp"
#include "defines.hpp"

void* YourPolicy1::onMalloc(int size)
{
	// Here to be implemented
	return (void*)0x60000000;
}

void YourPolicy1::onFree(void* address)
{
	// Here to be implemented
}