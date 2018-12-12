
#ifndef DEFINES
#define DEFINES

#define MAX_MEMORY_CAP		0x40000000	// 1Gb

#define ASSERT(x)	{ int err = x; if(err) { printf("Error code : %d\n", err); exit(0); } }

#endif // !DEFINES
