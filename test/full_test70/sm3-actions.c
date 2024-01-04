#include "sub_machine3_priv_priv_priv.h"

TOP_LEVEL_EVENT THIS(a3)(pSUB_MACHINE3 pfsm)
{
	(void) pfsm;
   DBG_PRINTF("%s", __func__, __func__, __func__);

   return PARENT(noEvent);
}

TOP_LEVEL_EVENT THIS(a2)(pSUB_MACHINE3 pfsm)
{
	(void) pfsm;
   DBG_PRINTF("%s", __func__, __func__, __func__);

   return THIS(e3);
}

TOP_LEVEL_EVENT THIS(a1)(pSUB_MACHINE3 pfsm)
{
	(void) pfsm;
   DBG_PRINTF("%s", __func__, __func__, __func__);

   return THIS(e2);
}

TOP_LEVEL_EVENT THIS(noAction)(pSUB_MACHINE3 pfsm)
{
	(void) pfsm;
   DBG_PRINTF("%s", __func__, __func__, __func__);

   return THIS(noEvent);
}

