/**
*  fsm_c_switch_common.h
*
*    Functions common to switch and event table
*
*    FSMLang (fsm) - A Finite State Machine description language.
*    Copyright (C) 2024  Steven Stanton
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

#ifndef FSM_C_SWITCH_COMMON_H
#define FSM_C_SWITCH_COMMON_H

#include "fsm_priv.h"
#include "fsm_c_common.h"

typedef struct _cswitch_machine_data_          CSwitchMachineData
                                               , *pCSwitchMachineData
											   ;
typedef struct _cevent_table_machine_data_     CEventTableMachineData
                                               , *pCEventTableMachineData
                                               ;

typedef void (*CEventTableFn)(pCSwitchMachineData);

struct _cevent_table_machine_data_
{
	CEventTableFn writeFSMFn;
};

struct _cswitch_machine_data_
{
	CMachineData           cmd;
	CEventTableMachineData cetmd;
};

void writeCSwitchMachineFN(pFSMOutputGenerator,pMACHINE_INFO);
void cswitchMachineHeaderEnd(pCMachineData,pMACHINE_INFO,bool);
void writeOriginalSwitchFSMLoop(pCMachineData,pMACHINE_INFO);
void writeOriginalSwitchSubFSMLoop(pCMachineData,pMACHINE_INFO);
void cswitchSubMachineHeaderEnd(pCMachineData,pMACHINE_INFO,bool);

#endif

