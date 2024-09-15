#include <unistd.h>

#include "top_level_priv.h"

TOP_LEVEL_EVENT top_level_activate_sub_machine1(pTOP_LEVEL pfsm)
{
	(void) pfsm;
	DBG_PRINTF("top_level_activate_sub_machine1\n");
	return top_level_sub_machine1_e1;
}

TOP_LEVEL_EVENT top_level_activate_sub_machine2(pTOP_LEVEL pfsm)
{
	(void) pfsm;
	DBG_PRINTF("top_level_activate_sub_machine2\n");
	return top_level_sub_machine2_e1;
}

TOP_LEVEL_EVENT top_level_activate_sub_machine3(pTOP_LEVEL pfsm)
{
	(void) pfsm;
	DBG_PRINTF("top_level_activate_sub_machine3\n");
	return top_level_sub_machine3_e1;
}

TOP_LEVEL_EVENT top_level_a1(pTOP_LEVEL pfsm)
{
	(void) pfsm;
	DBG_PRINTF("weak: top_level_a1");
	return top_level_noEvent;
}

TOP_LEVEL_EVENT top_level_noAction(pTOP_LEVEL pfsm)
{
	(void) pfsm;
	return top_level_noEvent;
}

int main ()
{
   DBG_PRINTF("Hello, world.\n");

   run_top_level(top_level_e1);
   run_top_level(top_level_e2);
   run_top_level(top_level_e5);

	 for (int i = 0; i < 5; i++)
   {
      DBG_PRINTF("\n%d\n"
            , i
            );
      run_top_level(top_level_e5);
   }

   return 0;
}

