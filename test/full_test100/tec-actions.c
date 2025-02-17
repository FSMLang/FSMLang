#include "test_empty_cells_priv.h"

ACTION_RETURN_TYPE UFMN(a1)(FSM_TYPE_PTR pfsm)
{
	(void) pfsm;
	DBG_PRINTF(__func__);
	return THIS(noEvent);
}

#if defined(FSM_VARIANT_CC)
TEST_EMPTY_CELLS_EVENT UFMN(share)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);
	(void) pfsm;
	return test_empty_cells_pass_shared_event(sharing_test_empty_cells_e3);
}
#endif

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

	ptest_empty_cells->state = test_empty_cells_s1;
	run_test_empty_cells(THIS(e2));	//prints noAction

	ptest_empty_cells->state = test_empty_cells_s1;
	run_test_empty_cells(THIS(e3));	//prints SUB(a1)

	ptest_empty_cells->state = test_empty_cells_s1;
	set_sub_state_s2();
	run_test_empty_cells(THIS(e3));	//prints SUB(noAction)

	ptest_empty_cells->state = test_empty_cells_s1;
	set_sub_state_s3();
	run_test_empty_cells(THIS(e3));	//prints i_am_empty

	ptest_empty_cells->state = test_empty_cells_s2;
	run_test_empty_cells(THIS(e1));	//prints i_am_empty

	ptest_empty_cells->state = test_empty_cells_s2;
	run_test_empty_cells(THIS(e2));	//prints i_am_empty

	ptest_empty_cells->state = test_empty_cells_s2;
	run_test_empty_cells(THIS(e3));	//prints i_am_empty

	return 0;
}

