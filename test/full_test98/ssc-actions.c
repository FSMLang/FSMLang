#include "sub_sub_compact_priv.h"

ACTION_RETURN_TYPE UFMN(a1)(FSM_TYPE_PTR pfsm)
{
	(void) pfsm;
	DBG_PRINTF(__func__);
	return THIS(noEvent);
}

ACTION_RETURN_TYPE UFMN(a2)(FSM_TYPE_PTR pfsm)
{
	(void) pfsm;
	DBG_PRINTF(__func__);
	return THIS(noEvent);
}

ACTION_RETURN_TYPE UFMN(i_am_empty)(FSM_TYPE_PTR pfsm)
{
	(void) pfsm;
	DBG_PRINTF(__func__);
	return THIS(noEvent);
}

void UFMN(no_transition)(FSM_TYPE_PTR pfsm, SUB_SUB_COMPACT_STATE dest)
{
	DBG_PRINTF(__func__);
	(void) pfsm;
	(void) dest;
}

void UFMN(on_transition)(FSM_TYPE_PTR pfsm, SUB_SUB_COMPACT_STATE s)
{
	DBG_PRINTF(__func__);
	DBG_PRINTF("from %s to %s"
              , SUB_SUB_COMPACT_STATE_NAMES[pfsm->state]
              , SUB_SUB_COMPACT_STATE_NAMES[s]
				 );
}

