/**
*  event_sequences.h
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

#ifndef EVENT_SEQUENCES_H
#define EVENT_SEQUENCES_H

#include "fsm_priv.h"
#include "list.h"

typedef enum {tn_none, tn_state_mismatch, tn_fn_mismatch, tn_fn_match, tn_first_return, tn_no_fsm_transition} TRANSITION_NOTE;

typedef struct _sequence_tracker_ SEQUENCE_TRACKER, *pSEQUENCE_TRACKER;

struct _sequence_tracker_
{
	pID_INFO pcurr_state;
	pID_INFO pcurr_transition;
	pLIST    pvisited_states;
};

/**
 * Use the determine the next state as indicated by the event
 * sequence node, or by the machine information.  Return a
 * comment on the source and reliability of the information.
 * 
 * @author Steven Stanton (7/6/2025)
 * 
 * @param pmi   The machine owning the event sequence under
 *  			consideration.
 * @param pesn  The current event sequence node.
 * @param psit  The strucuture tracking the current sequence. 
 * 
 * @return TRANSITION_NOTE 
 */
TRANSITION_NOTE determine_next_state(pMACHINE_INFO pmi, pEVENT_SEQUENCE_NODE pesn, pSEQUENCE_TRACKER psit);

#endif

