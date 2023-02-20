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
int initPlantUMLWriter(pFSMOutputGenerator,char *);
void writePlantUMLWriter(pFSMOutputGenerator,pMACHINE_INFO);
void closePlantUMLWriter(pFSMOutputGenerator,int);

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
   int      e;
   pID_INFO action;
};

FSMPlantUMLOutputGenerator PlantUMLMachineWriter = {
	{
     initPlantUMLWriter,
     writePlantUMLWriter,
     closePlantUMLWriter
  },
  NULL
};

FSMPlantUMLOutputGenerator PlantUMLSubMachineWriter = {
	{
     initPlantUMLWriter,
     writePlantUMLWriter,
     closePlantUMLWriter
  },
  NULL
};

pFSMOutputGenerator pPlantUMLMachineWriter    = (pFSMOutputGenerator) &PlantUMLMachineWriter;
pFSMOutputGenerator pPlantUMLSubMachineWriter = (pFSMOutputGenerator) &PlantUMLSubMachineWriter;

static pPlantUMLMachineData newPlantUMLMachineData(char *);

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

   pID_INFO pevent = eventPidByIndex(pfsmpumlog->pmd->pmi, ple->e);

   fprintf(pfsmpumlog->pmd->pumlFile
           ,"%s: %s"
           , pevent->name
           , ple->action ? ple->action->name : ""
           );

   if (pevent->type_data.event_data.psharing_sub_machines)
   {
      fprintf(pfsmpumlog->pmd->pumlFile
              , "\nShared with:\n"
              );

      iterate_list(pevent->type_data.event_data.psharing_sub_machines
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
      for (char *cp = pid->docCmnt; *cp; *cp++)
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
      fputc('\n', pfsmpumlog->pmd->pumlFile);
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

/* Main section */
int initPlantUMLWriter (pFSMOutputGenerator pfsmog, char *baseFileName)
{

	time_t		now;
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

       }

     }
  }

	/* this may look funny, but it does the trick */
	return ((int) !pfsmpumlog->pmd->pumlFile);

}

void writePlantUMLWriter(pFSMOutputGenerator pfsmog, pMACHINE_INFO pmi)
{

	pID_INFO			  pevent;
	pACTION_INFO	  pai;
  pACTION_SE_INFO pasei;
	int						  e,s;
  ITERATOR_HELPER ih;

  pFSMPlantUMLOutputGenerator pfsmpumlog = (pFSMPlantUMLOutputGenerator) pfsmog;

	if (!pmi)

		return;

  pfsmpumlog->pmd->pmi = pmi;

  iterate_list(pmi->state_list, print_state_name, pfsmpumlog);
  iterate_list(pmi->state_list, print_state_docCmnt, pfsmpumlog);

  /* show the initial state */
  fprintf(pfsmpumlog->pmd->pumlFile
         , "\n[*] --> %s\n"
         , stateNameByIndex(pmi, 0)
         );

	for (s = 0; s < pmi->state_list->count; s++) {

     pLIST loopBackEvents = init_list();

			for (e = 0; e < pmi->event_list->count; e++) {

        if (
            pmi->actionArray[e][s]
            )
        {
           pai = pmi->actionArray[e][s];
           pevent = eventPidByIndex(pmi, e);

           if (pai->transition)
           {
              if (pai->transition->type == STATE)
              {
                 /* simple state transition */
                 fprintf(pfsmpumlog->pmd->pumlFile
                         , pevent->type_data.event_data.shared_with_parent
                           ? "%s --> %s : Event: (%s::) %s\\nAction: %s"
                           : "%s --> %s : Event: %s%s\\nAction: %s"
                         , stateNameByIndex(pmi, s)
                         , pmi->actionArray[e][s]->transition->name
                         , pevent->type_data.event_data.shared_with_parent
                           ? pmi->parent->name->name
                           : ""
                         , pevent->name
                         , pai->action
                             ? pai->action->name
                             : "noAction"
                         );

                 if (pevent->type_data.event_data.psharing_sub_machines)
                 {
                    fprintf(pfsmpumlog->pmd->pumlFile
                            , "\\nEvent shared with:\\n"
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
              }
              else
              {
                 /* first, declare the choice */
                 fprintf(pfsmpumlog->pmd->pumlFile
                         , "state %s <<choice>>\n"
                         , pmi->actionArray[e][s]->transition->name
                         );

                 /* then the state that transitions via the choice */
                 fprintf(pfsmpumlog->pmd->pumlFile
                         , "%s --> %s : Event: %s\\nAction: %s\\nChoice: %s\n"
                         , stateNameByIndex(pmi, s)
                         , pmi->actionArray[e][s]->transition->name
                         , eventNameByIndex(pmi, e)
                         , pmi->actionArray[e][s]->action
                             ? pmi->actionArray[e][s]->action->name
                             : ""
                         , pmi->actionArray[e][s]->transition->name
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
                            , "transition function: %s\n"
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

              plbe->e      = e;
              plbe->action = pai->action;

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
         pevent = eventPidByIndex(pmi, ((pLOOP_BACK_EVENT)loopBackEvents->head->mbr)->e);
         fprintf(pfsmpumlog->pmd->pumlFile
                 , pevent->type_data.event_data.shared_with_parent
                   ? "%s --> %s : Event: (%s::) %s\\nAction: %s\n"
                   : "%s --> %s : Event: %s%s\\nAction: %s\n"
                 , stateNameByIndex(pmi, s)
                 , stateNameByIndex(pmi, s)
                 , pevent->type_data.event_data.shared_with_parent
                   ? pmi->parent->name->name
                   : ""
                 , pevent->name
                 , ((pLOOP_BACK_EVENT)loopBackEvents->head->mbr)->action
                     ? ((pLOOP_BACK_EVENT)loopBackEvents->head->mbr)->action->name
                     : "noAction"
                 );
         break;
      default:
         fprintf(pfsmpumlog->pmd->pumlFile
                 , "\nnote left of %s\nThese events loop back:\n\n"
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
     write_machines(pmi->machine_list, pPlantUMLSubMachineWriter);
  }

}

void closePlantUMLWriter(pFSMOutputGenerator pfsmog, int good)
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

