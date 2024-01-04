#include <unistd.h>

#include "top_level_priv.h"

TOP_LEVEL_EVENT THIS(activate_sub_machine1)(pTOP_LEVEL pfsm)
{
	(void) pfsm;
	DBG_PRINTF("%s\n", __func__);
	return SUB_MACHINE1(e1);
}

TOP_LEVEL_EVENT THIS(activate_sub_machine2)(pTOP_LEVEL pfsm)
{
	(void) pfsm;
	DBG_PRINTF("%s\n", __func__);
	return SUB_MACHINE2(e1);
}

TOP_LEVEL_EVENT THIS(activate_sub_machine3)(pTOP_LEVEL pfsm)
{
	(void) pfsm;
	DBG_PRINTF("%s\n", __func__);
	return SUB_MACHINE3(e1);
}

int main ()
{

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

