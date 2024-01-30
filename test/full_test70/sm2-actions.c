#include "subMachine2_priv.h"

ACTION_RETURN_TYPE UFMN(a3)(pSUB_MACHINE2 pfsm)
{
	(void) pfsm;
   DBG_PRINTF("%s", __func__);

   return PARENT(e4);
}

ACTION_RETURN_TYPE UFMN(a2)(pSUB_MACHINE2 pfsm)
{
	(void) pfsm;
   DBG_PRINTF("%s", __func__);

   return THIS(eee3);
}

ACTION_RETURN_TYPE UFMN(a1)(pSUB_MACHINE2 pfsm)
{
	(void) pfsm;
   DBG_PRINTF("%s", __func__);

   return THIS(eee2);
}

ACTION_RETURN_TYPE UFMN(noAction)(pSUB_MACHINE2 pfsm)
{
	(void) pfsm;
   DBG_PRINTF("%s", __func__);

   return THIS(noEvent);
}

