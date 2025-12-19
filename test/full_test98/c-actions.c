#include "compact_priv.h"

#if defined(FSM_VARIANT_CC)
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
	return compact_pass_shared_event(pfsm,sharing_compact_e2); 
}
#endif

ACTION_RETURN_TYPE UFMN(i_am_empty)(FSM_TYPE_PTR pfsm)
{
	(void) pfsm;
	DBG_PRINTF(__func__);
	return THIS(noEvent);
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

TR_FN_RETURN_TYPE compact_transition_to_s2(pCOMPACT pfsm, COMPACT_EVENT_ENUM e)
{
	DBG_PRINTF(__func__);

	(void) pfsm;
	(void) e;

	DECLARE_TR_FN_RET_VAR(ret, s2);
	return ret;
}

TR_FN_RETURN_TYPE compact_transitionTos1(pCOMPACT pfsm, COMPACT_EVENT_ENUM e)
{
	(void) pfsm;
	(void) e;

	DECLARE_TR_FN_RET_VAR(ret, s1);
	return ret;
}

#if defined(FSM_VARIANT_C) || defined(FSM_VARIANT_CC)
COMPACT_STATE compact_noTransitionFn(pCOMPACT pfsm, COMPACT_EVENT_ENUM e)
{
	(void) e;

	return pfsm->state;
}
#endif

void UFMN(no_transition)(FSM_TYPE_PTR pfsm, COMPACT_STATE dest)
{
	DBG_PRINTF(__func__);
	(void) pfsm;
	(void) dest;
}

void UFMN(on_transition)(FSM_TYPE_PTR pfsm, COMPACT_STATE s)
{
	DBG_PRINTF(__func__);
	DBG_PRINTF("from %s to %s"
              , COMPACT_STATE_NAMES[pfsm->state]
              , COMPACT_STATE_NAMES[s]
				 );
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

