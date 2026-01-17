/**
*  fsm_rst.c
*
*    Output restructured text
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

#include "fsm_rst.h"
#include "y.tab.h"
#include "event_sequences.h"
#include "util_file_inclusion.h"

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

bool include_uml_objects = false;

//!< A must be pFSMRSTOutputGenerator
#define FOUT(A) (A)->pmd->rstFile

//!< A must be pFSMRSTOutputGenerator
#define PMI(A) (A)->pmd->pmi

//!< A must be pFSMRSTOutputGenerator
#define PMD(A) (A)->pmd

typedef struct _fsm_rst_output_generator_ FSMRSTOutputGenerator, *pFSMRSTOutputGenerator;
typedef struct _rst_machine_data_ RSTMachineData, *pRSTMachineData;
typedef struct _rst_iterator_helper_ RST_ITERATOR_HELPER, *pRST_ITERATOR_HELPER;

static int initRSTWriter(pFSMOutputGenerator,char *);
static void writeRSTWriter(pFSMOutputGenerator,pMACHINE_INFO);
static void closeRSTWriter(pFSMOutputGenerator,int);

static int initRSTFileNameWriter(pFSMOutputGenerator,char *);
static void writeRSTFileName(pFSMOutputGenerator,pMACHINE_INFO);
static void closeRSTFileNameWriter(pFSMOutputGenerator,int);

static void print_toctree(pFSMRSTOutputGenerator);
static bool print_sub_machine_toctree_entry(pLIST_ELEMENT,void*);
static void print_actions(pFSMRSTOutputGenerator);
static void print_states(pFSMRSTOutputGenerator);
static void print_events(pFSMRSTOutputGenerator);
static bool print_transition_fn_data(pLIST_ELEMENT,void*);
static bool print_action_data(pLIST_ELEMENT,void*);
static bool print_state_data(pLIST_ELEMENT,void*);
static bool print_event_data(pLIST_ELEMENT,void*);
static void print_id_info_data(pID_INFO,FILE*);
static void print_state_chart(pFSMRSTOutputGenerator);
static bool print_state_chart_event_header_row(pLIST_ELEMENT,void*);
static bool print_state_chart_state_row(pLIST_ELEMENT,void*);
static bool print_state_chart_state_row_event(pLIST_ELEMENT,void*);
static void print_nchar(FILE*,char,size_t);
static void print_reference_anchor(FILE*,char*,pRSTMachineData);
static void print_machine_statistics(pFSMRSTOutputGenerator);
static void print_transition_fns(pFSMRSTOutputGenerator);
static void print_event_sequences(pFSMRSTOutputGenerator);
static void eat_spaces(FILE*,char*);
static bool print_pid_as_reference_in_list(pLIST_ELEMENT,void*);
static bool print_pmi_in_list(pLIST_ELEMENT,void*);
static bool print_sequence_svg(pLIST_ELEMENT,void*);
static bool print_sequence(pLIST_ELEMENT,void*);
static bool print_sequence_node_rst(pLIST_ELEMENT,void*);

static char * fqMachineName(pRSTMachineData);
static char * fqMachineNamePMI(pMACHINE_INFO);
static void   stream_multiline(FILE*,unsigned,char*);
static void   pad(FILE*,unsigned);

struct _rst_machine_data_ {

	FILE	      *rstFile;
	char	      *rstName;
    char          *baseName;
	char          *fqmn;
    pMACHINE_INFO pmi;
};

struct _fsm_rst_output_generator_
{
   FSMOutputGenerator     fsmog;
   pRSTMachineData        pmd;
   pFSMRSTOutputGenerator parent_fsmrstog;
};

struct _rst_iterator_helper_
{
	ITERATOR_HELPER        ih;
	pFSMRSTOutputGenerator pfsmrstog;
};

static FSMRSTOutputGenerator RSTMachineWriter = {
	{
     initRSTWriter,
     writeRSTWriter,
     closeRSTWriter,
	   generateRSTMachineWriter 
  },
  NULL,
  NULL
};

static char *indent = "   ";

pFSMOutputGenerator generateRSTMachineWriter(pFSMOutputGenerator parent)
{
	pFSMOutputGenerator pfsmog;

	if (parent)
	{
		pFSMRSTOutputGenerator pfsmrstog = calloc(1, sizeof(FSMRSTOutputGenerator));

		pfsmrstog->fsmog.writeMachine = writeRSTWriter;
		pfsmrstog->fsmog.initOutput   = initRSTWriter;
		pfsmrstog->fsmog.closeOutput  = closeRSTWriter;

		pfsmrstog->parent_fsmrstog    = (pFSMRSTOutputGenerator) parent;

		pfsmog = (pFSMOutputGenerator)pfsmrstog;
	}
	else
	{
		pfsmog = (pFSMOutputGenerator)&RSTMachineWriter;
	}

	if (output_generated_file_names_only)
	{
		pfsmog->writeMachine = writeRSTFileName;
		pfsmog->initOutput   = initRSTFileNameWriter;
		pfsmog->closeOutput  = closeRSTFileNameWriter;
	}

	return pfsmog;
}

static int initRSTFileNameWriter (pFSMOutputGenerator pfsmog, char *baseFileName)
{
	pFSMRSTOutputGenerator pfsmrstog = (pFSMRSTOutputGenerator) pfsmog;
	if (NULL != (pfsmrstog->pmd = calloc(1, sizeof(RSTMachineData))))
	{
		if (baseFileName)
		{
			pfsmrstog->pmd->rstName = createFileName(baseFileName,".rst");
		}
	}

	/* this may look funny, but it does the trick */
	return ((int) !pfsmrstog->pmd->rstName);
}

static void writeRSTFileName(pFSMOutputGenerator pfsmog, pMACHINE_INFO pmi)
{
	pFSMRSTOutputGenerator pfsmrstog = (pFSMRSTOutputGenerator)pfsmog;

	if (output_make_recipe && !pmi->parent)
	{
		printf("%s: %s.fsm "
			   , pfsmrstog->pmd->rstName
			   , inputFileName
			   );

		print_included_files_list();

		printf("\n");
	}

	if (!output_make_recipe || pmi->parent)
	{
		printf("%s ", pfsmrstog->pmd->rstName);
	}

	if (pmi->machine_list)
	{
		write_machines(pmi->machine_list, generateRSTMachineWriter, pfsmog);
	}

	if (output_make_recipe && !pmi->parent && pmi->machine_list)
	{
		printf(": %s\n"
			   , pfsmrstog->pmd->rstName
			   );
	}
}

static void closeRSTFileNameWriter(pFSMOutputGenerator pfsmog, int good)
{
	pFSMRSTOutputGenerator pfsmrstog = (pFSMRSTOutputGenerator) pfsmog;

	(void) good;

	if (!pfsmrstog || !pfsmrstog->pmd)
	{
		return;
	}

	CHECK_AND_FREE(pfsmrstog->pmd->rstName);

	free(pfsmrstog->pmd);
}

static int initRSTWriter (pFSMOutputGenerator pfsmog, char *baseFileName)
{

  pFSMRSTOutputGenerator pfsmrstog = (pFSMRSTOutputGenerator) pfsmog;

  if (NULL != (pfsmrstog->pmd = calloc(1, sizeof(RSTMachineData))))
  {
     if (!baseFileName)
     {

       FOUT(pfsmrstog) = stdout;

     }
     else {

       pfsmrstog->pmd->rstName = createFileName(baseFileName,".rst");
       pfsmrstog->pmd->baseName = strdup(baseFileName);

       if (!(FOUT(pfsmrstog) = openFile(pfsmrstog->pmd->rstName,"w"))) {

         CHECK_AND_FREE(pfsmrstog->pmd->rstName);

       }
       else {

         /* we're good to go; write the preamble */
         fprintf(FOUT(pfsmrstog)
				 ,"..\n%s%s\n\n"
				 , indent
				 ,pfsmrstog->pmd->rstName
				 );
         fprintf(FOUT(pfsmrstog)
				 ,"%sThis file automatically generated by FSMLang\n\n"
				 , indent
				 );

       }
     }
  }

	/* this may look funny, but it does the trick */
	return ((int) !FOUT(pfsmrstog));

}

static void writeRSTWriter(pFSMOutputGenerator pfsmog, pMACHINE_INFO pmi)
{

	RETURN_IF_NULL(pmi);

	pFSMRSTOutputGenerator pfsmrstog = (pFSMRSTOutputGenerator)pfsmog;

	PMI(pfsmrstog) = pmi;

	int title_decoration_len = strlen(pmi->name->name) + 1;

	print_nchar(FOUT(pfsmrstog), '=', title_decoration_len);

	fprintf(FOUT(pfsmrstog)
			, "\n%s\n"
			, pmi->name->name
			);

	print_nchar(FOUT(pfsmrstog), '=', title_decoration_len);

	if (pmi->name->docCmnt)
	{
		fprintf(FOUT(pfsmrstog), "\n\n");
		eat_spaces(FOUT(pfsmrstog), pmi->name->docCmnt);
		fprintf(FOUT(pfsmrstog), "\n\n");
	}

	fprintf(FOUT(pfsmrstog)
			, "\n"
			);

	if (pmi->machine_list)
	{
		print_toctree(pfsmrstog);
		write_machines(pmi->machine_list, generateRSTMachineWriter, pfsmog);
	}

	print_machine_statistics(pfsmrstog);

	if (include_svg_img)
	{
		fprintf(FOUT(pfsmrstog)
				, ".. image:: %s.svg\n%s:alt: PlantUML diagram separately generated.\n%s:class: plantuml\n\n"
				, pfsmrstog->pmd->baseName
				, indent
				, indent
			   );
	}
	else if (include_uml_objects)
	{
		fprintf(FOUT(pfsmrstog)
				, ".. uml:: %s.plantuml\n%s:width: 100%%\n\n"
				, pfsmrstog->pmd->baseName
				, indent
			   );
	}

	if (PMI(pfsmrstog)->data)
	{
		fprintf(FOUT(pfsmrstog)
				, "\n\nMachine data:\n\n.. code:: c\n\n"
				);

		ITERATOR_HELPER ih = {
			. fout = FOUT(pfsmrstog)
			, .tab_level = 1
		};

		iterate_list(PMI(pfsmrstog)->data
					 , print_data_field
					 , &ih
					 );
	}

	print_state_chart(pfsmrstog);
	print_events(pfsmrstog);
	print_states(pfsmrstog);
	print_actions(pfsmrstog);

	if (pmi->transition_fn_list->count)
	{
		print_transition_fns(pfsmrstog);
	}

	if (pmi->sequences)
	{
		print_event_sequences(pfsmrstog);
	}


}

static void closeRSTWriter(pFSMOutputGenerator pfsmog, int good)
{
	pFSMRSTOutputGenerator pfsmrstog = (pFSMRSTOutputGenerator) pfsmog;

	if (!pfsmrstog || !pfsmrstog->pmd)
	{
		return;
	}

	fclose(FOUT(pfsmrstog));

	if (!good)
	{
		unlink(pfsmrstog->pmd->rstName);
	}

	CHECK_AND_FREE(pfsmrstog->pmd->rstName);

	free(pfsmrstog->pmd);
}

static void print_toctree(pFSMRSTOutputGenerator pfsmrstog)
{
	fprintf(FOUT(pfsmrstog)
			, "\n\n.. toctree::\n%s:hidden:\n%s:maxdepth: 2\n\n"
			, indent
			, indent
			);

	iterate_list(PMI(pfsmrstog)->machine_list
				 , print_sub_machine_toctree_entry
				 , pfsmrstog
				 );
}

static bool print_sub_machine_toctree_entry(pLIST_ELEMENT pelem, void *data)
{
	pMACHINE_INFO          pmi       = (pMACHINE_INFO) pelem->mbr;
	pFSMRSTOutputGenerator pfsmrstog = (pFSMRSTOutputGenerator) data;

	fprintf(FOUT(pfsmrstog)
			, "%s%s\n"
			, indent
			, pmi->name->name
			);

	return false;
}


static void print_actions(pFSMRSTOutputGenerator pfsmrstog)
{
	fprintf(FOUT(pfsmrstog)
			, "\n\nActions\n--------\n"
			);

	iterate_list(PMI(pfsmrstog)->action_list
				 , print_action_data
				 , pfsmrstog
				 );
}

static void print_states(pFSMRSTOutputGenerator pfsmrstog)
{
	fprintf(FOUT(pfsmrstog)
			, "\n\nStates\n-------\n"
			);

	iterate_list(PMI(pfsmrstog)->state_list
				 , print_state_data
				 , pfsmrstog
				 );
}

static void print_events(pFSMRSTOutputGenerator pfsmrstog)
{
	fprintf(FOUT(pfsmrstog)
			, "\n\nEvents\n-------\n"
			);

	iterate_list(PMI(pfsmrstog)->event_list
				 , print_event_data
				 , pfsmrstog
				 );
}

static void print_transition_fns(pFSMRSTOutputGenerator pfsmrstog)
{
	fprintf(FOUT(pfsmrstog)
			, "\n\nTransition Functions\n---------------------\n"
			);

	iterate_list(PMI(pfsmrstog)->transition_fn_list
				 , print_transition_fn_data
				 , pfsmrstog
				 );
}

static void print_event_sequences(pFSMRSTOutputGenerator pfsmrstog)
{
	fprintf(FOUT(pfsmrstog)
			, "\n\nEvent Sequences\n---------------------\n"
			);

	if (!include_svg_img)
	{

		fprintf(FOUT(pfsmrstog)
				, "\n.. list-table::\n%s:align: left\n%s:header-rows: 1\n%s:stub-columns: 1\n"
				, indent
				, indent
				, indent
				);

		fprintf(FOUT(pfsmrstog)
				, "\n%s* - Name\n%s  - Comment\n%s  - Initial State\n"
				, indent
				, indent
				, indent
				);

		fprintf(FOUT(pfsmrstog)
				, "%s  - Sequence\n%s  - Final State\n"
				, indent
				, indent
				);

	}

	iterate_list(PMI(pfsmrstog)->sequences
				 , include_svg_img ? print_sequence_svg : print_sequence
				 , pfsmrstog
				 );
}

static void print_id_info_data(pID_INFO pid, FILE *fout)
{
	if (strlen(pid->name))
	{
		fprintf(fout
				, "\n%s\n"
				, pid->name
				);

		print_nchar(fout, '~', strlen(pid->name) + 1);

		if (pid->docCmnt)
		{
			fprintf(fout, "\n\n");
			eat_spaces(fout, pid->docCmnt);
			fprintf(fout, "\n\n");
		}
	}
}

static bool print_transition_fn_data(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO               ptransition_fn   = (pID_INFO) pelem->mbr;
	pFSMRSTOutputGenerator pfsmrstog = (pFSMRSTOutputGenerator) data;

	print_reference_anchor(FOUT(pfsmrstog)
						   , ptransition_fn->name
						   , PMD(pfsmrstog)
						   );

	print_id_info_data(ptransition_fn, FOUT(pfsmrstog));

	if (ptransition_fn->transition_fn_returns_decl)
	{
		fprintf(FOUT(pfsmrstog)
				, "\n\nReturns:\n\n"
				);

		iterate_list(ptransition_fn->transition_fn_returns_decl
					 , print_pid_as_reference_in_list
					 , pfsmrstog
					 );
	}

	return false;
}

static bool print_action_data(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO               paction   = (pID_INFO) pelem->mbr;
	pFSMRSTOutputGenerator pfsmrstog = (pFSMRSTOutputGenerator) data;
	pACTION_DATA           pdata     = &paction->type_data.action_data;

	print_reference_anchor(FOUT(pfsmrstog), paction->name, PMD(pfsmrstog));

	print_id_info_data(paction, FOUT(pfsmrstog));

	if (pdata->action_returns_decl)
	{
		fprintf(FOUT(pfsmrstog)
				, "\n\nThis action returns:\n\n"
				);

		iterate_list(pdata->action_returns_decl
					 , print_pid_as_reference_in_list
					 , pfsmrstog
					 );

		fprintf(FOUT(pfsmrstog), "\n");
	}

	return false;
}

static bool print_state_data(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO               pstate    = (pID_INFO) pelem->mbr;
	pFSMRSTOutputGenerator pfsmrstog = (pFSMRSTOutputGenerator) data;
	pSTATE_DATA            pdata     = &pstate->type_data.state_data;

	print_reference_anchor(FOUT(pfsmrstog), pstate->name, PMD(pfsmrstog));

	print_id_info_data(pstate, FOUT(pfsmrstog));

	if (pdata->state_flags & sfInibitSubMachines)
	{
		fprintf(FOUT(pfsmrstog)
				, "\n\nThis state prohibits sub machines from running.\n\n"
				);

	}

	if (pdata->entry_fn)
	{
		fprintf(FOUT(pfsmrstog)
				, "\n\n%s is called on entry.\n\n"
				, pdata->entry_fn->name
				);
	}

	if (pdata->exit_fn)
	{
		fprintf(FOUT(pfsmrstog)
				, "\n\n%s is called on exit.\n\n"
				, pdata->exit_fn->name
				);
	}

	if (pdata->pevents_handled->count)
	{
		fprintf(FOUT(pfsmrstog)
				, "\n\nThese events are handled in this state:\n\n"
				);

		iterate_list(pdata->pevents_handled
					 , print_pid_as_reference_in_list
					 , pfsmrstog
					 );

		fprintf(FOUT(pfsmrstog)
				, "\n\nThis yields an event density of %u%%.\n\n"
				, pdata->event_density_pct
				);

	}
	else
	{
		fprintf(FOUT(pfsmrstog)
				, "\n\n.. attention::\n\n   This state handles no events.\n\n"
				);

	}

	if (pdata->pactions_list->count)
	{
		fprintf(FOUT(pfsmrstog)
				, "\n\nThese actions are taken in this state:\n\n"
				);

		iterate_list(pdata->pactions_list
					 , print_pid_as_reference_in_list
					 , pfsmrstog
					 );

	}
	else
	{
		if (pdata->pevents_handled->count)
		{
			fprintf(FOUT(pfsmrstog)
					, "\n\n.. warning:\n\n   No actions are taken in this state.\n\n"
				   );
		}

	}

	if (pdata->pinbound_transitions->count)
	{
		fprintf(FOUT(pfsmrstog)
				, "\n\nThese states transition into this state:\n\n"
				);

		iterate_list(pdata->pinbound_transitions
					 , print_pid_as_reference_in_list
					 , pfsmrstog
					 );

	}
	else
	{
		fprintf(FOUT(pfsmrstog)
				, "\n\n.. warning::\n\n   There is no way into this state.\n\n"
				);

	}

	if (pdata->poutbound_transitions->count)
	{
		fprintf(FOUT(pfsmrstog)
				, "\n\nThis state transitions into these states:\n\n"
				);

		iterate_list(pdata->poutbound_transitions
					 , print_pid_as_reference_in_list
					 , pfsmrstog
					 );

	}
	else
	{
		fprintf(FOUT(pfsmrstog)
				, "\n\n.. warning::\n\n   There is no way out of this state.\n\n"
				);
	}


	return false;
}

static bool print_event_data(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO               pevent    = (pID_INFO) pelem->mbr;
	pFSMRSTOutputGenerator pfsmrstog = (pFSMRSTOutputGenerator) data;
	pEVENT_DATA            pdata     = &pevent->type_data.event_data;

	print_reference_anchor(FOUT(pfsmrstog), pevent->name, PMD(pfsmrstog));

	print_id_info_data(pevent, FOUT(pfsmrstog));

	if (pdata->shared_with_parent)
	{
		fprintf(FOUT(pfsmrstog)
				, "\n\nThis event is shared from the parent machine.\n\n"
				);

	}

	if (pdata->single_pai_for_all_states)
	{
		fprintf(FOUT(pfsmrstog)
				, "\n\nThis event is handled identically in all states.\n\n"
				);

	}
	else if (pdata->single_pai_state_count)
	{
		fprintf(FOUT(pfsmrstog)
				, "\n\nThis event is handled identically in %u states.\n\n"
				, pdata->single_pai_state_count
				);

	}

	if (pdata->externalDesignation)
	{
		fprintf(FOUT(pfsmrstog)
				, "\n\nThe enumeration value of this event is defined externally as %s.\n\n"
				, pdata->externalDesignation->name
				);

	}

	if (pdata->psharing_sub_machines)
	{
		fprintf(FOUT(pfsmrstog)
				, "\n\nThese sub-machines share this event:\n\n"
				);

		iterate_list(pdata->psharing_sub_machines
					 , print_pmi_in_list
					 , pfsmrstog
					 );

	}

	if (pdata->phandling_states->count)
	{
		fprintf(FOUT(pfsmrstog)
				, "\n\nThese states handle this event:\n\n"
				);

		iterate_list(pdata->phandling_states
					 , print_pid_as_reference_in_list
					 , pfsmrstog
					 );

		fprintf(FOUT(pfsmrstog)
				, "\n\nThis yields a state density of %u%%.\n\n"
				, pdata->state_density_pct
				);

	}
	else
	{
		if (!pdata->psharing_sub_machines)
		{
			fprintf(FOUT(pfsmrstog)
					, "\n\n.. warning::\n\n   This event is handled in no state.\n\n"
				   );
		}

	}

	if (pdata->pactions_list->count)
	{
		fprintf(FOUT(pfsmrstog)
				, "\n\nThese actions are taken in response to this event:\n\n"
				);

		iterate_list(pdata->pactions_list
					 , print_pid_as_reference_in_list
					 , pfsmrstog
					 );
	}
	else
	{
		if (pdata->phandling_states->count)
		{
			fprintf(FOUT(pfsmrstog)
					, "\n\nNo actions are taken in response to this event.\n\n"
				   );
		}

	}

	if (pdata->puser_event_data && pdata->puser_event_data->data_fields)
	{
		fprintf(FOUT(pfsmrstog)
				, "\n\nEvent data:\n\n.. code:: c\n\n"
				);

		ITERATOR_HELPER ih = {
			.fout = FOUT(pfsmrstog)
			, .tab_level = 1
		};

		iterate_list(pdata->puser_event_data->data_fields
					 , print_data_field
					 , &ih
					 );

	}

	return false;
}

static void print_state_chart(pFSMRSTOutputGenerator pfsmrstog)
{
	fprintf(FOUT(pfsmrstog)
			, "\n.. list-table:: State Chart\n%s:align: left\n%s:header-rows: 1\n%s:stub-columns: 1\n%s:class: scrollable\n"
			, indent
			, indent
			, indent
			, indent
			);

	fprintf(FOUT(pfsmrstog)
			, "\n%s* -\n"
			, indent
			);

	iterate_list(PMI(pfsmrstog)->event_list
				 , print_state_chart_event_header_row
				 , pfsmrstog
				 );


	iterate_list(PMI(pfsmrstog)->state_list
				 , print_state_chart_state_row
				 , pfsmrstog
				 );


}

static bool print_state_chart_event_header_row(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO               pevent    = (pID_INFO) pelem->mbr;
	pFSMRSTOutputGenerator pfsmrstog = (pFSMRSTOutputGenerator) data;

	fprintf(FOUT(pfsmrstog)
			, "%s  - :ref:`%s <%s.%s>`\n"
			, indent
			, pevent->name
			, fqMachineName(pfsmrstog->pmd)
			, pevent->name
			);

	return false;
}

char *fqMachineName(pRSTMachineData pcmd)
{
	FILE          *tmp;

	if (!pcmd->fqmn)
	{
		/* use a temporary file to exploit streaming function, avoiding messy strlen calc */
		if (NULL != (tmp = tmpfile()))
		{
			printAncestry(pcmd->pmi, tmp, ".", alc_lower, ai_include_self);

			pcmd->fqmn = create_string_from_file(tmp, NULL);

		}
	}

	return pcmd->fqmn;
}

char *fqMachineNamePMI(pMACHINE_INFO pmi)
{
	FILE *tmp;
	char *str = NULL;

	/* use a temporary file to exploit streaming function, avoiding messy strlen calc */
	if (NULL != (tmp = tmpfile()))
	{
		printAncestry(pmi, tmp, ".", alc_lower, ai_include_self);

		str = create_string_from_file(tmp, NULL);
	}

	return str;
}

static bool print_state_chart_state_row(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO               pstate    = (pID_INFO) pelem->mbr;
	pFSMRSTOutputGenerator pfsmrstog = (pFSMRSTOutputGenerator) data;

	RST_ITERATOR_HELPER    rstih = {
		.ih = {
			.state = pelem->ordinal
		}
		, .pfsmrstog = pfsmrstog
	};

	fprintf(FOUT(pfsmrstog)
			, "%s* - :ref:`%s <%s.%s>`\n"
			, indent
			, pstate->name
			, fqMachineName(PMD(pfsmrstog))
			, pstate->name
			);

	iterate_list(PMI(pfsmrstog)->event_list
				 , print_state_chart_state_row_event
				 , &rstih
				 );

	return false;
}

static bool print_state_chart_state_row_event(pLIST_ELEMENT pelem, void *data)
{
	pRST_ITERATOR_HELPER prstih = (pRST_ITERATOR_HELPER) data;
	pACTION_INFO         pai    = PMI(prstih->pfsmrstog)->actionArray[pelem->ordinal][prstih->ih.state];

	fprintf(FOUT(prstih->pfsmrstog)
			, "%s  - "
			, indent
			);

	if (pai)
	{
		if (strlen(pai->action->name))
		{
			fprintf(FOUT(prstih->pfsmrstog)
					, "%s:ref:`%s <%s.%s>`\n"
					, pai->transition ? "| " : ""
					, pai->action->name
					, fqMachineName(PMD(prstih->pfsmrstog))
					, pai->action->name
					);
		}

		if (pai->transition)
		{
			fprintf(FOUT(prstih->pfsmrstog)
					, "%stransition: :ref:`%s <%s.%s>`\n"
					, strlen(pai->action->name) ? "       | " : ""
					, pai->transition->name
					, fqMachineName(PMD(prstih->pfsmrstog))
					, pai->transition->name
					);
		}
	}
	else
	{
		fprintf(FOUT(prstih->pfsmrstog), "\n");
	}

	return false;
}

static void print_nchar(FILE *fout, char to_print, size_t how_many)
{
	while (how_many--)
	{
		fputc(to_print, fout);
	}
}

static void print_reference_anchor(FILE *fout, char *name, pRSTMachineData pmd)
{
	if (strlen(name))
	{
		fprintf(fout
				, "\n\n.. _%s.%s:\n"
				, fqMachineName(pmd)
				, name
				);
	}
}

static void print_machine_statistics(pFSMRSTOutputGenerator pfsmrstog)
{

	fprintf(FOUT(pfsmrstog)
			, "\n.. list-table:: Machine Statistics\n%s:align: left\n\n"
			, indent
			);


	fprintf(FOUT(pfsmrstog)
			, "%s* - Number of events:\n%s  - %u\n"
			, indent
			, indent
			, PMI(pfsmrstog)->event_list->count
			);

	fprintf(FOUT(pfsmrstog)
			, "%s* - Events not handled:\n%s  - %u\n"
			, indent
			, indent
			, PMI(pfsmrstog)->events_with_zero_handlers
			);

	fprintf(FOUT(pfsmrstog)
			, "%s* - Events handled in one state:\n%s  - %u\n"
			, indent
			, indent
			, PMI(pfsmrstog)->events_with_one_handler
			);

	fprintf(FOUT(pfsmrstog)
			, "%s* - At least one event handled the same in all states?\n%s  - %s\n"
			, indent
			, indent
			, PMI(pfsmrstog)->has_single_pai_events ? "yes" : "no"
			);

	fprintf(FOUT(pfsmrstog)
			, "%s* - Number of states:\n%s  - %u\n"
			, indent
			, indent
			, PMI(pfsmrstog)->state_list->count
			);

	fprintf(FOUT(pfsmrstog)
			, "%s* - Number of states with entry functions:\n%s  - %u\n"
			, indent
			, indent
			, PMI(pfsmrstog)->states_with_exit_fns_count
			);

	fprintf(FOUT(pfsmrstog)
			, "%s* - Number of states with exit functions:\n%s  - %u\n"
			, indent
			, indent
			, PMI(pfsmrstog)->states_with_exit_fns_count
			);

	fprintf(FOUT(pfsmrstog)
			, "%s* - States handling no events:\n%s  - %u\n"
			, indent
			, indent
			, PMI(pfsmrstog)->states_with_zero_events
			);

	fprintf(FOUT(pfsmrstog)
			, "%s* - States handling exactly one event:\n%s  - %u\n"
			, indent
			, indent
			, PMI(pfsmrstog)->states_with_one_event
			);

	fprintf(FOUT(pfsmrstog)
			, "%s* - States with no way in:\n%s  - %u\n"
			, indent
			, indent
			, PMI(pfsmrstog)->states_with_no_way_in
			);

	fprintf(FOUT(pfsmrstog)
			, "%s* - States with no way out:\n%s  - %u\n"
			, indent
			, indent
			, PMI(pfsmrstog)->states_with_no_way_out
			);

	fprintf(FOUT(pfsmrstog)
			, "\n\n"
			);


}

static void eat_spaces(FILE *fout, char *str)
{
	enum {printing, skipping, check_para} state = skipping;
	unsigned line_width = 0;
	for (char *cp = str; *cp; cp++)
	{
		switch (state)
		{
		case skipping:

			if ((*cp != ' ') && (*cp != '\n') && (*cp != '\t'))
			{
				fputc(*cp, fout);
				line_width++;
				state = printing;
			}

			break;

		case printing:
			fputc(*cp, fout);
			line_width++;

			if ((*cp == ' ') || (*cp == '\n'))
			{
				if (line_width > 75)
				{
					if (*cp != '\n')
					{
						fputc('\n', fout);
					}
					line_width = 0;
				}
				state = *cp == ' ' ? skipping : check_para;
			}
			break;

		case check_para:

			if ((*cp == '\n') || ((*cp != ' ') && (*cp != '\t')))
			{
				fputc(*cp, fout);
			}
			state = skipping;
			break;

		}
	}
}

static bool print_pid_as_reference_in_list(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO               pid       = (pID_INFO) pelem->mbr;
	pFSMRSTOutputGenerator pfsmrstog = (pFSMRSTOutputGenerator) data;

	bool no_reference = strstr(pid->name, "noEvent")
	                    || strstr(pid->name, "noTransition")
	                    ;

	if (no_reference)
	{
		fprintf(FOUT(pfsmrstog)
				, "* %s\n"
				, pid->name
				);
	}
	else
	{
		char *fq_machine_name = fqMachineName(PMD(pfsmrstog));
		bool free_str         = false;

		/* Reset the machine name to the parent, if this is a shared event or it belongs to the parent. */
		if (pid->type == EVENT)
		{
			if (
				pid->type_data.event_data.shared_with_parent
				|| (pfsmrstog->parent_fsmrstog && pid->powningMachine == pfsmrstog->parent_fsmrstog->pmd->pmi)
				)
			{
				fq_machine_name = fqMachineName(PMD(pfsmrstog->parent_fsmrstog));
			}
			else if (pid->powningMachine != pfsmrstog->pmd->pmi)
			{
				fq_machine_name = fqMachineNamePMI(pid->powningMachine);
				free_str        = true;
			}
		}

		fprintf(FOUT(pfsmrstog)
				, "* :ref:`%s <%s.%s>`\n"
				, pid->name
				, fq_machine_name
				, pid->name
				);

		if (free_str)
		{
			free(fq_machine_name);
		}
	}

	return false;
}


static bool print_pmi_in_list(pLIST_ELEMENT pelem, void *data)
{
	pMACHINE_INFO          pmi       = (pMACHINE_INFO) pelem->mbr;
	pFSMRSTOutputGenerator pfsmrstog = (pFSMRSTOutputGenerator) data;

	                    ;
	fprintf(FOUT(pfsmrstog)
			, "* %s\n"
			, pmi->name->name
			);

	return false;
}

static bool print_sequence_svg(pLIST_ELEMENT pelem, void *data)
{
	pEVENT_SEQUENCE        psequence = (pEVENT_SEQUENCE) pelem->mbr;
	pFSMRSTOutputGenerator pfsmrstog = (pFSMRSTOutputGenerator) data;
	char                   *fn;

	fprintf(FOUT(pfsmrstog)
			, "\n.. image:: %s.svg\n%s:class: plantuml\n\n"
			, (fn = generate_sequence_file_name(psequence, PMI(pfsmrstog))) ? fn : ""
			, indent
			);

	CHECK_AND_FREE(fn);

	return false;
}

static bool print_sequence(pLIST_ELEMENT pelem, void *data)
{
	pEVENT_SEQUENCE          psequence = (pEVENT_SEQUENCE) pelem->mbr;
	pFSMRSTOutputGenerator   pfsmrstog = (pFSMRSTOutputGenerator) data;
	SEQUENCE_ITERATOR_HELPER sih       = {0};

	sih.ih.fout        = FOUT(pfsmrstog);
	sih.ih.pmi         = PMI(pfsmrstog);
	sih.ih.first       = true;
	sih.st.pcurr_state = psequence->initial_state;

	fprintf(FOUT(pfsmrstog)
			,"%s* - %s\n"
			, indent
			, psequence->name->name
			);

  	fprintf(FOUT(pfsmrstog)
  			, "%s  - "
  			, indent
  			);

	if (psequence->docCmt)
	{
		stream_multiline(FOUT(pfsmrstog), 4, psequence->docCmt);
	}

	fprintf(FOUT(pfsmrstog)
			, "\n"
			);

	fprintf(FOUT(pfsmrstog)
			, "%s  - %s\n%s  - "
			, indent
			, psequence->initial_state->name
			, indent
			);

	iterate_list(psequence->sequence
				 , print_sequence_node_rst
				 , &sih
				 );

	fprintf(FOUT(pfsmrstog)
			, "\n"
			);

	fprintf(FOUT(pfsmrstog)
			, "%s  - %sTraced: %s\n"
			, indent
			, psequence->final_state ? "| " : ""
			, sih.st.pcurr_state->name
			);

	if (psequence->final_state)
	{
		fprintf(FOUT(pfsmrstog)
				, "%s    | \n"
				, indent
				);

		fprintf(FOUT(pfsmrstog)
				, "%s    | %s was given in the sequence%s.\n"
				, indent
				, psequence->final_state->name
				, psequence->final_state != sih.st.pcurr_state
				  ? "; this does not match the traced state"
				  : ""
				);

	}
	return false;
}

/**
 * Pad the given number of columns with spaces.  Start with
 * indent.
 * 
 * @author Steven Stanton (8/3/2025)
 * 
 * @param fout   Output stream
 * @param cols   Number of columns to pad after indent.
 */
static void pad(FILE *fout, unsigned cols)
{
	fputs(indent, fout);
	for (unsigned col = 0; col < cols; col++)
	{
		fputc(' ', fout);
	}
}

/**
 * Write the given string to the given stream.  After any
 * newline, pad the given number of columns, and insert
 * the line continuation string.
 * 
 * @author Steven Stanton (8/3/2025)
 * 
 * @param fout   Output stream.
 * @param cols   Number of columns to pad.
 * @param string String to stream.
 */
static void stream_multiline(FILE *fout, unsigned cols, char *string)
{
	enum {normal, padding} state = normal;

	for (char *cp = string; *cp; cp++)
	{
		printf("0x%02x %s\n"
			   , *cp
			   , state == normal ? "normal" : "padding"
			   );
		switch (state)
		{
		case normal:
			switch (*cp)
			{
			case '\n':
			case '\r':
				fputc('\n', fout);
				pad(fout,cols);
				state = padding;
				break;
			default:
				fputc(*cp, fout);
				break;
			}
			break;
		case padding:
			switch (*cp)
			{
			case '\t':
			case '\n':
			case '\r':
				break;
			default:
				fputc('|', fout);
				fputc(' ', fout);
				fputc(*cp, fout);
				state = normal;
				break;
			}
			break;
		}
	}
}

static bool print_sequence_node_rst(pLIST_ELEMENT pelem, void *data)
{
	pEVENT_SEQUENCE_NODE      pesn  = (pEVENT_SEQUENCE_NODE) pelem->mbr;
	pITERATOR_HELPER          pih   = (pITERATOR_HELPER) data;
	pSEQUENCE_ITERATOR_HELPER psih  = (pSEQUENCE_ITERATOR_HELPER) data;

	//Grab the current action before moving to the next state.
	pID_INFO paction = get_action(pih->pmi
								  , pesn->pevent->order
								  , psih->st.pcurr_state->order
								  );
	if (!pih->first)
	{
		pad(pih->fout, 4);
	}
	else
	{
		pih->first = false;
	}

	fprintf(pih->fout
			,"* %s: %s ->"
			, pesn->pevent->name
			, psih->st.pcurr_state->name
			);

	TRANSITION_NOTE tn = determine_next_state(pih->pmi, pesn, &psih->st);

	fprintf(pih->fout
			, "%s (%s)"
			, psih->st.pcurr_state->name
			, paction ? paction->name : "No Action"
			);

	if (tn != tn_none)
	{
		fprintf(pih->fout, "\n");

		pad(pih->fout, 7);

		switch (tn)
		{
		case tn_no_fsm_transition:
			fprintf(pih->fout
					, "* The sequence indicates a EVENT_SEQUENCE, but none is given in the FSM."
					);
			break;
		case tn_state_mismatch:
			fprintf(pih->fout
					, "* The transition indicated in the sequence does not match the FSM."
					);
			break;
		case tn_fn_mismatch:
		case tn_fn_match:
			fprintf(pih->fout
					, "* | State machine indicates transition is via function %s.\n"
					, psih->st.pcurr_transition->name
					);
			fprintf(pih->fout
					, "            | \n            | The transition given in the sequence is %s"
					, tn == tn_fn_match ? "" : "not "
					);
			fprintf(pih->fout
					, "found in the function's return declaration."
					);
			break;
		case tn_first_return:
			fprintf(pih->fout
					, "* | State machine indicates transition is via function %s and no transition was indicated in the sequence."
					, psih->st.pcurr_transition->name
					);
			fprintf(pih->fout
					, "\n            | \n            | The first indicated return value was chosen."
					);
			break;
		case tn_no_fn_return_list:
			fprintf(pih->fout
					, "* State machine indicates transition is via function %s and no transition was indicated in the sequence."
					, psih->st.pcurr_transition->name
					);
			fprintf(pih->fout
					, "\n          | \n          | Furthermore, no return list was given for the function."
					);
			break;
		default:
			break;
		}
		fprintf(pih->fout, "\n");
	}

	fprintf(pih->fout, "\n");

	return false;
}

