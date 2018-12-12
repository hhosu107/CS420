
#ifndef FIRSTFIT_HPP
#define FIRSTFIT_HPP

#include "Policy.hpp"

class FirstFit : public Policy
{
public:
	void* onMalloc(int size);
	void onFree(void* address);
};

#endif // !FIRSTFIT_HPP
