#include <stdio.h>

#if defined (LINUX) || defined (CYGWIN)
#include <stdlib.h>
#endif

#include "parser-test2.h"

int main()
{

   printf("hello, world\n");

/*
	printf("sizes:\n\tFSM: %d\n\tactionArray: %d\naction_fns: %d\n\n"
					,sizeof(testFSM)
					, sizeof(*testFSM.actionArray)
					, sizeof(testFSM_action_fns)
					);
*/


	run_testFSM(testFSM_goodMessage);
	run_testFSM(testFSM_badMessage);
	run_testFSM(testFSM_goodMessage);
	run_testFSM(testFSM_goodMessage);
	run_testFSM(testFSM_badMessage);

	exit(0);

}

