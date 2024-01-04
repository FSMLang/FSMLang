#include <stdio.h>
#include "subMachine1_priv.h"

static void print_machine_data(pSUB_MACHINE1_DATA);

NEW_MACHINE_EVENT_ENUM  THIS(aa1)(pSUB_MACHINE1 pfsm)
{
	(void) pfsm;
	DBG_PRINTF("%s", __func__);
	return THIS(noEvent);
}

NEW_MACHINE_EVENT_ENUM  THIS(aa2)(pSUB_MACHINE1 pfsm)
{
	(void) pfsm;
	DBG_PRINTF("%s", __func__);
	return THIS(noEvent);
}

NEW_MACHINE_EVENT_ENUM  THIS(noAction)(pSUB_MACHINE1 pfsm)
{
	DBG_PRINTF("%s", __func__);
	print_machine_data(&pfsm->data);
	return PARENT(noEvent);
}

void  THIS(eShared_dt)(pNEW_MACHINE_DATA pfsm_data)
{
	DBG_PRINTF("%s", __func__);
	psubMachine1->data.cp = pfsm_data->u.bop;
	print_machine_data(&psubMachine1->data);
}


void  THIS(foo)(pSUB_MACHINE1_DATA pdata)
{
	DBG_PRINTF("%s", __func__);
	print_machine_data(pdata);
}

void  THIS(bar)(pSUB_MACHINE1_DATA pdata)
{
	DBG_PRINTF("%s", __func__);
	print_machine_data(pdata);
}

void  THIS(onEntryTo_ss2)(pSUB_MACHINE1_DATA pdata)
{
	DBG_PRINTF("%s", __func__);
	print_machine_data(pdata);
}

void  THIS(onExitFrom_ss2)(pSUB_MACHINE1_DATA pdata)
{
	DBG_PRINTF("%s", __func__);
	print_machine_data(pdata);
}

void  THIS(foo1)(pSUB_MACHINE1_DATA pdata)
{
	DBG_PRINTF("%s", __func__);
	print_machine_data(pdata);
}

void  THIS(bar1)(pSUB_MACHINE1_DATA pdata)
{
	DBG_PRINTF("%s", __func__);
	print_machine_data(pdata);
}

static void print_machine_data(pSUB_MACHINE1_DATA pmd)
{
	DBG_PRINTF("sub_machine1.data.cp: %s"
          , pmd->cp
         );
}

