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
#include "fsm_c_switch_common.h"
#include "fsm_unused.h"
#include "ancestry.h"

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

static void writeCSwitchMachine(pFSMOutputGenerator,pMACHINE_INFO);
static void writeCSwitchSubMachine(pFSMOutputGenerator,pMACHINE_INFO);

static void defineAllStateHandler(pCMachineData,pMACHINE_INFO);
static void defineCSwitchMachineFSM(pFSMCOutputGenerator);
static void defineCSwitchSubMachineFSM(pFSMCOutputGenerator);
static void defineCSwitchMachineStruct(pCMachineData,pMACHINE_INFO);
static void declareCSwitchMachineStateFnArray(pCMachineData);
static void writeActionsReturnStateSwitchFSM(pFSMCOutputGenerator);
static void writeActionsReturnStateSwitchSubFSM(pFSMCOutputGenerator);
static void writeOriginalSwitchFSMAre(pFSMCOutputGenerator);
static void writeOriginalSwitchFSMArv(pFSMCOutputGenerator);
static void writeOriginalSwitchSubFSMAre(pFSMCOutputGenerator);
static void writeOriginalSwitchSubFSMArv(pFSMCOutputGenerator);
static int  writeCSwitchMachineInternal(pFSMCOutputGenerator);
static int  writeCSwitchSubMachineInternal(pFSMCOutputGenerator);
static void writeOriginalSwitchFSMLoopInnards(pFSMCOutputGenerator, char*);
static void writeOriginalSwitchSubFSMLoopInnards(pFSMCOutputGenerator,char*);
static void chooseWorkerFunctions(pFSMCOutputGenerator);
static void defineCSwitchMachineStateFns(pCMachineData,pMACHINE_INFO);
static void defineStateFnArray(pCMachineData,pMACHINE_INFO);
static bool print_state_fn_signature(pLIST_ELEMENT,void*);
static bool print_state_fn_name(pLIST_ELEMENT,void*);
static void declareOrDefineSinglePAIEventHandler(pCMachineData,pMACHINE_INFO,DECLARE_OR_DEFINE);
static bool find_first_array_element_with_transition(pLIST_ELEMENT,void*);
static void print_state_fn_epilogue(pCMachineData,pMACHINE_INFO,pID_INFO,bool);
static bool define_event_returning_state_fn(pLIST_ELEMENT,void*);
static bool define_state_returning_state_fn(pLIST_ELEMENT,void*);
static bool define_void_returning_state_fn(pLIST_ELEMENT,void*);
static bool print_event_returning_state_fn_case(pLIST_ELEMENT,void*);
static bool print_void_returning_state_fn_case(pLIST_ELEMENT,void*);
static bool print_state_returning_state_fn_case(pLIST_ELEMENT,void*);
static bool print_switch_cases_for_events_handled_in_all_states_arev(pLIST_ELEMENT,void*);
static bool print_switch_cases_for_events_handled_in_all_states_ars(pLIST_ELEMENT,void*);


#define writeFSMLoop(A) pfsmcog->wfsm((A))
#define writeCFSMLoopInnards(A) pfsmcog->cfsmliw(pfsmcog, (A))

static FSMCSwitchOutputGenerator CSwitchMachineWriter = {
	{
	   {
	      initCMachine
	      , writeCSwitchMachine
	      , closeCMachine
		  , generateCSwitchMachineWriter
	   }
	   , NULL
	   , NULL
	   , NULL
	   , NULL
	   , NULL
	   , NULL
	}
	, NULL
	, NULL
	, NULL
};

static void writeActionsReturnStateSwitchFSM(pFSMCOutputGenerator pfsmcog)
{
   unsigned tab_level = 0;

   pCMachineData pcmd = pfsmcog->pcmd;
   pMACHINE_INFO pmi  = pcmd->pmi;

   FSMLANG_DEVELOP_PRINTF(pcmd->cFile, "/* FSMLANG_DEVELOP: %s */\n", __func__);

   fprintf(pcmd->cFile
           , "\t%s s = STATE(noTransition);\n"
           , stateType(pcmd)
           );

   if (pmi->data_block_count)
   {
	   fprintf(pcmd->cFile
			   , "\t%s e = event->event;\n"
			   , eventType(pcmd)
			   );
   }

   fprintf(pcmd->cFile
		   , "\n#ifdef %s_DEBUG\n"
		   , fsmType(pcmd)
		   );
   fprintf(pcmd->cFile, "\n\tDBG_PRINTF(\"event: %%s; start state: %%s\"\n\t\t,");
   fprintf(pcmd->cFile
           , "%s_EVENT_NAMES[%s]\n\t\t,"
           , ucfqMachineName(pcmd)
		   , (pmi->data_block_count == 0) ? "event" : "e"
           );
   fprintf(pcmd->cFile
           , "%s_STATE_NAMES[pfsm->state]\n\t\t);\n\n"
           , ucnfMachineName(pcmd)
           );
   fprintf(pcmd->cFile
		   , "\n#endif\n"
		   );

   if (pmi->data_block_count)
   {
      fprintf(pcmd->cFile
              , "\ttranslateEventData(&pfsm->data, event);\n\n"
              );
   }

   if (pmi->machine_list)
   {
      fprintf(pcmd->cFile
              , "\tif (%s < THIS(%s))\n\t{\n"
			  , (pmi->data_block_count == 0) ? "event" : "e"
			  , pmi->modFlags & ACTIONS_RETURN_FLAGS ? "numEvents" : "noEvent"
              );
	  tab_level++;
   }

   if (pmi->has_single_pai_events)
   {
	  print_tab_levels(pcmd->cFile,tab_level);
      fprintf(pcmd->cFile
              , "\tif (eventIsNotHandledInAllStates(pfsm,%s))\n\t\t{\n"
			  , (pmi->data_block_count == 0) ? "event" : "e"
              );
	  tab_level++;
   }

   print_tab_levels(pcmd->cFile,tab_level);
   fprintf(pcmd->cFile
           , "\ts = ((* (*pfsm->statesArray)[pfsm->state])(pfsm,%s));\n\n"
		   , (pmi->data_block_count == 0) ? "event" : "e"
           );

   print_tab_levels(pcmd->cFile,tab_level);
   fprintf(pcmd->cFile
           , "\tif (s != STATE(noTransition))\n"
           );
   print_tab_levels(pcmd->cFile,tab_level);
   fprintf(pcmd->cFile
		   , "\t{\n"
           );

   if (pmi->executes_fns_on_state_transitions)
   {
	  print_tab_levels(pcmd->cFile,tab_level);
      fprintf(pcmd->cFile
              , "\t\tif (s != pfsm->state)\n"
             );
	  print_tab_levels(pcmd->cFile,tab_level);
      fprintf(pcmd->cFile
			  , "\t\t{\n"
			  );

      if (pmi->machineTransition)
      {
		 print_tab_levels(pcmd->cFile,tab_level);
         fprintf(pcmd->cFile
                 , "\t\t\tUFMN(%s)(pfsm,s);\n"
                 , pmi->machineTransition->name
                 );
      }

      if (pmi->states_with_exit_fns_count)
      {
		 print_tab_levels(pcmd->cFile,tab_level);
         fprintf(pcmd->cFile
                 ,"\t\t\trunAppropriateExitFunction(%spfsm->state);\n"
                 , pmi->data ? "&pfsm->data, " : ""
                 );
      }

      if (pmi->states_with_entry_fns_count)
      {
		 print_tab_levels(pcmd->cFile,tab_level);
         fprintf(pcmd->cFile
                 ,"\t\t\trunAppropriateEntryFunction(%ss);\n"
                 , pmi->data ? "&pfsm->data, " : ""
                 );
      }

	  print_tab_levels(pcmd->cFile,tab_level);
      fprintf(pcmd->cFile
              , "\t\t}\n"
			  );

   }

   print_tab_levels(pcmd->cFile,tab_level);
   fprintf(pcmd->cFile
           , "\t\tpfsm->state = s;\n"
          );
   print_tab_levels(pcmd->cFile,tab_level);
   fprintf(pcmd->cFile
		   , "\t}\n\n"
		   );

   if (pmi->has_single_pai_events)
   {
	   tab_level--;
	   print_tab_levels(pcmd->cFile,tab_level);
       fprintf(pcmd->cFile
               , "\t}\n\n"
               );
   }

   if (pmi->machine_list)
   {
	  tab_level--;
	  print_tab_levels(pcmd->cFile,tab_level);
      fprintf(pcmd->cFile
              , "\t}\n"
              );
	  print_tab_levels(pcmd->cFile,tab_level);
      fprintf(pcmd->cFile
			  , "\telse\n"
              );
	  print_tab_levels(pcmd->cFile,tab_level);
      fprintf(pcmd->cFile
			  , "\t{\n"
              );

      if (pmi->submachine_inhibitor_count)
      {
		 print_tab_levels(pcmd->cFile,tab_level);
         fprintf(pcmd->cFile
                 , "\t\tif (doNotInhibitSubMachines(pfsm->state))\n"
                 );
		 tab_level++;
      }

	  print_tab_levels(pcmd->cFile,tab_level);
      fprintf(pcmd->cFile
              , "\t\tfindAndRunSubMachine(pfsm, e);\n"
              );
	  if (pmi->submachine_inhibitor_count)
	  {
		  tab_level--;
	  }
	  print_tab_levels(pcmd->cFile,tab_level);
	  fprintf(pcmd->cFile
              , "\t}"
              );
   }

   fprintf(pcmd->cFile
		   , "\n#ifdef %s_DEBUG\n"
		   , fsmType(pcmd)
		   );
   fprintf(pcmd->cFile, "\n\tDBG_PRINTF(\"end state: %%s\"\n\t\t,");
   fprintf(pcmd->cFile
           , "%s_STATE_NAMES[pfsm->state]\n\t\t);\n"
           , ucnfMachineName(pcmd)
           );
   fprintf(pcmd->cFile
		   , "\n#endif\n"
		   );

}

static void writeActionsReturnStateSwitchSubFSM(pFSMCOutputGenerator pfsmcog)
{
   unsigned tab_level = 0;

   pCMachineData pcmd = pfsmcog->pcmd;
   pMACHINE_INFO pmi  = pcmd->pmi;

   FSMLANG_DEVELOP_PRINTF(pcmd->cFile, "/* FSMLANG_DEVELOP: %s */\n", __func__);

   fprintf(pcmd->cFile
           , "\t%s s = STATE(noTransition);\n"
           , stateType(pcmd)
           );

   fprintf(pcmd->cFile
		   , "\n#ifdef %s_DEBUG\n"
		   , fsmType(pcmd)
		   );
   fprintf(pcmd->cFile, "\n\tDBG_PRINTF(\"event: %%s; start state: %%s\"\n\t\t,");
   fprintf(pcmd->cFile
           , "%s_EVENT_NAMES[%s - THIS(firstEvent)]\n\t\t,"
           , ucfqMachineName(pcmd)
		   , (pmi->data_block_count == 0) ? "event" : "e"
           );
   fprintf(pcmd->cFile
           , "%s_STATE_NAMES[pfsm->state]\n\t\t);\n\n"
           , ucnfMachineName(pcmd)
           );
   fprintf(pcmd->cFile
		   , "\n#endif\n"
		   );

   if (pmi->machine_list)
   {
      fprintf(pcmd->cFile
              , "\tif (event < THIS(noEvent))\n\t\t{\n"
              );
	  tab_level++;
   }

   if (pmi->has_single_pai_events)
   {
	  print_tab_levels(pcmd->cFile,tab_level);
      fprintf(pcmd->cFile
              , "\tif (eventIsNotHandledInAllStates(pfsm,event))\n"
              );
	  print_tab_levels(pcmd->cFile,tab_level);
      fprintf(pcmd->cFile
			  , "\t{\n"
              );
	  tab_level++;
   }

   print_tab_levels(pcmd->cFile,tab_level);
   fprintf(pcmd->cFile
           , "\ts = ((* (*pfsm->statesArray)[pfsm->state])(pfsm,event));\n\n"
           );

   print_tab_levels(pcmd->cFile,tab_level);
   fprintf(pcmd->cFile
           , "\tif (s != STATE(noTransition))\n"
           );
   print_tab_levels(pcmd->cFile,tab_level);
   fprintf(pcmd->cFile
		   , "\t{\n"
           );

   if (pmi->executes_fns_on_state_transitions)
   {
      print_tab_levels(pcmd->cFile,tab_level);
      fprintf(pcmd->cFile
              , "\tif (s != pfsm->state)\n"
             );
      print_tab_levels(pcmd->cFile,tab_level);
      fprintf(pcmd->cFile
			  , "\t{\n"
			  );

      if (pmi->machineTransition)
      {
		 print_tab_levels(pcmd->cFile,tab_level);
         fprintf(pcmd->cFile
                 , "\t\tUFMN(%s)(pfsm,s);\n"
                 , pmi->machineTransition->name
                 );
      }

      if (pmi->states_with_exit_fns_count)
      {
		 print_tab_levels(pcmd->cFile,tab_level);
         fprintf(pcmd->cFile
                 ,"\t\trunAppropriateExitFunction(%spfsm->state);\n"
                 , pmi->data ? "&pfsm->data, " : ""
                 );
      }

      if (pmi->states_with_entry_fns_count)
      {
		 print_tab_levels(pcmd->cFile,tab_level);
         fprintf(pcmd->cFile
                 ,"\t\trunAppropriateEntryFunction(%ss);\n"
                 , pmi->data ? "&pfsm->data, " : ""
                 );
      }

	  print_tab_levels(pcmd->cFile,tab_level);
      fprintf(pcmd->cFile
              , "\t}\n"
			  );

   }

   print_tab_levels(pcmd->cFile,tab_level);
   fprintf(pcmd->cFile
           , "\t\tpfsm->state = s;\n"
          );
   print_tab_levels(pcmd->cFile,tab_level);
   fprintf(pcmd->cFile
		   , "\t}\n\n"
		   );

   if (pmi->has_single_pai_events)
   {
	   print_tab_levels(pcmd->cFile,tab_level);
       fprintf(pcmd->cFile
               , "}\n\n"
               );
	   tab_level--;
   }

   if (pmi->machine_list)
   {
	  print_tab_levels(pcmd->cFile,tab_level);
      fprintf(pcmd->cFile
              , "\t}\n\t\telse\n"
              );
	  print_tab_levels(pcmd->cFile,tab_level);
      fprintf(pcmd->cFile
			  , "\t\t{\n"
              );

      if (pmi->submachine_inhibitor_count)
      {
		 print_tab_levels(pcmd->cFile,tab_level);
         fprintf(pcmd->cFile
                 , "\tif (doNotInhibitSubMachines(pfsm->state))\n\t"
                 );
      }

	  print_tab_levels(pcmd->cFile,tab_level);
      fprintf(pcmd->cFile
              , "\tfindAndRunSubMachine(pfsm, event);\n"
              );
	  print_tab_levels(pcmd->cFile,tab_level);
      fprintf(pcmd->cFile
              , "\t}"
              );
	  tab_level--;
   }

   fprintf(pcmd->cFile
		   , "\n#ifdef %s_DEBUG\n"
		   , fsmType(pcmd)
		   );
   fprintf(pcmd->cFile, "\n\tDBG_PRINTF(\"end state: %%s\"\n\t\t,");
   fprintf(pcmd->cFile
           , "%s_STATE_NAMES[pfsm->state]\n\t\t);\n"
           , ucnfMachineName(pcmd)
           );
   fprintf(pcmd->cFile
		   , "\n#endif\n"
		   );

}

static void writeCSwitchMachine(pFSMOutputGenerator pfsmog, pMACHINE_INFO pmi)
{
   pFSMCOutputGenerator pfsmcog = (pFSMCOutputGenerator) pfsmog;

   pfsmcog->pcmd->pmi = pmi;
   pfsmcog->cfsmliw   = writeOriginalSwitchFSMLoopInnards;
   chooseWorkerFunctions(pfsmcog);

   writeCSwitchMachineInternal(pfsmcog);

   if (pmi->machine_list)
   {
      write_machines(pmi->machine_list, generateCSwitchMachineWriter, pfsmog);
   }
}

static void writeCSwitchSubMachine(pFSMOutputGenerator pfsmog, pMACHINE_INFO pmi)
{

   pFSMCOutputGenerator pfsmcog = (pFSMCOutputGenerator) pfsmog;

   pfsmcog->cfsmliw   = writeOriginalSwitchSubFSMLoopInnards;

   pfsmcog->pcmd->pmi                = pmi;

   /* for sub machines, some output strings are taken from the parent */
   if (!(pfsmcog->pcmd->pmi->modFlags & mfActionsReturnStates))
   {
	   pfsmcog->pcmd->action_return_type      = pfsmcog->parent_fsmcog->pcmd->action_return_type;
   }
   pfsmcog->pcmd->fsm_fn_event_type       = pfsmcog->parent_fsmcog->pcmd->event_type;
   pfsmcog->pcmd->sub_fsm_fn_event_type   = pfsmcog->parent_fsmcog->pcmd->sub_fsm_fn_event_type;
   pfsmcog->pcmd->sub_fsm_fn_return_type  = pfsmcog->parent_fsmcog->pcmd->sub_fsm_fn_return_type;
   pfsmcog->pcmd->event_type              = pfsmcog->parent_fsmcog->pcmd->event_type;
   pfsmcog->pcmd->instance_type           = pfsmcog->parent_fsmcog->pcmd->instance_type;

   chooseWorkerFunctions(pfsmcog);

   writeCSwitchSubMachineInternal(pfsmcog);

   if (pmi->machine_list)
   {
      write_machines(pmi->machine_list, generateCSwitchMachineWriter, pfsmog);
   }
}

static int writeCSwitchMachineInternal(pFSMCOutputGenerator pfsmcog)
{
	pCMachineData pcmd = pfsmcog->pcmd;
	pMACHINE_INFO pmi  = pfsmcog->pcmd->pmi;

   ITERATOR_CALLBACK_HELPER ich = {
	   .ih = {
		   .pmi    = pmi
	   }
       , .pcmd = pfsmcog->pcmd
		, .pfsmcog = pfsmcog
   };

   /* do this now, since some header stuff puts content into the source file.*/
   addNativeImplementationPrologIfThereIsAny(pmi, pcmd->cFile);

   commonHeaderStart(pcmd, pmi, "state_fn", true);

   declareCSwitchMachineStateFnArray(pcmd);

   defineCSwitchMachineStruct(pcmd, pmi);

   /* declare state fns */
   ich.define = false;
   iterate_list(pmi->state_list
				, print_state_fn_signature
                , &ich
                );

   fprintf(pcmd->hFile, "\n");

   cswitchMachineHeaderEnd(pcmd, pmi, empty_cell_fn != NULL);

   /*
     Source File
   */

   if (pmi->machine_list)
   {
      declareSubMachineManagers(pcmd, pmi);
   }

   if (pmi->data_block_count)
   {
      declareEventDataManager(pcmd);
   }

   if (pmi->has_single_pai_events)
   {
       declareOrDefineSinglePAIEventHandler(pcmd, pmi, dod_declare);
   }

   if (pmi->states_with_entry_fns_count || pmi->states_with_exit_fns_count)
   {
	   declareStateEntryAndExitManagers(pcmd, pmi);
   }

   defineStateFnArray(pcmd, pmi);

   defineSubMachineArray(pcmd, pmi);

   if (generate_instance)
   {
      generateInstance(pcmd, pmi, "statesArray", "state_fn");

	  if (generate_run_function)
	  {
		 generateRunFunction(pcmd, pmi);
	  }
   }
   else
   {
	   generateInstanceMacro(pcmd, pmi, "statesArray", "state_fn");
   }

   defineCSwitchMachineFSM(pfsmcog);

   /* write our sub-machine lookup, if needed */
   if (pmi->machine_list)
   {
      defineSubMachineFinder(pcmd, pmi);
   }

   if (pmi->has_single_pai_events)
   {
      defineAllStateHandler(pcmd, pmi);
   }

   defineStateEntryAndExitManagers(pcmd, pmi);

   if (pmi->data_block_count)
   {
      defineEventDataManager(pcmd, pmi);
   }

   defineCSwitchMachineStateFns(pcmd, pmi);

   if (generate_weak_fns)
   {

      defineWeakActionFunctionStubs(pcmd, pmi);

      defineWeakStateEntryAndExitFunctionStubs(pcmd, pmi);

      if (pmi->data_block_count)
      {
         defineWeakDataTranslatorStubs(pcmd, pmi);
      }

   }
   else if (force_generation_of_event_passing_actions)
   {
      defineEventPassingActions(pcmd, pmi);
   }

   writeDebugInfo(pcmd, pmi);

	addNativeImplementationEpilogIfThereIsAny(pmi, pcmd->cFile);

   return 0;
}

static bool print_state_fn_signature(pLIST_ELEMENT pelem, void *data)
{
    pID_INFO                  pstate = (pID_INFO) pelem->mbr;
    pITERATOR_CALLBACK_HELPER pich   = (pITERATOR_CALLBACK_HELPER) data;

	FSMLANG_DEVELOP_PRINTF(pich->pcmd->cFile, "/* FSMLANG_DEVELOP: %s */\n", __func__);

    fprintf(pich->pcmd->cFile
			, "static %s %s_stateFn(p%s%s%s,%s%s%s)%s"
			, actionReturnType(pich->pcmd)
			, pstate->name
			, fsmType(pich->pcmd)
			, pich->define ? " "    : ""
			, pich->define ? "pfsm" : ""
			, eventType(pich->pcmd)
			, pich->define ? " "    : ""
			, pich->define ? "e" : ""
            , pich->define ? "\n{\n" : ";\n"
			);

    return false;
}

static int writeCSwitchSubMachineInternal(pFSMCOutputGenerator pfsmcog)
{
	pCMachineData pcmd = pfsmcog->pcmd;
	pMACHINE_INFO pmi  = pcmd->pmi;

   ITERATOR_CALLBACK_HELPER ich = {
	   . ih = {
		   .pmi      = pmi
	   }
       , .pcmd   = pcmd
       , .define = false
		, .pfsmcog = pfsmcog
   };

   /* do this now, since some header stuff puts content into the source file.*/
   addNativeImplementationPrologIfThereIsAny(pmi, pcmd->cFile);

   subMachineHeaderStart(pcmd, pmi, "state_fn", true);

   declareCSwitchMachineStateFnArray(pcmd);

   defineCSwitchMachineStruct(pcmd, pmi);

   /* declare state fns */
   iterate_list(pmi->state_list
				, print_state_fn_signature
                , &ich
                );

   fprintf(pcmd->hFile, "\n");

   cswitchSubMachineHeaderEnd(pcmd, pmi, empty_cell_fn != NULL);

   /*
     Source File
   */

   if (pmi->machine_list)
   {
      declareSubMachineManagers(pcmd, pmi);
   }

   if (pmi->has_single_pai_events)
   {
       declareOrDefineSinglePAIEventHandler(pcmd, pmi, dod_declare);
   }

   defineStateFnArray(pcmd, pmi);

   defineSubMachineIF(pcmd);

   possiblyDefineSubMachineSharedEventStructures(pcmd, pmi);

   defineSubMachineArray(pcmd, pmi);

   if (generate_instance)
   {
      generateInstance(pcmd, pmi, "statesArray", "state_fn");
   }
   else
   {
	   generateSubMachineInstanceMacro(pcmd, pmi, "statesArray", "state_fn");
   }

   defineCSwitchSubMachineFSM(pfsmcog);

   /* write our sub-machine lookup, if needed */
   if (pmi->machine_list)
   {
      defineSubMachineFinder(pcmd, pmi);
   }

   if (pmi->has_single_pai_events)
   {
      defineAllStateHandler(pcmd, pmi);
   }

   defineStateEntryAndExitManagers(pcmd, pmi);

   defineCSwitchMachineStateFns(pcmd, pmi);

   if (generate_weak_fns)
   {
      defineWeakActionFunctionStubs(pcmd, pmi);

	  /* TODO: sub machine data translators are different
      defineWeakDataTranslatorStubs(pcmd, pmi);
	  */

      defineWeakStateEntryAndExitFunctionStubs(pcmd, pmi);

   }
   else if (force_generation_of_event_passing_actions)
   {
      defineEventPassingActions(pcmd, pmi);
   }

   writeDebugInfo(pcmd, pmi);

	addNativeImplementationEpilogIfThereIsAny(pmi, pcmd->cFile);

   return 0;
}

static void declareCSwitchMachineStateFnArray(pCMachineData pcmd)
{
	FILE *fout = generate_instance ? pcmd->hFile : pcmd->pubHFile;

	FSMLANG_DEVELOP_PRINTF(fout, "/* FSMLANG_DEVELOP: %s */\n", __func__);


	fprintf(fout
			, "typedef %s (*%s)(p%s,%s);\n\n"
			, actionReturnType(pcmd)
			, stateFnType(pcmd)
			, fsmType(pcmd)
			, eventType(pcmd)
			);

	fprintf(fout
			, "%sconst %s %s_state_fn_array[%s_numStates];\n\n"
			, generate_instance ? "static " : "extern "
			, stateFnType(pcmd)
			, machineName(pcmd)
			, machineName(pcmd)
			);

}

static void defineCSwitchMachineStruct(pCMachineData pcmd, pMACHINE_INFO pmi)
{
	FSMLANG_DEVELOP_PRINTF(pcmd->hFile, "/* FSMLANG_DEVELOP: %s */\n", __func__);

	FILE *fout = generate_instance ? pcmd->hFile : pcmd->pubHFile;

   /* put the machine structure definition into the header file */
   fprintf(fout
		   , "struct _%s_struct_ {\n"
		   , machineName(pcmd)
		   );

   if (pmi->data)
   {
      fprintf(fout
			  , "\t%-*s data;\n"
			  , (int)pcmd->sub_machine_struct_format_width + 6 /* for the "const " */
			  , fsmDataType(pcmd)
			 );
   }

   fprintf(fout
		   , "\t%-*s state;\n"
		   , (int)pcmd->sub_machine_struct_format_width + 6 /* for the "const " */
		   , stateType(pcmd)
		   );

   fprintf(fout
		   , "\t%-*s event;\n"
		   , (int)pcmd->sub_machine_struct_format_width + 6 /* for the "const " */
		   , eventType(pcmd)
		  );

   fprintf(fout
		   , "\t%-*s const (*statesArray)[%s_numStates];\n"
		   , (int)pcmd->sub_machine_struct_format_width
		   , stateFnType(pcmd)
		   , machineName(pcmd)
		   );

   if (pmi->machine_list)
   {
      fprintf(fout
			  , "\tp%-*s const (*subMachineArray)[%s_numSubMachines];\n"
			  , (int)pcmd->sub_machine_struct_format_width
			  , subFsmIfType(pcmd)
			   , fqMachineName(pcmd)
			 );
	  if (!generate_instance)
	  {
		  fprintf(fout
				  , "\t%-*s* const\t(*subMachines)[%s_numSubMachines];\n"
				  , (int) pcmd->c_machine_struct_format_width
				  , "void"
				  , fqMachineName(pcmd)
				  );
	  }
   }

   fprintf(fout
		   , "\t%-*s fsm;\n};\n\n"
		   , (int)pcmd->sub_machine_struct_format_width + 6 /* for the "const " */
		   , fsmFnType(pcmd)
		   );
}

static void declareOrDefineSinglePAIEventHandler(pCMachineData pcmd, pMACHINE_INFO pmi, DECLARE_OR_DEFINE dod)
{
#ifdef FSMLANG_DEVELOP
	fprintf(pcmd->cFile
			, "/* declareOrDefineSinglePAIEventHandler */\n"
		   );
#endif
    fprintf(pcmd->cFile, "static ");
    if (pmi->modFlags & ACTIONS_RETURN_FLAGS)
    {
        fprintf(pcmd->cFile, "bool eventIsNotHandledInAllStates");
    }
    else
    {
        fprintf(pcmd->cFile
                , "%s checkWhetherEventIsHandledInAllStates"
				, eventType(pcmd)
               );
    }
    fprintf(pcmd->cFile
            , "(p%s%s,"
			, fsmType(pcmd)
			, dod == dod_declare ? "" : " pfsm"
            );
    fprintf(pcmd->cFile
            , "%s%s%s)%s"
			, eventType(pcmd)
            , dod == dod_declare ? ""  : " "
            , dod == dod_define  ? "e" : ""
            , dod == dod_declare ? ";\n" : "\n{\n"
           );

}

static void defineCSwitchMachineFSM(pFSMCOutputGenerator pfsmcog)
{
	pCMachineData pcmd = pfsmcog->pcmd;
	pMACHINE_INFO pmi  = pcmd->pmi;

   fprintf(pcmd->cFile, "\n");

   fprintf(pcmd->cFile
           , "#ifndef EVENT_IS_NOT_EXCLUDED_FROM_LOG\n"
           );

   fprintf(pcmd->cFile
           , "#define EVENT_IS_NOT_EXCLUDED_FROM_LOG(e) ((e) == (e))\n"
           );

   fprintf(pcmd->cFile
           , "#endif\n"
           );

   fprintf(pcmd->cFile
           , "void %sFSM(p"
           , pmi->name->name
           );
   streamHungarianToUnderbarCaps(pcmd->cFile, pmi->name->name);
   fprintf(pcmd->cFile
           , " pfsm, %s"
           , pmi->data_block_count ? "p"  : ""
          );
   streamHungarianToUnderbarCaps(pcmd->cFile, pmi->name->name);
   fprintf(pcmd->cFile, "_EVENT event)\n{\n");

   writeReentrantPrologue(pcmd);

   if (add_profiling_macros)
   {
       fprintf(pcmd->cFile, "\n\tFSM_ENTRY(pfsm);\n\n");
   }

   writeFSMLoop(pfsmcog);

   if (add_profiling_macros)
   {
       fprintf(pcmd->cFile, "\n\tFSM_EXIT(pfsm);\n\n");
   }

   writeReentrantEpilogue(pcmd);

   fprintf(pcmd->cFile, "\n}\n\n");

}

static void defineCSwitchSubMachineFSM(pFSMCOutputGenerator pfsmcog)
{
	pCMachineData pcmd = pfsmcog->pcmd;
	pMACHINE_INFO pmi  = pcmd->pmi;

	FSMLANG_DEVELOP_PRINTF(pcmd->cFile, "/* FSMLANG_DEVELOP: %s */\n", __func__);

   fprintf(pcmd->cFile, "\n");

   fprintf(pcmd->cFile
           , "#ifndef EVENT_IS_NOT_EXCLUDED_FROM_LOG\n"
           );

   fprintf(pcmd->cFile
           , "#define EVENT_IS_NOT_EXCLUDED_FROM_LOG(e) (e == e)\n"
           );

   fprintf(pcmd->cFile
           , "#endif\n"
           );

   if (pmi->states_with_entry_fns_count || pmi->states_with_exit_fns_count)
   {
	   declareStateEntryAndExitManagers(pcmd, pmi);
   }

   fprintf(pcmd->cFile
           , "%s %sFSM(p%s pfsm, %s event)\n{\n"
		   , subFsmFnReturnType(pcmd)
		   , machineName(pcmd)
		   , fsmType(pcmd)
		   , eventType(pcmd)
          );

   if (add_profiling_macros && profile_sub_fsms)
   {
       fprintf(pcmd->cFile, "\n\tFSM_ENTRY(pfsm);\n\n");
   }

   writeFSMLoop(pfsmcog);

   //The FSM_EXIT invocation is added in the loop writer, since it 
   //    must precede the return statement.

   fprintf(pcmd->cFile
           , "\n}\n\n"
          );

}

static bool define_void_returning_state_fn(pLIST_ELEMENT pelem, void *data)
{
    pID_INFO                  pstate = (pID_INFO) pelem->mbr;
    pITERATOR_CALLBACK_HELPER pich   = (pITERATOR_CALLBACK_HELPER) data;

    pLIST_ELEMENT             ptransitionEvent;

    (void) print_state_fn_signature(pelem, pich);

    /* are transitions possible? */
    pich->pOtherElem = pelem;
    ptransitionEvent = iterate_list(pich->ih.pmi->event_list
                                    , find_first_array_element_with_transition
                                    , pich
                                    );

    if (
        pich->ih.pmi->executes_fns_on_state_transitions
        && ptransitionEvent
       )
    {
        fprintf(pich->pcmd->cFile
				,"\t%s new_s = %s_%s"
				, stateType(pich->pcmd)
            , nfMachineName(pich->pcmd)
            , pstate->name
				);
        fprintf(pich->pcmd->cFile, ";\n");
    }

    fprintf(pich->pcmd->cFile, "\n\tswitch(e)\n\t{\n");

    pich->counter = 0;
    iterate_list(pstate->type_data.state_data.pevents_handled
                 , print_void_returning_state_fn_case
                 , pich
                 );

    if (pich->counter < pich->ih.pmi->event_list->count + 1)
    {
		fprintf(pich->pcmd->cFile, "\tdefault:\n");
		if (empty_cell_fn)
		{
			fprintf(pich->pcmd->cFile
					, "\t\tUFMN(%s)(pfsm);\n"
					, empty_cell_fn
					);
		}
		else
		{
			fprintf(pich->pcmd->cFile
					, "\t\t%s(\"%s_noAction\");\n"
					, core_logging_only ? "NON_CORE_DEBUG_PRINTF" : "DBG_PRINTF"
					, ufMachineName(pich->pcmd)
				   );
		}
		fprintf(pich->pcmd->cFile, "\t\tbreak;\n");
    }

    fprintf(pich->pcmd->cFile, "\t}\n");

    if (!pich->counter)
    {
        fprintf(pich->pcmd->cFile
                , "/* we only just now know we did not use this */\n\t(void) pfsm;\n"
                );
    }

    print_state_fn_epilogue(pich->pcmd, pich->ih.pmi, pstate, ptransitionEvent != NULL);
    
    return false;

}

static void print_state_fn_epilogue(pCMachineData pcmd, pMACHINE_INFO pmi, pID_INFO pstate, bool transitions_are_possible)
{
    if (
        pmi->executes_fns_on_state_transitions
        && transitions_are_possible
        )
    {
       fprintf(pcmd->cFile
			   , "\n\n\tif (%s_%s != new_s)\n\t{\n"
			   , machineName(pcmd)
			   , pstate->name
			   );

       if (pmi->machineTransition)
       {
          fprintf(pcmd->cFile
                  , "\t\tUFMN(%s)(pfsm, new_s);\n"
                  , pmi->machineTransition->name
                  );
       }

       if (pmi->states_with_exit_fns_count)
       {
           fprintf(pcmd->cFile
                   ,"\t\trunAppropriateExitFunction(%s%s_%s);\n"
                   , pmi->data ? "&pfsm->data, " : ""
                   , machineName(pcmd)
                   , pstate->name
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

    (void) print_state_fn_signature(pelem, pich);

    fprintf(pich->pcmd->cFile, "\t");
    fprintf(pich->pcmd->cFile
			, "%s retVal = THIS(noEvent);\n"
			, actionReturnType(pich->pcmd)
           );

    /* are transitions possible? */
    pich->pOtherElem = pelem;
    ptransitionEvent = iterate_list(pich->ih.pmi->event_list, find_first_array_element_with_transition, pich);

    if (
        pich->ih.pmi->executes_fns_on_state_transitions
        && ptransitionEvent
       )
    {
        fprintf(pich->pcmd->cFile
				,"\t%s new_s = %s_%s;\n"
				, stateType(pich->pcmd)
				, machineName(pich->pcmd)
				, pstate->name
				);
    }

    fprintf(pich->pcmd->cFile, "\n\tswitch(e)\n\t{\n");

    pich->counter = 0;
    iterate_list(pstate->type_data.state_data.pevents_handled
                 , print_event_returning_state_fn_case
                 , pich
                 );

    if (pich->counter < pich->ih.pmi->event_list->count + 1)
    {
		fprintf(pich->pcmd->cFile, "\tdefault:\n");
		if (empty_cell_fn)
		{
			fprintf(pich->pcmd->cFile
					, "\t\tUFMN(%s)(pfsm);\n"
					, empty_cell_fn
					);
		}
		else
		{
			fprintf(pich->pcmd->cFile
					, "\t\t%s(\"%s_noAction\");\n"
					, core_logging_only ? "NON_CORE_DEBUG_PRINTF" : "DBG_PRINTF"
					, ufMachineName(pich->pcmd)
				   );
		}
		fprintf(pich->pcmd->cFile, "\t\tbreak;\n");
    }

    fprintf(pich->pcmd->cFile, "\t}\n");

    if (!pich->counter)
    {
        fprintf(pich->pcmd->cFile
                , "\n\t/* we only just now know we did not use this */\n\t(void) pfsm;\n"
                );
    }

    print_state_fn_epilogue(pich->pcmd, pich->ih.pmi, pstate, ptransitionEvent != NULL);

    return false;

}

static bool define_state_returning_state_fn(pLIST_ELEMENT pelem, void *data)
{
    pID_INFO                  pstate = (pID_INFO) pelem->mbr;
    pITERATOR_CALLBACK_HELPER pich   = (pITERATOR_CALLBACK_HELPER) data;

    FSMLANG_DEVELOP_PRINTF(pich->pcmd->cFile
                           , "/* FSMLANG_DEVELOP: %s */\n"
                           , __func__
                           );

    (void) print_state_fn_signature(pelem, pich);

    //do the body
    fprintf(pich->pcmd->cFile
            , "\t%s retVal = %s_noTransition;\n"
            , stateType(pich->pcmd)
            , machineName(pich->pcmd)
           );

    if (pich->pcmd->pmi->executes_fns_on_state_transitions)
    {
        fprintf(pich->pcmd->cFile
                , "\t%s new_s = %s_%s;\n"
                , stateType(pich->pcmd)
                , nfMachineName(pich->pcmd)
                , pstate->name
                );
    }

    fprintf(pich->pcmd->cFile, "\n\tswitch(e)\n\t{\n");
    
    pich->counter    = 0;
    pich->pOtherElem = pelem;
    iterate_list(pstate->type_data.state_data.pevents_handled
                 , print_state_returning_state_fn_case
                 , pich
                 );

    if (pich->counter < pich->ih.pmi->event_list->count + 1)
    {
		fprintf(pich->pcmd->cFile, "\tdefault:\n");
		if (empty_cell_fn)
		{
			fprintf(pich->pcmd->cFile
					, "\t\tUFMN(%s)(pfsm);\n"
					, empty_cell_fn
					);
		}
		else
		{
			fprintf(pich->pcmd->cFile
					, "\t\t%s(\"%s_noAction\");\n"
					, core_logging_only ? "NON_CORE_DEBUG_PRINTF" : "DBG_PRINTF"
					, ufMachineName(pich->pcmd)
				   );
		}
		fprintf(pich->pcmd->cFile, "\t\tbreak;\n");
    }

    fprintf(pich->pcmd->cFile, "\t}\n");

    if (!pich->counter)
    {
        fprintf(pich->pcmd->cFile
                , "/* we only just now know we did not use this */\n\t(void) pfsm;\n"
                );
    }

    print_state_fn_epilogue(pich->pcmd, pich->ih.pmi, pstate, true);

    return false;

}

static void defineCSwitchMachineStateFns(pCMachineData pcmd, pMACHINE_INFO pmi)
{
    ITERATOR_CALLBACK_HELPER ich = {
		. ih = {
			.pmi    = pmi
		}
        , .counter = 0
        , .define = true
        , .pcmd   = pcmd
    };

   iterate_list(pmi->state_list
                , pmi->modFlags & mfActionsReturnVoid
                  ? define_void_returning_state_fn
                  : pmi->modFlags & mfActionsReturnStates
                    ? define_state_returning_state_fn
                    : define_event_returning_state_fn
                , &ich
                );

   fprintf(pcmd->cFile, "\n");
}

static bool find_first_array_element_with_transition(pLIST_ELEMENT pelem, void *data)
{ 
    /* interpret our data */
    pITERATOR_CALLBACK_HELPER pich       = (pITERATOR_CALLBACK_HELPER) data;

    /* now, locate the relevant action info element */
    pLIST_ELEMENT             peventElem = pelem;
    pLIST_ELEMENT             pstateElem = pich->pOtherElem;
    pACTION_INFO              pai        = pich->ih.pmi->actionArray[peventElem->ordinal][pstateElem->ordinal];

    /* do we have a transition? */
    return pai && pai->transition;
}

static bool print_event_returning_state_fn_case(pLIST_ELEMENT pelem, void *data)
{
    /* interpret our data */
    pITERATOR_CALLBACK_HELPER pich       = (pITERATOR_CALLBACK_HELPER) data;
    pID_INFO                  pevent     = (pID_INFO)                  pelem->mbr;

	FSMLANG_DEVELOP_PRINTF(pich->pcmd->cFile, "/* FSMLANG_DEVELOP: %s */\n", __func__);

    /* now, locate the relevant action info element */
    pLIST_ELEMENT             pnextElem  = pelem->next;
    pID_INFO                  pnextEvent = (pID_INFO) (pnextElem ? pnextElem->mbr : NULL);
    pLIST_ELEMENT             pstateElem = pich->pOtherElem;
    pACTION_INFO              pai        = pich->ih.pmi->actionArray[pevent->order]
                                                                    [pstateElem->ordinal]
                                                                    ;
    pACTION_INFO              paiNext    = pnextEvent 
                                           ? pich->ih.pmi->actionArray[pnextEvent->order]
                                                                      [pstateElem->ordinal]
                                           : NULL
                                           ;

    if (!pevent->type_data.event_data.single_pai_for_all_states)
    {
        if (pai)
        {
            pich->counter++;
            fprintf(pich->pcmd->cFile
					, "\tcase THIS(%s):\n"
					, pevent->name
					);

            if (pai != paiNext)
            {
                if (strlen(pai->action->name))
                {
                    if (add_profiling_macros)
                    {
                        fprintf(pich->pcmd->cFile
                                , "\t\tACTION_ENTRY(pfsm);\n"
                               );
                    }

                    fprintf(pich->pcmd->cFile
                            , "\t\tretVal = UFMN(%s)(pfsm);\n"
                            , pai->action->name
                            );

                    if (add_profiling_macros)
                    {
                        fprintf(pich->pcmd->cFile
                                , "\t\t\tACTION_EXIT(pfsm);\n"
                               );
                    }

                }
                else
                {
					fprintf(pich->pcmd->cFile
							, "\t\t%s(\"%s_noAction\");\n"
							, core_logging_only ? "NON_CORE_DEBUG_PRINTF" : "DBG_PRINTF"
							, ufMachineName(pich->pcmd)
						   );
                }

                if (pai->transition)
                {
                   fprintf(pich->pcmd->cFile
                           , "\t\t%s = "
                           , pich->pcmd->pmi->executes_fns_on_state_transitions
                             ? "new_s" 
                             : "pfsm->state"
                          );

                   if (pai->transition->type == STATE)
                   {
                       fprintf(pich->pcmd->cFile
                               ,"%s_%s;\n"
                               , machineName(pich->pcmd)
                               , pai->transition->name
                               );
                   }
                   else
                   {
                       fprintf(pich->pcmd->cFile
                           , "UFMN(%s)(pfsm,e);\n"
                           , pai->transition->name
                           );
                   }
                }

                fprintf(pich->pcmd->cFile, "\t\tbreak;\n");
            }

        }
    }

    return false;
}

static bool print_void_returning_state_fn_case(pLIST_ELEMENT pelem, void *data)
{
    /* interpret our data */
    pITERATOR_CALLBACK_HELPER pich       = (pITERATOR_CALLBACK_HELPER) data;
    pID_INFO                  pevent     = (pID_INFO)                  pelem->mbr;

	FSMLANG_DEVELOP_PRINTF(pich->pcmd->cFile, "/* FSMLANG_DEVELOP: %s */\n", __func__);

    /* now, locate the relevant action info element */
    pLIST_ELEMENT             pnextElem  = pelem->next;
    pID_INFO                  pnextEvent = (pID_INFO) (pnextElem ? pnextElem->mbr : NULL);
    pLIST_ELEMENT             pstateElem = pich->pOtherElem;
    pACTION_INFO              pai        = pich->ih.pmi->actionArray[pevent->order]
                                                                    [pstateElem->ordinal]
                                                                    ;

    pACTION_INFO              paiNext    = pnextEvent 
                                           ? pich->ih.pmi->actionArray[pnextEvent->order]
                                                                      [pstateElem->ordinal]
                                           : NULL
                                           ;

    if (!pevent->type_data.event_data.single_pai_for_all_states)
    {
        if (pai)
        {
            pich->counter++;
            fprintf(pich->pcmd->cFile
                    , "\tcase THIS(%s):\n"
                    , pevent->name
                    );

            if (pai != paiNext)
            {

                if (strlen(pai->action->name))
                {
                    if (add_profiling_macros)
                    {
                        fprintf(pich->pcmd->cFile
                                , "\t\tACTION_ENTRY(pfsm);\n"
                               );
                    }

                    fprintf(pich->pcmd->cFile
                            , "\t\tUFMN(%s)(pfsm);\n"
                            , pai->action->name
                            );

                    if (add_profiling_macros)
                    {
                        fprintf(pich->pcmd->cFile
                                , "\t\tACTION_EXIT(pfsm);\n"
                               );
                    }
                }
                else
                {
					fprintf(pich->pcmd->cFile, "\tdefault:\n");
					if (empty_cell_fn)
					{
						fprintf(pich->pcmd->cFile
								, "\t\tUFMN(%s)(pfsm);\n"
								, empty_cell_fn
								);
					}
					else
					{
						fprintf(pich->pcmd->cFile
								, "\t\t%s(\"%s_noAction\");\n"
								, core_logging_only ? "NON_CORE_DEBUG_PRINTF" : "DBG_PRINTF"
								, ufMachineName(pich->pcmd)
							   );
					}
					fprintf(pich->pcmd->cFile, "\t\tbreak;\n");
                }

                if (pai->transition)
                {
                   fprintf(pich->pcmd->cFile
                           , "\t\t%s = %s_%s%s;\n"
                           , pich->pcmd->pmi->executes_fns_on_state_transitions
                             ? "new_s" 
                             : "pfsm->state"
                           , fqMachineName(pich->pcmd)
                           , pai->transition->name
                           , pai->transition->type == STATE ? "" : "(pfsm,e)"
                          );

                }

                fprintf(pich->pcmd->cFile, "\t\tbreak;\n");
            }
        }
    }

    return false;
}

static bool print_state_returning_state_fn_case(pLIST_ELEMENT pelem, void *data)
{
    /* interpret our data */
    pITERATOR_CALLBACK_HELPER pich       = (pITERATOR_CALLBACK_HELPER) data;
    pID_INFO                  pevent     = (pID_INFO)                  pelem->mbr;

	FSMLANG_DEVELOP_PRINTF(pich->pcmd->cFile, "/* FSMLANG_DEVELOP: %s */\n", __func__);

    /* now, locate the relevant action info element */
    pLIST_ELEMENT             pnextElem  = pelem->next;
    pID_INFO                  pnextEvent = (pID_INFO) (pnextElem ? pnextElem->mbr : NULL);
    pLIST_ELEMENT             pstateElem = pich->pOtherElem;
    pACTION_INFO              pai        = pich->ih.pmi->actionArray[pevent->order]
                                                                    [pstateElem->ordinal]
                                                                    ;
    pACTION_INFO              paiNext    = pnextEvent 
                                           ? pich->ih.pmi->actionArray[pnextEvent->order]
                                                                      [pstateElem->ordinal]
                                           : NULL
                                           ;

    if (!pevent->type_data.event_data.single_pai_for_all_states)
    {
        if (pai)
        {
            pich->counter++;
            fprintf(pich->pcmd->cFile
                    , "\tcase THIS(%s):\n"
                    , pevent->name
                    );

            if (pai != paiNext)
            {
                if (strlen(pai->action->name))
                {
                    if (add_profiling_macros)
                    {
                        fprintf(pich->pcmd->cFile
                                , "\t\t\tACTION_ENTRY(pfsm);\n"
                               );
                    }

                    fprintf(pich->pcmd->cFile
                            , "\t\tretVal = UFMN(%s)(pfsm);\n"
                            , pai->action->name
                            );

                    if (add_profiling_macros)
                    {
                        fprintf(pich->pcmd->cFile
                                , "\t\t\tACTION_EXIT(pfsm);\n"
                               );
                    }

                }
                else
                {
                   fprintf(pich->pcmd->cFile
                           , "\t\tretVal = UFMN(%s)%s;\n"
                           , pai->transition->name
                           , pai->transition->type == STATE ? "" : "(pfsm,e)"
                           );
                }

                fprintf(pich->pcmd->cFile, "\t\tbreak;\n");
            }
        }
    }

    return false;
}

static void writeOriginalSwitchFSMLoopInnards(pFSMCOutputGenerator pfsmcog,char *tabstr)
{

   char *local_tabstr = "";

   pCMachineData pcmd = pfsmcog->pcmd;
   pMACHINE_INFO pmi  = pcmd->pmi;

   if (pmi->has_single_pai_events)
   {
      if (pmi->modFlags & ACTIONS_RETURN_FLAGS)
      {
         fprintf(pcmd->cFile
                 , "%s\tif (eventIsNotHandledInAllStates(pfsm,%s))\n"
                 , tabstr
				 , ((pmi->modFlags & mfActionsReturnVoid) && (pmi->data_block_count == 0))
				   ? "event" 
				   : "e"
                 );
      }
      else
      {
         fprintf(pcmd->cFile
                 , "%s\tif ((e = checkWhetherEventIsHandledInAllStates(pfsm,e)) != THIS(noEvent))\n"
                 , tabstr
                 );
      }
      local_tabstr = "\t";
   }

   if (pmi->modFlags & mfActionsReturnVoid)
   {
      fprintf(pcmd->cFile
              , "%s%s\t((* (*pfsm->statesArray)[pfsm->state])(pfsm,%s));\n"
              , tabstr
              , local_tabstr
			  , (pmi->data_block_count == 0) ? "event" : "e"
              );
   }
   else
   {
      fprintf(pcmd->cFile
              , "%s%s\te = ((* (*pfsm->statesArray)[pfsm->state])(pfsm,e));\n"
              , tabstr
              , local_tabstr
              );
   }

}

static void writeOriginalSwitchSubFSMLoopInnards(pFSMCOutputGenerator pfsmcog, char *tabstr)
{
   (void) tabstr;

   pCMachineData pcmd = pfsmcog->pcmd;
   pMACHINE_INFO pmi  = pfsmcog->pcmd->pmi;

   if (pmi->has_single_pai_events)
   {
      if (pmi->modFlags & ACTIONS_RETURN_FLAGS)
      {
         fprintf(pcmd->cFile
                 , "\tif (eventIsNotHandledInAllStates(pfsm,event))\n\t{\n"
                 );
      }
      else
      {
         fprintf(pcmd->cFile
                 , "\tif ((e = checkWhetherEventIsHandledInAllStates(pfsm,e)) != THIS(noEvent))\n"
                 );
      }
   }

   fprintf(pcmd->cFile
           , "\t\tif ((%s >= THIS(firstEvent))\n\t\t    && (%s < THIS(noEvent))\n\t\t\t)\n\t\t{\n"
		   , pmi->modFlags & ACTIONS_RETURN_FLAGS ? "event" : "e"
		   , pmi->modFlags & ACTIONS_RETURN_FLAGS ? "event" : "e"
           );

   if (pmi->modFlags & ACTIONS_RETURN_FLAGS)
   {
      fprintf(pcmd->cFile
              , "\t\t((* (*pfsm->statesArray)[pfsm->state])(pfsm,event));\n"
              );
   }
   else
   {
      fprintf(pcmd->cFile
              , "\t\te = ((* (*pfsm->statesArray)[pfsm->state])(pfsm,e));\n"
              );
   }

   fprintf(pcmd->cFile
           , "\t\t}\n"
           );

}

static void writeOriginalSwitchFSMAre(pFSMCOutputGenerator pfsmcog)
{
    pCMachineData pcmd = pfsmcog->pcmd;
    pMACHINE_INFO pmi  = pfsmcog->pcmd->pmi;

    fprintf(pcmd->cFile
            , "\t%s_EVENT%s e = event%s;\n\n"
            , ucfqMachineName(pcmd)
            , pmi->data_block_count ? "_ENUM"  : ""
            , pmi->data_block_count ? "->event" : ""
           );

    writeOriginalSwitchFSMLoopAre(pfsmcog);

}

static void writeOriginalSwitchFSMArv(pFSMCOutputGenerator pfsmcog)
{
    pCMachineData pcmd = pfsmcog->pcmd;
    pMACHINE_INFO pmi  = pfsmcog->pcmd->pmi;

	if (pmi->data_block_count != 0)
	{
		fprintf(pcmd->cFile
				, "\t%s_EVENT_ENUM e = event->event;\n\n"
				, ucfqMachineName(pcmd)
			   );
	}

    writeOriginalSwitchFSMLoopArv(pfsmcog);
}

static void writeOriginalSwitchSubFSMAre(pFSMCOutputGenerator pfsmcog)
{
    pCMachineData pcmd = pfsmcog->pcmd;

    fprintf(pcmd->cFile
            , "\t%s e = event;\n\n"
            , eventType(pcmd)
           );

    writeOriginalSwitchSubFSMLoopAre(pfsmcog);

}

static void writeOriginalSwitchSubFSMArv(pFSMCOutputGenerator pfsmcog)
{
	FSMLANG_DEVELOP_PRINTF(pfsmcog->pcmd->cFile, "/* FSMLANG_DEVELOP: %s */\n", __func__);
    writeOriginalSwitchSubFSMLoopArv(pfsmcog);
}

static bool print_state_fn_name(pLIST_ELEMENT pelem, void *data)
{
    pID_INFO                  pstate = (pID_INFO) pelem->mbr;
    pITERATOR_CALLBACK_HELPER pich   = (pITERATOR_CALLBACK_HELPER) data;

    fprintf(pich->pcmd->cFile
            , "\t%s%s_stateFn\n"
            , pich->ih.first ? (pich->ih.first = false, "  ") : ", "
			, pstate->name
           );

    return false;
}

static void defineStateFnArray(pCMachineData pcmd, pMACHINE_INFO pmi)
{

    ITERATOR_CALLBACK_HELPER ich = {
		. ih = {
			.pmi     = pmi
			, .first = true
		}
        , .pcmd  = pcmd
    };

   fprintf(pcmd->cFile
		   , "%sconst %s %s_state_fn_array[%s_numStates] = \n{\n"
		   , generate_instance ? "static " : ""
		   , stateFnType(pcmd)
		   , machineName(pcmd)
		   , machineName(pcmd)
		   );

   /* declare state fns */
   iterate_list(pmi->state_list, print_state_fn_name, &ich);

   fprintf(pcmd->cFile, "};\n\n");
}

static bool print_switch_cases_for_events_handled_in_all_states_arev(pLIST_ELEMENT pelem, void *data)
{
   pITERATOR_CALLBACK_HELPER pich  = (pITERATOR_CALLBACK_HELPER) data;
   pID_INFO                  event = (pID_INFO) pelem->mbr;
   pEVENT_DATA               ped   = &event->type_data.event_data;

   if (ped->single_pai_for_all_states)
   {
      pich->counter++;

      fprintf(pich->pcmd->cFile
			  , "\t\tcase %s_%s:\n"
			  , fqMachineName(pich->pcmd)
			  , event->name
			  );

      if (pich->ih.pmi->modFlags & mfActionsReturnVoid)
      {
          if (strlen(ped->psingle_pai->action->name))
          {
              if (add_profiling_macros)
              {
                  fprintf(pich->pcmd->cFile
                          , "\t\t\tACTION_ENTRY(pfsm);\n"
                         );
              }

              fprintf(pich->pcmd->cFile
                     , "\t\t\tUFMN(%s)(pfsm);\n"
                     , ped->psingle_pai->action->name
                     );

              if (add_profiling_macros)
              {
                  fprintf(pich->pcmd->cFile
                          , "\t\t\tACTION_EXIT(pfsm);\n"
                         );
              }

          }
          else
          {
			  fprintf(pich->pcmd->cFile
					  , "\t\tDBG_PRINTF(\"%s_noAction\");\n"
					  , ufMachineName(pich->pcmd)
					 );
          }

         if (ped->psingle_pai->transition)
         {
            fprintf(pich->pcmd->cFile
                    , "\t\t\t%s = UFMN(%s)%s;\n"
                     , (pich->ih.pmi->executes_fns_on_state_transitions)
                      ? "new_s" 
                      : "pfsm->state"
                    , ped->psingle_pai->transition->name
                    , ped->psingle_pai->transition->type == STATE
                       ? ""
                       : "(pfsm,e)"
                    );
         }

         
         fprintf(pich->pcmd->cFile
                , "\t\t\thandled = true;\n"
                );

      }
      else
      {
          if (strlen(event->type_data.event_data.psingle_pai->action->name))
          {
             if (add_profiling_macros)
             {
                 fprintf(pich->pcmd->cFile
                         , "\t\t\tACTION_ENTRY(pfsm);\n"
                         );
             }

             fprintf(pich->pcmd->cFile
                     , "\t\t\tretVal = UFMN(%s)(pfsm);\n"
                     , ped->psingle_pai->action->name
                     );

             if (add_profiling_macros)
             {
                 fprintf(pich->pcmd->cFile
                         , "\t\t\tACTION_EXIT(pfsm);\n"
                         );
             }

          }
          else
          {
             fprintf(pich->pcmd->cFile,"\t\t\tretVal = THIS(noEvent);\n");
			 if (empty_cell_fn && (ped->psingle_pai->transition == NULL))
			 {
				 fprintf(pich->pcmd->cFile
						 , "\t\tUFMN(%s)(pfsm);\n"
						 , empty_cell_fn
						 );
			 }
			 else
			 {
				 fprintf(pich->pcmd->cFile
						 , "\t\t\tDBG_PRINTF(\"%s_noAction\");\n"
						 , ufMachineName(pich->pcmd)
						 );
			 }
          }
          if (ped->psingle_pai->transition)
          {
             fprintf(pich->pcmd->cFile
                     , "\t\t\t%s = UFMN(%s)%s;\n"
                     , pich->pcmd->pmi->executes_fns_on_state_transitions
                       ? "new_s" : "pfsm->state"
                     , ped->psingle_pai->transition->name
                     , ped->psingle_pai->transition->type == STATE
                        ? ""
                        : "(pfsm,e)"
                     );
          }


      }

      fprintf(pich->pcmd->cFile
              , "\t\t\tbreak;\n"
              );

   }

   return false;
}

static bool print_switch_cases_for_events_handled_in_all_states_ars(pLIST_ELEMENT pelem, void *data)
{
   pITERATOR_CALLBACK_HELPER pich  = (pITERATOR_CALLBACK_HELPER) data;
   pID_INFO                  event = (pID_INFO) pelem->mbr;

   if (event->type_data.event_data.single_pai_for_all_states)
   {
      pich->counter++;

      fprintf(pich->pcmd->cFile
			  , "\t\tcase %s_%s:\n"
			  , fqMachineName(pich->pcmd)
			  , event->name
			  );

      fprintf(pich->pcmd->cFile
              , "%s\t\t\tpfsm->state = UFMN(%s)(pfsm);\n%s"
              , event->type_data.event_data.psingle_pai->transition
                ? ""
                : add_profiling_macros ? "\t\t\tACTION_ENTRY(pfsm);\n" : ""
              , event->type_data.event_data.psingle_pai->transition
                ? event->type_data.event_data.psingle_pai->transition->name
                : event->type_data.event_data.psingle_pai->action->name
              , event->type_data.event_data.psingle_pai->transition
                ? ""
                : add_profiling_macros ? "\t\t\tACTION_EXIT(pfsm);\n" : ""
              );
      
      fprintf(pich->pcmd->cFile
             , "\t\t\thandled = true;\n"
             );

      fprintf(pich->pcmd->cFile
              , "\t\t\tbreak;\n"
              );

   }

   return false;
}

static void defineAllStateHandler(pCMachineData pcmd, pMACHINE_INFO pmi)
{
   ITERATOR_CALLBACK_HELPER ich;

   ich.pcmd = pcmd;
   ich.ih.pmi  = pmi;

   declareOrDefineSinglePAIEventHandler(pcmd, pmi, dod_define);

   if (!(pmi->modFlags & ACTIONS_RETURN_FLAGS))
   {
      fprintf(pcmd->cFile, "\t");
      fprintf(pcmd->cFile
              , "%s retVal;\n"
			  , eventType(pcmd)
             );
   }
   else
   {
      fprintf(pcmd->cFile
              ,"\tbool handled = false;\n"
              );
   }

   if (pmi->executes_fns_on_state_transitions && !(pmi->modFlags & mfActionsReturnStates))
   {
      fprintf(pcmd->cFile
			  , "\t%s new_s = pfsm->state;\n\n"
			  , stateType(pcmd)
			  );
   }

   fprintf(pcmd->cFile
           ,"\tswitch (e)\n\t{\n"
           );

   ich.counter = 0;
   iterate_list(pmi->event_list
				, pmi->modFlags & mfActionsReturnStates
                    ? print_switch_cases_for_events_handled_in_all_states_ars
                    : print_switch_cases_for_events_handled_in_all_states_arev
				, &ich
				);

   if (ich.counter == pmi->event_list->count)
   {
      fprintf(stderr
              , "warning: (%s): all events are handled identically in all states.\n"
              , fqMachineName(pcmd)
             );
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

   if (pmi->executes_fns_on_state_transitions && !(pmi->modFlags & mfActionsReturnStates))
   {
      fprintf(pcmd->cFile
              , "\n\n\tif (pfsm->state != new_s)\n\t{\n"
             );

      if (pmi->machineTransition)
      {
         fprintf(pcmd->cFile
				 , "\t\t%s_%s(pfsm, new_s);\n"
				  , fqMachineName(pcmd)
				  , pmi->machineTransition->name
				 );
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
              , "\t\tpfsm->state = new_s;\n"
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

pFSMOutputGenerator generateCSwitchMachineWriter(pFSMOutputGenerator parent)
{
	pFSMOutputGenerator pfsmog;

	if (parent)
	{
		pFSMCSwitchOutputGenerator pfsmcswitchog = calloc(1, sizeof(FSMCSwitchOutputGenerator));

		pfsmcswitchog->fsmcog.fsmog.writeMachine = writeCSwitchSubMachine;
		pfsmcswitchog->fsmcog.fsmog.initOutput   = initCSubMachine;
		pfsmcswitchog->fsmcog.fsmog.closeOutput  = closeCMachine;
		pfsmcswitchog->fsmcog.fsmog.fsmogFactory = generateCSwitchMachineWriter;

		pfsmcswitchog->fsmcog.top_level_fsmcog = (pFSMCOutputGenerator)&CSwitchMachineWriter;
		pfsmcswitchog->fsmcog.parent_fsmcog    = (pFSMCOutputGenerator) parent;

		pfsmog = (pFSMOutputGenerator)pfsmcswitchog;
	}
	else
	{
		pfsmog = (pFSMOutputGenerator)&CSwitchMachineWriter;
	}

	if (output_generated_file_names_only)
	{
		pfsmog->initOutput   = parent ? initCSubMachineFN : initCMachineFN;
		pfsmog->writeMachine = writeCMachineFN;
		pfsmog->closeOutput  = closeCMachineFN;
	}

	return pfsmog;

}

static void chooseWorkerFunctions(pFSMCOutputGenerator pfsmcog)
{
	switch ((pfsmcog->pcmd->pmi->modFlags) & ACTIONS_RETURN_FLAGS)
	{
	case 0: // Actions return events
		pfsmcog->wfsm = pfsmcog->pcmd->pmi->parent
			               ? writeOriginalSwitchSubFSMAre
			               : writeOriginalSwitchFSMAre
			               ;
		break;

	case mfActionsReturnStates:
		pfsmcog->wfsm = pfsmcog->pcmd->pmi->parent
			            ? writeActionsReturnStateSwitchSubFSM
			            : writeActionsReturnStateSwitchFSM
			            ;
		break;

	case mfActionsReturnVoid:
		pfsmcog->wfsm = pfsmcog->pcmd->pmi->parent
			               ? writeOriginalSwitchSubFSMArv
			               : writeOriginalSwitchFSMArv
			               ;
		break;
	}
}

