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

static bool print_states(pLIST_ELEMENT,void*);
static bool print_consolidated_action_info(pLIST_ELEMENT,void*);
static bool print_matrices(pLIST_ELEMENT,void*);
static bool print_trigger(pLIST_ELEMENT,void*);
static bool print_action_stubs(pLIST_ELEMENT,void*);
static void print_transition_as_dict(pID_INFO,pITERATOR_HELPER);
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

	if (NULL != (pfsmpytog->ptd = calloc(1, sizeof(pPyTransitionsData))))
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
			, "from Transitions import Machine\n"
			);

	if (pmi->native_impl_prologue)
	{
		fprintf(fout
				, "%s\n"
				, pmi->native_impl_prologue
			   );
	}

	fprintf(fout
			, "\nclass %s(object):\n\n"
			, pmi->name->name
			);

	// Write the states
	fprintf(fout
			, "\tstates = ["
			);

	ih.first = true;
	iterate_list(pmi->state_list
				 , print_states
				 , &ih
				 );

	fprintf(fout
			, "]\n"
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
			, "\t\tself.machine = Machine(states=self.states, "
			  "transitions=self.transitions, initial='%s')\n"
			, stateNameByIndex(pmi, 0)
			);

	fprintf(fout
			, "\n"
			);

	// Write action stubs
	if (generate_weak_fns)
	{
		iterate_list(pmi->action_list
					 , print_action_stubs
					 , &ih
					 );
	}
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

static bool print_states(pLIST_ELEMENT pelem, void *data)
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

	if (pih->pai->action && strlen(pih->pai->action->name))
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
	pMATRIX_INFO     pmi    = (pMATRIX_INFO) pih->pOtherElem->mbr;

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
				 , print_states
				 , pih
				 );
	pih->first = false;

	if (pmi->state_list->count > 1)
	{
		fprintf(pih->fout
				, "] "
				);
	}

	fprintf(pih->fout
			, ", '%s']\n"
			, pih->pai->transition ? pih->pai->transition->name : "None"
			);

}

static void print_transition_as_dict(pID_INFO pevent, pITERATOR_HELPER pih)
{
	pMATRIX_INFO     pmi    = (pMATRIX_INFO) pih->pOtherElem->mbr;

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
				 , print_states
				 , pih
				 );
	pih->first = false;

	if (pmi->state_list->count > 1)
	{
		fprintf(pih->fout
				, "]\n"
				);
	}

	fprintf(pih->fout
			, "\t%*.*s  , 'dest': '%s'\n"
			, transitions_str_len
			, transitions_str_len
			, " "
			, pih->pai->transition ? pih->pai->transition->name : "None"
			);

	fprintf(pih->fout
			, "\t%*.*s  , 'before': '%s'\n"
			, transitions_str_len
			, transitions_str_len
			, " "
			, pih->pai->action->name
			);

	fprintf(pih->fout
			, "\t%*.*s}\n"
			, transitions_str_len
			, transitions_str_len
			, " "
			);

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

