#include "test_empty_cells_priv.h"

#ifndef STATE
#define STATE(A) test_empty_cells_#A
#endif

ACTION_RETURN_TYPE UFMN(a1)(FSM_TYPE_PTR pfsm)
{
	(void) pfsm;
	DBG_PRINTF(__func__);
	return THIS(noEvent);
}

ACTION_RETURN_TYPE UFMN(i_am_empty)(FSM_TYPE_PTR pfsm)
{
	(void) pfsm;
	DBG_PRINTF(__func__);
	return THIS(noEvent);
}

ACTION_RETURN_TYPE UFMN(noAction)(FSM_TYPE_PTR pfsm)
{
	(void) pfsm;
	DBG_PRINTF(__func__);
	return THIS(noEvent);
}

int main(void)
{
	run_test_empty_cells(THIS(e1));	//prints a1

	ptest_empty_cells->state = STATE(s1);
	run_test_empty_cells(THIS(e2));	//prints noAction

	ptest_empty_cells->state = STATE(s1);
	run_test_empty_cells(THIS(e3));	//prints i_am_empty

	ptest_empty_cells->state = STATE(s2);
	run_test_empty_cells(THIS(e1));	//prints i_am_empty

	ptest_empty_cells->state = STATE(s2);
	run_test_empty_cells(THIS(e2));	//prints i_am_empty

	ptest_empty_cells->state = STATE(s2);
	run_test_empty_cells(THIS(e3));	//prints i_am_empty

	return 0;
}

