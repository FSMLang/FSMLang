/**
*  fsm_c_switch_common.c
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
#include "fsm_c_switch_common.h"
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
#define writeCFSMLoopInnards(A) pfsmcog->cfsmliw(pfsmcog, (A))

static bool            cswitch_sub_machine_declare_transition_fn_for_when_actions_return_events(pLIST_ELEMENT,void*);

void cswitchMachineHeaderEnd(pCMachineData pcmd, pMACHINE_INFO pmi, bool needNoOp)
{
   ITERATOR_CALLBACK_HELPER ich = { 0 };

   ich.pcmd      = pcmd;
   ich.ih.pmi    = pmi;
   ich.needNoOp  = needNoOp;

   /* declare the action functions themselves */
   iterate_list(pmi->action_list, declare_action_function, &ich);

   /* declare the dummy, or no op action */
   if (needNoOp)
   {
      print_action_function_declaration(pcmd
										, empty_cell_fn ? empty_cell_fn : "noAction"
										);
   }
   fprintf(pcmd->hFile, "\n");
 
   /* declare any machine transition function */
   if (pmi->machineTransition)
   {
      fprintf(pcmd->hFile
              , "void UFMN(%s)(p%s,%s);\n\n"
              , pmi->machineTransition->name
              , fsmType(pcmd)
              , stateType(pcmd)
              );
   }

   /* declare any transition functions */
   if (pmi->modFlags & mfActionsReturnStates)
   {
      if (pmi->transition_fn_list->count)
      {
         fprintf(pcmd->hFile
                 , "%s %s_noTransitionFn(p%s);\n"
                 , stateType(pcmd)
                 , machineName(pcmd)
                 , fsmType(pcmd)
                );

         iterate_list(pmi->transition_fn_list
                      , declare_transition_fn_for_when_actions_return_states
                      , &ich
                      );

         iterate_list(pmi->transition_list
                      , declare_state_only_transition_functions_for_when_actions_return_states
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
      iterate_list(pmi->state_list
                   , declare_state_entry_and_exit_functions
                   , &ich
                   );
      fprintf(pcmd->hFile, "\n");
   }

   /* declare needed data translators */
   if (pmi->data_block_count)
   {
      iterate_list(pmi->event_list, declare_data_translator_functions, &ich);
      fprintf(pcmd->hFile, "\n");
   }

   if (pmi->native_epilogue)
       print_native_epilogue(pcmd, pmi);

}

void writeOriginalSwitchFSMLoopAre(pFSMCOutputGenerator pfsmcog)
{
	pCMachineData pcmd = pfsmcog->pcmd;
    pMACHINE_INFO pmi  = pcmd->pmi;

	FSMLANG_DEVELOP_PRINTF(pcmd->cFile, "/* FSMLANG_DEVELOP: %s */\n", __func__);

   char *tabstr = "\t";

   if (pmi->data_block_count)
   {
      fprintf(pcmd->cFile
              , "\ttranslateEventData(&pfsm->data, event);\n\n"
              );
   }

   fprintf(pcmd->cFile
		   , "\twhile (e != %s_noEvent) {\n\n"
		   , pmi->name->name
		   );

   printFSMMachineDebugBlock(pcmd, pmi);

   fprintf(pcmd->cFile
           , "\t\t/* This is read-only data to facilitate error reporting in action functions */\n"
           );
   fprintf(pcmd->cFile
           , "\t\tpfsm->event = %s;\n\n"
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

   writeCFSMLoopInnards(tabstr);

   if (pmi->machine_list)
   {
      fprintf(pcmd->cFile
              , "\t\telse\n\t\t{\n"
              );
	  fprintf(pcmd->cFile
			  , "\t\t\t\te ="
			  );

      fprintf(pcmd->cFile
              , "%sfindAndRunSubMachine(pfsm, e)%s;"
			  , pmi->submachine_inhibitor_count ? " doNotInhibitSubMachines(pfsm->state) ? " : " "
			  , pmi->submachine_inhibitor_count ? " : THIS(noEvent)" : ""
              );
      fprintf(pcmd->cFile
              , "\n\t\t}\n\n\t}"
              );

   }

}

void writeOriginalSwitchFSMLoopArv(pFSMCOutputGenerator pfsmcog)
{
	pCMachineData pcmd = pfsmcog->pcmd;
	pMACHINE_INFO pmi  = pcmd->pmi;

	FSMLANG_DEVELOP_PRINTF(pcmd->cFile, "/* FSMLANG_DEVELOP: %s */\n", __func__);

   char *tabstr = "\t";

   if (pmi->data_block_count)
   {
      fprintf(pcmd->cFile
              , "\ttranslateEventData(&pfsm->data, event);\n\n"
              );
   }

   printFSMMachineDebugBlock(pcmd, pmi);

   fprintf(pcmd->cFile
           , "\t\t/* This is read-only data to facilitate error reporting in action functions */\n"
           );
   fprintf(pcmd->cFile
           , "\t\tpfsm->event = %s;\n\n"
		   , (pmi->data_block_count == 0) ? "event" : "e"
           );

   if (pmi->machine_list)
   {
      fprintf(pcmd->cFile
              , "\t\tif (%s < %s_noEvent)\n\t\t{\n"
			  , (pmi->data_block_count == 0) ? "event" : "e"
              , pmi->name->name
              );
      tabstr = "\t\t";
   }

   writeCFSMLoopInnards(tabstr);

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
              , "\t\t\tfindAndRunSubMachine(pfsm, e);"
              );
      fprintf(pcmd->cFile
              , "\n\t\t}\n\n\t}"
              );
   }

}

void cswitchSubMachineHeaderEnd(pCMachineData pcmd, pMACHINE_INFO pmi, bool needNoOp)
{
   ITERATOR_CALLBACK_HELPER ich = { 0 };
   ich.pcmd      = pcmd;
   ich.ih.pmi       = pmi;
   ich.needNoOp  = needNoOp;

   /* declare the action functions themselves */
   iterate_list(pmi->action_list, declare_action_function, &ich);

   /* declare the dummy, or no op action */
   if (needNoOp)
   {
      print_action_function_declaration(pcmd
										 , empty_cell_fn ? empty_cell_fn : "noAction"
										);
   }

   fprintf(pcmd->hFile
           , "\n"
          );

   /* declare any machine transition function */
   if (pmi->machineTransition)
   {
      fprintf(pcmd->hFile
              , "void UFMN(%s)(p%s,%s);\n\n"
              , pmi->machineTransition->name
              , fsmType(pcmd)
              , stateType(pcmd)
              );
   }

   /* declare any transition functions */
   if (pmi->modFlags & mfActionsReturnStates)
   {
      if (pmi->transition_fn_list->count)
      {
         print_transition_fn_declaration_for_when_actions_return_states(pcmd, pcmd->hFile, "noTransition");

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

   fprintf(pcmd->cFile, "\n");

   iterate_list(pmi->event_list
				, sub_machine_declare_data_translator_functions
				, &ich
				);

   /* declare any entry or exit functions */
   if (pmi->states_with_entry_fns_count || pmi->states_with_exit_fns_count)
   {
      iterate_list(pmi->state_list, declare_state_entry_and_exit_functions, &ich);
      fprintf(pcmd->hFile, "\n");
   }

   if (pmi->native_epilogue)
       print_native_epilogue(pcmd, pmi);

}

static bool cswitch_sub_machine_declare_transition_fn_for_when_actions_return_events(pLIST_ELEMENT pelem, void *data)
{
   pITERATOR_CALLBACK_HELPER pich = ((pITERATOR_CALLBACK_HELPER)data);
   pID_INFO pid_info              = ((pID_INFO)pelem->mbr);

   fprintf(pich->pcmd->hFile
		   , "%s UFMN(%s)(p%s,%s);\n"
		   , stateType(pich->pcmd)
		   , pid_info->name
		   , fsmType(pich->pcmd)
		   , eventType(pich->pcmd)
		   );

   return false;

}

void writeOriginalSwitchSubFSMLoopAre(pFSMCOutputGenerator pfsmcog)
{
	pCMachineData pcmd = pfsmcog->pcmd;
	pMACHINE_INFO pmi  = pcmd->pmi;

   fprintf(pcmd->cFile
           , "\twhile ((e != THIS(noEvent))\n\t       && (e >= THIS(firstEvent))\n"
           );

   if (pmi->machine_list)
   {
      fprintf(pcmd->cFile, "\n       && (e < THIS(lastEvent))");
   }

   fprintf(pcmd->cFile, "\t      )\n\t{\n\n");

   printFSMSubMachineDebugBlock(pcmd, pmi);

   fprintf(pcmd->cFile
           , "\t\t/* This is read-only data to facilitate error reporting in action functions */\n"
           );
   fprintf(pcmd->cFile
           , "\t\tpfsm->event = %s;\n\n"
           , (pmi->modFlags & mfActionsReturnVoid) ? "event" : "e"
           );

   writeCFSMLoopInnards("");

   if (pmi->machine_list)
   {
       fprintf(pcmd->cFile
               , "\n\t\tif ((e > THIS(noEvent))\n\t\t\t&& (e < THIS(lastEvent)))\n\t\t{\n\t\t\te ="
              );

       if (pmi->submachine_inhibitor_count)
       {
          fprintf(pcmd->cFile
                  , "\t\t\t(!doNotInhibitSubMachines(pfsm->state))\n\t\t\t\t? THIS(noEvent)\n\t\t\t\t:"
                  );
       }

       fprintf(pcmd->cFile
               , " findAndRunSubMachine(pfsm, e);\n\t\t}\n"
               );
   }

   fprintf(pcmd->cFile, "\n\t}\n");

   if (add_profiling_macros && profile_sub_fsms)
   {
	   fprintf(pcmd->cFile, "\n\tFSM_EXIT(pfsm);\n\n");
   }

   fprintf(pcmd->cFile
           , "\n\treturn e == THIS(noEvent) ? PARENT(noEvent) : e;"
           );

}

void writeOriginalSwitchSubFSMLoopArv(pFSMCOutputGenerator pfsmcog)
{
	pCMachineData pcmd = pfsmcog->pcmd;
	pMACHINE_INFO pmi  = pcmd->pmi;

   printFSMSubMachineDebugBlock(pcmd, pmi);

   fprintf(pcmd->cFile
           , "\t\t/* This is read-only data to facilitate error reporting in action functions */\n"
           );

   fprintf(pcmd->cFile
           , "\t\tpfsm->event = %s;\n\n"
           , (pmi->modFlags & mfActionsReturnVoid) ? "event" : "e"
           );

   writeCFSMLoopInnards("");

   if (pmi->machine_list)
   {
       fprintf(pcmd->cFile
               , "\n\t\tif ((e > THIS(noEvent))\n\t\t\t&& (e < THIS(lastEvent)))\n\t\t{\n\t\t\te ="
              );

	   fprintf(pcmd->cFile
			   , "%sfindAndRunSubMachine(pfsm, e)%s;\n\t\t}\n"
			   , pmi->submachine_inhibitor_count ? " doNotInhibitSubMachines(pfsm->state) ? " : " "
			   , pmi->submachine_inhibitor_count ? " : THIS(noEvent)" : ""
			   );
   }

   if (add_profiling_macros && profile_sub_fsms)
   {
	   fprintf(pcmd->cFile, "\n\tFSM_EXIT(pfsm);\n\n");
   }

   fprintf(pcmd->cFile, "\n\t}\n");

}

