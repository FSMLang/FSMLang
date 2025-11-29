/**
  fsm_c.c

	Creates C code to implement FSM


	FSMLang (fsm) - A Finite State Machine description language.
	Copyright (C) 2002  Steven Stanton

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

	Steven Stanton
	ringwinner@users.sourceforge.net

	For the latest on FSMLang: http://fsmlang.sourceforge.net

	And, finally, your possession of this source code implies nothing.

  Long Description:

  Creation: 	sstanton		Jan-21-2002
*/

#include "fsm_c.h"
#include "fsm_c_common.h"
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


/*
  Our interface to the outside world
*/
static void writeCMachine(pFSMOutputGenerator, pMACHINE_INFO);
static void writeCSubMachine(pFSMOutputGenerator, pMACHINE_INFO);

static void chooseWorkerFunctions(pFSMCOutputGenerator);
static int  writeCMachineInternal(pFSMCOutputGenerator);
static int  writeCSubMachineInternal(pFSMCOutputGenerator);
static void writeOriginalFSMAre(pFSMCOutputGenerator);
static void writeOriginalFSMArv(pFSMCOutputGenerator);
static void writeOriginalSubFSMAre(pFSMCOutputGenerator);
static void writeOriginalSubFSMArv(pFSMCOutputGenerator);
static void writeOriginalFSMLoop(pCMachineData, pMACHINE_INFO);
static void writeOriginalSubFSMLoop(pCMachineData, pMACHINE_INFO);
static void writeOriginalFSMLoopInnards(pCMachineData, pMACHINE_INFO, char *);
static void writeOriginalSubFSMLoopInnards(pCMachineData, pMACHINE_INFO, char *);
static void writeActionsReturnStateFSM(pFSMCOutputGenerator);
static void writeActionsReturnStateSubFSM(pFSMCOutputGenerator);
static void declareCMachineActionArray(pCMachineData, pMACHINE_INFO);
static void defineCMachineActionFnArray(pCMachineData, pMACHINE_INFO);
static void defineCMachineTransitionFnArray(pCMachineData, pMACHINE_INFO);
static void declareCMachineActionFnEnum(pCMachineData, pMACHINE_INFO);
static void declareCMachineTransitionFnEnum(pCMachineData, pMACHINE_INFO);
static void declareCMachineStruct(pCMachineData, pMACHINE_INFO);
static void defineActionArray(pCMachineData, pMACHINE_INFO);
static void defineCMachineFSM(pFSMCOutputGenerator);
static void defineCSubMachineFSM(pFSMCOutputGenerator);

#define writeFSMLoop(A) pfsmcog->wfsm((A))
#define writeCFSMLoopInnards(A) pfsmcog->cfsmliw(pfsmcog, (A))

FSMCOutputGenerator CMachineWriter = {
	.fsmog = {
		initCMachine
		, writeCMachine
		, closeCMachine
		, generateCMachineWriter
	}
	, .wfsm                   = NULL
	, .cfsmliw                = NULL
	, .wstate_chart           = NULL
	, .wconvenience_macros    = standardConvenienceMacros
	, .wtransition_fn_typedef = standardTransitionFnTypedef
	, .pcmd                   = NULL
	, .top_level_fsmcog       = NULL
	, .parent_fsmcog          = NULL
};

/* list iteration callbacks */

static bool declare_action_enum_member(pLIST_ELEMENT pelem, void *data)
{
	pITERATOR_CALLBACK_HELPER pich = ((pITERATOR_CALLBACK_HELPER)data);
	pID_INFO pid_info              = ((pID_INFO)pelem->mbr);

	FSMLANG_DEVELOP_PRINTF(pich->pcmd->hFile , "/* %s */\n", __func__ );

	if (pid_info->name && strlen(pid_info->name))
	{

		fprintf(pich->ih.fout
				, "%s%s_%s_e\n"
				, pich->ih.first ? (pich->ih.first = false, "\t") : "\t, "
				, fqMachineName(pich->pcmd)
				, pid_info->name
			   );

	}

	return false;
}

static bool declare_action_array_member(pLIST_ELEMENT pelem, void *data)
{
	pITERATOR_CALLBACK_HELPER pich = ((pITERATOR_CALLBACK_HELPER)data);
	pID_INFO pid_info              = ((pID_INFO)pelem->mbr);

	FSMLANG_DEVELOP_PRINTF(pich->pcmd->cFile , "/* %s */\n", __func__ );

	if (pid_info->name && strlen(pid_info->name))
	{

		fprintf(pich->pcmd->cFile
				, "\t%sUFMN(%s)\n"
				, pich->ih.first ? (pich->ih.first = false, "  ") : ", "
				, pid_info->name
			   );

	}

	return false;
}

static bool declare_transition_fn_enum_member(pLIST_ELEMENT pelem, void *data)
{
	pITERATOR_CALLBACK_HELPER pich = ((pITERATOR_CALLBACK_HELPER)data);
	pID_INFO pid_info              = ((pID_INFO)pelem->mbr);

	FSMLANG_DEVELOP_PRINTF(pich->ih.fout, "/* %s */\n", __func__ );

	fprintf(pich->ih.fout
			, "%s%s_%s_e\n"
			, pich->ih.first ? (pich->ih.first = false, "  ") : ", "
			, fqMachineName(pich->pcmd)
			, pid_info->name
		   );

	return false;
}

static bool declare_transition_enum_member(pLIST_ELEMENT pelem, void *data)
{
	pITERATOR_CALLBACK_HELPER pich = ((pITERATOR_CALLBACK_HELPER)data);
	pID_INFO pid_info              = ((pID_INFO)pelem->mbr);

	FSMLANG_DEVELOP_PRINTF(pich->ih.fout, "/* %s */\n", __func__ );

	fprintf(pich->ih.fout
			, "%s%s_transitionTo%s_e\n"
			, pich->ih.first ? (pich->ih.first = false, "  ") : ", "
			, fqMachineName(pich->pcmd)
			, pid_info->name
		   );

	return false;
}

static bool define_transition_fn_array_member(pLIST_ELEMENT pelem, void *data)
{
	pITERATOR_CALLBACK_HELPER pich = ((pITERATOR_CALLBACK_HELPER)data);
	pID_INFO pid_info              = ((pID_INFO)pelem->mbr);

	FSMLANG_DEVELOP_PRINTF(pich->pcmd->cFile , "/* %s */\n", __func__ );

	fprintf(pich->pcmd->cFile
			, "\t%sUFMN(%s)\n"
			, pich->ih.first ? (pich->ih.first = false, "  ") : ", "
			, pid_info->name
		   );

	return false;
}

static bool define_transition_array_member(pLIST_ELEMENT pelem, void *data)
{
	pITERATOR_CALLBACK_HELPER pich = ((pITERATOR_CALLBACK_HELPER)data);
	pID_INFO pid_info              = ((pID_INFO)pelem->mbr);

	FSMLANG_DEVELOP_PRINTF(pich->pcmd->cFile , "/* %s */\n", __func__ );

	fprintf(pich->pcmd->cFile
			, "\t%sUFMN(transitionTo%s)\n"
			, pich->ih.first ? (pich->ih.first = false, "  ") : ", "
			, pid_info->name
		   );

	return false;
}


static int writeCSubMachineInternal(pFSMCOutputGenerator pfsmcog)
{
	pCMachineData pcmd = pfsmcog->pcmd;
	pMACHINE_INFO pmi  = pfsmcog->pcmd->pmi;

	/* do this now, since some header stuff puts content into the source file.*/
	addNativeImplementationPrologIfThereIsAny(pmi, pcmd->cFile);

	subMachineHeaderStart(pfsmcog, "action", true);

	/* we need our count of events */
	fprintf(pcmd->hFile
			, "typedef enum { %s_numEvents = %u} "
			, fqMachineName(pcmd)
			, pmi->event_list->count
			);
	fprintf(pcmd->hFile
			, "%s_EVENTS;\n"
			, ucfqMachineName(pcmd)
			);

	declareCMachineActionArray(pcmd, pmi);

	declareCMachineStruct(pcmd, pmi);

	commonHeaderEnd(pfsmcog, true);

	/*
	  Source File
	*/

	if (pmi->machine_list)
	{
		declareSubMachineManagers(pcmd, pmi);
	}

	if (pmi->states_with_entry_fns_count || pmi->states_with_exit_fns_count)
	{
		declareStateEntryAndExitManagers(pcmd, pmi);
	}

	defineActionArray(pcmd, pmi);

	defineSubMachineIF(pcmd);

	possiblyDefineSubMachineSharedEventStructures(pcmd, pmi);

	defineSubMachineArray(pcmd, pmi);

	if (generate_instance)
	{
		generateInstance(pcmd, pmi, "actionArray", "action", true);
	}
	else
	{
		generateSubMachineInstanceMacro(pcmd, pmi, "actionArray", "action", true);
	}

	defineCSubMachineFSM(pfsmcog);

	/* write our sub-machine lookup, if needed */
	if (pmi->machine_list)
	{
		defineSubMachineFinder(pcmd, pmi);
	}

	defineStateEntryAndExitManagers(pcmd, pmi);

	if (generate_weak_fns)
	{
		/* write weak stubs for our action functions */
		defineWeakActionFunctionStubs(pcmd, pmi);

		/* ... and for the noAction case */
		defineWeakNoActionFunctionStubs(pcmd, pmi);

		/* write weak stubs for any data translators */
		defineSubMachineWeakDataTranslatorStubs(pcmd, pmi);

		/* write weak state entry and exit functions */
		defineWeakStateEntryAndExitFunctionStubs(pcmd, pmi);

		/* write our transition functions, if needed */
		if (pmi->transition_fn_list->count)
		{
			writeStateTransitions(pcmd, pmi);
			writeNoTransition(pcmd, pmi);
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

static int writeCMachineInternal(pFSMCOutputGenerator pfsmcog)
{
	pCMachineData pcmd = pfsmcog->pcmd;
	pMACHINE_INFO pmi  = pfsmcog->pcmd->pmi;

	/* do this now, since some header stuff puts content into the source file.*/
	addNativeImplementationPrologIfThereIsAny(pmi, pcmd->cFile);

	commonHeaderStart(pfsmcog, "action", true);

	declareCMachineActionArray(pcmd, pmi);

	declareCMachineStruct(pcmd, pmi);

	commonHeaderEnd(pfsmcog, true);

	/*
	  Source File
	*/

	if (pmi->machine_list)
	{
		declareSubMachineManagers(pcmd, pmi);
	}

	if (pmi->states_with_entry_fns_count || pmi->states_with_exit_fns_count)
	{
		declareStateEntryAndExitManagers(pcmd, pmi);
	}

	if (pmi->data_block_count)
	{
		declareEventDataManager(pcmd);
	}

	defineActionArray(pcmd, pmi);

	defineSubMachineArray(pcmd, pmi);

	if (generate_instance)
	{
		generateInstance(pcmd, pmi, "actionArray", "action", true);

		if (generate_run_function)
		{
			generateRunFunction(pcmd, pmi);
		}
	}
	else
	{
		generateInstanceMacro(pcmd, pmi, "actionArray", "action", true);
	}

	defineCMachineFSM(pfsmcog);

	/* write our sub-machine lookup, if needed */
	if (pmi->machine_list)
	{
		defineSubMachineFinder(pcmd, pmi);
	}

	defineStateEntryAndExitManagers(pcmd, pmi);

	if (pmi->data_block_count)
	{
		defineEventDataManager(pcmd, pmi);
	}

	if (generate_weak_fns)
	{
		/* write weak stubs for our action functions */
		defineWeakActionFunctionStubs(pcmd, pmi);

		/* ... and for the noAction case */
		defineWeakNoActionFunctionStubs(pcmd, pmi);

		/* ... don't forget any entry and exit functions */
		defineWeakStateEntryAndExitFunctionStubs(pcmd, pmi);

		/* write weak stubs for needed data translators */
		if (pmi->data_block_count)
		{
			defineWeakDataTranslatorStubs(pcmd, pmi);
		}

		/* write our transition functions, if needed */
		if (pmi->transition_fn_list->count)
		{
			writeStateTransitions(pcmd, pmi);
			writeNoTransition(pcmd, pmi);
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

static void writeCSubMachine(pFSMOutputGenerator pfsmog, pMACHINE_INFO pmi)
{

	pFSMCOutputGenerator pfsmcog = (pFSMCOutputGenerator)pfsmog;

	pfsmcog->pcmd->pmi = pmi;

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

	writeCSubMachineInternal(pfsmcog);

	if (pmi->machine_list)
	{
		write_machines(pmi->machine_list, generateCMachineWriter, pfsmog);
	}

}

static void writeCMachine(pFSMOutputGenerator pfsmog, pMACHINE_INFO pmi)
{

	pFSMCOutputGenerator pfsmcog = (pFSMCOutputGenerator)pfsmog;

	pfsmcog->pcmd->pmi = pmi;

	chooseWorkerFunctions(pfsmcog);

	writeCMachineInternal(pfsmcog);

	if (pmi->machine_list)
	{
		write_machines(pmi->machine_list, generateCMachineWriter, pfsmog);
	}

}


/**
  This function writes the original FSM when actions return
  events.
*/
static void writeOriginalFSMAre(pFSMCOutputGenerator pfsmcog)
{
	pCMachineData pcmd = pfsmcog->pcmd;
	pMACHINE_INFO pmi  = pfsmcog->pcmd->pmi;

	FSMLANG_DEVELOP_PRINTF(pcmd->cFile, "/* %s */\n", __func__);

 	fprintf(pcmd->cFile
 			, "\t%s new_e;\n"
 			, eventType(pcmd)
 			);

 	fprintf(pcmd->cFile
 			, "\t%s e = event%s;\n"
 			, eventType(pcmd)
 			, ultimateAncestor(pmi)->data_block_count ? "->event" : ""
 			);

	if (pmi->executes_fns_on_state_transitions)
	{
		fprintf(pcmd->cFile
				, "\t%s new_s;\n\n"
				, stateType(pcmd)
				);
	}

	writeOriginalFSMLoop(pcmd, pmi);

}

/**
  This function writes the original FSM when actions return
  void.
*/
static void writeOriginalFSMArv(pFSMCOutputGenerator pfsmcog)
{
	pCMachineData pcmd = pfsmcog->pcmd;
	pMACHINE_INFO pmi  = pfsmcog->pcmd->pmi;

	FSMLANG_DEVELOP_PRINTF(pcmd->cFile, "/* %s */\n", __func__);

	if (ultimateAncestor(pmi)->data_block_count)
	{
		fprintf(pcmd->cFile
				, "\t%s e = event->event;\n"
				, eventType(pcmd)
			   );
	}

	if (pmi->executes_fns_on_state_transitions)
	{
		fprintf(pcmd->cFile
				, "\t%s new_s;\n\n"
				, stateType(pcmd)
				);
	}

	writeOriginalFSMLoop(pcmd, pmi);

}

/**
  This function writes the original FSM when actions return
  events.
*/
static void writeOriginalSubFSMAre(pFSMCOutputGenerator pfsmcog)
{
	pCMachineData pcmd = pfsmcog->pcmd;
	pMACHINE_INFO pmi  = pfsmcog->pcmd->pmi;

	FSMLANG_DEVELOP_PRINTF(pcmd->cFile , "/* %s */\n", __func__ );

 	fprintf(pcmd->cFile
 			, "\t%s new_e;\n"
 			, eventType(pcmd)
 			);

 	fprintf(pcmd->cFile
 			, "\t%s e = event;\n"
 			, eventType(pcmd)
 			);

	if (pmi->executes_fns_on_state_transitions)
	{
		fprintf(pcmd->cFile
				, "\t%s new_s;\n"
				, stateType(pcmd)
				);
	}

	writeOriginalSubFSMLoop(pcmd, pmi);

	fprintf(pcmd->cFile
			, "\n\treturn e == THIS(noEvent) ? PARENT(noEvent) : e;"
		   );

}

/**
  This function writes the original FSM when actions return
  void.
*/
static void writeOriginalSubFSMArv(pFSMCOutputGenerator pfsmcog)
{
	pCMachineData pcmd = pfsmcog->pcmd;
	pMACHINE_INFO pmi  = pfsmcog->pcmd->pmi;

	FSMLANG_DEVELOP_PRINTF(pcmd->cFile , "/* %s */\n", __func__ );

	if (pmi->executes_fns_on_state_transitions)
	{
		fprintf(pcmd->cFile
				, "\t%s new_s;\n"
				, stateType(pcmd)
				);
	}

	writeOriginalSubFSMLoop(pcmd, pmi);

}

static void writeActionsReturnStateFSM(pFSMCOutputGenerator pfsmcog)
{
	pCMachineData pcmd = pfsmcog->pcmd;
	pMACHINE_INFO pmi  = pfsmcog->pcmd->pmi;

	FSMLANG_DEVELOP_PRINTF(pcmd->cFile , "/* %s */\n", __func__ );

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

	if (add_profiling_macros)
	{
		fprintf(pcmd->cFile, "\n\tFSM_ENTRY(pfsm);\n\n");
	}

	fprintf(pcmd->cFile
			, "\n#ifdef %s_DEBUG\n"
			, fsmType(pcmd)
			);
	fprintf(pcmd->cFile, "\n\tDBG_PRINTF(\"event: %%s; start state: %%s\"\n\t\t,");
	fprintf(pcmd->cFile
			, "%s_EVENT_NAMES[%s]\n\t\t,"
			, ucMachineName(pcmd)
			, pmi->data_block_count ? "e" : "event"
			);
	fprintf(pcmd->cFile
			, "%s_STATE_NAMES[pfsm->state]\n\t\t);\n"
			, ucMachineName(pcmd)
			);
	fprintf(pcmd->cFile
			, "\n#endif\n\n"
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
				, "\tif (e < THIS(numEvents))\n\t{"
				);
	}
	fprintf(pcmd->cFile
			, "\n%s\ts = (*(*pfsm->actionArray)[%s%s][pfsm->state])(pfsm);\n"
			, pmi->machine_list ? "\t" : ""
			, pmi->data_block_count ? "e" : "event"
			, pmi->parent ? " - THIS(firstEvent)" : ""
		   );
	if (pmi->machine_list)
	{
		fprintf(pcmd->cFile
				, "\t}\n\telse\n\t{\n\t\tfindAndRunSubMachine(pfsm,%s);\n\t}\n"
				, pmi->data_block_count ? "e" : "event"
				);
	}

	fprintf(pcmd->cFile
			, "\n\tif (s != STATE(noTransition))\n\t{\n"
			);

	if (pmi->executes_fns_on_state_transitions)
	{
		fprintf(pcmd->cFile
				, "\t\tif (s != pfsm->state)\n\t\t{\n"
			   );

		if (pmi->machineTransition)
		{
			fprintf(pcmd->cFile
					, "\t\t\tUFMN(%s)(pfsm,s);\n"
					, pmi->machineTransition->name
					);
		}

		if (pmi->states_with_exit_fns_count)
		{
			fprintf(pcmd->cFile
					, "\t\t\trunAppropriateExitFunction(%spfsm->state);\n"
					, pmi->data ? "&pfsm->data, " : ""
				   );
		}

		if (pmi->states_with_entry_fns_count)
		{
			fprintf(pcmd->cFile
					, "\t\t\trunAppropriateEntryFunction(%ss);\n"
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

	fprintf(pcmd->cFile
			, "\n#ifdef %s_DEBUG\n"
			, fsmType(pcmd)
			);
	fprintf(pcmd->cFile, "\n\tDBG_PRINTF(\"end state: %%s\"\n\t\t,");
	fprintf(pcmd->cFile
			, "%s_STATE_NAMES[pfsm->state]\n\t\t);\n"
			, ucMachineName(pcmd)
			);
	fprintf(pcmd->cFile
			, "\n#endif\n\n"
			);

	if (add_profiling_macros)
	{
		fprintf(pcmd->cFile, "\n\tFSM_EXIT(pfsm);\n\n");
	}

}

static void writeActionsReturnStateSubFSM(pFSMCOutputGenerator pfsmcog)
{
	pCMachineData pcmd    = pfsmcog->pcmd;
	pMACHINE_INFO pmi     = pfsmcog->pcmd->pmi;
	char          *tabstr = "\t";

	FSMLANG_DEVELOP_PRINTF(pcmd->cFile , "/* %s */\n", __func__ );

	fprintf(pcmd->cFile
			, "\t%s s = STATE(noTransition);\n"
			, stateType(pcmd)
			);

	if (add_profiling_macros)
	{
		fprintf(pcmd->cFile, "\n\tFSM_ENTRY(pfsm);\n\n");
	}

	fprintf(pcmd->cFile
			, "\n#ifdef %s_DEBUG\n"
			, fsmType(pcmd)
			);
	fprintf(pcmd->cFile, "\n\tDBG_PRINTF(\"event: %%s; start state: %%s\"\n\t\t,");
	fprintf(pcmd->cFile
			, "%s_EVENT_NAMES[event]\n\t\t,"
			, ucMachineName(pcmd)
			);
	fprintf(pcmd->cFile
			, "%s_STATE_NAMES[pfsm->state]\n\t\t);\n"
			, ucMachineName(pcmd)
			);
	fprintf(pcmd->cFile
			, "\n#endif\n\n"
			);

	if (pmi->machine_list)
	{
		fprintf(pcmd->cFile
				, "\tif (event < THIS(noEvent))\n\t{"
			   );
		tabstr = "\t\t";
	}
	fprintf(pcmd->cFile
			, "\n%ss = (*(*pfsm->actionArray)[event - THIS(firstEvent)][pfsm->state])(pfsm);\n"
			, tabstr
		   );
	fprintf(pcmd->cFile
			, "\n%sif (s != STATE(noTransition))\n\t{\n"
			, tabstr
			);

	if (pmi->executes_fns_on_state_transitions)
	{
		fprintf(pcmd->cFile
				, "%s\tif (s != pfsm->state)\n%s\t{\n"
				, tabstr
				, tabstr
			   );

		if (pmi->machineTransition)
		{
			fprintf(pcmd->cFile
					, "%s\t\tUFMN(%s)(pfsm,s);\n"
					, tabstr
					, pmi->machineTransition->name
					);
		}

		if (pmi->states_with_exit_fns_count)
		{
			fprintf(pcmd->cFile
					, "%s\t\trunAppropriateExitFunction(%spfsm->state);\n"
					, tabstr
					, pmi->data ? "&pfsm->data, " : ""
				   );
		}

		if (pmi->states_with_entry_fns_count)
		{
			fprintf(pcmd->cFile
					, "%s\t\trunAppropriateEntryFunction(%ss);\n"
					, tabstr
					, pmi->data ? "&pfsm->data, " : ""
				   );
		}

		fprintf(pcmd->cFile
				, "%s}\n"
				, tabstr
			   );

	}

	fprintf(pcmd->cFile
			, "%s\tpfsm->state = s;\n\t}\n\n"
			, tabstr
		   );

	if (pmi->machine_list)
	{
		fprintf(pcmd->cFile
				, "\t}\n\telse\n\t{\n\t\tfindAndRunSubMachine(pfsm,event);\n\t}\n"
				);
	}

	fprintf(pcmd->cFile
			, "\n#ifdef %s_DEBUG\n"
			, fsmType(pcmd)
			);
	fprintf(pcmd->cFile, "\n\tDBG_PRINTF(\"end state: %%s\"\n\t\t,");
	fprintf(pcmd->cFile
			, "%s_STATE_NAMES[pfsm->state]\n\t\t);\n"
			, ucMachineName(pcmd)
			);
	fprintf(pcmd->cFile
			, "\n#endif\n\n"
			);

	if (add_profiling_macros)
	{
		fprintf(pcmd->cFile, "\n\tFSM_EXIT(pfsm);\n\n");
	}

}

static void writeOriginalFSMLoopInnards(pCMachineData pcmd, pMACHINE_INFO pmi, char *tabstr)
{
	FSMLANG_DEVELOP_PRINTF(pcmd->cFile , "/* %s */\n", __func__ );

	if (!(pmi->modFlags & mfActionsReturnVoid))
	{
		if (add_profiling_macros)
		{
			fprintf(pcmd->cFile
					, "%s\tACTION_ENTRY(pfsm);\n\n"
					, tabstr
					);
		}

		if (compacting(pmi))
		{
			fprintf(pcmd->cFile
					, "%s\tnew_e = (*THIS(action_fns)[(*pfsm->actionArray)[e][pfsm->state].action])(pfsm);\n\n"
					, tabstr
				   );
		}
		else
		{
			fprintf(pcmd->cFile
					, "%s\tnew_e = ((* (*pfsm->actionArray)[e][pfsm->state].action)(pfsm));\n\n"
					, tabstr
				   );
		}

		if (add_profiling_macros)
		{
			fprintf(pcmd->cFile
					, "%s\tACTION_EXIT(pfsm);\n\n"
					, tabstr
				   );
		}

	}
	else
	{
		if (add_profiling_macros)
		{
			fprintf(pcmd->cFile
					, "%s\tACTION_ENTRY(pfsm);\n\n"
					, tabstr
				   );
		}

		if (compacting(pmi))
		{
			fprintf(pcmd->cFile
					, "%s(*THIS(action_fns)[(*pfsm->actionArray)[%s][pfsm->state].action])(pfsm);\n\n"
					, tabstr
					, (pmi->data_block_count == 0) ? "event" : "e"
				   );
		}
		else
		{
			fprintf(pcmd->cFile
					, "%s((* (*pfsm->actionArray)[%s][pfsm->state].action)(pfsm));\n\n"
					, tabstr
					, (pmi->data_block_count == 0) ? "event" : "e"
				   );
		}

		if (add_profiling_macros)
		{
			fprintf(pcmd->cFile
					, "%s\tACTION_EXIT(pfsm);\n\n"
					, tabstr
					);
		}

	}

	if (!pmi->transition_fn_list->count)
	{
		fprintf(pcmd->cFile
				, "%s\t%s = (*pfsm->actionArray)[%s][pfsm->state].transition;\n\n"
				, tabstr
				, pmi->executes_fns_on_state_transitions 
				? "new_s" : "pfsm->state"
				, ((pmi->modFlags & ACTIONS_RETURN_FLAGS) && (pmi->data_block_count == 0))
				  ? "event" 
				  : "e"
			   );
	}
	else
	{
		fprintf(pcmd->cFile
				, "%s\t%s = ("
				, tabstr
				, pmi->executes_fns_on_state_transitions
				? "new_s" : "pfsm->state"
			   );
		if (compacting(pmi))
		{
			fprintf(pcmd->cFile
					, "*THIS(transition_fns)[(*pfsm->actionArray)[%s][pfsm->state].transition])(pfsm,%s);\n\n"
					, ((pmi->modFlags & ACTIONS_RETURN_FLAGS) && (pmi->data_block_count == 0))
					  ? "event" 
					  : "e"
					, ((pmi->modFlags & ACTIONS_RETURN_FLAGS) && (pmi->data_block_count == 0))
					  ? "event" 
					  : "e"
					);
		}
		else
		{
			fprintf(pcmd->cFile
					, "(* (*pfsm->actionArray)[%s][pfsm->state].transition)(pfsm,%s));\n\n"
					, ((pmi->modFlags & ACTIONS_RETURN_FLAGS) && (pmi->data_block_count == 0))
					  ? "event" 
					  : "e"
					, ((pmi->modFlags & ACTIONS_RETURN_FLAGS) && (pmi->data_block_count == 0))
					  ? "event" 
					  : "e"
				   );
		}
	}

	if (pmi->executes_fns_on_state_transitions)
	{
		fprintf(pcmd->cFile
				, "\n%s\tif (pfsm->state != new_s)\n\t\t{\n"
				, tabstr
			   );
		FSMLANG_DEVELOP_PRINTF(pcmd->cFile, "/* if %s: %u */\n", __FILE__, __LINE__);

		if (pmi->machineTransition)
		{
			fprintf(pcmd->cFile
					, "%s\t\tUFMN(%s)(pfsm,new_s);\n"
					, tabstr
					, pmi->machineTransition->name
				   );
		}

		if (pmi->states_with_exit_fns_count)
		{
			fprintf(pcmd->cFile
					, "\t\trunAppropriateExitFunction(%spfsm->state);\n"
					, pmi->data ? "&pfsm->data, " : ""
				   );
		}

		if (pmi->states_with_entry_fns_count)
		{
			fprintf(pcmd->cFile
					, "\t\trunAppropriateEntryFunction(%snew_s);\n"
					, pmi->data ? "&pfsm->data, " : ""
				   );
		}

		fprintf(pcmd->cFile
				, "%s\t\tpfsm->state = new_s;\n\n"
				, tabstr
			   );

		fprintf(pcmd->cFile
				, "%s\t}\n\n"
				, tabstr
			   );
		FSMLANG_DEVELOP_PRINTF(pcmd->cFile, "/* end if %s: %u */\n", __FILE__, __LINE__);
	}

	if (!(pmi->modFlags & mfActionsReturnVoid))
	{
		fprintf(pcmd->cFile
				, "%s\te = new_e;\n\n"
				, tabstr
			   );
	}

}

static void writeOriginalSubFSMLoopInnards(pCMachineData pcmd, pMACHINE_INFO pmi, char *tabstr)
{
	FSMLANG_DEVELOP_PRINTF(pcmd->cFile , "/* %s */\n", __func__ );

	if (add_profiling_macros)
	{
		fprintf(pcmd->cFile
				, "\tACTION_ENTRY(pfsm);\n"
			   );
	}

	if (!(pmi->modFlags & mfActionsReturnVoid))
	{
		if (compacting(pmi))
		{
			fprintf(pcmd->cFile
					, "%s\tnew_e = (*THIS(action_fns)[(*pfsm->actionArray)[e - THIS(%s)][pfsm->state].action])(pfsm);\n\n"
					, tabstr
					, eventNameByIndex(pmi, 0)
				   );
		}
		else
		{
			fprintf(pcmd->cFile
					, "%s\tnew_e = ((* (*pfsm->actionArray)[e - THIS(%s)][pfsm->state].action)(pfsm));\n\n"
					, tabstr
					, eventNameByIndex(pmi, 0)
				   );
		}
	}
	else
	{
		if (compacting(pmi))
		{
			fprintf(pcmd->cFile
					, "%s(*THIS(action_fns)[(*pfsm->actionArray)[%s - THIS(%s)][pfsm->state].action])(pfsm);\n\n"
					, tabstr
					, ((pmi->modFlags & ACTIONS_RETURN_FLAGS) && (pmi->data_block_count == 0))
					  ? "event" 
					  : "e"
					, eventNameByIndex(pmi, 0)
				   );
		}
		else
		{
			fprintf(pcmd->cFile
					, "%s((* (*pfsm->actionArray)[%s - THIS(%s)][pfsm->state].action)(pfsm));\n\n"
					, tabstr
					, ((pmi->modFlags & ACTIONS_RETURN_FLAGS) && (pmi->data_block_count == 0))
					  ? "event" 
					  : "e"
					, eventNameByIndex(pmi, 0)
				   );
		}
	}

	if (add_profiling_macros)
	{
		fprintf(pcmd->cFile
				, "\tACTION_EXIT(pfsm);\n"
			   );
	}

	if (!pmi->transition_fn_list->count)
	{
		fprintf(pcmd->cFile
				, "%s\t%s = (*pfsm->actionArray)[%s - THIS(%s)][pfsm->state].transition;\n\n"
				, tabstr
				, pmi->executes_fns_on_state_transitions
				  ? "new_s" : "pfsm->state"
				, (pmi->modFlags & ACTIONS_RETURN_FLAGS) ? "event" : "e"
				, eventNameByIndex(pmi, 0)
			   );
	}
	else
	{
		if (compacting(pmi))
		{
			fprintf(pcmd->cFile
					, "%s\t%s = (*THIS(transition_fns)[(*pfsm->actionArray)[%s - THIS(%s)][pfsm->state].transition])(pfsm,%s);\n\n"
					, tabstr
					, pmi->executes_fns_on_state_transitions
					  ? "new_s" : "pfsm->state"
					, (pmi->modFlags & ACTIONS_RETURN_FLAGS) ? "event" : "e"
					, eventNameByIndex(pmi, 0)
					, (pmi->modFlags & ACTIONS_RETURN_FLAGS) ? "event" : "e"
				   );
		}
		else
		{
			fprintf(pcmd->cFile
					, "%s\t%s = ((* (*pfsm->actionArray)[%s - THIS(%s)][pfsm->state].transition)(pfsm,%s));\n\n"
					, tabstr
					, pmi->executes_fns_on_state_transitions
					  ? "new_s" : "pfsm->state"
					, (pmi->modFlags & ACTIONS_RETURN_FLAGS) ? "event" : "e"
					, eventNameByIndex(pmi, 0)
					, (pmi->modFlags & ACTIONS_RETURN_FLAGS) ? "event" : "e"
				   );
		}
	}

	if (pmi->executes_fns_on_state_transitions)
	{
		fprintf(pcmd->cFile
				, "\n%s\tif (pfsm->state != new_s)\n\t\t{\n"
				, tabstr
			   );

		if (pmi->machineTransition)
		{
			fprintf(pcmd->cFile
					, "%s\t\tUFMN(%s)(pfsm,new_s);\n"
					, tabstr
					, pmi->machineTransition->name
				   );
		}

		if (pmi->states_with_exit_fns_count)
		{
			fprintf(pcmd->cFile
					, "\t\trunAppropriateExitFunction(%spfsm->state);\n"
					, pmi->data ? "&pfsm->data, " : ""
				   );
		}

		if (pmi->states_with_entry_fns_count)
		{
			fprintf(pcmd->cFile
					, "\t\trunAppropriateEntryFunction(%snew_s);\n"
					, pmi->data ? "&pfsm->data, " : ""
				   );
		}

		fprintf(pcmd->cFile
				, "%s\t\tpfsm->state = new_s;\n\n"
				, tabstr
			   );

		fprintf(pcmd->cFile
				, "%s\t}\n\n"
				, tabstr
			   );
	}

	if (!(pmi->modFlags & mfActionsReturnVoid))
	{
		fprintf(pcmd->cFile
				, "%s\te = new_e;\n\n"
				, tabstr
			   );

	}
}

static void writeOriginalFSMLoop(pCMachineData pcmd, pMACHINE_INFO pmi)
{
	FSMLANG_DEVELOP_PRINTF(pcmd->cFile , "/* %s */\n", __func__ );

	char *tabstr = "\t";

	if (add_profiling_macros)
	{
		fprintf(pcmd->cFile, "\n\tFSM_ENTRY(pfsm);\n\n");
	}

	if (pmi->data_block_count)
	{
		fprintf(pcmd->cFile
				, "\ttranslateEventData(&pfsm->data, event);\n\n"
			   );
	}

	if (!(pmi->modFlags & mfActionsReturnVoid))
	{
		fprintf(pcmd->cFile
				, "\twhile (e != THIS(noEvent)) {\n\n"
				);
		FSMLANG_DEVELOP_PRINTF(pcmd->cFile, "/* while %s: %u */\n", __FILE__, __LINE__);
	}

	printFSMMachineDebugBlock(pcmd, pmi);

	fprintf(pcmd->cFile
			, "\t/* This is read-only data to facilitate error reporting in action functions */\n"
		   );

	fprintf(pcmd->cFile
			, "\t\tpfsm->event = %s;\n\n"
			, ((pmi->modFlags & ACTIONS_RETURN_FLAGS) && (pmi->data_block_count == 0))
			  ? "event" 
			  : "e"
		   );

	if (pmi->machine_list)
	{
		fprintf(pcmd->cFile
				, "\t\tif (e < THIS(%s))\n\t\t{\n\n"
				, pmi->modFlags & ACTIONS_RETURN_FLAGS ? "numEvents" : "noEvent"
			   );
		FSMLANG_DEVELOP_PRINTF(pcmd->cFile, "/* if %s: %u */\n", __FILE__, __LINE__);
		tabstr = "\t\t";
	}

	writeOriginalFSMLoopInnards(pcmd, pmi, tabstr);

	if (pmi->machine_list)
	{
		fprintf(pcmd->cFile
				, "\n\t\t}\n\t\telse\n\t\t{\n"
			   );
		FSMLANG_DEVELOP_PRINTF(pcmd->cFile, "/* %s: %u */\n", __FILE__, __LINE__);

		if (pmi->submachine_inhibitor_count)
		{
			fprintf(pcmd->cFile
					, "\t\t\tif (doNotInhibitSubMachines(pfsm->state))\n\t"
				   );
		}
		fprintf(pcmd->cFile
				, "\t\t\t%sfindAndRunSubMachine(pfsm, e);\n\t\t}\n"
				, pmi->modFlags & ACTIONS_RETURN_FLAGS ? "" : "e = "
			   );
		FSMLANG_DEVELOP_PRINTF(pcmd->cFile, "/* %s: %u */\n", __FILE__, __LINE__);
	}

	if (!(pmi->modFlags & mfActionsReturnVoid))
	{
		fprintf(pcmd->cFile
				, "\n\t} "
			   );
		FSMLANG_DEVELOP_PRINTF(pcmd->cFile, "/* end while %s: %u */\n", __FILE__, __LINE__);
	}

	if (add_profiling_macros)
	{
		fprintf(pcmd->cFile, "\n\tFSM_EXIT(pfsm);\n\n");
	}

}

static void writeOriginalSubFSMLoop(pCMachineData pcmd, pMACHINE_INFO pmi)
{
	FSMLANG_DEVELOP_PRINTF(pcmd->cFile , "/* %s */\n", __func__ );

	char *tabstr = "\t";

	if (add_profiling_macros && profile_sub_fsms)
	{
		fprintf(pcmd->cFile, "\n\tFSM_ENTRY(pfsm);\n\n");
	}

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

	if (pmi->machine_list)
	{
		fprintf(pcmd->cFile
				, "\t\tif (%s < THIS(noEvent))\n\t\t{\n\n"
				, pmi->modFlags & mfActionsReturnVoid ? "event" : "e"
			   );
		tabstr = "\t\t";
	}

	writeOriginalSubFSMLoopInnards(pcmd, pmi, tabstr);

	if (pmi->machine_list)
	{
		fprintf(pcmd->cFile
				, "\n\t\t}\n\t\telse\n\t\t{\n"
			   );

		if (pmi->submachine_inhibitor_count)
		{
			fprintf(pcmd->cFile
					, "\t\t\tif (doNotInhibitSubMachines(pfsm->state))\n\t"
				   );
		}
		fprintf(pcmd->cFile
				, "\t\t\t%sfindAndRunSubMachine(pfsm, %s);\n\t\t}\n"
				, pmi->modFlags & mfActionsReturnVoid ? "" : "e = "
				, pmi->modFlags & mfActionsReturnVoid ? "event" : "e"
			   );
	}

	if (add_profiling_macros && profile_sub_fsms)
	{
		fprintf(pcmd->cFile, "\n\tFSM_EXIT(pfsm);\n\n");
	}

	if (!(pmi->modFlags & mfActionsReturnVoid))
	{
		fprintf(pcmd->cFile, "\n\t}\n");
	}
}

static void declareCMachineActionArray(pCMachineData pcmd, pMACHINE_INFO pmi)
{

	FSMLANG_DEVELOP_PRINTF(pcmd->hFile , "/* %s */\n", __func__ );

	FILE *fout = generate_instance ? pcmd->hFile : pcmd->pubHFile;

	/*
	  Actions which return events or void have state transitions
		stored in a struct with the function pointer.
  
	  Actions which return states do not.
	*/
	if (!(pmi->modFlags & mfActionsReturnStates))
	{

		/* build the structure for the action array */

		/* 
		   if compacting, we will use an array for the action functions,
		   and the transition functions, should they exist
		 */
		if (compacting(pmi))
		{
			declareCMachineActionFnEnum(pcmd, pmi);
			if (pmi->transition_fn_list->count)
			{
				declareCMachineTransitionFnEnum(pcmd, pmi);
			}
		}

		/* now do the action/transition array */
		fprintf(fout
				, "typedef struct _%s_action_trans_struct_ {\n\t"
				, fqMachineName(pcmd)
				);

		fprintf(fout
				, "%s%s\taction;\n\t"
				, actionFnType(pcmd)
				, compacting(pmi) ? "_E" : ""
			   );

		fprintf(fout
				, "%s%s\ttransition;\n} "
				, pmi->transition_fn_list->count 
				  ? transitionFnType(pcmd)
				  : stateType(pcmd)
				, (pmi->transition_fn_list->count && compacting(pmi)) ? "_E" : ""
			   );
		fprintf(fout
				, "%s, *p%s;\n\n"
				, actionTransType(pcmd)
				, actionTransType(pcmd)
				);

	}

	/* publish the array, if not generating a machine instance */
	if (!generate_instance)
	{
		char *cp;
		fprintf(pcmd->pubHFile
				, "extern const %s %s_action_array[%s_numEvents][%s];\n\n"
				, pmi->modFlags & mfActionsReturnStates
				  ? actionFnType(pcmd) 
				  : actionTransType(pcmd)
				, generate_instance ? machineName(pcmd) : fqMachineName(pcmd)
				, fqMachineName(pcmd)
				, stateEnumMemberPmi("numStates", pcmd->pmi, &cp)
				);
		CHECK_AND_FREE(cp);
	}
}

static void declareCMachineActionFnEnum(pCMachineData pcmd, pMACHINE_INFO pmi)
{
	FSMLANG_DEVELOP_PRINTF(pcmd->hFile , "/* %s */\n", __func__ );

	FILE *fout = generate_instance ? pcmd->hFile : pcmd->pubHFile;

	ITERATOR_CALLBACK_HELPER ich = { 0 };

	ich.ih.first = true;
	ich.pcmd     = pcmd;
	ich.ih.pmi   = pmi;
	ich.ih.fout  = fout;

	/* enum */
	fprintf(fout
			, "typedef enum\n{\n"
		   );

	iterate_list(pmi->action_list, declare_action_enum_member, &ich);

	if (empty_cell_fn)
	{
		fprintf(fout
				, "\t, %s_%s_e\n"
				, fqMachineName(pcmd)
				, empty_cell_fn
				);
	}

	/* declare the dummy, or no op action */
	fprintf(fout, "\t, THIS(noAction_e)\n");

	fprintf(fout, "} __attribute__((__packed__)) ");
	fprintf(fout
			, "%s_E;\n\n"
			, actionFnType(pcmd)
			);

}

static void defineCMachineActionFnArray(pCMachineData pcmd, pMACHINE_INFO pmi)
{
	FSMLANG_DEVELOP_PRINTF(pcmd->cFile , "/* %s */\n", __func__ );

	ITERATOR_CALLBACK_HELPER ich = { 0 };

	ich.ih.first = true;
	ich.pcmd  = pcmd;
	ich.ih.pmi   = pmi;


	/* open the array */
	fprintf(pcmd->cFile
			, "%sconst %s THIS(action_fns)[] =\n{\n"
			, generate_instance ? "static " : ""
			, actionFnType(pcmd)
			);

	/* fill the array */
	iterate_list(pmi->action_list,  declare_action_array_member, &ich);

	if (empty_cell_fn)
	{
		fprintf(pcmd->cFile
				, "\t, UFMN(%s)\n"
				, empty_cell_fn
				);
	}

	/* declare the dummy, or no op action and close the array */
	fprintf(pcmd->cFile
			, "\t, UFMN(noAction)\n};\n\n"
			);

}

static void declareCMachineTransitionFnEnum(pCMachineData pcmd, pMACHINE_INFO pmi)
{
	FSMLANG_DEVELOP_PRINTF(pcmd->hFile , "/* %s */\n", __func__ );

	FILE *fout = generate_instance ? pcmd->hFile : pcmd->pubHFile;

	ITERATOR_CALLBACK_HELPER ich = { 0 };

	ich.ih.first = true;
	ich.ih.fout  = fout;
	ich.pcmd     = pcmd;
	ich.ih.pmi   = pmi;

	/* enum */
	fprintf(fout
			, "typedef enum\n{\n"
		   );

	iterate_list(pmi->transition_fn_list
				 , declare_transition_fn_enum_member
				 , &ich
				 );
	iterate_list(pmi->transition_list
				 , declare_transition_enum_member
				 , &ich
				 );

	fprintf(fout
			, ", %s_noTransition_e\n"
			, fqMachineName(pcmd)
			);

	fprintf(fout, "} __attribute__ (( __packed__ )) ");
	fprintf(fout
			, "%s_E;\n\n"
			, transitionFnType(pcmd)
			);

}

static void defineCMachineTransitionFnArray(pCMachineData pcmd, pMACHINE_INFO pmi)
{
	FSMLANG_DEVELOP_PRINTF(pcmd->cFile , "/* %s */\n", __func__ );

	ITERATOR_CALLBACK_HELPER ich = { 0 };

	ich.ih.first = true;
	ich.pcmd  = pcmd;
	ich.ih.pmi   = pmi;


	/* open the array */
	fprintf(pcmd->cFile
			, "%sconst %s UFMN(transition_fns)[] =\n{\n"
			, generate_instance ? "static " : ""
			, transitionFnType(pcmd)
			);

	/* fill the array */
	iterate_list(pmi->transition_fn_list
				 , define_transition_fn_array_member
				 , &ich
				);
	iterate_list(pmi->transition_list
				 , define_transition_array_member
				 , &ich
				);

	fprintf(pcmd->cFile
			, "\t, UFMN(noTransitionFn)\n"
			);

	fprintf(pcmd->cFile
			, "};\n\n"
		   );

}


static void declareCMachineStruct(pCMachineData pcmd, pMACHINE_INFO pmi)
{
	FSMLANG_DEVELOP_PRINTF(pcmd->hFile , "/* %s */\n", __func__ );

	FILE *fout = generate_instance ? pcmd->hFile : pcmd->pubHFile;
	char *cp;

	/* put the machine structure definition into the header */
	fprintf(fout
			, "struct _%s_struct_ {\n"
			, generate_instance ? machineName(pcmd) : fqMachineName(pcmd)
			);

	fprintf(fout
			, "\t%-*s instance;\n"
			, (int) pcmd->c_machine_struct_format_width
			, "unsigned"
			);

	if (pmi->data)
	{
		fprintf(fout
				, "\t%-*s data;\n"
				, (int) pcmd->c_machine_struct_format_width
				, fsmDataType(pcmd)
				);
	}

	fprintf(fout
			, "\t%-*s state;\n"
			, (int) pcmd->c_machine_struct_format_width
			, stateType(pcmd)
			);

	fprintf(fout
			, "\t%-*s event;\n"
			, (int) pcmd->c_machine_struct_format_width
			, eventType(pcmd)
			);

	fprintf(fout
			, "\t%-*s const\t(*actionArray)[%s_numEvents][%s];\n"
			, (int) pcmd->c_machine_struct_format_width
			, (pmi->modFlags & mfActionsReturnStates)
			  ? actionFnType(pcmd)
			  : actionTransType(pcmd)
			, fqMachineName(pcmd)
			, stateEnumMemberPmi("numStates", pcmd->pmi, &cp)
		   );
	CHECK_AND_FREE(cp);

	if (pmi->machine_list)
	{
		fprintf(fout
				, "\tp%-*s const\t(*subMachineArray)[%s_numSubMachines];\n"
				, (int) pcmd->c_machine_struct_format_width
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
			, "\t%-*sfsm;\n};\n\n"
			, (int) pcmd->c_machine_struct_format_width
			, fsmFnType(pcmd)
			);

}


static void defineActionArray(pCMachineData pcmd, pMACHINE_INFO pmi)
{
	FSMLANG_DEVELOP_PRINTF(pcmd->cFile , "/* %s */\n", __func__ );

	char *cp;

	/* 
	   if compacting, we will use an array for the action functions,
	   and the transition functions, should they exist
	 */
	if (compacting(pmi))
	{
		defineCMachineActionFnArray(pcmd, pmi);
		if (pmi->transition_fn_list->count)
		{
			defineCMachineTransitionFnArray(pcmd, pmi);
		}
	}

	fprintf(pcmd->cFile
			, "%sconst %s %s_action_array[THIS(numEvents)][%s] =\n{\n"
			, generate_instance ? "static " : ""
			, (pmi->modFlags & mfActionsReturnStates) 
			  ? actionFnType(pcmd)
			  : actionTransType(pcmd)
			, generate_instance ? machineName(pcmd) : fqMachineName(pcmd)
			, stateEnumMemberPmi("numStates", pcmd->pmi, &cp)
			);
	CHECK_AND_FREE(cp);

	for (unsigned i = 0; i < pmi->event_list->count; i++)
	{

		fprintf(pcmd->cFile, "\t{\n");

		fprintf(pcmd->cFile, "\t\t/* -- %s -- */\n\n",
				eventNameByIndex(pmi, i));

		for (unsigned j = 0; j < pmi->state_list->count; j++)
		{

			fprintf(pcmd->cFile, "\t\t/* -- %s -- */\t",
					stateNameByIndex(pmi, j)
				   );

			if (j) fprintf(pcmd->cFile, ", ");

			if (pmi->actionArray[i][j])
			{

				if (pmi->modFlags & mfActionsReturnStates)
				{
					if (strlen(pmi->actionArray[i][j]->action->name))
					{
						fprintf(pcmd->cFile, "UFMN(%s)\n",
								pmi->actionArray[i][j]->action->name);
					}
					else
					{
						fprintf(pcmd->cFile
								, (pmi->actionArray[i][j]->transition->type == STATE)
									? "UFMN(transitionTo%s)\n"
									: "UFMN(%s)\n"
								, pmi->actionArray[i][j]->transition->name
							   );
						CHECK_AND_FREE(cp);
					}
				}
				else //if (pmi->modFlags & mfActionsReturnStates)
				{

					fprintf(pcmd->cFile, "{");

					/* also handle the transition only case */
					fprintf(pcmd->cFile
							, "%s(%s%s),"
							, compacting(pmi) ? "THIS" : "UFMN"
							, strlen(pmi->actionArray[i][j]->action->name)
							  ? pmi->actionArray[i][j]->action->name
							  : "noAction"
							, compacting(pmi) ? "_e" : ""
						   );


					if (pmi->transition_fn_list->count)
					{
						if (pmi->actionArray[i][j]->transition)
						{
							fprintf(pcmd->cFile
									, "%s(%s%s%s)"
									, compacting(pmi) ? "THIS" : "UFMN"
									, pmi->actionArray[i][j]->transition->type == STATE
									  ? "transitionTo"
									  : ""
									, pmi->actionArray[i][j]->transition->name
									, compacting(pmi) ? "_e" : ""
								   );
						}
						else
						{
							fprintf(pcmd->cFile
									, "%s(noTransition%s)"
									, compacting(pmi) ? "THIS" : "UFMN"
									, compacting(pmi)
									  ? "_e"
									  : pmi->transition_fn_list->count
									    ? "Fn"
									    : ""
								   );
						}
					}
					else // if (pmi->transition_fn_list->count)
					{
						if (pmi->actionArray[i][j]->transition)
						{
							fprintf(pcmd->cFile
									, "%s"
									, stateEnumMemberPmi(pmi->actionArray[i][j]->transition->name, pmi, &cp)
									);
						}
						else
						{
							fprintf(pcmd->cFile
									, "%s"
									, stateEnumMemberPmi(stateNameByIndex(pmi, j), pmi, &cp)
									);
						}
						CHECK_AND_FREE(cp);
					}

					fprintf(pcmd->cFile, "}\n");

				}

			}
			else // if (pmi->actionArray[i][j])
			{

				/* we need to insert a dummy here */
				if (pmi->modFlags & mfActionsReturnStates)
				{

					fprintf(pcmd->cFile
							, "%s(noTransition%s)\n"
							, compacting(pmi) ? "THIS" : "UFMN"
							, compacting(pmi)
							  ? "_e"
							  : pmi->transition_fn_list->count
							    ? "Fn"
							    : ""
						   );

				}
				else
				{

					fprintf(pcmd->cFile, "{");

					fprintf(pcmd->cFile
							, "%s(%s%s), "
							, compacting(pmi) ? "THIS" : "UFMN"
							, empty_cell_fn ? empty_cell_fn : "noAction"
							, compacting(pmi) ? "_e" : ""
						   );

					if (pmi->transition_fn_list->count == 0)
					{
						fprintf(pcmd->cFile
								, "%s"
								, stateEnumMemberPmi(stateNameByIndex(pmi, j), pmi, &cp)
								);
					}
					else
					{
						fprintf(pcmd->cFile
								, "%s(noTransition%s)"
								, compacting(pmi) ? "THIS" : "UFMN"
								, compacting(pmi) ? "_e" : "Fn"
							   );
					}

					fprintf(pcmd->cFile, "}\n");

				}

			}

		}
		fprintf(pcmd->cFile, "\t},\n");

	}
	fprintf(pcmd->cFile, "};\n");

}

static void defineCMachineFSM(pFSMCOutputGenerator pfsmcog)
{
	pCMachineData pcmd = pfsmcog->pcmd;

	FSMLANG_DEVELOP_PRINTF(pcmd->cFile , "/* %s */\n", __func__ );

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
			, "void %sFSM(p%s pfsm, %s event)\n{\n"
			, generate_instance ? machineName(pcmd) : fqMachineName(pcmd)
			, fsmType(pcmd)
			, fsmFnEventType(pcmd)
			);
	FSMLANG_DEVELOP_PRINTF(pcmd->cFile, "/* begin func %s: %u */\n", __FILE__, __LINE__);

	writeReentrantPrologue(pcmd);
	writeFSMLoop(pfsmcog);
	writeReentrantEpilogue(pcmd);

	fprintf(pcmd->cFile, "\n\n}\n\n");
	FSMLANG_DEVELOP_PRINTF(pcmd->cFile, "/* end func %s: %u */\n", __FILE__, __LINE__);
}

static void defineCSubMachineFSM(pFSMCOutputGenerator pfsmcog)
{
	pCMachineData pcmd = pfsmcog->pcmd;
	pMACHINE_INFO pmi  = pfsmcog->pcmd->pmi;

	FSMLANG_DEVELOP_PRINTF(pcmd->cFile , "/* %s */\n", __func__ );

	if (pmi->machine_list)
	{
		declareSubMachineManagers(pcmd, pmi);
	}

	if (pmi->states_with_entry_fns_count || pmi->states_with_exit_fns_count)
	{
		declareStateEntryAndExitManagers(pcmd, pmi);
	}

	fprintf(pcmd->cFile
			, "#ifndef EVENT_IS_NOT_EXCLUDED_FROM_LOG\n"
		   );

	fprintf(pcmd->cFile
			, "#define EVENT_IS_NOT_EXCLUDED_FROM_LOG(e) (e == e)\n"
		   );

	fprintf(pcmd->cFile
			, "#endif\n"
		   );

	fprintf(pcmd->cFile
			, "%s%s %sFSM(p%s pfsm, %s event)\n{\n"
			, generate_instance ? "static " : ""
			, subFsmFnReturnType(pcmd)
			, generate_instance ? machineName(pcmd) : fqMachineName(pcmd)
			, fsmType(pcmd)
			, fsmFnEventType(pcmd)
		   );

	writeFSMLoop(pfsmcog);

	fprintf(pcmd->cFile, "\n\n}\n\n");

}

pFSMOutputGenerator generateCMachineWriter(pFSMOutputGenerator parent)
{
	pFSMOutputGenerator pfsmog;

	if (parent)
	{
		pFSMCOutputGenerator pfsmcsmog;

		pfsmcsmog = (pFSMCOutputGenerator)calloc(1, sizeof(FSMCOutputGenerator));

		pfsmcsmog->fsmog.writeMachine = writeCSubMachine;
		pfsmcsmog->fsmog.initOutput   = initCSubMachine;
		pfsmcsmog->fsmog.closeOutput  = closeCMachine;
		pfsmcsmog->fsmog.fsmogFactory = generateCMachineWriter;

		pfsmcsmog->wconvenience_macros    = standardConvenienceMacros;
		pfsmcsmog->wtransition_fn_typedef = standardTransitionFnTypedef;
		pfsmcsmog->top_level_fsmcog       = &CMachineWriter;
		pfsmcsmog->parent_fsmcog          = (pFSMCOutputGenerator)parent;

		pfsmog =  (pFSMOutputGenerator)pfsmcsmog;
	}
	else
	{
		pfsmog =  (pFSMOutputGenerator)&CMachineWriter;
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
			               ? writeOriginalSubFSMAre
			               : writeOriginalFSMAre
			               ;
		break;

	case mfActionsReturnStates:
		pfsmcog->wfsm = pfsmcog->pcmd->pmi->parent
			            ? writeActionsReturnStateSubFSM
			            : writeActionsReturnStateFSM
			            ;
		break;

	case mfActionsReturnVoid:
		pfsmcog->wfsm = pfsmcog->pcmd->pmi->parent
			               ? writeOriginalSubFSMArv
			               : writeOriginalFSMArv
			               ;
		break;
	}
}

#ifdef FSM_C_TEST

	#include "y.tab.h"

ID_INFO id_info8;
ID_INFO id_info7;
ID_INFO id_info6;
ID_INFO id_info5;
ID_INFO id_info4;
ID_INFO id_info3;
ID_INFO id_info2;
ID_INFO id_info1;

ACTION_SE_INFO a_se_info11 = {
	&id_info6,				NULL
};

ACTION_SE_INFO a_se_info10 = {
	&id_info5,				&a_se_info11
};

ACTION_SE_INFO a_se_info9 = {
	&id_info4,				NULL
};

ACTION_SE_INFO a_se_info8 = {
	&id_info8,				NULL
};

ACTION_SE_INFO a_se_info7 = {
	&id_info7,				&a_se_info8
};

ACTION_SE_INFO a_se_info6 = {
	&id_info4,				NULL
};

ACTION_SE_INFO a_se_info5 = {
	&id_info3,				&a_se_info6
};

ACTION_SE_INFO a_se_info4 = {
	&id_info6,				NULL
};

ACTION_SE_INFO a_se_info3 = {
	&id_info3,				NULL
};

ACTION_SE_INFO a_se_info2 = {
	&id_info2,				&a_se_info3
};

ACTION_SE_INFO a_se_info1 = {
	&id_info5,				NULL
};

ACTION_SE_INFO a_se_info0 = {
	&id_info1,				NULL
};

ACTION_INFO	action_info3 = {
	&a_se_info9,				&a_se_info10,				NULL
};

ACTION_INFO	action_info2 = {
	&a_se_info5,				&a_se_info7,				&id_info1
};

ACTION_INFO	action_info1 = {
	&a_se_info2,				&a_se_info4,				&id_info2
};

ACTION_INFO	action_info0 = {
	&a_se_info0,				&a_se_info1,				NULL
};

ID_INFO id_info12 = {
	"a4",					ACTION,		NULL,				NULL,				NULL,				NULL,					&action_info3,			0
};

ID_INFO id_info11 = {
	"a3",					ACTION,		NULL,				NULL,				NULL,				&id_info12,		&action_info2,			0
};

ID_INFO id_info10 = {
	"a2",					ACTION,		NULL,				NULL,				NULL,				&id_info11,		&action_info1,			0
};

ID_INFO id_info9 = {
	"a1",					ACTION,		NULL,				NULL,				NULL,				&id_info10,		&action_info0,			0
};

ID_INFO id_info8 = {
	"e4",					EVENT,		NULL,				NULL,				NULL,				NULL,					NULL,								3
};

ID_INFO id_info7 = {
	"e3",					EVENT,		&id_info8,	NULL,				&id_info8,	NULL,					NULL,								2
};

ID_INFO id_info6 = {
	"e2",					EVENT,		&id_info7,	NULL,				&id_info7,	NULL,					NULL,								1
};

ID_INFO id_info5 = {
	"e1",					EVENT,		&id_info6,	NULL,				&id_info6,	NULL,					NULL,								0
};

ID_INFO id_info4 = {
	"s4",					STATE,		&id_info5,	NULL,				NULL,				NULL,					NULL,								3
};

ID_INFO id_info3 = {
	"s3",					STATE,		&id_info4,	&id_info4,	NULL,				NULL,					NULL,								2
};

ID_INFO id_info2 = {
	"s2",					STATE,		&id_info3,	&id_info3,	NULL,				NULL,					NULL,								1
};

ID_INFO id_info1 = {
	"s1",					STATE,		&id_info2,	&id_info2,	NULL,				NULL,					NULL,								0
};

ID_INFO id_info0 = {
	"newMachine",	MACHINE,	&id_info1,	NULL,				NULL,				NULL,					NULL,								0
};

MACHINE_INFO machine = {
	&id_info1, 4, &id_info5, 4, &id_info9, &id_info0
};

int main(int argc, char **argv)
{

	pCMachineData	pcmd = newCMachineData("foo");

	if (!populateActionArray(&machine)) writeCMachine(pcmd, &machine);

/*
  id_info0.name = "anotherMachine";

  writeCMachine(pcmd,&machine);
*/
	destroyCMachineData(pcmd, 1);

	return (0);

}
#endif

