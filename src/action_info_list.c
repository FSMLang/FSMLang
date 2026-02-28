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

#if defined (CYGWIN) || defined (LINUX)
	#include <stdio.h>
	#include <ctype.h>
	#include <unistd.h>
#endif
#if defined (LINUX) || defined (VS) || defined (CYGWIN)
	#include <time.h>
#endif
#include <string.h>
#include <stdlib.h>

static bool compose_consolidated_list(pLIST_ELEMENT,void*);
static bool consolidate_records(pLIST_ELEMENT,void*);
static bool free_matrix_list(pLIST_ELEMENT,void*);

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
	pLIST_ELEMENT  pconsolidated_elem = NULL;

	// Look for the consolidated record on the list.
	if (NULL == (pconsolidated_elem = iterate_list(pconsolidated_list, consolidate_records, pai)))
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

	if ((pcai->pai->action == pai->action) && (pcai->pai->transition == pai->transition))
	{
		found = true;

		if (pcai->pai != pai)
		{
			add_unique_to_list(pcai->matrices, pai->matrix);
		}
	}

	return found;
}

static bool free_matrix_list(pLIST_ELEMENT pelem, void *data)
{
	(void) data;
	pCONSOLIDATED_ACTION_INFO pcai = (pCONSOLIDATED_ACTION_INFO) pelem->mbr;

	free_list(pcai->matrices);

	return false;
}

