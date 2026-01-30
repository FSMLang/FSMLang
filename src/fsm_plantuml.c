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
#include "event_sequences.h"
#include "util_file_inclusion.h"

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

static pFSMPlantUMLSqOutputGenerator generatePlantUMLSequenceWriter(pFSMPlantUMLOutputGenerator,unsigned,pEVENT_SEQUENCE);
static pFSMPlantUMLSqOutputGenerator generatePlantUMLSeqFNWriter(pFSMPlantUMLOutputGenerator,unsigned,pEVENT_SEQUENCE);

static void escape_newlines(FILE*,char*);

static char* create_sequence_file_name(pFSMPlantUMLSqOutputGenerator);
static bool write_sequence(pLIST_ELEMENT,void*);
static void write_sequences(pFSMPlantUMLOutputGenerator);

static void write_sequence_names(pFSMPlantUMLOutputGenerator);

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

typedef pFSMPlantUMLSqOutputGenerator (*fpGeneratePlantUMLSequenceWriter)(pFSMPlantUMLOutputGenerator,unsigned,pEVENT_SEQUENCE);

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
	pFSMPlantUMLOutputGenerator      pparent_og;
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

	if (!member_is_in_list(psih->st.pvisited_states, pstate))
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

	pID_INFO paction = get_action(pih->pmi
								  , pesn->pevent->order
								  , psih->st.pcurr_state->order
								  );

	fprintf(pih->fout
			, "%s --> "
			, psih->st.pcurr_state->name
			);

	TRANSITION_NOTE note_on_transition = determine_next_state(pih->pmi, pesn, &psih->st);

	fprintf(pih->fout, "%s", psih->st.pcurr_state->name);

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
					, "The sequence indicates a transition, but none is given in the FSM."
					);
			break;
		case tn_state_mismatch:
			fprintf(pih->fout
					, "The transition indicated in the sequence does not match the FSM."
					);
			break;
		case tn_fn_mismatch:
		case tn_fn_match:
			fprintf(pih->fout
					, "State machine indicates transition is via function %s.\n\nThe transition given in the sequence is %sfound in the function's return declaration."
					, psih->st.pcurr_transition->name
					, note_on_transition == tn_fn_match ? "" : "not "
					);
			break;
		case tn_first_return:
			fprintf(pih->fout
					, "State machine indicates transition is via function %s and no transition was indicated in the sequence.\n\n"
					, psih->st.pcurr_transition->name
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

static bool print_guard_name(pLIST_ELEMENT pelem, void *data)
{
   pID_INFO pid                           = ((pID_INFO)pelem->mbr);
   pFSMPlantUMLOutputGenerator pfsmpumlog = (pFSMPlantUMLOutputGenerator) data;

   fprintf(pfsmpumlog->pmd->pumlFile
		   , "state %s <<choice>>\n"
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
	  escape_newlines(pfsmpumlog->pmd->pumlFile, pid->docCmnt);
      fprintf(pfsmpumlog->pmd->pumlFile, "\\n\n");
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
              , "%s: **Entry Function:** %s%s"
              , pid->name
              , pid->type_data.state_data.entry_fn
                ? pid->type_data.state_data.entry_fn->name
                : "onEntryTo_"
              , pid->type_data.state_data.entry_fn
                ? ""
                : pid->name
              );
	   
	  if (
		  pid->type_data.state_data.entry_fn
		  && pid->type_data.state_data.entry_fn->docCmnt
		  )
	  {
		  fprintf(pfsmpumlog->pmd->pumlFile, "\\n");
		  escape_newlines(pfsmpumlog->pmd->pumlFile
						  , pid->type_data.state_data.entry_fn->docCmnt
						  );
	  }
	  fprintf(pfsmpumlog->pmd->pumlFile, "\n");
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
              , "%s: **Exit Function:** %s%s"
              , pid->name
              , pid->type_data.state_data.exit_fn
                ? pid->type_data.state_data.exit_fn->name
                : "onExitFrom_"
              , pid->type_data.state_data.exit_fn
                ? ""
                : pid->name
              );

	  if (
		  pid->type_data.state_data.exit_fn
		  && pid->type_data.state_data.exit_fn->docCmnt
		  )
	  {
		  fprintf(pfsmpumlog->pmd->pumlFile, "\\n");
		  escape_newlines(pfsmpumlog->pmd->pumlFile
						  , pid->type_data.state_data.exit_fn->docCmnt
						  );
	  }

	  fprintf(pfsmpumlog->pmd->pumlFile, "\n");
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
   pID_INFO         pid = ((pID_INFO) pelem->mbr);
   pITERATOR_HELPER pih = (pITERATOR_HELPER) data;

   fprintf(pih->fout
		   , "%s -[#blue]-> %s\n"
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
              ,"|  %s  |  "
              , pid->name
              );
	  escape_newlines(pfsmpumlog->pmd->pumlFile, pid->docCmnt);
      fprintf(pfsmpumlog->pmd->pumlFile, "  |\n");

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

	if (output_make_recipe && !pfsmpumlog->parent)
	{
		printf("%s: %s.fsm "
			   , pfsmpumlog->pmd->pumlName
			   , inputFileName
			   );

		print_included_files_list();

		printf("\n");
	}

	if (!output_make_recipe || pmi->parent)
	{
		printf("%s ", pfsmpumlog->pmd->pumlName);
	}

	pfsmpumlog->pmd->pmi = pmi;
	if (pmi->machine_list)
	{
		write_machines(pmi->machine_list, generatePlantUMLMachineWriter, pfsmog);
	}

	write_sequence_names(pfsmpumlog);

	if (
		(output_make_recipe && !pfsmpumlog->parent)
		&& (pmi->machine_list || pmi->sequences)
		)
	{
		printf(": %s\n"
			   , pfsmpumlog->pmd->pumlName
			   );
	}
}

static void writePlantUMLWriter(pFSMOutputGenerator pfsmog, pMACHINE_INFO pmi)
{
  pID_INFO        pevent;
  pACTION_INFO    pai;
  unsigned        e,s;
  ITERATOR_HELPER ih;

  if (!pmi)

	return;

  pLIST           ptransition_fns_seen = init_list();

  pFSMPlantUMLOutputGenerator pfsmpumlog = (pFSMPlantUMLOutputGenerator) pfsmog;

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

  iterate_list(pmi->transition_fn_list, print_guard_name, pfsmpumlog);

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

              if (pai->transition->type != STATE)
              {
                 if (!member_is_in_list(ptransition_fns_seen, pai->transition))
                 {
                    add_to_list(ptransition_fns_seen, pai->transition);

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
                               , "**guard function:** %s\n"
                               , pai->transition->name
                              );
                       yyerror("It is required to declare what the transition function returns.");
                    }
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

	free_list(ptransition_fns_seen);
	
  if (pmi->machine_list)
  {
     write_machines(pmi->machine_list, generatePlantUMLMachineWriter, pfsmog);
  }

  write_sequences(pfsmpumlog);

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

static pFSMPlantUMLSqOutputGenerator generatePlantUMLSequenceWriter(pFSMPlantUMLOutputGenerator pparent_og, unsigned ordinal, pEVENT_SEQUENCE sequence)
{
	pFSMPlantUMLSqOutputGenerator pfsmplantumlsqog = calloc(1, sizeof(FSMPlantUMLSqOutputGenerator));

	if (pfsmplantumlsqog)
	{
		pfsmplantumlsqog->fsmpumlog.fsmog.writeMachine = writePlantUMLEventSequence;
		pfsmplantumlsqog->fsmpumlog.fsmog.initOutput   = initPlantUMLEventSequenceWriter;
		pfsmplantumlsqog->fsmpumlog.fsmog.closeOutput  = closePlantUMLWriter;
		pfsmplantumlsqog->fsmpumlog.parent             = pparent_og;
		
		pfsmplantumlsqog->sequence = sequence;
		pfsmplantumlsqog->ordinal  = ordinal;
	}

	return pfsmplantumlsqog;
}

static pFSMPlantUMLSqOutputGenerator generatePlantUMLSeqFNWriter(pFSMPlantUMLOutputGenerator pparent_og, unsigned ordinal, pEVENT_SEQUENCE sequence)
{
	pFSMPlantUMLSqOutputGenerator pfsmplantumlsqog = calloc(1, sizeof(FSMPlantUMLSqOutputGenerator));

	if (pfsmplantumlsqog)
	{
		pfsmplantumlsqog->fsmpumlog.fsmog.writeMachine = writePlantUMLSeqFN;
		pfsmplantumlsqog->fsmpumlog.fsmog.initOutput   = initPlantUMLSeqFNWriter;
		pfsmplantumlsqog->fsmpumlog.fsmog.closeOutput  = closePlantUMLFileNameWriter;
		pfsmplantumlsqog->fsmpumlog.parent             = pparent_og;
		
		pfsmplantumlsqog->sequence = sequence;
		pfsmplantumlsqog->ordinal  = ordinal;
	}

	return pfsmplantumlsqog;
}

static void write_sequences(pFSMPlantUMLOutputGenerator pfsmpumlog)
{
//   process_event_sequences(pmi);

	if (pfsmpumlog->pmd->pmi->sequences)
	{
		FSMPumlSqOgFactoryStr factory = {
			.pparent_og = pfsmpumlog
			, .factory = generatePlantUMLSequenceWriter
		};

		iterate_list(pfsmpumlog->pmd->pmi->sequences, write_sequence, &factory);
	}
}

static void write_sequence_names(pFSMPlantUMLOutputGenerator pfsmpumlog)
{
	if (pfsmpumlog->pmd->pmi->sequences)
	{
		FSMPumlSqOgFactoryStr factory = {
			.pparent_og = pfsmpumlog
			, .factory = generatePlantUMLSeqFNWriter
		};

		iterate_list(pfsmpumlog->pmd->pmi->sequences, write_sequence, &factory);
	}
}

static bool write_sequence(pLIST_ELEMENT pelem, void *data)
{
	pEVENT_SEQUENCE        psequence = (pEVENT_SEQUENCE) pelem->mbr;
	pFSMPumlSqOgFactoryStr factory   = (pFSMPumlSqOgFactoryStr) data;

	pFSMPlantUMLSqOutputGenerator pfsmpumlsqog = (*factory->factory)(factory->pparent_og, pelem->ordinal, psequence);

	if (pfsmpumlsqog)
	{
		pfsmpumlsqog->fsmpumlog.fsmog.initOutput((pFSMOutputGenerator) pfsmpumlsqog, psequence->name ? psequence->name->name : NULL);
		pfsmpumlsqog->fsmpumlog.fsmog.writeMachine((pFSMOutputGenerator)pfsmpumlsqog, factory->pparent_og->pmd->pmi);
		pfsmpumlsqog->fsmpumlog.fsmog.closeOutput((pFSMOutputGenerator)pfsmpumlsqog, 1);

		free(pfsmpumlsqog);
	}

	return false;
}

static char* create_sequence_file_name(pFSMPlantUMLSqOutputGenerator pfsmpumlsqog)
{

	FILE *tmp = tmpfile();
	char *cp  = NULL;

	if (tmp)
	{
		if (pfsmpumlsqog->fsmpumlog.parent->parent)
		{
			printAncestry(pfsmpumlsqog->fsmpumlog.parent->pmd->pmi
						  , tmp
						  , "_"
						  , alc_lower
						  , ai_include_self
						  );
			fprintf(tmp,"_");
		}

		fprintf(tmp
				,"%s"
				, pfsmpumlsqog->name
				);

		cp = create_string_from_file(tmp, NULL);
	}

	return cp;
}

static int initPlantUMLEventSequenceWriter (pFSMOutputGenerator pfsmog, char *baseFileName)
{
	int                           retVal       = 1; //assume failure
	pFSMPlantUMLSqOutputGenerator pfsmpumlsqog = (pFSMPlantUMLSqOutputGenerator) pfsmog;
	char                          *tmpFileName = NULL;

	pfsmpumlsqog->name = baseFileName ? baseFileName : create_sequence_name(pfsmpumlsqog->ordinal);

	if (NULL != (tmpFileName = create_sequence_file_name(pfsmpumlsqog)))
	{
		retVal = initPlantUMLWriter((pFSMOutputGenerator) pfsmpumlsqog
									, tmpFileName
									);
	}

	CHECK_AND_FREE(tmpFileName);

	return retVal;
}

static void writePlantUMLEventSequence(pFSMOutputGenerator pfsmog, pMACHINE_INFO pmi)
{
	pFSMPlantUMLSqOutputGenerator pfsmpumlsqog = (pFSMPlantUMLSqOutputGenerator) pfsmog;
	SEQUENCE_ITERATOR_HELPER      sih = {0};

	sih.ih.fout            = pfsmpumlsqog->fsmpumlog.pmd->pumlFile;
	sih.ih.pmi             = pmi;
	sih.st.pcurr_state     = pfsmpumlsqog->sequence->initial_state;
	sih.st.pvisited_states = init_list();

	if (add_plantuml_title)
	{
		fprintf(sih.ih.fout, "title ");
		if (pfsmpumlsqog->fsmpumlog.parent->parent)
		{
			printAncestry(pmi, sih.ih.fout, "::", alc_lower, ai_include_self);
		}
		fprintf(sih.ih.fout
				, "%s%s\n"
				, pfsmpumlsqog->fsmpumlog.parent->parent ? "::" : ""
				, pfsmpumlsqog->name
				);
	}

	if (pfsmpumlsqog->sequence->docCmt)
	{
		fprintf(sih.ih.fout
				, "legend\n%s\nendlegend\n"
				, pfsmpumlsqog->sequence->docCmt
				);
	}

	iterate_list(pmi->state_list, print_state_name, pfsmpumlsqog);

	fprintf(sih.ih.fout
			, "[*] --> %s\n"
			, sih.st.pcurr_state->name
			);

	add_unique_to_list(sih.st.pvisited_states, sih.st.pcurr_state);

	iterate_list(pfsmpumlsqog->sequence->sequence
				 , print_seqence_node
				 , &sih
				 );

	//final state
	fprintf(sih.ih.fout
			, "note bottom of %s\n"
			, sih.st.pcurr_state->name
			);
	if (pfsmpumlsqog->sequence->final_state)
	{
		fprintf(sih.ih.fout
				, "%s was given in the sequence as the final state."
				, pfsmpumlsqog->sequence->final_state->name
				);

		if (sih.st.pcurr_state != pfsmpumlsqog->sequence->final_state)
		{
			fprintf(sih.ih.fout
					, "\n\nThis does not match the last state shown."
					);
		}

	}
	else
	{
		fprintf(sih.ih.fout, "No final state was indicated in the sequence.");
	}
	fprintf(sih.ih.fout, "\nend note\n");

	//remove any states not visited
	fprintf(sih.ih.fout, "\n");
	iterate_list(pmi->state_list, hide_unvisited_state, &sih);

	free_list(sih.st.pvisited_states);
	
}

static int  initPlantUMLSeqFNWriter(pFSMOutputGenerator pfsmog, char *baseFileName)
{
	int                           retVal = 1;  //assume failure
	pFSMPlantUMLSqOutputGenerator pfsmpumlsqog = (pFSMPlantUMLSqOutputGenerator) pfsmog;
	char                          *tmpFileName = NULL;

	pfsmpumlsqog->name = baseFileName ? baseFileName : create_sequence_name(pfsmpumlsqog->ordinal);

	if (NULL != (tmpFileName = create_sequence_file_name(pfsmpumlsqog)))
	{
		retVal = initPlantUMLFileNameWriter((pFSMOutputGenerator)pfsmpumlsqog
											, tmpFileName
											);
	}

	CHECK_AND_FREE(tmpFileName);

	return retVal;
}

static void writePlantUMLSeqFN(pFSMOutputGenerator pfsmog, pMACHINE_INFO pmi)
{
	(void) pmi;

	pFSMPlantUMLOutputGenerator pfsmpumlog = (pFSMPlantUMLOutputGenerator)pfsmog;

	printf("%s ", pfsmpumlog->pmd->pumlName);

}

static void escape_newlines(FILE *fout, char *str)
{
	enum {initial, normal, eating_ws} state = initial;
	char *end_of_initial_ws = NULL;

	if (str)
	{
		for (char *cp = str; *cp; cp++)
		{
			switch (state)
			{
			case initial:
			case normal:
				switch (*cp)
				{
				case '\n':
					fputc('\\', fout);
					fputc('n', fout);
					state = eating_ws;
					break;
				case '\r':
					break;
				case ' ':
				case '\t':
					if (state == initial)
					{
						end_of_initial_ws = cp;
					}
				default:
					fputc(*cp, fout);
					break;
				}
				break;
			case eating_ws:
				switch (*cp)
				{
				case ' ':
				case '\t':
				case '\r':
				case '\n':
					break;
				default:
					for (char *cp_ws = str; cp_ws <= end_of_initial_ws; cp_ws++)
					{
						fputc(*cp_ws, fout);
					}
					fputc(*cp, fout);
					state = normal;
					break;
				}
				break;
			}
		}
	}
}

