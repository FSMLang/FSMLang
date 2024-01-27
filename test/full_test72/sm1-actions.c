#include <stdio.h>
#include "subMachine1_priv.h"

static void print_machine_data(pSUB_MACHINE1_DATA);

NEW_MACHINE_EVENT_ENUM  UFMN(aa1)(pSUB_MACHINE1 pfsm)
{
	DBG_PRINTF("%s", __func__);
	(void) pfsm;
	return THIS(noEvent);
}

NEW_MACHINE_EVENT_ENUM  UFMN(aa2)(pSUB_MACHINE1 pfsm)
{
	DBG_PRINTF("%s", __func__);
	(void) pfsm;
	return THIS(noEvent);
}

NEW_MACHINE_EVENT_ENUM  UFMN(noAction)(pSUB_MACHINE1 pfsm)
{
	DBG_PRINTF("%s", __func__);
	print_machine_data(&pfsm->data);
	return PARENT(noEvent);
}

void  UFMN(eShared_dt)(pNEW_MACHINE_DATA pfsm_data)
{
	DBG_PRINTF("%s", __func__);
	psubMachine1->data.cp = pfsm_data->u.bop;
	print_machine_data(&psubMachine1->data);
}


void  UFMN(foo)(pSUB_MACHINE1_DATA pdata)
{
	DBG_PRINTF("%s", __func__);
	print_machine_data(pdata);
}

void  UFMN(bar)(pSUB_MACHINE1_DATA pdata)
{
	DBG_PRINTF("%s", __func__);
	print_machine_data(pdata);
}

void  UFMN(onEntryTo_ss2)(pSUB_MACHINE1_DATA pdata)
{
	DBG_PRINTF("%s", __func__);
	print_machine_data(pdata);
}

void  UFMN(onExitFrom_ss2)(pSUB_MACHINE1_DATA pdata)
{
	DBG_PRINTF("%s", __func__);
	print_machine_data(pdata);
}

void  UFMN(foo1)(pSUB_MACHINE1_DATA pdata)
{
	DBG_PRINTF("%s", __func__);
	print_machine_data(pdata);
}

void  UFMN(bar1)(pSUB_MACHINE1_DATA pdata)
{
	DBG_PRINTF("%s", __func__);
	print_machine_data(pdata);
}

static void print_machine_data(pSUB_MACHINE1_DATA pmd)
{
	printf("sub_machine1.data.cp: %s\n"
          , pmd->cp
         );
}

