/**
	Test the action_info_list functions.
*/
#include <stdlib.h>

#include "unit_test.h"

#include "action_info_list.h"
#include "print_consolidated.h"
#include "grist.h"

int main(void)
{
	ACTION_INFO ai1, ai2, ai3, ai4, ai5;
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

	//action a2[e2,s2] transition t1;
	ai2.action     = &a2;

	ai2.matrix = (pMATRIX_INFO) calloc(1, sizeof(MATRIX_INFO));
	ai2.matrix->event_list = init_list();
	add_to_list(ai2.matrix->event_list, &e2);

	ai2.matrix->state_list = init_list();
	add_to_list(ai2.matrix->state_list, &s2);

	ai2.transition = &t1;

	add_to_list(action_info_list, &ai2);

	//action a2[(e1, e2),s3] transition t3;
	ai3.action     = &a2;

	ai3.matrix = (pMATRIX_INFO) calloc(1, sizeof(MATRIX_INFO));
	ai3.matrix->event_list = init_list();
	add_to_list(ai3.matrix->event_list, &e1);
	add_to_list(ai3.matrix->event_list, &e2);

	ai3.matrix->state_list = init_list();
	add_to_list(ai3.matrix->state_list, &s2);

	ai3.transition = &t1;

	add_to_list(action_info_list, &ai3);

	//action a2[(e3, e4), (s1, s2)] transition t3;
	ai4.action     = &a2;

	ai4.matrix = (pMATRIX_INFO) calloc(1, sizeof(MATRIX_INFO));
	ai4.matrix->event_list = init_list();
	add_to_list(ai4.matrix->event_list, &e3);
	add_to_list(ai4.matrix->event_list, &e4);

	ai4.matrix->state_list = init_list();
	add_to_list(ai4.matrix->state_list, &s1);
	add_to_list(ai4.matrix->state_list, &s2);

	ai4.transition = &t1;

	add_to_list(action_info_list, &ai4);

	//action a1[(e3, e4, e5), s3] transition t3;
	ai5.action     = &a1;

	ai5.matrix = (pMATRIX_INFO) calloc(1, sizeof(MATRIX_INFO));
	ai5.matrix->event_list = init_list();
	add_to_list(ai5.matrix->event_list, &e3);
	add_to_list(ai5.matrix->event_list, &e4);
	add_to_list(ai5.matrix->event_list, &e5);

	ai5.matrix->state_list = init_list();
	add_to_list(ai5.matrix->state_list, &s3);

	ai5.transition = &t1;

	add_to_list(action_info_list, &ai5);

	pLIST consolidated = consolidate_action_info_list(action_info_list);

	print_consolidated(consolidated);

	return 0;
}


