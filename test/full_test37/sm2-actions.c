#include "sub_machine2.h"

ACTION_RETURN_TYPE THIS(a3)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF(__func__);

   return PARENT(e4);
}

ACTION_RETURN_TYPE THIS(a2)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF(__func__);

   return THIS(e3);
}

ACTION_RETURN_TYPE THIS(a1)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF(__func__);

   return THIS(e2);
}

ACTION_RETURN_TYPE THIS(noAction)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF(__func__);

   return THIS(noEvent);
}

