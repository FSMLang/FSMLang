#include <stdio.h>

#include "unit_test.h"

#include "fsm_c_utils.h"

// test data
ID_INFO id_who_knows = {
	.name = "who_knows"
};
MACHINE_INFO mi_who_knows = {
	.name = &id_who_knows
};
CMachineData cmd_who_knows = {
	.pmi = &mi_who_knows
};

ID_INFO id_george_c = {
	.name = "george_c"
};
MACHINE_INFO mi_george_c = {
	.name = &id_george_c
};
CMachineData cmd_george_c = {
	.pmi = &mi_george_c
	, .parent_pcmd = &cmd_who_knows
};

ID_INFO id_george_charles = {
	.name = "george_charles"
};
MACHINE_INFO mi_george_charles = {
	.name = &id_george_charles
};
CMachineData cmd_george_charles = {
	.pmi = &mi_george_charles
	, .parent_pcmd = &cmd_george_c
};

ID_INFO id_steve = {
	.name = "steve"
};
MACHINE_INFO mi_steve = {
	.name = &id_steve
};

CMachineData cmd_steve = {
	.pmi = &mi_steve
	, .parent_pcmd = &cmd_george_charles
};

// tests
static void immediate_PARENT(void);
static void child_PARENT(void);
static void grand_child_PARENT(void);
static void great_grand_child_PARENT(void);

static void immediate_MACROS(void);
static void child_MACROS(void);
static void grand_child_MACROS(void);
static void great_grand_child_MACROS(void);

static void immediate_GENERATION_COUNT(void);
static void child_GENERATION_COUNT(void);
static void grand_child_GENERATION_COUNT(void);
static void great_grand_child_GENERATION_COUNT(void);

static void immediate_NTH_GENERATION(void);
static void child_NTH_GENERATION(void);
static void grand_child_NTH_GENERATION(void);
static void great_grand_child_NTH_GENERATION(void);

static void immediate_ULTIMATE_ANCESTOR(void);
static void child_ULTIMATE_ANCESTOR(void);
static void grand_child_ULTIMATE_ANCESTOR(void);
static void great_grand_child_ULTIMATE_ANCESTOR(void);

static void immediate_CASE_STATEMENTS(void);
static void child_CASE_STATEMENTS(void);
static void grand_child_CASE_STATEMENTS(void);
static void great_grand_child_CASE_STATEMENTS(void);

VOID_TEST_FN tests[] = {
	immediate_PARENT
	, child_PARENT
	, grand_child_PARENT
	, great_grand_child_PARENT
	, immediate_MACROS
	, child_MACROS
	, grand_child_MACROS
	, great_grand_child_MACROS
	, immediate_GENERATION_COUNT
	, child_GENERATION_COUNT
	, grand_child_GENERATION_COUNT
	, great_grand_child_GENERATION_COUNT
	, immediate_NTH_GENERATION
	, child_NTH_GENERATION
	, grand_child_NTH_GENERATION
	, great_grand_child_NTH_GENERATION
	, immediate_ULTIMATE_ANCESTOR
	, child_ULTIMATE_ANCESTOR
	, grand_child_ULTIMATE_ANCESTOR
	, great_grand_child_ULTIMATE_ANCESTOR
	, great_grand_child_NTH_GENERATION
	, immediate_CASE_STATEMENTS
	, child_CASE_STATEMENTS
	, grand_child_CASE_STATEMENTS
	, great_grand_child_CASE_STATEMENTS
	, NULL
};

int main(void)
{
	for (VOID_TEST_FN *test_fn = tests; *test_fn; test_fn++)
	{
		(*test_fn)();
		printf("\n");
	}

	return 0;
}

static void immediate_PARENT(void)
{
	printf("[");
	print_ancestor_macro(stdout, &cmd_who_knows);
	printf("]\n");
}

static void child_PARENT(void)
{
	printf("[");
	print_ancestor_macro(stdout, &cmd_george_c);
	printf("]\n");
}

static void grand_child_PARENT(void)
{
	printf("[");
	print_ancestor_macro(stdout, &cmd_george_charles);
	printf("]\n");
}

static void great_grand_child_PARENT(void)
{
	printf("[");
	print_ancestor_macro(stdout, &cmd_steve);
	printf("]\n");
}

static void immediate_MACROS(void)
{
	printf("[");
	printf("\nme: %s\n", cmd_who_knows.pmi->name->name);
	define_ancestor_macros(stdout, &cmd_who_knows);
	printf("]\n");
}

static void child_MACROS(void)
{
	printf("[");
	printf("\nme: %s\n", cmd_george_c.pmi->name->name);
	define_ancestor_macros(stdout, &cmd_george_c);
	printf("]\n");
}

static void grand_child_MACROS(void)
{
	printf("[");
	printf("\nme: %s\n", cmd_george_charles.pmi->name->name);
	define_ancestor_macros(stdout, &cmd_george_charles);
	printf("]\n");
}

static void great_grand_child_MACROS(void)
{
	printf("[");
	printf("\nme: %s\n", cmd_steve.pmi->name->name);
	define_ancestor_macros(stdout, &cmd_steve);
	printf("]\n");
}

static void immediate_GENERATION_COUNT(void)
{
	printf("[");
	printf("%u", count_generations(&cmd_who_knows));
	printf("]\n");
}

static void child_GENERATION_COUNT(void)
{
	printf("[");
	printf("%u", count_generations(&cmd_george_c));
	printf("]\n");
}

static void grand_child_GENERATION_COUNT(void)
{
	printf("[");
	printf("%u", count_generations(&cmd_george_charles));
	printf("]\n");
}

static void great_grand_child_GENERATION_COUNT(void)
{
	printf("[");
	printf("%u", count_generations(&cmd_steve));
	printf("]\n");
}

static void immediate_NTH_GENERATION(void)
{
	printf("[");
	for (unsigned generation = count_generations(&cmd_who_knows);
			generation;
			generation--
		 )
	{
		printf("generation %u: %s\n"
				 , generation
				 , (nth_generation(&cmd_who_knows, generation))->pmi->name->name
				);
	}
	printf("]\n");
}

static void child_NTH_GENERATION(void)
{
	for (unsigned generation = count_generations(&cmd_george_c);
			generation;
			generation--
		 )
	{
		printf("generation %u: %s\n"
				 , generation
				 , (nth_generation(&cmd_george_c, generation))->pmi->name->name
				);
	}
}

static void grand_child_NTH_GENERATION(void)
{
	for (unsigned generation = count_generations(&cmd_george_charles);
			generation;
			generation--
		 )
	{
		printf("generation %u: %s\n"
				 , generation
				 , (nth_generation(&cmd_george_charles, generation))->pmi->name->name
				);
	}
}

static void great_grand_child_NTH_GENERATION(void)
{
	for (unsigned generation = count_generations(&cmd_steve);
			generation;
			generation--
		 )
	{
		printf("generation %u: %s\n"
				 , generation
				 , (nth_generation(&cmd_steve, generation))->pmi->name->name
				);
	}
}

static void immediate_ULTIMATE_ANCESTOR(void)
{
	printf("[");
	printf("Me: %s; ultimate ancestor: %s"
			 , cmd_who_knows.pmi->name->name
			 , ultimatePcmdAncestor(&cmd_who_knows)->pmi->name->name
			);
	printf("]\n");
}

static void child_ULTIMATE_ANCESTOR(void)
{
	printf("[");
	printf("Me: %s; ultimate ancestor: %s"
			 , cmd_george_c.pmi->name->name
			 , ultimatePcmdAncestor(&cmd_george_c)->pmi->name->name
			);
	printf("]\n");
}

static void grand_child_ULTIMATE_ANCESTOR(void)
{
	printf("[");
	printf("Me: %s; ultimate ancestor: %s"
			 , cmd_george_charles.pmi->name->name
			 , ultimatePcmdAncestor(&cmd_george_charles)->pmi->name->name
			);
	printf("]\n");
}

static void great_grand_child_ULTIMATE_ANCESTOR(void)
{
	printf("[");
	printf("Me: %s; ultimate ancestor: %s"
			 , cmd_steve.pmi->name->name
			 , ultimatePcmdAncestor(&cmd_steve)->pmi->name->name
			);
	printf("]\n");
}

static void immediate_CASE_STATEMENTS(void)
{
	printf("[");
	print_ancestor_case_statements(stdout, &cmd_who_knows, "", "e1");
	printf("]\n");
}

static void child_CASE_STATEMENTS(void)
{
	printf("[");
	print_ancestor_case_statements(stdout, &cmd_george_c, "", "e1");
	printf("]\n");
}

static void grand_child_CASE_STATEMENTS(void)
{
	printf("[");
	print_ancestor_case_statements(stdout, &cmd_george_charles, "", "e1");
	printf("]\n");
}

static void great_grand_child_CASE_STATEMENTS(void)
{
	printf("[");
	print_ancestor_case_statements(stdout, &cmd_steve, "", "e1");
	printf("]\n");
}

