#include <stdio.h>
#include <stdlib.h>
#include "unit_test.h"

#include "fsm_priv.h"
#include "ancestry.h"

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

static void print_pmi_name(pMACHINE_INFO);
static void leading_underbar(void);
static void no_leading_underbar(void);
static void internal_underbar(void);
static void double_underbar(void);
static void trailing_underbar(void);
static void doubled_caps(void);
static void all_caps(void);
static void stream_leading_underbar(void);
static void stream_no_leading_underbar(void);
static void stream_internal_underbar(void);
static void stream_double_underbar(void);
static void stream_trailing_underbar(void);
static void stream_doubled_caps(void);
static void stream_all_caps(void);
static void ancestry_no_caps_no_self(void);
static void ancestry_no_caps_self(void);
static void ancestry_caps_self(void);
static void ancestry_caps_no_self(void);
static void ancestry_caps_self_ultimate(void);
static void ancestry_caps_self_stop_at_parent(void);
static void ancestry_caps_ultimate_stop_at_parent(void);
static void ancestry_caps_omit_self_stop_at_parent(void);
static void ancestry_file_name(void);
static void name_with_ancestry_lc_to_uc(void);
static void name_with_ancestry_lc_to_lc(void);
static void name_with_ancestry_uc_to_lc(void);

VOID_TEST_FN tests[] = {
	leading_underbar,
	no_leading_underbar,
	internal_underbar,
	double_underbar,
	trailing_underbar,
	doubled_caps,
	all_caps,
	stream_leading_underbar,
	stream_no_leading_underbar,
	stream_internal_underbar,
	stream_double_underbar,
	stream_trailing_underbar,
	stream_doubled_caps,
	stream_all_caps,
	ancestry_no_caps_no_self,
	ancestry_no_caps_self,
	ancestry_caps_self,
	ancestry_caps_no_self,
	ancestry_caps_self_ultimate,
	ancestry_file_name,
	ancestry_caps_self_stop_at_parent,
	ancestry_caps_ultimate_stop_at_parent,
	ancestry_caps_omit_self_stop_at_parent,
	name_with_ancestry_lc_to_uc,
	name_with_ancestry_lc_to_lc,
	name_with_ancestry_uc_to_lc,
	NULL
};

static void leading_underbar()
{
	char *input = "_fooBar";
	char *output = hungarianToUnderbarCaps(input);

	printf("\n%s:\n", __func__);

	printf("%s becomes: %s"
			, input
			, output
		  );
}

static void no_leading_underbar()
{
	char *input = "fooBar";
	char *output = hungarianToUnderbarCaps(input);

	printf("\n%s:\n", __func__);
	printf("%s becomes: %s"
			, input
			, output
		  );
}

static void double_underbar()
{
	char *input = "foo__bar";
	char *output = hungarianToUnderbarCaps(input);

	printf("\n%s:\n", __func__);
	printf("%s becomes: %s"
			, input
			, output
		  );
}

static void internal_underbar()
{
	char *input = "foo_bar";
	char *output = hungarianToUnderbarCaps(input);

	printf("\n%s:\n", __func__);
	printf("%s becomes: %s"
			, input
			, output
		  );
}

static void trailing_underbar()
{
	char *input = "fooBar_";
	char *output = hungarianToUnderbarCaps(input);

	printf("\n%s:\n", __func__);
	printf("%s becomes: %s"
			, input
			, output
		  );
}

static void doubled_caps()
{
	char *input = "fooBAr";
	char *output = hungarianToUnderbarCaps(input);

	printf("\n%s:\n", __func__);
	printf("%s becomes: %s"
			, input
			, output
		  );
}

static void all_caps()
{
	char *input = "FOOBAR";
	char *output = hungarianToUnderbarCaps(input);

	printf("\n%s:\n", __func__);
	printf("%s becomes: %s"
			, input
			, output
		  );
}

static void stream_leading_underbar()
{
	char *input = "_fooBar";

	printf("\n%s:\n", __func__);

	printf("%s becomes: ", input);
	streamHungarianToUnderbarCaps(stdout, input);
}

static void stream_no_leading_underbar()
{
	char *input = "fooBar";

	printf("\n%s:\n", __func__);
	printf("%s becomes: ", input);
	streamHungarianToUnderbarCaps(stdout, input);
}

static void stream_double_underbar()
{
	char *input = "foo__bar";

	printf("\n%s:\n", __func__);
	printf("%s becomes: ", input);
	streamHungarianToUnderbarCaps(stdout, input);
}

static void stream_internal_underbar()
{
	char *input = "foo_bar";

	printf("\n%s:\n", __func__);
	printf("%s becomes: ", input);
	streamHungarianToUnderbarCaps(stdout, input);
}

static void stream_trailing_underbar()
{
	char *input = "fooBar_";

	printf("\n%s:\n", __func__);
	printf("%s becomes: ", input);
	streamHungarianToUnderbarCaps(stdout, input);
}

static void stream_doubled_caps()
{
	char *input = "fooBAr";

	printf("\n%s:\n", __func__);
	printf("%s becomes: ", input);
	streamHungarianToUnderbarCaps(stdout, input);
}

static void stream_all_caps()
{
	char *input = "FOOBAR";

	printf("\n%s:\n", __func__);
	printf("%s becomes: ", input);
	streamHungarianToUnderbarCaps(stdout, input);
}

static void ancestry_no_caps_no_self()
{
	pMACHINE_INFO pmi = &grand_child;
	
	printf("\n%s:\n", __func__);
	print_pmi_name(pmi);
	(void) printAncestry(pmi, stdout, "_", alc_lower, ai_omit_self);
}

static void ancestry_no_caps_self()
{
	pMACHINE_INFO pmi = &grand_child;

	printf("\n%s:\n", __func__);
	print_pmi_name(pmi);
	(void) printAncestry(pmi, stdout, "_", alc_lower, ai_include_self);
}

static void ancestry_caps_self()
{
	pMACHINE_INFO pmi = &grand_child;

	printf("\n%s:\n", __func__);
	print_pmi_name(pmi);
	(void) printAncestry(pmi, stdout, "_", alc_upper, ai_include_self);
}

static void ancestry_caps_self_ultimate()
{
	pMACHINE_INFO pmi = &grand_parent;
	
	printf("\n%s:\n", __func__);
	print_pmi_name(pmi);
	(void) printAncestry(pmi, stdout, "_", alc_upper, ai_include_self);
}

static void ancestry_caps_self_stop_at_parent()
{
	pMACHINE_INFO pmi = &grand_child;
	
	printf("\n%s:\n", __func__);
	print_pmi_name(pmi);
	(void) printAncestry(pmi, stdout, "_", alc_upper, ai_include_self | ai_stop_at_parent);
}

static void ancestry_caps_ultimate_stop_at_parent()
{
	pMACHINE_INFO pmi = &grand_parent;
	
	printf("\n%s:\n", __func__);
	print_pmi_name(pmi);
	(void) printAncestry(pmi, stdout, "_", alc_upper, ai_include_self | ai_stop_at_parent);
}

static void ancestry_caps_no_self()
{
	pMACHINE_INFO pmi = &grand_child;
	
	printf("\n%s:\n", __func__);
	print_pmi_name(pmi);
	(void) printAncestry(pmi, stdout, "_", alc_upper, ai_omit_self);
}

static void ancestry_caps_omit_self_stop_at_parent()
{
	pMACHINE_INFO pmi = &grand_child;
	
	printf("\n%s:\n", __func__);
	print_pmi_name(pmi);
	(void) printAncestry(pmi, stdout, "_", alc_upper, ai_omit_self | ai_stop_at_parent);
}

static void ancestry_file_name()
{
	pMACHINE_INFO pmi = &grand_child;
	
	printf("\n%s:\n", __func__);
	print_pmi_name(pmi);

	char *fn = createAncestryFileName(pmi);

	printf("%s", fn);
}

static void name_with_ancestry_lc_to_uc(void)
{
	pMACHINE_INFO pmi = &grand_child;
	
	printf("\n%s:\n", __func__);

	char *name = "name";

	(void) printNameWithAncestry(name, pmi, stdout, "_", alc_upper, ai_include_self);

}

static void name_with_ancestry_lc_to_lc(void)
{
	pMACHINE_INFO pmi = &grand_child;
	
	printf("\n%s:\n", __func__);

	char *name = "name";

	(void) printNameWithAncestry(name, pmi, stdout, "_", alc_lower, ai_include_self);

}

static void name_with_ancestry_uc_to_lc(void)
{
	pMACHINE_INFO pmi = &grand_child;
	
	printf("\n%s:\n", __func__);

	char *name = "NAME";

	(void) printNameWithAncestry(name, pmi, stdout, "_", alc_lower, ai_include_self);

}

int main(int argc, char **argv)
{
	printf("%s: hello, world\n",argv[0]);

	for (VOID_TEST_FN *test_fn = tests; *test_fn; test_fn++)
	{
		(*test_fn)();
		printf("\n");
	}

	(void) argc;

	return 0;
}

static void print_pmi_name(pMACHINE_INFO pmi)
{
	printf("input %s: ", pmi->name->name);
}

