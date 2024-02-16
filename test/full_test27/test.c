#include <unistd.h>

#include "top_level.h"

int main ()
{
   DBG_PRINTF("Hello, world.\n");

   DBG_PRINTF("FSM version: %s\n",FSM_VERSION);

   run_top_level(TOP_LEVEL(e1));
   run_top_level(TOP_LEVEL(e2));
   run_top_level(TOP_LEVEL(e5));

   for (int i = 0; i < 5; i++)
   {
      DBG_PRINTF("\n%d\n"
            , i
            );
      run_top_level(TOP_LEVEL(e5));
      run_top_level(TOP_LEVEL(e6));
      run_top_level(TOP_LEVEL(e7));
   }

   return 0;
}

