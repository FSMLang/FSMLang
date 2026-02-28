/**
	Test the action_info_list functions.
*/
#include <stdlib.h>

#include "unit_test.h"

#include "action_info_list.h"
#include "print_consolidated.h"
#include "grist.h"

//consolidate a two member action info list having two occurrences of one action/transition pair
int main(void)
{
	ACTION_INFO ai1, ai2;
	pLIST       action_info_list = init_list();

	//action a1[e1,s1] transition t1;
	ai1.action     = &a1;

	ai1.matrix = (pMATRIX_INFO) calloc(1, sizeof(MATRIX_INFO));
	ai1.matrix->event_list = init_list();
	add_to_list(ai1.matrix->event_list, &e1);

	ai1.matrix->state_list = init_list();
	add_to_list(ai1.matrix->state_list, &s1);

	ai1.transition = &t1;

	add_to_list(action_info_list, &ai1);

	//action a1[e2,s2] transition t1;
	ai2.action     = &a1;

	ai2.matrix = (pMATRIX_INFO) calloc(1, sizeof(MATRIX_INFO));
	ai2.matrix->event_list = init_list();
	add_to_list(ai2.matrix->event_list, &e2);

	ai2.matrix->state_list = init_list();
	add_to_list(ai2.matrix->state_list, &s2);

	ai2.transition = &t1;

	add_to_list(action_info_list, &ai2);

	pLIST consolidated = consolidate_action_info_list(action_info_list);

	print_consolidated(consolidated);

	return 0;
}


