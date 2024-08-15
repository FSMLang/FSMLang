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

typedef struct _fsm_c_switch_output_generator_    FSMCSwitchOutputGenerator
                                                  , *pFSMCSwitchOutputGenerator
												  ;


typedef void (*COutputFn$fp$pid$pai$pmi)  (FILE*,pID_INFO,pACTION_INFO,pMACHINE_INFO);
typedef void (*COutputFn$fp$ped$pich)     (FILE*,pEVENT_DATA,pITERATOR_CALLBACK_HELPER);
typedef bool (*COutputFn$ple$pv)          (pLIST_ELEMENT,void*);

struct _fsm_c_switch_output_generator_
{
	FSMCOutputGenerator      fsmcog;
	COutputFn$fp$pid$pai$pmi pethbsspe;
	COutputFn$fp$ped$pich    pethbmse;
	COutputFn$ple$pv         pethsc;
};

void writeCSwitchMachineFN(pFSMOutputGenerator,pMACHINE_INFO);
void cswitchMachineHeaderEnd(pCMachineData,pMACHINE_INFO,bool);
void writeOriginalSwitchFSMLoop(pFSMCOutputGenerator);
void writeOriginalSwitchSubFSMLoop(pFSMCOutputGenerator);
void cswitchSubMachineHeaderEnd(pCMachineData,pMACHINE_INFO,bool);

#endif

