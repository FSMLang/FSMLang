#include <stdio.h>
#include "parser-test13_priv.h"


ACTION_RETURN_TYPE THIS(a1)(FSM_TYPE_PTR pfsm)
{
	printf("%s\n", __func__);

	(void) pfsm;

	return THIS(noEvent);
}

ACTION_RETURN_TYPE THIS(a2)(FSM_TYPE_PTR pfsm)
{
	printf("%s\n", __func__);

	(void) pfsm;

	return THIS(noEvent);
}

void THIS(baz)(FSM_TYPE_PTR pfsm, NEW_MACHINE_STATE s)
{
	printf("%s\n", __func__);

	printf("Transitioning to %s because of %s\n"
          , NEW_MACHINE_STATE_NAMES[s]
          , NEW_MACHINE_EVENT_NAMES[pfsm->event]
          );
}

NEW_MACHINE_STATE THIS(transitionFn)(FSM_TYPE_PTR pfsm, NEW_MACHINE_EVENT e)
{
	printf("%s\n", __func__);

	(void) pfsm;

	unsigned u_e = ((unsigned) e % (unsigned) THIS(numStates));

	return (NEW_MACHINE_STATE) u_e;
}

NEW_MACHINE_STATE THIS(transitionFn1)(FSM_TYPE_PTR pfsm, NEW_MACHINE_EVENT e)
{
	printf("%s\n", __func__);

	(void) pfsm;

	unsigned u_e = ((unsigned) e % (unsigned) THIS(numStates));

	return (NEW_MACHINE_STATE) u_e;
}

