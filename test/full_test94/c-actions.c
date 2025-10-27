#include "compact_priv.h"

ACTION_RETURN_TYPE UFMN(a1)(FSM_TYPE_PTR pfsm)
{
	(void) pfsm;
	DBG_PRINTF(__func__);
	return compact_pass_shared_event(pfsm, sharing_compact_e1); 
}

ACTION_RETURN_TYPE UFMN(a2)(FSM_TYPE_PTR pfsm)
{
	(void) pfsm;
	DBG_PRINTF(__func__);
	return compact_pass_shared_event(pfsm, sharing_compact_e1); 
}

ACTION_RETURN_TYPE UFMN(a3)(FSM_TYPE_PTR pfsm)
{
	(void) pfsm;
	DBG_PRINTF(__func__);
	return THIS(e4);
}

ACTION_RETURN_TYPE UFMN(noAction)(FSM_TYPE_PTR pfsm)
{
	(void) pfsm;
	DBG_PRINTF(__func__);
	return THIS(noEvent);
}

COMPACT_STATE UFMN(transition_to_s2)(pCOMPACT pfsm,COMPACT_EVENT e)
{
	(void) pfsm;
	(void) e;
	DBG_PRINTF(__func__);
	return STATE(s2);
}

COMPACT_STATE UFMN(transitionTos1)(pCOMPACT pfsm,COMPACT_EVENT e)
{
	(void) pfsm;
	(void) e;
	DBG_PRINTF(__func__);
	return STATE(s1);
}

COMPACT_STATE UFMN(noTransitionFn)(pCOMPACT pfsm,COMPACT_EVENT e)
{
	(void) e;
	DBG_PRINTF(__func__);
	return pfsm->state;
}

int main(void)
{
	run_compact(THIS(e1));
	run_compact(THIS(e2));
	run_compact(THIS(e3));

	return 0;
}

