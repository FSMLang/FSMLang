#define FSMLANG_DEVELOP
/**
*  fsm_cswitch.c
*
*    Creates a Switch style C Machine
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
*  Long Description:
*
*   Date        Author      Description of change
*   8/24/2022      sstanton  Initial Version
*/

#include "fsm_cswitch.h"
#include "fsm_c_common.h"
#include "fsm_unused.h"

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

#if defined (CYGWIN) || defined (LINUX)
#include "y.tab.h"
#elif defined (VS)
#include "parser.h"
#endif

typedef enum DECLARE_OR_DEFINE
{
    dod_declare
    , dod_define
} DECLARE_OR_DEFINE;

static void            writeCSwitchMachine(pFSMOutputGenerator,pMACHINE_INFO);
static void            writeCSwitchSubMachine(pFSMOutputGenerator,pMACHINE_INFO);
static void            writeCSwitchMachineFN(pFSMOutputGenerator,pMACHINE_INFO);

static void            defineAllStateHandler(pCMachineData,pMACHINE_INFO);
static void            defineCSwitchMachineFSM(pCMachineData,pMACHINE_INFO);
static void            defineCSwitchSubMachineFSM(pCMachineData,pMACHINE_INFO);
static void            declareCSwitchMachineStruct(pCMachineData,pMACHINE_INFO);
static void            declareCSwitchMachineStateFnArray(pCMachineData,pMACHINE_INFO);
static void            declareCSwitchSubMachineStateFnArray(pCMachineData,pMACHINE_INFO);
static void            writeActionsReturnStateSwitchFSM(pCMachineData, pMACHINE_INFO);
static void            writeReentrantSwitchFSM(pCMachineData,pMACHINE_INFO);
static void            writeOriginalSwitchFSM(pCMachineData,pMACHINE_INFO);
static void            writeOriginalSwitchSubFSM(pCMachineData,pMACHINE_INFO);
static int             writeCSwitchMachineInternal(pCMachineData, pMACHINE_INFO);
static int             writeCSwitchSubMachineInternal(pCMachineData, pMACHINE_INFO);
static void            writeOriginalSwitchFSMLoop(pCMachineData,pMACHINE_INFO);
static void            writeOriginalSwitchFSMLoopInnards(pCMachineData,pMACHINE_INFO,char*);
static void            writeOriginalSwitchSubFSMLoop(pCMachineData,pMACHINE_INFO);
static void            defineCSwitchMachineStateFns(pCMachineData,pMACHINE_INFO);
static void            defineCSwitchSubMachineStateFns(pCMachineData, pMACHINE_INFO, char *);
static void            defineStateFnArray(pCMachineData,pMACHINE_INFO);
static void            cswitchHeaderEnd(pCMachineData,pMACHINE_INFO,bool);
static void            cswitchSubMachineHeaderEnd(pCMachineData,pMACHINE_INFO,bool);
static bool            cswitch_sub_machine_declare_transition_fn_for_when_actions_return_events(pLIST_ELEMENT,void*);
static bool            areTransitionsPossible(pMACHINE_INFO,unsigned);
static bool            print_state_returning_state_fn_signature(pLIST_ELEMENT,void*);
static bool            print_event_returning_state_fn_signature(pLIST_ELEMENT,void*);
static void            start_event_returning_state_fn_signature(pITERATOR_CALLBACK_HELPER);
static void            start_state_returning_state_fn_signature(pITERATOR_CALLBACK_HELPER);
static bool            print_void_returning_state_fn_signature(pLIST_ELEMENT,void*);
static void            print_final_state_fn_signature_fragment(pITERATOR_CALLBACK_HELPER);
static bool            print_state_fn_name(pLIST_ELEMENT,void*);
static void            declareOrDefineSinglePAIEventHandler(pMachineData,pMACHINE_INFO,DECLARE_OR_DEFINE);
static bool            find_first_array_element_with_transition(pLIST_ELEMENT,void*);
static void            print_state_fn_epilogue(pCMachineData,pMACHINE_INFO,pID_INFO,bool);
static bool            define_event_returning_state_fn(pLIST_ELEMENT,void*);
static bool            define_state_returning_state_fn(pLIST_ELEMENT,void*);
static bool            define_void_returning_state_fn(pLIST_ELEMENT,void*);
static bool            print_event_returning_state_fn_case(pLIST_ELEMENT,void*);
static bool            print_void_returning_state_fn_case(pLIST_ELEMENT,void*);
static bool            print_state_returning_state_fn_case(pLIST_ELEMENT,void*);


FSMCOutputGenerator CSwitchMachineWriter = {
   {
      initCMachine,
      writeCSwitchMachine,
      closeCMachine
   },
   NULL
};

/**
  This function writes the ActionsReturnState Switch FSM
*/
static void writeActionsReturnStateSwitchFSM(pCMachineData pcmw, pMACHINE_INFO pmi)
{
   char *tabstr = "";

   fprintf(pcmw->cFile, "\t%s_STATE s%s;\n"
           , cp
           , (pmi->machineTransition || pmi->states_with_entry_fns_count || pmi->states_with_exit_fns_count)
             ? " = THIS(noTransition)" 
             : ""
           );

   fprintf(pcmw->cFile, "\n\tDBG_PRINTF(\"event: %%s; start state: %%s\"\n\t\t,");
   printAncestry(pmi, pcmw->cFile, alc_upper, ai_include_self);
   fprintf(pcmw->cFile, "_EVENT_NAMES[event]\n\t\t,");
   printAncestry(pmi, pcmw->cFile, alc_upper, ai_include_self);
   fprintf(pcmw->cFile, "_STATE_NAMES[pfsm->state]\n\t\t);\n\n");

   if (pmi->data_block_count)
   {
      fprintf(pcmw->cFile
              , "\ttranslateEventData(&pfsm->data, event);\n\n"
              );
   }

   if (pmi->has_single_pai_events)
   {
      fprintf(pcmw->cFile
              , "\t\tif (eventIsNotHandledInAllStates(pfsm,e))\n"
              );
      tabstr = "\t";
   }

   fprintf(pcmw->cFile
           , "\t\t%ss = ((* (*pfsm->statesArray)[pfsm->state])(pfsm,e));\n\n"
           , tabstr
           );

   fprintf(pcmw->cFile, "\tif (s != ");
   printAncestry(pmi, pcmw->cFile, alc_lower, ai_include_self);
   fprintf(pcmw->cFile, "%s_noTransition)\n\t{\n");

   if (pmi->machineTransition || pmi->states_with_entry_fns_count || pmi->states_with_exit_fns_count)
   {
      fprintf(pcmw->cFile
              , "\t\tif (s != pfsm->state)\n\t\t{\n"
             );

      if (pmi->machineTransition)
      {
         fprintf(pcmw->cFile, "\t\t\t");
		 printAncestry(pmi, pcmw->cFile, alc_lower, ai_include_self);
		 fprintf(pcmw->cFile
				 , "_%s(pfsm,s);\n"
                 , pmi->machineTransition->name
                );
      }

      if (pmi->states_with_exit_fns_count)
      {
         fprintf(pcmw->cFile
                 ,"\t\t\trunAppropriateExitFunction(%spfsm->state);\n"
                 , pmi->data ? "&pfsm->data, " : ""
                 );
      }

      if (pmi->states_with_entry_fns_count)
      {
         fprintf(pcmw->cFile
                 ,"\t\t\trunAppropriateEntryFunction(%ss);\n"
                 , pmi->data ? "&pfsm->data, " : ""
                 );
      }

      fprintf(pcmw->cFile
              , "\t\t}\n"
              );

   }

   fprintf(pcmw->cFile
           , "\t\tpfsm->state = s;\n\t}\n\n"
          );

   fprintf(pcmw->cFile, "\n\tDBG_PRINTF(\"end state: %%s\"\n\t\t,");
   printAncestry(pmi, pcmw->cFile, alc_upper, ai_include_self);
   fprintf(pcmw->cFile, "_STATE_NAMES[pfsm->state]\n\t\t);\n");

}

static void writeCSwitchMachine(pFSMOutputGenerator pfsmog, pMACHINE_INFO pmi)
{
   pFSMCOutputGenerator pfsmcog = (pFSMCOutputGenerator) pfsmog;

   writeCSwitchMachineInternal(pfsmcog->pcmd, pmi);

   if (pmi->machine_list)
   {
      write_machines(pmi->machine_list, generateCSwitchMachineWriter);
   }
}

static void writeCSwitchSubMachine(pFSMOutputGenerator pfsmog, pMACHINE_INFO pmi)
{

   pFSMCOutputGenerator pfsmcog = (pFSMCOutputGenerator) pfsmog;

   writeCSwitchSubMachineInternal(pfsmcog->pcmd, pmi);

   if (pmi->machine_list)
   {
      write_machines(pmi->machine_list, generateCSwitchMachineWriter);
   }
}

static int writeCSwitchMachineInternal(pCMachineData pcmw, pMACHINE_INFO pmi)
{
   if (!pmi || !pcmw) return 1;

   ITERATOR_CALLBACK_HELPER ich = {
       .pmi    = pmi
       , .pcmw = pcmw
   };

   commonHeaderStart(pcmw, pmi, "state_fn");

   declareCSwitchMachineStateFnArray(pcmw, pmi);

   declareCSwitchMachineStruct(pcmw, pmi);

   /* declare state fns */
   iterate_list(pmi->state_list
                , pmi->modFlags & mfActionsReturnVoid 
                  ? print_void_returning_state_fn_signature 
                  : pmi->modFlags & mfActionsReturnState
                    ? print_state_returning_state_fn_signature
                    : print_event_returning_state_fn_signature
                , &ich
                );

   fprintf(pcmw->hFile, "\n");

   cswitchHeaderEnd(pcmw, pmi, false);

   /*
     Source File
   */

   addNativeImplementationIfThereIsAny(pmi, pcmw->cFile);

   defineStateFnArray(pcmw, pmi);

   defineSubMachineArray(pcmw, pmi);

   if (generate_instance)
   {
      generateInstance(pcmw, pmi, "state_fn");
   }

   if (generate_run_function)
   {
      generateRunFunction(pcmw, pmi);
   }

   defineCSwitchMachineFSM(pcmw, pmi);

   /* write our sub-machine lookup, if needed */
   if (pmi->machine_list)
   {
      defineSubMachineFinder(pcmw, pmi);
   }

   if (pmi->has_single_pai_events)
   {
      defineAllStateHandler(pcmw, pmi);
   }

   defineStateEntryAndExitManagers(pcmw, pmi);

   if (pmi->data_block_count)
   {
      defineEventDataManager(pcmw, pmi);
   }

   defineCSwitchMachineStateFns(pcmw, pmi);

   if (generate_weak_fns)
   {

      defineWeakActionFunctionStubs(pcmw, pmi);

      defineWeakStateEntryAndExitFunctionStubs(pcmw, pmi);

      if (pmi->data_block_count)
      {
         defineWeakDataTranslatorStubs(pcmw, pmi);
      }

   }
   else if (force_generation_of_event_passing_actions)
   {
      defineEventPassingActions(pcmw, pmi);
   }

   writeDebugInfo(pcmw, pmi, cp);

   FREE_AND_CLEAR(cp);

   return 0;
}

static void print_final_state_fn_signature_fragment(pITERATOR_CALLBACK_HELPER pich)
{
    FILE *file = pich->define ? pich->pcmw->cFile : pich->pcmw->hFile;

    printNameWithAncestry(state->name, pich->pcmw->hFile, "_", alc_lower, ai_include_self);
    fprintf(file, " _stateFn(p");
    streamHungarianToUnderbarCaps(file, pich->pmi->name->name);
    fprintf(file, ",");
    streamHungarianToUnderbarCaps(file, ultimateAncestor(pich->pmi));
    fprintf(file
            , "_EVENT%s)%s"
            , ulitmateAncestor(pich->pmi)->data_block_count ? "_ENUM" : ""
            , pich->define ? "\n{\n" : ";\n"
           );
}

static bool print_void_returning_state_fn_signature(pLIST_ELEMENT pelem, void *data)
{
    pID_INFO                  pstate = (pID_INFO) pelem->mbr;
    pITERATOR_CALLBACK_HELPER pich   = (pITERATOR_CALLBACK_HELPER) data;

    FILE *file = pich->define ? pich->pcmw->cFile : pich->pcmw->hFile;

    fprintf(file, "static void ");

    print_final_state_fn_signature_fragment(pich);

    return false;
}

static void start_event_returning_state_fn_signature(pITERATOR_CALLBACK_HELPER pich)
{
    pMACHINE_INFO             pua    = ultimateAncestor(pich->pmi);

    FILE *file = pich->define ? pich->pcmw->cFile : pich->pcmw->hFile;

    fprintf(file, "static ");
    printAncestry(pich->pmi, pich->pcmw->hFile, "_", alc_upper, ai_include_self);
    fprintf(file
            , "_EVENT%s"
            , (pua->data_block_count && !(pua->modFlags & mfActionsReturnStates))? "_ENUM"  : ""
            );
}

static bool print_event_returning_state_fn_signature(pLIST_ELEMENT pelem, void *data)
{
    pID_INFO                  pstate = (pID_INFO) pelem->mbr;
    pITERATOR_CALLBACK_HELPER pich   = (pITERATOR_CALLBACK_HELPER) data;

    start_event_returning_state_fn_signature(pich);
    print_final_state_fn_signature_fragment(pich);

    return false;
}

static void start_state_returning_state_fn_signature(pITERATOR_CALLBACK_HELPER pich)
{
    FILE *file = pich->define ? pich->pcmw->cFile : pich->pcmw->hFile;

    fprintf(file, "static ");
    streamHungarianToUnderbarCaps(file, pich->pmi->name->name);
    fprintf(file, "_STATE");
}

static bool print_state_returning_state_fn_signature(pLIST_ELEMENT pelem, void *data)
{
    pID_INFO                  pstate = (pID_INFO) pelem->mbr;
    pITERATOR_CALLBACK_HELPER pich   = (pITERATOR_CALLBACK_HELPER) data;

    start_state_returning_state_fn_signature(pich);
    print_final_state_fn_signature_fragment(pich);

    return false;
}

static int writeCSwitchSubMachineInternal(pCMachineData pcmw, pMACHINE_INFO pmi)
{
   if (!pmi || !pcmw) return 1;

   ITERATOR_CALLBACK_HELPER ich = {
       .pmi      = pmi
       , .pcmw   = pcmw
       , .define = false
   };

   subMachineHeaderStart(pcmw, pmi, "state_fn");

   declareCSwitchSubMachineStateFnArray(pcmw, pmi);

   declareCSwitchMachineStruct(pcmw, pmi);

   /* declare state fns */
   iterate_list(pmi->state_list
                , pmi->modFlags & mfActionsReturnVoid 
                  ? print_void_returning_state_fn_signature 
                  : pmi->modFlags & mfActionsReturnState
                    ? print_state_returning_state_fn_signature
                    : print_event_returning_state_fn_signature
                , &ich
                );

   fprintf(pcmw->hFile, "\n");

   cswitchSubMachineHeaderEnd(pcmw, pmi, false);

   /*
     Source File
   */

   addNativeImplementationIfThereIsAny(pmi, pcmw->cFile);

   defineStateFnArray(pcmw, pmi);

   defineSubMachineIF(pcmw, pmi);

   possiblyDefineSubMachineSharedEventStructures(pcmw, pmi);

   defineSubMachineArray(pcmw, pmi);

   if (generate_instance)
   {
      generateInstance(pcmw, pmi, "state_fn");
   }

   defineCSwitchSubMachineFSM(pcmw, pmi);

   if (pmi->has_single_pai_events)
   {
      defineAllStateHandler(pcmw, pmi, parent_cp);
   }

   defineStateEntryAndExitManagers(pcmw, pmi, cp);

   defineCSwitchSubMachineStateFns(pcmw, pmi, cp);

   if (generate_weak_fns)
   {
      defineWeakActionFunctionStubs(pcmw, pmi, cp);

      defineSubMachineWeakDataTranslatorStubs(pcmw, pmi, cp);

      defineWeakStateEntryAndExitFunctionStubs(pcmw, pmi, cp);

   }
   else if (force_generation_of_event_passing_actions)
   {
      defineEventPassingActions(pcmw, pmi, cp);
   }

   writeDebugInfo(pcmw, pmi, cp);

   FREE_AND_CLEAR(cp);
   FREE_AND_CLEAR(parent_cp);

   return 0;
}

static void declareCSwitchMachineStateFnArray(pCMachineData pcmw, pMACHINE_INFO pmi)
{
   if (pmi->modFlags & mfActionsReturnVoid)
   {
      fprintf(pcmw->hFile, "typedef void (*");
	  printAncestry(pmi, pcmw->hFile, alc_upper, ai_include_self);
	  fprintf(pcmw->hFile "_STATE_FN)(p");
	  printAncestry(pmi, pcmw->hFile, alc_upper, ai_include_self);
	  fprintf(pcmw->hFile, ",");
	  printAncestry(pmi, pcmw->hFile, alc_upper, ai_include_self);
	  fprintf(pcmw->hFile
			  , "_EVENT%s);\n\n"
              , ultimateAncestor(pmi)->data_block_count ? "_ENUM"  : ""
             );
   }
   else
   {
      fprintf(pcmw->hFile
              , "typedef %s_%s%s (*%s_STATE_FN)(p%s,%s_EVENT%s);\n\n"
              , cp
              , (pmi->modFlags & mfActionsReturnStates) ? "STATE" : "EVENT"
              , (pmi->data_block_count && !(pmi->modFlags & mfActionsReturnStates))? "_ENUM"  : ""
              , cp
              , cp
              , cp
              , pmi->data_block_count ? "_ENUM"  : ""
             );
   }

   fprintf(pcmw->hFile
           , "extern const %s_STATE_FN %s_state_fn_array[%s_numStates];\n\n"
           , cp
           , pmi->name->name
           , pmi->name->name
          );
}

static void declareCSwitchSubMachineStateFnArray(pCMachineData pcmw, pMACHINE_INFO pmi)
{
   fprintf(pcmw->hFile, "typedef ");
   if (pmi->modFlags & mfActionsReturnVoid)
   {
      fprintf(pcmw->hFile, "void (*");
	  printAncestry(pmi, pcmw->hFile, "_", alc_upper, ai_include_self);
      fprintf(pcmw->hFile, "_STATE_FN)(p");
	  printAncestry(pmi, pcmw->hFile, "_", alc_upper, ai_include_self);
      fprintf(pcmw->hFile, ",");
	  streamHungarianToUnderbarCaps(pcmw->hFile, ultimateAncestor(pmi));
      fprintf(pcmw->hFile
			  , "_EVENT%s);\n\n"
              , ultimateAncestor(pmi)->parent->data_block_count ? "_ENUM"  : ""
             );
   }
   else
   {
	   if (pmi->modFlags & mfActionsReturnStates)
	   {
		   printAncestry(pmi, pcmw->hFile, "_", alc_upper, ai_include_self);
		   fprintf(pcmw->hFile, "_STATE (*");
	   }
	   else
	   {
		   streamHungarianToUnderbarCaps(pcmw->hFile, ultimateAncestor(pmi));
		   fprintf(pcmw->hFile
				   , "_EVENT%s (*"
				   , ultimateAncestor(pmi)->data_block_count ? "_ENUM" : ""
	   }
	   printAncestry(pmi, pcmw->hFile, "_", alc_upper, ai_include_self);
	   fprintf(pcmw->hFile, "_STATE_FN)(p");
	   printAncestry(pmi, pcmw->hFile, "_", alc_upper, ai_include_self);
	   fprintf(pcmw->hFile, ",");
	   streamHungarianToUnderbarCaps(pcmw->hFile, ultimateAncestor(pmi));
	   fprintf(pcmw->hFile
			   , "_EVENT%s);\n\n"
              , ultimateAncestor(pmi)->parent->data_block_count ? "_ENUM"  : ""
             );
   }

   fprintf(pcmw->hFile, "extern const ");
   printAncestry(pmi, pcmw->hFile, "_", alc_upper, ai_include_self);
   fprintf(pcmw->hFile, "_STATE_FN ");
   printAncestry(pmi, pcmw->hFile, "_", alc_lower, ai_include_self);
   fprintf(pcmw->hFile, "_state_fn_array[");
   printAncestry(pmi, pcmw->hFile, "_", alc_lower, ai_include_self);
   fprintf(pcmw->hFile, "_numStates];\n\n");

}

static void declareCSwitchMachineStruct(pCMachineData pcmw, pMACHINE_INFO pmi)
{
   /* put the machine structure definition into the header */
   fprintf(pcmw->hFile, "struct _");
   printAncestry(pmi, pcmw->hFile, alc_lower, ai_include_self);
   fprintf(pcmw->hFile "_struct_ {\n",);

   if (pmi->data)
   {
      fprintf(pcmw->hFile, "\t");
	  printAncestry(pmi, pcmw->hFile, alc_upper, ai_include_self);
	  fprintf(pcmw->hFile, "_DATA\t\t\t\t\tdata;\n");
   }

   fprintf(pcmw->hFile, "\t");
   printAncestry(pmi, pcmw->hFile, alc_upper, ai_include_self);
   fprintf(pcmw->hFile, "_STATE\t\t\t\t\tstate;\n");

   fprintf(pcmw->hFile, "\t");
   streamHungarianToUnderbarCaps(pcmw->hFile, ultimateAncestor(pmi)->name->name);
   fprintf(pcmw->hFile
		   , "_EVENT%s\t\t\t\t\tevent;\n"
           , ulitmateAncestor(pmi)->data_block_count ? "_ENUM"  : ""
           );

   fprintf(pcmw->hFile, "\t");
   printAncestry(pmi, pcmw->hFile, alc_upper, ai_include_self);
   fprintf(pcmw->hFile "_STATE_FN const\t(*statesArray)[");
   printAncestry(pmi, pcmw->hFile, alc_lower, ai_include_self);
   fprintf(pcmw->hFile, "_numStates];\n");

   if (pmi->machine_list)
   {
      fprintf(pcmw->hFile, "\tp");
	  printAncestry(pmi, pcmw->hFile, alc_upper, ai_include_self);
	  fprintf(pcmw->hFile, "_SUB_FSM_IF\t(*subMachineArray)[");
	  printAncestry(pmi, pcmw->hFile, alc_lower, ai_include_self);
	  fprintf(pcmw->hFile, "_numSubMachines];\n");
   }

   fprintf(pcmw->hFile, "\t");
   printAncestry(pmi, pcmw->hFile, alc_upper, ai_include_self);
   fprintf(pcmw->hFile, "_FSM\t\t\t\t\t\tfsm;\n};\n\n",);

}

static void declareOrDefineSinglePAIEventHandler(pMachineData pcmw, pMACHINE_INFO pmi, DECLARE_OR_DEFINE dod)
{
    fprintf(pcmw->cFile, "static ");
    if (pmi->modFlags & ACTIONS_RETURN_FLAGS)
    {
        fprintf(pcmw->cFile, "bool eventIsNotHandledInAllStates(p");
    }
    else
    {
        streamHungarianToUnderbarCaps(pcmw->cFile, ultimateAncestor(pmi)->name->name);
        fprintf(pcmw->cFile
                , "_EVENT%s checkWhetherEventIsHandledInAllStates(p"
                , ultimateAncestor(pmi)->data_block_count ? "_ENUM" : ""
               );
    }
    printAncestry(pmi, pcmw->cFile, "_", alc_upper, ai_include_self);
    fprintf(pcmw->cFile
            , " %s,"
            , dod == dod_declare ? "" : "pfsm"
            );
    streamHungarianToUnderbarCaps(pcmw->cFile, ultimateAncestor(pmi)->name->name);
    fprintf(pcmw->cFile
            , "_EVENT%s%s%s)%s"
            , ultimateAncestor(pmi)->data_block_count ? "_ENUM" : ""
            , dod == dod_declare ? ""  : " "
            , dod == dod_define  ? "e" : ""
            , dod == dod_declare ? ";\n" : "\n{\n"
           );

}

static void defineCSwitchMachineFSM(pCMachineData pcmw, pMACHINE_INFO pmi)
{
   if (pmi->machine_list)
   {
      declareSubMachineManagers(pcmw, pmi);
   }

   if (pmi->data_block_count)
   {
      declareEventDataManager(pcmw);
   }

   if (pmi->has_single_pai_events)
   {
       declareOrDefineSinglePAIEventHandler(pcmw, pmi, dod_declare);
   }

   declareStateEntryAndExitManagers(pcmw, pmi);

   fprintf(pcmw->cFile, "\n");

   fprintf(pcmw->cFile
           , "#ifndef EVENT_IS_NOT_EXCLUDED_FROM_LOG\n"
           );

   fprintf(pcmw->cFile
           , "#define EVENT_IS_NOT_EXCLUDED_FROM_LOG(e) ((e) == (e))\n"
           );

   fprintf(pcmw->cFile
           , "#endif\n"
           );

   fprintf(pcmw->cFile
           , "void %sFSM(p"
           , pmi->name->name
           );
   streamHungarianToUnderbarCaps(pcmw->cFile, pmi->name->name);
   fprintf(pcmw->cFile
           , " pfsm, %s"
           , pmi->data_block_count ? "p"  : ""
          );
   streamHungarianToUnderbarCaps(pcmw->cFile, pmi->name->name);
   fprintf(pcmw->cFile, "_EVENT event)\n{\n");

   if (!(pmi->modFlags & mfActionsReturnVoid))
   {
      fprintf(pcmw->cFile, "\t");
      printAncestry(pmi, pcmw->cFile, "_", alc_upper, ai_include_self);
      fprintf(pcmw->cFile
              , "_EVENT%s e = event%s;\n\n"
              , pmi->data_block_count ? "_ENUM"  : ""
              , pmi->data_block_count ? "->event" : ""
             );
   }

   if      (pmi->modFlags & mfReentrant)           writeReentrantSwitchFSM(pcmw, pmi);
   else if (pmi->modFlags & mfActionsReturnStates) writeActionsReturnStateSwitchFSM(pcmw, pmi);
   else                                            writeOriginalSwitchFSM(pcmw, pmi, cp);

   fprintf(pcmw->cFile, "\n}\n\n");

}

static void defineCSwitchSubMachineFSM(pCMachineData pcmw, pMACHINE_INFO pmi)
{
   if (pmi->has_single_pai_events)
   {
       declareOrDefineSinglePAIEventHandler(pcmw, pmi, dod_declare);
   }

   fprintf(pcmw->cFile, "\n");

   fprintf(pcmw->cFile
           , "#ifndef EVENT_IS_NOT_EXCLUDED_FROM_LOG\n"
           );

   fprintf(pcmw->cFile
           , "#define EVENT_IS_NOT_EXCLUDED_FROM_LOG(e) (e == e)\n"
           );

   fprintf(pcmw->cFile
           , "#endif\n"
           );

   declareStateEntryAndExitManagers(pcmw, pmi);

   streamHungarianToUnderbarCaps(pcmw->cFile, ultimateAncestor(pmi)->name->name);
   fprintf(pcmw->cFile
           , "_EVENT%s "
           , ultimateAncestor(pmi)->data_block_count ? "_ENUM"  : ""
          );
   printAncestry(pmi, pcmw->cFile, "_", alc_lower, ai_include_self);
   fprintf(pcmw->cFile, "FSM(p");
   printAncestry(pmi, pcmw->cFile, "_", alc_upper, ai_include_self);
   fprintf(pcmw->cFile, " pfsm, ");
   streamHungarianToUnderbarCaps(pcmw->cFile, pmi->name->name);
   fprintf(pcmw->cFile
           , "_EVENT%s event)\n{\n"
           , ulitmateAncestor(pmi)->data_block_count ? "_ENUM"  : ""
          );

   if (!(pmi->modFlags & mfActionsReturnVoid))
   {
      fprintf(pcmw->cFile, "\t");
      streamHungarianToUnderbarCaps(pcmw->cFile, pmi->name->name);
      fprintf(pcmw->cFile
              , "_EVENT%s e = event;\n\n"
              , ultimateAncestor(pmi)->data_block_count ? "_ENUM"  : ""
             );
   }

   if      (pmi->modFlags & mfReentrant)           writeReentrantSwitchFSM(pcmw, pmi);
   else if (pmi->modFlags & mfActionsReturnStates) writeActionsReturnStateSwitchFSM(pcmw, pmi);
   else                                            writeOriginalSwitchSubFSM(pcmw, pmi);

   fprintf(pcmw->cFile
           , "\n}\n\n"
          );

}

static bool define_void_returning_state_fn(pLIST_ELEMENT pelem, void *data)
{
    pID_INFO                  pstate = (pID_INFO) pelem->mbr;
    pITERATOR_CALLBACK_HELPER pich   = (pITERATOR_CALLBACK_HELPER) data;

    pLIST_ELEMENT             ptransitionEvent;

    fprintf(pich->pcmw->cFile, "static void ");
    print_final_state_fn_signature_fragment(pich);

    /* are transitions possible? */
    pich->pOtherElem = pelem;
    ptransitionEvent = iterate_list(pich->pmi->event_list, find_first_array_element_with_transition, pich);

    if (
        (pich->pmi->machineTransition || pich->pmi->states_with_entry_fns_count || pich->pmi->states_with_exit_fns_count)
        && ptransitionEvent
       )
    {
        fprintf(pich->pcmw->cFile,"\t");
        printNameWithAncestry("STATE", pich->pmi, pich->pcmw->cFile, "_", alc_upper, ai_include_self);
        fprintf(pich->pcmw->cFile, " new_s = ");
        printNameWithAncestry(pstate->name, pich->pmi, pich->pcmw->cFile, "_", alc_lower, ai_include_self);
        fprintf(pich->pcmw->cFile, ";\n");
    }

    fprintf(pich->pcmw->cFile, "\n\tswitch(e)\n\t{\n");

    pich->counter = 0;
    iterate_list(pich->pmi->event_list, print_event_returning_state_fn_case, pich);

    if (pich->counter < pich->pmi->event_list->count + 1)
    {
        fprintf(pcmw->cFile
                , "\tdefault:\n\t\t%s(\""
                , core_logging_only ? "NON_CORE_DEBUG_PRINTF" : "DBG_PRINTF"
               );
        printNameWithAncestry("noAction", pich->pmi, pich->pcmw->cFile, "_", alc_lower, ai_include_self);
        fprintf(pich->pcmw->cFile, "\");\n\t\tbreak;\n"
                , pmi->name->name
               );
    }

    if (!pich->counter)
    {
        fprintf(pich->pcmw->cFile
                , "/* we only just now know we did not use this */\n\t(void) pfsm;\n"
                );
    }

    fprintf(pcmw->cFile, "\t}\n");

    print_state_fn_epilogue(pich->pcmw, pich->pmi, pstate, ptransitionEvent != NULL);
    
    return false;

}

static void print_state_fn_epilogue(pCMachineData pcmd, pMACHINE_INFO pmi, pID_INFO pstate, bool transitions_are_possible)
{
    if (
        (pmi->machineTransition || pmi->states_with_entry_fns_count || pmi->states_with_exit_fns_count)
        && transitions_are_possible
        )
    {
       fprintf(pcmd->cFile, "\n\n\tif (");
       printNameWithAncestry(pstate->name, pmi, pcmd->cFile, "_", alc_lower, ai_include_self);
       fprintf(pcmd->cFile, " != new_s)\n\t{\n");

       if (pmi->machineTransition)
       {
          fprintf(pcmd->cFile, "\t\t");
          printNameWithAncestry(pmi->machineTransition->name, pmi, pcmd->cFile, "_", alc_lower, ai_include_self);
          fprintf(pcmd->cFile, "(pfsm,new_s);\n");
       }

       if (pmi->states_with_exit_fns_count)
       {
          fprintf(pcmd->cFile
                  ,"\t\trunAppropriateExitFunction(%s"
                  , pmi->data ? "&pfsm->data, " : ""
                 );
          printNameWithAncestry(pstate->name, pmi, pcmd->cFile, "_", alc_lower, ai_include_self);
          fprintf(pcmd->cFile, "%s_%s);\n");
       }

       if (pmi->states_with_entry_fns_count)
       {
           fprintf(pcmd->cFile
                   ,"\t\trunAppropriateEntryFunction(%s"
                   , pmi->data ? "&pfsm->data, " : ""
                  );
           printNameWithAncestry(pstate->name, pmi, pcmd->cFile, "_", alc_lower, ai_include_self);
           fprintf(pcmd->cFile, "%s_%s);\n");
       }

       fprintf(pcmd->cFile
               , "\t\tpfsm->state = new_s;\n\n"
              );

       fprintf(pcmd->cFile
               , "\t}\n\n"
              );
    }

    if (!(pmi->modFlags & mfActionsReturnVoid))
    {
       fprintf(pcmd->cFile
               , "\n\treturn retVal;\n"
              );
    }

    fprintf(pcmd->cFile, "}\n\n");
}

static bool define_event_returning_state_fn(pLIST_ELEMENT pelem, void *data)
{
    pID_INFO                  pstate = (pID_INFO)                  pelem->mbr;
    pITERATOR_CALLBACK_HELPER pich   = (pITERATOR_CALLBACK_HELPER) data;

    pLIST_ELEMENT             ptransitionEvent;

    start_event_returning_state_fn_signature(pich);
    print_final_state_fn_signature_fragment(pich);

    fprintf(pich->pcmw->cFile, "\t");
    streamHungarianToUnderbarCaps(pich->pcmw->cFile, ultimateAncestor(pich->pmi)->name->name);
    fprintf(pich->pcmw->cFile
              "_EVENT%s retVal = "
            , ultimateAncestor(pich->pmi)->data_block_count ? "_ENUM"  : ""
           );
    printNameWithAncestry("noEvent", pich->pmi, pich->pcmw->cFile, "_", alc_lower, ai_include_self);
    fprintf(pich->pcmw->cFile, ";\n");

    /* are transitions possible? */
    pich->pOtherElem = pelem;
    ptransitionEvent = iterate_list(pich->pmi->event_list, find_first_array_element_with_transition, pich);

    if (
        (pich->pmi->machineTransition || pich->pmi->states_with_entry_fns_count || pich->pmi->states_with_exit_fns_count)
        && ptransitionEvent
       )
    {
        fprintf(pich->pcmw->cFile,"\t");
        printNameWithAncestry("STATE", pich->pmi, pich->pcmw->cFile, "_", alc_upper, ai_include_self);
        fprintf(pich->pcmw->cFile, " new_s = ");
        printNameWithAncestry(pstate->name, pich->pmi, pich->pcmw->cFile, "_", alc_lower, ai_include_self);
        fprintf(pich->pcmw->cFile, ";\n");
    }

    fprintf(pich->pcmw->cFile, "\n\tswitch(e)\n\t{\n");

    pich->counter = 0;
    iterate_list(pich->pmi->event_list, print_event_returning_state_fn_case, pich);

    if (pich->counter < pich->pmi->event_list->count + 1)
    {
        fprintf(pcmw->cFile
                , "\tdefault:\n\t\t%s(\""
                , core_logging_only ? "NON_CORE_DEBUG_PRINTF" : "DBG_PRINTF"
               );
        printNameWithAncestry("noAction", pich->pmi, pich->pcmw->cFile, "_", alc_lower, ai_include_self);
        fprintf(pich->pcmw->cFile, "\");\n\t\tbreak;\n"
                , pmi->name->name
               );
    }

    if (!pich->counter)
    {
        fprintf(pich->pcmw->cFile
                , "/* we only just now know we did not use this */\n\t(void) pfsm;\n"
                );
    }

    fprintf(pcmw->cFile, "\t}\n");

    print_state_fn_epilogue(pich->pcmw, pich->pmi, pstate, ptransitionEvent != NULL);

    return false;

}

static bool define_state_returning_state_fn(pLIST_ELEMENT pelem, void *data)
{
    pID_INFO                  pstate = (pID_INFO) pelem->mbr;
    pITERATOR_CALLBACK_HELPER pich   = (pITERATOR_CALLBACK_HELPER) data;

    start_state_returning_state_fn_signature(pich);
    print_final_state_fn_signature_fragment(pich);

    //do the body
    fprintf(pich->pcmw->cFile, "\t");
    streamHungarianToUnderbarCaps(pich->pcmw->cFile, pich->pmi->name->name);
    fprintf(pich->pcmw->cFile
            , "_STATE retVal = %s_noTransition;\n"
            , pmi->name->name
           );

    /* are transitions possible? */
    pich->pOtherElem = pelem;
    ptransitionEvent = iterate_list(pich->pmi->event_list, find_first_array_element_with_transition, pich);

    if (
        (pich->pmi->machineTransition || pich->pmi->states_with_entry_fns_count || pich->pmi->states_with_exit_fns_count)
       )
    {
        fprintf(pich->pcmw->cFile,"\t");
        printNameWithAncestry("STATE", pich->pmi, pich->pcmw->cFile, "_", alc_upper, ai_include_self);
        fprintf(pich->pcmw->cFile, " new_s = ");
        printNameWithAncestry(pstate->name, pich->pmi, pich->pcmw->cFile, "_", alc_lower, ai_include_self);
        fprintf(pich->pcmw->cFile, ";\n");
    }

    fprintf(pich->pcmw->cFile, "\n\tswitch(e)\n\t{\n");
    
    pich->counter = 0;
    iterate_list(pich->pmi->event_list, print_event_returning_state_fn_case, pich);

    fprintf(pcmw->cFile, "\t}\n");

    print_state_fn_epilogue(pich->pcmw, pich->pmi, pstate, true);

    return false;

}

static void defineCSwitchMachineStateFns(pCMachineData pcmw, pMACHINE_INFO pmi)
{
    ITERATOR_CALLBACK_HELPER ich = {
        .counter = 0
        , .define = true
        , .pcmw   = pcmw
        , .pmi    = pmi
    }

   iterate_list(pmi->state_list
                , pmi->modFlags & mfActionsReturnVoid
                  ? define_void_returning_state_fn
                  : pmi->modFlags & mfActionsReturnState
                    ? define_state_returning_state_fn
                    : define_event_returning_state_fn
                , &ich
                )

   fprintf(pcmw->cFile, "\n");
}

static bool find_first_array_element_with_transition(pLIST_ELEMENT pelem, void *data)
{ 
    /* interpret our data */
    pITERATOR_CALLBACK_HELPER pich       = (pITERATOR_CALLBACK_HELPER) data;

    /* now, locate the relevant action info element */
    pLIST_ELEMENT             peventElem = pelem;
    pLIST_ELEMENT             pstateElem = pich->pOtherElem;
    pACTION_INFO              pai        = pich->pmi->actionArray[peventElem->ordinal][pstateElem->ordinal];

    /* do we have a transition? */
    return pai && pai->transition;
}

static bool print_event_returning_state_fn_case(pLIST_ELEMENT pelem, void *data)
{
    /* interpret our data */
    pITERATOR_CALLBACK_HELPER pich       = (pITERATOR_CALLBACK_HELPER) data;
    pID_INFO                  pevent     = (pID_INFO)                  pelem->mbr;

    /* now, locate the relevant action info element */
    pLIST_ELEMENT             peventElem = pelem;
    pLIST_ELEMENT             pstateElem = pich->pOtherElem;
    pACTION_INFO              pai        = pich->pmi->actionArray[peventElem->ordinal][pstateElem->ordinal];

    if (!pevent->type_data.event_data.single_pai_for_all_states)
    {
        if (pai)
        {
            pich->counter++;
            fprintf(pich->pcmw->cFile, "\tcase ");
            printNameWithAncestry(pevent->name, pich->pmi, pich->pcmw->cFile, "_", alc_lower, ai_include_self);
            fprintf(pich->pcmw->cFile, ":\n");

            if (strlen(pai->action->name))
            {
                fprintf(pich->pcmw->cFile, "\t\tretVal = ");
                printNameWithAncestry(pai->action->name, pich->pmi, pich->pcmw->cFile, "_", alc_lower, ai_include_self);
                fprintf(pich->pcmw->cFile, "(pfsm);\n");
            }
            else
            {
                fprintf(pich->pcmw->cFile, "#ifdef ");
                printNameWithAncestry("DEBUG", pich->pmi, pich->pcmw->cFile, "_", alc_upper, ai_include_self);
                fprintf(pich->pcmw->cFile
                        , "\n\t\t%s(\""
                        , core_logging_only ? "NON_CORE_DEBUG_PRINTF" : "DBG_PRINTF"
                        );
                printNameWithAncestry("noAction", pich->pmi, pich->pcmw->cFile, "_", alc_upper, ai_include_self);
                fprintf(pich->pcmw->cFile, "\");\n#endif\n");
            }

            if (pai->transition)
            {
               fprintf(pich->pcmw->cFile
                       , "\t\t%s = "
                       , (pich->pmi->machineTransition || pich->pmi->states_with_entry_fns_count || pich->pmi->states_with_exit_fns_count)
                         ? "new_s" 
                         : "pfsm->state"
                      );

               printNameWithAncestry(pai->transition->name, pich->pmi, pich->pcmw->cFile, "_", alc_lower, ai_include_self);
               fprintf(pcmw->cFile
                       , "%s;\n"
                       , pmi->actionArray[j][i]->transition->type == STATE ? "" : "(pfsm,e)"
                       );
            }

            fprintf(pcmw->cFile, "\t\tbreak;\n");
        }
    }

    return false;
}

static bool print_void_returning_state_fn_case(pLIST_ELEMENT pelem, void *data)
{
    /* interpret our data */
    pITERATOR_CALLBACK_HELPER pich       = (pITERATOR_CALLBACK_HELPER) data;
    pID_INFO                  pevent     = (pID_INFO)                  pelem->mbr;

    /* now, locate the relevant action info element */
    pLIST_ELEMENT             peventElem = pelem;
    pLIST_ELEMENT             pstateElem = pich->pOtherElem;
    pACTION_INFO              pai        = pich->pmi->actionArray[peventElem->ordinal][pstateElem->ordinal];

    if (!pevent->type_data.event_data.single_pai_for_all_states)
    {
        if (pai)
        {
            pich->counter++;
            fprintf(pich->pcmw->cFile, "\tcase ");
            printNameWithAncestry(pevent->name, pich->pmi, pich->pcmw->cFile, "_", alc_lower, ai_include_self);
            fprintf(pich->pcmw->cFile, ":\n");

            if (strlen(pai->action->name))
            {
                printNameWithAncestry(pai->action->name, pich->pmi, pich->pcmw->cFile, "_", alc_lower, ai_include_self);
                fprintf(pich->pcmw->cFile, "(pfsm);\n");
            }
            else
            {
                fprintf(pich->pcmw->cFile, "#ifdef ");
                printNameWithAncestry("DEBUG", pich->pmi, pich->pcmw->cFile, "_", alc_upper, ai_include_self);
                fprintf(pich->pcmw->cFile
                        , "\n\t\t%s(\""
                        , core_logging_only ? "NON_CORE_DEBUG_PRINTF" : "DBG_PRINTF"
                        );
                printNameWithAncestry("noAction", pich->pmi, pich->pcmw->cFile, "_", alc_upper, ai_include_self);
                fprintf(pich->pcmw->cFile, "\");\n#endif\n");
            }

            if (pai->transition)
            {
               fprintf(pich->pcmw->cFile
                       , "\t\t%s = "
                       , (pich->pmi->machineTransition || pich->pmi->states_with_entry_fns_count || pich->pmi->states_with_exit_fns_count)
                         ? "new_s" 
                         : "pfsm->state"
                      );

               printNameWithAncestry(pai->transition->name, pich->pmi, pich->pcmw->cFile, "_", alc_lower, ai_include_self);
               fprintf(pcmw->cFile
                       , "%s;\n"
                       , pmi->actionArray[j][i]->transition->type == STATE ? "" : "(pfsm,e)"
                       );
            }

            fprintf(pcmw->cFile, "\t\tbreak;\n");
        }
    }

    return false;
}

static bool print_state_returning_state_fn_case(pLIST_ELEMENT pelem, void *data)
{
    /* interpret our data */
    pITERATOR_CALLBACK_HELPER pich       = (pITERATOR_CALLBACK_HELPER) data;
    pID_INFO                  pevent     = (pID_INFO)                  pelem->mbr;

    /* now, locate the relevant action info element */
    pLIST_ELEMENT             peventElem = pelem;
    pLIST_ELEMENT             pstateElem = pich->pOtherElem;
    pACTION_INFO              pai        = pich->pmi->actionArray[peventElem->ordinal][pstateElem->ordinal];

    if (!pevent->type_data.event_data.single_pai_for_all_states)
    {
        if (pai)
        {
            pich->counter++;
            fprintf(pich->pcmw->cFile, "\tcase ");
            printNameWithAncestry(pevent->name, pich->pmi, pich->pcmw->cFile, "_", alc_lower, ai_include_self);
            fprintf(pich->pcmw->cFile, ":\n");

            fprintf(pich->pcmw->cFile, "\t\tretVal = ");
            if (strlen(pai->action->name))
            {
                printNameWithAncestry(pai->action->name, pich->pmi, pich->pcmw->cFile, "_", alc_lower, ai_include_self);
                fprintf(pich->pcmw->cFile, "(pfsm);\n");
            }
            else
            {
               printNameWithAncestry(pai->transition->name, pich->pmi, pich->pcmw->cFile, "_", alc_lower, ai_include_self);
               fprintf(pcmw->cFile
                       , "%s;\n"
                       , pmi->actionArray[j][i]->transition->type == STATE ? "" : "(pfsm,e)"
                       );
            }

            fprintf(pcmw->cFile, "\t\tbreak;\n");
        }
    }

    return false;
}

static bool areTransitionsPossible(pMACHINE_INFO pmi, unsigned state)
{
   bool transition_found = false;

   if (pmi->modFlags & mfActionsReturnStates)
   {
      transition_found = true;
   }
   else
   {
      for (unsigned event = 0; event < pmi->event_list->count && !transition_found; event++)
      {
         transition_found = pmi->actionArray[event][state] && (pmi->actionArray[event][state]->transition != NULL);
      }
   }

   return transition_found;
}

static void defineCSwitchSubMachineStateFns(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{
   unsigned  events_handled;
   bool      transitions_are_possible;
   char     *parent_cp = hungarianToUnderbarCaps(pmi->parent->name->name);

   for (unsigned i = 0; i < pmi->state_list->count; i++)
   {
      transitions_are_possible = areTransitionsPossible(pmi,i);

      if (pmi->modFlags & mfActionsReturnVoid)
      {
         fprintf(pcmw->cFile
                 , "void %s_%s_stateFn(p%s pfsm, %s_EVENT%s e)\n{"
                 , pmi->name->name
                 , stateNameByIndex(pmi, i)
                 , cp
                 , parent_cp
                 , pmi->parent->data_block_count ? "_ENUM"  : ""
                );
      }
      else
      {
         fprintf(pcmw->cFile
                 , "%s_%s%s %s_%s_stateFn(p%s pfsm, %s_EVENT%s e)\n{\n"
                 , parent_cp
                 , (pmi->modFlags & mfActionsReturnStates) ? "STATE" : "EVENT"
                 , (pmi->parent->data_block_count && !(pmi->modFlags & mfActionsReturnStates))? "_ENUM"  : ""
                 , pmi->name->name
                 , stateNameByIndex(pmi, i)
                 , cp
                 , parent_cp
                 , pmi->parent->data_block_count ? "_ENUM"  : ""
                );

         fprintf(pcmw->cFile
                 , "\t%s_%s%s retVal = THIS(no%s);\n"
                 , parent_cp
                 , (pmi->modFlags & mfActionsReturnStates) ? "STATE" : "EVENT"
                 , (pmi->parent->data_block_count && !(pmi->modFlags & mfActionsReturnStates))? "_ENUM"  : ""
                 , (pmi->modFlags & mfActionsReturnStates) ? "Transition" : "Event"
                );
      }

      if (
          (pmi->machineTransition || pmi->states_with_entry_fns_count || pmi->states_with_exit_fns_count)
          && transitions_are_possible
          )
      {
         fprintf(pcmw->cFile
                 , "\t%s_STATE new_s = pfsm->state;\n"
                 , cp
                );

      }

      fprintf(pcmw->cFile
              , "\n\tswitch(e)\n\t{\n"
             );

      events_handled   = 0;
      for (unsigned j = 0; j < pmi->event_list->count; j++)
      {
         if (pmi->actionArray[j][i])
         {
            events_handled++;

            fprintf(pcmw->cFile
                    , "\tcase THIS(%s):\n"
                    , eventNameByIndex(pmi, j)
                   );

            if (pmi->modFlags & mfActionsReturnStates)
            {

               fprintf(pcmw->cFile, "\t\tretVal = "
                      );

               if (strlen(pmi->actionArray[j][i]->action->name))
               {
                  fprintf(pcmw->cFile
                          , "%s_%s(pfsm);\n",
                          pmi->name->name, pmi->actionArray[j][i]->action->name);
               }
               else
               {
                  fprintf(pcmw->cFile
                          , (pmi->actionArray[j][i]->transition->type == STATE)
                          ? "%s_%s;\n"
                          : "%s_%s(pfsm);\n"
                          , pmi->name->name
                          , pmi->actionArray[j][i]->transition->name
                         );
               }

            }
            else
            {
               if (strlen(pmi->actionArray[j][i]->action->name))
               {
                  fprintf(pcmw->cFile
                          , "\t\t%s%s_%s(pfsm);\n"
                          , (pmi->modFlags & mfActionsReturnVoid) ? "" : "retVal = "
                          , pmi->name->name
                          , pmi->actionArray[j][i]->action->name
                         );
               }
               else
               {
                  fprintf(pcmw->cFile
                          , "#ifdef %s_DEBUG\n\t\tDBG_PRINTF(\"%s_noAction\");\n#endif\n"
                          , cp
                          , pmi->name->name
                         );
               }

               if (pmi->actionArray[j][i]->transition)
               {
                  fprintf(pcmw->cFile
                          , "\t\t%s = "
                          , (pmi->machineTransition || pmi->states_with_entry_fns_count || pmi->states_with_exit_fns_count)
                            ? "new_s" 
                            : "pfsm->state"
                         );

                  fprintf(pcmw->cFile, "%s_%s%s;\n"
                          , pmi->name->name
                          , pmi->actionArray[j][i]->transition->name
                          , pmi->actionArray[j][i]->transition->type == STATE
                             ? ""
                             : "(pfsm,e)"
                          );
               }
            }

            fprintf(pcmw->cFile
                    , "\t\tbreak;\n"
                   );
         }

      }

      if (events_handled < pmi->event_list->count + 1)
      {
         fprintf(pcmw->cFile
                 , "\tdefault:\n\t\t%s(\"%s_noAction\");\n\t\tbreak;\n"
                 , core_logging_only ? "NON_CORE_DEBUG_PRINTF" : "DBG_PRINTF"
                 , pmi->name->name
                );
      }

      fprintf(pcmw->cFile
              , "\t}\n"
             );

      if (
          (pmi->machineTransition || pmi->states_with_entry_fns_count || pmi->states_with_exit_fns_count)
          && transitions_are_possible
          )
      {
         fprintf(pcmw->cFile
                 , "\n\n\tif (%s_%s != new_s)\n\t{\n"
                 , pmi->name->name
                 , stateNameByIndex(pmi,i)
                );

         if (pmi->machineTransition)
         {
            fprintf(pcmw->cFile
                    , "\t\t%s_%s(pfsm,new_s);\n"
                    , pmi->name->name
                    , pmi->machineTransition->name
                   );
         }

         if (pmi->states_with_exit_fns_count)
         {
            fprintf(pcmw->cFile
                    ,"\t\trunAppropriateExitFunction(%s%s_%s);\n"
                    , pmi->data ? "&pfsm->data, " : ""
                    , pmi->name->name
                    , stateNameByIndex(pmi, i)
                    );
         }

         if (pmi->states_with_entry_fns_count)
         {
            fprintf(pcmw->cFile
                    ,"\t\trunAppropriateEntryFunction(%snew_s);\n"
                    , pmi->data ? "&pfsm->data, " : ""
                    );
         }

         if (!(pmi->modFlags & ACTIONS_RETURN_FLAGS))
         {
            fprintf(pcmw->cFile
                    , "\t\tpfsm->state = new_s;\n\n"
                   );
         }

         fprintf(pcmw->cFile
                 , "\t}\n\n"
                );
      }

      if (!(pmi->modFlags & mfActionsReturnVoid))
      {
         fprintf(pcmw->cFile
                 , "\n\treturn %s;\n"
                 , pmi->modFlags & mfActionsReturnStates
                   ? "new_s" 
                   : "retVal"
                );
      }

      if (!events_handled)
      {
         fprintf(pcmw->cFile
                 , "/* we only just now know we did not use this */\n\t(void) pfsm;\n"
                 );
      }

      fprintf(pcmw->cFile, "}\n\n");

   }

   fprintf(pcmw->cFile, "\n");

   FREE_AND_CLEAR(parent_cp);

}

static void writeOriginalSwitchFSMLoopInnards(pCMachineData pcmw, pMACHINE_INFO pmi, char *tabstr)
{

   char *local_tabstr = "";

   if (pmi->has_single_pai_events)
   {
      if (pmi->modFlags & ACTIONS_RETURN_FLAGS)
      {
         fprintf(pcmw->cFile
                 , "%s\tif (eventIsNotHandledInAllStates(pfsm,%s))\n"
                 , tabstr
                 , pmi->modFlags & mfActionsReturnVoid ? "event" : "e"
                 );
      }
      else
      {
         fprintf(pcmw->cFile
                 , "%s\tif ((e = checkWhetherEventIsHandledInAllStates(pfsm,e)) != THIS(noEvent))\n"
                 , tabstr
                 );
      }
      local_tabstr = "\t";
   }

   if (pmi->modFlags & mfActionsReturnVoid)
   {
      fprintf(pcmw->cFile
              , "%s%s\t((* (*pfsm->statesArray)[pfsm->state])(pfsm,event));\n"
              , tabstr
              , local_tabstr
              );
   }
   else
   {
      fprintf(pcmw->cFile
              , "%s%s\te = ((* (*pfsm->statesArray)[pfsm->state])(pfsm,e));\n"
              , tabstr
              , local_tabstr
              );
   }

   if (!(pmi->modFlags & mfActionsReturnVoid))
   {
      fprintf(pcmw->cFile
              , "%s}\n"
              , tabstr
              );
   }
}

static void writeOriginalSwitchFSMLoop(pCMachineData pcmw, pMACHINE_INFO pmi)
{
   char *tabstr = "\t";

   if (pmi->data_block_count)
   {
      fprintf(pcmw->cFile
              , "\ttranslateEventData(&pfsm->data, event);\n\n"
              );
   }

   if (!(pmi->modFlags & mfActionsReturnVoid))
   {
      fprintf(pcmw->cFile, "\twhile (e != %s_noEvent) {\n\n",
              pmi->name->name);
   }
   
   printFSMMachineDebugBlock(pcmw, pmi);

   fprintf(pcmw->cFile
           , "\t/* This is read-only data to facilitate error reporting in action functions */\n"
           );

   fprintf(pcmw->cFile
           , "\tpfsm->event = %s;\n\n"
           , (pmi->modFlags & mfActionsReturnVoid) ? "event" : "e"
           );

   if (pmi->machine_list)
   {
      fprintf(pcmw->cFile
              , "\t\tif (e < %s_noEvent)\n\t\t{\n"
              , pmi->name->name
              );
      tabstr = "\t\t";
   }

   writeOriginalSwitchFSMLoopInnards(pcmw,pmi,tabstr);

   if (pmi->machine_list)
   {
      fprintf(pcmw->cFile
              , "\t\telse\n\t\t{\n"
              );

      if (pmi->submachine_inhibitor_count)
      {
         fprintf(pcmw->cFile
                 , "\t\t\tif (doNotInhibitSubMachines(pfsm->state))\n\t"
                 );
      }
      fprintf(pcmw->cFile
              , "\t\t\te = findAndRunSubMachine(pfsm, e);"
              );

      fprintf(pcmw->cFile
              , "\n\t\t}\n\n\t}"
              );
   }

}

static void writeOriginalSwitchSubFSMLoop(pCMachineData pcmw, pMACHINE_INFO pmi)
{
   char *local_tabstr = "\t";

   if (!(pmi->modFlags & mfActionsReturnVoid))
   {
      fprintf(pcmw->cFile
              , "\twhile (\n\t\t(e != THIS(noEvent))\n\t\t&& (e >= THIS(%s))\n\t)\n\t{\n\n"
              , eventNameByIndex(pmi, 0)
              );
   }

   printFSMSubMachineDebugBlock(pcmw, pmi);

   fprintf(pcmw->cFile
           , "\t/* This is read-only data to facilitate error reporting in action functions */\n"
           );

   fprintf(pcmw->cFile
           , "\tpfsm->event = %s;\n\n"
           , (pmi->modFlags & mfActionsReturnVoid) ? "event" : "e"
           );

   if (pmi->has_single_pai_events)
   {
      if (pmi->modFlags & ACTIONS_RETURN_FLAGS)
      {
         fprintf(pcmw->cFile
                 , "%s\tif (eventIsNotHandledInAllStates(pfsm,%s))\n"
                 , local_tabstr
                 , pmi->modFlags & mfActionsReturnVoid ? "event" : "e"
                 );
      }
      else
      {
         fprintf(pcmw->cFile
                 , "%s\tif ((e = checkWhetherEventIsHandledInAllStates(pfsm,e)) != THIS(noEvent))\n"
                 , local_tabstr
                 );
      }
      local_tabstr = "\t\t";
   }

   if (pmi->modFlags & mfActionsReturnVoid)
   {
      fprintf(pcmw->cFile
              , "%s((* (*pfsm->statesArray)[pfsm->state])(pfsm,event));\n\n"
              , local_tabstr
              );
   }
   else
   {
      fprintf(pcmw->cFile
              , "%s\te = ((* (*pfsm->statesArray)[pfsm->state])(pfsm,e));\n\n"
              , local_tabstr
              );
   }

   if (!(pmi->modFlags & mfActionsReturnVoid))
   {

      fprintf(pcmw->cFile, "\t}\n");

      fprintf(pcmw->cFile
              , "\n\treturn e == THIS(noEvent) ? PARENT(noEvent) : e;"
              );
   }

}

static void writeOriginalSwitchFSM(pCMachineData pcmw, pMACHINE_INFO pmi)
{
   writeOriginalSwitchFSMLoop(pcmw, pmi);
}

static void writeOriginalSwitchSubFSM(pCMachineData pcmw, pMACHINE_INFO pmi)
{
   writeOriginalSwitchSubFSMLoop(pcmw, pmi);
}

static void writeReentrantSwitchFSM(pCMachineData pcmw, pMACHINE_INFO pmi)
{
   fprintf(pcmw->cFile, "#ifdef FSM_START_CRITICAL\n");
   fprintf(pcmw->cFile, "\tFSM_START_CRITICAL;\n");
   fprintf(pcmw->cFile, "#endif\n\n");

   writeOriginalSwitchFSMLoop(pcmw, pmi);

   fprintf(pcmw->cFile, "\n\n#ifdef FSM_END_CRITICAL\n");
   fprintf(pcmw->cFile, "\tFSM_END_CRITICAL;\n");
   fprintf(pcmw->cFile, "#endif\n\n");

}

static bool print_state_fn_name(pLIST_ELEMENT pelem, void *data)
{
    pID_INFO                  pstate = (pID_INFO) pelem->mbr;
    pITERATOR_CALLBACK_HELPER pich   = (pITERATOR_CALLBACK_HELPER) data;

    fprintf(pich->pcmw->cFile
            , "\t%s%s_%s_stateFn\n"
            , pich->first ? (pich->first = false, "  ") : ", "
            , pich->pmi->name->name
            , pstate->name
           );

    return false;
}

static void defineStateFnArray(pCMachineData pcmw, pMACHINE_INFO pmi)
{
    ITERATOR_CALLBACK_HELPER ich = {
        .pmi     = pmi
        , .pcmw  = pcmw
        , .first = true
    };

   fprintf(pcmw->cFile, "const ");
   printAncestry(pmi, pcmw->cFile, "_", alc_upper, ai_include_self);
   fprintf(pcmw->cFile, "_STATE_FN ");
   printAncestry(pmi, pcmw->cFile, "_", alc_lower, ai_include_self);
   fprintf(pcmw->cFile, "_state_fn_array[");
   printAncestry(pmi, pcmw->cFile, "_", alc_lower, ai_include_self);
   fprintf(pcmw->cFile, "_numStates] = \n{\n");

   /* declare state fns */
   iterate_list(pmi->state_list, print_state_fn_name, &ich);

   fprintf(pcmw->cFile, "};\n\n");
}

void cswitchHeaderEnd(pCMachineData pcmw, pMACHINE_INFO pmi, bool needNoOp)
{
   ITERATOR_CALLBACK_HELPER ich = { 0 };

   ich.pcmw      = pcmw;
   ich.pmi       = pmi;
   ich.needNoOp  = needNoOp;

   /* declare the action functions themselves */
   iterate_list(pmi->action_list, declare_action_function, &ich);

   /* declare the dummy, or no op action */
   if (needNoOp)
   {
      print_action_function_declaration(pmi, pcmw->hFile, "noAction");
   }

   fprintf(pcmw->hFile, "\n");

   /* declare any machine transition function */
   if (pmi->machineTransition)
   {
      fprintf(pcmw->hFile, "void ");
      printNameWithAncestry(pmi->machineTransition->name, pmi, pcmw->hFile, "_", alc_lower, ai_include_self);
      fprintf(pcmw->hFile, "(p");
      streamHungarianToUnderbarCaps(pcmw->hFile, pmi->name->name);
      fprintf(pcmw->hFile, ",");
      streamHungarianToUnderbarCaps(pcmw->hFile, pmi->name->name);
      fprintf(pcmw->hFile, "_STATE);\n\n");
   }

   /* declare any transition functions */
   if (pmi->modFlags & mfActionsReturnStates)
   {
      if (pmi->transition_fn_list->count)
      {
         streamHungarianToUnderbarCaps(pcmw->hFile, pmi->name->name);
         fprintf(pcmw->hFile
                 , "_STATE %s_noTransitionFn(p"
                 , pmi->name->name
                );
         streamHungarianToUnderbarCaps(pcmw->hFile, pmi->name->name);
         fprintf(pcmw->hFile ");\n");

         iterate_list(pmi->transition_fn_list
                      , declare_transition_fn_for_when_actions_return_states
                      , &ich
                      );

      }

      fprintf(pcmw->hFile, "\n");

   }
   else
   {

      if (pmi->transition_fn_list->count)
      {
         iterate_list(pmi->transition_fn_list
                      , declare_transition_fn_for_when_actions_return_events
                      , &ich
                      );

      }

      fprintf(pcmw->hFile, "\n");

   }

   /* declare any entry or exit functions */
   if (pmi->states_with_entry_fns_count || pmi->states_with_exit_fns_count)
   {
      iterate_list(pmi->state_list, declare_state_entry_and_exit_functions, &ich);
      fprintf(pcmw->hFile, "\n");
   }

   /* declare needed data translators */
   if (pmi->data_block_count)
   {
      iterate_list(pmi->event_list, declare_data_translator_functions, &ich);
      fprintf(pcmw->hFile, "\n");
   }

}

void cswitchSubMachineHeaderEnd(pCMachineData pcmw, pMACHINE_INFO pmi, bool needNoOp)
{
   ITERATOR_CALLBACK_HELPER ich = { 0 };

   ich.pcmw      = pcmw;
   ich.pmi       = pmi;
   ich.needNoOp  = needNoOp;

   /* declare the action functions themselves */
   iterate_list(pmi->action_list, declare_action_function, &ich);

   /* declare the dummy, or no op action */
   if (needNoOp)
   {
      print_action_function_declaration(pmi, pcmw->hFile, "noAction");
   }

   fprintf(pcmw->hFile
           , "\n"
          );

   /* declare any machine transition function */
   if (pmi->machineTransition)
   {
      fprintf(pcmw->hFile, "void ");
      printAncestry(pmi, pcmw->hFile, "_", alc_lower, ai_include_self);
      fprintf(pcmw->hFile
              , "_%s(p"
              , pmi->machineTransition->name
              , cp
              , cp
             );
      printAncestry(pmi, pcmw->hFile, "_", alc_upper, ai_include_self);
      fprintf(pcmw->hFile, ",");
      printAncestry(pmi, pcmw->hFile, "_", alc_upper, ai_include_self);
      fprintf(pcmw->hFile, "_STATE);\n\n");
   }

   /* declare any transition functions */
   if (pmi->modFlags & mfActionsReturnStates)
   {
      if (pmi->transition_fn_list->count)
      {
         print_transition_fn_declaration_for_when_actions_return_states(pmi, pcmw->hFile, "noTransition");
         iterate_list(pmi->transition_fn_list
                      , declare_transition_fn_for_when_actions_return_states
                      , &ich
                      );
      }
   }
   else
   {
      if (pmi->transition_fn_list->count)
      {
         iterate_list(pmi->transition_fn_list
                      , cswitch_sub_machine_declare_transition_fn_for_when_actions_return_events
                      , &ich
                      );
      }
   }

   iterate_list(pmi->event_list, sub_machine_declare_data_translator_functions, &ich);

   fprintf(pcmw->cFile, "\n");

   /* declare any entry or exit functions */
   if (pmi->states_with_entry_fns_count || pmi->states_with_exit_fns_count)
   {
      iterate_list(pmi->state_list, declare_state_entry_and_exit_functions, &ich);
      fprintf(pcmw->hFile, "\n");
   }

}

bool cswitch_sub_machine_declare_transition_fn_for_when_actions_return_events(pLIST_ELEMENT pelem, void *data)
{
   pITERATOR_CALLBACK_HELPER pich = ((pITERATOR_CALLBACK_HELPER)data);
   pID_INFO pid_info              = ((pID_INFO)pelem->mbr);

   fprintf(pich->pcmw->hFile
           , "%s_STATE %s_%s(p%s,%s_EVENT%s);\n"
           , pich->cp
           , pich->pmi->name->name
           , pid_info->name
           , pich->cp
           , pich->parent_cp
           , pich->pmi->data_block_count ? "_ENUM"  : ""
          );

   return false;
}

static bool print_switch_cases_for_events_handled_in_all_states(pLIST_ELEMENT pelem, void *data)
{
   pITERATOR_CALLBACK_HELPER pich  = (pITERATOR_CALLBACK_HELPER) data;
   pID_INFO                  event = (pID_INFO) pelem->mbr;

   if (event->type_data.event_data.single_pai_for_all_states)
   {
      pich->counter++;

      fprintf(pich->pcmw->cFile, "\t\tcase ");
      printNameWithAncestry(event->name, pmi, pcmw->cFile, "_", alc_lower, ai_include_self);
      fprintf(pich->pcmw->cFile, ":\n");

      if (pich->pmi->modFlags & ACTIONS_RETURN_FLAGS)
      {
         if (event->type_data.event_data.psingle_pai->transition)
         {
            fprintf(pich->pcmw->cFile
                    , "\t\t\t%s = %s_%s%s;\n"
                    , (pich->pmi->machineTransition || pich->pmi->states_with_entry_fns_count || pich->pmi->states_with_exit_fns_count)
                      ? "new_s" 
                      : "pfsm->state"
                    , pich->pmi->name->name
                    , event->type_data.event_data.psingle_pai->transition->name
                    , event->type_data.event_data.psingle_pai->transition->type == STATE
                       ? ""
                       : pich->pmi->modFlags & mfActionsReturnStates ? "(pfsm)" : "(pfsm,e)"
                    );
         }
         else
         {

            if (strlen(event->type_data.event_data.psingle_pai->action->name))
            {
               fprintf(pich->pcmw->cFile
                       , "\t\t\t%s%s_%s(pfsm);\n"
                       , pich->pmi->modFlags & mfActionsReturnStates 
                          ? (pich->pmi->machineTransition || pich->pmi->states_with_entry_fns_count || pich->pmi->states_with_exit_fns_count)
                            ? "new_s = " : "pfsm->state = " 
                          : ""
                       , pich->pmi->name->name
                       , event->type_data.event_data.psingle_pai->action->name
                       );
            }
            else
            {
               fprintf(pich->pcmw->cFile
                       , (event->type_data.event_data.psingle_pai->transition->type == STATE)
                         ? "\t\t\t%s%s_%s;\n"
                         : "\t\t\t%s%s_%s(pfsm);\n"
                       , pich->pmi->modFlags & mfActionsReturnStates 
                          ? (pich->pmi->machineTransition || pich->pmi->states_with_entry_fns_count || pich->pmi->states_with_exit_fns_count)
                            ? "new_s = " : "pfsm->state = "
                          : ""
                       , pich->pmi->name->name
                       , event->type_data.event_data.psingle_pai->transition->name
                      );
            }

         }
         
         fprintf(pich->pcmw->cFile
                , "\t\t\thandled = true;\n"
                );

      }
      else
      {
         if (event->type_data.event_data.psingle_pai->transition)
         {
            fprintf(pich->pcmw->cFile
                    , "\t\t\t%s = %s_%s%s;\n"
                    , (pich->pmi->machineTransition || pich->pmi->states_with_entry_fns_count || pich->pmi->states_with_exit_fns_count)
                      ? "new_s" : "pfsm->state"
                    , pich->pmi->name->name
                    , event->type_data.event_data.psingle_pai->transition->name
                    , event->type_data.event_data.psingle_pai->transition->type == STATE
                       ? ""
                       : "(pfsm,e)"
                    );
         }

         if (strlen(event->type_data.event_data.psingle_pai->action->name))
         {
            fprintf(pich->pcmw->cFile
                    , "\t\t\tretVal = %s_%s(pfsm);\n"
                    , pich->pmi->name->name
                    , event->type_data.event_data.psingle_pai->action->name
                    );
         }
         else
         {
            fprintf(pich->pcmw->cFile,"\t\t\tretVal = ");
            printNameWithAncestry("noEvent", pich->pmi, pich->pcmw->cFile, alc_lower, ai_include_self);
            fprintf(pich->pcmw->cFile, ";\n");
         }

      }

      fprintf(pich->pcmw->cFile
              , "\t\t\tbreak;\n"
              );

   }

   return false;
}

static void defineAllStateHandler(pCMachineData pcmd, pMACHINE_INFO pmi)
{
   ITERATOR_CALLBACK_HELPER ich;

   ich.pcmw = pcmd;
   ich.pmi  = pmi;

   declareOrDefineSinglePAIEventHandler(pcmd, pmi, dod_define);

   if (!(pmi->modFlags & ACTIONS_RETURN_FLAGS))
   {
      fprintf(pcmd->cFile, "\t");
      streamHungarianToUnderbarCaps(pcmd->cFile, pmi->name->name);
      fprintf(pcmd->cFile
              , "_EVENT%s retVal;\n"
              , ultimateAncestor(pmi)->data_block_count ? "_ENUM"  : ""
             );
   }
   else
   {
      fprintf(pcmd->cFile
              ,"\tbool handled = false;\n"
              );
   }

   if (pmi->machineTransition || pmi->states_with_entry_fns_count || pmi->states_with_exit_fns_count)
   {
      fprintf(pcmd->cFile, "\t");
      streamHungarianToUnderbarCaps(pcmd->cFile, pmi->name->name);
      fprintf(pcmd->cFile "_STATE new_s = pfsm->state;\n\n");
   }

   fprintf(pcmd->cFile
           ,"\tswitch (e)\n\t{\n"
           );

   ich.counter = 0;
   iterate_list(pmi->event_list,print_switch_cases_for_events_handled_in_all_states,&ich);

   if (ich.counter == pmi->event_list->count)
   {
      printf("warning: (");
      printAncestry(pmi, stdout, "_", alc_lower, ai_include_self);
      printf( ") all events are handled identically in all states.\n");
   }

   if (pmi->modFlags & ACTIONS_RETURN_FLAGS)
   {
      fprintf(pcmd->cFile
              , "\t\tdefault:\n\t\t\tbreak;\n\t}\n"
              );
   }
   else
   {
      fprintf(pcmd->cFile
              , "\t\tdefault:\n\t\t\tretVal = e;\n\t\t\tbreak;\n\t}"
              );
   }

   if (pmi->machineTransition || pmi->states_with_entry_fns_count || pmi->states_with_exit_fns_count)
   {
      fprintf(pcmd->cFile
              , "\n\n\tif (pfsm->state != new_s)\n\t{\n"
             );

      if (pmi->machineTransition)
      {
         fprintf(pcmd->cFile, "\t\t");
         printNameWithAncestry(pmi->machineTransition->name, pmi, pcmd->cFile, "_", alc_lower, ai_include_self);
         fprintf(pcmd->cFile, "(pfsm,new_s);\n");
      }

      if (pmi->states_with_exit_fns_count)
      {
         fprintf(pcmd->cFile
                 ,"\t\trunAppropriateExitFunction(%spfsm->state);\n"
                 , pmi->data ? "&pfsm->data, " : ""
                 );
      }

      if (pmi->states_with_entry_fns_count)
      {
         fprintf(pcmd->cFile
                 ,"\t\trunAppropriateEntryFunction(%snew_s);\n"
                 , pmi->data ? "&pfsm->data, " : ""
                 );
      }

      fprintf(pcmd->cFile
              , "\t\tpfsm->state = new_s;\n\n"
             );

      fprintf(pcmd->cFile
              , "\t}\n\n"
             );
   }

   if (pmi->modFlags & ACTIONS_RETURN_FLAGS)
   {
      fprintf(pcmd->cFile 
              , "\n\t return !handled;\n\n}\n\n"
              );
   }
   else
   {
      fprintf(pcmd->cFile 
              , "\n\t return retVal;\n\n}\n\n"
              );
   }

}

static void writeCSwitchMachineFN(pFSMOutputGenerator pfsmog, pMACHINE_INFO pmi)
{

   pFSMCOutputGenerator pfsmcog = (pFSMCOutputGenerator) pfsmog;

   printf("%s ", pfsmcog->pcmd->cName);

   if (pmi->machine_list)
   {
      write_machines(pmi->machine_list, generateCSwitchMachineWriter);
   }

}

pFSMOutputGenerator generateCSwitchMachineWriter(FSMOGF_TYPE fsmogft)
{
	pFSMOutputGenerator pfsmog;

	if (fsmogft == fsmogft_sub_machine)
	{
		pFSMCSubMachineOutputGenerator pfsmcswitchog = calloc(1, sizeof(FSMCSubMachineOutputGenerator));

		pfsmcswitchog->fsmog.writeMachine = writeCSwitchSubMachine;
		pfsmcswitchog->fsmog.initOutput   = initCSubMachine;
		pfsmcswitchog->fsmog.closeOutput  = closeCMachine;

		pfsmcswitchog->parent_fsmcog = &CSwitchMachineWriter;

		pfsmog = (pFSMOutputGenerator)pfsmcswitchog;
	}
	else
	{
		pfsmog = (pFSMOutputGenerator)&CSwitchMachineWriter;
	}

	if (output_generated_file_names_only)
	{
		pfsmog->initOutput   = initCMachineFN;
		pfsmog->writeMachine = writeCSwitchMachineFN;
		pfsmog->closeOutput  = closeCMachineFN;
	}

	return pfsmog;

}

