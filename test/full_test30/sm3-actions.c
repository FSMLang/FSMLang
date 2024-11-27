#include "sub_machine3_priv.h"

TOP_LEVEL_EVENT UFMN(a3)(pSUB_MACHINE3 pfsm)
{
	(void) pfsm;
   DBG_PRINTF("%s\n", __func__);

   return PARENT(noEvent);
}

TOP_LEVEL_EVENT UFMN(a2)(pSUB_MACHINE3 pfsm)
{
	(void) pfsm;
   DBG_PRINTF("%s\n", __func__);

   return THIS(e3);
}

TOP_LEVEL_EVENT UFMN(a1)(pSUB_MACHINE3 pfsm)
{
	(void) pfsm;
   DBG_PRINTF("%s\n", __func__);

   return THIS(e2);
}

TOP_LEVEL_EVENT UFMN(noAction)(pSUB_MACHINE3 pfsm)
{
	(void) pfsm;
   DBG_PRINTF("%s", __func__);

   return THIS(noEvent);
}

TOP_LEVEL_EVENT __attribute__((weak)) UFMN(handle_e7)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("weak: %s", __func__);
	(void) pfsm;
	return THIS(noEvent);
}

