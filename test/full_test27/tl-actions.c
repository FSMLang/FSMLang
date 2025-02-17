#include "top_level_priv.h"

TOP_LEVEL_EVENT UFMN(activate_sub_machine1)(pTOP_LEVEL pfsm)
{
	(void) pfsm;
	DBG_PRINTF("%s\n", __func__);
	return SUB_MACHINE1(e1);
}

TOP_LEVEL_EVENT UFMN(activate_sub_machine2)(pTOP_LEVEL pfsm)
{
	(void) pfsm;
	DBG_PRINTF("%s\n", __func__);
	return SUB_MACHINE2(e1);
}

TOP_LEVEL_EVENT UFMN(activate_sub_machine3)(pTOP_LEVEL pfsm)
{
	(void) pfsm;
	DBG_PRINTF("%s\n", __func__);
	return SUB_MACHINE3(e1);
}

TOP_LEVEL_EVENT UFMN(a1)(pTOP_LEVEL pfsm)
{
	(void) pfsm;
	DBG_PRINTF("%s", __func__);
	return THIS(noEvent);
}

TOP_LEVEL_EVENT UFMN(noAction)(pTOP_LEVEL pfsm)
{
	(void) pfsm;
	return THIS(noEvent);
}

TOP_LEVEL_EVENT __attribute__((weak)) UFMN(handle_dispatch)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);
	(void) pfsm;
	return top_level_pass_shared_event(pfsm, sharing_top_level_e6);
}

TOP_LEVEL_EVENT __attribute__((weak)) UFMN(handle_other_passed_event)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);
	(void) pfsm;
	return top_level_pass_shared_event(pfsm, sharing_top_level_e7);
}

