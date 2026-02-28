/**
 printf a consolidated action info list
*/
#include <stdio.h>
#include <string.h>

#include "print_consolidated.h"
#include "action_info_list.h"

#include "fsm_priv.h"

static bool print_record(pLIST_ELEMENT,void*);
static bool print_pid_name(pLIST_ELEMENT,void*);
static bool print_matrix(pLIST_ELEMENT,void*);

void print_consolidated(pLIST plist)
{
	iterate_list(plist, print_record, NULL);
}

static bool print_record(pLIST_ELEMENT pelem, void *data)
{
	(void) data;
	pCONSOLIDATED_ACTION_INFO pcai = (pCONSOLIDATED_ACTION_INFO) pelem->mbr;

	pACTION_INFO pai = pcai->pai;
	ITERATOR_HELPER ih = {
		.first = true
		, .str = ", "
	};

	if (pai->action->name && strlen(pai->action->name))
	{
		printf("action %s ", pai->action->name);
	}

	iterate_list(pcai->matrices, print_matrix, &ih);

	if (pai->transition)
	{
		printf(" transition %s ", pai->transition->name);
	}

	printf("\n");

	return false;
}

static bool print_pid_name(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO         pid = (pID_INFO) pelem->mbr;
	pITERATOR_HELPER pih = (pITERATOR_HELPER) data;

	if (!pih->first)
	{
		printf("%s", pih->str);
	}
	pih->first = false;

	printf("%s", pid->name);

	return false;
}

static bool print_matrix(pLIST_ELEMENT pelem, void *data)
{
	pMATRIX_INFO pmi = (pMATRIX_INFO) pelem->mbr;
	pITERATOR_HELPER pih = (pITERATOR_HELPER) data;

	if (!pih->first)
	{
		printf("%s", pih->str);
	}
	printf("[");

	if (pmi->event_list->count > 1)
	{
		printf("(");
	}

	pih->first = true;
	iterate_list(pmi->event_list, print_pid_name, pih);

	if (pmi->event_list->count > 1)
	{
		printf(")");
	}

	printf(", ");

	if (pmi->state_list->count > 1)
	{
		printf("(");
	}

	pih->first = true;
	iterate_list(pmi->state_list, print_pid_name, pih);

	if (pmi->state_list->count > 1)
	{
		printf(")");
	}

	printf("]");

	pih->first = false;

	return false;
}

