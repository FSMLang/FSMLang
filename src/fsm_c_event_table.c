/**
*  fsm_c_event_table.c
*
*    Creates an event-table based machine
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
*    File created by :Steven Stanton
*
*  Long Description:
*
*/

#include "fsm_c_event_table.h"

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

#include "fsm_c_common.h"
#include "fsm_c_switch_common.h"
#include "ancestry.h"

typedef struct _cevent_table_machine_data_     CEventTableMachineData
                                               , *pCEventTableMachineData
                                               ;

static void writeCEventTableMachine(pFSMOutputGenerator,pMACHINE_INFO); 
static void writeCEventTableSubMachine(pFSMOutputGenerator,pMACHINE_INFO); 
static void writeCEventTableMachineInternal(pFSMCOutputGenerator);
static void declareCEventTableMachineEventTable(pCMachineData);
static void declareCEventTableMachineEventTableSize(pCMachineData);
static void defineCEventTableMachineStruct(pCMachineData);
static void defineEventFnArray(pCMachineData);
static void defineCEventTableHandlers(pFSMCOutputGenerator);
static void defineCEventTableMachineFSM(pFSMCOutputGenerator);
static void defineCEventTableSubMachineFSM(pFSMCOutputGenerator);
static void writeOriginalEventTableFSMAre(pFSMCOutputGenerator);
static void writeOriginalEventTableFSMArv(pFSMCOutputGenerator);
static void writeOriginalEventTableSubFSMAre(pFSMCOutputGenerator);
static void writeOriginalEventTableSubFSMArv(pFSMCOutputGenerator);
static void writeActionsReturnStateEventTableFSM(pFSMCOutputGenerator);
static void writeActionsReturnStateEventTableSubFSM(pFSMCOutputGenerator);
static void writeEventTableFSMLoopInnards(pFSMCOutputGenerator,char*);
static void writeEventTableSubFSMLoopInnards(pFSMCOutputGenerator,char*);
static int  writeCEventTableSubMachineInternal(pFSMCOutputGenerator);
static void print_event_table_handler_body_for_single_state_or_pai_events_are(FILE*,pID_INFO,pACTION_INFO,pCMachineData);
static void print_event_table_handler_body_for_single_state_or_pai_events_arv(FILE*,pID_INFO,pACTION_INFO,pCMachineData);
static void print_event_table_handler_body_for_single_state_or_pai_events_ars(FILE*,pID_INFO,pACTION_INFO,pCMachineData);
static void print_event_table_handler_body_for_multiple_state_events_are(FILE*,pEVENT_DATA,pITERATOR_CALLBACK_HELPER);
static void print_event_table_handler_body_for_multiple_state_events_arv(FILE*,pEVENT_DATA,pITERATOR_CALLBACK_HELPER);
static void print_event_table_handler_body_for_multiple_state_events_ars(FILE*,pEVENT_DATA,pITERATOR_CALLBACK_HELPER);
static void chooseWorkerFunctions(pFSMCSwitchOutputGenerator);
static void print_any_transition_handlers(FILE*,pMACHINE_INFO);

static bool event_handler_cannot_be_its_action(pID_INFO,pACTION_INFO*);
static bool print_event_fn_signature(pLIST_ELEMENT,void*);
static bool print_event_handler_name(pLIST_ELEMENT,void*);
static bool define_event_table_handler(pLIST_ELEMENT,void*);
static bool print_event_table_handler_state_case_are(pLIST_ELEMENT,void*);
static bool print_event_table_handler_state_case_arv(pLIST_ELEMENT,void*);
static bool print_event_table_handler_state_case_ars(pLIST_ELEMENT,void*);
static bool find_transition(pLIST_ELEMENT,void*);

#define writeFSMLoop(A) pfsmcog->wfsm((A))

#define print_event_table_handler_body_for_single_state_or_pai_events(A,B,C,D) \
((pFSMCSwitchOutputGenerator)pfsmcog)->pethbsspe((A),(B),(C),(D))

#define print_event_table_handler_body_for_multiple_state_events(A,B,C) \
((pFSMCSwitchOutputGenerator)pfsmcog)->pethbmse((A),(B),(C))

#define print_event_table_handler_state_case ((pFSMCSwitchOutputGenerator)pfsmcog)->pethsc

static FSMCSwitchOutputGenerator CEventTableMachineWriter = {
	{
	   {
	      initCMachine
	      , writeCEventTableMachine
	      , closeCMachine
		  , generateCEventTableMachineWriter
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

static void chooseWorkerFunctions(pFSMCSwitchOutputGenerator pfsmcswog)
{
	switch ((pfsmcswog->fsmcog.pcmd->pmi->modFlags & ACTIONS_RETURN_FLAGS))
	{
	case 0: //this is "actions return events"
		pfsmcswog->fsmcog.wfsm = pfsmcswog->fsmcog.pcmd->pmi->parent
			                 ? writeOriginalEventTableSubFSMAre
			                 : writeOriginalEventTableFSMAre
			                 ;
		pfsmcswog->pethbsspe = print_event_table_handler_body_for_single_state_or_pai_events_are;
		pfsmcswog->pethbmse  = print_event_table_handler_body_for_multiple_state_events_are; 
		pfsmcswog->pethsc    = print_event_table_handler_state_case_are;
		break;
	case mfActionsReturnVoid:
		pfsmcswog->fsmcog.wfsm      = pfsmcswog->fsmcog.pcmd->pmi->parent
			                           ? writeOriginalEventTableSubFSMArv
			                           : writeOriginalEventTableFSMArv
			                           ;
		pfsmcswog->pethbsspe = print_event_table_handler_body_for_single_state_or_pai_events_arv;
		pfsmcswog->pethbmse  = print_event_table_handler_body_for_multiple_state_events_arv; 
		pfsmcswog->pethsc    = print_event_table_handler_state_case_arv;
		break;
	case mfActionsReturnStates:
		pfsmcswog->fsmcog.wfsm      = pfsmcswog->fsmcog.pcmd->pmi->parent
			                          ? writeActionsReturnStateEventTableSubFSM
			                          : writeActionsReturnStateEventTableFSM
			                          ;
		pfsmcswog->pethbsspe = print_event_table_handler_body_for_single_state_or_pai_events_ars;
		pfsmcswog->pethbmse  = print_event_table_handler_body_for_multiple_state_events_ars; 
		pfsmcswog->pethsc    = print_event_table_handler_state_case_ars;
		break;
	default:
		yyerror("invalid actions return statement");
		break;
	}
}
static void writeCEventTableMachine(pFSMOutputGenerator pfsmog, pMACHINE_INFO pmi) 
{
	pFSMCSwitchOutputGenerator pfsmcswog = (pFSMCSwitchOutputGenerator) pfsmog;

	pfsmcswog->fsmcog.pcmd->pmi = pmi;
	pfsmcswog->fsmcog.cfsmliw   = writeEventTableFSMLoopInnards;
	chooseWorkerFunctions(pfsmcswog);

	writeCEventTableMachineInternal(&(pfsmcswog->fsmcog));

	if (pmi->machine_list)
	{
	   write_machines(pmi->machine_list, generateCEventTableMachineWriter, pfsmog);
	}
}

static int writeCEventTableSubMachineInternal(pFSMCOutputGenerator pfsmcog)
{
   if (!pfsmcog)   return 1;

   pCMachineData pcmd = pfsmcog->pcmd;
   pMACHINE_INFO pmi  = pcmd->pmi;

   ITERATOR_CALLBACK_HELPER ich = {
	   . ih = {
		   .pmi      = pmi
	   }
	   , .pcmd    = pcmd
       , .define  = false
	   , .pfsmcog = pfsmcog
   };

   /* do this now, since some header stuff puts content into the source file.*/
   addNativeImplementationPrologIfThereIsAny(pmi, pcmd->cFile);

   subMachineHeaderStart(pcmd, pmi, "event_fn", false);

   declareCEventTableMachineEventTable(pcmd);

   defineCEventTableMachineStruct(pcmd);

   cswitchSubMachineHeaderEnd(pcmd, pmi, empty_cell_fn != NULL);

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

   /* Declare needed event functions. */
   ich.define = false;
   iterate_list(pmi->event_list
				, print_event_fn_signature
				, &ich
				);

   defineEventFnArray(pcmd);

   defineSubMachineIF(pcmd);

   possiblyDefineSubMachineSharedEventStructures(pcmd, pmi);

   defineSubMachineArray(pcmd, pmi);

   if (generate_instance)
   {
      generateInstance(pcmd, pmi, "eventsArray", "event_fn");
   }
   else
   {
	   generateSubMachineInstanceMacro(pcmd, pmi, "eventsArray", "event_fn");
   }

   defineCEventTableSubMachineFSM(pfsmcog);

   /* write our sub-machine lookup, if needed */
   if (pmi->machine_list)
   {
	  defineSubMachineFinder(pcmd, pmi);
   }

   defineStateEntryAndExitManagers(pcmd, pmi);

   defineCEventTableHandlers(pfsmcog);

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

static void writeCEventTableSubMachine(pFSMOutputGenerator pfsmog, pMACHINE_INFO pmi) 
{

	pFSMCSwitchOutputGenerator pfsmcswog = (pFSMCSwitchOutputGenerator) pfsmog;

	pfsmcswog->fsmcog.pcmd->pmi = pmi;
	if (!(pmi->modFlags & mfActionsReturnStates))
	{
		pfsmcswog->fsmcog.pcmd->action_return_type = pfsmcswog->fsmcog.parent_fsmcog->pcmd->action_return_type;
	}
	pfsmcswog->fsmcog.pcmd->fsm_fn_event_type       = pfsmcswog->fsmcog.parent_fsmcog->pcmd->event_type;
	pfsmcswog->fsmcog.pcmd->sub_fsm_fn_return_type  = pfsmcswog->fsmcog.parent_fsmcog->pcmd->sub_fsm_fn_return_type;
	pfsmcswog->fsmcog.pcmd->event_type              = pfsmcswog->fsmcog.parent_fsmcog->pcmd->event_type;
	pfsmcswog->fsmcog.pcmd->instance_type           = pfsmcswog->fsmcog.parent_fsmcog->pcmd->instance_type;
	pfsmcswog->fsmcog.pcmd->sub_fsm_fn_event_type   = pfsmcswog->fsmcog.parent_fsmcog->pcmd->sub_fsm_fn_event_type;

	pfsmcswog->fsmcog.cfsmliw = writeEventTableSubFSMLoopInnards;

	chooseWorkerFunctions(pfsmcswog);
	writeCEventTableSubMachineInternal(&pfsmcswog->fsmcog);

	if (pmi->machine_list)
	{
	   write_machines(pmi->machine_list, generateCEventTableMachineWriter, pfsmog);
	}
}

pFSMOutputGenerator generateCEventTableMachineWriter(pFSMOutputGenerator parent)
{
	pFSMOutputGenerator pfsmog;

	if (parent)
	{
		pFSMCSwitchOutputGenerator pfsmceventtableog = calloc(1, sizeof(FSMCSwitchOutputGenerator));

		pfsmceventtableog->fsmcog.fsmog.writeMachine = writeCEventTableSubMachine;
		pfsmceventtableog->fsmcog.fsmog.initOutput   = initCSubMachine;
		pfsmceventtableog->fsmcog.fsmog.closeOutput  = closeCMachine;
		pfsmceventtableog->fsmcog.fsmog.fsmogFactory = generateCEventTableMachineWriter;

		pfsmceventtableog->fsmcog.top_level_fsmcog = (pFSMCOutputGenerator)&CEventTableMachineWriter;
		pfsmceventtableog->fsmcog.parent_fsmcog    = (pFSMCOutputGenerator) parent;

		pfsmog = (pFSMOutputGenerator)pfsmceventtableog;
	}
	else
	{
		pfsmog = (pFSMOutputGenerator)&CEventTableMachineWriter;
	}

	if (output_generated_file_names_only)
	{
		pfsmog->initOutput   = parent ? initCSubMachineFN : initCMachineFN;
		pfsmog->writeMachine = writeCMachineFN;
		pfsmog->closeOutput  = closeCMachineFN;
	}

	return pfsmog;

}

static void writeCEventTableMachineInternal(pFSMCOutputGenerator pfsmcog)
{
	pMACHINE_INFO pmi  = pfsmcog->pcmd->pmi;
	pCMachineData pcmd = pfsmcog->pcmd;

	ITERATOR_CALLBACK_HELPER ich = {
		.ih = {
			.pmi = pfsmcog->pcmd->pmi
		}
		, .pcmd = pfsmcog->pcmd
	};

	/* do this now, since some header stuff puts content into the source file.*/
	addNativeImplementationPrologIfThereIsAny(pmi, pcmd->cFile);

	commonHeaderStart(pcmd, pmi, "event_fn", false);

	declareCEventTableMachineEventTable(pcmd);

	defineCEventTableMachineStruct(pcmd);

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

	if (pmi->states_with_entry_fns_count || pmi->states_with_exit_fns_count)
	{
		declareStateEntryAndExitManagers(pcmd, pmi);
	}

	/* Declare needed event functions. */
	ich.define = false;
	iterate_list(pmi->event_list
				 , print_event_fn_signature
				 , &ich
				 );

	defineEventFnArray(pcmd);

	defineSubMachineArray(pcmd, pmi);

	if (generate_instance)
	{
		generateInstance(pcmd, pmi, "eventsArray", "event_fn");

		if (generate_run_function)
		{
		   generateRunFunction(pcmd, pmi);
		}

	}
	else
	{
		generateInstanceMacro(pcmd, pmi, "eventsArray", "event_fn");
	}

	defineCEventTableMachineFSM(pfsmcog);

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

	defineCEventTableHandlers(pfsmcog);

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

	fprintf(pcmd->cFile, "\n");

}

static void declareCEventTableMachineEventTableSize(pCMachineData pcmd)
{
	FILE *fout = generate_instance ? pcmd->hFile : pcmd->pubHFile;

	FSMLANG_DEVELOP_PRINTF(fout, "/* FSMLANG_DEVELOP: %s */\n", __func__);

	fprintf(fout
			, "#define %s_numMachineEvents %u\n"
			, machineName(pcmd)
			, pcmd->pmi->event_list->count
			);

}

static void declareCEventTableMachineEventTable(pCMachineData pcmd)
{
	FSMLANG_DEVELOP_PRINTF(pcmd->hFile, "/* FSMLANG_DEVELOP: %s */\n", __func__);

	declareCEventTableMachineEventTableSize(pcmd);

	if (!generate_instance)
	{
		fprintf(pcmd->pubHFile
				, "extern const %s %s_event_fn_array[%s_numMachineEvents];\n"
				, actionFnType(pcmd)
				, machineName(pcmd)
				, machineName(pcmd)
				);
	}

}


static void defineCEventTableMachineStruct(pCMachineData pcmd)
{
	pMACHINE_INFO pmi = pcmd->pmi;

	FILE *fout = generate_instance ? pcmd->hFile : pcmd->pubHFile;

	FSMLANG_DEVELOP_PRINTF(fout, "/* FSMLANG_DEVELOP: %s */\n", __func__);

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
		   , subFsmFnEventType(pcmd)
		  );

   fprintf(fout
		   , "\t%-*s const (*eventsArray)[%s_numMachineEvents];\n"
		   , (int)pcmd->sub_machine_struct_format_width
		   , actionFnType(pcmd)
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

static bool print_event_fn_signature(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO                  pevent = (pID_INFO)                  pelem->mbr;
	pITERATOR_CALLBACK_HELPER pich   = (pITERATOR_CALLBACK_HELPER) data;

	if (event_handler_cannot_be_its_action(pevent, NULL))
	{
		fprintf(pich->pcmd->cFile
				, "static ACTION_RETURN_TYPE %s_handle_%s(FSM_TYPE_PTR%s)%s\n"
				, machineName(pich->pcmd)
				, pevent->name
				, pich->define ? " pfsm" : ""
				, pich->define ? "\n{"   : ";"
			   );
	}

	return false;
}

static void defineEventFnArray(pCMachineData pcmd)
{
	ITERATOR_CALLBACK_HELPER ich = {
		.pcmd = pcmd
	};

	fprintf(pcmd->cFile
			, "\n%sconst %s %s_event_fn_array[%s_numMachineEvents] =\n{\n"
			, generate_instance ? "static " : ""
			, actionFnType(pcmd)
			, machineName(pcmd)
			, machineName(pcmd)
			);

	ich.ih.first = true;
	iterate_list(pcmd->pmi->event_list
				 , print_event_handler_name
				 , &ich
				 );

	fprintf(pcmd->cFile, "};\n\n");
}

static bool print_event_handler_name(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO                  pevent = (pID_INFO)                  pelem->mbr;
	pITERATOR_CALLBACK_HELPER pich   = (pITERATOR_CALLBACK_HELPER) data;
	pACTION_INFO              pai;

	if (event_handler_cannot_be_its_action(pevent, &pai))
	{
		fprintf(pich->pcmd->cFile
				, "\t%s%s_handle_%s\n"
				, pich->ih.first ? (pich->ih.first = false, "") : ", "
				, machineName(pich->pcmd)
				, pevent->name
			   );
	}
	else
	{
		fprintf(pich->pcmd->cFile
				, "\t%sUFMN(%s)\n"
				, pich->ih.first ? (pich->ih.first = false, "") : ", "
				, pai->action->name
			   );
	}

	return false;
}

static bool event_handler_cannot_be_its_action(pID_INFO pevent, pACTION_INFO *ppai)
{
	bool         event_handler_can_be_its_action = false;
	pACTION_INFO pai                             = NULL;
	pEVENT_DATA  ped                             = &pevent->type_data.event_data;

	/* An event can be handled by its action function
	   if there is a single pai for that event in all states
	   and that pai has no transition.
	*/
	pai = ped->psingle_pai;

	event_handler_can_be_its_action =
		ped->single_pai_for_all_states
		&& (ped->psingle_pai->transition == NULL);

	if (ppai)
	{
		(*ppai) = pai;
	}

	return add_profiling_macros || !event_handler_can_be_its_action;
}

static void defineCEventTableHandlers(pFSMCOutputGenerator pfsmcog)
{
	ITERATOR_CALLBACK_HELPER ich = {
		.ih = {
			.pmi = pfsmcog->pcmd->pmi
		}
		, .pcmd    = pfsmcog->pcmd
		, .pfsmcog = pfsmcog
		, .define  = true
	};

	iterate_list(pfsmcog->pcmd->pmi->event_list
				 , define_event_table_handler
				 , &ich
				 );
}

static bool define_event_table_handler(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO                  pevent  = (pID_INFO)                  pelem->mbr;
	pITERATOR_CALLBACK_HELPER pich    = (pITERATOR_CALLBACK_HELPER) data;

	pEVENT_DATA               ped     = &pevent->type_data.event_data;
	pMACHINE_INFO             pmi     = pich->ih.pmi;
	FILE                     *fout    = pich->pcmd->cFile;
	pFSMCOutputGenerator      pfsmcog = pich->pfsmcog;

	pACTION_INFO              pai;

	if (event_handler_cannot_be_its_action(pevent, &pai))
	{
		(void)print_event_fn_signature(pelem, pich);

		if ( ped->single_pai_for_all_states ) {

			if (
				pmi->executes_fns_on_state_transitions
				&& !(pmi->modFlags & mfActionsReturnStates)
				&& (!pai || pai->transition)
				)
			{
				fprintf(fout
						, "\t%s s = pfsm->state;\n"
						, stateType(pich->pcmd)
					   );
			}

			print_event_table_handler_body_for_single_state_or_pai_events(fout, pevent, pai, pich->pcmd);

		}
		else
		{
			pich->pOtherElem = pelem;

			if (
				pmi->executes_fns_on_state_transitions
				&& iterate_list(ped->phandling_states, find_transition, pich)
				&& !(pmi->modFlags & mfActionsReturnStates)
				)
			{
				fprintf(fout
						, "\t%s s = pfsm->state;\n"
						, stateType(pich->pcmd)
					   );
			}

			print_event_table_handler_body_for_multiple_state_events(fout, ped, pich);
		}

		fprintf(fout, "}\n\n");

	}

	return false;
}

static void print_event_table_handler_body_for_single_state_or_pai_events_are(FILE *fout
				, pID_INFO pevent
				, pACTION_INFO pai
				, pCMachineData pcmd
				)
{
	pMACHINE_INFO pmi     = pcmd->pmi;
	bool          have_an_action = (pai->action->name && strlen(pai->action->name));

	if (have_an_action)
	{
		if (add_profiling_macros)
		{
			fprintf(fout
					, "\tACTION_ENTRY(pfsm);\n"
				   );
		}

		fprintf(fout
				, "\tACTION_RETURN_TYPE event = UFMN(%s)(pfsm);\n"
				, pai->action->name
				);

		if (add_profiling_macros)
		{
			fprintf(fout
					, "\tACTION_EXIT(pfsm);\n"
				   );
		}
	}
	else 
	{
		fprintf(fout, "\tACTION_RETURN_TYPE event = THIS(noEvent);\n\n");
		fprintf(fout
				, "\tDBG_PRINTF(\"%s_noAction\");\n"
				, ufMachineName(pcmd)
				);
	}

	if (pai->transition)
	{
		fprintf(fout
				, "\n\t%s = "
				, pmi->executes_fns_on_state_transitions
				  ? "s"
				  : "pfsm->state"
				);

		print_transition_for_assignment_to_state_var(pmi
													 , pai->transition
													 , pevent->name
													 , fout
													 );

		fprintf(fout, ";\n");
	}

	if (pai->transition)
	{
		print_any_transition_handlers(fout, pmi);
	}

	if (!(pmi->modFlags & mfActionsReturnVoid))
	{
		fprintf(fout, "\treturn event;\n");
	}
}

static void print_event_table_handler_body_for_single_state_or_pai_events_arv(FILE *fout, pID_INFO pevent, pACTION_INFO pai, pCMachineData pcmd)
{
	pMACHINE_INFO pmi     = pcmd->pmi;

	if (pai->action->name && strlen(pai->action->name))
	{
		if (add_profiling_macros)
		{
			fprintf(fout
					, "\tACTION_ENTRY(pfsm);\n"
				   );
		}

		fprintf(fout
				, "\tUFMN(%s)(pfsm);\n"
				, pai->action->name
				);

		if (add_profiling_macros)
		{
			fprintf(fout
					, "\tACTION_EXIT(pfsm);\n"
				   );
		}

	}
	else
	{
		fprintf(fout
				, "\tDBG_PRINTF(\"%s_noAction\");\n"
				, ufMachineName(pcmd)
				);
	}

	if (pai->transition)
	{
		fprintf(fout
				, "\t%s = "
				, pmi->executes_fns_on_state_transitions
				  ? "s"
				  : "pfsm->state"
				);

		print_transition_for_assignment_to_state_var(pmi
													 , pai->transition
													 , pevent->name
													 , fout
													 );

	}

	fprintf(fout, ";\n");

	if (pai->transition)
	{
		print_any_transition_handlers(fout, pmi);
	}

}

static void print_event_table_handler_body_for_single_state_or_pai_events_ars(FILE *fout, pID_INFO pevent, pACTION_INFO pai, pCMachineData pcmd)
{
	pMACHINE_INFO pmi = pcmd->pmi;

	fprintf(fout
			, "\tACTION_RETURN_TYPE state;\n\n"
			);

	if (pai->action->name && strlen(pai->action->name))
	{
		if (add_profiling_macros)
		{
			fprintf(fout
					, "\tACTION_ENTRY(pfsm);\n"
				   );
		}

		fprintf(fout
				, "\tstate = UFMN(%s)(pfsm);\n"
				, pai->action->name
				);

		if (add_profiling_macros)
		{
			fprintf(fout
					, "\tACTION_EXIT(pfsm);\n"
				   );
		}
	}

	if (pai->transition)
	{
		fprintf(fout
				, "\tstate = "
				);

		print_transition_for_assignment_to_state_var(pmi
													 , pai->transition
													 , pevent->name
													 , fout
													 );

	}

	fprintf(fout, ";\n\n\treturn state;\n");

}

static void print_event_table_handler_body_for_multiple_state_events_are(FILE *fout, pEVENT_DATA ped, pITERATOR_CALLBACK_HELPER pich)
{
	FSMLANG_DEVELOP_PRINTF(fout, "/* FSMLANG_DEVELOP: %s */\n", __func__);

	pFSMCOutputGenerator pfsmcog = pich->pfsmcog;

	if (ped->phandling_states->count > 0)
	{
		fprintf(fout
				, "\tACTION_RETURN_TYPE event = THIS(noEvent);\n\n"
			   );

		fprintf(fout
				, "\tswitch (pfsm->state)\n\t{\n"
			   );

		iterate_list(ped->phandling_states
					 , print_event_table_handler_state_case
					 , pich
					);

		if (ped->phandling_states->count != pich->pcmd->pmi->state_list->count)
		{
			fprintf(fout
					, "\t\tdefault:\n"
					);

			if (empty_cell_fn)
			{
				fprintf(pich->pcmd->cFile
						, "\t\t\tUFMN(%s)(pfsm);\n"
						, empty_cell_fn
						);
			}
			else
			{
				fprintf(fout
						, "\t\t\tDBG_PRINTF(\"%s_noAction\");\n"
						, ufMachineName(pich->pcmd)
						);
			}

			fprintf(fout
					, "\t\t\tbreak;\n"
					);
		}

		fprintf(fout
				, "\t}\n"
			   );

		if (iterate_list(ped->phandling_states, find_transition, pich))
		{
			print_any_transition_handlers(fout, pich->pcmd->pmi);
		}

		fprintf(fout
				, "\n\treturn event;\n"
				);
	}
	else
	{
		fprintf(fout, "\t(void) pfsm;\n\n");
		if (empty_cell_fn)
		{
			fprintf(pich->pcmd->cFile
					, "\tUFMN(%s)(pfsm);\n"
					, empty_cell_fn
					);
		}
		else
		{
			fprintf(fout
					, "\tDBG_PRINTF(\"%s_noAction\");\n"
					, ufMachineName(pich->pcmd)
					);
		}
		fprintf(fout, "\treturn THIS(noEvent);\n");
	}

}

static void print_event_table_handler_body_for_multiple_state_events_arv(FILE *fout, pEVENT_DATA ped, pITERATOR_CALLBACK_HELPER pich)
{
	FSMLANG_DEVELOP_PRINTF(fout, "/* FSMLANG_DEVELOP: %s */\n", __func__);

	pFSMCOutputGenerator pfsmcog = pich->pfsmcog;

	if (ped->phandling_states->count > 0)
	{
		fprintf(fout
				, "\tswitch (pfsm->state)\n\t{\n"
			   );

		iterate_list(ped->phandling_states
					 , print_event_table_handler_state_case
					 , pich
					);

		if (ped->phandling_states->count != pich->pcmd->pmi->state_list->count)
		{
			fprintf(fout, "\t\tdefault:\n\t\t\tbreak;\n");
		}

		fprintf(fout
				, "\t}\n"
			   );

		if (iterate_list(ped->phandling_states, find_transition, pich))
		{
			print_any_transition_handlers(fout, pich->pcmd->pmi);
		}
	}
	else
	{
		fprintf(fout, "\t(void) pfsm;\n\n");
		if (empty_cell_fn)
		{
			fprintf(pich->pcmd->cFile
					, "\tUFMN(%s)(pfsm);\n"
					, empty_cell_fn
					);
		}
		else
		{
			fprintf(fout
					, "\tDBG_PRINTF(\"%s_noAction\");\n"
					, ufMachineName(pich->pcmd)
					);
		}
	}

}

static void print_event_table_handler_body_for_multiple_state_events_ars(FILE *fout, pEVENT_DATA ped, pITERATOR_CALLBACK_HELPER pich)
{
	FSMLANG_DEVELOP_PRINTF(fout, "/* FSMLANG_DEVELOP: %s */\n", __func__);

	pFSMCOutputGenerator pfsmcog = pich->pfsmcog;

	if (ped->phandling_states->count)
	{
		fprintf(fout
				, "\tACTION_RETURN_TYPE state;\n"
			   );

		fprintf(fout
				, "\tswitch (pfsm->state)\n\t{\n"
			   );

		iterate_list(ped->phandling_states
					 , print_event_table_handler_state_case
					 , pich
					);

		fprintf(fout, "\t\tdefault:\n\t\t\tbreak;\n\t}\n\n");

		fprintf(fout, "\treturn state;\n");
	}
	else
	{
		if (empty_cell_fn)
		{
			fprintf(pich->pcmd->cFile
					, "\tUFMN(%s)(pfsm);\n"
					, empty_cell_fn
					);
		}
		else
		{
			fprintf(fout
					, "\tDBG_PRINTF(\"%s_noAction\");\n"
					, ufMachineName(pich->pcmd)
					);
		}
		fprintf(fout, "\treturn pfsm->state;\n");
	}

}

static bool print_event_table_handler_state_case_are(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO                  pstate     = (pID_INFO) pelem->mbr;
	pID_INFO                  pnextState = (pID_INFO) (pelem->next ? pelem->next->mbr : NULL);
	pITERATOR_CALLBACK_HELPER pich       = (pITERATOR_CALLBACK_HELPER) data;
	pID_INFO                  pevent     = (pID_INFO) pich->pOtherElem->mbr;
	FILE                     *fout       = pich->pcmd->cFile;
	pMACHINE_INFO             pmi        = pich->pcmd->pmi;
	pACTION_INFO              pai        = pmi->actionArray[pevent->order][pstate->order];
	pACTION_INFO              paiNext    = pnextState
		                                   ? pmi->actionArray[pevent->order][pnextState->order]
		                                   : NULL
		                                   ;

	FSMLANG_DEVELOP_PRINTF(fout, "/* FSMLANG_DEVELOP: %s */\n", __func__);

	fprintf(fout
			, "\t\tcase STATE(%s):\n"
			, pstate->name
			);

	if (pai != paiNext)
	{
		if (pai->action->name && strlen(pai->action->name))
		{
			if (add_profiling_macros)
			{
				fprintf(fout
						, "\tACTION_ENTRY(pfsm);\n"
					   );
			}

			fprintf(fout
					, "\t\t\tevent = UFMN(%s)(pfsm);\n"
					, pai->action->name
					);

			if (add_profiling_macros)
			{
				fprintf(fout
						, "\tACTION_EXIT(pfsm);\n"
					   );
			}

		}
		else
		{
			if (empty_cell_fn && (pai->transition == NULL))
			{
				fprintf(pich->pcmd->cFile
						, "\t\t\tUFMN(%s)(pfsm);\n"
						, empty_cell_fn
						);
			}
			else
			{
				fprintf(fout
						, "\t\t\tDBG_PRINTF(\"%s_noAction\");\n"
						, ufMachineName(pich->pcmd)
						);
			}
		}

		if (pai->transition)
		{
			fprintf(fout
					, "\t\t\t%s = "
					, pmi->executes_fns_on_state_transitions
					  ? "s"
					  : "pfsm->state"
					);
			print_transition_for_assignment_to_state_var(pich->pcmd->pmi
														 , pai->transition
														 , pevent->name
														 , fout);
			fprintf(fout, ";\n");
		}

		fprintf(fout, "\t\t\tbreak;\n");
	}

	return false;
}

static bool print_event_table_handler_state_case_arv(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO                  pstate     = (pID_INFO) pelem->mbr;
	pID_INFO                  pnextState = (pID_INFO) (pelem->next ? pelem->next->mbr : NULL);
	pITERATOR_CALLBACK_HELPER pich       = (pITERATOR_CALLBACK_HELPER) data;
	FILE                     *fout       = pich->pcmd->cFile;
	pMACHINE_INFO             pmi        = pich->pcmd->pmi;
	pID_INFO                  pevent     = (pID_INFO) pich->pOtherElem->mbr;
	pACTION_INFO              pai        = pmi->actionArray[pevent->order][pstate->order];
	pACTION_INFO              paiNext    = pnextState
		                                   ? pmi->actionArray[pevent->order][pnextState->order]
		                                   : NULL
		                                   ;

	FSMLANG_DEVELOP_PRINTF(fout, "/* FSMLANG_DEVELOP: %s */\n", __func__);

	fprintf(fout
			, "\t\tcase STATE(%s):\n"
			, pstate->name
			);

	if (pai != paiNext)
	{
		if (pai->action->name && strlen(pai->action->name))
		{
			if (add_profiling_macros)
			{
				fprintf(fout
						, "\tACTION_ENTRY(pfsm);\n"
					   );
			}

			fprintf(fout
					, "\t\t\tUFMN(%s)(pfsm);\n"
					, pai->action->name
					);

			if (add_profiling_macros)
			{
				fprintf(fout
						, "\tACTION_EXIT(pfsm);\n"
					   );
			}

		}
		else
		{
			if (empty_cell_fn)
			{
				fprintf(pich->pcmd->cFile
						, "\t\t\tUFMN(%s)(pfsm);\n"
						, empty_cell_fn
						);
			}
			else
			{
				fprintf(fout
						, "\t\t\tDBG_PRINTF(\"%s_noAction\");\n"
						, ufMachineName(pich->pcmd)
						);
			}
		}

		if (pai->transition)
		{
			fprintf(fout
					, "\n\t%s = "
					, pmi->executes_fns_on_state_transitions
					  ? "s"
					  : "pfsm->state"
					);
			print_transition_for_assignment_to_state_var(pich->pcmd->pmi
														 , pai->transition
														 , pevent->name
														 , fout);
			fprintf(fout, ";\n");
		}

		fprintf(fout, "\t\t\tbreak;\n");
	}

	return false;
}

static bool print_event_table_handler_state_case_ars(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO                  pstate     = (pID_INFO) pelem->mbr;
	pID_INFO                  pnextState = (pID_INFO) (pelem->next ? pelem->next->mbr : NULL);
	pITERATOR_CALLBACK_HELPER pich       = (pITERATOR_CALLBACK_HELPER) data;
	FILE                     *fout       = pich->pcmd->cFile;
	pMACHINE_INFO             pmi        = pich->pcmd->pmi;
	pID_INFO                  pevent     = (pID_INFO) pich->pOtherElem->mbr;
	pACTION_INFO              pai        = pmi->actionArray[pevent->order][pstate->order];
	pACTION_INFO              paiNext    = pnextState
		                                   ? pmi->actionArray[pevent->order][pnextState->order]
		                                   : NULL
		                                   ;
	bool                      handled    = false;

	FSMLANG_DEVELOP_PRINTF(fout, "/* FSMLANG_DEVELOP: %s */\n", __func__);

	fprintf(fout
			, "\t\tcase STATE(%s):\n"
			, pstate->name
			);

	if (pai != paiNext)
	{
		if (pai->action->name && strlen(pai->action->name))
		{
			if (add_profiling_macros)
			{
				fprintf(fout
						, "\tACTION_ENTRY(pfsm);\n"
					   );
			}

			fprintf(fout
					, "\t\t\tstate = UFMN(%s)(pfsm);\n"
					, pai->action->name
					);

			if (add_profiling_macros)
			{
				fprintf(fout
						, "\tACTION_EXIT(pfsm);\n"
					   );
			}

			handled = true;
		}

		if (pai->transition)
		{
			fprintf(fout
					, "\t\t\tstate = "
					);
			print_transition_for_assignment_to_state_var(pich->pcmd->pmi
														 , pai->transition
														 , pevent->name
														 , fout);
			fprintf(fout, ";\n");

			handled = true;
		}

		if (!handled)
		{
			if (empty_cell_fn)
			{
				fprintf(pich->pcmd->cFile
						, "\t\t\tUFMN(%s)(pfsm);\n"
						, empty_cell_fn
						);
			}
			else
			{
				fprintf(fout
						, "\t\t\tDBG_PRINTF(\"%s_noAction\");\n"
						, ufMachineName(pich->pcmd)
						);
			}
		}

		fprintf(fout, "\t\t\tbreak;\n");
	}

	return false;
}

static bool find_transition(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO                  pstate = (pID_INFO) pelem->mbr;
	pITERATOR_CALLBACK_HELPER pich   = (pITERATOR_CALLBACK_HELPER) data;
	pID_INFO                  pevent = (pID_INFO) pich->pOtherElem->mbr;
    pACTION_INFO              pai    = pich->ih.pmi->actionArray[pevent->order][pstate->order];

	return pai && pai->transition;
}


static void defineCEventTableMachineFSM(pFSMCOutputGenerator pfsmcog)
{
	pCMachineData pcmd = pfsmcog->pcmd;
	pMACHINE_INFO pmi  = pcmd->pmi;

	FSMLANG_DEVELOP_PRINTF(pcmd->cFile, "/* FSMLANG_DEVELOP: %s */\n", __func__);

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

   if (!(pmi->modFlags & ACTIONS_RETURN_FLAGS))
   {
      fprintf(pcmd->cFile
              , "\t%s_EVENT%s e = event%s;\n\n"
              , ucfqMachineName(pcmd)
              , pmi->data_block_count ? "_ENUM"  : ""
              , pmi->data_block_count ? "->event" : ""
             );
   }
   else if (pmi->data_block_count != 0)
   {
	   fprintf(pcmd->cFile
			   , "\t%s_EVENT_ENUM e = event->event;\n\n"
			   , ucfqMachineName(pcmd)
			   );
   }

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

   fprintf(pcmd->cFile
		   , "\n}\n\n"
		   );
   FSMLANG_DEVELOP_PRINTF(pcmd->cFile, "/* %s: %u */\n", __FILE__, __LINE__);

}

static void writeOriginalEventTableFSMAre(pFSMCOutputGenerator pfsmcog)
{
	writeOriginalSwitchFSMLoopAre(pfsmcog);
}

static void writeOriginalEventTableFSMArv(pFSMCOutputGenerator pfsmcog)
{
	writeOriginalSwitchFSMLoopArv(pfsmcog);
}

static void writeOriginalEventTableSubFSMAre(pFSMCOutputGenerator pfsmcog)
{
	writeOriginalSwitchSubFSMLoopAre(pfsmcog);
}

static void writeOriginalEventTableSubFSMArv(pFSMCOutputGenerator pfsmcog)
{
	writeOriginalSwitchSubFSMLoopArv(pfsmcog);
}

static void writeEventTableFSMLoopInnards(pFSMCOutputGenerator pfsmcog, char *tabstr)
{
	pCMachineData pcmd = pfsmcog->pcmd;
	pMACHINE_INFO pmi  = pcmd->pmi;

	if (pmi->modFlags & mfActionsReturnVoid)
	{
	   fprintf(pcmd->cFile
			   , "%s\t((* (*pfsm->eventsArray)[%s])(pfsm));\n"
			   , tabstr
			   , (pmi->data_block_count == 0) ? "event" : "e"
			   );
	}
	else
	{
	   fprintf(pcmd->cFile
			   , "%s\te = ((* (*pfsm->eventsArray)[e])(pfsm));\n"
			   , tabstr
			   );
	}

}

static void writeEventTableSubFSMLoopInnards(pFSMCOutputGenerator pfsmcog, char *tabstr)
{
	(void) tabstr;
	pCMachineData pcmd = pfsmcog->pcmd;
	pMACHINE_INFO pmi  = pcmd->pmi;

	fprintf(pcmd->cFile
			, "\t\tif ((%s >= THIS(firstEvent))\n\t\t    && (%s < THIS(noEvent))\n\t\t\t)\n\t\t{\n"
			, pmi->modFlags & ACTIONS_RETURN_FLAGS ? "event" : "e"
			, pmi->modFlags & ACTIONS_RETURN_FLAGS ? "event" : "e"
			);

	if (pmi->modFlags & ACTIONS_RETURN_FLAGS)
	{
	   fprintf(pcmd->cFile
			   , "\t\t((* (*pfsm->eventsArray)[event - THIS(firstEvent)])(pfsm));\n"
			   );
	}
	else
	{
	   fprintf(pcmd->cFile
			   , "\t\te = ((* (*pfsm->eventsArray)[e - THIS(firstEvent)])(pfsm));\n"
			   );
	}

	fprintf(pcmd->cFile
			, "\t\t}\n"
			);

}

static void writeActionsReturnStateEventTableFSM(pFSMCOutputGenerator pfsmcog)
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

	print_tab_levels(pcmd->cFile,tab_level);
	fprintf(pcmd->cFile
			, "\ts = ((* (*pfsm->eventsArray)[%s])(pfsm));\n\n"
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

static void writeActionsReturnStateEventTableSubFSM(pFSMCOutputGenerator pfsmcog)
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

	print_tab_levels(pcmd->cFile,tab_level);
	fprintf(pcmd->cFile
			, "\ts = ((* (*pfsm->eventsArray)[event - THIS(firstEvent)])(pfsm));\n\n"
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

	fprintf(pcmd->cFile, "}\n\n");
}

static void defineCEventTableSubMachineFSM(pFSMCOutputGenerator pfsmcog)
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

	fprintf(pcmd->cFile
			, "%s %sFSM(p%s pfsm, %s event)\n{\n"
			, subFsmFnReturnType(pcmd)
			, machineName(pcmd)
			, fsmType(pcmd)
			, fsmFnEventType(pcmd)
		   );

	if (!(pmi->modFlags & ACTIONS_RETURN_FLAGS))
	{
		fprintf(pcmd->cFile
				, "\t%s e = event;\n\n"
				, eventType(pcmd)
			   );
	}

	if (add_profiling_macros && profile_sub_fsms)
	{
		fprintf(pcmd->cFile
				, "\tFSM_ENTRY(pfsm);\n\n"
			   );
	}

	writeFSMLoop(pfsmcog);

	if (add_profiling_macros && profile_sub_fsms)
	{
		fprintf(pcmd->cFile
				, "\tFSM_EXIT(pfsm);\n"
			   );
	}

	if (!(pmi->modFlags & ACTIONS_RETURN_FLAGS))
	{
		fprintf(pcmd->cFile, "\n}\n\n");
	}

}

static void print_any_transition_handlers(FILE *fout, pMACHINE_INFO pmi)
{
	if (pmi->executes_fns_on_state_transitions)
	{
		fprintf(fout
				, "\n\tif (s != pfsm->state)\n\t{\n"
				);

		if (pmi->machineTransition)
		{
			fprintf(fout
					, "\t\tUFMN(%s)(pfsm,s);\n"
					, pmi->machineTransition->name
					);
		}

		if (pmi->states_with_exit_fns_count)
		{
			fprintf(fout
					, "\t\trunAppropriateExitFunction(%spfsm->state);\n"
					, pmi->data ? "&pfsm->data, " : ""
				   );
		}

		if (pmi->states_with_entry_fns_count)
		{
			fprintf(fout
					, "\t\trunAppropriateEntryFunction(%ss);\n"
					, pmi->data ? "&pfsm->data, " : ""
				   );
		}

		fprintf(fout
				,"\t}\n\n"
				);

		fprintf(fout
				, "\tpfsm->state = s;\n\n"
				);
	}

}

