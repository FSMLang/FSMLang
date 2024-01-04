#include <stdio.h>
#include <string.h>

#include "top_level_priv.h"

NEW_MACHINE_EVENT nme;

// this is used by tl-actions.c as well
void print_newMachine_data(pNEW_MACHINE_DATA);

int main()
{
	printf("Hello, world.\n");

	memset(&nme,0,sizeof(nme));

	print_newMachine_data(&pnewMachine->data);

	nme.event = THIS(e1);
	nme.event_data.e1_data.cp = "Hello, world.\n";

	printf("marker 1\n");
	run_newMachine(&nme);
	printf("\n");

	nme.event = THIS(e2);
	nme.event_data.e2_data.i = 2;
	nme.event_data.e2_data.f = 2.0;

	printf("marker 2\n");
	run_newMachine(&nme);
	printf("\n");

	nme.event = THIS(e1);
	nme.event_data.e1_data.cp = "Good-bye, world.\n";

	printf("marker 3\n");
	run_newMachine(&nme);
	printf("\n");

	nme.event = THIS(e2);
	nme.event_data.e2_data.i = 3;
	nme.event_data.e2_data.f = 3.0;

	printf("marker 4\n");
	run_newMachine(&nme);
	printf("\n");

	nme.event = THIS(e2);
	nme.event_data.e2_data.i = 3;
	nme.event_data.e2_data.f = 3.0;

	printf("marker 5\n");
	run_newMachine(&nme);
	printf("\n");

	nme.event = THIS(e3);
	nme.event_data.e3_data.i = 4;
	nme.event_data.e3_data.s.i = 4;
	nme.event_data.e3_data.s.f = 4.0;

	printf("marker 6\n");
	run_newMachine(&nme);
	printf("\n");

	nme.event = THIS(e4);
	printf("marker 7\n");
	run_newMachine(&nme);
	printf("\n");

	nme.event = THIS(e3);
	nme.event_data.e3_data.i = 5;
	nme.event_data.e3_data.s.i = 5;
	nme.event_data.e3_data.s.f = 5.0;

	printf("marker 8\n");
	run_newMachine(&nme);
	printf("\n");

	nme.event = THIS(e4);
	printf("marker 9\n");
	run_newMachine(&nme);
	printf("\n");

	nme.event = THIS(e3);
	nme.event_data.e3_data.i = 6;
	nme.event_data.e3_data.s.i = 6;
	nme.event_data.e3_data.s.f = 6.0;

	printf("marker 10\n");
	run_newMachine(&nme);
	printf("\n");

	nme.event = THIS(e4);
	printf("marker 11\n");
	run_newMachine(&nme);
	printf("\n");

	nme.event = THIS(eShared);
	printf("marker 12\n");
	run_newMachine(&nme);
	printf("\n");

	nme.event = THIS(e3);
	nme.event_data.e3_data.i = 7;
	nme.event_data.e3_data.s.i = 7;
	nme.event_data.e3_data.s.f = 7.0;

	printf("marker 13\n");
	run_newMachine(&nme);
	printf("\n");

	nme.event = THIS(eShared);
	printf("marker 14\n");
	run_newMachine(&nme);
	printf("\n");

	nme.event = THIS(e4);
	run_newMachine(&nme);
	printf("\n");

	nme.event = THIS(eShared);
	printf("marker 15\n");
	run_newMachine(&nme);
	printf("\n");

	nme.event = THIS(e3);
	nme.event_data.e3_data.i = 8;
	nme.event_data.e3_data.s.i = 8;
	nme.event_data.e3_data.s.f = 8.0;

	printf("marker 16\n");
	run_newMachine(&nme);
	printf("\n");

	nme.event = THIS(eShared);
	printf("marker 17\n");
	run_newMachine(&nme);
	printf("\n");

	nme.event = THIS(e4);
	printf("marker 18\n");
	run_newMachine(&nme);
	printf("\n");

	nme.event = THIS(eShared);
	printf("marker 19\n");
	run_newMachine(&nme);
	printf("\n");

	nme.event = THIS(e3);
	nme.event_data.e3_data.i = 9;
	nme.event_data.e3_data.s.i = 9;
	nme.event_data.e3_data.s.f = 9.0;

	printf("marker 20\n");
	run_newMachine(&nme);
	printf("\n");

	nme.event = THIS(eShared);
	printf("marker 21\n");
	run_newMachine(&nme);
	printf("\n");

	nme.event = THIS(e4);
	printf("marker 22\n");
	run_newMachine(&nme);
	printf("\n");

	nme.event = THIS(e3);
	nme.event_data.e3_data.i = 10;
	nme.event_data.e3_data.s.i = 10;
	nme.event_data.e3_data.s.f = 10.0;

	printf("marker 23\n");
	run_newMachine(&nme);
	printf("\n");

	nme.event = THIS(eShared);
	printf("marker 24\n");
	run_newMachine(&nme);
	printf("\n");

	nme.event = THIS(e4);
	printf("marker 25\n");
	run_newMachine(&nme);
	printf("\n");

	nme.event = THIS(eShared);
	printf("marker 26\n");
	run_newMachine(&nme);
	printf("\n");

	nme.event = THIS(e3);
	nme.event_data.e3_data.i = 11;
	nme.event_data.e3_data.s.i = 11;
	nme.event_data.e3_data.s.f = 11.0;

	printf("marker 27\n");
	run_newMachine(&nme);
	printf("\n");

	nme.event = THIS(e4);
	printf("marker 28\n");
	run_newMachine(&nme);
	printf("\n");

	nme.event = THIS(eShared);
	printf("marker 29\n");
	run_newMachine(&nme);
	printf("\n");

	return 0;
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

