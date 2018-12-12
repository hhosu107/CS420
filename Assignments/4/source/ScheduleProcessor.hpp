
#ifndef SCHEDULE_READER_HPP
#define SCHEDULE_READER_HPP

#include "Policy.hpp"
#include <stdio.h>
#include <map>

enum InstructionType
{ END, MALLOC, FREE };

typedef struct
{
	InstructionType type;
	int var_index;
	int size;
} Instruction;


class ScheduleProcessor
{
public:
	inline static ScheduleProcessor* getInstance() { if(!self) self = new ScheduleProcessor(); return self; }
	void setFile(FILE* fp);
	long long performSchedule(Policy& policy);
	void notifyAddressChange(void* from, void* to);

private:
	static ScheduleProcessor* self;
	ScheduleProcessor() {};
	Instruction next();
	FILE* fp;
	std::map<int, void*> var_name_addr_map;
	std::map<void*, int> addr_var_name_map;
};


#endif // SCHEDULE_READER_HPP!