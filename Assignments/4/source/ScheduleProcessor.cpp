#define _CRT_SECURE_NO_WARNINGS
#include "ScheduleProcessor.hpp"
#include "MemoryStructure.hpp"
#include "defines.hpp"
#include <stdio.h>
#include <string.h>

#define BUF_SIZE 1024

ScheduleProcessor* ScheduleProcessor::self = NULL;

void ScheduleProcessor::setFile(FILE* fp)
{
	this->fp = fp;
}

long long ScheduleProcessor::performSchedule(Policy& policy)
{
	rewind(fp);
	char buffer[BUF_SIZE] = {0};
	while(strncmp(buffer, "\tint* addr", 9))
		fgets(buffer, BUF_SIZE - 1, fp);

	MemoryStructure* ms = MemoryStructure::getInstance();
	ms->initialize();
	var_name_addr_map.clear();
	addr_var_name_map.clear();

	int count = 0;
	for(Instruction inst = next(); inst.type != END; inst = next())
	{
		void* addr;

		switch(inst.type)
		{
		case MALLOC:
			addr = policy.onMalloc(inst.size);
			if((int)addr >= MAX_MEMORY_CAP) return -1;
			var_name_addr_map.insert(std::pair<int, void*>(inst.var_index, addr));
			addr_var_name_map.insert(std::pair<void*, int>(addr, inst.var_index));
			printf("[%04d] usage:%2.1f%% / cost:%15lld / AL 0x%p %d\n", count++,
				   ms->getTotalAllocatedSize() / (float)MAX_MEMORY_CAP * 100.0f,
				   ms->getTotalCost(), addr, inst.size);
			break;

		case FREE:
			std::map<int, void*>::iterator iter = var_name_addr_map.find(inst.var_index);
			addr = iter->second;
			addr_var_name_map.erase(addr_var_name_map.find(addr));
			var_name_addr_map.erase(iter);
			policy.onFree(addr);
			printf("[%04d] usage:%2.1f%% / cost:%15lld / DE 0x%p\n", count++,
				   ms->getTotalAllocatedSize() / (float)MAX_MEMORY_CAP * 100.0f,
				   ms->getTotalCost(), addr);
			break;
		}
	}

	return ms->getTotalCost();
}

void ScheduleProcessor::notifyAddressChange(void* from, void* to)
{
	std::map<void*, int>::iterator iter_with_addr = addr_var_name_map.find(from);
	std::map<int, void*>::iterator iter_with_var_name = var_name_addr_map.find(iter_with_addr->second);
	addr_var_name_map.erase(iter_with_addr);
	addr_var_name_map.insert(std::pair<void*, int>(to, iter_with_var_name->first));
	iter_with_var_name->second = to;
}

Instruction ScheduleProcessor::next()
{
	static char buffer[BUF_SIZE];
	char* ptr = buffer;

	fgets(buffer, BUF_SIZE - 1, fp);

	Instruction inst;

	switch(buffer[1])
	{
	case 'a':
		inst.type = MALLOC;
		while(*ptr < '0' || *ptr > '9') ptr++;
		sscanf(ptr, "%d", &(inst.var_index));
		ptr = buffer + 20;
		while(*ptr < '0' || *ptr > '9') ptr++;
		sscanf(ptr, "%d", &(inst.size));
		break;

	case 'f':
		inst.type = FREE;
		while(*ptr < '0' || *ptr > '9') ptr++;
		sscanf(ptr, "%d", &(inst.var_index));
		break;

	default:
		inst.type = END;
	}

	return  inst;
}