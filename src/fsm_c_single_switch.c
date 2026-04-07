/**
*  fsm_c_single_switch.c
*
*    Each machine has a single switch combining states and events.
*
*    FSMLang (fsm) - A Finite State Machine description language.
*    Copyright (C) 2026  Steven Stanton
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

#include "fsm_c_single_switch.h"
#include "fsm_c_switch_common.h"

#include "ancestry.h"
#include "y.tab.h"
#include "action_info_list.h"

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

static void writeCSingleSwitchMachine(pFSMOutputGenerator,pMACHINE_INFO);
static void writeCSingleSwitchSubMachine(pFSMOutputGenerator,pMACHINE_INFO);

static void writeCSingleSwitchMachineInternal(pFSMCSwitchOutputGenerator);
static void writeCSingleSwitchSubMachineInternal(pFSMCSwitchOutputGenerator);
static void chooseWorkerFunctions(pFSMCSwitchOutputGenerator);
static void writeSingleSwitchSubFSMLoop(pFSMCOutputGenerator);
static void writeSingleSwitchFSMLoopInnardsAre(pFSMCOutputGenerator,char*);
static void writeSingleSwitchFSMLoopInnardsArv(pFSMCOutputGenerator,char*);
static void writeSingleSwitchFSMLoopInnardsArs(pFSMCOutputGenerator,char*);
static void defineCSingleSwitchMachineStruct(pCMachineData);
static void defineCSingleSwitchMachineFSM(pFSMCOutputGenerator);
static void defineCSingleSwitchSubMachineFSM(pFSMCOutputGenerator);

static bool print_event_state_case_are(pLIST_ELEMENT,void*);
static bool print_event_state_case_arv(pLIST_ELEMENT,void*);
static bool print_event_state_case_ars(pLIST_ELEMENT,void*);
static void setNewState(pFSMCOutputGenerator);
static void handleEmptyCells(pFSMCOutputGenerator,char*);
static bool print_consolidated_cases_are(pLIST_ELEMENT,void*);
static bool print_consolidated_cases_arv(pLIST_ELEMENT,void*);
static bool print_consolidated_cases_ars(pLIST_ELEMENT,void*);
static bool print_matrices(pLIST_ELEMENT,void*);
static bool print_matrix_event_cases(pLIST_ELEMENT,void*);
static bool print_matrix_case(pLIST_ELEMENT,void*);

static FSMCSwitchOutputGenerator CSingleSwitchMachineWriter =
{
	.fsmcog = {
		.fsmog = {
			.writeMachine   = writeCSingleSwitchMachine
			, .initOutput   = initCMachine
			, .closeOutput  = closeCMachine
			, .fsmogFactory = generateCSingleSwitchMachineWriter
		}
		, .wconvenience_macros = standardConvenienceMacros
		, .wtransition_fn_typedef = empty
	}
};

static void writeCSingleSwitchMachine(pFSMOutputGenerator pfsmog, pMACHINE_INFO pmi)
{
	pFSMCSwitchOutputGenerator pfsmcswog = (pFSMCSwitchOutputGenerator) pfsmog;

	pfsmcswog->fsmcog.pcmd->pmi = pmi;
	chooseWorkerFunctions(pfsmcswog);

	writeCFilePreambles(&(pfsmcswog->fsmcog));
	writeCSingleSwitchMachineInternal(pfsmcswog);

	if (pmi->machine_list)
	{
		write_machines(pmi->machine_list
					   , generateCSingleSwitchMachineWriter
					   , pfsmog
					   );
	}
}

static void chooseWorkerFunctions(pFSMCSwitchOutputGenerator pfsmcswog)
{
	pFSMCOutputGenerator pfsmcog = &pfsmcswog->fsmcog;

	switch (pfsmcog->pcmd->pmi->modFlags & ACTIONS_RETURN_FLAGS)
	{
	case 0: // actions return events
		pfsmcog->wfsm = pfsmcog->pcmd->pmi->parent
			            ? writeSingleSwitchSubFSMLoop
			            : writeOriginalSwitchFSMLoopAre
			            ;
		pfsmcog->cfsmliw = pfsmcog->pcmd->pmi->parent
			               ? writeSingleSwitchFSMLoopInnardsAre
			               : writeSingleSwitchFSMLoopInnardsAre
			               ;
		break;
	case mfActionsReturnStates:
		pfsmcog->wfsm = pfsmcog->pcmd->pmi->parent
			            ? writeSingleSwitchSubFSMLoop
			            : writeOriginalSwitchFSMLoopArs
			            ;
		pfsmcog->cfsmliw = pfsmcog->pcmd->pmi->parent
			               ? writeSingleSwitchFSMLoopInnardsArs
			               : writeSingleSwitchFSMLoopInnardsArs
			               ;
		break;
	case mfActionsReturnVoid:
		pfsmcog->wfsm = pfsmcog->pcmd->pmi->parent
			            ? writeSingleSwitchSubFSMLoop
			            : writeOriginalSwitchFSMLoopArv
			            ;
		pfsmcog->cfsmliw = pfsmcog->pcmd->pmi->parent
			               ? writeSingleSwitchFSMLoopInnardsArv
			               : writeSingleSwitchFSMLoopInnardsArv
			               ;
		break;
	default: //here really only to quiet a warning
		yyerror("invalid actions return statement");
		break;
	}
}

pFSMOutputGenerator generateCSingleSwitchMachineWriter(pFSMOutputGenerator parent)
{
	pFSMOutputGenerator pfsmog;

	if (parent)
	{
		pFSMCSwitchOutputGenerator pfsmcssog = calloc(1, sizeof(FSMCSwitchOutputGenerator));

		pfsmcssog->fsmcog.fsmog.writeMachine = writeCSingleSwitchSubMachine;
		pfsmcssog->fsmcog.fsmog.initOutput   = initCSubMachine;
		pfsmcssog->fsmcog.fsmog.closeOutput  = closeCMachine;
		pfsmcssog->fsmcog.fsmog.fsmogFactory = generateCSingleSwitchMachineWriter;

		pfsmcssog->fsmcog.wconvenience_macros    = standardConvenienceMacros;
		pfsmcssog->fsmcog.wtransition_fn_typedef = empty;
		pfsmcssog->fsmcog.top_level_fsmcog       = (pFSMCOutputGenerator)&CSingleSwitchMachineWriter;
		pfsmcssog->fsmcog.parent_fsmcog          = (pFSMCOutputGenerator) parent;

		pfsmog = (pFSMOutputGenerator)pfsmcssog;
	}
	else
	{
		pfsmog = (pFSMOutputGenerator)&CSingleSwitchMachineWriter;
	}

	if (output_generated_file_names_only)
	{
		pfsmog->initOutput   = parent ? initCSubMachineFN : initCMachineFN;
		pfsmog->writeMachine = writeCMachineFN;
		pfsmog->closeOutput  = closeCMachineFN;
	}

	return pfsmog;
}

static void writeCSingleSwitchMachineInternal(pFSMCSwitchOutputGenerator pfsmcssog)
{
	FSMLANG_DEVELOP_PRINTF(pfsmcssog->fsmcog.pcmd->cFile
						   , "/* FSMLANG_DEVELOP: %s */\n"
						   , __func__
						   );

	pFSMCOutputGenerator pfsmcog = (pFSMCOutputGenerator) pfsmcssog;
	pMACHINE_INFO pmi  = pfsmcog->pcmd->pmi;
	pCMachineData pcmd = pfsmcog->pcmd;

	/* do this now, since some header stuff puts content into the source file.*/
	addNativeImplementationPrologIfThereIsAny(pmi, pcmd->cFile);

	commonHeaderStart(pfsmcog, false);

	defineCSingleSwitchMachineStruct(pcmd);

	cswitchMachineHeaderEnd(pfsmcog, empty_cell_fn != NULL);
	
	/*
	    Source File
	*/

	fprintf(pcmd->cFile
			, "#define COMBINE(E, S) \\\n"
			  "(((unsigned long long)(E) << (sizeof(unsigned long long)/2)\\\n"
			  " | (unsigned long long)(S)))\n"
			);

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
		declareStateEntryAndExitManagers(pcmd, pmi, true);
	}

	defineSubMachineArray(pcmd, pmi);

	if (generate_instance)
	{
		generateInstance(pcmd, pmi, NULL, NULL, false);

		if (generate_run_function)
		{
		   generateRunFunction(pcmd, pmi);
		}

	}
	else
	{
		generateInstanceMacro(pcmd, pmi, NULL, NULL, false);
	}

	defineCSingleSwitchMachineFSM(pfsmcog);

	/* write our sub-machine lookup, if needed */
	if (pmi->machine_list)
	{
	   defineSubMachineFinder(pcmd, pmi);
	}

	defineStateEntryAndExitManagers(pcmd, pmi, true);

	if (pmi->data_block_count)
	{
	   defineEventDataManager(pcmd, pmi);
	}

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

static void writeCSingleSwitchSubMachineInternal(pFSMCSwitchOutputGenerator pfsmcswog)
{
	pFSMCOutputGenerator pfsmcog = (pFSMCOutputGenerator) pfsmcswog;
	pCMachineData        pcmd    = pfsmcog->pcmd;
	pMACHINE_INFO        pmi     = pcmd->pmi;

   /* do this now, since some header stuff puts content into the source file.*/
   addNativeImplementationPrologIfThereIsAny(pmi, pcmd->cFile);

   subMachineHeaderStart(pfsmcog, true);

   defineCSingleSwitchMachineStruct(pcmd);

   cswitchSubMachineHeaderEnd(pfsmcog, empty_cell_fn != NULL);

   /*
     Source File
   */

   fprintf(pcmd->cFile
		   , "#define COMBINE(E, S) ((E << 16) | S)\n"
		   );

   if (pmi->machine_list)
   {
      declareSubMachineManagers(pcmd, pmi);
   }

   defineSubMachineIF(pcmd);

   possiblyDefineSubMachineSharedEventStructures(pcmd, pmi);

   defineSubMachineArray(pcmd, pmi);

   if (generate_instance)
   {
      generateInstance(pcmd, pmi, NULL, NULL, false);
   }
   else
   {
	   generateSubMachineInstanceMacro(pcmd, pmi, NULL, NULL, false);
   }

   defineCSingleSwitchSubMachineFSM(pfsmcog);

   /* write our sub-machine lookup, if needed */
   if (pmi->machine_list)
   {
      defineSubMachineFinder(pcmd, pmi);
   }

   defineStateEntryAndExitManagers(pcmd, pmi, true);

   if (generate_weak_fns)
   {
      defineWeakActionFunctionStubs(pcmd, pmi);

	  /* TODO: sub machine data translators are different
      defineWeakDataTranslatorStubs(pcmd, pmi);
	  */

      defineWeakStateEntryAndExitFunctionStubs(pcmd, pmi);

	  defineWeakUserTransitionFns(pfsmcog);
   }
   else if (force_generation_of_event_passing_actions)
   {
      defineEventPassingActions(pcmd, pmi);
   }

   writeDebugInfo(pcmd, pmi);

	addNativeImplementationEpilogIfThereIsAny(pmi, pcmd->cFile);

}

static void defineCSingleSwitchMachineStruct(pCMachineData pcmd)
{
	pMACHINE_INFO pmi = pcmd->pmi;

	FILE *fout = generate_instance ? pcmd->hFile : pcmd->pubHFile;

	FSMLANG_DEVELOP_PRINTF(fout, "/* FSMLANG_DEVELOP: %s */\n", __func__);

   /* put the machine structure definition into the header file */
   fprintf(fout
		   , "struct _%s_struct_ {\n"
		   , generate_instance ? machineName(pcmd) : fqMachineName(pcmd)
		   );

   if (generate_instance)
   {
	   fprintf(fout
			   , "\t%-*s instance;\n"
			   , (int) pcmd->sub_machine_struct_format_width
			   , "unsigned"
			   );
   }

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

static void defineCSingleSwitchMachineFSM(pFSMCOutputGenerator pfsmcog)
{
	FSMLANG_DEVELOP_PRINTF(pfsmcog->pcmd->cFile
						   , "/* FSMLANG_DEVELOP: %s */\n"
						   , __func__
						   );

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
           , "%s %sFSM(p%s pfsm, %s event)\n{\n"
		   , pfsmcog->parent_fsmcog ? subFsmFnReturnType(pcmd) : "void"
           , pmi->name->name
		   , ucMachineName(pcmd)
		   , fsmFnEventType(pcmd)
           );

   if (!pfsmcog->parent_fsmcog)
   {
	   writeReentrantPrologue(pcmd);
   }

   fprintf(pcmd->cFile
		   , "\t%s e = event%s;\n"
		   , eventType(pcmd)
		   , pmi->data_block_count ? "->event" : ""
		  );

   fprintf(pcmd->cFile
		   , "\t%s s = pfsm->state;\n\n"
		   , stateType(pcmd)
		   );

   if (add_profiling_macros)
   {
	   fprintf(pcmd->cFile, "\n\tFSM_ENTRY(pfsm);\n\n");
   }

   writeFSMLoop(pfsmcog);

   if (add_profiling_macros)
   {
	   fprintf(pcmd->cFile, "\n\tFSM_EXIT(pfsm);\n\n");
   }

   if (!pfsmcog->parent_fsmcog)
   {
	   writeReentrantEpilogue(pcmd);
   }

   if (pmi->modFlags & mfActionsReturnStates)
   {
	   fprintf(pcmd->cFile
			   , "\n#ifdef %s_DEBUG\n"
			   , ucfqMachineName(pcmd)
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

   fprintf(pcmd->cFile
		   , "\n}\n\n"
		   );
   FSMLANG_DEVELOP_PRINTF(pcmd->cFile, "/* EXIT %s */\n", __func__);

}

static void writeCSingleSwitchSubMachine(pFSMOutputGenerator pfsmog, pMACHINE_INFO pmi)
{
	pFSMCSwitchOutputGenerator pfsmcswog = (pFSMCSwitchOutputGenerator) pfsmog;
	pFSMCOutputGenerator       pfsmcog   = (pFSMCOutputGenerator)       pfsmog;

	FSMLANG_DEVELOP_PRINTF(pfsmcswog->fsmcog.pcmd->cFile
						   , "/* FSMLANG_DEVELOP: %s */\n"
						   , __func__
						   );


	pfsmcswog->fsmcog.pcmd->pmi = pmi;

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

	chooseWorkerFunctions(pfsmcswog);

	writeCFilePreambles(&(pfsmcswog->fsmcog));
	writeCSingleSwitchSubMachineInternal(pfsmcswog);

	if (pmi->machine_list)
	{
		write_machines(pmi->machine_list
					   , generateCSingleSwitchMachineWriter
					   , pfsmog
					   );
	}
}

static void writeSingleSwitchFSMLoopInnardsAre(pFSMCOutputGenerator pfsmcog, char *tabstr)
{
	FSMLANG_DEVELOP_PRINTF(pfsmcog->pcmd->cFile
						   , "/* FSMLANG_DEVELOP: %s */\n"
						   , __func__
						   );

	ITERATOR_CALLBACK_HELPER ich = {
		. ih = {
				.fout = pfsmcog->pcmd->cFile
				, .pmi = pfsmcog->pcmd->pmi
				, .str = tabstr
				, .pfn_sub_iterator = print_event_state_case_are
		}
		, .pcmd = pfsmcog->pcmd
	};
	fprintf(pfsmcog->pcmd->cFile
			, "\n%s\tswitch(COMBINE(e, pfsm->state))\n%s\t{\n"
			, tabstr
			, tabstr
			);

	pLIST pconsolidated = consolidate_action_info_list(pfsmcog->pcmd->pmi->action_info_list);
	iterate_list(pconsolidated
				 , print_consolidated_cases_are
				 , &ich
				 );
	free_consolidated_action_info_list(pconsolidated);

	fprintf(pfsmcog->pcmd->cFile
			, "%s\tdefault:\n"
			, tabstr
			);

	fprintf(pfsmcog->pcmd->cFile
			, "%s\t\te = THIS(noEvent);\n"
			, tabstr
			);

	handleEmptyCells(pfsmcog, tabstr);

	fprintf(pfsmcog->pcmd->cFile
			, "%s\t\tbreak;\n\t%s}\n"
			, tabstr
			, tabstr
			);

	setNewState(pfsmcog);

}

static void writeSingleSwitchFSMLoopInnardsArv(pFSMCOutputGenerator pfsmcog, char *tabstr)
{
	FSMLANG_DEVELOP_PRINTF(pfsmcog->pcmd->cFile
						   , "/* FSMLANG_DEVELOP: %s */\n"
						   , __func__
						   );

	ITERATOR_CALLBACK_HELPER ich = {
		. ih = {
				.fout = pfsmcog->pcmd->cFile
				, .pmi = pfsmcog->pcmd->pmi
				, .str = tabstr
				, .pfn_sub_iterator = print_event_state_case_arv
		}
		, .pcmd = pfsmcog->pcmd
	};

	fprintf(pfsmcog->pcmd->cFile
			, "\n%s\tswitch(COMBINE(%s, pfsm->state))\n%s\t{\n"
			, tabstr
			, pfsmcog->parent_fsmcog ? "event" : "e"
			, tabstr
			);

	pLIST pconsolidated = consolidate_action_info_list(pfsmcog->pcmd->pmi->action_info_list);
	iterate_list(pconsolidated
				 , print_consolidated_cases_arv
				 , &ich
				 );
	free_consolidated_action_info_list(pconsolidated);

	fprintf(pfsmcog->pcmd->cFile
			, "%s\tdefault:\n"
			, tabstr
			);

	handleEmptyCells(pfsmcog, tabstr);

	fprintf(pfsmcog->pcmd->cFile
			, "%s\t\tbreak;\n\t%s}\n"
			, tabstr
			, tabstr
			);

	setNewState(pfsmcog);

}

static void writeSingleSwitchFSMLoopInnardsArs(pFSMCOutputGenerator pfsmcog, char *tabstr)
{
	FSMLANG_DEVELOP_PRINTF(pfsmcog->pcmd->cFile
						   , "/* FSMLANG_DEVELOP: %s */\n"
						   , __func__
						   );

	ITERATOR_CALLBACK_HELPER ich = {
		. ih = {
				.fout = pfsmcog->pcmd->cFile
				, .pmi = pfsmcog->pcmd->pmi
				, .str = tabstr
				, .pfn_sub_iterator = print_event_state_case_ars
		}
		, .pcmd = pfsmcog->pcmd
	};

	fprintf(pfsmcog->pcmd->cFile
			, "\n%s\tswitch(COMBINE(%s, pfsm->state))\n%s\t{\n"
			, tabstr
			, pfsmcog->parent_fsmcog ? "event" : "e"
			, tabstr
			);

	pLIST pconsolidated = consolidate_action_info_list(pfsmcog->pcmd->pmi->action_info_list);
	iterate_list(pconsolidated
				 , print_consolidated_cases_ars
				 , &ich
				 );
	free_consolidated_action_info_list(pconsolidated);

	fprintf(pfsmcog->pcmd->cFile
			, "%s\tdefault:\n"
			, tabstr
			);

	handleEmptyCells(pfsmcog, tabstr);

	fprintf(pfsmcog->pcmd->cFile
			, "%s\t\tbreak;\n\t%s}\n"
			, tabstr
			, tabstr
			);

	setNewState(pfsmcog);

}

static bool print_event_state_case_are(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO                  pstate = (pID_INFO) pelem->mbr;
	pITERATOR_CALLBACK_HELPER pich   = (pITERATOR_CALLBACK_HELPER) data;
	pITERATOR_HELPER          pih    = (pITERATOR_HELPER) data;

	pACTION_INFO pai = pih->pmi->actionArray[pih->pid->order][pstate->order];

	if (pai)
	{
		fprintf(pih->fout
				, "%s\tcase COMBINE(THIS(%s),STATE(%s)):\n"
				, pih->str
				, pih->pid->name
				, pstate->name
				);

		if (pai->action && strlen(pai->action->name))
		{
			if (add_profiling_macros)
			{
				fprintf(pih->fout
						, "%s\t\tACTION_ENTRY(pfsm);\n"
						, pih->str
					   );
			}

			fprintf(pih->fout
					, "%s\t\te = UFMN(%s)(pfsm);\n"
					, pih->str
					, pai->action->name
					);

			if (add_profiling_macros)
			{
				fprintf(pih->fout
						, "%s\t\tACTION_EXIT(pfsm);\n"
						, pih->str
					   );
			}
		}
		else
		{
			fprintf(pih->fout
					, "%s\t\te = THIS(noEvent);\n"
					, pih->str
					);

			fprintf(pih->fout
					, "%s\t\t%s(\"%s_noAction\");\n"
					, pih->str
					, core_logging_only ? "NON_CORE_DEBUG_PRINTF" : "DBG_PRINTF"
					, ufMachineName(pich->pcmd)
				   );
		}

		if (pai->transition)
		{
			fprintf(pih->fout
					, "%s\t\ts = "
					, pih->str
					);
			if (pai->transition->type == STATE)
			{
				fprintf(pih->fout
						, "STATE(%s)"
						, pai->transition->name
						);
			}
			else
			{
				fprintf(pih->fout
						, "UFMN(%s)(pfsm,e)"
						, pai->transition->name
						);
			}

			fprintf(pih->fout, ";\n");

		}

		fprintf(pih->fout
				, "%s\t\tbreak;\n"
				, pih->str
				);
	}

	return false;
}

static bool print_event_state_case_arv(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO                  pstate = (pID_INFO) pelem->mbr;
	pITERATOR_CALLBACK_HELPER pich   = (pITERATOR_CALLBACK_HELPER) data;
	pITERATOR_HELPER          pih    = (pITERATOR_HELPER) data;

	pACTION_INFO pai = pih->pmi->actionArray[pih->pid->order][pstate->order];

	if (pai)
	{
		fprintf(pih->fout
				, "%s\tcase COMBINE(THIS(%s),STATE(%s)):\n"
				, pih->str
				, pih->pid->name
				, pstate->name
				);

		if (pai->action && strlen(pai->action->name))
		{
			if (add_profiling_macros)
			{
				fprintf(pih->fout
						, "%s\t\tACTION_ENTRY(pfsm);\n"
						, pih->str
					   );
			}

			fprintf(pih->fout
					, "%s\t\tUFMN(%s)(pfsm);\n"
					, pih->str
					, pai->action->name
					);

			if (add_profiling_macros)
			{
				fprintf(pih->fout
						, "%s\t\tACTION_EXIT(pfsm);\n"
						, pih->str
					   );
			}
		}
		else
		{
			fprintf(pih->fout
					, "%s\t\t%s(\"%s_noAction\");\n"
					, pih->str
					, core_logging_only ? "NON_CORE_DEBUG_PRINTF" : "DBG_PRINTF"
					, ufMachineName(pich->pcmd)
				   );
		}

		if (pai->transition)
		{
			fprintf(pih->fout
					, "%s\t\ts = "
					, pih->str
					);
			if (pai->transition->type == STATE)
			{
				fprintf(pih->fout
						, "STATE(%s)"
						, pai->transition->name
						);
			}
			else
			{
				fprintf(pih->fout
						, "UFMN(%s)(pfsm,e)"
						, pai->transition->name
						);
			}

			fprintf(pih->fout, ";\n");

		}

		fprintf(pih->fout
				, "%s\t\tbreak;\n"
				, pih->str
				);
	}

	return false;
}

static bool print_event_state_case_ars(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO         pstate = (pID_INFO) pelem->mbr;
	pITERATOR_HELPER pih    = (pITERATOR_HELPER) data;

	pACTION_INFO pai = pih->pmi->actionArray[pih->pid->order][pstate->order];

	if (pai)
	{
		fprintf(pih->fout
				, "%s\tcase COMBINE(THIS(%s),STATE(%s)):\n"
				, pih->str
				, pih->pid->name
				, pstate->name
				);

		if (pai->action && strlen(pai->action->name))
		{
			if (add_profiling_macros)
			{
				fprintf(pih->fout
						, "%s\t\tACTION_ENTRY(pfsm);\n"
						, pih->str
					   );
			}

			fprintf(pih->fout
					, "%s\t\ts = UFMN(%s)(pfsm);\n"
					, pih->str
					, pai->action->name
					);

			if (add_profiling_macros)
			{
				fprintf(pih->fout
						, "%s\t\tACTION_EXIT(pfsm);\n"
						, pih->str
					   );
			}
		}

		if (pai->transition)
		{
			fprintf(pih->fout
					, "%s\t\ts = "
					, pih->str
					);
			if (pai->transition->type == STATE)
			{
				fprintf(pih->fout
						, "STATE(%s)"
						, pai->transition->name
						);
			}
			else
			{
				fprintf(pih->fout
						, "UFMN(%s)(pfsm,e)"
						, pai->transition->name
						);
			}

			fprintf(pih->fout, ";\n");

		}

		fprintf(pih->fout
				, "%s\t\tbreak;\n"
				, pih->str
				);
	}

	return false;
}

static void defineCSingleSwitchSubMachineFSM(pFSMCOutputGenerator pfsmcog)
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
	   declareStateEntryAndExitManagers(pcmd, pmi, true);
   }

   fprintf(pcmd->cFile
		   , "%s%s %sFSM(p%s pfsm, %s event)\n{\n"
		   , generate_instance ? "static " : ""
		   , subFsmFnReturnType(pcmd)
		   , generate_instance ? machineName(pcmd) : fqMachineName(pcmd)
		   , fsmType(pcmd)
		   , fsmFnEventType(pcmd)
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

static void writeSingleSwitchSubFSMLoop(pFSMCOutputGenerator pfsmcog)
{
	pCMachineData pcmd = pfsmcog->pcmd;
	pMACHINE_INFO pmi  = pcmd->pmi;

	FSMLANG_DEVELOP_PRINTF(pcmd->cFile, "/* FSMLANG_DEVELOP: %s */\n", __func__);

	if (!(pmi->modFlags & ACTIONS_RETURN_FLAGS))
	{
		fprintf(pcmd->cFile
				, "\t%s e = event;\n"
				, eventType(pcmd)
			   );
	}

	fprintf(pcmd->cFile
			, "\t%s s = pfsm->state;\n"
			, stateType(pcmd)
		   );

	fprintf(pcmd->cFile
			, "\n\t%s ((%s >= THIS(firstEvent))\n\t\t    && (%s < THIS(noEvent))\n\t\t\t)\n\t{\n"
			, pmi->modFlags & ACTIONS_RETURN_FLAGS ? "if" : "while"
			, pmi->modFlags & ACTIONS_RETURN_FLAGS ? "event" : "e"
			, pmi->modFlags & ACTIONS_RETURN_FLAGS ? "event" : "e"
			);

	printFSMSubMachineDebugBlock(pcmd, pmi);

	writeCFSMLoopInnards("\t");

	handleStateTransitionActions(pcmd, pmi, 1);

	if (pmi->modFlags & mfActionsReturnStates)
	{
		fprintf(pcmd->cFile
				, "\n\t\tif (s != STATE(noTransition))\n\t\t"
				  "{\n\t\t\tpfsm->state = s;\n\t\t}\n"
				);
	}
	else
	{
		fprintf(pcmd->cFile
				, "\n\t\tpfsm->state = s;\n"
				);
	}

	fprintf(pcmd->cFile
			, "\t}\n"
			);

	if (pmi->machine_list)
	{
		fprintf(pcmd->cFile
				, "%s\t%sif ((event > THIS(noEvent))\n\t\t&& (event < THIS(lastEvent)))\n\t{\n\t\t"
				, pmi->modFlags & ACTIONS_RETURN_FLAGS ? "" : "\n"
				, pmi->modFlags & ACTIONS_RETURN_FLAGS ? "else " : ""
			   );

		fprintf(pcmd->cFile
				, "%sfindAndRunSubMachine(pfsm, event);\n\t}\n"
				, pmi->submachine_inhibitor_count ? "if (doNotInhibitSubMachines(pfsm->state))\n\t\t\t  " : ""
				);
	}

	if (pfsmcog->parent_fsmcog)
	{
		if (add_profiling_macros && profile_sub_fsms)
		{
			fprintf(pcmd->cFile, "\n\tFSM_EXIT(pfsm);\n\n");
		}
	}

	if (!(pmi->modFlags & ACTIONS_RETURN_FLAGS))
	{
		fprintf(pcmd->cFile
				, "\treturn (e == THIS(noEvent)) ? PARENT(noEvent) : e;\n"
				);
	}
}

static void setNewState(pFSMCOutputGenerator pfsmcog)
{
	pCMachineData pcmd = pfsmcog->pcmd;
	pMACHINE_INFO pmi  = pcmd->pmi;

	handleStateTransitionActions(pcmd, pmi, 2);

	if (pmi->modFlags & mfActionsReturnStates)
	{
		fprintf(pcmd->cFile
				, "\n\t\t\tif (s != STATE(noTransition))\n\t\t\t"
				  "{\n\t\t\t\tpfsm->state = s;\n\t\t\t}\n"
				);
	}
	else
	{
		fprintf(pcmd->cFile
				, "\n\t\t\tpfsm->state = s;\n"
				);
	}
}

static void handleEmptyCells(pFSMCOutputGenerator pfsmcog, char *tabstr)
{
	if (empty_cell_fn)
	{
		fprintf(pfsmcog->pcmd->cFile
				, "%s\t\tUFMN(%s)(pfsm);\n"
				, tabstr
				, empty_cell_fn
				);
	}
	else
	{
		fprintf(pfsmcog->pcmd->cFile
				, "%s\t%s(\"%s_noAction\");\n"
				, tabstr
				, core_logging_only ? "NON_CORE_DEBUG_PRINTF" : "DBG_PRINTF"
				, ufMachineName(pfsmcog->pcmd)
			   );
	}
}

static bool print_consolidated_cases_are(pLIST_ELEMENT pelem, void *data)
{
	pCONSOLIDATED_ACTION_INFO pcai = (pCONSOLIDATED_ACTION_INFO) pelem->mbr;
	pITERATOR_CALLBACK_HELPER pich = (pITERATOR_CALLBACK_HELPER) data;

	iterate_list(pcai->matrices
				 , print_matrices
				 , pich
				 );

	if (pcai->pai->action->name && strlen(pcai->pai->action->name))
	{
		if (add_profiling_macros)
		{
			fprintf(pich->ih.fout
					, "%s\t\tACTION_ENTRY(pfsm);\n"
					, pich->ih.str
				   );
		}

		fprintf(pich->ih.fout
				, "%s\t\te = UFMN(%s)(pfsm);\n"
				, pich->ih.str
				, pcai->pai->action->name
			   );

		if (add_profiling_macros)
		{
			fprintf(pich->ih.fout
					, "%s\t\tACTION_EXIT(pfsm);\n"
					, pich->ih.str
				   );
		}
	}
	else
	{
		fprintf(pich->ih.fout
				, "%s\t\te = THIS(noEvent);\n"
				, pich->ih.str
				);

		fprintf(pich->ih.fout
				, "%s\t\t%s(\"%s_noAction\");\n"
				, pich->ih.str
				, core_logging_only ? "NON_CORE_DEBUG_PRINTF" : "DBG_PRINTF"
				, ufMachineName(pich->pcmd)
			   );
	}

	if (pcai->pai->transition)
	{
		fprintf(pich->ih.fout
				, "%s\t\ts = %s(%s)%s;\n"
				, pich->ih.str
				, pcai->pai->transition->type == STATE ? "STATE" : "UFMN"
				, pcai->pai->transition->name
				, pcai->pai->transition->type == STATE ? "" : "(pfsm,e)"
				);
	}

	fprintf(pich->ih.fout
			, "%s\t\tbreak;\n"
			, pich->ih.str
			);

	return false;
}

static bool print_consolidated_cases_arv(pLIST_ELEMENT pelem, void *data)
{
	pCONSOLIDATED_ACTION_INFO pcai = (pCONSOLIDATED_ACTION_INFO) pelem->mbr;
	pITERATOR_CALLBACK_HELPER pich = (pITERATOR_CALLBACK_HELPER) data;

	iterate_list(pcai->matrices
				 , print_matrices
				 , pich
				 );

	if (pcai->pai->action->name && strlen(pcai->pai->action->name))
	{
		if (add_profiling_macros)
		{
			fprintf(pich->ih.fout
					, "%s\t\tACTION_ENTRY(pfsm);\n"
					, pich->ih.str
				   );
		}

		fprintf(pich->ih.fout
				, "%s\t\tUFMN(%s)(pfsm);\n"
				, pich->ih.str
				, pcai->pai->action->name
			   );

		if (add_profiling_macros)
		{
			fprintf(pich->ih.fout
					, "%s\t\tACTION_EXIT(pfsm);\n"
					, pich->ih.str
				   );
		}
	}
	else
	{
		fprintf(pich->ih.fout
				, "%s\t\t%s(\"%s_noAction\");\n"
				, pich->ih.str
				, core_logging_only ? "NON_CORE_DEBUG_PRINTF" : "DBG_PRINTF"
				, ufMachineName(pich->pcmd)
			   );
	}

	if (pcai->pai->transition)
	{
		fprintf(pich->ih.fout
				, "%s\t\ts = %s(%s)%s;\n"
				, pich->ih.str
				, pcai->pai->transition->type == STATE ? "STATE" : "UFMN"
				, pcai->pai->transition->name
				, pcai->pai->transition->type == STATE ? "" : "(pfsm,e)"
				);
	}

	fprintf(pich->ih.fout
			, "%s\t\tbreak;\n"
			, pich->ih.str
			);

	return false;
}

static bool print_consolidated_cases_ars(pLIST_ELEMENT pelem, void *data)
{
	pCONSOLIDATED_ACTION_INFO pcai = (pCONSOLIDATED_ACTION_INFO) pelem->mbr;
	pITERATOR_CALLBACK_HELPER pich = (pITERATOR_CALLBACK_HELPER) data;

	iterate_list(pcai->matrices
				 , print_matrices
				 , pich
				 );

	if (pcai->pai->action->name && strlen(pcai->pai->action->name))
	{
		if (add_profiling_macros)
		{
			fprintf(pich->ih.fout
					, "%s\t\tACTION_ENTRY(pfsm);\n"
					, pich->ih.str
				   );
		}

		fprintf(pich->ih.fout
				, "%s\t\ts = UFMN(%s)(pfsm);\n"
				, pich->ih.str
				, pcai->pai->action->name
			   );

		if (add_profiling_macros)
		{
			fprintf(pich->ih.fout
					, "%s\t\tACTION_EXIT(pfsm);\n"
					, pich->ih.str
				   );
		}
	}

	if (pcai->pai->transition)
	{
		fprintf(pich->ih.fout
				, "%s\t\ts = %s(%s)%s;\n"
				, pich->ih.str
				, pcai->pai->transition->type == STATE ? "STATE" : "UFMN"
				, pcai->pai->transition->name
				, pcai->pai->transition->type == STATE ? "" : "(pfsm)"
				);
	}

	fprintf(pich->ih.fout
			, "%s\t\tbreak;\n"
			, pich->ih.str
			);

	return false;
}

static bool print_matrices(pLIST_ELEMENT pelem, void *data)
{
	pMATRIX_INFO              pmi  = (pMATRIX_INFO) pelem->mbr;
	pITERATOR_CALLBACK_HELPER pich = (pITERATOR_CALLBACK_HELPER) data;

	pich->pOtherElem = pelem;
	iterate_list(pmi->event_list
				 , print_matrix_event_cases
				 , pich
				 );

	return false;
}

static bool print_matrix_event_cases(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO                  pevent = (pID_INFO) pelem->mbr;
	pITERATOR_CALLBACK_HELPER pich   = (pITERATOR_CALLBACK_HELPER) data;
	pMATRIX_INFO              pmi    = (pMATRIX_INFO) pich->pOtherElem->mbr;

	pich->ih.pid = pevent;
	iterate_list(pmi->state_list
				 , print_matrix_case
				 , pich
				 );

	return false;
}

static bool print_matrix_case(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO                  pstate = (pID_INFO) pelem->mbr;
	pITERATOR_CALLBACK_HELPER pich   = (pITERATOR_CALLBACK_HELPER) data;

	fprintf(pich->ih.fout
			, "%s\tcase COMBINE(THIS(%s),STATE(%s)):\n"
			, pich->ih.str
			, pich->ih.pid->name
			, pstate->name
			);

	return false;
}

