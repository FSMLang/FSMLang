/**
	Test the action_info_list functions.
*/
#include <stdlib.h>

#include "unit_test.h"

#include "action_info_list.h"
#include "print_consolidated.h"
#include "grist.h"

//add a single member action info list to an empty consolidated list
int main(void)
{
	ACTION_INFO ai;
	pLIST       action_info_list = init_list();

	//action a1[e1,s1] transition t1;
	ai.action     = &a1;

	ai.matrix = (pMATRIX_INFO) calloc(1, sizeof(MATRIX_INFO));
	ai.matrix->event_list = init_list();
	add_to_list(ai.matrix->event_list, &e1);

	ai.matrix->state_list = init_list();
	add_to_list(ai.matrix->state_list, &s1);

	ai.transition = &t1;

	add_to_list(action_info_list, &ai);

	pLIST consolidated = consolidate_action_info_list(action_info_list);

	print_consolidated(consolidated);

	return 0;
}


