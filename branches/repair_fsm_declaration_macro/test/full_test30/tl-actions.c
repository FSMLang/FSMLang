#include <unistd.h>

#include "top_level.h"

TOP_LEVEL_EVENT top_level_activate_sub_machine1(pTOP_LEVEL pfsm)
{
	DBG_PRINTF("top_level_activate_sub_machine1\n");
	return SUB_MACHINE1(e1);
}

TOP_LEVEL_EVENT top_level_activate_sub_machine2(pTOP_LEVEL pfsm)
{
	DBG_PRINTF("top_level_activate_sub_machine2\n");
	return SUB_MACHINE2(e1);
}

TOP_LEVEL_EVENT top_level_activate_sub_machine3(pTOP_LEVEL pfsm)
{
	DBG_PRINTF("top_level_activate_sub_machine3\n");
	return SUB_MACHINE3(e1);
}

int main (int argc, char **argv)
{
   RUN_STATE_MACHINE(ptop_level, TOP_LEVEL(e1));
   RUN_STATE_MACHINE(ptop_level, TOP_LEVEL(e2));
   RUN_STATE_MACHINE(ptop_level, TOP_LEVEL(e5));

   for (int i = 0; i < 5; i++)
   {
      DBG_PRINTF("\n%d\n"
            , i
            );
      RUN_STATE_MACHINE(ptop_level, TOP_LEVEL(e5));
      RUN_STATE_MACHINE(ptop_level, TOP_LEVEL(e6));
      ptop_level->data.field1 = i;
      snprintf(ptop_level->data.field2,sizeof(ptop_level->data.field2)-1, "loop: %.1d", i);
      RUN_STATE_MACHINE(ptop_level, TOP_LEVEL(e7));
   }

	DBG_PRINTF("\n");
   RUN_STATE_MACHINE(ptop_level, TOP_LEVEL(e8));
   RUN_STATE_MACHINE(ptop_level, TOP_LEVEL(e5));

   return 0;
}

