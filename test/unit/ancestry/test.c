#include <stdio.h>
#include <stdlib.h>
#include "unit_test.h"

#include "fsm_priv.h"

ID_INFO grand_parent_name = {
	.name = "grandParent"
};
ID_INFO child_name = {
	.name = "child"
};
ID_INFO grand_child_name = {
	.name = "grandChild"
};
MACHINE_INFO grand_parent = {
	.name = &grand_parent_name
};
MACHINE_INFO child = {
	.parent = &grand_parent
	, .name = &child_name
};
MACHINE_INFO grand_child = {
	.parent = &child
	, .name = &grand_child_name
};

static void leading_underbar(void);
static void no_leading_underbar(void);
static void internal_underbar(void);
static void double_underbar(void);
static void trailing_underbar(void);
static void doubled_caps(void);
static void ancestry_no_caps_no_self(void);
static void ancestry_no_caps_self(void);
static void ancestry_caps_self(void);
static void ancestry_caps_no_self(void);
static void ancestry_caps_self_ultimate(void);
static void ancestry_file_name(void);

VOID_TEST_FN tests[] = {
	leading_underbar,
	no_leading_underbar,
	internal_underbar,
	double_underbar,
	trailing_underbar,
	doubled_caps,
	ancestry_no_caps_no_self,
	ancestry_no_caps_self,
	ancestry_caps_self,
	ancestry_caps_no_self,
	ancestry_caps_self_ultimate,
	ancestry_file_name,
	NULL
};

static void leading_underbar()
{
	streamHungarianToUnderbarCaps(stdout, "_fooBar");
}

static void no_leading_underbar()
{
	streamHungarianToUnderbarCaps(stdout, "fooBar");
}

static void double_underbar()
{
	streamHungarianToUnderbarCaps(stdout, "foo__bar");
}

static void internal_underbar()
{
	streamHungarianToUnderbarCaps(stdout, "foo_bar");
}

static void trailing_underbar()
{
	streamHungarianToUnderbarCaps(stdout, "fooBar_");
}

static void doubled_caps()
{
	streamHungarianToUnderbarCaps(stdout, "fooBAr");
}

static void ancestry_no_caps_no_self()
{
	(void) printAncestry(&grand_child, stdout, "_", alc_lower, ai_omit_self);
}

static void ancestry_no_caps_self()
{
	(void) printAncestry(&grand_child, stdout, "_", alc_lower, ai_include_self);
}

static void ancestry_caps_self()
{
	(void) printAncestry(&grand_child, stdout, "_", alc_upper, ai_include_self);
}

static void ancestry_caps_self_ultimate()
{
	(void) printAncestry(&grand_parent, stdout, "_", alc_upper, ai_include_self);
}

static void ancestry_caps_no_self()
{
	(void) printAncestry(&grand_child, stdout, "_", alc_upper, ai_omit_self);
}

static void ancestry_file_name()
{
	char *fn = createAncestryFileName(&grand_child);

	printf("%s", fn);
}

int main(int argc, char **argv)
{
	int result = 0;

	printf("%s: hello, world\n",argv[0]);

	for (VOID_TEST_FN *test_fn = tests; *test_fn; test_fn++)
	{
		(*test_fn)();
		printf("\n");
	}

	return result;
}

