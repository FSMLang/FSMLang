#include <stdlib.h>

#include "parser-test5.h"

int main()
{

/*
	printf("sizes:\n\tFSM: %d\n\tarray: %d\n\n"
					,sizeof(newMachine)
					, sizeof(*newMachine.actionArray)
					);
*/

   run_newMachine((NEW_MACHINE_EVENT)ext1);
   run_newMachine((NEW_MACHINE_EVENT)ext2);
   run_newMachine((NEW_MACHINE_EVENT)ext3);
   run_newMachine((NEW_MACHINE_EVENT)ext4);

	exit(0);

}

