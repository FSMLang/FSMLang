#include <stdio.h>
#include "subMachine1.h"

static void print_machine_data(pSUB_MACHINE1_DATA);

NEW_MACHINE_EVENT_ENUM  subMachine1_aa1(pSUB_MACHINE1 pfsm)
{
	(void) pfsm;
	DBG_PRINTF("subMachine1_aa1");
	return THIS(noEvent);
}

NEW_MACHINE_EVENT_ENUM  subMachine1_aa2(pSUB_MACHINE1 pfsm)
{
	(void) pfsm;
	DBG_PRINTF("subMachine1_aa2");
	return THIS(noEvent);
}

NEW_MACHINE_EVENT_ENUM  subMachine1_noAction(pSUB_MACHINE1 pfsm)
{
	DBG_PRINTF("subMachine1_noAction");
	print_machine_data(&pfsm->data);
	return PARENT(noEvent);
}

void  subMachine1_eShared_dt(pNEW_MACHINE pfsm)
{
	DBG_PRINTF("subMachine1_eShared_dt");
	psubMachine1->data.cp = pfsm->data.u.bop;
	print_machine_data(&psubMachine1->data);
}


void  subMachine1_foo(pSUB_MACHINE1_DATA pdata)
{
	DBG_PRINTF("subMachine1_foo");
	print_machine_data(pdata);
}

void  subMachine1_bar(pSUB_MACHINE1_DATA pdata)
{
	DBG_PRINTF("subMachine1_bar");
	print_machine_data(pdata);
}

void  subMachine1_onEntryTo_ss2(pSUB_MACHINE1_DATA pdata)
{
	DBG_PRINTF("subMachine1_onEntryTo_ss2");
	print_machine_data(pdata);
}

void  subMachine1_onExitFrom_ss2(pSUB_MACHINE1_DATA pdata)
{
	DBG_PRINTF("subMachine1_onExitFrom_ss2");
	print_machine_data(pdata);
}

void  subMachine1_foo1(pSUB_MACHINE1_DATA pdata)
{
	DBG_PRINTF("subMachine1_foo1");
	print_machine_data(pdata);
}

void  subMachine1_bar1(pSUB_MACHINE1_DATA pdata)
{
	DBG_PRINTF("subMachine1_bar1");
	print_machine_data(pdata);
}

static void print_machine_data(pSUB_MACHINE1_DATA pmd)
{
	printf("sub_machine1.data.cp: %s\n"
          , pmd->cp
         );
}

