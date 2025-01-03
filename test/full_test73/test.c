#include "test_fsm.h"

int main(void)
{
	run_test(THIS(e1));
	run_test(THIS(e2));
	run_test(THIS(e3));

	run_test(SUB(e1));

	return 0;
}

