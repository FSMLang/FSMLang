#include "sub_compact_priv.h"

#if defined(FSM_VARIANT_CC)
ACTION_RETURN_TYPE UFMN(a1)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF(__func__);
	return sub_compact_pass_shared_event(pfsm, sharing_sub_compact_e1); 
}

ACTION_RETURN_TYPE UFMN(a2)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF(__func__);
	return sub_compact_pass_shared_event(pfsm, sharing_sub_compact_e2); 
}
#endif

ACTION_RETURN_TYPE UFMN(i_am_empty)(FSM_TYPE_PTR pfsm)
{
	(void) pfsm;
	DBG_PRINTF(__func__);
	return THIS(noEvent);
}

void compact_sub_compact_grab_e1_data(pCOMPACT_DATA pdata)
{
	DBG_PRINTF(__func__);
	psub_compact->data.e1_payload = pdata->e1_payload;
}

void UFMN(no_transition)(FSM_TYPE_PTR pfsm, SUB_COMPACT_STATE dest)
{
	DBG_PRINTF(__func__);
	(void) pfsm;
	(void) dest;
}

void UFMN(on_transition)(FSM_TYPE_PTR pfsm, SUB_COMPACT_STATE s)
{
	DBG_PRINTF(__func__);
	DBG_PRINTF("from %s to %s"
              , SUB_COMPACT_STATE_NAMES[pfsm->state]
              , SUB_COMPACT_STATE_NAMES[s]
				 );
}

