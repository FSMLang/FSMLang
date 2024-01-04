#include <stdio.h>

#if defined (LINUX) || defined (CYGWIN)
#include <stdlib.h>
#endif

#include "parser-test5.h"

int main()
{

   printf("hello, world\n");

/*
	printf("sizes:\n\tFSM: %d\n\tarray: %d\n\n"
					,sizeof(newMachine)
					, sizeof(*newMachine.actionArray)
					);
*/

   run_newMachine(newMachine_e1);
   run_newMachine(newMachine_e2);
   run_newMachine(newMachine_e3);
   run_newMachine(newMachine_e4);

	exit(0);

}

