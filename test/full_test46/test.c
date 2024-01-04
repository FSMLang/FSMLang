#include <stdio.h>
#include <string.h>

#include "top_level_priv.h"

NEW_MACHINE_EVENT nme;

// this is defined in tl-actions.c
extern void print_newMachine_data(pNEW_MACHINE_DATA);

int main()
{
	printf("Hello, world.\n");

	memset(&nme,0,sizeof(nme));

	print_newMachine_data(&pnewMachine->data);

	nme.event = THIS(e1);
	nme.event_data.e1_data.cp = "Hello, world.\n";

	run_newMachine(&nme);
	printf("\n");

	nme.event = THIS(e2);
	nme.event_data.e2_data.i = 2;
	nme.event_data.e2_data.f = 2.0;

	run_newMachine(&nme);
	printf("\n");

	nme.event = THIS(e1);
	nme.event_data.e1_data.cp = "Good-bye, world.\n";

	run_newMachine(&nme);
	printf("\n");

	nme.event = THIS(e2);
	nme.event_data.e2_data.i = 3;
	nme.event_data.e2_data.f = 3.0;

	run_newMachine(&nme);
	printf("\n");

	nme.event = THIS(e2);
	nme.event_data.e2_data.i = 3;
	nme.event_data.e2_data.f = 3.0;

	run_newMachine(&nme);
	printf("\n");

	nme.event = THIS(e3);
	nme.event_data.e3_data.i = 4;
	nme.event_data.e3_data.s.i = 4;
	nme.event_data.e3_data.s.f = 4.0;

	run_newMachine(&nme);
	printf("\n");

	nme.event = THIS(e4);
	run_newMachine(&nme);
	printf("\n");

	nme.event = THIS(e3);
	nme.event_data.e3_data.i = 5;
	nme.event_data.e3_data.s.i = 5;
	nme.event_data.e3_data.s.f = 5.0;

	run_newMachine(&nme);
	printf("\n");

	nme.event = THIS(e4);
	run_newMachine(&nme);
	printf("\n");

	nme.event = THIS(e3);
	nme.event_data.e3_data.i = 6;
	nme.event_data.e3_data.s.i = 6;
	nme.event_data.e3_data.s.f = 6.0;

	run_newMachine(&nme);
	printf("\n");

	nme.event = THIS(e4);
	run_newMachine(&nme);
	printf("\n");

	nme.event = THIS(eShared);
	run_newMachine(&nme);
	printf("\n");

	nme.event = THIS(e3);
	nme.event_data.e3_data.i = 7;
	nme.event_data.e3_data.s.i = 7;
	nme.event_data.e3_data.s.f = 7.0;

	run_newMachine(&nme);
	printf("\n");

	nme.event = THIS(eShared);
	run_newMachine(&nme);
	printf("\n");

	nme.event = THIS(e4);
	run_newMachine(&nme);
	printf("\n");

	nme.event = THIS(eShared);
	run_newMachine(&nme);
	printf("\n");

	nme.event = THIS(e3);
	nme.event_data.e3_data.i = 8;
	nme.event_data.e3_data.s.i = 8;
	nme.event_data.e3_data.s.f = 8.0;

	run_newMachine(&nme);
	printf("\n");

	nme.event = THIS(eShared);
	run_newMachine(&nme);
	printf("\n");

	nme.event = THIS(e4);
	run_newMachine(&nme);
	printf("\n");

	nme.event = THIS(eShared);
	run_newMachine(&nme);
	printf("\n");

	nme.event = THIS(e3);
	nme.event_data.e3_data.i = 9;
	nme.event_data.e3_data.s.i = 9;
	nme.event_data.e3_data.s.f = 9.0;

	run_newMachine(&nme);
	printf("\n");

	nme.event = THIS(eShared);
	run_newMachine(&nme);
	printf("\n");

	nme.event = THIS(e4);
	run_newMachine(&nme);
	printf("\n");

	nme.event = THIS(e3);
	nme.event_data.e3_data.i = 10;
	nme.event_data.e3_data.s.i = 10;
	nme.event_data.e3_data.s.f = 10.0;

	run_newMachine(&nme);
	printf("\n");

	nme.event = THIS(eShared);
	run_newMachine(&nme);
	printf("\n");

	nme.event = THIS(e4);
	run_newMachine(&nme);
	printf("\n");

	nme.event = THIS(eShared);
	run_newMachine(&nme);
	printf("\n");

	nme.event = THIS(e3);
	nme.event_data.e3_data.i = 11;
	nme.event_data.e3_data.s.i = 11;
	nme.event_data.e3_data.s.f = 11.0;

	run_newMachine(&nme);
	printf("\n");

	nme.event = THIS(e4);
	run_newMachine(&nme);
	printf("\n");

	nme.event = THIS(eShared);
	run_newMachine(&nme);
	printf("\n");

	return 0;
}

