#include "sub_compact_priv.h"

ACTION_RETURN_TYPE UFMN(a1)(FSM_TYPE_PTR pfsm)
{
	(void) pfsm;
	DBG_PRINTF(__func__);
	return sub_compact_pass_shared_event(pfsm, sharing_sub_compact_e1); 
}

ACTION_RETURN_TYPE UFMN(a2)(FSM_TYPE_PTR pfsm)
{
	(void) pfsm;
	DBG_PRINTF(__func__);
	return sub_compact_pass_shared_event(pfsm, sharing_sub_compact_e1); 
}

ACTION_RETURN_TYPE UFMN(noAction)(FSM_TYPE_PTR pfsm)
{
	(void) pfsm;
	DBG_PRINTF(__func__);
	return THIS(noEvent);
}

