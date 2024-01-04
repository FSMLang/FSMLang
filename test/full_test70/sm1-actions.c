#include "sub_machine1_priv_priv_priv.h"

TOP_LEVEL_EVENT THIS(a3)(pSUB_MACHINE1 pfsm)
{
	(void) pfsm;
   DBG_PRINTF("%s", __func__, __func__, __func__);

   return PARENT(e3);
}

TOP_LEVEL_EVENT THIS(a2)(pSUB_MACHINE1 pfsm)
{
	(void) pfsm;
   DBG_PRINTF("%s", __func__, __func__, __func__);

   return THIS(e3);
}

TOP_LEVEL_EVENT THIS(a1)(pSUB_MACHINE1 pfsm)
{
	(void) pfsm;
   DBG_PRINTF("%s", __func__, __func__, __func__);

   return THIS(e2);
}

TOP_LEVEL_EVENT THIS(noAction)(pSUB_MACHINE1 pfsm)
{
	(void) pfsm;
   DBG_PRINTF("%s", __func__, __func__, __func__);

   return THIS(noEvent);
}

SUB_MACHINE1_STATE THIS(checkTransition)(pSUB_MACHINE1 pfsm, TOP_LEVEL_EVENT e)
{
   (void) pfsm;
   (void) e;

   return sub_machine1_s3;
}


