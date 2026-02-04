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
static void chooseWorkerFunctions(pFSMCSwitchOutputGenerator);
static void writeSingleSwitchFSMLoopInnardsAre(pFSMCOutputGenerator,char*);
static void writeSingleSwitchSubFSMLoopInnardsAre(pFSMCOutputGenerator,char*);
static void defineCSingleSwitchMachineStruct(pCMachineData);
static void defineCSingleSwitchMachineFSM(pFSMCOutputGenerator);

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
			            ? writeOriginalSwitchSubFSMLoopAre
			            : writeOriginalSwitchFSMLoopAre
			            ;
		pfsmcog->cfsmliw = pfsmcog->pcmd->pmi->parent
			              ? writeSingleSwitchFSMLoopInnardsAre
			              : writeSingleSwitchSubFSMLoopInnardsAre
			              ;
		break;
	case mfActionsReturnStates:
		break;
	case mfActionsReturnVoid:
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

	ITERATOR_CALLBACK_HELPER ich = {
		.ih = {
			.pmi = pfsmcog->pcmd->pmi
		}
		, .pcmd = pfsmcog->pcmd
	};

	/* do this now, since some header stuff puts content into the source file.*/
	addNativeImplementationPrologIfThereIsAny(pmi, pcmd->cFile);

	commonHeaderStart(pfsmcog, false);

	defineCSingleSwitchMachineStruct(pcmd);

	cswitchMachineHeaderEnd(pfsmcog, empty_cell_fn != NULL);
	
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

}

static void writeCSingleSwitchSubMachine(pFSMOutputGenerator pfsmog, pMACHINE_INFO pmi)
{
	pFSMCSwitchOutputGenerator pfsmcssog = (pFSMCSwitchOutputGenerator) pfsmog;

	FSMLANG_DEVELOP_PRINTF(pfsmcssog->fsmcog.pcmd->cFile
						   , "/* FSMLANG_DEVELOP: %s */\n"
						   , __func__
						   );

}

static void writeSingleSwitchFSMLoopInnardsAre(pFSMCOutputGenerator pfsmcog, char *tabstr)
{
	FSMLANG_DEVELOP_PRINTF(pfsmcog->pcmd->cFile
						   , "/* FSMLANG_DEVELOP: %s */\n"
						   , __func__
						   );

}

static void writeSingleSwitchSubFSMLoopInnardsAre(pFSMCOutputGenerator pfsmcog, char *tabstr)
{
	FSMLANG_DEVELOP_PRINTF(pfsmcog->pcmd->cFile
						   , "/* FSMLANG_DEVELOP: %s */\n"
						   , __func__
						   );

}

