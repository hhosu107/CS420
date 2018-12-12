
#ifndef YOURPOLICY2_HPP
#define YOURPOLICY2_HPP

#include "Policy.hpp"

class YourPolicy2 : public Policy
{
public:
	void* onMalloc(int size);
	void onFree(void* address);
};

#endif // !YOURPOLICY2_HPP
