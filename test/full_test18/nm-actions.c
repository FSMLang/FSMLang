#include "full-test18_priv.h"

NEW_MACHINE_EVENT newMachine_a1(pNEW_MACHINE pfsm)
{
	(void) pfsm;
	printf("newMachine_a1\n");

	return newMachine_noEvent;
}

NEW_MACHINE_EVENT newMachine_a2(pNEW_MACHINE pfsm)
{
	(void) pfsm;
	printf("newMachine_a2n");

	return newMachine_noEvent;
}

NEW_MACHINE_EVENT newMachine_noAction(pNEW_MACHINE pfsm)
{
	(void) pfsm;
	printf("newMachine_noAction");

	return newMachine_noEvent;
}



void newMachine_baz(pNEW_MACHINE pfsm,NEW_MACHINE_STATE state)
{
	(void) pfsm;
	(void) state;
	printf("newMachine_baz");
}


TR_FN_RETURN_TYPE newMachine_transitionFn(pNEW_MACHINE pfsm,NEW_MACHINE_EVENT e)
{
	(void) pfsm;
	(void) e;
	printf("newMachine_transitionFn");

	DECLARE_TR_FN_RET_VAR(ret, s1);

	return ret;
}

TR_FN_RETURN_TYPE newMachine_transitionFn1(pNEW_MACHINE pfsm,NEW_MACHINE_EVENT e)
{
	(void) pfsm;
	(void) e;
	printf("newMachine_transitionFn1");

	DECLARE_TR_FN_RET_VAR(ret, s2);

	return ret;
}

TR_FN_RETURN_TYPE __attribute__((weak)) newMachine_transitionTos1(FSM_TYPE_PTR pfsm,NEW_MACHINE_EVENT e)
{
	(void) e;
	(void) pfsm;

	DECLARE_TR_FN_RET_VAR(ret, s1);
	return ret;
}

NEW_MACHINE_STATE UFMN(noTransitionFn)(pNEW_MACHINE pfsm, NEW_MACHINE_EVENT e)
{
	(void) e;
	DBG_PRINTF("%s\n", __func__);
	return pfsm->state;
}

int main()
{
	run_newMachine(newMachine_e1);
	run_newMachine(newMachine_subMachine1_ee1);

	return 0;
}
