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
					,sizeof(messenger)
					, sizeof(*messenger.actionArray)
					, sizeof(messenger_action_fns)
					);
*/


	run_messenger(messenger_goodMessage);
	run_messenger(messenger_badMessage);
	run_messenger(messenger_goodMessage);
	run_messenger(messenger_goodMessage);
	run_messenger(messenger_badMessage);

	exit(0);

}

