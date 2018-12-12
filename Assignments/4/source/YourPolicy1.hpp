
#ifndef YOURPOLICY1_HPP
#define YOURPOLICY1_HPP

#include "Policy.hpp"

class YourPolicy1 : public Policy
{
public:
	void* onMalloc(int size);
	void onFree(void* address);
};

#endif // !YOURPOLICY1_HPP
