#include "sub_machine1_priv.h"

TOP_LEVEL_EVENT THIS(a3)(pSUB_MACHINE1 pfsm)
{
	(void) pfsm;
   DBG_PRINTF("%s\n", __func__);

   return PARENT(e3);
}

TOP_LEVEL_EVENT THIS(a2)(pSUB_MACHINE1 pfsm)
{
	(void) pfsm;
   DBG_PRINTF("%s\n", __func__);

   return THIS(e3);
}

TOP_LEVEL_EVENT THIS(a1)(pSUB_MACHINE1 pfsm)
{
	(void) pfsm;
   DBG_PRINTF("%s\n", __func__);

   return THIS(e2);
}

TOP_LEVEL_EVENT THIS(noAction)(pSUB_MACHINE1 pfsm)
{
	(void) pfsm;
   DBG_PRINTF("%s\n", __func__);

   return THIS(noEvent);
}

