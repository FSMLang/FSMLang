/**
	fsm_plantuml.c

 *  Creates PlantUML source to describe Finite State Machines


		FSMLang (fsm) - A Finite State Machine description language.
    Copyright (C) 2022  Steven Stanton

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

    Steven Stanton fsmlang@pesticidesoftware.com

    For the latest on FSMLang: http://fsmlang.pesicidesoftware.com

		And, finally, your possession of this source code implies nothing.

	Long Description:

	Creation: 	sstanton		Nov-26-2022
*/

#include "fsm_plantuml.h"
#include "list.h"
//#include "event_sequences.h"

#include <stdio.h>
#include <stdlib.h>

#include <time.h>
#if defined (LINUX) || defined (CYGWIN)
#include <string.h>
#include <unistd.h>
#endif

#if defined (CYGWIN) || defined (LINUX)
#include "y.tab.h"
#elif defined (VS)
#include "parser.h"
#endif

/*
	Our interface to the outside world
*/
bool                 add_plantuml_title                   = false;
bool                 add_plantuml_legend                  = false;
bool                 exclude_events_from_plantuml_legend  = false;
bool                 exclude_states_from_plantuml_legend  = false;
bool                 exclude_actions_from_plantuml_legend = false;
HORIZONTAL_PLACEMENT plantuml_legend_horizontal_placement = hp_center;
VERTICAL_PLACEMENT   plantuml_legend_vertical_placement   = vp_bottom;

typedef struct _fsm_puml_output_generator_ FSMPlantUMLOutputGenerator, *pFSMPlantUMLOutputGenerator;
typedef struct _puml_machine_data_ PlantUMLMachineData, *pPlantUMLMachineData;
typedef struct _loop_back_event_ LOOP_BACK_EVENT, *pLOOP_BACK_EVENT;
typedef struct _fsm_puml_sq_output_generator_ FSMPlantUMLSqOutputGenerator, *pFSMPlantUMLSqOutputGenerator;
typedef struct _fsmpumlsq_og_factory_str_ FSMPumlSqOgFactoryStr, *pFSMPumlSqOgFactoryStr;
typedef struct _sequence_iterator_helper_ SEQUENCE_ITERATOR_HELPER, *pSEQUENCE_ITERATOR_HELPER;

static pFSMPlantUMLSqOutputGenerator generatePlantUMLSequenceWriter(unsigned,pEVENT_SEQUENCE);
static pFSMPlantUMLSqOutputGenerator generatePlantUMLSeqFNWriter(unsigned,pEVENT_SEQUENCE);

static char *create_sequence_name(unsigned);
static bool write_sequence(pLIST_ELEMENT,void*);
static void write_sequences(pMACHINE_INFO);

static void write_sequence_names(pMACHINE_INFO);

static int  initPlantUMLWriter(pFSMOutputGenerator,char *);
static void writePlantUMLWriter(pFSMOutputGenerator,pMACHINE_INFO);
static void closePlantUMLWriter(pFSMOutputGenerator,int);

static int  initPlantUMLFileNameWriter(pFSMOutputGenerator,char *);
static void writePlantUMLFileName(pFSMOutputGenerator,pMACHINE_INFO);
static void closePlantUMLFileNameWriter(pFSMOutputGenerator,int);

static int initPlantUMLEventSequenceWriter(pFSMOutputGenerator,char*);
static void writePlantUMLEventSequence(pFSMOutputGenerator,pMACHINE_INFO);

static int  initPlantUMLSeqFNWriter(pFSMOutputGenerator,char *);
static void writePlantUMLSeqFN(pFSMOutputGenerator,pMACHINE_INFO);

typedef pFSMPlantUMLSqOutputGenerator (*fpGeneratePlantUMLSequenceWriter)(unsigned,pEVENT_SEQUENCE);

struct _sequence_iterator_helper_
{
	ITERATOR_HELPER ih;
	pID_INFO        pcurr_state;
	pLIST           pvisited_states;
};

struct _puml_machine_data_ {

	FILE	        *pumlFile;
	char	        *pumlName;
	pMACHINE_INFO    pmi;
};

struct _fsm_puml_output_generator_
{
   FSMOutputGenerator          fsmog;
   pPlantUMLMachineData        pmd;
   pFSMPlantUMLOutputGenerator parent;
};

struct _fsm_puml_sq_output_generator_
{
   FSMPlantUMLOutputGenerator fsmpumlog;
   pEVENT_SEQUENCE            sequence;
   unsigned                   ordinal;
   char                       *name;
};

struct _fsmpumlsq_og_factory_str_
{
	pMACHINE_INFO                    pmi;
	fpGeneratePlantUMLSequenceWriter factory;
};

struct _loop_back_event_
{
   pID_INFO     pevent;
   pACTION_INFO pai;
};

FSMPlantUMLOutputGenerator PlantUMLMachineWriter = {
	{
		initPlantUMLWriter
		, writePlantUMLWriter
		, closePlantUMLWriter
		, generatePlantUMLMachineWriter
  }
  , NULL
  , NULL
};

const char * const horizontal_placement_strs[] =
{
   [hp_none_given] = ""
   , [hp_left]     = "left"
   , [hp_right]    = "right"
   , [hp_center]   = ""                      // this is the plantuml default
};

const char * const vertical_placement_strs[] =
{
   [vp_none_given] = ""
   , [vp_top]      = "top"
   , [vp_bottom]   = ""                      // this is the plantuml default
   , [vp_center]   = "center"
};

/* list iteration callbacks */
static bool hide_unvisited_state(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO                  pstate = (pID_INFO) pelem->mbr;
	pITERATOR_HELPER          pih    = (pITERATOR_HELPER) data;
	pSEQUENCE_ITERATOR_HELPER psih   = (pSEQUENCE_ITERATOR_HELPER) data;

	if (!member_is_in_list(psih->pvisited_states, pstate))
	{
		fprintf(pih->fout
				, "hide %s\n"
				, pstate->name
				);
	}

	return false;
	
}

static bool print_seqence_node(pLIST_ELEMENT pelem, void *data)
{
	pEVENT_SEQUENCE_NODE      pesn  = (pEVENT_SEQUENCE_NODE) pelem->mbr;
	pITERATOR_HELPER          pih   = (pITERATOR_HELPER) data;
	pSEQUENCE_ITERATOR_HELPER psih  = (pSEQUENCE_ITERATOR_HELPER) data;

	pID_INFO paction     = get_action(pih->pmi, pesn->pevent->order, psih->pcurr_state->order);
	pID_INFO ptransition = get_transition(pih->pmi, pesn->pevent->order, psih->pcurr_state->order);

	enum transition_note {tn_none, tn_state_mismatch, tn_fn_mismatch, tn_fn_match, tn_first_return, tn_no_fsm_transition} note_on_transition = tn_none;

	fprintf(pih->fout
			, "%s --> "
			, psih->pcurr_state->name
			);

	if (pesn->pnew_state)
	{
		psih->pcurr_state = pesn->pnew_state;
		if (ptransition)
		{
			if (ptransition->type == STATE && ptransition != pesn->pnew_state)
			{
				note_on_transition = tn_state_mismatch;
			}
			else if (ptransition->type != STATE)
			{
				note_on_transition = member_is_in_list(ptransition->transition_fn_returns_decl, pesn->pnew_state) ? tn_fn_match : tn_fn_mismatch;
			}
		}
		else
		{
			note_on_transition = tn_no_fsm_transition;
		}
	}
	else if (ptransition)
	{
		if (STATE == ptransition->type)
		{
			psih->pcurr_state = ptransition;
		}
		else
		{
			psih->pcurr_state = (pID_INFO) find_nth_list_member(ptransition->transition_fn_returns_decl, 0);
			note_on_transition = tn_first_return;
		}
	}
	fprintf(pih->fout, "%s", psih->pcurr_state->name);

	fprintf(pih->fout
			, ": **Event %u:** %s\\n"
			, pelem->ordinal + 1
			, pesn->pevent->name
			);

	fprintf(pih->fout
			, " **Action:** %s"
			, paction ? paction->name : "None"
			);

	fprintf(pih->fout, "\n");

	if (note_on_transition != tn_none)
	{
		fprintf(pih->fout
				, "note on link\n"
				);
		switch (note_on_transition)
		{
		case tn_no_fsm_transition:
			fprintf(pih->fout
					, "The sequence indicates a tranisition, but none is given in the FSM."
					);
			break;
		case tn_state_mismatch:
			fprintf(pih->fout
					, "The tranisition indicated in the sequence does not match the FSM."
					);
			break;
		case tn_fn_mismatch:
		case tn_fn_match:
			fprintf(pih->fout
					, "State machine indicates transition is via function %s.\n\nThe transition given in the sequence is %sfound in the function's return declaration."
					, ptransition->name
					, note_on_transition == tn_fn_match ? "" : "not "
					);
			break;
		case tn_first_return:
			fprintf(pih->fout
					, "State machine indicates transition is via function %s and no transition was indicated in the sequence.\n\n"
					, ptransition->name
					);
			fprintf(pih->fout, "The first indicated return value was chosen.");
			break;
		default:
			break;
		}
		fprintf(pih->fout
				, "\nend note\n"
				);
	}

	add_unique_to_list(psih->pvisited_states, psih->pcurr_state);

	return false;
}

static bool print_sharing_machines(pLIST_ELEMENT pelem, void *data)
{
   pMACHINE_INFO               pmi        = (pMACHINE_INFO) pelem->mbr;
   pFSMPlantUMLOutputGenerator pfsmpumlog = (pFSMPlantUMLOutputGenerator) data;

   fprintf(pfsmpumlog->pmd->pumlFile
           , "%s\\n"
           , pmi->name->name
           );

   return false;
}

static bool print_sharing_machines_in_note(pLIST_ELEMENT pelem, void *data)
{
   pMACHINE_INFO               pmi        = (pMACHINE_INFO) pelem->mbr;
   pFSMPlantUMLOutputGenerator pfsmpumlog = (pFSMPlantUMLOutputGenerator) data;

   fprintf(pfsmpumlog->pmd->pumlFile
           , "%s\n"
           , pmi->name->name
           );

   return false;
}

static bool print_loop_back_event(pLIST_ELEMENT pelem, void *data)
{
   pLOOP_BACK_EVENT            ple        = (pLOOP_BACK_EVENT) pelem->mbr;
   pFSMPlantUMLOutputGenerator pfsmpumlog = (pFSMPlantUMLOutputGenerator) data;

   fprintf(pfsmpumlog->pmd->pumlFile
           ,"%s: %s"
           , ple->pevent->name
           , ple->pai->action ? ple->pai->action->name : ""
           );

   if (ple->pai->docCmnt)
   {
      fprintf(pfsmpumlog->pmd->pumlFile
              , "\n%s\n"
              , ple->pai->docCmnt
              );
   }

   if (ple->pevent->type_data.event_data.psharing_sub_machines)
   {
      fprintf(pfsmpumlog->pmd->pumlFile
              , "\nShared with:\n"
              );

      iterate_list(ple->pevent->type_data.event_data.psharing_sub_machines
                   , print_sharing_machines_in_note
                   , pfsmpumlog
                   );

   }

   fprintf(pfsmpumlog->pmd->pumlFile
           , "\n"
           );

   return false;
}

static bool print_state_name(pLIST_ELEMENT pelem, void *data)
{
   pID_INFO pid                           = ((pID_INFO)pelem->mbr);
   pFSMPlantUMLOutputGenerator pfsmpumlog = (pFSMPlantUMLOutputGenerator) data;

   fprintf(pfsmpumlog->pmd->pumlFile
           , "state %s\n"
           , pid->name
           );

   return false;
}

static bool print_state_docCmnt(pLIST_ELEMENT pelem, void *data)
{
   pID_INFO pid                           = ((pID_INFO)pelem->mbr);
   pFSMPlantUMLOutputGenerator pfsmpumlog = (pFSMPlantUMLOutputGenerator) data;

   if (pid->docCmnt)
   {
      fprintf(pfsmpumlog->pmd->pumlFile
              , "%s: "
              , pid->name
              );
      for (char *cp = pid->docCmnt; *cp; cp++)
      {
         switch (*cp)
         {
               case '\n':
                  fputc('\\', pfsmpumlog->pmd->pumlFile);
                  fputc('n', pfsmpumlog->pmd->pumlFile);
                  break;
               case '\r':
                  break;
               default:
                  fputc(*cp, pfsmpumlog->pmd->pumlFile);
         }
      }
      fputc('\\', pfsmpumlog->pmd->pumlFile);
      fputc('n', pfsmpumlog->pmd->pumlFile);
      fputc('\n', pfsmpumlog->pmd->pumlFile);
   }

   return false;
}


static bool print_state_entry_fn(pLIST_ELEMENT pelem, void *data)
{
   pID_INFO pid                           = ((pID_INFO)pelem->mbr);
   pFSMPlantUMLOutputGenerator pfsmpumlog = (pFSMPlantUMLOutputGenerator) data;

   if (pid->type_data.state_data.state_flags & sfHasEntryFn)
   {
      fprintf(pfsmpumlog->pmd->pumlFile
              , "%s: **Entry Function:** %s%s\n"
              , pid->name
              , pid->type_data.state_data.entry_fn
                ? pid->type_data.state_data.entry_fn->name
                : "onEntryTo_"
              , pid->type_data.state_data.entry_fn
                ? ""
                : pid->name
              );
   }

   return false;
}

static bool print_state_exit_fn(pLIST_ELEMENT pelem, void *data)
{
   pID_INFO pid                           = ((pID_INFO)pelem->mbr);
   pFSMPlantUMLOutputGenerator pfsmpumlog = (pFSMPlantUMLOutputGenerator) data;

   if (pid->type_data.state_data.state_flags & sfHasExitFn)
   {
      fprintf(pfsmpumlog->pmd->pumlFile
              , "%s: **Exit Function:** %s%s\n"
              , pid->name
              , pid->type_data.state_data.exit_fn
                ? pid->type_data.state_data.exit_fn->name
                : "onExitFrom_"
              , pid->type_data.state_data.exit_fn
                ? ""
                : pid->name
              );
   }

   return false;
}

static bool print_state_inhibitions(pLIST_ELEMENT pelem, void *data)
{
   pID_INFO pid                           = ((pID_INFO)pelem->mbr);
   pFSMPlantUMLOutputGenerator pfsmpumlog = (pFSMPlantUMLOutputGenerator) data;

   if (pid->type_data.state_data.state_flags & sfInibitSubMachines)
   {
      fprintf(pfsmpumlog->pmd->pumlFile
              , "%s: Inhibits sub-machines\n"
              , pid->name
              );
   }

   return false;
}


static bool print_transition_options(pLIST_ELEMENT pelem, void *data)
{
   pID_INFO pid         = ((pID_INFO) pelem->mbr);
   pITERATOR_HELPER pih = (pITERATOR_HELPER) data;

   fprintf(pih->fout
           , "%s --> %s\n"
           , pih->pid->name
           , pid->name
           );

   return false;
}

static bool print_list_for_legend(pLIST_ELEMENT pelem, void *data)
{
   pID_INFO pid                           = ((pID_INFO) pelem->mbr);
   pFSMPlantUMLOutputGenerator pfsmpumlog = (pFSMPlantUMLOutputGenerator) data;

   if (pid->name && strlen(pid->name))
   {
      fprintf(pfsmpumlog->pmd->pumlFile
              ,"|  %s  |  %s  |\n"
              , pid->name
              , pid->docCmnt ? pid->docCmnt : ""
              );
   }
  
  return false;
  
}

static bool print_prefix_string(pLIST_ELEMENT pelem, void *data)
{
   char *str  = (char *) pelem->mbr;
   FILE *fout = (FILE *) data;

   fprintf(fout, "%s\n", str);

   return false;
}

static void writeLegend(pFSMPlantUMLOutputGenerator pfsmpumlog)
{
   fprintf(pfsmpumlog->pmd->pumlFile
           , "hide empty description\n"
           );

   fprintf(pfsmpumlog->pmd->pumlFile
           ,"\nlegend %s %s\n"
           , vertical_placement_strs[plantuml_legend_vertical_placement]
           , horizontal_placement_strs[plantuml_legend_horizontal_placement]
           );

   fprintf(pfsmpumlog->pmd->pumlFile
           ,"%s\n\n%s\n"
           , pfsmpumlog->pmd->pmi->name->name
           , pfsmpumlog->pmd->pmi->name->docCmnt
             ? pfsmpumlog->pmd->pmi->name->docCmnt
             : ""
          );

   static char *table_hdr = "\n%s\n|  Name  |  Comment  |\n";
   if (!exclude_states_from_plantuml_legend)
   {
      fprintf(pfsmpumlog->pmd->pumlFile
              , table_hdr
              , "States"
             );

      iterate_list(pfsmpumlog->pmd->pmi->state_list
                   , print_list_for_legend
                   , pfsmpumlog
                  );
   }

   if (!exclude_events_from_plantuml_legend)
   {
      fprintf(pfsmpumlog->pmd->pumlFile
              , table_hdr
              , "Events"
             );

      iterate_list(pfsmpumlog->pmd->pmi->event_list
                   , print_list_for_legend
                   , pfsmpumlog
                  );
   }

   if (!exclude_actions_from_plantuml_legend)
   {
      fprintf(pfsmpumlog->pmd->pumlFile
              , table_hdr
              , "Actions"
             );

      iterate_list(pfsmpumlog->pmd->pmi->action_list
                   , print_list_for_legend
                   , pfsmpumlog
                  );
   }

   fprintf(pfsmpumlog->pmd->pumlFile
           ,"\nend legend\n\n"
          );
}

static bool print_prefix_file(pLIST_ELEMENT pelem, void *data)
{
   char *fName  = (char *) pelem->mbr;
   FILE *fout = (FILE *) data;

   copyFileContents(fout, fName);

   return false;
}

/* Main section */
static int initPlantUMLFileNameWriter (pFSMOutputGenerator pfsmog, char *baseFileName)
{
	pFSMPlantUMLOutputGenerator pfsmpumlog = (pFSMPlantUMLOutputGenerator) pfsmog;
	if (NULL != (pfsmpumlog->pmd = calloc(1, sizeof(PlantUMLMachineData))))
	{
		if (baseFileName)
		{
			pfsmpumlog->pmd->pumlName = createFileName(baseFileName,".plantuml");
		}
	}

	/* this may look funny, but it does the trick */
	return ((int) !pfsmpumlog->pmd->pumlName);
}

static int initPlantUMLWriter (pFSMOutputGenerator pfsmog, char *baseFileName)
{

  pFSMPlantUMLOutputGenerator pfsmpumlog = (pFSMPlantUMLOutputGenerator) pfsmog;

  if (NULL != (pfsmpumlog->pmd = calloc(1, sizeof(PlantUMLMachineData))))
  {
     if (!baseFileName)
     {

       pfsmpumlog->pmd->pumlFile = stdout;

     }
     else {

       pfsmpumlog->pmd->pumlName = createFileName(baseFileName,".plantuml");

       if (!(pfsmpumlog->pmd->pumlFile = openFile(pfsmpumlog->pmd->pumlName,"w"))) {

         CHECK_AND_FREE(pfsmpumlog->pmd->pumlName);

       }
       else {

         /* we're good to go; write the preamble */
         fprintf(pfsmpumlog->pmd->pumlFile,"/'\n\t%s\n\n",pfsmpumlog->pmd->pumlName);
         fprintf(pfsmpumlog->pmd->pumlFile,"\tThis file generated by FSMLang\n\n");
         fprintf(pfsmpumlog->pmd->pumlFile,"\n'/\n\n");

         fprintf(pfsmpumlog->pmd->pumlFile,"@startuml\n");

         if (pplantuml_prefix_strings_list)
         {
            iterate_list(pplantuml_prefix_strings_list
                         , print_prefix_string
                         , pfsmpumlog->pmd->pumlFile
                        );
            fprintf(pfsmpumlog->pmd->pumlFile,"\n");
         }

         if (pplantuml_prefix_files_list)
         {
            iterate_list(pplantuml_prefix_files_list
                         , print_prefix_file
                         , pfsmpumlog->pmd->pumlFile
                        );
            fprintf(pfsmpumlog->pmd->pumlFile,"\n");
         }

       }

     }
  }

	/* this may look funny, but it does the trick */
	return ((int) !pfsmpumlog->pmd->pumlFile);

}

static void writePlantUMLFileName(pFSMOutputGenerator pfsmog, pMACHINE_INFO pmi)
{
	pFSMPlantUMLOutputGenerator pfsmpumlog = (pFSMPlantUMLOutputGenerator)pfsmog;

	printf("%s ", pfsmpumlog->pmd->pumlName);

	if (pmi->machine_list)
	{
		write_machines(pmi->machine_list, generatePlantUMLMachineWriter, pfsmog);
	}

   write_sequence_names(pmi);

	if (output_make_recipe && !pfsmpumlog->parent)
	{
		printf(": %s.fsm\n"
			   , inputFileName
			   );

		printf("\t$(FSM) -tp $(FSM_PLANTUML_FLAGS) $<\n\n");
	}
}

static void writePlantUMLWriter(pFSMOutputGenerator pfsmog, pMACHINE_INFO pmi)
{

	pID_INFO			  pevent;
	pACTION_INFO	  pai;
	unsigned        e,s;
  ITERATOR_HELPER ih;

  pFSMPlantUMLOutputGenerator pfsmpumlog = (pFSMPlantUMLOutputGenerator) pfsmog;

	if (!pmi)

		return;

  pfsmpumlog->pmd->pmi = pmi;

  if (add_plantuml_title)
  {
     fprintf(pfsmpumlog->pmd->pumlFile, "title ");
	 printAncestry(pmi, pfsmpumlog->pmd->pumlFile, "::", alc_lower, ai_include_self);
     fprintf(pfsmpumlog->pmd->pumlFile, "\n");
  }

  if (add_plantuml_legend)
  {
     writeLegend(pfsmpumlog);
  }

  iterate_list(pmi->state_list, print_state_name, pfsmpumlog);
  if (!add_plantuml_legend || (add_plantuml_legend && exclude_states_from_plantuml_legend))
  {
     iterate_list(pmi->state_list, print_state_docCmnt, pfsmpumlog);
  }
  iterate_list(pmi->state_list, print_state_entry_fn, pfsmpumlog);
  iterate_list(pmi->state_list, print_state_exit_fn, pfsmpumlog);
  iterate_list(pmi->state_list, print_state_inhibitions, pfsmpumlog);

  /* show the initial state */
  fprintf(pfsmpumlog->pmd->pumlFile
         , "\n[*] --> %s\n"
         , stateNameByIndex(pmi, 0)
         );

	for (s = 0; s < pmi->state_list->count; s++) {

     pLIST loopBackEvents = init_list();

			for (e = 0; e < pmi->event_list->count; e++) {
            pai = pmi->actionArray[e][s];

        if (pai)
        {
           pevent = eventPidByIndex(pmi, e);

           if (pai->transition)
           {
              if (pai->transition->type == STATE)
              {
                 /* simple state transition */
                 fprintf(pfsmpumlog->pmd->pumlFile
                         , pevent->type_data.event_data.shared_with_parent
                           ? "%s --> %s : **Event:** (%s::) %s\\n**Action:** %s"
                           : "%s --> %s : **Event:** %s%s\\n**Action:** %s"
                         , stateNameByIndex(pmi, s)
                         , pai->transition->name
                         , pevent->type_data.event_data.shared_with_parent
                           ? pmi->parent->name->name
                           : ""
                         , pevent->name
                         , strlen(pai->action->name)
                             ? pai->action->name
                             : "none"
                         );

                 if (pevent->type_data.event_data.psharing_sub_machines)
                 {
                    fprintf(pfsmpumlog->pmd->pumlFile
                            , "\\n**Event shared with:**\\n"
                            );

                    iterate_list(pevent->type_data.event_data.psharing_sub_machines
                                 , print_sharing_machines
                                 , pfsmpumlog
                                 );

                    fprintf(pfsmpumlog->pmd->pumlFile
                            , "\n"
                            );

                 }

                 fprintf(pfsmpumlog->pmd->pumlFile
                         , "\n"
                         );

                 if (pai->docCmnt)
                 {
                    fprintf(pfsmpumlog->pmd->pumlFile
                            , "note on link\n%s\nend note\n"
                            , pai->docCmnt
                            );
                 }
              }
              else
              {
                 /* first, declare the choice */
                 fprintf(pfsmpumlog->pmd->pumlFile
                         , "state %s <<choice>>\n"
                         , pai->transition->name
                         );

                 /* then the state that transitions via the choice */
                 fprintf(pfsmpumlog->pmd->pumlFile
                         , "%s --> %s : **Event:** %s\\n**Action:** %s\\n**Choice:** %s\n"
                         , stateNameByIndex(pmi, s)
                         , pai->transition->name
                         , eventNameByIndex(pmi, e)
                         , pai->action
                             ? pai->action->name
                             : ""
                         , pai->transition->name
                         );

                 if (pai->transition->transition_fn_returns_decl)
                 {
                    ih.pid = pai->transition;
                    ih.fout = pfsmpumlog->pmd->pumlFile;

                    iterate_list(ih.pid->transition_fn_returns_decl
                                 , print_transition_options
                                 , &ih
                                 );
                 }
                 else
                 {
                    fprintf(stderr
                            , "**transition function:** %s\n"
                            , pai->transition->name
                            );
                    yyerror("It is required to declare what the transition function returns.");
                 }
              }

           }
           else
           {
              /* simple "no transition" 
              */
              pLOOP_BACK_EVENT plbe = malloc(sizeof(LOOP_BACK_EVENT));

              plbe->pevent = pevent;
              plbe->pai    = pai;

              add_to_list(loopBackEvents, plbe);
           }
				}

			}

      /* now write any loop back events */
      switch (loopBackEvents->count)
      {
      case 0:
         //do nothing
         break;
      case 1:
         pevent = ((pLOOP_BACK_EVENT)loopBackEvents->head->mbr)->pevent;
         pai    = ((pLOOP_BACK_EVENT)loopBackEvents->head->mbr)->pai;

         fprintf(pfsmpumlog->pmd->pumlFile
                 , pevent->type_data.event_data.shared_with_parent
                   ? "%s --> %s : **Event:** (%s::) %s\\n**Action:** %s\n"
                   : "%s --> %s : **Event:** %s%s\\n**Action:** %s\n"
                 , stateNameByIndex(pmi, s)
                 , stateNameByIndex(pmi, s)
                 , pevent->type_data.event_data.shared_with_parent
                   ? pmi->parent->name->name
                   : ""
                 , pevent->name
                 , pai->action
                     ? pai->action->name
                     : "noAction"
                 );

         if (pai->docCmnt)
         {
            fprintf(pfsmpumlog->pmd->pumlFile
                    , "\nnote on link\n%s\nend note\n"
                    , pai->docCmnt
                   );
         }
         break;
      default:
         fprintf(pfsmpumlog->pmd->pumlFile
                 , "\nnote left of %s\n**These events loop back:**\n\n"
                 , stateNameByIndex(pmi, s)
                 );

         iterate_list(loopBackEvents, print_loop_back_event, pfsmpumlog);

         fprintf(pfsmpumlog->pmd->pumlFile
                 , "end note\n"
                 );
         break;
      }

      free_list(loopBackEvents);

	}
	
  if (pmi->machine_list)
  {
     write_machines(pmi->machine_list, generatePlantUMLMachineWriter, pfsmog);
  }

  write_sequences(pmi);

}

static void closePlantUMLFileNameWriter(pFSMOutputGenerator pfsmog, int good)
{
	pFSMPlantUMLOutputGenerator pfsmpumlog = (pFSMPlantUMLOutputGenerator) pfsmog;

	(void) good;

	if (!pfsmpumlog || !pfsmpumlog->pmd)
	{
		return;
	}

	CHECK_AND_FREE(pfsmpumlog->pmd->pumlName);
}

static void closePlantUMLWriter(pFSMOutputGenerator pfsmog, int good)
{
   pFSMPlantUMLOutputGenerator pfsmpumlog = (pFSMPlantUMLOutputGenerator) pfsmog;

	if (good) {

		fprintf(pfsmpumlog->pmd->pumlFile,"@enduml\n");

	}

	fclose(pfsmpumlog->pmd->pumlFile);

	if (!good) {

		unlink(pfsmpumlog->pmd->pumlName);

	}

	CHECK_AND_FREE(pfsmpumlog->pmd->pumlName);

}

pFSMOutputGenerator generatePlantUMLMachineWriter(pFSMOutputGenerator parent)
{
	pFSMOutputGenerator pfsmog;

	if (parent)
	{
		pFSMPlantUMLOutputGenerator pfsmplantumlog = calloc(1, sizeof(FSMPlantUMLOutputGenerator));

		pfsmplantumlog->fsmog.writeMachine = writePlantUMLWriter;
		pfsmplantumlog->fsmog.initOutput   = initPlantUMLWriter;
		pfsmplantumlog->fsmog.closeOutput  = closePlantUMLWriter;

		pfsmplantumlog->parent = (pFSMPlantUMLOutputGenerator) parent;

		pfsmog = (pFSMOutputGenerator)pfsmplantumlog;
	}
	else
	{
		pfsmog = (pFSMOutputGenerator) &PlantUMLMachineWriter;
	}

	if (output_generated_file_names_only)
	{
		pfsmog->writeMachine = writePlantUMLFileName;
		pfsmog->initOutput   = initPlantUMLFileNameWriter;
		pfsmog->closeOutput  = closePlantUMLFileNameWriter;
	}

	return pfsmog;
}

static pFSMPlantUMLSqOutputGenerator generatePlantUMLSequenceWriter(unsigned ordinal, pEVENT_SEQUENCE sequence)
{
	pFSMPlantUMLSqOutputGenerator pfsmplantumlsqog = calloc(1, sizeof(FSMPlantUMLSqOutputGenerator));

	if (pfsmplantumlsqog)
	{
		pfsmplantumlsqog->fsmpumlog.fsmog.writeMachine = writePlantUMLEventSequence;
		pfsmplantumlsqog->fsmpumlog.fsmog.initOutput   = initPlantUMLEventSequenceWriter;
		pfsmplantumlsqog->fsmpumlog.fsmog.closeOutput  = closePlantUMLWriter;
		
		pfsmplantumlsqog->sequence = sequence;
		pfsmplantumlsqog->ordinal  = ordinal;
	}

	return pfsmplantumlsqog;
}

static pFSMPlantUMLSqOutputGenerator generatePlantUMLSeqFNWriter(unsigned ordinal, pEVENT_SEQUENCE sequence)
{
	pFSMPlantUMLSqOutputGenerator pfsmplantumlsqog = calloc(1, sizeof(FSMPlantUMLSqOutputGenerator));

	if (pfsmplantumlsqog)
	{
		pfsmplantumlsqog->fsmpumlog.fsmog.writeMachine = writePlantUMLSeqFN;
		pfsmplantumlsqog->fsmpumlog.fsmog.initOutput   = initPlantUMLSeqFNWriter;
		pfsmplantumlsqog->fsmpumlog.fsmog.closeOutput  = closePlantUMLFileNameWriter;
		
		pfsmplantumlsqog->sequence = sequence;
		pfsmplantumlsqog->ordinal  = ordinal;
	}

	return pfsmplantumlsqog;
}

static void write_sequences(pMACHINE_INFO pmi)
{
//   process_event_sequences(pmi);

	FSMPumlSqOgFactoryStr factory = {
		.pmi = pmi
		, .factory = generatePlantUMLSequenceWriter
	};

	if (pmi->sequences)
	{
		iterate_list(pmi->sequences, write_sequence, &factory);
	}
}

static void write_sequence_names(pMACHINE_INFO pmi)
{
	FSMPumlSqOgFactoryStr factory = {
		.pmi = pmi
		, .factory = generatePlantUMLSeqFNWriter
	};

	if (pmi->sequences)
	{
		iterate_list(pmi->sequences, write_sequence, &factory);
	}
}

static bool write_sequence(pLIST_ELEMENT pelem, void *data)
{
	pEVENT_SEQUENCE        psequence = (pEVENT_SEQUENCE) pelem->mbr;
	pFSMPumlSqOgFactoryStr factory   = (pFSMPumlSqOgFactoryStr) data;

	pFSMPlantUMLSqOutputGenerator pfsmpumlsqog = (*factory->factory)(pelem->ordinal, psequence);

	if (pfsmpumlsqog)
	{
		pfsmpumlsqog->fsmpumlog.fsmog.initOutput((pFSMOutputGenerator) pfsmpumlsqog, psequence->name ? psequence->name->name : NULL);
		pfsmpumlsqog->fsmpumlog.fsmog.writeMachine((pFSMOutputGenerator) pfsmpumlsqog, factory->pmi);
		pfsmpumlsqog->fsmpumlog.fsmog.closeOutput((pFSMOutputGenerator)pfsmpumlsqog, 1);

		free(pfsmpumlsqog);
	}

	return false;
}

static char *create_sequence_name(unsigned ordinal)
{
	FILE *tmp = tmpfile();
	char *cp  = NULL;

	if (tmp)
	{
		fprintf(tmp
				, "Sequence_%u"
				, ordinal
				);
		cp = create_string_from_file(tmp, NULL);
	}

	return cp;
}

static int initPlantUMLEventSequenceWriter (pFSMOutputGenerator pfsmog, char *baseFileName)
{
	(void) baseFileName;
	int                           retVal;
	pFSMPlantUMLSqOutputGenerator pfsmpumlsqog = (pFSMPlantUMLSqOutputGenerator) pfsmog;

	pfsmpumlsqog->name = baseFileName ? baseFileName : create_sequence_name(pfsmpumlsqog->ordinal);

	retVal = initPlantUMLWriter((pFSMOutputGenerator) pfsmpumlsqog
								, pfsmpumlsqog->name
								);

	return retVal;
}

static void writePlantUMLEventSequence(pFSMOutputGenerator pfsmog, pMACHINE_INFO pmi)
{
	pFSMPlantUMLSqOutputGenerator pfsmpumlsqog = (pFSMPlantUMLSqOutputGenerator) pfsmog;
	SEQUENCE_ITERATOR_HELPER      sih;

	sih.ih.fout         = pfsmpumlsqog->fsmpumlog.pmd->pumlFile;
	sih.ih.pmi          = pmi;
	sih.pcurr_state     = pfsmpumlsqog->sequence->initial_state;
	sih.pvisited_states = init_list();

	if (add_plantuml_title)
	{
		fprintf(sih.ih.fout
				, "title %s\n"
				, pfsmpumlsqog->sequence->name ? pfsmpumlsqog->name : pfsmpumlsqog->name
				);
	}
	iterate_list(pmi->state_list, print_state_name, pfsmpumlsqog);

	fprintf(sih.ih.fout
			, "[*] --> %s\n"
			, sih.pcurr_state->name
			);

	add_unique_to_list(sih.pvisited_states, sih.pcurr_state);

	iterate_list(pfsmpumlsqog->sequence->sequence
				 , print_seqence_node
				 , &sih
				 );

	//remove any states not visited
	fprintf(sih.ih.fout, "\n");
	iterate_list(pmi->state_list, hide_unvisited_state, &sih);
	
}

static int  initPlantUMLSeqFNWriter(pFSMOutputGenerator pfsmog, char *baseFileName)
{
	int                           retVal;
	pFSMPlantUMLSqOutputGenerator pfsmpumlsqog = (pFSMPlantUMLSqOutputGenerator) pfsmog;

	pfsmpumlsqog->name = baseFileName ? baseFileName : create_sequence_name(pfsmpumlsqog->ordinal);
	retVal = initPlantUMLFileNameWriter((pFSMOutputGenerator) pfsmpumlsqog
										, pfsmpumlsqog->name
										);

	return retVal;
}

static void writePlantUMLSeqFN(pFSMOutputGenerator pfsmog, pMACHINE_INFO pmi)
{
	(void) pmi;

	pFSMPlantUMLOutputGenerator pfsmpumlog = (pFSMPlantUMLOutputGenerator)pfsmog;

	printf("%s ", pfsmpumlog->pmd->pumlName);

}

