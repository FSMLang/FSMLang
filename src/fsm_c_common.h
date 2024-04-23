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

typedef enum ENTRY_OR_EXIT
{
    eoe_entry
    , eoe_exit
} ENTRY_OR_EXIT;

typedef enum DECLARE_OR_DEFINE
{
    dod_declare
    , dod_define
} DECLARE_OR_DEFINE;

typedef struct _fsm_c_output_generator_             FSMCOutputGenerator, *pFSMCOutputGenerator;
typedef struct _fsm_c_sub_machine_output_generator_ FSMCSubMachineOutputGenerator
                                                    , *pFSMCSubMachineOutputGenerator;
typedef struct _c_machine_data_                     CMachineData,        *pCMachineData;
typedef struct _iterator_callback_helper_ ITERATOR_CALLBACK_HELPER, *pITERATOR_CALLBACK_HELPER;

typedef void (*CFSMLoopInnardsWriter)(pCMachineData,pMACHINE_INFO,char*);
typedef void (*COutputFn)            (pCMachineData);
typedef void (*COutputFn$fp$pid$pai$pmi)  (FILE*,pID_INFO,pACTION_INFO,pMACHINE_INFO);
typedef void (*COutputFn$fp$ped$pich)(FILE *, pEVENT_DATA, pITERATOR_CALLBACK_HELPER);
typedef bool (*COutputFn$ple$pv)(pLIST_ELEMENT,void*);


struct _iterator_callback_helper_
{
	ITERATOR_HELPER ih;

	bool          needNoOp;
	bool          define;     //as opposed to "declare"
	unsigned      counter;    //generic name intentional
	pLIST_ELEMENT pOtherElem; //generic name intentional - allows to iterate within iterations to traverse action matrix
	pCMachineData pcmd;

};

/*
Because we now have four files to manage, we
convert to arrays of FILE and char pointers.
*/
typedef enum CREATED_FILES
{
	cf_c
	, cf_first            = cf_c
	, cf_h
	, cf_pubH
	, cf_subMachineH
    , cf_eventsH
	, cf_numCreatedFiles
} CREATED_FILES;

/* These macros allow existing code to remain.*/
#define cFile           file_array[cf_c]
#define hFile           file_array[cf_h]
#define pubHFile        file_array[cf_pubH]
#define subMachineHFile file_array[cf_subMachineH]
#define eventsHFile     file_array[cf_eventsH]

#define cName           file_name_array[cf_c]
#define hName           file_name_array[cf_h]
#define pubHName        file_name_array[cf_pubH]
#define subMachineHName file_name_array[cf_subMachineH]
#define eventsHName     file_name_array[cf_eventsH]

struct _c_machine_data_
{
   FILE	*file_array[cf_numCreatedFiles];
   char	*file_name_array[cf_numCreatedFiles];

   char *action_return_type
	   , *fsm_type
	   , *fsm_data_type
	   , *fsm_fn_type
	   , *event_type
	   , *fsm_fn_event_type
	   , *state_type
	   , *sub_fsm_if_type
	   , *sub_machine_fn_type
	   , *sub_fsm_data_handler_if_stem
	   , *data_translation_fn_type
	   , *shared_event_str_type
	   , *state_fn_type
	   , *event_fn_type
	   , *fq_machine_name              //!< fully qualified name
	   , *nf_machine_name              //!< "nuclear family" - parent and child.
	   , *uc_nf_machine_name           
	   , *action_fn_type
	   , *action_trans_type
	   , *transition_fn_type
       , *uc_machine_name
       , *ucfq_machine_name
	   , *uf_machine_name
      ;

   unsigned long sub_fsm_if_format_width
	   , shared_event_str_format_width
	   , sub_machine_struct_format_width
	   , c_machine_struct_format_width
	   ;

   pCMachineData parent_pcmd;

   /* careful - this only gets set once we start to write the machine */
   pMACHINE_INFO pmi;

   bool a_sub_machine_was_encountered;

   CFSMLoopInnardsWriter    cfsmliw;
   COutputFn                wfsm;
   COutputFn$fp$pid$pai$pmi pethbsspe;
   COutputFn$fp$ped$pich    pethbmse;
   COutputFn$ple$pv         pethsc;
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
   pFSMCOutputGenerator top_level_fsmcog;
   pFSMCOutputGenerator parent_fsmcog;
};

int  initCMachine(pFSMOutputGenerator,char*);
int  initCMachineFN(pFSMOutputGenerator,char*);
int  initCSubMachine(pFSMOutputGenerator,char*);
int  initCSubMachineFN(pFSMOutputGenerator,char*);
void closeCMachine(pFSMOutputGenerator,int);
void closeCMachineFN(pFSMOutputGenerator,int);

void            commonHeaderStart(pCMachineData,pMACHINE_INFO,char*,bool);
void            addEventCrossReference(pCMachineData,pMACHINE_INFO,pITERATOR_CALLBACK_HELPER);
void            commonHeaderEnd(pCMachineData,pMACHINE_INFO,bool);
void            generateInstance(pCMachineData,pMACHINE_INFO,char*);
void            generateRunFunction(pCMachineData,pMACHINE_INFO);
void            defineWeakActionFunctionStubs(pCMachineData,pMACHINE_INFO); void            defineWeakNoActionFunctionStubs(pCMachineData,pMACHINE_INFO);
void            defineWeakStateEntryAndExitFunctionStubs(pCMachineData,pMACHINE_INFO);
void            writeStateTransitions(pCMachineData,pMACHINE_INFO);
void            writeDebugInfo(pCMachineData,pMACHINE_INFO);
pCMachineData   newCMachineData(char*);
void            destroyCMachineData(pCMachineData,int);
void            writeCFilePreambles(pCMachineData,bool);
bool            assignExternalEventValues(pMACHINE_INFO);
bool            declare_transition_fn_for_when_actions_return_states(pLIST_ELEMENT,void*);
bool            declare_transition_fn_for_when_actions_return_events(pLIST_ELEMENT,void*);
bool            declare_state_only_transition_functions_for_when_actions_return_states(pLIST_ELEMENT,void*);
bool            declare_state_only_transition_functions_for_when_actions_return_events(pLIST_ELEMENT,void*);
bool            declare_state_entry_and_exit_functions(pLIST_ELEMENT,void*);
bool            define_state_entry_and_exit_functions(pLIST_ELEMENT,void*);
bool            declare_data_translator_functions(pLIST_ELEMENT,void*);
bool            sub_machine_declare_data_translator_functions(pLIST_ELEMENT,void*);
bool            define_weak_data_translator_functions(pLIST_ELEMENT,void*);
bool            sub_machine_define_weak_data_translator_functions(pLIST_ELEMENT,void*);

void            subMachineHeaderStart(pCMachineData,pMACHINE_INFO,char*);
void            defineSubMachineIF(pCMachineData);
void            possiblyDefineSubMachineSharedEventStructures(pCMachineData,pMACHINE_INFO);
void            defineSubMachineArray(pCMachineData,pMACHINE_INFO);
bool            print_sub_machine_if(pLIST_ELEMENT,void*);
void            defineEventPassingActions(pCMachineData,pMACHINE_INFO);
void            defineWeakDataTranslatorStubs(pCMachineData,pMACHINE_INFO);
void            defineSubMachineWeakDataTranslatorStubs(pCMachineData,pMACHINE_INFO);
void            defineSubMachineFinder(pCMachineData,pMACHINE_INFO);
bool            declare_action_function(pLIST_ELEMENT,void*);
void            declareSubMachineManagers(pCMachineData,pMACHINE_INFO);
void            declareStateEntryAndExitManagers(pCMachineData,pMACHINE_INFO);
void            defineStateEntryAndExitManagers(pCMachineData,pMACHINE_INFO);
void            declareEventDataManager(pCMachineData);
void            defineEventDataManager(pCMachineData,pMACHINE_INFO);
void            printSubMachinesDeclarations(pCMachineData,pMACHINE_INFO);
void            printFSMMachineDebugBlock(pCMachineData,pMACHINE_INFO);
void            printFSMSubMachineDebugBlock(pCMachineData,pMACHINE_INFO);
void            print_action_function_declaration(pCMachineData,char*);
void            print_transition_fn_declaration_for_when_actions_return_states(pCMachineData,FILE*,char*);
void            print_weak_action_function_body_omitting_return_statement(pCMachineData, char *);
void            print_native_epilogue(pCMachineData,pMACHINE_INFO);
void            print_transition_for_assignment_to_state_var(pMACHINE_INFO,pID_INFO,char*,FILE*);
void writeNoTransition(pCMachineData, pMACHINE_INFO);

#endif

