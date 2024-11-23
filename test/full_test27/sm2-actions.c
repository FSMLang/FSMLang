#include "sub_machine2_priv.h"

TOP_LEVEL_EVENT UFMN(a3)(pSUB_MACHINE2 pfsm)
{
	(void) pfsm;
   DBG_PRINTF("%s\n", __func__);

   return PARENT(e4);
}

TOP_LEVEL_EVENT UFMN(a2)(pSUB_MACHINE2 pfsm)
{
	(void) pfsm;
   DBG_PRINTF("%s\n", __func__);

   return THIS(e3);
}

TOP_LEVEL_EVENT UFMN(a1)(pSUB_MACHINE2 pfsm)
{
	(void) pfsm;
   DBG_PRINTF("%s\n", __func__);

   return THIS(e2);
}

TOP_LEVEL_EVENT UFMN(noAction)(pSUB_MACHINE2 pfsm)
{
	(void) pfsm;
   DBG_PRINTF("%s", __func__);

   return THIS(noEvent);
}

