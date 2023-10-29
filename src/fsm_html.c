/**
	fsm_html.c

		Creates HTML page and table to describe Finite State Machines


		FSMLang (fsm) - A Finite State Machine description language.
    Copyright (C) 2002  Steven Stanton

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

		Steven Stanton
		ringwinner@users.sourceforge.net

		For the latest on FSMLang: http://fsmlang.sourceforge.net

		And, finally, your possession of this source code implies nothing.

	Long Description:

	Creation: 	sstanton		Mar-13-2002
*/

#include "fsm_html.h"
#include "list.h"

#include <stdio.h>
#include <stdlib.h>

#include <time.h>
#if defined (LINUX) || defined (CYGWIN)
#include <string.h>
#include <unistd.h>
#endif

/*
	Our interface to the outside world
*/
int initHTMLWriter(pFSMOutputGenerator,char *);
void writeHTMLWriter(pFSMOutputGenerator,pMACHINE_INFO);
void closeHTMLWriter(pFSMOutputGenerator,int);

typedef struct _fsm_html_output_generator_ FSMHTMLOutputGenerator, *pFSMHTMLOutputGenerator;
typedef struct _html_machine_data_ HTMLMachineData, *pHTMLMachineData;

struct _html_machine_data_ {

	FILE	        *htmlFile;
	char	        *htmlName;
  char          *baseName;
  pMACHINE_INFO pmi;
};

struct _fsm_html_output_generator_
{
   FSMOutputGenerator fsmog;
   pHTMLMachineData   pmd;
};

FSMHTMLOutputGenerator HTMLMachineWriter = {
	{
     initHTMLWriter,
     writeHTMLWriter,
     closeHTMLWriter
  },
  NULL
};

bool  css_content_internal = false;
char *css_content_filename = "fsmlang.css";

/* list iteration callbacks */
static bool print_machine_name_as_list_element(pLIST_ELEMENT pelem, void *data)
{
   pMACHINE_INFO pmi  = (pMACHINE_INFO) pelem->mbr;
   FILE         *fout = (FILE*) data;

   fprintf(fout
           , "<li>%s</li>\n"
           , pmi->name->name
           );

   return false;
}

static bool print_id_info_as_html_list_element(pLIST_ELEMENT pelem, void *data)
{
   pFSMHTMLOutputGenerator pfsmhtmlog = (pFSMHTMLOutputGenerator) data;
   pID_INFO                pid        = (pID_INFO) pelem->mbr;

   fprintf(pfsmhtmlog->pmd->htmlFile
           , (pid->powningMachine && (pid->powningMachine != pfsmhtmlog->pmd->pmi))
               ? "\t\t<li>%s::%s</li>\n"
               : "\t\t<li>%s%s</li>\n"
           , (pid->powningMachine && (pid->powningMachine != pfsmhtmlog->pmd->pmi))
               ? pid->powningMachine->name->name
               : ""
           , pid->name
           );

   return false;
}

static bool print_action_table_row(pLIST_ELEMENT pelem, void *data)
{
   pID_INFO pid = ((pID_INFO)pelem->mbr);
   pFSMHTMLOutputGenerator pfsmhtmlog = (pFSMHTMLOutputGenerator) data;

   fprintf(pfsmhtmlog->pmd->htmlFile,"<tr>\n");
   fprintf(pfsmhtmlog->pmd->htmlFile,"<td class=\"label\">%s</td>\n"
     , strlen(pid->name) ? pid->name : "transition");
   fprintf(pfsmhtmlog->pmd->htmlFile,"<td>\n");
   fprintf(pfsmhtmlog->pmd->htmlFile,"%s"
           , pid->docCmnt ? pid->docCmnt : "&nbsp;"
           );

   if (pid->actionInfo)
   {
      /* if this action is associated with a shared event, it will have exactly one event */
      pID_INFO pevent = (pID_INFO)find_nth_list_member(pid->actionInfo->matrix->event_list,0);

      /* and, that event will have a list of sharing machines */
      if (pevent->type_data.event_data.psharing_sub_machines)
      {
         fprintf(pfsmhtmlog->pmd->htmlFile,"\n<br/><br/>Shares event to:<ul class=\"return_decl\">");
         iterate_list(pevent->type_data.event_data.psharing_sub_machines
                      , print_machine_name_as_list_element
                      , pfsmhtmlog->pmd->htmlFile
                      );
         fprintf(pfsmhtmlog->pmd->htmlFile,"</ul>\n"
                 );
      }
      else
      {
         if (pevent->type_data.event_data.shared_with_parent)
         {
            fprintf(pfsmhtmlog->pmd->htmlFile
                    ,"\n<p>Handles an event shared from %s</p>\n"
                    , pevent->powningMachine->parent->name->name
                    );
         }
         else
         {
            if (pid->action_returns_decl)
            {
               fprintf(pfsmhtmlog->pmd->htmlFile,"\n<br/><br/>Returns:<ul class=\"return_decl\">");
               iterate_list(pid->action_returns_decl,print_id_info_as_html_list_element,pfsmhtmlog);
               fprintf(pfsmhtmlog->pmd->htmlFile,"</ul>\n"
                       );
            }
         }
      }

      fprintf(pfsmhtmlog->pmd->htmlFile, "</td>\n</tr>\n");

   }

   return false;
}

static bool print_sub_machine_row(pLIST_ELEMENT pelem, void *data)
{
   pMACHINE_INFO           pmi        = ((pMACHINE_INFO)pelem->mbr);
   pFSMHTMLOutputGenerator pfsmhtmlog = (pFSMHTMLOutputGenerator) data;

   fprintf(pfsmhtmlog->pmd->htmlFile
           ,"<tr>\n"
           );
   fprintf(pfsmhtmlog->pmd->htmlFile
           ,"<td class=\"label\"><a href=\"%s.html\" target=\"FSMLang-%s\">%s</a></td>\n"
           , pmi->name->name
           , pmi->name->name
           , pmi->name->name
           );
   fprintf(pfsmhtmlog->pmd->htmlFile
           ,"<td>\n"
           );
   fprintf(pfsmhtmlog->pmd->htmlFile
           ,"%s"
           , pmi->name->docCmnt ? pmi->name->docCmnt : "&nbsp;"
           );

   fprintf(pfsmhtmlog->pmd->htmlFile, "</td>\n</tr>\n");

   return false;
}

/* Main section */
int initHTMLWriter (pFSMOutputGenerator pfsmog, char *baseFileName)
{

  pFSMHTMLOutputGenerator pfsmhtmlog = (pFSMHTMLOutputGenerator) pfsmog;

  if (NULL != (pfsmhtmlog->pmd = calloc(1, sizeof(HTMLMachineData))))
  {
     if (!baseFileName)
     {

       pfsmhtmlog->pmd->htmlFile = stdout;

     }
     else {

       pfsmhtmlog->pmd->htmlName = createFileName(baseFileName,".html");
	   if (output_generated_file_names_only)
	   {
		   fprintf(stdout
				   ,"%s "
				   , pfsmhtmlog->pmd->htmlName
				   );
		   return 0;
	   }
       pfsmhtmlog->pmd->baseName = strdup(baseFileName);

       if (!(pfsmhtmlog->pmd->htmlFile = openFile(pfsmhtmlog->pmd->htmlName,"w"))) {

         CHECK_AND_FREE(pfsmhtmlog->pmd->htmlName);

       }
       else {

         /* we're good to go; write the preamble */
         fprintf(pfsmhtmlog->pmd->htmlFile,"<!DOCTYPE html>\n<html>\n");

         fprintf(pfsmhtmlog->pmd->htmlFile,"<!--\n\t%s\n\n",pfsmhtmlog->pmd->htmlName);
         fprintf(pfsmhtmlog->pmd->htmlFile,"\tThis file automatically generated by FSMLang\n\n");
         fprintf(pfsmhtmlog->pmd->htmlFile, "-->\n");

         fprintf(pfsmhtmlog->pmd->htmlFile,"<head>\n");

         fprintf(pfsmhtmlog->pmd->htmlFile,"<title>FSM Lang : %s</title>\n",baseFileName);
         fprintf(pfsmhtmlog->pmd->htmlFile
                 ,"<script>window.name=\"FSMLang-%s\"</script>\n"
                 ,baseFileName
                 );

         if (css_content_internal)
         {
            fprintf(pfsmhtmlog->pmd->htmlFile, "<style>\n");
            if (copyFileContents(pfsmhtmlog->pmd->htmlFile, css_content_filename))
            {
                fprintf(stderr,"%s: Could not copy css file contents\n",me);
                return (1);
            }
            fprintf(pfsmhtmlog->pmd->htmlFile, "</style>\n");
         }
         else
         {
            fprintf(pfsmhtmlog->pmd->htmlFile
                    , "<link REL=stylesheet type=\"text/css\" href=\"%s\">\n"
                    , css_content_filename
                    );
         }

         fprintf(pfsmhtmlog->pmd->htmlFile,"</head><body>\n");

       }

     }
  }

	/* this may look funny, but it does the trick */
	return ((int) !pfsmhtmlog->pmd->htmlFile);

}

void writeHTMLWriter(pFSMOutputGenerator pfsmog, pMACHINE_INFO pmi)
{

	pID_INFO			  pid;
	unsigned				e,s;

	if (output_generated_file_names_only)
	{
		return;
	}

	pFSMHTMLOutputGenerator pfsmhtmlog = (pFSMHTMLOutputGenerator)pfsmog;

	if (!pmi)

		return;

  pfsmhtmlog->pmd->pmi = pmi;

	fprintf(pfsmhtmlog->pmd->htmlFile,"<h2>%s</h2>\n"
		, pmi->name->name
		);

	if (pmi->name->docCmnt)
		fprintf(pfsmhtmlog->pmd->htmlFile,"<p>%s</p>\n",pmi->name->docCmnt);

  if (include_svg_img)
  {
     fprintf(pfsmhtmlog->pmd->htmlFile
             , "<img src=\"%s.svg\" alt=\"PlantUML diagram separately generated.\"/>\n"
             , pfsmhtmlog->pmd->baseName
             );
  }

	fprintf(pfsmhtmlog->pmd->htmlFile,"<div class=\"scrollable\">\n<table class=\"machine\">\n");

	/* first row */
	fprintf(pfsmhtmlog->pmd->htmlFile,"\t<thead>\n\t\t<tr>\n");
	fprintf(pfsmhtmlog->pmd->htmlFile,"\t\t\t<th class=\"blankCorner\">&nbsp;</th>\n");

	/* event names */
	for (e = 0;e < pmi->event_list->count;e++)
  {
    pid = eventPidByIndex(pmi,e);
		fprintf(pfsmhtmlog->pmd->htmlFile
            , pid->type_data.event_data.shared_with_parent
               ? "\t\t\t<th class=eventName>(%s::) %s</th>\n"
               : "\t\t\t<th class=eventName>%s%s</th>\n"
            , pid->type_data.event_data.shared_with_parent
               ? pmi->parent->name->name
               : ""
            , pid->name
			      );
  }
	fprintf(pfsmhtmlog->pmd->htmlFile,"\t\t</tr>\n\t</thead>\n");

	/* State Label column */
	fprintf(pfsmhtmlog->pmd->htmlFile,"\t<tbody>\n\t\t<tr>\n");

	/* now, it gets a bit tricky with the row breaks */
	for (s = 0; s < pmi->state_list->count; s++) {

		if (s)

			fprintf(pfsmhtmlog->pmd->htmlFile,"\t<tr>\n");

		fprintf(pfsmhtmlog->pmd->htmlFile,"\t\t<th class=stateName>%s</th>\n"
			,	stateNameByIndex(pmi,s)
			);

			for (e = 0; e < pmi->event_list->count; e++) {

         fprintf(pfsmhtmlog->pmd->htmlFile
                 ,"\t\t<td class="
                 );
				if (pmi->modFlags & mfActionsReturnStates) {

					fprintf(pfsmhtmlog->pmd->htmlFile
                  ,"%s>%s"
                  ,	pmi->actionArray[e][s] ?
								     (strlen(pmi->actionArray[e][s]->action->name) 
                        ? "action" : "noAction") 
                        : "nullAction"
                  ,	pmi->actionArray[e][s] 
                        ? (strlen(pmi->actionArray[e][s]->action->name) 
                           ? pmi->actionArray[e][s]->action->name 
                           : "transition") 
                        : "Not Defined"
                  );

          if (
              pmi->actionArray[e][s]
             && pmi->actionArray[e][s]->action->action_returns_decl
              )
          {
             fprintf(pfsmhtmlog->pmd->htmlFile
                     ,"<br/>returns:\n\t<ul class=\"return_decl\">\n"
                     );
             iterate_list(pmi->actionArray[e][s]->action->action_returns_decl,print_id_info_as_html_list_element,pfsmhtmlog);
             fprintf(pfsmhtmlog->pmd->htmlFile
                     , "\t</ul>\n"
                     );
          }

          if (
              pmi->actionArray[e][s]
             && pmi->actionArray[e][s]->transition
              )
          {
             fprintf(pfsmhtmlog->pmd->htmlFile
                     , "<br/>%s"
                     , pmi->actionArray[e][s]->transition->name
                     );

             if (pmi->actionArray[e][s]->transition->transition_fn_returns_decl)
             {
                fprintf(pfsmhtmlog->pmd->htmlFile
                        ,"<br/>returns:\n\t<ul class=\"return_decl\">\n"
                        );
                iterate_list(pmi->actionArray[e][s]->transition->transition_fn_returns_decl, print_id_info_as_html_list_element, pfsmhtmlog);
                fprintf(pfsmhtmlog->pmd->htmlFile
                        , "\t</ul>\n"
                        );
             }
          }

				}
				else {

					fprintf(pfsmhtmlog->pmd->htmlFile
                  ,"%s>%s"
                  ,	pmi->actionArray[e][s] 
                     ? (strlen(pmi->actionArray[e][s]->action->name) 
                        ? "action" 
                        : "noAction") 
                     : "nullAction"
                  ,	pmi->actionArray[e][s] 
                     ? (strlen(pmi->actionArray[e][s]->action->name) 
                        ? pmi->actionArray[e][s]->action->name 
                        : "noAction") 
                     : "&nbsp;"
                  );

          if (
              pmi->actionArray[e][s]
              )
          {
             /* if this action is associated with a shared event, it will have exactly one event */
             pID_INFO paction = pmi->actionArray[e][s]->action;
             pID_INFO pevent = (pID_INFO)find_nth_list_member(paction->actionInfo->matrix->event_list,0);

             /* and, that event will have a list of sharing machines */
             if (pevent->type_data.event_data.psharing_sub_machines)
             {
                fprintf(pfsmhtmlog->pmd->htmlFile
                        ,"<br/>shares event with:\n\t<ul class=\"return_decl\">\n"
                        );
                iterate_list(pevent->type_data.event_data.psharing_sub_machines
                             , print_machine_name_as_list_element
                             , pfsmhtmlog->pmd->htmlFile
                             );
                fprintf(pfsmhtmlog->pmd->htmlFile,"</ul>\n"
                        );
             }
             else
             {
                if (paction->action_returns_decl)
                {
                   fprintf(pfsmhtmlog->pmd->htmlFile
                           ,"<br/>returns:\n\t<ul class=\"return_decl\">\n"
                           );
                   iterate_list(pmi->actionArray[e][s]->action->action_returns_decl, print_id_info_as_html_list_element, pfsmhtmlog);
                   fprintf(pfsmhtmlog->pmd->htmlFile
                           , "\t</ul>\n"
                           );
                }
             }
          }
          
          fprintf(pfsmhtmlog->pmd->htmlFile
                  , "<br/><b>transition</b> : %s"
                  , pmi->actionArray[e][s] ? 
                    (pmi->actionArray[e][s]->transition ? 
                      pmi->actionArray[e][s]->transition->name : "none")
                    : "none"
                  );

          if (
              pmi->actionArray[e][s]
              && pmi->actionArray[e][s]->transition 
             && pmi->actionArray[e][s]->transition->transition_fn_returns_decl
              )
          {
             fprintf(pfsmhtmlog->pmd->htmlFile
                     ,"<br/>returns:\n\t<ul class=\"return_decl\">\n"
                     );
             iterate_list(pmi->actionArray[e][s]->transition->transition_fn_returns_decl, print_id_info_as_html_list_element, pfsmhtmlog);
             fprintf(pfsmhtmlog->pmd->htmlFile
                     , "\t</ul>\n"
                     );
          }

				}

        if (
            pmi->actionArray[e][s]
            && pmi->actionArray[e][s]->docCmnt
            )
        {
           fprintf(pfsmhtmlog->pmd->htmlFile
                   ,"<p class=\"transition_comment\">%s</p>"
                   , pmi->actionArray[e][s]->docCmnt
                   );
        }

        fprintf(pfsmhtmlog->pmd->htmlFile
                , "</td>\n"
                );

			}

			fprintf(pfsmhtmlog->pmd->htmlFile,"\t</tr>\n");

	}
	
  fprintf(pfsmhtmlog->pmd->htmlFile, "\t</tbody>\n</table>\n</div>\n");

	/*
    Now, list the events, states, actions, and any transition functions
    with their Document Comments
	*/
	fprintf(pfsmhtmlog->pmd->htmlFile,"<table class=\"elements\">\n");

	fprintf(pfsmhtmlog->pmd->htmlFile,"<tr>\n");
	fprintf(pfsmhtmlog->pmd->htmlFile,"<th colspan=2 align=left>Events</th>\n");
	fprintf(pfsmhtmlog->pmd->htmlFile,"</tr>\n");
	
	for (e = 0; e < pmi->event_list->count; e++) {

		pid = eventPidByIndex(pmi,e);
	
		fprintf(pfsmhtmlog->pmd->htmlFile,"<tr>\n");
		fprintf(pfsmhtmlog->pmd->htmlFile
            , pid->type_data.event_data.shared_with_parent
              ? "<td class=\"label\">(%s::) %s</td>\n"
              : "<td class=\"label\">%s%s</td>\n"
            , pid->type_data.event_data.shared_with_parent
              ? pmi->parent->name->name
              : ""
            , pid->name
            );
		fprintf(pfsmhtmlog->pmd->htmlFile,"<td>%s\n"
			, pid->docCmnt ? pid->docCmnt : "&nbsp;");
    if (pid->type_data.event_data.psharing_sub_machines)
    {
       fprintf(pfsmhtmlog->pmd->htmlFile
               ,"<p>This event is shared with:</p><ul class=\"return_decl\">\n"
               );
       iterate_list(pid->type_data.event_data.psharing_sub_machines
                    , print_machine_name_as_list_element
                    , pfsmhtmlog->pmd->htmlFile
                    );
       fprintf(pfsmhtmlog->pmd->htmlFile,"</ul>\n"
               );
    }

    if (pid->type_data.event_data.puser_event_data)
    {
       if (pid->type_data.event_data.puser_event_data->translator)
       {
          fprintf(pfsmhtmlog->pmd->htmlFile, "<p>\n");
          fprintf(pfsmhtmlog->pmd->htmlFile
                  , "Data translator: %s\n"
                  , pid->type_data.event_data.puser_event_data->translator->name
                  );
          fprintf(pfsmhtmlog->pmd->htmlFile, "</p>\n");
       }

       if (pid->type_data.event_data.puser_event_data->data_fields)
       {
          ITERATOR_HELPER ih = {
             .fout = pfsmhtmlog->pmd->htmlFile
             , .tab_level = 0
          };

          fprintf(pfsmhtmlog->pmd->htmlFile, "<p>\n");
          fprintf(pfsmhtmlog->pmd->htmlFile
                  , "Event data:"
                  );
          fprintf(pfsmhtmlog->pmd->htmlFile, "</p>\n");

          fprintf(pfsmhtmlog->pmd->htmlFile, "<code>\n");
          iterate_list(pid->type_data.event_data.puser_event_data->data_fields
                       , print_data_field
                       , &ih
                       );
          fprintf(pfsmhtmlog->pmd->htmlFile, "</code>\n");
       }
    }

		fprintf(pfsmhtmlog->pmd->htmlFile,"</td>\n");
		fprintf(pfsmhtmlog->pmd->htmlFile,"</tr>\n");

	}

  fprintf(pfsmhtmlog->pmd->htmlFile,"</table>\n");
	fprintf(pfsmhtmlog->pmd->htmlFile,"<table class=\"elements\">\n");

	fprintf(pfsmhtmlog->pmd->htmlFile,"<tr>\n");
	fprintf(pfsmhtmlog->pmd->htmlFile,"<th colspan=2 align=left>States</th>\n");
	fprintf(pfsmhtmlog->pmd->htmlFile,"</tr>\n");
	
	for (s = 0; s < pmi->state_list->count; s++) {

     bool something_printed = false;
	
		pid = statePidByIndex(pmi,s);

		fprintf(pfsmhtmlog->pmd->htmlFile,"<tr>\n");
		fprintf(pfsmhtmlog->pmd->htmlFile,"<td class=\"label\">%s</td>\n<td>\n"
			, pid->name);

    if (pid->docCmnt)
    {
       fprintf(pfsmhtmlog->pmd->htmlFile
               , "<p>%s</p>\n"
               , pid->docCmnt
               );
       something_printed = true;
    }

    if (pid->type_data.state_data.state_flags & sfHasEntryFn)
    {
       fprintf(pfsmhtmlog->pmd->htmlFile
               , "<p>On Entry: %s%s</p>\n"
               , pid->type_data.state_data.entry_fn
                 ? pid->type_data.state_data.entry_fn->name
                 : "onEntryTo_"
               , pid->type_data.state_data.entry_fn
                 ? ""
                 : pid->name
               );
       something_printed = true;
    }

    if (pid->type_data.state_data.state_flags & sfHasExitFn)
    {
       fprintf(pfsmhtmlog->pmd->htmlFile
               , "<p>On Exit: %s%s</p>\n"
               , pid->type_data.state_data.exit_fn
                 ? pid->type_data.state_data.exit_fn->name
                 : "onExitTo_"
               , pid->type_data.state_data.exit_fn
                 ? ""
                 : pid->name
               );
       something_printed = true;
    }

    if (pid->type_data.state_data.state_flags & sfInibitSubMachines)
    {
       fprintf(pfsmhtmlog->pmd->htmlFile
               , "<p>Inhibits Sub-machines</p>\n"
               );
       something_printed = true;
    }

    if (!something_printed)
    {
       fprintf(pfsmhtmlog->pmd->htmlFile,"&nbsp;");
    }

    fprintf(pfsmhtmlog->pmd->htmlFile, "</td></tr>\n");

	}

  fprintf(pfsmhtmlog->pmd->htmlFile,"</table>\n");
	fprintf(pfsmhtmlog->pmd->htmlFile,"<table class=\"elements\">\n");

	fprintf(pfsmhtmlog->pmd->htmlFile,"<tr>\n");
	fprintf(pfsmhtmlog->pmd->htmlFile,"<th colspan=2 align=left>Actions</th>\n");
	fprintf(pfsmhtmlog->pmd->htmlFile,"</tr>\n");
	
  iterate_list(pmi->action_list,print_action_table_row,pfsmhtmlog);

  fprintf(pfsmhtmlog->pmd->htmlFile, "</table>\n");

  if (pmi->transition_fn_list->count)
  {
     fprintf(pfsmhtmlog->pmd->htmlFile,"<table class=\"elements\">\n");
     fprintf(pfsmhtmlog->pmd->htmlFile, "<tr>\n");
     fprintf(pfsmhtmlog->pmd->htmlFile,"<th colspan=2 align=left>Transition Functions</th>\n");
     fprintf(pfsmhtmlog->pmd->htmlFile,"</tr>\n");

     iterate_list(pmi->transition_fn_list,print_action_table_row,pfsmhtmlog);

     fprintf(pfsmhtmlog->pmd->htmlFile, "</table>\n");
     
  }


  if (pmi->machine_list)
  {
     fprintf(pfsmhtmlog->pmd->htmlFile,"<table class=\"elements\">\n");
     fprintf(pfsmhtmlog->pmd->htmlFile, "<tr>\n");
     fprintf(pfsmhtmlog->pmd->htmlFile,"<th colspan=2 align=left>Sub Machines</th>\n");
     fprintf(pfsmhtmlog->pmd->htmlFile,"</tr>\n");

     iterate_list(pmi->machine_list, print_sub_machine_row, pfsmhtmlog);

     fprintf(pfsmhtmlog->pmd->htmlFile, "</table>\n");

     write_machines(pmi->machine_list, generateHTMLMachineWriter);
  }
}

void closeHTMLWriter(pFSMOutputGenerator pfsmog, int good)
{
   pFSMHTMLOutputGenerator pfsmhtmlog = (pFSMHTMLOutputGenerator) pfsmog;

   if (!output_generated_file_names_only)
   {
	   if (good)
	   {

		   fprintf(pfsmhtmlog->pmd->htmlFile, "</body>\n</html>\n");

	   }

	   fclose(pfsmhtmlog->pmd->htmlFile);

	   if (!good)
	   {

		   unlink(pfsmhtmlog->pmd->htmlName);

	   }

	   CHECK_AND_FREE(pfsmhtmlog->pmd->htmlName);
   }

}

pFSMOutputGenerator generateHTMLMachineWriter(FSMOGF_TYPE fsmogft)
{
	pFSMOutputGenerator pfsmog;

	if (fsmogft == fsmogft_sub_machine)
	{
		pFSMHTMLOutputGenerator pfsmhtmlog = calloc(1, sizeof(FSMHTMLOutputGenerator));

		pfsmhtmlog->fsmog.writeMachine = writeHTMLWriter;
		pfsmhtmlog->fsmog.initOutput   = initHTMLWriter;
		pfsmhtmlog->fsmog.closeOutput  = closeHTMLWriter;

		pfsmog = (pFSMOutputGenerator)pfsmhtmlog;
	}
	else
	{
		pfsmog = (pFSMOutputGenerator)&HTMLMachineWriter;
	}
}

