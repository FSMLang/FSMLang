#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "unit_test.h"

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
	, .sub_machine_depth = 2
};

MACHINE_INFO child = {
	.parent = &grand_parent
	, .name = &child_name
	, .sub_machine_depth = 1
};

MACHINE_INFO grand_child = {
	.parent = &child
	, .name = &grand_child_name
};

MACHINE_INFO two_generation_parent = {
	.name = &grand_parent_name
	, .sub_machine_depth = 1
};

MACHINE_INFO two_generation_child = {
	.name = &child_name
	, .parent = &two_generation_parent
};

CMachineData cmd;

static void action_return_type_event(void);
static void action_return_type_event_enum(void);
static void action_return_type_state(void);
static void action_return_type_void(void);
static void fsm_type_ultimate(void);
static void fsm_type_child(void);
static void event_type_ultimate(void);
static void event_type_ultimate_enum(void);
static void fq_machine_name(void);
static void nf_machine_name(void);
static void nf_machine_name_pmi(void);
static void uc_machine_name(void);
static void uc_fq_machine_name(void);
static void uc_nf_machine_name(void);
static void uf_machine_name_two_generation(void);
static void uf_machine_name_two_generation_restricted(void);


VOID_TEST_FN tests[] = {
	action_return_type_event,
	action_return_type_event_enum,
	action_return_type_state,
	action_return_type_void,
	fsm_type_ultimate,
	fsm_type_child,
	event_type_ultimate,
	event_type_ultimate_enum,
	fq_machine_name,
	nf_machine_name,
	nf_machine_name_pmi,
	uc_machine_name,
	uc_fq_machine_name,
	uc_nf_machine_name,
	uf_machine_name_two_generation,
	uf_machine_name_two_generation_restricted,
	NULL
};

static void action_return_type_state()
{
	memset(&cmd, 0, sizeof(cmd));
	cmd.pmi = &grand_parent;
	cmd.pmi->modFlags = mfActionsReturnStates;
	printf("%s:\n", __func__);
	printf("initial: %s\n",   cmd.pmi->name->name);
	printf("subsequent: %s\n",actionReturnType(&cmd));
}

static void action_return_type_void()
{
	memset(&cmd, 0, sizeof(cmd));
	cmd.pmi = &grand_parent;
	cmd.pmi->modFlags = mfActionsReturnVoid;
	printf("%s:\n", __func__);
	printf("initial: %s\n",   cmd.pmi->name->name);
	printf("subsequent: %s\n",actionReturnType(&cmd));
}

static void action_return_type_event()
{
	memset(&cmd, 0, sizeof(cmd));
	cmd.pmi = &grand_parent;
	cmd.pmi->modFlags = 0;
	printf("%s:\n", __func__);
	printf("initial: %s\n",   cmd.pmi->name->name);
	printf("subsequent: %s\n",actionReturnType(&cmd));
}

static void action_return_type_event_enum()
{
	memset(&cmd, 0, sizeof(cmd));
	cmd.pmi = &grand_parent;
	cmd.pmi->modFlags = 0;
	cmd.pmi->data_block_count = 1;
	printf("%s:\n", __func__);
	printf("initial: %s\n",   cmd.pmi->name->name);
	printf("subsequent: %s\n",actionReturnType(&cmd));
}

static void fsm_type_ultimate()
{
	memset(&cmd, 0, sizeof(cmd));
	cmd.pmi = &grand_parent;
	printf("%s:\n", __func__);
	printf("initial: %s\n",   cmd.pmi->name->name);
	printf("subsequent: %s\n",fsmType(&cmd));
}

static void event_type_ultimate()
{
	memset(&cmd, 0, sizeof(cmd));
	cmd.pmi = &grand_parent;
	cmd.pmi->data_block_count = 0;
	printf("%s:\n", __func__);
	printf("initial: %s\n",   cmd.pmi->name->name);
	printf("subsequent: %s\n",eventType(&cmd));
}

static void event_type_ultimate_enum()
{
	memset(&cmd, 0, sizeof(cmd));
	cmd.pmi = &grand_parent;
	cmd.pmi->data_block_count = 1;
	printf("%s:\n", __func__);
	printf("initial: %s\n",   cmd.pmi->name->name);
	printf("subsequent: %s\n",eventType(&cmd));
}

static void fsm_type_child()
{
	memset(&cmd, 0, sizeof(cmd));
	cmd.pmi = &child;
	printf("%s:\n", __func__);
	printf("initial: %s\n",   cmd.pmi->name->name);
	printf("subsequent: %s\n",fsmType(&cmd));
}

static void fq_machine_name()
{
	memset(&cmd, 0, sizeof(cmd));
	cmd.pmi = &grand_child;
	printf("%s:\n", __func__);
	printf("initial: %s\n",   cmd.pmi->name->name);
	printf("subsequent: %s\n", fqMachineName(&cmd));
}

static void nf_machine_name()
{
	memset(&cmd, 0, sizeof(cmd));
	cmd.pmi = &grand_child;
	printf("%s:\n", __func__);
	printf("initial: %s\n",   cmd.pmi->name->name);
	printf("subsequent: %s\n", nfMachineName(&cmd));
}

static void nf_machine_name_pmi()
{
	char *cp;
	pMACHINE_INFO pmi = &child;

	printf("%s:\n", __func__);
	printf("initial: %s\n",   pmi->name->name);
	printf("subsequent: %s\n", nfMachineNamePmi(pmi, &cp));

	if (cp) free(cp);
}

static void uc_machine_name()
{
	memset(&cmd, 0, sizeof(cmd));
	cmd.pmi = &grand_child;
	printf("%s:\n", __func__);
	printf("initial: %s\n",   cmd.pmi->name->name);
	printf("subsequent: %s\n", ucMachineName(&cmd));
}

static void uc_fq_machine_name()
{
	memset(&cmd, 0, sizeof(cmd));
	cmd.pmi = &grand_child;
	printf("%s:\n", __func__);
	printf("initial: %s\n",   cmd.pmi->name->name);
	printf("subsequent: %s\n", ucfqMachineName(&cmd));
}

static void uc_nf_machine_name()
{
	memset(&cmd, 0, sizeof(cmd));
	cmd.pmi = &grand_child;
	printf("%s:\n", __func__);
	printf("initial: %s\n",   cmd.pmi->name->name);
	printf("subsequent: %s\n", ucnfMachineName(&cmd));
}

static void uf_machine_name_two_generation(void)
{
	short_user_fn_names = false;

	memset(&cmd, 0, sizeof(cmd));
	cmd.pmi = &two_generation_child;
	printf("%s:\n", __func__);
	printf("initial: %s\n",   cmd.pmi->name->name);
	printf("subsequent: %s\n", ufMachineName(&cmd));
}

static void uf_machine_name_two_generation_restricted(void)
{
	short_user_fn_names = true;

	memset(&cmd, 0, sizeof(cmd));
	cmd.pmi = &two_generation_child;
	printf("%s:\n", __func__);
	printf("initial: %s\n",   cmd.pmi->name->name);
	printf("subsequent: %s\n", ufMachineName(&cmd));
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

	(void) argc;

	return result;
}

