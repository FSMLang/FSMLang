#include <unistd.h>

#include "top_level_priv.h"

ACTION_RETURN_TYPE UFMN(activate_sub_machine1)(FSM_TYPE_PTR pfsm)
{
	(void) pfsm;
	DBG_PRINTF(__func__);
	return SUB_MACHINE1(e1);
}

ACTION_RETURN_TYPE UFMN(activate_sub_machine2)(FSM_TYPE_PTR pfsm)
{
	(void) pfsm;
	DBG_PRINTF(__func__);
	return SUB_MACHINE2(e1);
}

ACTION_RETURN_TYPE UFMN(activate_sub_machine3)(FSM_TYPE_PTR pfsm)
{
	(void) pfsm;
	DBG_PRINTF(__func__);
	return SUB_MACHINE3(e1);
}

ACTION_RETURN_TYPE UFMN(a1)(FSM_TYPE_PTR pfsm)
{
	(void) pfsm;
	DBG_PRINTF(__func__);
	return THIS(noEvent);
}

TOP_LEVEL_EVENT __attribute__((weak)) UFMN(noAction)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);
	(void) pfsm;
	return THIS(noEvent);
}

TOP_LEVEL_EVENT __attribute__((weak)) UFMN(handle_dispatch)(pTOP_LEVEL pfsm)
{
	DBG_PRINTF("%s", __func__);
	return top_level_pass_shared_event(pfsm, sharing_top_level_e6);
}

TOP_LEVEL_EVENT __attribute__((weak)) UFMN(handle_other_passed_event)(pTOP_LEVEL pfsm)
{
	DBG_PRINTF("%s", __func__);
	return top_level_pass_shared_event(pfsm, sharing_top_level_e7);
}

