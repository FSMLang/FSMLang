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

static int  initPlantUMLWriter(pFSMOutputGenerator,char *);
static void writePlantUMLWriter(pFSMOutputGenerator,pMACHINE_INFO);
static void closePlantUMLWriter(pFSMOutputGenerator,int);

static int  initPlantUMLFileNameWriter(pFSMOutputGenerator,char *);
static void writePlantUMLFileName(pFSMOutputGenerator,pMACHINE_INFO);
static void closePlantUMLFileNameWriter(pFSMOutputGenerator,int);

typedef struct _fsm_puml_output_generator_ FSMPlantUMLOutputGenerator, *pFSMPlantUMLOutputGenerator;
typedef struct _puml_machine_data_ PlantUMLMachineData, *pPlantUMLMachineData;
typedef struct _loop_back_event_ LOOP_BACK_EVENT, *pLOOP_BACK_EVENT;

struct _puml_machine_data_ {

	FILE	        *pumlFile;
	char	        *pumlName;
  pMACHINE_INFO pmi;
};

struct _fsm_puml_output_generator_
{
   FSMOutputGenerator fsmog;
   pPlantUMLMachineData   pmd;
};

struct _loop_back_event_
{
   pID_INFO     pevent;
   pACTION_INFO pai;
};

FSMPlantUMLOutputGenerator PlantUMLMachineWriter = {
	{
     initPlantUMLWriter,
     writePlantUMLWriter,
     closePlantUMLWriter
  },
  NULL
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
         fprintf(pfsmpumlog->pmd->pumlFile,"\tThis file automatically generated by FSMLang\n\n");
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
		pFSMPlantUMLOutputGenerator pfsmhtmlog = calloc(1, sizeof(FSMPlantUMLOutputGenerator));

		pfsmhtmlog->fsmog.writeMachine = writePlantUMLWriter;
		pfsmhtmlog->fsmog.initOutput   = initPlantUMLWriter;
		pfsmhtmlog->fsmog.closeOutput  = closePlantUMLWriter;

		pfsmog = (pFSMOutputGenerator)pfsmhtmlog;
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

