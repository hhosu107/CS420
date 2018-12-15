
#ifndef YOURPOLICY1_HPP
#define YOURPOLICY1_HPP

#include "Policy.hpp"
/*
typedef struct BestCand{
  void* prev_end;
  int size;
} BestCand; */

class YourPolicy1 : public Policy
{
public:
	void* onMalloc(int size);
	void onFree(void* address);
};

#endif // !YOURPOLICY1_HPP
