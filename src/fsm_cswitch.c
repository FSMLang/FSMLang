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
static void            defineStateFnArray(pCMachineData,pMACHINE_INFO);
static void            cswitchMachineHeaderEnd(pCMachineData,pMACHINE_INFO,bool);
static void            cswitchSubMachineHeaderEnd(pCMachineData,pMACHINE_INFO,bool);
static bool            cswitch_sub_machine_declare_transition_fn_for_when_actions_return_events(pLIST_ELEMENT,void*);
static bool            areTransitionsPossible(pMACHINE_INFO,unsigned);
static bool            print_state_returning_state_fn_signature(pLIST_ELEMENT,void*);
static bool            print_event_returning_state_fn_signature(pLIST_ELEMENT,void*);
static void            start_event_returning_state_fn_signature(pITERATOR_CALLBACK_HELPER);
static void            start_state_returning_state_fn_signature(pITERATOR_CALLBACK_HELPER);
static bool            print_void_returning_state_fn_signature(pLIST_ELEMENT,void*);
static void            print_final_state_fn_signature_fragment(pID_INFO,pITERATOR_CALLBACK_HELPER);
static bool            print_state_fn_name(pLIST_ELEMENT,void*);
static void            declareOrDefineSinglePAIEventHandler(pCMachineData,pMACHINE_INFO,DECLARE_OR_DEFINE);
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
static void writeActionsReturnStateSwitchFSM(pCMachineData pcmd, pMACHINE_INFO pmi)
{
   char *tabstr = "";

   fprintf(pcmd->cFile, "\t");
   printNameWithAncestry("STATE", pmi, pcmd->cFile, "_", alc_upper, ai_include_self | ai_stop_at_parent);
   fprintf(pcmd->cFile
		   , " s%s;\n"
           , (pmi->machineTransition || pmi->states_with_entry_fns_count || pmi->states_with_exit_fns_count)
             ? " = THIS(noTransition)" 
             : ""
           );

   fprintf(pcmd->cFile, "\n\tDBG_PRINTF(\"event: %%s; start state: %%s\"\n\t\t,");
   printAncestry(pmi, pcmd->cFile, "_", alc_upper, ai_include_self);
   fprintf(pcmd->cFile, "_EVENT_NAMES[event]\n\t\t,");
   printAncestry(pmi, pcmd->cFile, "_", alc_upper, ai_include_self | ai_stop_at_parent);
   fprintf(pcmd->cFile, "_STATE_NAMES[pfsm->state]\n\t\t);\n\n");

   if (pmi->data_block_count)
   {
      fprintf(pcmd->cFile
              , "\ttranslateEventData(&pfsm->data, event);\n\n"
              );
   }

   if (pmi->has_single_pai_events)
   {
      fprintf(pcmd->cFile
              , "\t\tif (eventIsNotHandledInAllStates(pfsm,e))\n"
              );
      tabstr = "\t";
   }

   fprintf(pcmd->cFile
           , "\t\t%ss = ((* (*pfsm->statesArray)[pfsm->state])(pfsm,e));\n\n"
           , tabstr
           );

   fprintf(pcmd->cFile, "\tif (s != ");
   printAncestry(pmi, pcmd->cFile, "_", alc_lower, ai_include_self);
   fprintf(pcmd->cFile, "_noTransition)\n\t{\n");

   if (pmi->machineTransition || pmi->states_with_entry_fns_count || pmi->states_with_exit_fns_count)
   {
      fprintf(pcmd->cFile
              , "\t\tif (s != pfsm->state)\n\t\t{\n"
             );

      if (pmi->machineTransition)
      {
         fprintf(pcmd->cFile, "\t\t\t");
		 printAncestry(pmi, pcmd->cFile, "_", alc_lower, ai_include_self);
		 fprintf(pcmd->cFile
				 , "_%s(pfsm,s);\n"
                 , pmi->machineTransition->name
                );
      }

      if (pmi->states_with_exit_fns_count)
      {
         fprintf(pcmd->cFile
                 ,"\t\t\trunAppropriateExitFunction(%spfsm->state);\n"
                 , pmi->data ? "&pfsm->data, " : ""
                 );
      }

      if (pmi->states_with_entry_fns_count)
      {
         fprintf(pcmd->cFile
                 ,"\t\t\trunAppropriateEntryFunction(%ss);\n"
                 , pmi->data ? "&pfsm->data, " : ""
                 );
      }

      fprintf(pcmd->cFile
              , "\t\t}\n"
              );

   }

   fprintf(pcmd->cFile
           , "\t\tpfsm->state = s;\n\t}\n\n"
          );

   fprintf(pcmd->cFile, "\n\tDBG_PRINTF(\"end state: %%s\"\n\t\t,");
   printAncestry(pmi, pcmd->cFile, "_", alc_upper, ai_include_self | ai_stop_at_parent);
   fprintf(pcmd->cFile, "_STATE_NAMES[pfsm->state]\n\t\t);\n");

}

static void writeCSwitchMachine(pFSMOutputGenerator pfsmog, pMACHINE_INFO pmi)
{
   pFSMCOutputGenerator pfsmcog = (pFSMCOutputGenerator) pfsmog;

   writeCSwitchMachineInternal(pfsmcog->pcmd, pmi);

   if (pmi->machine_list)
   {
      write_machines(pmi->machine_list, generateCSwitchMachineWriter, pfsmog);
   }
}

static void writeCSwitchSubMachine(pFSMOutputGenerator pfsmog, pMACHINE_INFO pmi)
{

   pFSMCOutputGenerator pfsmcog = (pFSMCOutputGenerator) pfsmog;

   writeCSwitchSubMachineInternal(pfsmcog->pcmd, pmi);

   if (pmi->machine_list)
   {
      write_machines(pmi->machine_list, generateCSwitchMachineWriter, pfsmog);
   }
}

static int writeCSwitchMachineInternal(pCMachineData pcmd, pMACHINE_INFO pmi)
{
   if (!pmi || !pcmd) return 1;

   ITERATOR_CALLBACK_HELPER ich = {
       .pmi    = pmi
       , .pcmd = pcmd
   };

   commonHeaderStart(pcmd, pmi, "state_fn");

   declareCSwitchMachineStateFnArray(pcmd, pmi);

   declareCSwitchMachineStruct(pcmd, pmi);

   /* declare state fns */
   ich.define = false;
   iterate_list(pmi->state_list
                , pmi->modFlags & mfActionsReturnVoid 
                  ? print_void_returning_state_fn_signature 
				  : pmi->modFlags & mfActionsReturnStates
                    ? print_state_returning_state_fn_signature
                    : print_event_returning_state_fn_signature
                , &ich
                );

   fprintf(pcmd->hFile, "\n");

   cswitchMachineHeaderEnd(pcmd, pmi, false);

   /*
     Source File
   */

   addNativeImplementationIfThereIsAny(pmi, pcmd->cFile);

   defineStateFnArray(pcmd, pmi);

   defineSubMachineArray(pcmd, pmi);

   if (generate_instance)
   {
      generateInstance(pcmd, pmi, "state_fn");
   }

   if (generate_run_function)
   {
      generateRunFunction(pcmd, pmi);
   }

   defineCSwitchMachineFSM(pcmd, pmi);

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

   return 0;
}

static void print_final_state_fn_signature_fragment(pID_INFO pstate, pITERATOR_CALLBACK_HELPER pich)
{
    FILE *file = pich->define ? pich->pcmd->cFile : pich->pcmd->hFile;

    printNameWithAncestry(pstate->name, pich->pmi, file, "_", alc_lower, ai_include_self);
    fprintf(file, "_stateFn(p");
    printAncestry(pich->pmi, file, "_", alc_upper, ai_include_self);
    fprintf(file
			, "%s%s,"
			, pich->define ? " "    : ""
			, pich->define ? "pfsm" : ""
			);
    streamHungarianToUnderbarCaps(file, ultimateAncestor(pich->pmi)->name->name);
    fprintf(file
            , "_EVENT%s%s%s)%s"
            , ultimateAncestor(pich->pmi)->data_block_count ? "_ENUM" : ""
			, pich->define ? " "    : ""
			, pich->define ? "e" : ""
            , pich->define ? "\n{\n" : ";\n"
           );
}

static bool print_void_returning_state_fn_signature(pLIST_ELEMENT pelem, void *data)
{
    pID_INFO                  pstate = (pID_INFO) pelem->mbr;
    pITERATOR_CALLBACK_HELPER pich   = (pITERATOR_CALLBACK_HELPER) data;

    FILE *file = pich->define ? pich->pcmd->cFile : pich->pcmd->hFile;

    fprintf(file, "static void ");

    print_final_state_fn_signature_fragment(pstate, pich);

    return false;
}

static void start_event_returning_state_fn_signature(pITERATOR_CALLBACK_HELPER pich)
{
    pMACHINE_INFO pua  = ultimateAncestor(pich->pmi);
    FILE         *file = pich->define ? pich->pcmd->cFile : pich->pcmd->hFile;

    fprintf(file, "static ");
	streamHungarianToUnderbarCaps(file, pua->name->name);
    fprintf(file
            , "_EVENT%s "
            , (pua->data_block_count && !(pua->modFlags & mfActionsReturnStates))? "_ENUM"  : ""
            );
}

static bool print_event_returning_state_fn_signature(pLIST_ELEMENT pelem, void *data)
{
    pID_INFO                  pstate = (pID_INFO) pelem->mbr;
    pITERATOR_CALLBACK_HELPER pich   = (pITERATOR_CALLBACK_HELPER) data;

    start_event_returning_state_fn_signature(pich);
    print_final_state_fn_signature_fragment(pstate, pich);

    return false;
}

static void start_state_returning_state_fn_signature(pITERATOR_CALLBACK_HELPER pich)
{
    FILE *file = pich->define ? pich->pcmd->cFile : pich->pcmd->hFile;

    fprintf(file, "static ");
    streamHungarianToUnderbarCaps(file, pich->pmi->name->name);
    fprintf(file, "_STATE");
}

static bool print_state_returning_state_fn_signature(pLIST_ELEMENT pelem, void *data)
{
    pID_INFO                  pstate = (pID_INFO) pelem->mbr;
    pITERATOR_CALLBACK_HELPER pich   = (pITERATOR_CALLBACK_HELPER) data;

    start_state_returning_state_fn_signature(pich);
    print_final_state_fn_signature_fragment(pstate, pich);

    return false;
}

static int writeCSwitchSubMachineInternal(pCMachineData pcmd, pMACHINE_INFO pmi)
{
   if (!pmi || !pcmd) return 1;

   ITERATOR_CALLBACK_HELPER ich = {
       .pmi      = pmi
       , .pcmd   = pcmd
       , .define = false
   };

   subMachineHeaderStart(pcmd, pmi, "state_fn");

   declareCSwitchMachineStateFnArray(pcmd, pmi);

   declareCSwitchMachineStruct(pcmd, pmi);

   /* declare state fns */
   iterate_list(pmi->state_list
                , pmi->modFlags & mfActionsReturnVoid 
                  ? print_void_returning_state_fn_signature 
                  : pmi->modFlags & mfActionsReturnStates
                    ? print_state_returning_state_fn_signature
                    : print_event_returning_state_fn_signature
                , &ich
                );

   fprintf(pcmd->hFile, "\n");

   cswitchSubMachineHeaderEnd(pcmd, pmi, false);

   /*
     Source File
   */

   addNativeImplementationIfThereIsAny(pmi, pcmd->cFile);

   defineStateFnArray(pcmd, pmi);

   defineSubMachineIF(pcmd, pmi);

   possiblyDefineSubMachineSharedEventStructures(pcmd, pmi);

   defineSubMachineArray(pcmd, pmi);

   if (generate_instance)
   {
      generateInstance(pcmd, pmi, "state_fn");
   }

   defineCSwitchSubMachineFSM(pcmd, pmi);

   if (pmi->has_single_pai_events)
   {
      defineAllStateHandler(pcmd, pmi);
   }

   defineStateEntryAndExitManagers(pcmd, pmi);

   defineCSwitchMachineStateFns(pcmd, pmi);

   if (generate_weak_fns)
   {
      defineWeakActionFunctionStubs(pcmd, pmi);

      defineWeakDataTranslatorStubs(pcmd, pmi);

      defineWeakStateEntryAndExitFunctionStubs(pcmd, pmi);

   }
   else if (force_generation_of_event_passing_actions)
   {
      defineEventPassingActions(pcmd, pmi);
   }

   writeDebugInfo(pcmd, pmi);

   return 0;
}

static void declareCSwitchMachineStateFnArray(pCMachineData pcmd, pMACHINE_INFO pmi)
{
   fprintf(pcmd->hFile, "typedef ");
   if (pmi->modFlags & mfActionsReturnVoid)
   {
      fprintf(pcmd->hFile, "void ");
   }
   else
   {
	  if (pmi->modFlags & mfActionsReturnStates)
	  {
		  printNameWithAncestry("STATE", pmi, pcmd->hFile, "_", alc_upper, ai_include_self | ai_stop_at_parent);
	  }
	  else
	  {
		  streamHungarianToUnderbarCaps(pcmd->hFile, ultimateAncestor(pmi)->name->name);
		  fprintf(pcmd->hFile
				  , "_EVENT%s"
				  , ultimateAncestor(pmi)->data_block_count ? "_ENUM" : ""
				  );
	  }
   }

   fprintf(pcmd->hFile, " (*");
   printAncestry(pmi, pcmd->hFile, "_", alc_upper, ai_include_self);
   fprintf(pcmd->hFile, "_STATE_FN)(p");
   printAncestry(pmi, pcmd->hFile, "_", alc_upper, ai_include_self);
   fprintf(pcmd->hFile, ",");
   streamHungarianToUnderbarCaps(pcmd->hFile, ultimateAncestor(pmi)->name->name);
   fprintf(pcmd->hFile
		   , "_EVENT%s);\n\n"
		   , ultimateAncestor(pmi)->data_block_count ? "_ENUM"  : ""
		  );

   fprintf(pcmd->hFile, "extern const ");

   printNameWithAncestry("STATE_FN "       , pmi, pcmd->hFile, "_", alc_upper, ai_include_self);
   printNameWithAncestry("state_fn_array[" , pmi, pcmd->hFile, "_", alc_lower, ai_include_self);
   fprintf(pcmd->hFile
		   , "%s_numStates];\n\n"
		   , pmi->name->name
		   );
}

static void declareCSwitchMachineStruct(pCMachineData pcmd, pMACHINE_INFO pmi)
{
   /* put the machine structure definition into the header */
   fprintf(pcmd->hFile, "struct _");
   printAncestry(pmi, pcmd->hFile, "_", alc_lower, ai_include_self);
   fprintf(pcmd->hFile, "_struct_ {\n");

   if (pmi->data)
   {
      fprintf(pcmd->hFile, "\t");
	  printAncestry(pmi, pcmd->hFile, "_", alc_upper, ai_include_self);
	  fprintf(pcmd->hFile, "_DATA\t\t\t\t\tdata;\n");
   }

   fprintf(pcmd->hFile, "\t");
   streamHungarianToUnderbarCaps(pcmd->hFile, pmi->name->name);
   fprintf(pcmd->hFile, "_STATE\t\t\t\t\tstate;\n");

   fprintf(pcmd->hFile, "\t");
   streamHungarianToUnderbarCaps(pcmd->hFile, ultimateAncestor(pmi)->name->name);
   fprintf(pcmd->hFile
		   , "_EVENT%s\t\t\t\t\tevent;\n"
           , ultimateAncestor(pmi)->data_block_count ? "_ENUM"  : ""
           );

   fprintf(pcmd->hFile, "\t");
   printAncestry(pmi, pcmd->hFile, "_", alc_upper, ai_include_self);
   fprintf(pcmd->hFile, "_STATE_FN const\t(*statesArray)[");
   fprintf(pcmd->hFile
		   , "%s_numStates];\n"
		   , pmi->name->name
		   );

   if (pmi->machine_list)
   {
      fprintf(pcmd->hFile, "\tp");
	  printAncestry(pmi, pcmd->hFile, "_", alc_upper, ai_include_self);
	  fprintf(pcmd->hFile, "_SUB_FSM_IF\t(*subMachineArray)[");
	  printAncestry(pmi, pcmd->hFile, "_", alc_lower, ai_include_self);
	  fprintf(pcmd->hFile, "_numSubMachines];\n");
   }

   fprintf(pcmd->hFile, "\t");
   printAncestry(pmi, pcmd->hFile, "_", alc_upper, ai_include_self);
   fprintf(pcmd->hFile, "_FSM\t\t\t\t\t\tfsm;\n};\n\n");

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
        fprintf(pcmd->cFile, "bool eventIsNotHandledInAllStates(p");
    }
    else
    {
        streamHungarianToUnderbarCaps(pcmd->cFile, ultimateAncestor(pmi)->name->name);
        fprintf(pcmd->cFile
                , "_EVENT%s checkWhetherEventIsHandledInAllStates(p"
                , ultimateAncestor(pmi)->data_block_count ? "_ENUM" : ""
               );
    }
    printAncestry(pmi, pcmd->cFile, "_", alc_upper, ai_include_self);
    fprintf(pcmd->cFile
            , " %s,"
            , dod == dod_declare ? "" : "pfsm"
            );
    streamHungarianToUnderbarCaps(pcmd->cFile, ultimateAncestor(pmi)->name->name);
    fprintf(pcmd->cFile
            , "_EVENT%s%s%s)%s"
            , ultimateAncestor(pmi)->data_block_count ? "_ENUM" : ""
            , dod == dod_declare ? ""  : " "
            , dod == dod_define  ? "e" : ""
            , dod == dod_declare ? ";\n" : "\n{\n"
           );

}

static void defineCSwitchMachineFSM(pCMachineData pcmd, pMACHINE_INFO pmi)
{
   if (pmi->machine_list)
   {
      declareSubMachineManagers(pcmd, pmi);
   }

   if (pmi->data_block_count)
   {
      declareEventDataManager(pcmd, pmi);
   }

   if (pmi->has_single_pai_events)
   {
       declareOrDefineSinglePAIEventHandler(pcmd, pmi, dod_declare);
   }

   if (pmi->states_with_entry_fns_count || pmi->states_with_exit_fns_count)
   {
	   declareStateEntryAndExitManagers(pcmd, pmi);
   }

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

   if (!(pmi->modFlags & mfActionsReturnVoid))
   {
      fprintf(pcmd->cFile, "\t");
      printAncestry(pmi, pcmd->cFile, "_", alc_upper, ai_include_self);
      fprintf(pcmd->cFile
              , "_EVENT%s e = event%s;\n\n"
              , pmi->data_block_count ? "_ENUM"  : ""
              , pmi->data_block_count ? "->event" : ""
             );
   }

   if      (pmi->modFlags & mfReentrant)           writeReentrantSwitchFSM(pcmd, pmi);
   else if (pmi->modFlags & mfActionsReturnStates) writeActionsReturnStateSwitchFSM(pcmd, pmi);
   else                                            writeOriginalSwitchFSM(pcmd, pmi);

   fprintf(pcmd->cFile, "\n}\n\n");

}

static void defineCSwitchSubMachineFSM(pCMachineData pcmd, pMACHINE_INFO pmi)
{
   if (pmi->has_single_pai_events)
   {
       declareOrDefineSinglePAIEventHandler(pcmd, pmi, dod_declare);
   }

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

   streamHungarianToUnderbarCaps(pcmd->cFile, ultimateAncestor(pmi)->name->name);
   fprintf(pcmd->cFile
           , "_EVENT%s "
           , ultimateAncestor(pmi)->data_block_count ? "_ENUM"  : ""
          );
   printAncestry(pmi, pcmd->cFile, "_", alc_lower, ai_include_self);
   fprintf(pcmd->cFile, "FSM(p");
   printAncestry(pmi, pcmd->cFile, "_", alc_upper, ai_include_self);
   fprintf(pcmd->cFile, " pfsm, ");
   streamHungarianToUnderbarCaps(pcmd->cFile, pmi->name->name);
   fprintf(pcmd->cFile
           , "_EVENT%s event)\n{\n"
           , ultimateAncestor(pmi)->data_block_count ? "_ENUM"  : ""
          );

   if (!(pmi->modFlags & mfActionsReturnVoid))
   {
      fprintf(pcmd->cFile, "\t");
      streamHungarianToUnderbarCaps(pcmd->cFile, pmi->name->name);
      fprintf(pcmd->cFile
              , "_EVENT%s e = event;\n\n"
              , ultimateAncestor(pmi)->data_block_count ? "_ENUM"  : ""
             );
   }

   if      (pmi->modFlags & mfReentrant)           writeReentrantSwitchFSM(pcmd, pmi);
   else if (pmi->modFlags & mfActionsReturnStates) writeActionsReturnStateSwitchFSM(pcmd, pmi);
   else                                            writeOriginalSwitchSubFSM(pcmd, pmi);

   fprintf(pcmd->cFile
           , "\n}\n\n"
          );

}

static bool define_void_returning_state_fn(pLIST_ELEMENT pelem, void *data)
{
    pID_INFO                  pstate = (pID_INFO) pelem->mbr;
    pITERATOR_CALLBACK_HELPER pich   = (pITERATOR_CALLBACK_HELPER) data;

    pLIST_ELEMENT             ptransitionEvent;

    fprintf(pich->pcmd->cFile, "static void ");
    print_final_state_fn_signature_fragment(pstate, pich);

    /* are transitions possible? */
    pich->pOtherElem = pelem;
    ptransitionEvent = iterate_list(pich->pmi->event_list, find_first_array_element_with_transition, pich);

    if (
        (pich->pmi->machineTransition || pich->pmi->states_with_entry_fns_count || pich->pmi->states_with_exit_fns_count)
        && ptransitionEvent
       )
    {
        fprintf(pich->pcmd->cFile,"\t");
		streamHungarianToUnderbarCaps(pich->pcmd->hFile, pich->pmi->name->name);
        fprintf(pich->pcmd->cFile,"_STATE new_s = ");
		printNameWithAncestry(pstate->name, pich->pmi, pich->pcmd->cFile, "_", alc_lower, ai_include_self | ai_stop_at_parent);
        fprintf(pich->pcmd->cFile, ";\n");
    }

    fprintf(pich->pcmd->cFile, "\n\tswitch(e)\n\t{\n");

    pich->counter = 0;
    iterate_list(pich->pmi->event_list, print_event_returning_state_fn_case, pich);

    if (pich->counter < pich->pmi->event_list->count + 1)
    {
        fprintf(pich->pcmd->cFile
                , "\tdefault:\n\t\t%s(\""
                , core_logging_only ? "NON_CORE_DEBUG_PRINTF" : "DBG_PRINTF"
               );
        printNameWithAncestry("noAction", pich->pmi, pich->pcmd->cFile, "_", alc_lower, ai_include_self);
        fprintf(pich->pcmd->cFile, "\");\n\t\tbreak;\n");
    }

    fprintf(pich->pcmd->cFile, "\t}\n");

    if (!pich->counter)
    {
        fprintf(pich->pcmd->cFile
                , "/* we only just now know we did not use this */\n\t(void) pfsm;\n"
                );
    }

    print_state_fn_epilogue(pich->pcmd, pich->pmi, pstate, ptransitionEvent != NULL);
    
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
          fprintf(pcmd->cFile, ");\n");
       }

       if (pmi->states_with_entry_fns_count)
       {
           fprintf(pcmd->cFile
                   ,"\t\trunAppropriateEntryFunction(%s"
                   , pmi->data ? "&pfsm->data, " : ""
                  );
           printNameWithAncestry(pstate->name, pmi, pcmd->cFile, "_", alc_lower, ai_include_self);
           fprintf(pcmd->cFile, ");\n");
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
    print_final_state_fn_signature_fragment(pstate, pich);

    fprintf(pich->pcmd->cFile, "\t");
    streamHungarianToUnderbarCaps(pich->pcmd->cFile, ultimateAncestor(pich->pmi)->name->name);
    fprintf(pich->pcmd->cFile
			, "_EVENT%s retVal = "
            , ultimateAncestor(pich->pmi)->data_block_count ? "_ENUM"  : ""
           );
    printNameWithAncestry("noEvent", pich->pmi, pich->pcmd->cFile, "_", alc_lower, ai_include_self);
    fprintf(pich->pcmd->cFile, ";\n");

    /* are transitions possible? */
    pich->pOtherElem = pelem;
    ptransitionEvent = iterate_list(pich->pmi->event_list, find_first_array_element_with_transition, pich);

    if (
        (pich->pmi->machineTransition || pich->pmi->states_with_entry_fns_count || pich->pmi->states_with_exit_fns_count)
        && ptransitionEvent
       )
    {
        fprintf(pich->pcmd->cFile,"\t");
		printNameWithAncestry("STATE", pich->pmi, pich->pcmd->cFile, "_", alc_upper, ai_include_self | ai_stop_at_parent);
        fprintf(pich->pcmd->cFile, " new_s = ");
        printNameWithAncestry(pstate->name, pich->pmi, pich->pcmd->cFile, "_", alc_lower, ai_include_self | ai_stop_at_parent);
        fprintf(pich->pcmd->cFile, ";\n");
    }

    fprintf(pich->pcmd->cFile, "\n\tswitch(e)\n\t{\n");

    pich->counter = 0;
    iterate_list(pich->pmi->event_list, print_event_returning_state_fn_case, pich);

    if (pich->counter < pich->pmi->event_list->count + 1)
    {
        fprintf(pich->pcmd->cFile
                , "\tdefault:\n\t\t%s(\""
                , core_logging_only ? "NON_CORE_DEBUG_PRINTF" : "DBG_PRINTF"
               );
        printNameWithAncestry("noAction", pich->pmi, pich->pcmd->cFile, "_", alc_lower, ai_include_self);
        fprintf(pich->pcmd->cFile, "\");\n\t\tbreak;\n");
    }

    fprintf(pich->pcmd->cFile, "\t}\n");

    if (!pich->counter)
    {
        fprintf(pich->pcmd->cFile
                , "\n\t/* we only just now know we did not use this */\n\t(void) pfsm;\n"
                );
    }

    print_state_fn_epilogue(pich->pcmd, pich->pmi, pstate, ptransitionEvent != NULL);

    return false;

}

static bool define_state_returning_state_fn(pLIST_ELEMENT pelem, void *data)
{
    pID_INFO                  pstate = (pID_INFO) pelem->mbr;
    pITERATOR_CALLBACK_HELPER pich   = (pITERATOR_CALLBACK_HELPER) data;

    start_state_returning_state_fn_signature(pich);
    print_final_state_fn_signature_fragment(pstate, pich);

    //do the body
    fprintf(pich->pcmd->cFile, "\t");
    streamHungarianToUnderbarCaps(pich->pcmd->cFile, pich->pmi->name->name);
    fprintf(pich->pcmd->cFile
            , "_STATE retVal = %s_noTransition;\n"
            , pich->pmi->name->name
           );

    /* are transitions possible? */
    pich->pOtherElem = pelem;
    pID_INFO ptransitionEvent = (pID_INFO)(iterate_list(pich->pmi->event_list, find_first_array_element_with_transition, pich))->mbr;

    if (
        (pich->pmi->machineTransition || pich->pmi->states_with_entry_fns_count || pich->pmi->states_with_exit_fns_count)
       )
    {
        fprintf(pich->pcmd->cFile,"\t");
        printNameWithAncestry("STATE", pich->pmi, pich->pcmd->cFile, "_", alc_upper, ai_include_self | ai_stop_at_parent);
        fprintf(pich->pcmd->cFile, " new_s = ");
        printNameWithAncestry(pstate->name, pich->pmi, pich->pcmd->cFile, "_", alc_lower, ai_include_self | ai_stop_at_parent);
        fprintf(pich->pcmd->cFile, ";\n");
    }

    fprintf(pich->pcmd->cFile, "\n\tswitch(e)\n\t{\n");
    
    pich->counter = 0;
    iterate_list(pich->pmi->event_list, print_event_returning_state_fn_case, pich);

    fprintf(pich->pcmd->cFile, "\t}\n");

	print_state_fn_epilogue(pich->pcmd, pich->pmi, pstate, true);

    return false;

}

static void defineCSwitchMachineStateFns(pCMachineData pcmd, pMACHINE_INFO pmi)
{
    ITERATOR_CALLBACK_HELPER ich = {
        .counter = 0
        , .define = true
        , .pcmd   = pcmd
        , .pmi    = pmi
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
            fprintf(pich->pcmd->cFile, "\tcase ");
            printNameWithAncestry(pevent->name, pich->pmi, pich->pcmd->cFile, "_", alc_lower, ai_include_self);
            fprintf(pich->pcmd->cFile, ":\n");

            if (strlen(pai->action->name))
            {
                fprintf(pich->pcmd->cFile, "\t\tretVal = ");
                printNameWithAncestry(pai->action->name, pich->pmi, pich->pcmd->cFile, "_", alc_lower, ai_include_self);
                fprintf(pich->pcmd->cFile, "(pfsm);\n");
            }
            else
            {
                fprintf(pich->pcmd->cFile, "#ifdef ");
                printNameWithAncestry("DEBUG", pich->pmi, pich->pcmd->cFile, "_", alc_upper, ai_include_self);
                fprintf(pich->pcmd->cFile
                        , "\n\t\t%s(\""
                        , core_logging_only ? "NON_CORE_DEBUG_PRINTF" : "DBG_PRINTF"
                        );
                printNameWithAncestry("noAction", pich->pmi, pich->pcmd->cFile, "_", alc_upper, ai_include_self);
                fprintf(pich->pcmd->cFile, "\");\n#endif\n");
            }

            if (pai->transition)
            {
               fprintf(pich->pcmd->cFile
                       , "\t\t%s = "
                       , (pich->pmi->machineTransition || pich->pmi->states_with_entry_fns_count || pich->pmi->states_with_exit_fns_count)
                         ? "new_s" 
                         : "pfsm->state"
                      );

               printNameWithAncestry(pai->transition->name, pich->pmi, pich->pcmd->cFile, "_", alc_lower, ai_include_self);
               fprintf(pich->pcmd->cFile
                       , "%s;\n"
                       , pai->transition->type == STATE ? "" : "(pfsm,e)"
                       );
            }

            fprintf(pich->pcmd->cFile, "\t\tbreak;\n");
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
            fprintf(pich->pcmd->cFile, "\tcase ");
            printNameWithAncestry(pevent->name, pich->pmi, pich->pcmd->cFile, "_", alc_lower, ai_include_self);
            fprintf(pich->pcmd->cFile, ":\n");

            if (strlen(pai->action->name))
            {
                printNameWithAncestry(pai->action->name, pich->pmi, pich->pcmd->cFile, "_", alc_lower, ai_include_self);
                fprintf(pich->pcmd->cFile, "(pfsm);\n");
            }
            else
            {
                fprintf(pich->pcmd->cFile, "#ifdef ");
                printNameWithAncestry("DEBUG", pich->pmi, pich->pcmd->cFile, "_", alc_upper, ai_include_self);
                fprintf(pich->pcmd->cFile
                        , "\n\t\t%s(\""
                        , core_logging_only ? "NON_CORE_DEBUG_PRINTF" : "DBG_PRINTF"
                        );
                printNameWithAncestry("noAction", pich->pmi, pich->pcmd->cFile, "_", alc_upper, ai_include_self);
                fprintf(pich->pcmd->cFile, "\");\n#endif\n");
            }

            if (pai->transition)
            {
               fprintf(pich->pcmd->cFile
                       , "\t\t%s = "
                       , (pich->pmi->machineTransition || pich->pmi->states_with_entry_fns_count || pich->pmi->states_with_exit_fns_count)
                         ? "new_s" 
                         : "pfsm->state"
                      );

               printNameWithAncestry(pai->transition->name, pich->pmi, pich->pcmd->cFile, "_", alc_lower, ai_include_self);
               fprintf(pich->pcmd->cFile
                       , "%s;\n"
                       , pai->transition->type == STATE ? "" : "(pfsm,e)"
                       );
            }

            fprintf(pich->pcmd->cFile, "\t\tbreak;\n");
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
            fprintf(pich->pcmd->cFile, "\tcase ");
            printNameWithAncestry(pevent->name, pich->pmi, pich->pcmd->cFile, "_", alc_lower, ai_include_self);
            fprintf(pich->pcmd->cFile, ":\n");

            fprintf(pich->pcmd->cFile, "\t\tretVal = ");
            if (strlen(pai->action->name))
            {
                printNameWithAncestry(pai->action->name, pich->pmi, pich->pcmd->cFile, "_", alc_lower, ai_include_self);
                fprintf(pich->pcmd->cFile, "(pfsm);\n");
            }
            else
            {
               printNameWithAncestry(pai->transition->name, pich->pmi, pich->pcmd->cFile, "_", alc_lower, ai_include_self);
               fprintf(pich->pcmd->cFile
                       , "%s;\n"
                       , pai->transition->type == STATE ? "" : "(pfsm,e)"
                       );
            }

            fprintf(pich->pcmd->cFile, "\t\tbreak;\n");
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

static void writeOriginalSwitchFSMLoopInnards(pCMachineData pcmd, pMACHINE_INFO pmi, char *tabstr)
{

   char *local_tabstr = "";

   if (pmi->has_single_pai_events)
   {
      if (pmi->modFlags & ACTIONS_RETURN_FLAGS)
      {
         fprintf(pcmd->cFile
                 , "%s\tif (eventIsNotHandledInAllStates(pfsm,%s))\n"
                 , tabstr
                 , pmi->modFlags & mfActionsReturnVoid ? "event" : "e"
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
              , "%s%s\t((* (*pfsm->statesArray)[pfsm->state])(pfsm,event));\n"
              , tabstr
              , local_tabstr
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

   if (!(pmi->modFlags & mfActionsReturnVoid))
   {
      fprintf(pcmd->cFile
              , "%s}\n"
              , tabstr
              );
   }
}

static void writeOriginalSwitchFSMLoop(pCMachineData pcmd, pMACHINE_INFO pmi)
{
   char *tabstr = "\t";

   if (pmi->data_block_count)
   {
      fprintf(pcmd->cFile
              , "\ttranslateEventData(&pfsm->data, event);\n\n"
              );
   }

   if (!(pmi->modFlags & mfActionsReturnVoid))
   {
      fprintf(pcmd->cFile, "\twhile (e != %s_noEvent) {\n\n",
              pmi->name->name);
   }
   
   printFSMMachineDebugBlock(pcmd, pmi);

   fprintf(pcmd->cFile
           , "\t/* This is read-only data to facilitate error reporting in action functions */\n"
           );

   fprintf(pcmd->cFile
           , "\tpfsm->event = %s;\n\n"
           , (pmi->modFlags & mfActionsReturnVoid) ? "event" : "e"
           );

   if (pmi->machine_list)
   {
      fprintf(pcmd->cFile
              , "\t\tif (e < %s_noEvent)\n\t\t{\n"
              , pmi->name->name
              );
      tabstr = "\t\t";
   }

   writeOriginalSwitchFSMLoopInnards(pcmd,pmi,tabstr);

   if (pmi->machine_list)
   {
      fprintf(pcmd->cFile
              , "\t\telse\n\t\t{\n"
              );

      if (pmi->submachine_inhibitor_count)
      {
         fprintf(pcmd->cFile
                 , "\t\t\tif (doNotInhibitSubMachines(pfsm->state))\n\t"
                 );
      }
      fprintf(pcmd->cFile
              , "\t\t\te = findAndRunSubMachine(pfsm, e);"
              );

      fprintf(pcmd->cFile
              , "\n\t\t}\n\n\t}"
              );
   }

}

static void writeOriginalSwitchSubFSMLoop(pCMachineData pcmd, pMACHINE_INFO pmi)
{
   char *local_tabstr = "\t";

   if (!(pmi->modFlags & mfActionsReturnVoid))
   {
      fprintf(pcmd->cFile
              , "\twhile (\n\t\t(e != THIS(noEvent))\n\t\t&& (e >= THIS(%s))\n\t)\n\t{\n\n"
              , eventNameByIndex(pmi, 0)
              );
   }

   printFSMSubMachineDebugBlock(pcmd, pmi);

   fprintf(pcmd->cFile
           , "\t/* This is read-only data to facilitate error reporting in action functions */\n"
           );

   fprintf(pcmd->cFile
           , "\tpfsm->event = %s;\n\n"
           , (pmi->modFlags & mfActionsReturnVoid) ? "event" : "e"
           );

   if (pmi->has_single_pai_events)
   {
      if (pmi->modFlags & ACTIONS_RETURN_FLAGS)
      {
         fprintf(pcmd->cFile
                 , "%s\tif (eventIsNotHandledInAllStates(pfsm,%s))\n"
                 , local_tabstr
                 , pmi->modFlags & mfActionsReturnVoid ? "event" : "e"
                 );
      }
      else
      {
         fprintf(pcmd->cFile
                 , "%s\tif ((e = checkWhetherEventIsHandledInAllStates(pfsm,e)) != THIS(noEvent))\n"
                 , local_tabstr
                 );
      }
      local_tabstr = "\t\t";
   }

   if (pmi->modFlags & mfActionsReturnVoid)
   {
      fprintf(pcmd->cFile
              , "%s((* (*pfsm->statesArray)[pfsm->state])(pfsm,event));\n\n"
              , local_tabstr
              );
   }
   else
   {
      fprintf(pcmd->cFile
              , "%s\te = ((* (*pfsm->statesArray)[pfsm->state])(pfsm,e));\n\n"
              , local_tabstr
              );
   }

   if (!(pmi->modFlags & mfActionsReturnVoid))
   {

      fprintf(pcmd->cFile, "\t}\n");

      fprintf(pcmd->cFile
              , "\n\treturn e == THIS(noEvent) ? PARENT(noEvent) : e;"
              );
   }

}

static void writeOriginalSwitchFSM(pCMachineData pcmd, pMACHINE_INFO pmi)
{
   writeOriginalSwitchFSMLoop(pcmd, pmi);
}

static void writeOriginalSwitchSubFSM(pCMachineData pcmd, pMACHINE_INFO pmi)
{
   writeOriginalSwitchSubFSMLoop(pcmd, pmi);
}

static void writeReentrantSwitchFSM(pCMachineData pcmd, pMACHINE_INFO pmi)
{
   fprintf(pcmd->cFile, "#ifdef FSM_START_CRITICAL\n");
   fprintf(pcmd->cFile, "\tFSM_START_CRITICAL;\n");
   fprintf(pcmd->cFile, "#endif\n\n");

   writeOriginalSwitchFSMLoop(pcmd, pmi);

   fprintf(pcmd->cFile, "\n\n#ifdef FSM_END_CRITICAL\n");
   fprintf(pcmd->cFile, "\tFSM_END_CRITICAL;\n");
   fprintf(pcmd->cFile, "#endif\n\n");

}

static bool print_state_fn_name(pLIST_ELEMENT pelem, void *data)
{
    pID_INFO                  pstate = (pID_INFO) pelem->mbr;
    pITERATOR_CALLBACK_HELPER pich   = (pITERATOR_CALLBACK_HELPER) data;

    fprintf(pich->pcmd->cFile
            , "\t%s"
            , pich->first ? (pich->first = false, "  ") : ", "
           );
	printAncestry(pich->pmi, pich->pcmd->cFile, "_", alc_lower, ai_include_self);
    fprintf(pich->pcmd->cFile
			, "_%s_stateFn\n"
            , pstate->name
           );

    return false;
}

static void defineStateFnArray(pCMachineData pcmd, pMACHINE_INFO pmi)
{
    ITERATOR_CALLBACK_HELPER ich = {
        .pmi     = pmi
        , .pcmd  = pcmd
        , .first = true
    };

   fprintf(pcmd->cFile, "const ");
   printAncestry(pmi, pcmd->cFile, "_", alc_upper, ai_include_self);
   fprintf(pcmd->cFile, "_STATE_FN ");
   printAncestry(pmi, pcmd->cFile, "_", alc_lower, ai_include_self);
   fprintf(pcmd->cFile, "_state_fn_array[");
   fprintf(pcmd->cFile
		   , "%s_numStates] = \n{\n"
		   , pmi->name->name
		   );

   /* declare state fns */
   iterate_list(pmi->state_list, print_state_fn_name, &ich);

   fprintf(pcmd->cFile, "};\n\n");
}

void cswitchMachineHeaderEnd(pCMachineData pcmd, pMACHINE_INFO pmi, bool needNoOp)
{
   ITERATOR_CALLBACK_HELPER ich = { 0 };

   ich.pcmd      = pcmd;
   ich.pmi       = pmi;
   ich.needNoOp  = needNoOp;

   /* declare the action functions themselves */
   iterate_list(pmi->action_list, declare_action_function, &ich);

   /* declare the dummy, or no op action */
   if (needNoOp)
   {
      print_action_function_declaration(pmi, pcmd->hFile, "noAction");
   }

   fprintf(pcmd->hFile, "\n");

   /* declare any machine transition function */
   if (pmi->machineTransition)
   {
      fprintf(pcmd->hFile, "void ");
      printNameWithAncestry(pmi->machineTransition->name, pmi, pcmd->hFile, "_", alc_lower, ai_include_self);
      fprintf(pcmd->hFile, "(p");
      streamHungarianToUnderbarCaps(pcmd->hFile, pmi->name->name);
      fprintf(pcmd->hFile, ",");
      streamHungarianToUnderbarCaps(pcmd->hFile, pmi->name->name);
      fprintf(pcmd->hFile, "_STATE);\n\n");
   }

   /* declare any transition functions */
   if (pmi->modFlags & mfActionsReturnStates)
   {
      if (pmi->transition_fn_list->count)
      {
         streamHungarianToUnderbarCaps(pcmd->hFile, pmi->name->name);
         fprintf(pcmd->hFile
                 , "_STATE %s_noTransitionFn(p"
                 , pmi->name->name
                );
         streamHungarianToUnderbarCaps(pcmd->hFile, pmi->name->name);
         fprintf(pcmd->hFile, ");\n");

         iterate_list(pmi->transition_fn_list
                      , declare_transition_fn_for_when_actions_return_states
                      , &ich
                      );

      }

      fprintf(pcmd->hFile, "\n");

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

      fprintf(pcmd->hFile, "\n");

   }

   /* declare any entry or exit functions */
   if (pmi->states_with_entry_fns_count || pmi->states_with_exit_fns_count)
   {
      iterate_list(pmi->state_list, declare_state_entry_and_exit_functions, &ich);
      fprintf(pcmd->hFile, "\n");
   }

   /* declare needed data translators */
   if (pmi->data_block_count)
   {
      iterate_list(pmi->event_list, declare_data_translator_functions, &ich);
      fprintf(pcmd->hFile, "\n");
   }

}

void cswitchSubMachineHeaderEnd(pCMachineData pcmd, pMACHINE_INFO pmi, bool needNoOp)
{
   ITERATOR_CALLBACK_HELPER ich = { 0 };

   ich.pcmd      = pcmd;
   ich.pmi       = pmi;
   ich.needNoOp  = needNoOp;

   /* declare the action functions themselves */
   iterate_list(pmi->action_list, declare_action_function, &ich);

   /* declare the dummy, or no op action */
   if (needNoOp)
   {
      print_action_function_declaration(pmi, pcmd->hFile, "noAction");
   }

   fprintf(pcmd->hFile
           , "\n"
          );

   /* declare any machine transition function */
   if (pmi->machineTransition)
   {
      fprintf(pcmd->hFile, "void ");
      printAncestry(pmi, pcmd->hFile, "_", alc_lower, ai_include_self);
      fprintf(pcmd->hFile
              , "_%s(p"
              , pmi->machineTransition->name
             );
      printAncestry(pmi, pcmd->hFile, "_", alc_upper, ai_include_self);
      fprintf(pcmd->hFile, ",");
      printAncestry(pmi, pcmd->hFile, "_", alc_upper, ai_include_self | ai_stop_at_parent);
      fprintf(pcmd->hFile, "_STATE);\n\n");
   }

   /* declare any transition functions */
   if (pmi->modFlags & mfActionsReturnStates)
   {
      if (pmi->transition_fn_list->count)
      {
         print_transition_fn_declaration_for_when_actions_return_states(pmi, pcmd->hFile, "noTransition");
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

   iterate_list(pmi->event_list, declare_data_translator_functions, &ich);

   fprintf(pcmd->cFile, "\n");

   /* declare any entry or exit functions */
   if (pmi->states_with_entry_fns_count || pmi->states_with_exit_fns_count)
   {
      iterate_list(pmi->state_list, declare_state_entry_and_exit_functions, &ich);
      fprintf(pcmd->hFile, "\n");
   }

}

bool cswitch_sub_machine_declare_transition_fn_for_when_actions_return_events(pLIST_ELEMENT pelem, void *data)
{
   pITERATOR_CALLBACK_HELPER pich = ((pITERATOR_CALLBACK_HELPER)data);
   pID_INFO pid_info              = ((pID_INFO)pelem->mbr);

   printNameWithAncestry("STATE ", pich->pmi, pich->pcmd->hFile, "_", alc_upper, ai_include_self);
   printNameWithAncestry(pid_info->name, pich->pmi, pich->pcmd->hFile, "_", alc_upper, ai_include_self);
   fprintf(pich->pcmd->hFile, "(p");
   printAncestry(pich->pmi, pich->pcmd->hFile, "_", alc_upper, ai_include_self);
   fprintf(pich->pcmd->hFile, ",");
   streamHungarianToUnderbarCaps(pich->pcmd->hFile, ultimateAncestor(pich->pmi)->name->name);
   fprintf(pich->pcmd->hFile
		   , "_EVENT%s);\n"
           , ultimateAncestor(pich->pmi)->data_block_count ? "_ENUM"  : ""
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

      fprintf(pich->pcmd->cFile, "\t\tcase ");
      printNameWithAncestry(event->name, pich->pmi, pich->pcmd->cFile, "_", alc_lower, ai_include_self);
      fprintf(pich->pcmd->cFile, ":\n");

      if (pich->pmi->modFlags & ACTIONS_RETURN_FLAGS)
      {
         if (event->type_data.event_data.psingle_pai->transition)
         {
            fprintf(pich->pcmd->cFile
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
               fprintf(pich->pcmd->cFile
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
               fprintf(pich->pcmd->cFile
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
         
         fprintf(pich->pcmd->cFile
                , "\t\t\thandled = true;\n"
                );

      }
      else
      {
         if (event->type_data.event_data.psingle_pai->transition)
         {
            fprintf(pich->pcmd->cFile
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
            fprintf(pich->pcmd->cFile
                    , "\t\t\tretVal = %s_%s(pfsm);\n"
                    , pich->pmi->name->name
                    , event->type_data.event_data.psingle_pai->action->name
                    );
         }
         else
         {
            fprintf(pich->pcmd->cFile,"\t\t\tretVal = ");
            printNameWithAncestry("noEvent", pich->pmi, pich->pcmd->cFile, "_", alc_lower, ai_include_self);
            fprintf(pich->pcmd->cFile, ";\n");
         }

      }

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
      fprintf(pcmd->cFile, "_STATE new_s = pfsm->state;\n\n");
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
      write_machines(pmi->machine_list, generateCSwitchMachineWriter, pfsmog);
   }

}

pFSMOutputGenerator generateCSwitchMachineWriter(pFSMOutputGenerator parent)
{
	pFSMOutputGenerator pfsmog;

	if (parent)
	{
		pFSMCSubMachineOutputGenerator pfsmcswitchog = calloc(1, sizeof(FSMCSubMachineOutputGenerator));

		pfsmcswitchog->fsmog.writeMachine = writeCSwitchSubMachine;
		pfsmcswitchog->fsmog.initOutput   = initCSubMachine;
		pfsmcswitchog->fsmog.closeOutput  = closeCMachine;

		pfsmcswitchog->top_level_fsmcog = &CSwitchMachineWriter;
		pfsmcswitchog->parent_fsmcog    = (pFSMCOutputGenerator) parent;

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

