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
static bool print_event_data_action_stubs(pLIST_ELEMENT,void*);
static bool print_entry_exit_stubs(pLIST_ELEMENT,void*);
static bool print_event_data_entry_exit_stubs(pLIST_ELEMENT,void*);
static bool print_transition_stubs(pLIST_ELEMENT,void*);
static bool print_transition_fn_stubs(pLIST_ELEMENT,void*);
static bool print_transition_fn_return_stubs(pLIST_ELEMENT,void*);
static bool print_transition_fn_return_cond_stubs(pLIST_ELEMENT,void*);
static bool print_transition_fn_return_option(pLIST_ELEMENT,void*);
static bool print_transition_fn_return_cond_option(pLIST_ELEMENT,void*);
static void print_transition_as_dict(pID_INFO,pITERATOR_HELPER);
static void print_simple_transition_as_dict(pID_INFO,pITERATOR_HELPER);
static void print_transition_fn_as_dict(pID_INFO,pITERATOR_HELPER);
static void print_transition_as_list(pID_INFO,pITERATOR_HELPER);
static bool print_event_translator_mapping(pLIST_ELEMENT,void*);
static bool print_event_translator_stub(pLIST_ELEMENT,void*);
static bool scan_consolidated(pLIST_ELEMENT,void*);
static bool scan_matrix(pLIST_ELEMENT,void*);
static bool scan_event(pLIST_ELEMENT,void*);
static bool print_actions_dict_consolidated(pLIST_ELEMENT,void*);
static bool print_actions_dict_matrix(pLIST_ELEMENT,void*);
static bool print_actions_dict_event(pLIST_ELEMENT,void*);
static bool print_actions_dict_state(pLIST_ELEMENT,void*);

typedef struct _fsm_pytransitions_output_generator_ FSMPyTransitionsOutputGenerator, *pFSMPyTransitionsOutputGenerator;
typedef struct _pytransitions_machine_data_ PyTransitionsData, *pPyTransitionsData;

struct _pytransitions_machine_data_ {
  FILE	        *file;
  char	        *fileName;
  char          *baseName;
  pMACHINE_INFO pmi;
  bool           uses_event_data;
  bool           uses_actions;
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
	CHECK_AND_FREE(pfsmpytog->ptd->baseName);

	CHECK_AND_FREE(pfsmpytog->ptd);

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
	pPyTransitionsData ptd  = ((pFSMPyTransitionsOutputGenerator)pfsmog)->ptd;
	FILE              *fout = ptd->file;
	ITERATOR_HELPER ih      = {.fout = fout };

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

	if (pmi->states_with_entry_fns_count || pmi->states_with_exit_fns_count)
	{
		// Mixed or all-dict format: multi-line
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
	}
	else
	{
		// All plain strings: single-line format
		fprintf(fout
				, "\t%s"
				, states_str
				);

		ih.first = true;
		iterate_list(pmi->state_list
					 , print_state_names
					 , &ih
					 );

		fprintf(fout, "]\n");
	}

	// Write the transitions
	transitions_str_len = strlen(transitions_str);

	pLIST pconsolidated = consolidate_action_info_list(pmi->action_info_list);

	// Pre-detect uses_event_data (scan_ih.found) and uses_actions (scan_ih.error)
	// before printing so that the transition-printing pass can suppress
	// 'before' callbacks when action-matrix semantics are active.
	{
		ITERATOR_HELPER scan_ih = { .fout = NULL, .found = false, .error = false };
		iterate_list(pconsolidated, scan_consolidated, &scan_ih);
		ptd->uses_event_data = scan_ih.found; // true if any data-bearing event appears
		ptd->uses_actions    = scan_ih.error; // true if any action-matrix action exists
	}

	fprintf(fout
			, "\t%s\n"
			, transitions_str
			);

	// ih.error is repurposed here as "uses_prepare_event": when true, transition
	// printing will NOT emit 'before' callbacks (actions go through _fsm_prepare_event).
	ih.error = ptd->uses_event_data || ptd->uses_actions;
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

	if (ptd->uses_event_data)
	{
		fprintf(fout, "\n\t_fsm_translators = {\n");
		iterate_list(pmi->event_list, print_event_translator_mapping, &ih);
		fprintf(fout, "\t}\n");
	}

	if (ptd->uses_event_data || ptd->uses_actions)
	{
		fprintf(fout, "\n\t_fsm_actions = {\n");
		iterate_list(pconsolidated, print_actions_dict_consolidated, &ih);
		fprintf(fout, "\t}\n");
	}

	fprintf(fout
			, "\n\tdef __init__(self):\n"
			);

	if (ptd->uses_event_data)
	{
		fprintf(fout
				, "\t\tself.data = {}\n"
				);
	}

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

	if (ptd->uses_event_data || ptd->uses_actions)
	{
		fprintf(fout
				, ", send_event=True"
				", prepare_event=\"_fsm_prepare_event\""
				);
	}

	if (pmi->data)
	{
		fprintf(fout
				, ")\n\t\tself.data_init()\n\n"
				);
	}
	else
	{
		fprintf(fout
				, ")\n\n"
				);
	}

	if (ptd->uses_event_data || ptd->uses_actions)
	{
		fprintf(fout
				, "\n\tdef _fsm_get_trigger_name(self, e):\n"
				  "\t\tev = getattr(e, \"event\", None)\n"
				  "\t\tif ev is not None:\n"
				  "\t\t\tname = getattr(ev, \"name\", None)\n"
				  "\t\t\tif name:\n"
				  "\t\t\t\treturn name\n"
				  "\t\t\tname = getattr(ev, \"trigger\", None)\n"
				  "\t\t\tif name:\n"
				  "\t\t\t\treturn name\n"
				  "\n"
				  "\t\tfor attr in (\"trigger\", \"name\", \"event_name\"):\n"
				  "\t\t\tname = getattr(e, attr, None)\n"
				  "\t\t\tif name:\n"
				  "\t\t\t\treturn name\n"
				  "\n"
				  "\t\ttr = getattr(e, \"transition\", None)\n"
				  "\t\tif tr is not None:\n"
				  "\t\t\tname = getattr(tr, \"trigger\", None)\n"
				  "\t\t\tif name:\n"
				  "\t\t\t\treturn name\n"
				  "\n"
				  "\t\treturn None\n"
				  "\n"
				);

		if (ptd->uses_event_data)
		{
			fprintf(fout
					, "\n\tdef _fsm_prepare_event(self, e):\n"
					  "\t\ttrigger = self._fsm_get_trigger_name(e)\n"
					  "\t\tif trigger is None:\n"
					  "\t\t\ttrigger = \"__unknown_trigger__\"\n"
					  "\n"
					  "\t\ttranslator_name = self._fsm_translators.get(trigger)\n"
					  "\t\tif translator_name is not None:\n"
					  "\t\t\tgetattr(self, translator_name)(e)\n"
					  "\n"
					  "\t\taction_name = self._fsm_actions.get((self.state, trigger))\n"
					  "\t\tif action_name is not None:\n"
					  "\t\t\tgetattr(self, action_name)(e)\n"
					  "\n"
					  "\t\treturn True\n"
					  "\n"
					);
		}
		else
		{
			fprintf(fout
					, "\n\tdef _fsm_prepare_event(self, e):\n"
					  "\t\ttrigger = self._fsm_get_trigger_name(e)\n"
					  "\t\tif trigger is None:\n"
					  "\t\t\ttrigger = \"__unknown_trigger__\"\n"
					  "\n"
					  "\t\taction_name = self._fsm_actions.get((self.state, trigger))\n"
					  "\t\tif action_name is not None:\n"
					  "\t\t\tgetattr(self, action_name)(e)\n"
					  "\n"
					  "\t\treturn True\n"
					  "\n"
					);
		}
	}

	// Write action stubs
	if (generate_weak_fns)
	{
		bool uses_prepare_event = ptd->uses_event_data || ptd->uses_actions;

		if (pmi->machineTransition)
		{
			fprintf(fout
					, "\tdef %s(self%s):\n\t\tprint('%s')\n"
					, pmi->machineTransition->name
					, uses_prepare_event ? ", event=None" : ""
					, pmi->machineTransition->name
					);
		}

		if (uses_prepare_event)
		{
			iterate_list(pmi->action_list
						 , print_event_data_action_stubs
						 , &ih
						 );
		}
		else
		{
			iterate_list(pmi->action_list
						 , print_action_stubs
						 , &ih
						 );
		}

		if (pmi->states_with_entry_fns_count
			|| pmi->states_with_exit_fns_count)
		{
			if (uses_prepare_event)
			{
				iterate_list(pmi->state_list
							 , print_event_data_entry_exit_stubs
							 , &ih
							 );
			}
			else
			{
				iterate_list(pmi->state_list
							 , print_entry_exit_stubs
							 , &ih
							 );
			}
		}

		iterate_list(pmi->transition_list
					 , print_transition_stubs
					 , &ih
					 );

		iterate_list(pmi->transition_fn_list
					 , print_transition_fn_stubs
					 , &ih
					 );

		if (ptd->uses_event_data)
		{
			iterate_list(pmi->event_list
						 , print_event_translator_stub
						 , &ih
						 );
		}

	}

}

static void closePyTransitionsWriter(pFSMOutputGenerator pfsmog, int good)
{
	pFSMPyTransitionsOutputGenerator pfsmpytog
		= (pFSMPyTransitionsOutputGenerator) pfsmog;

	if (!pfsmpytog || !pfsmpytog->ptd)
	{
		return;
	}

	if (pfsmpytog->ptd->file && pfsmpytog->ptd->file != stdout)
	{
		fclose(pfsmpytog->ptd->file);
	}

	if (!good && pfsmpytog->ptd->fileName)
	{
		unlink(pfsmpytog->ptd->fileName);
	}

	free(pfsmpytog->ptd->fileName);
	free(pfsmpytog->ptd->baseName);
	free(pfsmpytog->ptd);
	pfsmpytog->ptd = NULL;
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
	bool             is_data_bearing = (pevent->type_data.event_data.puser_event_data != NULL);

	if (is_data_bearing)
	{
		pih->found = true;
	}

	if (
		// pih->error means "uses_prepare_event": suppress action-only dict format
		// because actions are dispatched via _fsm_prepare_event, not transition 'before'.
		(!pih->error && pih->pai->action && strlen(pih->pai->action->name))
		|| (pih->pai->transition && pih->pai->transition->name->type == TRANSITION_FN)
		|| is_data_bearing
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
	pMATRIX_INFO     pmi           = (pMATRIX_INFO) pih->pOtherElem->mbr;
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

	// pih->error means "uses_prepare_event": actions go through _fsm_prepare_event,
	// so do not emit a 'before' callback here.
	if (action_present && !pih->error)
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
			, "\t%*.*s  }\n"
			, transitions_str_len
			, transitions_str_len
			, " "
			);

}

static void print_transition_fn_as_dict(pID_INFO pevent, pITERATOR_HELPER pih)
{
	pih->pid = pevent;
	if (pih->pai->transition->name->transition_fn_returns_cond_decl)
	{
		/* Mode B: use when/otherwise choices */
		iterate_list(pih->pai->transition->name->transition_fn_returns_cond_decl
					 , print_transition_fn_return_cond_option
					 , pih
					 );
	}
	else
	{
		/* Mode A: use plain state list with synthesized choose_<state> conditions */
		iterate_list(pih->pai->transition->name->transition_fn_returns_decl
					 , print_transition_fn_return_option
					 , pih
					 );
	}
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

	// pih->error means "uses_prepare_event": actions go through _fsm_prepare_event,
	// so do not emit a 'before' callback here.
	if (action_present && !pih->error)
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

	fprintf(pih->fout
			, "\t%*.*s  }\n"
			, transitions_str_len
			, transitions_str_len
			, " "
			);

	return false;
}

static bool print_transition_fn_return_cond_option(pLIST_ELEMENT pelem, void *data)
{
	pRETURN_CHOICE_DATA prcd         = (pRETURN_CHOICE_DATA) pelem->mbr;
	pITERATOR_HELPER    pih          = (pITERATOR_HELPER) data;
	pMATRIX_INFO        pmi          = (pMATRIX_INFO) pih->pOtherElem->mbr;
	pID_INFO            pevent       = pih->pid;
	bool                action_present = (pih->pai->action && strlen(pih->pai->action->name));

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
			, prcd->state->name
			);

	// pih->error means "uses_prepare_event": actions go through _fsm_prepare_event,
	// so do not emit a 'before' callback here.
	if (action_present && !pih->error)
	{
		fprintf(pih->fout
				, "\t%*.*s  , 'before': '%s'\n"
				, transitions_str_len
				, transitions_str_len
				, " "
				, pih->pai->action->name
				);
	}

	if (prcd->condition_fn)
	{
		/* 'when <condFn>': use the named condition function directly */
		fprintf(pih->fout
				, "\t%*.*s  , 'conditions': ['%s']\n"
				, transitions_str_len
				, transitions_str_len
				, " "
				, prcd->condition_fn->name
				);
	}
	else if (prcd->is_otherwise)
	{
		/* 'otherwise': synthesize choose_<state> condition */
		fprintf(pih->fout
				, "\t%*.*s  , 'conditions': ['choose_%s']\n"
				, transitions_str_len
				, transitions_str_len
				, " "
				, prcd->state->name
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
	(void) pelem;
	(void) data;
	return false;
}

static bool print_transition_fn_stubs(pLIST_ELEMENT pelem, void *data)
{
	pTRANSITION_DATA ptransition = (pTRANSITION_DATA) pelem->mbr;
	pITERATOR_HELPER pih         = (pITERATOR_HELPER) data;

	if (ptransition->name->transition_fn_returns_cond_decl)
	{
		/* Mode B: generate stubs from when/otherwise choices */
		iterate_list(ptransition->name->transition_fn_returns_cond_decl
					 , print_transition_fn_return_cond_stubs
					 , pih
					 );
	}
	else if (ptransition->name->transition_fn_returns_decl)
	{
		/* Mode A: generate choose_<state> stubs */
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
			, "\n\t@property\n\tdef choose_%s(self):\n\t\treturn True\n\n"
			, pid->name
			);

	return false;
}

static bool print_transition_fn_return_cond_stubs(pLIST_ELEMENT pelem, void *data)
{
	pRETURN_CHOICE_DATA prcd = (pRETURN_CHOICE_DATA) pelem->mbr;
	pITERATOR_HELPER    pih  = (pITERATOR_HELPER) data;

	if (prcd->condition_fn)
	{
		/* 'when <condFn>': generate a stub for condFn */
		fprintf(pih->fout
				, "\n\t@property\n\tdef %s(self):\n\t\treturn True\n\n"
				, prcd->condition_fn->name
				);
	}
	else if (prcd->is_otherwise)
	{
		/* 'otherwise': generate choose_<state> stub */
		fprintf(pih->fout
				, "\n\t@property\n\tdef choose_%s(self):\n\t\treturn True\n\n"
				, prcd->state->name
				);
	}

	return false;
}

static bool print_event_data_action_stubs(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO         paction = ((pID_INFO)pelem->mbr);
	pITERATOR_HELPER pih     = ((pITERATOR_HELPER)data);

	if (paction->name && strlen(paction->name))
	{
		fprintf(pih->fout
				, "\n\tdef %s(self, event=None):\n\t\tprint('%s')\n\n"
				, paction->name
				, paction->name
				);
	}

	return false;
}

static bool print_event_data_entry_exit_stubs(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO         pstate = ((pID_INFO)pelem->mbr);
	pITERATOR_HELPER pih    = ((pITERATOR_HELPER)data);
	pSTATE_DATA      psd    = &(pstate->type_data.state_data);

	if (psd->entry_fn)
	{
		fprintf(pih->fout
				, "\n\tdef %s(self, event=None):\n\t\tprint('%s')\n\n"
				, psd->entry_fn->name
				, psd->entry_fn->name
				);
	}

	if (psd->exit_fn)
	{
		fprintf(pih->fout
				, "\n\tdef %s(self, event=None):\n\t\tprint('%s')\n\n"
				, psd->exit_fn->name
				, psd->exit_fn->name
				);
	}

	return false;
}

static bool print_event_translator_mapping(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO         pevent = (pID_INFO) pelem->mbr;
	pITERATOR_HELPER pih    = (pITERATOR_HELPER) data;
	pUSER_EVENT_DATA pued   = pevent->type_data.event_data.puser_event_data;
	const char      *translator_name;

	if (!pued)
	{
		return false;
	}

	translator_name = (pued->translator) ? pued->translator->name : NULL;

	if (translator_name)
	{
		fprintf(pih->fout
				, "\t\t\"%s\": \"%s\",\n"
				, pevent->name
				, translator_name
				);
	}
	else
	{
		fprintf(pih->fout
				, "\t\t\"%s\": \"translate_%s\",\n"
				, pevent->name
				, pevent->name
				);
	}

	return false;
}

static bool print_event_translator_stub(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO         pevent = (pID_INFO) pelem->mbr;
	pITERATOR_HELPER pih    = (pITERATOR_HELPER) data;
	pUSER_EVENT_DATA pued   = pevent->type_data.event_data.puser_event_data;
	const char      *translator_name;

	if (!pued)
	{
		return false;
	}

	translator_name = (pued->translator) ? pued->translator->name : NULL;

	if (translator_name)
	{
		fprintf(pih->fout
				, "\n\tdef %s(self, e):\n\t\tprint(\"%s\")\n\t\t# intentionally empty\n\n"
				, translator_name
				, translator_name
				);
	}
	else
	{
		fprintf(pih->fout
				, "\n\tdef translate_%s(self, e):\n\t\tprint(\"translate_%s\")\n\t\t# intentionally empty\n\n"
				, pevent->name
				, pevent->name
				);
	}

	return false;
}

static bool scan_consolidated(pLIST_ELEMENT pelem, void *data)
{
	pCONSOLIDATED_ACTION_INFO pcai = (pCONSOLIDATED_ACTION_INFO) pelem->mbr;
	pITERATOR_HELPER          pih  = (pITERATOR_HELPER) data;

	if (pcai->pai->action && strlen(pcai->pai->action->name))
	{
		pih->error = true;
	}

	iterate_list(pcai->matrices, scan_matrix, pih);

	return false;
}

static bool scan_matrix(pLIST_ELEMENT pelem, void *data)
{
	pMATRIX_INFO     pmatrix = (pMATRIX_INFO) pelem->mbr;
	pITERATOR_HELPER pih     = (pITERATOR_HELPER) data;

	iterate_list(pmatrix->event_list, scan_event, pih);

	return false;
}

static bool scan_event(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO         pevent = (pID_INFO) pelem->mbr;
	pITERATOR_HELPER pih    = (pITERATOR_HELPER) data;

	if (pevent->type_data.event_data.puser_event_data)
	{
		pih->found = true;
	}

	return false;
}

static bool print_actions_dict_consolidated(pLIST_ELEMENT pelem, void *data)
{
	pCONSOLIDATED_ACTION_INFO pcai = (pCONSOLIDATED_ACTION_INFO) pelem->mbr;
	pITERATOR_HELPER          pih  = (pITERATOR_HELPER) data;

	if (!pcai->pai->action || !strlen(pcai->pai->action->name))
	{
		return false;
	}

	pih->pai = pcai->pai;
	iterate_list(pcai->matrices, print_actions_dict_matrix, pih);

	return false;
}

static bool print_actions_dict_matrix(pLIST_ELEMENT pelem, void *data)
{
	pMATRIX_INFO     pmatrix = (pMATRIX_INFO) pelem->mbr;
	pITERATOR_HELPER pih     = (pITERATOR_HELPER) data;

	pih->pOtherElem = pelem;
	iterate_list(pmatrix->event_list, print_actions_dict_event, pih);

	return false;
}

static bool print_actions_dict_event(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO         pevent  = (pID_INFO) pelem->mbr;
	pITERATOR_HELPER pih     = (pITERATOR_HELPER) data;
	pMATRIX_INFO     pmatrix = (pMATRIX_INFO) pih->pOtherElem->mbr;

	pih->pid = pevent;
	iterate_list(pmatrix->state_list, print_actions_dict_state, pih);

	return false;
}

static bool print_actions_dict_state(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO         pstate = (pID_INFO) pelem->mbr;
	pITERATOR_HELPER pih    = (pITERATOR_HELPER) data;

	fprintf(pih->fout
			, "\t\t(\"%s\", \"%s\"): \"%s\",\n"
			, pstate->name
			, pih->pid->name
			, pih->pai->action->name
			);

	return false;
}
