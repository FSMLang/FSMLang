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

void add_one_file_test                         (void);
void add_two_file_test                         (void);
void add_two_files_with_intervening_print_test (void);
void check_panchor_is_null                     (void);

VOID_TEST_FN tests[] = {
	add_one_file_test
	, add_two_file_test
	, add_two_files_with_intervening_print_test
   , check_panchor_is_null
	, NULL
};

void add_one_file_test()
{
	printf("%s\n", __func__);
	track_included_file("foo");
	print_included_files_list();
}

void add_two_file_test()
{
	printf("%s\n", __func__);
	track_included_file("foo");
	track_included_file("bar");
	print_included_files_list();
}

void add_two_files_with_intervening_print_test()
{
	printf("%s\n", __func__);
	track_included_file("foo");
	print_included_files_list();
	printf("\n");
	track_included_file("bar");
	print_included_files_list();
}

void check_panchor_is_null(void)
{
	printf("%s\n", __func__);
	track_included_file("foo");
	print_included_files_list();
	printf("\npanchor is %snull\n"
          , panchor ? "not " : ""
          );
          
}

int main(void)
{
	VOID_TEST_FN *test_fn;

	for (test_fn = tests; *test_fn; test_fn++)
	{
		(*test_fn)();
		printf("\n");
	}

	return 0;
}

