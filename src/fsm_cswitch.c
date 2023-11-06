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

static void            writeCSwitchMachine(pFSMOutputGenerator,pMACHINE_INFO);
static void            writeCSwitchSubMachine(pFSMOutputGenerator,pMACHINE_INFO);
static void            writeCSwitchMachineFN(pFSMOutputGenerator,pMACHINE_INFO);

static void            defineAllStateHandler(pCMachineData,pMACHINE_INFO,char*);
static void            defineCSwitchMachineFSM(pCMachineData,pMACHINE_INFO,char*);
static void            defineCSwitchSubMachineFSM(pCMachineData,pMACHINE_INFO,char*);
static void            declareCSwitchMachineStruct(pCMachineData,pMACHINE_INFO,char*);
static void            declareCSwitchMachineStateFnArray(pCMachineData,pMACHINE_INFO,char*);
static void            declareCSwitchSubMachineStateFnArray(pCMachineData,pMACHINE_INFO);
static void            writeActionsReturnStateSwitchFSM(pCMachineData, pMACHINE_INFO, char *);
static void            writeReentrantSwitchFSM(pCMachineData, pMACHINE_INFO, char *);
static void            writeOriginalSwitchFSM(pCMachineData, pMACHINE_INFO, char *);
static void            writeOriginalSwitchSubFSM(pCMachineData, pMACHINE_INFO, char *);
static int             writeCSwitchMachineInternal(pCMachineData, pMACHINE_INFO);
static int             writeCSwitchSubMachineInternal(pCMachineData, pMACHINE_INFO);
static void            writeOriginalSwitchFSMLoop(pCMachineData, pMACHINE_INFO, char *);
static void            writeOriginalSwitchFSMLoopInnards(pCMachineData, pMACHINE_INFO, char *, char *);
static void            writeOriginalSwitchSubFSMLoop(pCMachineData, pMACHINE_INFO, char *);
static void            defineCSwitchMachineStateFns(pCMachineData, pMACHINE_INFO, char *);
static void            defineCSwitchSubMachineStateFns(pCMachineData, pMACHINE_INFO, char *);
static void            defineStateFnArray(pCMachineData, pMACHINE_INFO, char *);
static void            cswitchHeaderEnd(pCMachineData, pMACHINE_INFO, char *, bool);
static void            cswitchSubMachineHeaderEnd(pCMachineData, pMACHINE_INFO, char *, bool);
static bool            cswitch_sub_machine_declare_transition_fn_for_when_actions_return_events(pLIST_ELEMENT,void*);
static bool            areTransitionsPossible(pMACHINE_INFO,unsigned);

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
static void writeActionsReturnStateSwitchFSM(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{
   char *tabstr = "";

   fprintf(pcmw->cFile, "\t%s_STATE s%s;\n"
           , cp
           , (pmi->machineTransition || pmi->states_with_entry_fns_count || pmi->states_with_exit_fns_count)
             ? " = THIS(noTransition)" 
             : ""
           );

   fprintf(pcmw->cFile
           , "\n\tDBG_PRINTF(\"event: %%s; start state: %%s\"\n\t\t,%s_EVENT_NAMES[event]\n\t\t,%s_STATE_NAMES[pfsm->state]\n\t\t);\n\n"
           , cp
           , cp
          );

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

   fprintf(pcmw->cFile
           , "\tif (s != %s_noTransition)\n\t{\n"
           , pmi->name->name
           );

   if (pmi->machineTransition || pmi->states_with_entry_fns_count || pmi->states_with_exit_fns_count)
   {
      fprintf(pcmw->cFile
              , "\t\tif (s != pfsm->state)\n\t\t{\n"
             );

      if (pmi->machineTransition)
      {
         fprintf(pcmw->cFile
                 , "\t\t\t%s_%s(pfsm,s);\n"
                 , pmi->name->name
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

   fprintf(pcmw->cFile
           , "\n\tDBG_PRINTF(\"end state: %%s\"\n\t\t,%s_STATE_NAMES[pfsm->state]\n\t\t);\n"
           , cp
          );

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
   char     *cp;

   if (!pmi || !pcmw) return 1;

   if ((cp = commonHeaderStart(pcmw, pmi, "state_fn")) == NULL) return 1;

   declareCSwitchMachineStateFnArray(pcmw, pmi, cp);

   declareCSwitchMachineStruct(pcmw, pmi, cp);

   /* declare state fns */
   for (unsigned i = 0; i < pmi->state_list->count; i++)
   {
      if (pmi->modFlags & mfActionsReturnVoid)
      {
         fprintf(pcmw->hFile
                 , "void %s_%s_stateFn(p%s,%s_EVENT%s);\n"
                 , pmi->name->name
                 , stateNameByIndex(pmi, i)
                 , cp
                 , cp
                 , pmi->data_block_count ? "_ENUM"  : ""
                );
      }
      else
      {
         fprintf(pcmw->hFile
                 , "%s_%s%s %s_%s_stateFn(p%s,%s_EVENT%s);\n"
                 , cp
                 , (pmi->modFlags & mfActionsReturnStates) ? "STATE" : "EVENT"
                 , (pmi->data_block_count && !(pmi->modFlags & mfActionsReturnStates))? "_ENUM"  : ""
                 , pmi->name->name
                 , stateNameByIndex(pmi, i)
                 , cp
                 , cp
                 , pmi->data_block_count ? "_ENUM"  : ""
                );
      }
   }

   fprintf(pcmw->hFile, "\n");

   cswitchHeaderEnd(pcmw, pmi, cp, false);

   /*
     Source File
   */

   addNativeImplementationIfThereIsAny(pmi, pcmw->cFile);

   defineStateFnArray(pcmw, pmi, cp);

   defineSubMachineArray(pcmw, pmi, cp);

   if (generate_instance)
   {
      generateInstance(pcmw, pmi, cp, "state_fn");
   }

   if (generate_run_function)
   {
      generateRunFunction(pcmw, pmi, cp);
   }

   defineCSwitchMachineFSM(pcmw, pmi, cp);

   /* write our sub-machine lookup, if needed */
   if (pmi->machine_list)
   {
      defineSubMachineFinder(pcmw, pmi, cp);
   }

   if (pmi->has_single_pai_events)
   {
      defineAllStateHandler(pcmw, pmi, cp);
   }

   defineStateEntryAndExitManagers(pcmw, pmi, cp);

   if (pmi->data_block_count)
   {
      defineEventDataManager(pcmw, pmi, cp);
   }

   defineCSwitchMachineStateFns(pcmw, pmi, cp);

   if (generate_weak_fns)
   {

      defineWeakActionFunctionStubs(pcmw, pmi, cp);

      defineWeakStateEntryAndExitFunctionStubs(pcmw, pmi, cp);

      if (pmi->data_block_count)
      {
         defineWeakDataTranslatorStubs(pcmw, pmi, cp);
      }

   }
   else if (force_generation_of_event_passing_actions)
   {
      defineEventPassingActions(pcmw, pmi, cp);
   }

   writeDebugInfo(pcmw, pmi, cp);

   FREE_AND_CLEAR(cp);

   return 0;
}

static int writeCSwitchSubMachineInternal(pCMachineData pcmw, pMACHINE_INFO pmi)
{
   if (!pmi || !pcmw) return 1;

   char *parent_cp = hungarianToUnderbarCaps(pmi->parent->name->name);

   subMachineHeaderStart(pcmw, pmi, "state_fn");

   declareCSwitchSubMachineStateFnArray(pcmw, pmi, cp);

   declareCSwitchMachineStruct(pcmw, pmi, cp);

   /* declare state fns */
   for (unsigned i = 0; i < pmi->state_list->count; i++)
   {
      if (pmi->modFlags & mfActionsReturnVoid)
      {
         fprintf(pcmw->hFile
                 , "void %s_%s_stateFn(p%s,%s_EVENT%s);\n"
                 , pmi->name->name
                 , stateNameByIndex(pmi, i)
                 , cp
                 , parent_cp
                 , pmi->parent->data_block_count ? "_ENUM"  : ""
                );
      }
      else
      {
         fprintf(pcmw->hFile
                 , "%s_%s%s %s_%s_stateFn(p%s,%s_EVENT%s);\n"
                 , parent_cp
                 , (pmi->modFlags & mfActionsReturnStates) ? "STATE" : "EVENT"
                 , (pmi->parent->data_block_count && !(pmi->modFlags & mfActionsReturnStates))? "_ENUM"  : ""
                 , pmi->name->name
                 , stateNameByIndex(pmi, i)
                 , cp
                 , parent_cp
                 , pmi->parent->data_block_count ? "_ENUM"  : ""
                );
      }
   }

   fprintf(pcmw->hFile, "\n");

   cswitchSubMachineHeaderEnd(pcmw, pmi, cp, false);

   /*
     Source File
   */

   addNativeImplementationIfThereIsAny(pmi, pcmw->cFile);

   defineStateFnArray(pcmw, pmi, cp);

   defineSubMachineIF(pcmw, pmi, cp);

   possiblyDefineSubMachineSharedEventStructures(pcmw, pmi, cp);

   defineSubMachineArray(pcmw, pmi, cp);

   if (generate_instance)
   {
      generateInstance(pcmw, pmi, cp, "state_fn");
   }

   defineCSwitchSubMachineFSM(pcmw, pmi, cp);

   if (pmi->has_single_pai_events)
   {
      defineAllStateHandler(pcmw, pmi, parent_cp);
   }

   defineStateEntryAndExitManagers(pcmw, pmi, cp);

   defineCSwitchSubMachineStateFns(pcmw, pmi, cp);

   if (generate_weak_fns)
   {
      defineSubMachineWeakActionFunctionStubs(pcmw, pmi, cp);

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

static void declareCSwitchMachineStateFnArray(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{
   if (pmi->modFlags & mfActionsReturnVoid)
   {
      fprintf(pcmw->hFile
              , "typedef void (*%s_STATE_FN)(p%s,%s_EVENT%s);\n\n"
              , cp
              , cp
              , cp
              , pmi->data_block_count ? "_ENUM"  : ""
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

static void declareCSwitchMachineStruct(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{
   char *parent_cp = NULL;
   pMACHINE_INFO pdata_block = pmi->parent ? pmi->parent : pmi;

   /* put the machine structure definition into the header */
   fprintf(pcmw->hFile, "struct _%s_struct_ {\n",
           pmi->name->name);

   if (pmi->data)
   {
      fprintf(pcmw->hFile, "\t%s_DATA\t\t\t\t\tdata;\n",
              cp);
   }

   fprintf(pcmw->hFile, "\t%s_STATE\t\t\t\t\tstate;\n",
           cp);

   fprintf(pcmw->hFile
           , "\t%s_EVENT%s\t\t\t\t\tevent;\n"
           , pmi->parent ? (parent_cp = hungarianToUnderbarCaps(pmi->parent->name->name)) : cp
           , pdata_block->data_block_count ? "_ENUM"  : ""
           );

   CHECK_AND_FREE(parent_cp);

   fprintf(pcmw->hFile, "\t%s_STATE_FN const\t(*statesArray)[%s_numStates];\n"
           , cp
           , pmi->name->name
          );

   if (pmi->machine_list)
   {
      fprintf(pcmw->hFile
              , "\tp%s_SUB_FSM_IF\t(*subMachineArray)[%s_numSubMachines];\n"
              , cp
              , pmi->name->name
             );
   }

   fprintf(pcmw->hFile, "\t%s_FSM\t\t\t\t\t\tfsm;\n};\n\n",
           cp);

}

static void defineCSwitchMachineFSM(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{
   if (pmi->machine_list)
   {
      declareSubMachineManagers(pcmw, pmi, cp);
   }

   if (pmi->data_block_count)
   {
      declareEventDataManager(pcmw, cp);
   }

   if (pmi->has_single_pai_events)
   {
      if (pmi->data_block_count)
      {
         fprintf(pcmw->cFile
                 , pmi->modFlags & ACTIONS_RETURN_FLAGS 
                     ? "static %s %s(p%s,p%s_EVENT);\n"
                     : "static %s_EVENT_ENUM %s(p%s,%s_EVENT_ENUM);\n"
                 , pmi->modFlags & ACTIONS_RETURN_FLAGS ? "bool" : cp
                 , pmi->modFlags & ACTIONS_RETURN_FLAGS 
                    ? "eventIsNotHandledInAllStates" 
                    : "checkWhetherEventIsHandledInAllStates"
                 , cp
                 , cp
                 );
      }
      else
      {
         fprintf(pcmw->cFile
                 , pmi->modFlags & ACTIONS_RETURN_FLAGS 
                     ? "static %s %s(p%s,%s_EVENT);\n"
                     : "static %s_EVENT %s(p%s,%s_EVENT);\n"
                 , pmi->modFlags & ACTIONS_RETURN_FLAGS ? "bool" : cp
                 , pmi->modFlags & ACTIONS_RETURN_FLAGS 
                    ? "eventIsNotHandledInAllStates" 
                    : "checkWhetherEventIsHandledInAllStates"
                 , cp
                 , cp
                 );
      }
   }

   declareStateEntryAndExitManagers(pcmw, pmi, cp);

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

   fprintf(pcmw->cFile
           , "void %sFSM(p%s pfsm, %s%s_EVENT event)\n{\n"
           , pmi->name->name
           , cp
           , pmi->data_block_count ? "p"  : ""
           , cp
          );

   if (!(pmi->modFlags & mfActionsReturnVoid))
   {
      fprintf(pcmw->cFile
              , "\t%s_EVENT%s e = event%s;\n\n"
              , cp
              , pmi->data_block_count ? "_ENUM"  : ""
              , pmi->data_block_count ? "->event" : ""
             );
   }

   if      (pmi->modFlags & mfReentrant)           writeReentrantSwitchFSM(pcmw, pmi, cp);
   else if (pmi->modFlags & mfActionsReturnStates) writeActionsReturnStateSwitchFSM(pcmw, pmi, cp);
   else                                            writeOriginalSwitchFSM(pcmw, pmi, cp);

   fprintf(pcmw->cFile
           , "\n}\n\n"
          );

}

static void defineCSwitchSubMachineFSM(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{
   char *parent_cp = hungarianToUnderbarCaps(pmi->parent->name->name);

   if (pmi->has_single_pai_events)
   {
      if (pmi->parent->data_block_count)
      {
         fprintf(pcmw->cFile
                 , pmi->modFlags & ACTIONS_RETURN_FLAGS 
                     ? "static %s %s(p%s,p%s_EVENT);\n"
                     : "static %s_EVENT_ENUM %s(p%s,%s_EVENT_ENUM);\n"
                 , pmi->modFlags & ACTIONS_RETURN_FLAGS ? "bool" : parent_cp
                 , pmi->modFlags & ACTIONS_RETURN_FLAGS 
                    ? "eventIsNotHandledInAllStates" 
                    : "checkWhetherEventIsHandledInAllStates"
                 , cp
                 , parent_cp
                 );
      }
      else
      {
         fprintf(pcmw->cFile
                 , pmi->modFlags & ACTIONS_RETURN_FLAGS 
                     ? "static %s %s(p%s,%s_EVENT);\n"
                     : "static %s_EVENT %s(p%s,%s_EVENT);\n"
                 , pmi->modFlags & ACTIONS_RETURN_FLAGS ? "bool" : parent_cp
                 , pmi->modFlags & ACTIONS_RETURN_FLAGS 
                    ? "eventIsNotHandledInAllStates" 
                    : "checkWhetherEventIsHandledInAllStates"
                 , cp
                 , parent_cp
                 );
      }
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

   declareStateEntryAndExitManagers(pcmw, pmi, cp);

   fprintf(pcmw->cFile
           , "%s_EVENT%s %sFSM(p%s pfsm, %s_EVENT%s event)\n{\n"
           , parent_cp
           , pmi->parent->data_block_count ? "_ENUM"  : ""
           , pmi->name->name
           , cp
           , parent_cp
           , pmi->parent->data_block_count ? "_ENUM"  : ""
          );

   if (!(pmi->modFlags & mfActionsReturnVoid))
   {
      fprintf(pcmw->cFile
              , "\t%s_EVENT%s e = event;\n\n"
              , parent_cp
              , pmi->parent->data_block_count ? "_ENUM"  : ""
             );
   }

   if      (pmi->modFlags & mfReentrant)           writeReentrantSwitchFSM(pcmw, pmi, cp);
   else if (pmi->modFlags & mfActionsReturnStates) writeActionsReturnStateSwitchFSM(pcmw, pmi, cp);
   else                                            writeOriginalSwitchSubFSM(pcmw, pmi, cp);

   fprintf(pcmw->cFile
           , "\n}\n\n"
          );

   FREE_AND_CLEAR(parent_cp);
}

static void defineCSwitchMachineStateFns(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{
   unsigned events_handled;
   bool     transitions_are_possible;

   for (unsigned i = 0; i < pmi->state_list->count; i++)
   {
      transitions_are_possible = areTransitionsPossible(pmi,i);

      if (pmi->modFlags & mfActionsReturnVoid)
      {
         fprintf(pcmw->cFile
                 , "void %s_%s_stateFn(p%s pfsm, %s_EVENT%s e)\n{\n"
                 , pmi->name->name
                 , stateNameByIndex(pmi, i)
                 , cp
                 , cp
                 , pmi->data_block_count ? "_ENUM"  : ""
                );
      }
      else
      {
         fprintf(pcmw->cFile
                 , "%s_%s%s %s_%s_stateFn(p%s pfsm, %s_EVENT%s e)\n{\n"
                 , cp
                 , (pmi->modFlags & mfActionsReturnStates) ? "STATE" : "EVENT"
                 , (pmi->data_block_count && !(pmi->modFlags & mfActionsReturnStates))? "_ENUM"  : ""
                 , pmi->name->name
                 , stateNameByIndex(pmi, i)
                 , cp
                 , cp
                 , pmi->data_block_count ? "_ENUM"  : ""
                );

         fprintf(pcmw->cFile
                 , "\t%s_%s%s retVal = %s_no%s;\n"
                 , cp
                 , (pmi->modFlags & mfActionsReturnStates) ? "STATE" : "EVENT"
                 , (pmi->data_block_count && !(pmi->modFlags & mfActionsReturnStates))? "_ENUM"  : ""
                 , pmi->name->name
                 , (pmi->modFlags & mfActionsReturnStates) ? "Transition" : "Event"
                );
      }

      if (
          (pmi->machineTransition || pmi->states_with_entry_fns_count || pmi->states_with_exit_fns_count)
          && transitions_are_possible
          )
      {
         fprintf(pcmw->cFile
                 , "\t%s_STATE new_s  = %s_%s;\n"
                 , cp
                 , pmi->name->name
                 , stateNameByIndex(pmi, i)
                );

      }

      fprintf(pcmw->cFile
              , "\n\tswitch(e)\n\t{\n"
             );

      events_handled   = 0;
      for (unsigned j = 0; j < pmi->event_list->count; j++)
      {
         if (!eventPidByIndex(pmi, j)->type_data.event_data.single_pai_for_all_states)
         {
            if (pmi->actionArray[j][i])
            {
               events_handled++;

               fprintf(pcmw->cFile
                       , "\tcase %s_%s:\n"
                       , pmi->name->name
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
                             , "#ifdef %s_DEBUG\n\t\t%s(\"%s_noAction\");\n#endif\n"
                             , cp
                             , core_logging_only ? "NON_CORE_DEBUG_PRINTF" : "DBG_PRINTF"
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
         #if 0
         {
            events_handled++;
         }
         #endif
      }

      if (events_handled < pmi->event_list->count + 1)
      {
         fprintf(pcmw->cFile
                 , "\tdefault:\n\t\t%s(\"%s_noAction\");\n\t\tbreak;\n"
                 , core_logging_only ? "NON_CORE_DEBUG_PRINTF" : "DBG_PRINTF"
                 , pmi->name->name
                );
      }

      if (!events_handled)
      {
         fprintf(pcmw->cFile
                 , "/* we only just now know we did not use this */\n\t(void) pfsm;\n"
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

         fprintf(pcmw->cFile
                 , "\t\tpfsm->state = new_s;\n\n"
                );

         fprintf(pcmw->cFile
                 , "\t}\n\n"
                );
      }

      if (!(pmi->modFlags & mfActionsReturnVoid))
      {
         fprintf(pcmw->cFile
                 , "\n\treturn retVal;\n"
                );
      }

      fprintf(pcmw->cFile, "}\n\n");

   }

   fprintf(pcmw->cFile, "\n");
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

static void writeOriginalSwitchFSMLoopInnards(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp, char *tabstr)
{

   FSMLANG_MAYBE_UNUSED(cp);

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

static void writeOriginalSwitchFSMLoop(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
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
   

   fprintf(pcmw->cFile, "#ifdef %s_DEBUG\n", cp);
   fprintf(pcmw->cFile
           , "if (EVENT_IS_NOT_EXCLUDED_FROM_LOG(%s))\n{\n"
           , (pmi->modFlags & mfActionsReturnVoid) ? "event" : "e"
           );
   if (short_dbg_names && add_machine_name)
   {
      fprintf(pcmw->cFile, "\tDBG_PRINTF(\"%s: event: %%s; state: %%s\"\n,%s_EVENT_NAMES[%s]\n,%s_STATE_NAMES[pfsm->state]\n);\n}\n"
              , pmi->name->name
              , cp
              , (pmi->modFlags & mfActionsReturnVoid) ? "event" : "e"
              , cp
             );
   }
   else
   {
      fprintf(pcmw->cFile, "\tDBG_PRINTF(\"event: %%s; state: %%s\"\n,%s_EVENT_NAMES[%s]\n,%s_STATE_NAMES[pfsm->state]\n);\n}\n"
              , cp
              , (pmi->modFlags & mfActionsReturnVoid) ? "event" : "e"
              , cp
             );
   }
   fprintf(pcmw->cFile, "#endif\n\n");

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

   writeOriginalSwitchFSMLoopInnards(pcmw,pmi,cp,tabstr);

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

static void writeOriginalSwitchSubFSMLoop(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{
   char *local_tabstr = "\t";

   if (!(pmi->modFlags & mfActionsReturnVoid))
   {
      fprintf(pcmw->cFile
              , "\twhile (\n\t\t(e != THIS(noEvent))\n\t\t&& (e >= THIS(%s))\n\t)\n\t{\n\n"
              , eventNameByIndex(pmi, 0)
              );
   }

   fprintf(pcmw->cFile, "#ifdef %s_DEBUG\n", cp);
   fprintf(pcmw->cFile
           , "if (EVENT_IS_NOT_EXCLUDED_FROM_LOG(%s))\n{\n"
           , (pmi->modFlags & mfActionsReturnVoid) ? "event" : "e"
           );
   if (short_dbg_names && add_machine_name)
   {
      fprintf(pcmw->cFile
              , "\tDBG_PRINTF(\"%s: event: %%s; state: %%s\"\n,%s_EVENT_NAMES[%s - THIS(%s)]\n,%s_STATE_NAMES[pfsm->state]\n);\n}\n"
              , pmi->name->name
              , cp
              , (pmi->modFlags & mfActionsReturnVoid) ? "event" : "e"
              , eventNameByIndex(pmi,0)
              , cp
             );
   }
   else
   {
      fprintf(pcmw->cFile
              , "\tDBG_PRINTF(\"event: %%s; state: %%s\"\n,%s_EVENT_NAMES[%s - THIS(%s)]\n,%s_STATE_NAMES[pfsm->state]\n);\n}\n"
              , cp
              , (pmi->modFlags & mfActionsReturnVoid) ? "event" : "e"
              , eventNameByIndex(pmi,0)
              , cp
             );
   }
   fprintf(pcmw->cFile, "#endif\n\n");

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

static void writeOriginalSwitchFSM(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{
   writeOriginalSwitchFSMLoop(pcmw, pmi, cp);
}

static void writeOriginalSwitchSubFSM(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{
   writeOriginalSwitchSubFSMLoop(pcmw, pmi, cp);
}

static void writeReentrantSwitchFSM(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{
   fprintf(pcmw->cFile, "#ifdef FSM_START_CRITICAL\n");
   fprintf(pcmw->cFile, "\tFSM_START_CRITICAL;\n");
   fprintf(pcmw->cFile, "#endif\n\n");

   writeOriginalSwitchFSMLoop(pcmw, pmi, cp);

   fprintf(pcmw->cFile, "\n\n#ifdef FSM_END_CRITICAL\n");
   fprintf(pcmw->cFile, "\tFSM_END_CRITICAL;\n");
   fprintf(pcmw->cFile, "#endif\n\n");

}

static void defineStateFnArray(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{
   fprintf(pcmw->cFile
           , "const %s_STATE_FN %s_state_fn_array[%s_numStates] = \n{\n"
           , cp
           , pmi->name->name
           , pmi->name->name
          );


   /* declare state fns */
   for (unsigned i = 0; i < pmi->state_list->count; i++)
   {
      fprintf(pcmw->cFile
              , "\t%s%s_%s_stateFn\n"
              , i ? ", " : "  "
              , pmi->name->name
              , stateNameByIndex(pmi, i)
             );
   }

   fprintf(pcmw->cFile, "};\n\n");
}

void cswitchHeaderEnd(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp, bool needNoOp)
{
   ITERATOR_CALLBACK_HELPER ich = { 0 };

   ich.pcmw      = pcmw;
   ich.pmi       = pmi;
   ich.cp        = cp;
   ich.needNoOp  = needNoOp;

   /* declare the action functions themselves */
   iterate_list(pmi->action_list, declare_action_function, &ich);

   /* declare the dummy, or no op action */
   if (needNoOp)
   {
      if (pmi->modFlags & mfActionsReturnVoid)
      {
         fprintf(pcmw->hFile
                 , "void %s_noAction(p%s);\n\n"
                 , pmi->name->name
                 , cp
                );
      }
      else
      {
         if (pmi->modFlags & mfActionsReturnStates)
         {
            fprintf(pcmw->hFile
                    , "%s_STATE %s_noAction(p%s);\n\n"
                    , cp
                    , pmi->name->name
                    , cp
                   );
         }
         else
         {
            fprintf(pcmw->hFile
                    , "%s_EVENT%s %s_noAction(p%s);\n\n"
                    , cp
                    , pmi->data_block_count ? "_ENUM"  : ""
                    , pmi->name->name
                    , cp
                   );
         }
      }
   }

   fprintf(pcmw->hFile
           , "\n"
          );

   /* declare any machine transition function */
   if (pmi->machineTransition)
   {
      fprintf(pcmw->hFile
              , "void %s_%s(p%s,%s_STATE);\n\n"
              , pmi->name->name
              , pmi->machineTransition->name
              , cp
              , cp
             );
   }

   /* declare any transition functions */
   if (pmi->modFlags & mfActionsReturnStates)
   {
      if (pmi->transition_fn_list->count)
      {
         fprintf(pcmw->hFile
                 , "%s_STATE %s_noTransitionFn(p%s);\n"
                 , cp
                 , pmi->name->name
                 , cp
                );

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

   /* if the machine has data, declare the data init function
 
     This is pre-mature.  We need to parse the data structure
       to know what parameters this function needs.
 
   if (pmi->data)
     fprintf(pcmw->hFile,"void %s_initData(p%s);\n",
       pmi->name->name,
       cp);
   */

}

void cswitchSubMachineHeaderEnd(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp, bool needNoOp)
{
   ITERATOR_CALLBACK_HELPER ich = { 0 };

   ich.pcmw      = pcmw;
   ich.pmi       = pmi;
   ich.cp        = cp;
   ich.parent_cp = hungarianToUnderbarCaps(pmi->parent->name->name);
   ich.needNoOp  = needNoOp;

   /* declare the action functions themselves */
   iterate_list(pmi->action_list, declare_sub_machine_action_function, &ich);

   /* declare the dummy, or no op action */
   if (needNoOp)
   {
      if (pmi->modFlags & mfActionsReturnVoid)
      {
         fprintf(pcmw->hFile
                 , "void %s_noAction(p%s);\n"
                 , pmi->name->name
                 , cp
                );
      }
      else
      {
         if (pmi->modFlags & mfActionsReturnStates)
         {
            fprintf(pcmw->hFile
                    , "%s_STATE %s_noAction(p%s);\n\n"
                    , cp
                    , pmi->name->name
                    , cp
                   );
         }
         else
         {
            fprintf(pcmw->hFile
                    , "%s_EVENT%s %s_noAction(p%s);\n\n"
                    , ich.parent_cp
                    , pmi->parent->data_block_count ? "_ENUM"  : ""
                    , pmi->name->name
                    , cp
                   );
         }
      }
   }

   fprintf(pcmw->hFile
           , "\n"
          );

   /* declare any machine transition function */
   if (pmi->machineTransition)
   {
      fprintf(pcmw->hFile
              , "void %s_%s(p%s,%s_STATE);\n\n"
              , pmi->name->name
              , pmi->machineTransition->name
              , cp
              , cp
             );
   }

   /* declare any transition functions */
   if (pmi->modFlags & mfActionsReturnStates)
   {
      if (pmi->transition_fn_list->count)
      {
         fprintf(pcmw->hFile
                 , "%s_STATE %s_noTransitionFn(p%s);\n"
                 , cp
                 , pmi->name->name
                 , cp
                );

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

   FREE_AND_CLEAR(ich.parent_cp);

   /* if the machine has data, declare the data init function
 
     This is pre-mature.  We need to parse the data structure
       to know what parameters this function needs.
 
   if (pmi->data)
     fprintf(pcmw->hFile,"void %s_initData(p%s);\n",
       pmi->name->name,
       cp);
   */

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

      fprintf(pich->pcmw->cFile
              , pich->pmi->parent 
                 ? "\t\tcase %s_%s_%s:\n"
                 : "\t\tcase %s%s_%s:\n"
              , pich->pmi->parent ? pich->pmi->parent->name->name : ""
              , pich->pmi->name->name
              , event->name
              );

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
            fprintf(pich->pcmw->cFile
                    ,"\t\t\tretVal = %s_noEvent;\n"
                    , pich->pmi->name->name
                    );
         }

      }

      fprintf(pich->pcmw->cFile
              , "\t\t\tbreak;\n"
              );

   }
   return false;
}

static void defineAllStateHandler(pCMachineData pcmd, pMACHINE_INFO pmi, char *cp)
{
   ITERATOR_CALLBACK_HELPER ich;

   ich.pcmw = pcmd;
   ich.pmi  = pmi;
   ich.cp   = cp;

   char *local_cp = hungarianToUnderbarCaps(pmi->name->name);

   if (pmi->data_block_count)
   {
      fprintf(pcmd->cFile
              , pmi->modFlags & ACTIONS_RETURN_FLAGS 
                  ? "static %s %s(p%s pfsm, %s_EVENT e)\n{\n"
                  : "static %s_EVENT_ENUM %s(p%s pfsm, %s_EVENT_ENUM e)\n{\n"
              , pmi->modFlags & ACTIONS_RETURN_FLAGS ? "bool" : cp
              , pmi->modFlags & ACTIONS_RETURN_FLAGS 
                 ? "eventIsNotHandledInAllStates" 
                 : "checkWhetherEventIsHandledInAllStates"
              , local_cp
              , cp
              );
   }
   else
   {
      fprintf(pcmd->cFile
              , pmi->modFlags & ACTIONS_RETURN_FLAGS 
                  ? "static %s %s(p%s pfsm, %s_EVENT e)\n{\n"
                  : "static %s_EVENT %s(p%s pfsm, %s_EVENT e)\n{\n"
              , pmi->modFlags & ACTIONS_RETURN_FLAGS ? "bool" : cp
              , pmi->modFlags & ACTIONS_RETURN_FLAGS 
                 ? "eventIsNotHandledInAllStates" 
                 : "checkWhetherEventIsHandledInAllStates"
              , local_cp
              , cp
              );
   }

   if (!(pmi->modFlags & ACTIONS_RETURN_FLAGS))
   {
      fprintf(pcmd->cFile
              , "\t%s_EVENT%s retVal;\n"
              , cp
              , pmi->data_block_count ? "_ENUM"  : ""
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
      fprintf(pcmd->cFile
              , "\t%s_STATE new_s = pfsm->state;\n\n"
              , local_cp
              );
   }

   fprintf(pcmd->cFile
           ,"\tswitch (e)\n\t{\n"
           );

   ich.counter = 0;
   iterate_list(pmi->event_list,print_switch_cases_for_events_handled_in_all_states,&ich);

   if (ich.counter == pmi->event_list->count)
   {
      printf("warning: (%s) all events are handled identically in all states.\n"
             , pmi->name->name
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

   if (pmi->machineTransition || pmi->states_with_entry_fns_count || pmi->states_with_exit_fns_count)
   {
      fprintf(pcmd->cFile
              , "\n\n\tif (pfsm->state != new_s)\n\t{\n"
             );

      if (pmi->machineTransition)
      {
         fprintf(pcmd->cFile
                 , "\t\t%s_%s(pfsm,new_s);\n"
                 , pmi->name->name
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

   CHECK_AND_FREE(local_cp);
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

