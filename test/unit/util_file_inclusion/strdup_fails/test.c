#include <stdio.h>
#include <stdlib.h>

#include "../../unit_test.h"

#include "util_file_inclusion.h"
#include "fsm_priv.h"

extern pLIST panchor;

char *strdup(const char *s)
{
	(void) s;
	return NULL;
}

void yyerror(char *msg)
{
	printf("%s\n", msg);
}

void add_to_list_test  (void);

VOID_TEST_FN tests[] = {
	add_to_list_test
	, NULL
};

void add_to_list_test()
{
	track_included_file("foo");
}

int main(void)
{
	VOID_TEST_FN *test_fn;

	for (test_fn = tests; *test_fn; test_fn++)
		(*test_fn)();

	return 0;
}

