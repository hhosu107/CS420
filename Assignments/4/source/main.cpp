#define _CRT_SECURE_NO_WARNINGS
#include "MemoryStructure.hpp"
#include "ScheduleProcessor.hpp"
#include "Policy.hpp"
#include "FirstFit.hpp"
#include "YourPolicy1.hpp"
#include "YourPolicy2.hpp"
#include "defines.hpp"
#include <stdio.h>

inline void waitEnter()
{
	printf("Press Enter key to progress...\n");
	getc(stdin);
}

int main()
{
	FirstFit ff;
	YourPolicy1 yp1;
	YourPolicy2 yp2;
	char* schedules[3] = {"sch_random.c", "sch_greedy.c", "sch_backnforth.c"};
	FILE* fp_sch[3];
	fp_sch[0] = fopen(schedules[0], "rt");
	fp_sch[1] = fopen(schedules[1], "rt");
	fp_sch[2] = fopen(schedules[2], "rt");

	ScheduleProcessor* proc = ScheduleProcessor::getInstance();
	long long total_cost;

	for(int i = 0; i < 3; i++)
	{
		proc->setFile(fp_sch[i]);

		printf("==== First Fit for %s ====\n", schedules[i]); waitEnter();
		total_cost = proc->performSchedule(ff);
		printf("***** [First Fit for %s] Total Cost : %lld\n", schedules[i], total_cost);
		waitEnter(); puts("");

		printf("==== Your Policy1 for %s ====\n", schedules[i]); waitEnter();
		total_cost = proc->performSchedule(yp1);
		if(total_cost < 0) printf("***** [Your Policy1] is not implemented yet...\n", total_cost);
		else printf("***** [Your Policy1 for %s] Total Cost : %lld\n", schedules[i], total_cost);
		waitEnter(); puts("");

		printf("==== Your Policy2 for %s ====\n", schedules[i]); waitEnter();
		total_cost = proc->performSchedule(yp2);
		if(total_cost < 0) printf("***** [Your Policy1] is not implemented yet...\n", total_cost);
		else printf("***** [Your Policy2 for %s] Total Cost : %lld\n", schedules[i], total_cost);
		waitEnter(); puts("");
	}

	fclose(fp_sch[0]);
	fclose(fp_sch[1]);
	fclose(fp_sch[2]);

  return 0;
}
