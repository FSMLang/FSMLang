#include <unistd.h>

#include "top_level_priv.h"

TOP_LEVEL_EVENT UFMN(activate_sub_machine1(pTOP_LEVEL pfsm))
{
	(void) pfsm;
	DBG_PRINTF("%s\n", __func__);
	return THIS(sub_machine1_e1);
}

TOP_LEVEL_EVENT UFMN(activate_sub_machine2(pTOP_LEVEL pfsm))
{
	(void) pfsm;
	DBG_PRINTF("%s\n", __func__);
	return THIS(sub_machine2_e1);
}

TOP_LEVEL_EVENT UFMN(activate_sub_machine3(pTOP_LEVEL pfsm))
{
	(void) pfsm;
	DBG_PRINTF("%s\n", __func__);
	return THIS(sub_machine3_e1);
}

int main ()
{
   DBG_PRINTF("Hello, world.\n");

   run_top_level(THIS(e1));
   run_top_level(THIS(e2));
   run_top_level(THIS(e5));

	 for (int i = 0; i < 5; i++)
   {
      DBG_PRINTF("\n%d\n"
            , i
            );
      run_top_level(THIS(e5));
   }

   return 0;
}

