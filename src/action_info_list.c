/**
*  action_info_list.c
*
*    Functions for managing the action_info_list.
*
*    FSMLang (fsm) - A Finite State Machine description language.
*    Copyright (C) 2026  Steven Stanton
*
*    This program is free software; you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation; either version 2 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program; if not, write to the Free Software
*    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
*    Steven Stanton
*    fsmlang@pesticidesoftware.com
*
*    For the latest on FSMLang: https://fsmlang.github.io
*
*    And, finally, your possession of this source code implies nothing.
*
*    File created by Steven Stanton
*
*  Long Description:
*
*/

#include "action_info_list.h"

#include <string.h>
#include <stdlib.h>

static bool compose_consolidated_list(pLIST_ELEMENT,void*);
static bool consolidate_records(pLIST_ELEMENT,void*);
static bool free_matrix_list(pLIST_ELEMENT,void*);
static bool check_event_mismatch(pLIST_ELEMENT,void*);
static bool matrices_have_same_events(pMATRIX_INFO,pMATRIX_INFO);

pLIST consolidate_action_info_list(pLIST action_info_list)
{
	pLIST pconsolidated_list = init_list();

	iterate_list(action_info_list
				 , compose_consolidated_list
				 , pconsolidated_list
				 );

	return pconsolidated_list;
}

void free_consolidated_action_info_list(pLIST plist)
{
	if (!plist)
	{
		return;
	}

	iterate_list(plist
				 , free_matrix_list
				 , NULL
				 );

	free_list(plist);
}

static bool compose_consolidated_list(pLIST_ELEMENT pelem, void *data)
{
	pACTION_INFO   pai                = (pACTION_INFO) pelem->mbr;
	pLIST          pconsolidated_list = (pLIST) data;

	// Look for the consolidated record on the list.
	if (NULL == iterate_list(pconsolidated_list, consolidate_records, pai))
	{
		//Not found; create a new consolidated record.
		pCONSOLIDATED_ACTION_INFO pcai = (pCONSOLIDATED_ACTION_INFO) calloc(1, sizeof(CONSOLIDATED_ACTION_INFO));
		pcai->pai      = pai;
		pcai->matrices = init_list();

		add_to_list(pcai->matrices, pai->matrix);

		//Add it to the consolidated list
		add_to_list(pconsolidated_list, pcai);
	}

	return false;
}

static bool consolidate_records(pLIST_ELEMENT pelem, void *data)
{
	pCONSOLIDATED_ACTION_INFO pcai = (pCONSOLIDATED_ACTION_INFO) pelem->mbr;
	pACTION_INFO              pai  = (pACTION_INFO) data;

	bool found = false;

	if ((pcai->pai->action == pai->action)
		&& (pcai->pai->transition == pai->transition)
		&& matrices_have_same_events(pcai->pai->matrix, pai->matrix)
		)
	{
		found = true;

		if (pcai->pai != pai)
		{
			add_unique_to_list(pcai->matrices, pai->matrix);
		}
	}

	return found;
}

static bool check_event_mismatch(pLIST_ELEMENT pelem, void *data)
{
	pLIST_ELEMENT other = (pLIST_ELEMENT) data;

	return (pelem->mbr != other->mbr);
}

static bool matrices_have_same_events(pMATRIX_INFO m1, pMATRIX_INFO m2)
{
	if (m1->event_list->count != m2->event_list->count)
	{
		return false;
	}

	pLIST_ELEMENT e1 = m1->event_list->head;
	pLIST_ELEMENT e2 = m2->event_list->head;

	while (e1 && e2)
	{
		if (check_event_mismatch(e1, e2))
		{
			return false;
		}
		e1 = e1->next;
		e2 = e2->next;
	}

	return true;
}

static bool free_matrix_list(pLIST_ELEMENT pelem, void *data)
{
	(void) data;
	pCONSOLIDATED_ACTION_INFO pcai = (pCONSOLIDATED_ACTION_INFO) pelem->mbr;

	free_list(pcai->matrices);

	return false;
}

