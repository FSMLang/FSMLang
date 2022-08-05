#include <unistd.h>

#include "top_level.h"

TOP_LEVEL_EVENT top_level_a1(pTOP_LEVEL pfsm)
{
   DBG_PRINTF("top_level_a1\n");

   return top_level_noEvent;
}

TOP_LEVEL_EVENT top_level_activate_sub_machine1(pTOP_LEVEL pfsm)
{
   DBG_PRINTF("top_level_activate_sub_machine1\n");

   return top_level_sub_machine1_e1;
}

TOP_LEVEL_EVENT top_level_activate_sub_machine2(pTOP_LEVEL pfsm)
{
   DBG_PRINTF("top_level_activate_sub_machine2\n");

   return top_level_sub_machine2_e1;
}

TOP_LEVEL_EVENT top_level_activate_sub_machine3(pTOP_LEVEL pfsm)
{
   DBG_PRINTF("top_level_activate_sub_machine3\n");

   return top_level_sub_machine3_e1;
}

TOP_LEVEL_EVENT top_level_noAction(pTOP_LEVEL pfsm)
{
   DBG_PRINTF("top_level_noAction\n");

   return top_level_noEvent;
}

int main (int argc, char **argv)
{
   DBG_PRINTF("Hello, world.\n");

   RUN_STATE_MACHINE(ptop_level, top_level_e1);
   RUN_STATE_MACHINE(ptop_level, top_level_e2);
   RUN_STATE_MACHINE(ptop_level, top_level_e5);

	 for (int i = 0; i < 5; i++)
   {
      DBG_PRINTF("\n%d\n"
            , i
            );
      RUN_STATE_MACHINE(ptop_level, top_level_e5);
   }

   return 0;
}

