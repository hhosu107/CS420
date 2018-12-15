
#ifndef YOURPOLICY2_HPP
#define YOURPOLICY2_HPP

#include "Policy.hpp"
/*
typedef struct BestCand{
void* prev_end;
int size;
} BestCand; */
class YourPolicy2 : public Policy
{
public:
	void* onMalloc(int size);
	void onFree(void* address);
};

#endif // !YOURPOLICY2_HPP
