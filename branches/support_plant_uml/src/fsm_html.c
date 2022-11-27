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

FSMHTMLOutputGenerator HTMLSubMachineWriter = {
	{
     initHTMLWriter,
     writeHTMLWriter,
     closeHTMLWriter
  },
  NULL
};

pFSMOutputGenerator pHTMLMachineWriter    = (pFSMOutputGenerator) &HTMLMachineWriter;
pFSMOutputGenerator pHTMLSubMachineWriter = (pFSMOutputGenerator) &HTMLSubMachineWriter;

static pHTMLMachineData newHTMLMachineData(char *);

/* list iteration callbacks */
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
   if (pid->action_returns_decl)
   {
      fprintf(pfsmhtmlog->pmd->htmlFile,"\n<br/><br/>Returns:<ul class=\"return_decl\">");
      iterate_list(pid->action_returns_decl,print_id_info_as_html_list_element,pfsmhtmlog);
      fprintf(pfsmhtmlog->pmd->htmlFile,"</ul>\n"
              );
   }
   fprintf(pfsmhtmlog->pmd->htmlFile, "</td>\n</tr>\n");

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
           ,"<td class=\"label\"><a href=\"%s.html\" target=\"_blank\">%s</a></td>\n"
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

	time_t		now;
  pFSMHTMLOutputGenerator pfsmhtmlog = (pFSMHTMLOutputGenerator) pfsmog;

  if (NULL != (pfsmhtmlog->pmd = calloc(1, sizeof(HTMLMachineData))))
  {
     if (!baseFileName)
     {

       pfsmhtmlog->pmd->htmlFile = stdout;

     }
     else {

       pfsmhtmlog->pmd->htmlName = createFileName(baseFileName,".html");
       pfsmhtmlog->pmd->baseName = strdup(baseFileName);

       if (!(pfsmhtmlog->pmd->htmlFile = openFile(pfsmhtmlog->pmd->htmlName,"w"))) {

         CHECK_AND_FREE(pfsmhtmlog->pmd->htmlName);

       }
       else {

         /* we're good to go; write the preamble */

         time(&now);

         fprintf(pfsmhtmlog->pmd->htmlFile,"<!DOCTYPE html>\n<html>\n");

         fprintf(pfsmhtmlog->pmd->htmlFile,"<!--\n\t%s\n\n",pfsmhtmlog->pmd->htmlName);
         fprintf(pfsmhtmlog->pmd->htmlFile,"\tThis file automatically generated by FSMLang\n\n");
         fprintf(pfsmhtmlog->pmd->htmlFile,"\tOn %s\n\n-->\n",ctime(&now));

         fprintf(pfsmhtmlog->pmd->htmlFile,"<head>\n");

         fprintf(pfsmhtmlog->pmd->htmlFile,"<title>FSM Lang : %s</title>\n",baseFileName);

         fprintf(pfsmhtmlog->pmd->htmlFile,"<link REL=stylesheet type=\"text/css\" href=\"fsmlang.css\">\n");

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
	pACTION_INFO	  pai;
  pACTION_SE_INFO pasei;
	int						  e,s;
  ITERATOR_HELPER ih;

  pFSMHTMLOutputGenerator pfsmhtmlog = (pFSMHTMLOutputGenerator) pfsmog;

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

	fprintf(pfsmhtmlog->pmd->htmlFile,"<table class=machine>\n");

	/* first row */
	fprintf(pfsmhtmlog->pmd->htmlFile,"\t<tr>\n");
	fprintf(pfsmhtmlog->pmd->htmlFile,"\t\t<th class=blankCorner rowspan=2 colspan=2>&nbsp;</th>\n");
	fprintf(pfsmhtmlog->pmd->htmlFile,"\t\t<th class=eventLabel colspan=%d>%s</th>\n"
		,pmi->event_list->count	
		,"Events"
		);
	fprintf(pfsmhtmlog->pmd->htmlFile,"\t</tr>\n");

	/* event names row */
	fprintf(pfsmhtmlog->pmd->htmlFile,"\t<tr>\n");
	for (e = 0;e < pmi->event_list->count;e++)
		fprintf(pfsmhtmlog->pmd->htmlFile,"\t\t<th class=eventName>%s</th>\n"
			, eventNameByIndex(pmi,e)
			);
	fprintf(pfsmhtmlog->pmd->htmlFile,"\t</tr>\n");

	/* State Label column */
	fprintf(pfsmhtmlog->pmd->htmlFile,"\t<tr>\n");
	fprintf(pfsmhtmlog->pmd->htmlFile,"\t\t<th class=stateLabel rowspan=%d>%s</th>\n"
		, pmi->state_list->count
		, "S<br/>t<br/>a<br/>t<br/>e<br/>s"
		);

	/* now, it gets a bit tricky with the row breaks */
	for (s = 0; s < pmi->state_list->count; s++) {

		if (s)

			fprintf(pfsmhtmlog->pmd->htmlFile,"\t<tr>\n");

		fprintf(pfsmhtmlog->pmd->htmlFile,"\t\t<th class=stateName>%s</th>\n"
			,	stateNameByIndex(pmi,s)
			);

			for (e = 0; e < pmi->event_list->count; e++) {

				if (pmi->modFlags & mfActionsReturnStates) {

					fprintf(pfsmhtmlog->pmd->htmlFile
                  ,"\t\t<td class=%s>%s"
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
             fprintf(pfsmhtmlog->pmd->htmlFile
                     , "</td>\n"
                     );
             }

          fprintf(pfsmhtmlog->pmd->htmlFile
                  , "</td>\n"
                  );

				}
				else {

					fprintf(pfsmhtmlog->pmd->htmlFile
                  ,"\t\t<td class=%s>%s"
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
             && pmi->actionArray[e][s]->action->action_returns_decl
              )
          {
             fprintf(pfsmhtmlog->pmd->htmlFile
                     ,"<br/>returns:\n\t<ul class=\"return_decl\">\n"
                     );
             iterate_list(pmi->actionArray[e][s]->action->action_returns_decl, print_id_info_as_html_list_element, pfsmhtmlog);
             fprintf(pfsmhtmlog->pmd->htmlFile
                     , "\t</ul>\n"
                     );
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
          fprintf(pfsmhtmlog->pmd->htmlFile
                  , "</td>\n"
                  );

				}

			}

			fprintf(pfsmhtmlog->pmd->htmlFile,"\t</tr>\n");

	}
	
	fprintf(pfsmhtmlog->pmd->htmlFile,"</table>\n<p>\n");

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
		fprintf(pfsmhtmlog->pmd->htmlFile,"<td class=\"label\">%s</td>\n"
			, pid->name);
		fprintf(pfsmhtmlog->pmd->htmlFile,"<td>%s</td>\n"
			, pid->docCmnt ? pid->docCmnt : "&nbsp;");
		fprintf(pfsmhtmlog->pmd->htmlFile,"</tr>\n");

	}

  fprintf(pfsmhtmlog->pmd->htmlFile,"</table>\n");
	fprintf(pfsmhtmlog->pmd->htmlFile,"<table class=\"elements\">\n");

	fprintf(pfsmhtmlog->pmd->htmlFile,"<tr>\n");
	fprintf(pfsmhtmlog->pmd->htmlFile,"<th colspan=2 align=left>States</th>\n");
	fprintf(pfsmhtmlog->pmd->htmlFile,"</tr>\n");
	
	for (s = 0; s < pmi->state_list->count; s++) {
	
		pid = statePidByIndex(pmi,s);

		fprintf(pfsmhtmlog->pmd->htmlFile,"<tr>\n");
		fprintf(pfsmhtmlog->pmd->htmlFile,"<td class=\"label\">%s</td>\n"
			, pid->name);
		fprintf(pfsmhtmlog->pmd->htmlFile,"<td>%s</td>\n"
			, pid->docCmnt ? pid->docCmnt : "&nbsp;");
		fprintf(pfsmhtmlog->pmd->htmlFile,"</tr>\n");

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

     write_machines(pmi->machine_list, pHTMLSubMachineWriter);
  }
}

void closeHTMLWriter(pFSMOutputGenerator pfsmog, int good)
{
   pFSMHTMLOutputGenerator pfsmhtmlog = (pFSMHTMLOutputGenerator) pfsmog;

	if (good) {

		fprintf(pfsmhtmlog->pmd->htmlFile,"</body>\n</html>\n");

	}

	fclose(pfsmhtmlog->pmd->htmlFile);

	if (!good) {

		unlink(pfsmhtmlog->pmd->htmlName);

	}

	CHECK_AND_FREE(pfsmhtmlog->pmd->htmlName);

}

