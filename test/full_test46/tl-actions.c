#include <string.h>

#include "top_level_priv.h"

// this is also used in test.c
void print_newMachine_data(pNEW_MACHINE_DATA);

void newMachine_translate_e1_data(pNEW_MACHINE_DATA pfsm_data, pNEW_MACHINE_E1_DATA pevent_data)
{
	DBG_PRINTF("%s\n", __func__);

	strncpy(pfsm_data->u.bop,pevent_data->cp,NUM_CHARS);
	pfsm_data->u.bop[NUM_CHARS-1] = 0;
}

void newMachine_translate_e2_data(pNEW_MACHINE_DATA pfsm_data, pNEW_MACHINE_E2_DATA pevent_data)
{
	DBG_PRINTF("%s\n", __func__);

	pfsm_data->u.foo.a[pfsm_data->u.foo.count_ints++ % NUM_INTS] = pevent_data->i;
	pfsm_data->u.beep.f = pevent_data->f;
}

void newMachine_translate_e3_data(pNEW_MACHINE_DATA pfsm_data, pNEW_MACHINE_E3_DATA pevent_data)
{
	DBG_PRINTF("%s\n", __func__);

	pfsm_data->u.foo.a[pfsm_data->u.foo.count_ints++ % NUM_INTS] = pevent_data->s.i;
	pfsm_data->u.beep.f = pevent_data->s.f;

	pfsm_data->u.e3_int = pevent_data->i;
}

NEW_MACHINE_STATE newMachine_transitionFn(pNEW_MACHINE pfsm, ACTION_RETURN_TYPE e)
{
	(void) e;
	DBG_PRINTF("%s\n", __func__);
	return pfsm->data.u.foo.count_ints < NUM_INTS ?  THIS(s1) : THIS(s2);
}

NEW_MACHINE_STATE newMachine_transitionFn1(pNEW_MACHINE pfsm, ACTION_RETURN_TYPE e)
{
	(void) e;
	DBG_PRINTF("%s\n", __func__);
	return pfsm->data.u.beep.f > 0.0f ? THIS(s3) : THIS(s1);
}

NEW_MACHINE_STATE newMachine_transitionTos1(pNEW_MACHINE pfsm, ACTION_RETURN_TYPE e)
{
	(void) pfsm;
	(void) e;
	return THIS(s1);
}

void newMachine_baz(pNEW_MACHINE pfsm, NEW_MACHINE_STATE s)
{
	DBG_PRINTF("%s: event %s caused transition from %s to %s\n"
			 , __func__
          , NEW_MACHINE_EVENT_NAMES[pfsm->event]
          , NEW_MACHINE_STATE_NAMES[pfsm->state]
          , NEW_MACHINE_STATE_NAMES[s]
         );
}

NEW_MACHINE_EVENT_ENUM newMachine_a1(pNEW_MACHINE pfsm)
{
	DBG_PRINTF("%s\n", __func__);

	print_newMachine_data(&pfsm->data);

	return THIS(noEvent);
} 

NEW_MACHINE_EVENT_ENUM newMachine_a2(pNEW_MACHINE pfsm)
{
	DBG_PRINTF("%s\n", __func__);

	print_newMachine_data(&pfsm->data);

	return THIS(noEvent);
}

NEW_MACHINE_EVENT_ENUM newMachine_doNothing(pNEW_MACHINE pfsm)
{
	DBG_PRINTF("%s\n", __func__);

	print_newMachine_data(&pfsm->data);

	switch (pfsm->data.u.foo.count_ints % 5)
	{
		case 0:  return SUB_MACHINE1(ee2);
		case 1:  return SUB_MACHINE1(ee3);
		case 2:  return SUB_MACHINE2(eee1);
		case 3:  return SUB_MACHINE2(eee2);
		case 4:  return SUB_MACHINE2(eee3);
		default: return THIS(noEvent);
	}
}

void print_newMachine_data(pNEW_MACHINE_DATA pnmd)
{
	int int_to_print = pnmd->u.foo.count_ints;

	if (int_to_print > 0)
		int_to_print--;

	int_to_print %= NUM_INTS;

	printf("machine.data.u.bop: %s\n"
          , pnmd->u.bop[0] ? pnmd->u.bop : "<EMPTY>"
         );

	printf("newMachine.data.beep.f: %0.2f\n"
          , pnmd->u.beep.f
         );

	printf("newMachine.data.u.foo.count_ints: %d\nnewMachine.data.u.foo.a[%d]: %d\n"
           , pnmd->u.foo.count_ints
           , int_to_print
           , pnmd->u.foo.a[int_to_print]
          );

	printf("newMachine.data.u.e3_int: %d\n"
          , pnmd->u.e3_int
         );

	printf("\n\n");
}

NEW_MACHINE_STATE __attribute__((weak)) newMachine_transitionTos4(FSM_TYPE_PTR pfsm,NEW_MACHINE_EVENT_ENUM e)
{
	(void) e;
	(void) pfsm;

	return newMachine_s4;
}

NEW_MACHINE_STATE __attribute__((weak)) UFMN(noTransitionFn)(pNEW_MACHINE pfsm, NEW_MACHINE_EVENT_ENUM e)
{
	(void) e;
	return pfsm->state;
}

NEW_MACHINE_EVENT_ENUM __attribute__((weak)) UFMN(noAction)(FSM_TYPE_PTR pfsm)
{
	(void) pfsm;
	DBG_PRINTF("%s", __func__);
	return THIS(noEvent);
}

NEW_MACHINE_EVENT_ENUM __attribute__((weak)) UFMN(shareSharedEvent)(pNEW_MACHINE pfsm)
{
	DBG_PRINTF("%s", __func__);
	return newMachine_pass_shared_event(pfsm, sharing_newMachine_eShared);
}

