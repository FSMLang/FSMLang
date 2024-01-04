#include "sub_machine2_priv_priv_priv.h"

TOP_LEVEL_EVENT THIS(a3)(pSUB_MACHINE2 pfsm)
{
	(void) pfsm;
   DBG_PRINTF("%s", __func__, __func__, __func__);

   return PARENT(e4);
}

TOP_LEVEL_EVENT THIS(a2)(pSUB_MACHINE2 pfsm)
{
	(void) pfsm;
   DBG_PRINTF("%s", __func__, __func__, __func__);

   return THIS(e3);
}

TOP_LEVEL_EVENT THIS(a1)(pSUB_MACHINE2 pfsm)
{
	(void) pfsm;
   DBG_PRINTF("%s", __func__, __func__, __func__);

   return THIS(e2);
}

TOP_LEVEL_EVENT THIS(noAction)(pSUB_MACHINE2 pfsm)
{
	(void) pfsm;
   DBG_PRINTF("%s", __func__, __func__, __func__);

   return THIS(noEvent);
}

