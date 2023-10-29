/*****************************************************************************************
*
*    FSMLang (fsm) - A Finite State Machine description language.
*    Copyright (C) 2022  Steven Stanton
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
*    ringwinner@users.sourceforge.net
*
*    For the latest on FSMLang: http://fsmlang.sourceforge.net
*
*    And, finally, your possession of this source code implies nothing.
*
*
*
*   File:                   fsm_cswitch.h
*   Description:            Creates a Switch style C Machine
*
*   Date        Author      Description of change
*   8/24/2022      sstanton  Initial Version
*****************************************************************************************/
#ifndef FSM_CSWITCH_H_
#define FSM_CSWITCH_H_

#include "fsm_priv.h"

pFSMOutputGenerator generateCSwitchMachineWriter(FSMOGF_TYPE);

#endif

