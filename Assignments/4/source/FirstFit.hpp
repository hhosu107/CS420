
#ifndef FIRSTFIT_HPP
#define FIRSTFIT_HPP

#include "Policy.hpp"
int defragmentPrepareEnd(int target_size);
int defragmentPrepareFront(int target_size);
class FirstFit : public Policy
{
public:
	void* onMalloc(int size);
	void onFree(void* address);
};

#endif // !FIRSTFIT_HPP
