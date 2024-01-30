#include <stdio.h>
#include "parser-test13_priv.h"


ACTION_RETURN_TYPE UFMN(a1)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);

	(void) pfsm;

	return THIS(noEvent);
}

ACTION_RETURN_TYPE UFMN(a2)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);

	(void) pfsm;

	return THIS(noEvent);
}

void UFMN(baz)(FSM_TYPE_PTR pfsm, NEW_MACHINE_STATE s)
{
	DBG_PRINTF("%s\nTransitioning to %s because of %s"
			 , __func__
          , NEW_MACHINE_STATE_NAMES[s]
          , NEW_MACHINE_EVENT_NAMES[pfsm->event]
          );
}

NEW_MACHINE_STATE UFMN(transitionFn)(FSM_TYPE_PTR pfsm, NEW_MACHINE_EVENT e)
{
	DBG_PRINTF("%s", __func__);

	(void) pfsm;

	unsigned u_e = ((unsigned) e % (unsigned) THIS(numStates));

	return (NEW_MACHINE_STATE) u_e;
}

NEW_MACHINE_STATE UFMN(transitionFn1)(FSM_TYPE_PTR pfsm, NEW_MACHINE_EVENT e)
{
	DBG_PRINTF("%s", __func__);

	(void) pfsm;

	unsigned u_e = ((unsigned) e % (unsigned) THIS(numStates));

	return (NEW_MACHINE_STATE) u_e;
}

