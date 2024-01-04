#include <unistd.h>

#include "top_level_priv.h"

ACTION_RETURN_TYPE THIS(activate_sub_machine1)(FSM_TYPE_PTR pfsm)
{
	(void) pfsm;
	DBG_PRINTF(__func__);
	return SUB_MACHINE1(e1);
}

ACTION_RETURN_TYPE THIS(activate_sub_machine2)(FSM_TYPE_PTR pfsm)
{
	(void) pfsm;
	DBG_PRINTF(__func__);
	return SUB_MACHINE2(e1);
}

ACTION_RETURN_TYPE THIS(activate_sub_machine3)(FSM_TYPE_PTR pfsm)
{
	(void) pfsm;
	DBG_PRINTF(__func__);
	return SUB_MACHINE3(e1);
}

ACTION_RETURN_TYPE THIS(a1)(FSM_TYPE_PTR pfsm)
{
	(void) pfsm;
	DBG_PRINTF(__func__);
	return THIS(noEvent);
}

