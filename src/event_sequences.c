/**
*  event_sequences.c
*
*    Diagrams event sequences
*
*    FSMLang (fsm) - A Finite State Machine description language.
*    Copyright (C) 2025  Steven Stanton
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

#include "event_sequences.h"
#include "y.tab.h"

TRANSITION_NOTE determine_next_state(pMACHINE_INFO pmi, pEVENT_SEQUENCE_NODE pesn, pSEQUENCE_TRACKER psit)
{
	TRANSITION_NOTE note_on_transition = tn_none;

	pID_INFO ptransition = get_transition(pmi, pesn->pevent->order, psit->pcurr_state->order);

	if (pesn->pnew_state)
	{
		psit->pcurr_state = pesn->pnew_state;
		if (ptransition)
		{
			if (ptransition->type == STATE && ptransition != pesn->pnew_state)
			{
				note_on_transition = tn_state_mismatch;
			}
			else if (ptransition->type != STATE)
			{
				note_on_transition = member_is_in_list(ptransition->transition_fn_returns_decl, pesn->pnew_state) ? tn_fn_match : tn_fn_mismatch;
			}
		}
		else
		{
			note_on_transition = tn_no_fsm_transition;
		}
	}
	else if (ptransition)
	{
		if (STATE == ptransition->type)
		{
			psit->pcurr_state = ptransition;
		}
		else
		{
			psit->pcurr_state = (pID_INFO) find_nth_list_member(ptransition->transition_fn_returns_decl, 0);
			note_on_transition = tn_first_return;
		}
	}

	psit->pcurr_transition = ptransition;
	add_unique_to_list(psit->pvisited_states, psit->pcurr_state);

	return note_on_transition;
}

