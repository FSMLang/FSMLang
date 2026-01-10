#include <stdio.h>
#include <stdlib.h>

#include "../../unit_test.h"

#include "util_file_inclusion.h"
#include "fsm_priv.h"

extern pLIST panchor;

void yyerror(char *msg)
{
	printf("%s\n", msg);
}

bool print_id_info_name (pLIST_ELEMENT,void*);

bool add_to_list_test  (void);

TEST_FN tests[] = {
	add_to_list_test
	, NULL
};

bool print_id_info_name(pLIST_ELEMENT piilm, void *data)
{
	pID_INFO pid = (pID_INFO) piilm->mbr;

  (void) data;

	printf("%s\n", pid->name);
	return false;
}

bool add_to_list_test()
{
	track_included_file("foo");

	return panchor->count == 1;
}

int main(void)
{
	TEST_FN *test_fn;

	for (test_fn = tests; *test_fn && (*test_fn)(); test_fn++)
		;

	printf("%s\n", *test_fn ? "Test failed" : "All tests pass");

	return *test_fn ? 1 : 0;
}

