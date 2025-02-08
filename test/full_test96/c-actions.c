#include "compact_priv.h"

ACTION_RETURN_TYPE UFMN(a1)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF(__func__);
	return compact_pass_shared_event(pfsm,sharing_compact_e1); 
}

ACTION_RETURN_TYPE UFMN(a2)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF(__func__);
	return compact_pass_shared_event(pfsm,sharing_compact_e1); 
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

void compact_translate_e1_data(pCOMPACT_DATA pfsm_data,pCOMPACT_E1_DATA pe_data)
{
	pfsm_data->e1_payload = pe_data->payload;
}

COMPACT_STATE compact_transition_to_s2(pCOMPACT pfsm, COMPACT_EVENT_ENUM e)
{
	DBG_PRINTF(__func__);

	(void) pfsm;
	(void) e;

	return STATE(s2);
}

COMPACT_STATE compact_transitionTos1(pCOMPACT pfsm, COMPACT_EVENT_ENUM e)
{
	DBG_PRINTF(__func__);

	(void) pfsm;
	(void) e;

	return STATE(s1);
}

COMPACT_STATE compact_noTransitionFn(pCOMPACT pfsm, COMPACT_EVENT_ENUM e)
{
	DBG_PRINTF(__func__);

	(void) e;

	return pfsm->state;
}


int main(void)
{
	COMPACT_EVENT event;

	event.event_data.e1_data.payload = 1;
	event.event = THIS(e1);
	run_compact(&event);

	event.event = THIS(e2);
	run_compact(&event);

	event.event = THIS(e3);
	run_compact(&event);

	return 0;
}

