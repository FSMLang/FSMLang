#include <stdio.h>
#include "another_priv.h"


ACTION_RETURN_TYPE anotherNewMachine_a1(FSM_TYPE_PTR pfsm)
{
	printf("%s\n", __func__);

	(void) pfsm;

	return THIS(noEvent);
}

ACTION_RETURN_TYPE anotherNewMachine_a2(FSM_TYPE_PTR pfsm)
{
	printf("%s\n", __func__);

	(void) pfsm;

	return THIS(noEvent);
}

void anotherNewMachine_baz(pANOTHER_NEW_MACHINE pfsm, ANOTHER_NEW_MACHINE_STATE s)
{
	printf("%s\n", __func__);

	printf("Transitioning to %s because of %s\n"
          , ANOTHER_NEW_MACHINE_STATE_NAMES[s]
          , ANOTHER_NEW_MACHINE_EVENT_NAMES[pfsm->event]
          );
}

ANOTHER_NEW_MACHINE_STATE anotherNewMachine_transitionFn(pANOTHER_NEW_MACHINE pfsm, ANOTHER_NEW_MACHINE_EVENT e)
{
	printf("%s\n", __func__);

	(void) pfsm;

	unsigned u_e = ((unsigned) e % (unsigned) THIS(numStates));

	return (ANOTHER_NEW_MACHINE_STATE) u_e;
}

ANOTHER_NEW_MACHINE_STATE anotherNewMachine_transitionFn1(pANOTHER_NEW_MACHINE pfsm, ANOTHER_NEW_MACHINE_EVENT e)
{
	printf("%s\n", __func__);

	(void) pfsm;

	unsigned u_e = ((unsigned) e % (unsigned) THIS(numStates));

	return (ANOTHER_NEW_MACHINE_STATE) u_e;
}

