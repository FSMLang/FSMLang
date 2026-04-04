/**
*  fsm_python_transitions.c
*
*    Creates an FSM based on the pytransitions model.
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
*  See: https://github.com/pytransitions/transitions.
*
*  Only flat machines are supported.
*
*/

#include "fsm_python_transitions.h"
#include "util_file_inclusion.h"
#include "action_info_list.h"
#include "y.tab.h"


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

static int initPyTransitionsWriter(pFSMOutputGenerator,char *);
static void writePyTransitionsWriter(pFSMOutputGenerator,pMACHINE_INFO);
static void closePyTransitionsWriter(pFSMOutputGenerator,int);

static int initPyTransitionsFN(pFSMOutputGenerator,char *);
static void writePyTransitionsFN(pFSMOutputGenerator,pMACHINE_INFO);
static void closePyTransitionsFN(pFSMOutputGenerator,int);

static bool print_state_names(pLIST_ELEMENT,void*);
static bool print_state_declarations(pLIST_ELEMENT,void*);
static bool print_consolidated_action_info(pLIST_ELEMENT,void*);
static bool print_matrices(pLIST_ELEMENT,void*);
static bool print_trigger(pLIST_ELEMENT,void*);
static bool print_action_stubs(pLIST_ELEMENT,void*);
static bool print_entry_exit_stubs(pLIST_ELEMENT,void*);
static bool print_transition_stubs(pLIST_ELEMENT,void*);
static bool print_transition_fn_stubs(pLIST_ELEMENT,void*);
static bool print_transition_fn_return_stubs(pLIST_ELEMENT,void*);
static bool print_transition_fn_return_option(pLIST_ELEMENT,void*);
static void print_transition_as_dict(pID_INFO,pITERATOR_HELPER);
static void print_simple_transition_as_dict(pID_INFO,pITERATOR_HELPER);
static void print_transition_fn_as_dict(pID_INFO,pITERATOR_HELPER);
static void print_transition_as_list(pID_INFO,pITERATOR_HELPER);

typedef struct _fsm_pytransitions_output_generator_ FSMPyTransitionsOutputGenerator, *pFSMPyTransitionsOutputGenerator;
typedef struct _pytransitions_machine_data_ PyTransitionsData, *pPyTransitionsData;

struct _pytransitions_machine_data_ {
  FILE	        *file;
  char	        *fileName;
  char          *baseName;
  pMACHINE_INFO pmi;
};

struct _fsm_pytransitions_output_generator_
{
   FSMOutputGenerator        fsmog;
   pPyTransitionsData ptd;
};

static FSMPyTransitionsOutputGenerator PyTransitionsMachineWriter = {
 {
    initPyTransitionsWriter
    , writePyTransitionsWriter
    , closePyTransitionsWriter
    , generatePyTransitionsWriter
  }
  , NULL
};

static char *transitions_str = "transitions = [";
static int transitions_str_len;

static char *states_str = "states = [";
static int states_str_len;

pFSMOutputGenerator generatePyTransitionsWriter(pFSMOutputGenerator parent)
{
	pFSMOutputGenerator pfsmog;

	if (parent)
	{
		yyerror("PyTransitions supports only flat machines.");
	}
	else
	{
		pfsmog =  (pFSMOutputGenerator)&PyTransitionsMachineWriter;
	}

	if (output_generated_file_names_only)
	{
		pfsmog->initOutput   = initPyTransitionsFN;
		pfsmog->writeMachine = writePyTransitionsFN;
		pfsmog->closeOutput  = closePyTransitionsFN;
	}

	return pfsmog;
}

static int initPyTransitionsFN(pFSMOutputGenerator pfsmog, char *baseFileName)
{
  pFSMPyTransitionsOutputGenerator pfsmpytog
    = (pFSMPyTransitionsOutputGenerator) pfsmog;

  if (NULL != (pfsmpytog->ptd = calloc(1
                           , sizeof(PyTransitionsData)
                           )
         )
    )
  {
    if (baseFileName)
    {
      pfsmpytog->ptd->fileName = createFileName(baseFileName,".py");
      pfsmpytog->ptd->baseName = strdup(baseFileName);
    }
  }

  /* this may look funny, but it does the trick */
  return ((int) !pfsmpytog->ptd->fileName);
}

static void writePyTransitionsFN(pFSMOutputGenerator pfsmog, pMACHINE_INFO pmi)
{ 
  pFSMPyTransitionsOutputGenerator pfsmpytog
	  = (pFSMPyTransitionsOutputGenerator)pfsmog;

  if (output_make_recipe)
  {
    printf("%s: %s.fsm "
		   , pfsmpytog->ptd->fileName
		   , pfsmpytog->ptd->baseName
         );
    
    print_included_files_list();

    printf("\n");
  }
  else
  {
    printf("%s ", pfsmpytog->ptd->fileName);
  }

  (void) pmi;
}

static void closePyTransitionsFN(pFSMOutputGenerator pfsmog, int good)
{
	pFSMPyTransitionsOutputGenerator pfsmpytog
		= (pFSMPyTransitionsOutputGenerator)pfsmog;

	if (!pfsmpytog || !pfsmpytog->ptd)
	{
		return;
	}

	CHECK_AND_FREE(pfsmpytog->ptd->fileName);

	free(pfsmpytog->ptd);

	(void) good;
}

static int initPyTransitionsWriter(pFSMOutputGenerator pfsmog, char *baseFileName)
{
	pFSMPyTransitionsOutputGenerator pfsmpytog
		= (pFSMPyTransitionsOutputGenerator) pfsmog;

	if (NULL != (pfsmpytog->ptd = calloc(1, sizeof(PyTransitionsData))))
	{
		if (!baseFileName)
		{
			pfsmpytog->ptd->file = stdout;
		}
		else
		{
			pfsmpytog->ptd->fileName = createFileName(baseFileName, ".py");
			pfsmpytog->ptd->baseName = strdup(baseFileName);

			if (NULL == (pfsmpytog->ptd->file = openFile(pfsmpytog->ptd->fileName, "w")))
			{
				CHECK_AND_FREE(pfsmpytog->ptd->fileName);
				CHECK_AND_FREE(pfsmpytog->ptd->baseName);
			}
			else
			{
				fprintf(pfsmpytog->ptd->file
						, "\"\"\"\n\t%s\n\n"
						, pfsmpytog->ptd->fileName
						);
				fprintf(pfsmpytog->ptd->file
						, "\tThis file generated by FSMLang\n\n\"\"\"\n\n"
						);
			}
		}
	}

	/* this may look funny, but it does the trick */
	return ((int) !pfsmpytog->ptd->fileName);
}

static void writePyTransitionsWriter(pFSMOutputGenerator pfsmog, pMACHINE_INFO pmi)
{
	FILE            *fout = ((pFSMPyTransitionsOutputGenerator)pfsmog)->ptd->file;
	ITERATOR_HELPER ih    = {.fout = fout };

	if (pmi->native_prologue)
	{
		fprintf(fout
				, "%s\n"
				, pmi->native_prologue
				);
	}

	fprintf(fout
			, "from transitions import Machine\n"
			);

	if (pmi->native_prologue)
	{
		fprintf(fout
				, "%s\n"
				, pmi->native_prologue
			   );
	}

	fprintf(fout
			, "\nclass %s(object):\n\n"
			, pmi->name->name
			);

	// Write any native implementation block
	if (pmi->native_impl_prologue)
	{
		fprintf(fout
				, "\n%s\n"
				, pmi->native_impl_prologue
				);
	}

	// Write the states
	states_str_len = strlen(states_str);
	fprintf(fout
			, "\t%s"
			, states_str
			);

	ih.first = true;
	iterate_list(pmi->state_list
				 , print_state_declarations
				 , &ih
				 );

	fprintf(fout
			, "\t%*.*s]\n"
			, states_str_len
			, states_str_len
			, "  "
			);

	// Write the transitions
	transitions_str_len = strlen(transitions_str);
	fprintf(fout
			, "\t%s\n"
			, transitions_str
			);

	pLIST pconsolidated = consolidate_action_info_list(pmi->action_info_list);
	ih.first = true;
	iterate_list(pconsolidated
				 , print_consolidated_action_info
				 , &ih
				 );

	fprintf(fout
			, "\t%*.*s]\n"
			, transitions_str_len
			, transitions_str_len
			," "
			);

	fprintf(fout
			, "\n\tdef __init__(self):\n"
			);

	fprintf(fout
			, "\t\tself.machine = Machine(model=self, states=%s.states, "
			  "transitions=%s.transitions, initial='%s'"
			, pmi->name->name
			, pmi->name->name
			, stateNameByIndex(pmi, 0)
			);

	if (pmi->machineTransition)
	{
		fprintf(fout
				, ", after_state_change='%s'"
				, pmi->machineTransition->name
				);
	}

	if (pmi->data_block_count)
	{
		fprintf(fout
				, ", send_event=True"
				);
	}

	fprintf(fout
			, ")\n"
			);

	if (pmi->data_block_count)
	{
		fprintf(fout
				, "\t\tself.data = {}\n"
				);
	}

	fprintf(fout
			, "\n"
			);

	// Write action stubs
	if (generate_weak_fns)
	{

		if (pmi->machineTransition)
		{
			fprintf(fout
					, "\tdef %s(self):\n\t\tprint('%s')\n"
					, pmi->machineTransition->name
					, pmi->machineTransition->name
					);
		}

		iterate_list(pmi->action_list
					 , print_action_stubs
					 , &ih
					 );

		if (pmi->states_with_entry_fns_count
			|| pmi->states_with_exit_fns_count)
		{
			iterate_list(pmi->state_list
						 , print_entry_exit_stubs
						 , &ih
						 );
		}

		iterate_list(pmi->transition_list
					 , print_transition_stubs
					 , &ih
					 );

		iterate_list(pmi->transition_fn_list
					 , print_transition_fn_stubs
					 , &ih
					 );

	}

	fprintf(fout, "\n");
}

static void closePyTransitionsWriter(pFSMOutputGenerator pfsmog, int good)
{
	pFSMPyTransitionsOutputGenerator pfsmpytog
		= (pFSMPyTransitionsOutputGenerator) pfsmog;

	fclose(pfsmpytog->ptd->file);

	if (!good)
	{
		unlink(pfsmpytog->ptd->fileName);
	}

}

static bool print_state_names(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO         pstate = (pID_INFO) pelem->mbr;
	pITERATOR_HELPER pih    = (pITERATOR_HELPER) data;

	fprintf(pih->fout
			, "%s'%s'"
			, pih->first ? (pih->first = false, "") : " ,"
			, pstate->name
			);

	return false;
}

static bool print_state_declarations(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO         pstate = (pID_INFO) pelem->mbr;
	pITERATOR_HELPER pih    = (pITERATOR_HELPER) data;
	pSTATE_DATA      psd    = &(pstate->type_data.state_data);

	if (psd->entry_fn || psd->exit_fn)
	{
		if (pih->first)
		{
			pih->first = false;
			fprintf(pih->fout
					, "  {'name':"
					);
		}
		else
		{
			fprintf(pih->fout
					, "\t%*.*s{'name':"
					, states_str_len + 4
					, states_str_len + 4
					, " , "
					);
		}
		fprintf(pih->fout
				, " '%s'"
				, pstate->name
				);

		if (psd->entry_fn)
		{
			fprintf(pih->fout
					, ", 'on_enter': ['%s']"
					, psd->entry_fn->name
					);
		}

		if (psd->exit_fn)
		{
			fprintf(pih->fout
					, ", 'on_exit': ['%s']"
					, psd->exit_fn->name
					);
		}

		fprintf(pih->fout, "}\n");
	}
	else
	{
		fprintf(pih->fout
				, "\t%*.*s%s'%s'\n"
				, states_str_len
				, states_str_len
				, " "
				, pih->first ? (pih->first = false, "") : " ,"
				, pstate->name
				);
	}

	return false;
}

static bool print_consolidated_action_info(pLIST_ELEMENT pelem, void *data)
{
	pCONSOLIDATED_ACTION_INFO pcai = (pCONSOLIDATED_ACTION_INFO) pelem->mbr;
	pITERATOR_HELPER          pih  = (pITERATOR_HELPER) data;

	pih->pai   = pcai->pai;
	iterate_list(pcai->matrices
				 , print_matrices
				 , pih
				 );

	return false;
}

static bool print_matrices(pLIST_ELEMENT pelem, void *data)
{
	pMATRIX_INFO     pmi = (pMATRIX_INFO) pelem->mbr;
	pITERATOR_HELPER pih = (pITERATOR_HELPER) data;

	pih->pOtherElem = pelem;
	iterate_list(pmi->event_list
				 , print_trigger
				 , pih
				 );

	return false;
}

static bool print_trigger(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO         pevent = (pID_INFO) pelem->mbr;
	pITERATOR_HELPER pih    = (pITERATOR_HELPER) data;
	pUSER_EVENT_DATA pued   = pevent->type_data.event_data.puser_event_data;

	if (pued)
	{
		if (pued->translator)
		{
			pih->str = pued->translator->name;
		}
		else
		{
			char default_translator[256];
			snprintf(default_translator, sizeof(default_translator), "translate_%s_data", pevent->name);
			pih->str = default_translator;
		}
		print_transition_as_dict(pevent, pih);
		pih->str = NULL;
	}
	else if (
		(pih->pai->action && strlen(pih->pai->action->name))
		|| pih->pai->transition->condition_fn
		|| (pih->pai->transition->name->type == TRANSITION_FN)
		)
	{
		print_transition_as_dict(pevent, pih);
	}
	else
	{
		print_transition_as_list(pevent,pih);
	}

	return false;
}

static void print_transition_as_list(pID_INFO pevent, pITERATOR_HELPER pih)
{
	pMATRIX_INFO pmi = (pMATRIX_INFO) pih->pOtherElem->mbr;

	fprintf(pih->fout
			, "\t%*.*s%s['%s', %s"
			, transitions_str_len
			, transitions_str_len
			, " "
			, pih->first ? "" : ", "
			, pevent->name
			, pmi->state_list->count > 1 ? "[" : ""
			);

	// To avoid creating our own callback helper, we re-use "first"
	pih->first = true;
	iterate_list(pmi->state_list
				 , print_state_names
				 , pih
				 );
	pih->first = false;

	if (pmi->state_list->count > 1)
	{
		fprintf(pih->fout
				, "] "
				);
	}

	if (pih->pai->transition)
	{
		fprintf(pih->fout
				, ", '%s']\n"
				, pih->pai->transition->name->name
				);
	}
	else
	{
		fprintf(pih->fout
				, ", None]\n"
				);
	}

}

static void print_transition_as_dict(pID_INFO pevent, pITERATOR_HELPER pih)
{
	if (!pih->pai->transition
		|| (pih->pai->transition && pih->pai->transition->name->type == STATE)
		)
	{
		print_simple_transition_as_dict(pevent, pih);
	}
	else
	{
		print_transition_fn_as_dict(pevent, pih);
	}
}

static void print_simple_transition_as_dict(pID_INFO pevent, pITERATOR_HELPER pih)
{
	pMATRIX_INFO pmi    = (pMATRIX_INFO) pih->pOtherElem->mbr;
	bool         action_present = (pih->pai->action && strlen(pih->pai->action->name));

	fprintf(pih->fout
			, "\t%*.*s%s{'trigger': '%s'\n"
			, transitions_str_len
			, transitions_str_len
			, " "
			, pih->first ? "" : ", "
			, pevent->name
			);

	fprintf(pih->fout
			, "\t%*.*s  , 'source': %s"
			, transitions_str_len
			, transitions_str_len
			, " "
			, pmi->state_list->count > 1 ? "[" : ""
			);

	// To avoid creating our own callback helper, we re-use "first"
	pih->first = true;
	iterate_list(pmi->state_list
				 , print_state_names
				 , pih
				 );
	pih->first = false;

	if (pmi->state_list->count > 1)
	{
		fprintf(pih->fout
				, "]"
				);
	}

	fprintf(pih->fout
			, "\n"
			);

	fprintf(pih->fout
			, "\t%*.*s  , 'dest': "
			, transitions_str_len
			, transitions_str_len
			, " "
			);
	if (pih->pai->transition)
	{
		fprintf(pih->fout
				, "'%s'"
				, pih->pai->transition->name->name
			   );
	}
	else
	{
		fprintf(pih->fout
				, "None"
			   );
	}
	fprintf(pih->fout, "\n");

	if (action_present)
	{
		fprintf(pih->fout
				, "\t%*.*s  , 'before': '%s'\n"
				, transitions_str_len
				, transitions_str_len
				, " "
				, pih->pai->action->name
				);
	}

	if (
		pih->pai->transition
		&& pih->pai->transition->condition_fn
		)
	{
		fprintf(pih->fout
				, "\t%*.*s  , 'conditions': ['%s']\n"
				, transitions_str_len
				, transitions_str_len
				, " "
				, pih->pai->transition->condition_fn->name
				);
	}

	if (pih->str)
	{
		fprintf(pih->fout
				, "\t%*.*s  , 'prepare': '%s'\n"
				, transitions_str_len
				, transitions_str_len
				, " "
				, pih->str
				);
	}

	fprintf(pih->fout
			, "\t%*.*s  }\n"
			, transitions_str_len
			, transitions_str_len
			, " "
			);

}

static void print_transition_fn_as_dict(pID_INFO pevent, pITERATOR_HELPER pih)
{
	pih->pid = pevent;
	iterate_list(pih->pai->transition->name->transition_fn_returns_decl
				 , print_transition_fn_return_option
				 , pih
				 );
}

static bool print_transition_fn_return_option(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO         pstate = (pID_INFO) pelem->mbr;
	pITERATOR_HELPER pih    = (pITERATOR_HELPER) data;
	pMATRIX_INFO     pmi    = (pMATRIX_INFO) pih->pOtherElem->mbr;
	pID_INFO         pevent = pih->pid;
	bool             action_present = (pih->pai->action && strlen(pih->pai->action->name));

	fprintf(pih->fout
			, "\t%*.*s%s{'trigger': '%s'\n"
			, transitions_str_len
			, transitions_str_len
			, " "
			, pih->first ? "" : ", "
			, pevent->name
			);

	fprintf(pih->fout
			, "\t%*.*s  , 'source': %s"
			, transitions_str_len
			, transitions_str_len
			, " "
			, pmi->state_list->count > 1 ? "[" : ""
			);

	// To avoid creating our own callback helper, we re-use "first"
	pih->first = true;
	iterate_list(pmi->state_list
				 , print_state_names
				 , pih
				 );
	pih->first = false;

	if (pmi->state_list->count > 1)
	{
		fprintf(pih->fout
				, "]"
				);
	}

	fprintf(pih->fout
			, "\n"
			);

	fprintf(pih->fout
			, "\t%*.*s  , 'dest': '%s'\n"
			, transitions_str_len
			, transitions_str_len
			, " "
			, pstate->name
			);

	if (action_present)
	{
		fprintf(pih->fout
				, "\t%*.*s  , 'before': '%s'\n"
				, transitions_str_len
				, transitions_str_len
				, " "
				, pih->pai->action->name
				);
	}

	fprintf(pih->fout
			, "\t%*.*s  , 'conditions': ['choose_%s']\n"
			, transitions_str_len
			, transitions_str_len
			, " "
			, pstate->name
			);

	if (pih->str)
	{
		fprintf(pih->fout
				, "\t%*.*s  , 'prepare': '%s'\n"
				, transitions_str_len
				, transitions_str_len
				, " "
				, pih->str
				);
	}

	fprintf(pih->fout
			, "\t%*.*s  }\n"
			, transitions_str_len
			, transitions_str_len
			, " "
			);

	return false;
}

static bool print_action_stubs(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO         paction = ((pID_INFO)pelem->mbr);
	pITERATOR_HELPER pih     = ((pITERATOR_HELPER)data);

	if (paction->name && strlen(paction->name))
	{
		fprintf(pih->fout
				, "\n\tdef %s(self):\n\t\tprint('%s')\n\n"
				, paction->name
				, paction->name
				);
	}

	return false;
}

static bool print_entry_exit_stubs(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO         pstate = ((pID_INFO)pelem->mbr);
	pITERATOR_HELPER pih    = ((pITERATOR_HELPER)data);
	pSTATE_DATA      psd    = &(pstate->type_data.state_data);

	if (psd->entry_fn)
	{
		fprintf(pih->fout
				, "\n\tdef %s(self):\n\t\tprint('%s')\n\n"
				, psd->entry_fn->name
				, psd->entry_fn->name
				);
	}

	if (psd->exit_fn)
	{
		fprintf(pih->fout
				, "\n\tdef %s(self):\n\t\tprint('%s')\n\n"
				, psd->exit_fn->name
				, psd->exit_fn->name
				);
	}

	return false;
}

static bool print_transition_stubs(pLIST_ELEMENT pelem, void *data)
{
	pTRANSITION_DATA ptransition = (pTRANSITION_DATA) pelem->mbr;
	pITERATOR_HELPER pih         = (pITERATOR_HELPER) data;

	if (ptransition->condition_fn)
	{
		fprintf(pih->fout
				, "\n\t@property\n\tdef %s(self):\n\t\treturn True\n\n"
				, ptransition->condition_fn->name
				);
	}

	return false;
}

static bool print_transition_fn_stubs(pLIST_ELEMENT pelem, void *data)
{
	pTRANSITION_DATA ptransition = (pTRANSITION_DATA) pelem->mbr;
	pITERATOR_HELPER pih         = (pITERATOR_HELPER) data;

	if (ptransition->name->transition_fn_returns_decl)
	{
		iterate_list(ptransition->name->transition_fn_returns_decl
					 , print_transition_fn_return_stubs
					 , pih
					 );
	}

	return false;
}

static bool print_transition_fn_return_stubs(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO pid         = (pID_INFO) pelem->mbr;
	pITERATOR_HELPER pih = (pITERATOR_HELPER) data;

	fprintf(pih->fout
			, "\n\t@property\n\tdef choose_%s(self):\n\t\treturn True\n"
			, pid->name
			);

	return false;
}

