#include <stdio.h>
#include <string.h>

#include "top_level.h"

NEW_MACHINE_EVENT nme;

static void print_newMachine_data(pNEW_MACHINE_DATA);

int main(int argc, char **argv)
{
	printf("Hello, world.\n");

	memset(&nme,0,sizeof(nme));

	print_newMachine_data(&pnewMachine->data);

	nme.event = THIS(e1);
	nme.event_data.e1_data.cp = "Hello, world.\n";

	RUN_STATE_MACHINE(pnewMachine, &nme);
	printf("\n");

	nme.event = THIS(e2);
	nme.event_data.e2_data.i = 2;
	nme.event_data.e2_data.f = 2.0;

	RUN_STATE_MACHINE(pnewMachine, &nme);
	printf("\n");

	nme.event = THIS(e1);
	nme.event_data.e1_data.cp = "Good-bye, world.\n";

	RUN_STATE_MACHINE(pnewMachine, &nme);
	printf("\n");

	nme.event = THIS(e2);
	nme.event_data.e2_data.i = 3;
	nme.event_data.e2_data.f = 3.0;

	RUN_STATE_MACHINE(pnewMachine, &nme);
	printf("\n");

	nme.event = THIS(e2);
	nme.event_data.e2_data.i = 3;
	nme.event_data.e2_data.f = 3.0;

	RUN_STATE_MACHINE(pnewMachine, &nme);
	printf("\n");

	nme.event = THIS(e3);
	nme.event_data.e3_data.i = 4;
	nme.event_data.e3_data.s.i = 4;
	nme.event_data.e3_data.s.f = 4.0;

	RUN_STATE_MACHINE(pnewMachine, &nme);
	printf("\n");

	nme.event = THIS(e4);
	RUN_STATE_MACHINE(pnewMachine, &nme);
	printf("\n");

	nme.event = THIS(e3);
	nme.event_data.e3_data.i = 5;
	nme.event_data.e3_data.s.i = 5;
	nme.event_data.e3_data.s.f = 5.0;

	RUN_STATE_MACHINE(pnewMachine, &nme);
	printf("\n");

	nme.event = THIS(e4);
	RUN_STATE_MACHINE(pnewMachine, &nme);
	printf("\n");

	nme.event = THIS(e3);
	nme.event_data.e3_data.i = 6;
	nme.event_data.e3_data.s.i = 6;
	nme.event_data.e3_data.s.f = 6.0;

	RUN_STATE_MACHINE(pnewMachine, &nme);
	printf("\n");

	nme.event = THIS(e4);
	RUN_STATE_MACHINE(pnewMachine, &nme);
	printf("\n");

	nme.event = THIS(e3);
	nme.event_data.e3_data.i = 7;
	nme.event_data.e3_data.s.i = 7;
	nme.event_data.e3_data.s.f = 7.0;

	RUN_STATE_MACHINE(pnewMachine, &nme);
	printf("\n");

	nme.event = THIS(e4);
	RUN_STATE_MACHINE(pnewMachine, &nme);
	printf("\n");

	nme.event = THIS(e3);
	nme.event_data.e3_data.i = 8;
	nme.event_data.e3_data.s.i = 8;
	nme.event_data.e3_data.s.f = 8.0;

	RUN_STATE_MACHINE(pnewMachine, &nme);
	printf("\n");

	nme.event = THIS(e4);
	RUN_STATE_MACHINE(pnewMachine, &nme);
	printf("\n");

	nme.event = THIS(e3);
	nme.event_data.e3_data.i = 9;
	nme.event_data.e3_data.s.i = 9;
	nme.event_data.e3_data.s.f = 9.0;

	RUN_STATE_MACHINE(pnewMachine, &nme);
	printf("\n");

	nme.event = THIS(e4);
	RUN_STATE_MACHINE(pnewMachine, &nme);
	printf("\n");

	nme.event = THIS(e3);
	nme.event_data.e3_data.i = 10;
	nme.event_data.e3_data.s.i = 10;
	nme.event_data.e3_data.s.f = 10.0;

	RUN_STATE_MACHINE(pnewMachine, &nme);
	printf("\n");

	nme.event = THIS(e4);
	RUN_STATE_MACHINE(pnewMachine, &nme);
	printf("\n");

	nme.event = THIS(e3);
	nme.event_data.e3_data.i = 11;
	nme.event_data.e3_data.s.i = 11;
	nme.event_data.e3_data.s.f = 11.0;

	RUN_STATE_MACHINE(pnewMachine, &nme);
	printf("\n");

	nme.event = THIS(e4);
	RUN_STATE_MACHINE(pnewMachine, &nme);
	printf("\n");

	return 0;
}

void newMachine_translate_e1_data(pNEW_MACHINE_DATA pfsm_data, pNEW_MACHINE_E1_DATA pevent_data)
{
	printf("translate_e1_data\n");

	strncpy(pfsm_data->u.bop,pevent_data->cp,NUM_CHARS);
	pfsm_data->u.bop[NUM_CHARS-1] = 0;
}

void newMachine_translate_e2_data(pNEW_MACHINE_DATA pfsm_data, pNEW_MACHINE_E2_DATA pevent_data)
{
	printf("translate_e2_data\n");

	pfsm_data->u.foo.a[pfsm_data->u.foo.count_ints++ % NUM_INTS] = pevent_data->i;
	pfsm_data->u.beep.f = pevent_data->f;
}

void newMachine_translate_e3_data(pNEW_MACHINE_DATA pfsm_data, pNEW_MACHINE_E3_DATA pevent_data)
{
	printf("translate_e3_data\n");

	pfsm_data->u.foo.a[pfsm_data->u.foo.count_ints++ % NUM_INTS] = pevent_data->s.i;
	pfsm_data->u.beep.f = pevent_data->s.f;

	pfsm_data->u.e3_int = pevent_data->i;
}

NEW_MACHINE_STATE newMachine_transitionFn(pNEW_MACHINE pfsm, ACTION_RETURN_TYPE e)
{
	(void) e;

	return pfsm->data.u.foo.count_ints < NUM_INTS ?  THIS(s1) : THIS(s2);
}

NEW_MACHINE_STATE newMachine_transitionFn1(pNEW_MACHINE pfsm, ACTION_RETURN_TYPE e)
{
	(void) e;
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
	printf("Event %s caused transition from %s to %s\n"
          , NEW_MACHINE_EVENT_NAMES[pfsm->event]
          , NEW_MACHINE_STATE_NAMES[pfsm->state]
          , NEW_MACHINE_STATE_NAMES[s]
         );
}

NEW_MACHINE_EVENT_ENUM newMachine_a1(pNEW_MACHINE pfsm)
{
	printf("newMachine_a1\n");

	print_newMachine_data(&pfsm->data);

	return THIS(noEvent);
} 

NEW_MACHINE_EVENT_ENUM newMachine_a2(pNEW_MACHINE pfsm)
{
	printf("newMachine_a2\n");

	print_newMachine_data(&pfsm->data);

	return THIS(noEvent);
}

NEW_MACHINE_EVENT_ENUM newMachine_doNothing(pNEW_MACHINE pfsm)
{
	printf("newMachine_doNothing\n");

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

static void print_newMachine_data(pNEW_MACHINE_DATA pnmd)
{
	int int_to_print = pnmd->u.foo.count_ints;

	if (int_to_print > 0)
		int_to_print--;

	int_to_print %= NUM_INTS;

	printf("machine.data.u.bop: %s\n"
          , pnmd->u.bop[0] ? pnmd->u.bop : "<EMPTY>"
         );

	printf("newMachine data.beep.f: %0.2f\n"
          , pnmd->u.beep.f
         );

	printf("newMachine.data.u.foo.count_ints: %d\nnewMachine.data.u.foo.a[%d]: %d\n"
           , pnmd->u.foo.count_ints
           , int_to_print
           , pnmd->u.foo.a[int_to_print]
          );

	printf("\n\n");
}

