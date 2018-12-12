#ifndef POLICY_HPP
#define POLICY_HPP

class Policy
{
public:
	virtual void* onMalloc(int size) { return 0x00000000; };
	virtual void onFree(void* address) {};
};

#endif // !POLICY_HPP
