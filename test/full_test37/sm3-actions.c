#include "sub_machine3_priv.h"

ACTION_RETURN_TYPE THIS(a3)(FSM_TYPE_PTR pfsm)
{
	(void) pfsm;
	DBG_PRINTF(__func__);

   return PARENT(noEvent);
}

ACTION_RETURN_TYPE THIS(a2)(FSM_TYPE_PTR pfsm)
{
	(void) pfsm;
	DBG_PRINTF(__func__);

   return THIS(e3);
}

ACTION_RETURN_TYPE THIS(a1)(FSM_TYPE_PTR pfsm)
{
	(void) pfsm;
	DBG_PRINTF(__func__);

   return THIS(e2);
}

ACTION_RETURN_TYPE THIS(noAction)(FSM_TYPE_PTR pfsm)
{
	(void) pfsm;
	DBG_PRINTF(__func__);

   return THIS(noEvent);
}

ACTION_RETURN_TYPE THIS(handle_e7)(FSM_TYPE_PTR pfsm)
{
	(void) pfsm;
	DBG_PRINTF(__func__);
	return THIS(noEvent);
}

