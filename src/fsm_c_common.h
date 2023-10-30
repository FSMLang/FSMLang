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
*   File:                   fsm_c_common.h
*   Description:            Functions common to C machine creation.
*
*   Date        Author      Description of change
*   8/24/2022      sstanton  Initial Version
*****************************************************************************************/
#ifndef FSM_C_COMMON_H_
#define FSM_C_COMMON_H_

#include "fsm_priv.h"

typedef struct _fsm_c_output_generator_             FSMCOutputGenerator, *pFSMCOutputGenerator;
typedef struct _fsm_c_sub_machine_output_generator_ FSMCSubMachineOutputGenerator, *pFSMCSubMachineOutputGenerator;
typedef struct _c_machine_data_                     CMachineData,        *pCMachineData;
typedef struct _iterator_callback_helper_ ITERATOR_CALLBACK_HELPER, *pITERATOR_CALLBACK_HELPER;

struct _iterator_callback_helper_
{
   pCMachineData pcmw;
   pMACHINE_INFO pmi;
   char          *cp;
   char          *parent_cp;
   bool          needNoOp;
   bool          first;
   bool          define;  //as opposed to "declare"
   unsigned      counter; //generic name intentional
};

struct _c_machine_data_
{
   FILE	*cFile
      ,	*hFile
      ;

   char	*cName
      ,	*hName
      ;
};

struct _fsm_c_output_generator_
{
   FSMOutputGenerator fsmog;
   pCMachineData      pcmd;
};

struct _fsm_c_sub_machine_output_generator_
{
   FSMOutputGenerator   fsmog;
   pCMachineData        pcmd;
   pFSMCOutputGenerator parent_fsmcog;
};

int  initCMachine(pFSMOutputGenerator,char*);
int  initCMachineFN(pFSMOutputGenerator,char*);
int  initCSubMachine(pFSMOutputGenerator,char*);
int  initCSubMachineFN(pFSMOutputGenerator,char*);
void closeCMachine(pFSMOutputGenerator,int);
void closeCMachineFN(pFSMOutputGenerator,int);

char          * commonHeaderStart(pCMachineData,pMACHINE_INFO,char*);
void            addEventCrossReference(pCMachineData,pMACHINE_INFO,pITERATOR_HELPER);
void            commonHeaderEnd(pCMachineData,pMACHINE_INFO,char*,bool);
void            generateInstance(pCMachineData,pMACHINE_INFO,char*,char*);
void            generateRunFunction(pCMachineData,pMACHINE_INFO,char*);
void            defineWeakActionFunctionStubs(pCMachineData,pMACHINE_INFO,char*);
void            defineWeakNoActionFunctionStubs(pCMachineData,pMACHINE_INFO,char*);
void            defineSubMachineWeakNoActionFunctionStubs(pCMachineData,pMACHINE_INFO,char*);
void            defineWeakStateEntryAndExitFunctionStubs(pCMachineData,pMACHINE_INFO,char*);
void            writeStateTransitions(pCMachineData,pMACHINE_INFO,char*);
void            subMachineWriteStateTransitions(pCMachineData,pMACHINE_INFO,char*);
void            writeDebugInfo(pCMachineData,pMACHINE_INFO,char*);
pCMachineData   newCMachineData(char*);
void            destroyCMachineData(pCMachineData,int);
void            writeCFilePreambles(pCMachineData);
bool            assignExternalEventValues(pMACHINE_INFO);
bool            declare_transition_fn_for_when_actions_return_states(pLIST_ELEMENT,void*);
bool            sub_machine_declare_transition_fn_for_when_actions_return_states(pLIST_ELEMENT,void*);
bool            declare_transition_fn_for_when_actions_return_events(pLIST_ELEMENT,void*);
bool            sub_machine_declare_transition_fn_for_when_actions_return_events(pLIST_ELEMENT,void*);
bool            declare_state_only_transition_functions_for_when_actions_return_states(pLIST_ELEMENT,void*);
bool            sub_machine_declare_state_only_transition_functions_for_when_actions_return_states(pLIST_ELEMENT,void*);
bool            declare_state_only_transition_functions_for_when_actions_return_events(pLIST_ELEMENT,void*);
bool            declare_state_entry_and_exit_functions(pLIST_ELEMENT,void*);
bool            define_state_entry_and_exit_functions(pLIST_ELEMENT,void*);
bool            sub_machine_declare_state_only_transition_functions_for_when_actions_return_events(pLIST_ELEMENT,void*);
bool            declare_data_translator_functions(pLIST_ELEMENT,void*);
bool            sub_machine_declare_data_translator_functions(pLIST_ELEMENT,void*);
bool            define_weak_data_translator_functions(pLIST_ELEMENT,void*);
bool            sub_machine_define_weak_data_translator_functions(pLIST_ELEMENT,void*);

char          * subMachineHeaderStart(pCMachineData,pMACHINE_INFO,char*);
void            subMachineHeaderEnd(pCMachineData,pMACHINE_INFO,char*,bool);
void            defineSubMachineIF(pCMachineData,pMACHINE_INFO,char*);
void            possiblyDefineSubMachineSharedEventStructures(pCMachineData,pMACHINE_INFO,char*);
void            defineSubMachineArray(pCMachineData,pMACHINE_INFO,char*);
bool            declare_sub_machine_action_function(pLIST_ELEMENT,void*);
bool            print_sub_machine_if(pLIST_ELEMENT,void*);
void            defineSubMachineWeakActionFunctionStubs(pCMachineData,pMACHINE_INFO,char*);
void            defineEventPassingActions(pCMachineData,pMACHINE_INFO,char*);
void            defineWeakDataTranslatorStubs(pCMachineData,pMACHINE_INFO,char*);
void            defineSubMachineWeakDataTranslatorStubs(pCMachineData,pMACHINE_INFO,char*);
void            defineSubMachineFinder(pCMachineData,pMACHINE_INFO,char*);
bool            declare_action_function(pLIST_ELEMENT,void*);
void            declareSubMachineManagers(pCMachineData,pMACHINE_INFO,char*);
void            declareStateEntryAndExitManagers(pCMachineData,pMACHINE_INFO,char*);
void            defineStateEntryAndExitManagers(pCMachineData,pMACHINE_INFO,char*);
void            declareEventDataManager(pCMachineData,char*);
void            defineEventDataManager(pCMachineData,pMACHINE_INFO,char*);

#endif

