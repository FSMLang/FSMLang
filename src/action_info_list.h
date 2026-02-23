/**
*  action_info_list.h
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

#ifndef ACTION_INFO_LIST_H
#define ACTION_INFO_LIST_H

#include "list.h"
#include "fsm_priv.h"

typedef struct _consolidated_action_info_ CONSOLIDATED_ACTION_INFO, *pCONSOLIDATED_ACTION_INFO;
struct _consolidated_action_info_
{
	pACTION_INFO pai;
	pLIST        matrices;          //!< The list of all matrices (pMATRIC_INFO) having the same action and transition in this pai.
};

pLIST consolidate_action_info_list(pLIST);

#endif

