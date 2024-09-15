#include <stdio.h>
#include "subMachine1_priv.h"


NEW_MACHINE_EVENT_ENUM __attribute__((weak)) UFMN(aa1)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("weak: %s", __func__);
	(void) pfsm;
	return THIS(noEvent);
}

NEW_MACHINE_EVENT_ENUM __attribute__((weak)) UFMN(aa2)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("weak: %s", __func__);
	(void) pfsm;
	return THIS(noEvent);
}

NEW_MACHINE_EVENT_ENUM __attribute__((weak)) UFMN(noAction)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("weak: %s", __func__);
	(void) pfsm;
	return THIS(noEvent);
}

void newMachine_subMachine1_ee1_dt(pNEW_MACHINE_DATA pdata)
{
	(void) pdata;

	DBG_PRINTF("weak: %s", __func__);
}

void __attribute__((weak)) newMachine_subMachine1_foo(void)
{
	DBG_PRINTF("weak: %s", __func__);
}

void __attribute__((weak)) newMachine_subMachine1_bar(void)
{
	DBG_PRINTF("weak: %s", __func__);
}

void __attribute__((weak)) newMachine_subMachine1_onEntryTo_ss2(void)
{
	DBG_PRINTF("weak: %s", __func__);
}

void __attribute__((weak)) newMachine_subMachine1_onExitFrom_ss2(void)
{
	DBG_PRINTF("weak: %s", __func__);
}

void __attribute__((weak)) newMachine_subMachine1_foo1(void)
{
	DBG_PRINTF("weak: %s", __func__);
}

void __attribute__((weak)) newMachine_subMachine1_bar1(void)
{
	DBG_PRINTF("weak: %s", __func__);
}


