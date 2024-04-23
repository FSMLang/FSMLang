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

static void writeCEventTableMachine(pFSMOutputGenerator,pMACHINE_INFO); 
static void writeCEventTableSubMachine(pFSMOutputGenerator,pMACHINE_INFO); 
static void writeCEventTableMachineInternal(pFSMCOutputGenerator);
static void declareCEventTableMachineEventTableSize(pCMachineData);
static void defineCEventTableMachineStruct(pCMachineData);
static void defineEventFnArray(pCMachineData);
static void defineCEventTableHandlers(pCMachineData);
static void defineCEventTableMachineFSM(pCMachineData);
static void defineCEventTableSubMachineFSM(pCMachineData);
static void writeOriginalEventTableFSM(pCMachineData);
static void writeOriginalEventTableSubFSM(pCMachineData);
static void writeActionsReturnStateEventTableFSM(pCMachineData);
static void writeEventTableFSMLoopInnards(pCMachineData,pMACHINE_INFO,char*);
static void writeEventTableSubFSMLoopInnards(pCMachineData,pMACHINE_INFO,char*);
static int  writeCEventTableSubMachineInternal(pCMachineData);
static void print_event_table_handler_body_for_single_state_or_pai_events_are(FILE*,pID_INFO,pACTION_INFO,pMACHINE_INFO);
static void print_event_table_handler_body_for_single_state_or_pai_events_arv(FILE*,pID_INFO,pACTION_INFO,pMACHINE_INFO);
static void print_event_table_handler_body_for_single_state_or_pai_events_ars(FILE*,pID_INFO,pACTION_INFO,pMACHINE_INFO);
static void print_event_table_handler_body_for_multiple_state_events_are(FILE*,pEVENT_DATA,pITERATOR_CALLBACK_HELPER);
static void print_event_table_handler_body_for_multiple_state_events_arv(FILE*,pEVENT_DATA,pITERATOR_CALLBACK_HELPER);
static void print_event_table_handler_body_for_multiple_state_events_ars(FILE*,pEVENT_DATA,pITERATOR_CALLBACK_HELPER);
static void chooseWorkerFunctions(pCMachineData);

static bool event_handler_cannot_be_its_action(pID_INFO,pMACHINE_INFO,pACTION_INFO*);
static bool print_event_fn_signature(pLIST_ELEMENT,void*);
static bool print_event_handler_name(pLIST_ELEMENT,void*);
static bool define_event_table_handler(pLIST_ELEMENT,void*);
static bool print_event_table_handler_state_case_are(pLIST_ELEMENT,void*);
static bool print_event_table_handler_state_case_arv(pLIST_ELEMENT,void*);
static bool print_event_table_handler_state_case_ars(pLIST_ELEMENT,void*);

#define writeFSMLoop(A) pcmd->wfsm((A))

#define print_event_table_handler_body_for_single_state_or_pai_events(A,B,C,D) \
pcmd->pethbsspe((A),(B),(C),(D))

#define print_event_table_handler_body_for_multiple_state_events(A,B,C) \
pcmd->pethbmse((A),(B),(C))

#define print_event_table_handler_state_case pcmd->pethsc

static FSMCOutputGenerator CEventTableMachineWriter = {
   {
      initCMachine
      , writeCEventTableMachine
      , closeCMachine
	  , generateCEventTableMachineWriter
   }
   , NULL
};

static void chooseWorkerFunctions(pCMachineData pcmd)
{
	switch ((pcmd->pmi->modFlags & ACTIONS_RETURN_FLAGS))
	{
	case 0: //this is "actions return events"
		pcmd->wfsm      = pcmd->pmi->parent
			                 ? writeOriginalEventTableSubFSM
			                 : writeOriginalEventTableFSM
			                 ;
		pcmd->pethbsspe = print_event_table_handler_body_for_single_state_or_pai_events_are;
		pcmd->pethbmse  = print_event_table_handler_body_for_multiple_state_events_are; 
		pcmd->pethsc    = print_event_table_handler_state_case_are;
		break;
	case mfActionsReturnVoid:
		pcmd->wfsm      = writeOriginalEventTableFSM;
		pcmd->pethbsspe = print_event_table_handler_body_for_single_state_or_pai_events_arv;
		pcmd->pethbmse  = print_event_table_handler_body_for_multiple_state_events_arv; 
		pcmd->pethsc    = print_event_table_handler_state_case_arv;
		break;
	case mfActionsReturnStates:
		pcmd->wfsm      = writeActionsReturnStateEventTableFSM;
		pcmd->pethbsspe = print_event_table_handler_body_for_single_state_or_pai_events_ars;
		pcmd->pethbmse  = print_event_table_handler_body_for_multiple_state_events_ars; 
		pcmd->pethsc    = print_event_table_handler_state_case_ars;
		break;
	default:
		yyerror("invalid actions return statement");
		break;
	}
}
static void writeCEventTableMachine(pFSMOutputGenerator pfsmog, pMACHINE_INFO pmi) 
{
	pFSMCOutputGenerator pfsmcog = (pFSMCOutputGenerator) pfsmog;

	pfsmcog->pcmd->pmi     = pmi;
	pfsmcog->pcmd->cfsmliw = writeEventTableFSMLoopInnards;
	chooseWorkerFunctions(pfsmcog->pcmd);

	writeCEventTableMachineInternal(pfsmcog);

	if (pmi->machine_list)
	{
	   write_machines(pmi->machine_list, generateCEventTableMachineWriter, pfsmog);
	}
}

static int writeCEventTableSubMachineInternal(pCMachineData pcmd)
{
   if (!pcmd)   return 1;

   pMACHINE_INFO pmi = pcmd->pmi;

   ITERATOR_CALLBACK_HELPER ich = {
	   . ih = {
		   .pmi      = pmi
	   }
       , .pcmd   = pcmd
       , .define = false
   };

   /* do this now, since some header stuff puts content into the source file.*/
   addNativeImplementationPrologIfThereIsAny(pmi, pcmd->cFile);

   subMachineHeaderStart(pcmd, pmi, "event_fn");

   declareCEventTableMachineEventTableSize(pcmd);

   defineCEventTableMachineStruct(pcmd);

   cswitchSubMachineHeaderEnd(pcmd, pmi, false);

   /*
     Source File
   */

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
      generateInstance(pcmd, pmi, "event_fn");
   }

   defineCEventTableSubMachineFSM(pcmd);

   /* write our sub-machine lookup, if needed */
   if (pmi->machine_list)
   {
	  defineSubMachineFinder(pcmd, pmi);
   }

   defineStateEntryAndExitManagers(pcmd, pmi);

   defineCEventTableHandlers(pcmd);

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

   /* write our transition functions, if needed */
   if (pmi->transition_fn_list->count)
   {
	   writeStateTransitions(pcmd, pmi);
	   writeNoTransition(pcmd, pmi);
   }

   writeDebugInfo(pcmd, pmi);

	addNativeImplementationEpilogIfThereIsAny(pmi, pcmd->cFile);

   return 0;
}

static void writeCEventTableSubMachine(pFSMOutputGenerator pfsmog, pMACHINE_INFO pmi) 
{

	pFSMCSubMachineOutputGenerator pfsmcsmog = (pFSMCSubMachineOutputGenerator) pfsmog;

	pfsmcsmog->pcmd->pmi = pmi;
	pfsmcsmog->pcmd->action_return_type = pfsmcsmog->parent_fsmcog->pcmd->action_return_type;
	pfsmcsmog->pcmd->cfsmliw = writeEventTableSubFSMLoopInnards;

	chooseWorkerFunctions(pfsmcsmog->pcmd);
	writeCEventTableSubMachineInternal(pfsmcsmog->pcmd);

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
		pFSMCSubMachineOutputGenerator pfsmceventtableog = calloc(1, sizeof(FSMCSubMachineOutputGenerator));

		pfsmceventtableog->fsmog.writeMachine = writeCEventTableSubMachine;
		pfsmceventtableog->fsmog.initOutput   = initCSubMachine;
		pfsmceventtableog->fsmog.closeOutput  = closeCMachine;
		pfsmceventtableog->fsmog.fsmogFactory = generateCEventTableMachineWriter;

		pfsmceventtableog->top_level_fsmcog = &CEventTableMachineWriter;
		pfsmceventtableog->parent_fsmcog    = (pFSMCOutputGenerator) parent;

		pfsmog = (pFSMOutputGenerator)pfsmceventtableog;
	}
	else
	{
		pfsmog = (pFSMOutputGenerator)&CEventTableMachineWriter;
	}

	if (output_generated_file_names_only)
	{
		pfsmog->initOutput   = parent ? initCSubMachineFN : initCMachineFN;
		pfsmog->writeMachine = writeCSwitchMachineFN;
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

	declareCEventTableMachineEventTableSize(pcmd);

	defineCEventTableMachineStruct(pcmd);

	cswitchMachineHeaderEnd(pcmd, pmi, false);

	/*
	    Source File
	*/

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
		generateInstance(pcmd, pmi, "event_fn");
	}

	if (generate_run_function)
	{
	   generateRunFunction(pcmd, pmi);
	}

	defineCEventTableMachineFSM(pcmd);

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

	defineCEventTableHandlers(pcmd);

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

	/* write our transition functions, if needed */
	if (pmi->transition_fn_list->count)
	{
		writeStateTransitions(pcmd, pmi);
		writeNoTransition(pcmd, pmi);
	}

	writeDebugInfo(pcmd, pmi);

	addNativeImplementationEpilogIfThereIsAny(pmi, pcmd->cFile);

	fprintf(pcmd->cFile, "\n");

}

static void declareCEventTableMachineEventTableSize(pCMachineData pcmd)
{
	FSMLANG_DEVELOP_PRINTF(pcmd->hFile, "/* FSMLANG_DEVELOP: %s */\n", __func__);

	fprintf(pcmd->hFile
			, "#define %s_numMachineEvents %u\n"
			, machineName(pcmd)
			, pcmd->pmi->event_list->count
			);

}


static void defineCEventTableMachineStruct(pCMachineData pcmd)
{
	pMACHINE_INFO pmi = pcmd->pmi;

	FSMLANG_DEVELOP_PRINTF(pcmd->hFile, "/* FSMLANG_DEVELOP: %s */\n", __func__);

   /* put the machine structure definition into the header file */
   fprintf(pcmd->hFile
		   , "struct _%s_struct_ {\n"
		   , machineName(pcmd)
		   );

   if (pmi->data)
   {
      fprintf(pcmd->hFile
			  , "\t%-*sdata;\n"
			  , (int)pcmd->sub_machine_struct_format_width + 6 /* for the "const " */
			  , fsmDataType(pcmd)
			 );
   }

   fprintf(pcmd->hFile
		   , "\t%-*sstate;\n"
		   , (int)pcmd->sub_machine_struct_format_width + 6 /* for the "const " */
		   , stateType(pcmd)
		   );

   fprintf(pcmd->hFile
		   , "\t%-*sevent;\n"
		   , (int)pcmd->sub_machine_struct_format_width + 6 /* for the "const " */
		   , eventType(pcmd)
		  );

   fprintf(pcmd->hFile
		   , "\t%-*s const (*eventsArray)[%s_numMachineEvents];\n"
		   , (int)pcmd->sub_machine_struct_format_width
		   , actionFnType(pcmd)
		   , machineName(pcmd)
		   );

   if (pmi->machine_list)
   {
      fprintf(pcmd->hFile
			  , "\tp%-*sconst (*subMachineArray)[%s_numSubMachines];\n"
			  , (int)pcmd->sub_machine_struct_format_width
			  , subFsmIfType(pcmd)
			   , fqMachineName(pcmd)
			 );
   }

   fprintf(pcmd->hFile
		   , "\t%-*sfsm;\n};\n\n"
		   , (int)pcmd->sub_machine_struct_format_width + 6 /* for the "const " */
		   , fsmFnType(pcmd)
		   );
}

static bool print_event_fn_signature(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO                  pevent = (pID_INFO)                  pelem->mbr;
	pITERATOR_CALLBACK_HELPER pich   = (pITERATOR_CALLBACK_HELPER) data;
	pMACHINE_INFO             pmi    = pich->ih.pmi;

	if (event_handler_cannot_be_its_action(pevent, pmi, NULL))
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
			, "\nstatic const %s %s_event_fn_array[%s_numMachineEvents] =\n{\n"
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
	pMACHINE_INFO             pmi    = pich->pcmd->pmi;
	pACTION_INFO              pai;

	if (event_handler_cannot_be_its_action(pevent, pmi, &pai))
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

static bool event_handler_cannot_be_its_action(pID_INFO pevent, pMACHINE_INFO pmi, pACTION_INFO *ppai)
{
	bool         event_handler_can_be_its_action = false;
	pACTION_INFO pai                             = NULL;
	pEVENT_DATA  ped                             = &pevent->type_data.event_data;

	/* An event can be handled by its action function
	   if there is only one state in which the event is handled
	   and there is no transition for that event.
	*/
	if (ped->phandling_states->count == 1)
	{
		pID_INFO pstate = find_nth_list_member(ped->phandling_states, 0);
		pai = pmi->actionArray[pevent->order][pstate->order];

		event_handler_can_be_its_action = (pai->transition == NULL);
	}
	
	/* Or, an event can be handled by its action function
	   if there is a single pai for that event in all states
	   and that pai has no transition.
	*/
	if (!event_handler_can_be_its_action)
	{
		pai = ped->psingle_pai;

		event_handler_can_be_its_action =
			ped->single_pai_for_all_states
			&& (ped->psingle_pai->transition == NULL);

	}

	if (ppai)
	{
		(*ppai) = pai;
	}

	return !event_handler_can_be_its_action;
}

static void defineCEventTableHandlers(pCMachineData pcmd)
{
	ITERATOR_CALLBACK_HELPER ich = {
		.ih = {
			.pmi = pcmd->pmi
		}
		, .pcmd = pcmd
		, .define = true
	};

	iterate_list(pcmd->pmi->event_list
				 , define_event_table_handler
				 , &ich
				 );
}

static bool define_event_table_handler(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO                  pevent = (pID_INFO)                  pelem->mbr;
	pITERATOR_CALLBACK_HELPER pich   = (pITERATOR_CALLBACK_HELPER) data;
	pEVENT_DATA               ped    = &pevent->type_data.event_data;
	pMACHINE_INFO             pmi    = pich->ih.pmi;
	FILE                     *fout   = pich->pcmd->cFile;
	pCMachineData             pcmd   = pich->pcmd;
	pACTION_INFO              pai;

	if (event_handler_cannot_be_its_action(pevent, pmi, &pai))
	{
		(void)print_event_fn_signature(pelem, pich);

		if (
			(ped->phandling_states->count == 1)
			|| ped->single_pai_for_all_states
		   ) {
			print_event_table_handler_body_for_single_state_or_pai_events(fout, pevent, pai, pmi);
		}
		else
		{
			pich->pOtherElem = pelem;

			print_event_table_handler_body_for_multiple_state_events(fout, ped, pich);
		}

		fprintf(fout, "}\n\n");

	}

	return false;
}

static void print_event_table_handler_body_for_single_state_or_pai_events_are(FILE *fout, pID_INFO pevent, pACTION_INFO pai, pMACHINE_INFO pmi)
{
	if (!(pmi->modFlags & mfActionsReturnVoid))
	{
		fprintf(fout
				, "\tACTION_RETURN_TYPE event = "
				);
	}

	if (pai->action->name && strlen(pai->action->name))
	{
		fprintf(fout
				, "UFMN(%s)(pfsm);\n"
				, pai->action->name
				);
	}
	else if (!(pmi->modFlags & mfActionsReturnVoid))
	{
		fprintf(fout, "THIS(noEvent);\n");
	}

	if (pai->transition)
	{
		fprintf(fout
				, "\tpfsm->state = "
				);

		print_transition_for_assignment_to_state_var(pmi
													 , pai->transition
													 , pevent->name
													 , fout
													 );

	}

	fprintf(fout, ";\n");

	if (!(pmi->modFlags & mfActionsReturnVoid))
	{
		fprintf(fout, "\treturn event;\n");
	}
}

static void print_event_table_handler_body_for_single_state_or_pai_events_arv(FILE *fout, pID_INFO pevent, pACTION_INFO pai, pMACHINE_INFO pmi)
{
	if (!(pmi->modFlags & mfActionsReturnVoid))
	{
		fprintf(fout
				, "\tACTION_RETURN_TYPE event = "
				);
	}

	if (pai->action->name && strlen(pai->action->name))
	{
		fprintf(fout
				, "UFMN(%s)(pfsm);\n"
				, pai->action->name
				);
	}
	else if (!(pmi->modFlags & mfActionsReturnVoid))
	{
		fprintf(fout, "THIS(noEvent);\n");
	}

	if (pai->transition)
	{
		fprintf(fout
				, "\tpfsm->state = "
				);

		print_transition_for_assignment_to_state_var(pmi
													 , pai->transition
													 , pevent->name
													 , fout
													 );

	}

	fprintf(fout, ";\n");

	if (!(pmi->modFlags & mfActionsReturnVoid))
	{
		fprintf(fout, "\treturn event;\n");
	}
}

static void print_event_table_handler_body_for_single_state_or_pai_events_ars(FILE *fout, pID_INFO pevent, pACTION_INFO pai, pMACHINE_INFO pmi)
{
	fprintf(fout
			, "\tACTION_RETURN_TYPE state;\n\n"
			);

	if (pai->action->name && strlen(pai->action->name))
	{
		fprintf(fout
				, "\tstate = UFMN(%s)(pfsm);\n"
				, pai->action->name
				);
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
	pCMachineData pcmd = pich->pcmd;

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
			fprintf(fout, "\t\tdefault:\n\t\t\tbreak;\n");
		}

		fprintf(fout
				, "\t}\n\n\treturn event;\n"
			   );
	}
	else
	{
		fprintf(fout, "\t(void) pfsm;\n\n");
		fprintf(fout, "\treturn THIS(noEvent);\n");
	}

}

static void print_event_table_handler_body_for_multiple_state_events_arv(FILE *fout, pEVENT_DATA ped, pITERATOR_CALLBACK_HELPER pich)
{
	pCMachineData pcmd = pich->pcmd;

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

}

static void print_event_table_handler_body_for_multiple_state_events_ars(FILE *fout, pEVENT_DATA ped, pITERATOR_CALLBACK_HELPER pich)
{
	FSMLANG_DEVELOP_PRINTF(fout, "/* FSMLANG_DEVELOP: %s */\n", __func__);

	pCMachineData pcmd = pich->pcmd;

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
		fprintf(fout, "\treturn pfsm->state;\n");
	}

}

static bool print_event_table_handler_state_case_are(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO                  pstate = (pID_INFO) pelem->mbr;
	pITERATOR_CALLBACK_HELPER pich   = (pITERATOR_CALLBACK_HELPER) data;
	FILE                     *fout   = pich->pcmd->cFile;
	pMACHINE_INFO             pmi    = pich->pcmd->pmi;
	pACTION_INFO              pai    = pmi->actionArray[pich->pOtherElem->ordinal][pelem->ordinal];
	pID_INFO                  pevent = (pID_INFO) pich->pOtherElem->mbr;

	fprintf(fout
			, "\t\tcase STATE(%s):\n"
			, pstate->name
			);

	if (pai->action->name)
	{
		fprintf(fout
				, "\t\t\tevent = UFMN(%s)(pfsm);\n"
				, pai->action->name
				);
	}

	if (pai->transition)
	{
		fprintf(fout
				, "\t\t\tpfsm->state = "
				);
		print_transition_for_assignment_to_state_var(pich->pcmd->pmi
													 , pai->transition
													 , pevent->name
													 , fout);
		fprintf(fout, ";\n");
	}

	fprintf(fout, "\t\t\tbreak;\n");

	return false;
}

static bool print_event_table_handler_state_case_arv(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO                  pstate = (pID_INFO) pelem->mbr;
	pITERATOR_CALLBACK_HELPER pich   = (pITERATOR_CALLBACK_HELPER) data;
	FILE                     *fout   = pich->pcmd->cFile;
	pMACHINE_INFO             pmi    = pich->pcmd->pmi;
	pACTION_INFO              pai    = pmi->actionArray[pich->pOtherElem->ordinal][pelem->ordinal];
	pID_INFO                  pevent = (pID_INFO) pich->pOtherElem->mbr;

	fprintf(fout
			, "\t\tcase STATE(%s):\n"
			, pstate->name
			);

	if (pai->action->name && strlen(pai->action->name))
	{
		fprintf(fout
				, "UFMN(%s)(pfsm);\n"
				, pai->action->name
				);
	}

	if (pai->transition)
	{
		fprintf(fout
				, "\t\t\tpfsm->state = "
				);
		print_transition_for_assignment_to_state_var(pich->pcmd->pmi
													 , pai->transition
													 , pevent->name
													 , fout);
		fprintf(fout, ";\n");
	}

	fprintf(fout, "\t\t\tbreak;\n");

	return false;
}

static bool print_event_table_handler_state_case_ars(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO                  pstate = (pID_INFO) pelem->mbr;
	pITERATOR_CALLBACK_HELPER pich   = (pITERATOR_CALLBACK_HELPER) data;
	FILE                     *fout   = pich->pcmd->cFile;
	pMACHINE_INFO             pmi    = pich->pcmd->pmi;
	pACTION_INFO              pai    = pmi->actionArray[pich->pOtherElem->ordinal][pelem->ordinal];
	pID_INFO                  pevent = (pID_INFO) pich->pOtherElem->mbr;

	fprintf(fout
			, "\t\tcase STATE(%s):\n"
			, pstate->name
			);

	if (pai->action->name && strlen(pai->action->name))
	{
		fprintf(fout
				, "\t\t\tstate = UFMN(%s)(pfsm);\n"
				, pai->action->name
				);
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
	}

	fprintf(fout, "\t\t\tbreak;\n");

	return false;
}


static void defineCEventTableMachineFSM(pCMachineData pcmd)
{
	pMACHINE_INFO pmi = pcmd->pmi;

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

   if (!(pmi->modFlags & ACTIONS_RETURN_FLAGS))
   {
      fprintf(pcmd->cFile
              , "\t%s_EVENT%s e = event%s;\n\n"
              , ucfqMachineName(pcmd)
              , pmi->data_block_count ? "_ENUM"  : ""
              , pmi->data_block_count ? "->event" : ""
             );
   }

   writeFSMLoop(pcmd);

   fprintf(pcmd->cFile, "\n}\n\n");

}

static void writeOriginalEventTableFSM(pCMachineData pcmd)
{
	pMACHINE_INFO pmi = pcmd->pmi;

	writeOriginalSwitchFSMLoop(pcmd, pmi);
}

static void writeOriginalEventTableSubFSM(pCMachineData pcmd)
{
	pMACHINE_INFO pmi = pcmd->pmi;

	writeOriginalSwitchSubFSMLoop(pcmd, pmi);
}

static void writeEventTableFSMLoopInnards(pCMachineData pcmd, pMACHINE_INFO pmi, char *tabstr)
{

	if (pmi->modFlags & mfActionsReturnVoid)
	{
	   fprintf(pcmd->cFile
			   , "%s\t((* (*pfsm->eventsArray)[event])(pfsm));\n"
			   , tabstr
			   );
	}
	else
	{
	   fprintf(pcmd->cFile
			   , "%s\te = ((* (*pfsm->eventsArray)[e])(pfsm));\n"
			   , tabstr
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

static void writeEventTableSubFSMLoopInnards(pCMachineData pcmd, pMACHINE_INFO pmi, char *tabstr)
{
	(void) tabstr;

	fprintf(pcmd->cFile
			, "\t\tif ((e >= THIS(firstEvent))\n\t\t    && (e < THIS(noEvent))\n\t\t\t)\n\t\t{\n"
			);

	if (pmi->modFlags & mfActionsReturnVoid)
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

static void writeActionsReturnStateEventTableFSM(pCMachineData pcmd)
{
	pMACHINE_INFO pmi = pcmd->pmi;

   char *tabstr = "";

   fprintf(pcmd->cFile
           , "\t%s s%s;\n"
           , stateType(pcmd)
           , (pmi->machineTransition || pmi->states_with_entry_fns_count || pmi->states_with_exit_fns_count)
             ? " = UFMN(noTransition)" 
             : ""
           );

   fprintf(pcmd->cFile, "\n\tDBG_PRINTF(\"event: %%s; start state: %%s\"\n\t\t,");
   fprintf(pcmd->cFile
           , "%s_EVENT_NAMES[event]\n\t\t,"
           , ucfqMachineName(pcmd)
           );
   fprintf(pcmd->cFile
           , "%s_STATE_NAMES[pfsm->state]\n\t\t);\n\n"
           , ucnfMachineName(pcmd)
           );

   if (pmi->data_block_count)
   {
      fprintf(pcmd->cFile
              , "\ttranslateEventData(&pfsm->data, event);\n\n"
              );
   }

   fprintf(pcmd->cFile
           , "\t%ss = ((* (*pfsm->eventsArray)[event])(pfsm));\n\n"
           , tabstr
           );

   fprintf(pcmd->cFile
           , "\tif (s != UFMN(noTransition))\n\t{\n"
           );

   if (pmi->machineTransition || pmi->states_with_entry_fns_count || pmi->states_with_exit_fns_count)
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
   fprintf(pcmd->cFile
           , "%s_STATE_NAMES[pfsm->state]\n\t\t);\n"
           , ucnfMachineName(pcmd)
           );

}

static void defineCEventTableSubMachineFSM(pCMachineData pcmd)
{
	FSMLANG_DEVELOP_PRINTF(pcmd->cFile, "/* FSMLANG_DEVELOP: %s */\n", __func__);

	pMACHINE_INFO pmi = pcmd->pmi;

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

	if (pmi->machine_list)
	{
		declareSubMachineManagers(pcmd, pmi);
	}

	if (pmi->states_with_entry_fns_count || pmi->states_with_exit_fns_count)
	{
		declareStateEntryAndExitManagers(pcmd, pmi);
	}

	fprintf(pcmd->cFile
			, "%s %sFSM(p%s pfsm, %s event)\n{\n"
			, eventType(pcmd)
			, machineName(pcmd)
			, fsmType(pcmd)
			, eventType(pcmd)
		   );

	if (!(pmi->modFlags & mfActionsReturnVoid))
	{
		fprintf(pcmd->cFile
				, "\t%s e = event;\n\n"
				, eventType(pcmd)
			   );
	}

	writeFSMLoop(pcmd);

	fprintf(pcmd->cFile, "\n}\n\n");

}

