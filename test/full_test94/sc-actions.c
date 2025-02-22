#include "sub_compact_priv.h"

ACTION_RETURN_TYPE UFMN(a1)(FSM_TYPE_PTR pfsm)
{
	(void) pfsm;
	DBG_PRINTF(__func__);
	return sub_compact_pass_shared_event(sharing_sub_compact_e1); 
}

ACTION_RETURN_TYPE UFMN(a2)(FSM_TYPE_PTR pfsm)
{
	(void) pfsm;
	DBG_PRINTF(__func__);
	return sub_compact_pass_shared_event(sharing_sub_compact_e1); 
}

ACTION_RETURN_TYPE UFMN(noAction)(FSM_TYPE_PTR pfsm)
{
	(void) pfsm;
	DBG_PRINTF(__func__);
	return THIS(noEvent);
}

SUB_COMPACT_STATE UFMN(transition_to_s2)(FSM_TYPE_PTR pfsm,COMPACT_EVENT e)
{
	(void) pfsm;
	(void) e;
	DBG_PRINTF(__func__);
	return STATE(s2);
}

SUB_COMPACT_STATE UFMN(transitionTos1)(FSM_TYPE_PTR pfsm,COMPACT_EVENT e)
{
	(void) pfsm;
	(void) e;
	DBG_PRINTF(__func__);
	return STATE(s1);
}

SUB_COMPACT_STATE UFMN(noTransitionFn)(FSM_TYPE_PTR pfsm,COMPACT_EVENT e)
{
	(void) e;
	DBG_PRINTF(__func__);
	return pfsm->state;
}

