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

static int initHTMLWriter(char *);
static void writeHTMLWriter(pMACHINE_INFO);
static void closeHTMLWriter(int);

FSMOutputGenerator HTMLMachineWriter = {
	initHTMLWriter,
	writeHTMLWriter,
	closeHTMLWriter
};

pFSMOutputGenerator pHTMLMachineWriter = &HTMLMachineWriter;


/*
	Our internal data
*/
typedef struct _html_machine_data_ HTMLMachineData, *pHTMLMachineData;

struct _html_machine_data_ {

	FILE	*htmlFile
				;

	char	*htmlName
				;

  char *origBaseName;

};

HTMLMachineData htmlMachineData = {
		NULL
	,	NULL
  , NULL
};

char buf[256];

static void _html_page_preamble(char*);
static void _print_action_data_block(pHTMLMachineData, pMACHINE_INFO, int, int);

static int initHTMLWriter (char *baseFileName)
{

	if (!baseFileName) {

		htmlMachineData.htmlFile = stdout;

	}
	else {

     if (!htmlMachineData.origBaseName)
     {

		htmlMachineData.origBaseName = baseFileName;
     }

     htmlMachineData.htmlName = createFileName(baseFileName,".html");

		if (!(htmlMachineData.htmlFile = openFile(htmlMachineData.htmlName,"w"))) {

			CHECK_AND_FREE(htmlMachineData.htmlName);

		}
		else {

			/* we're good to go; write the preamble */
       _html_page_preamble(baseFileName);
		}

	}

	/* this may look funny, but it does the trick */
	return ((int) !htmlMachineData.htmlFile);

}

static void writeHTMLWriter(pMACHINE_INFO pmi)
{

	pID_INFO			  pid;
	pACTION_INFO	  pai;
  pACTION_SE_INFO pasei;
	int						  e,s;

	if (!pmi)

		return;

			fprintf(htmlMachineData.htmlFile,"<h2>%s</h2>\n"
		, pmi->name->name
		);

	if (pmi->name->docCmnt)
		fprintf(htmlMachineData.htmlFile,"<p>%s<p>\n",pmi->name->docCmnt);
			fprintf(htmlMachineData.htmlFile,"<div class=\"scrollable\">\n<table class=\"machine\">\n");

	/* event names row */
	fprintf(htmlMachineData.htmlFile,"\t<thead>\n\t\t<tr>\n\t\t\t<th class=\"blankCorner\">&nbsp;</th>\n");
	for (e = 0;e < pmi->event_count;e++)
  {
     pid = eventPidByIndex(pmi, e);
     if (has_actions_or_transitions(pid))
     {
        fprintf(htmlMachineData.htmlFile,"\t\t\t<th class=\"eventName\"><a href=\"%s_%s_%s.html\">%s</a></th>\n", htmlMachineData.origBaseName
                , pmi->name->name
                , get_event_name(pid, pmi)
                , get_event_name(pid, pmi)
               );
     }
  }
	fprintf(htmlMachineData.htmlFile,"\t\t</tr>\n\t</thead>\n");

	/* State Label column */
	fprintf(htmlMachineData.htmlFile,"\t<tbody>\n");

	/* now, it gets a bit tricky with the row breaks */
	for (s = 0; s < pmi->state_count; s++) {

    fprintf(htmlMachineData.htmlFile,"\t\t<tr>\n");

		fprintf(htmlMachineData.htmlFile
            ,"\t\t\t<th class=\"stateName\"><a href=\"%s_%s_%s.html\">%s</a></th>\n"
            , htmlMachineData.origBaseName
            , pmi->name->name
            ,	stateNameByIndex(pmi,s)
            ,	stateNameByIndex(pmi,s)
            );

			for (e = 0; e < pmi->event_count; e++) {
         if (has_actions_or_transitions(eventPidByIndex(pmi,e)))
         {
            _print_action_data_block(&htmlMachineData, pmi, e, s);
         }
			}

			fprintf(htmlMachineData.htmlFile,"\t\t</tr>\n");

	}
	
	fprintf(htmlMachineData.htmlFile,"\t</tbody>\n</table>\n</div>\n");

  closeHTMLWriter(1);

	/*
    Now, list the events, states, actions, and any transition functions
    with their Document Comments
	*/
	for (e = 0; e < pmi->event_count; e++) {

		pid = eventPidByIndex(pmi,e);

	if (htmlMachineData.origBaseName)
    {
       snprintf(buf
                , sizeof(buf)
                , "%s_%s_%s"
                , htmlMachineData.origBaseName
                , pmi->name->name
                , get_event_name(pid, pmi)
                );
       initHTMLWriter(buf);
    }
    else
    {
       initHTMLWriter(NULL);
    }
	
    fprintf(htmlMachineData.htmlFile,"<h2 class=\"eventName\">%s</h2>\n", get_event_name(pid, pmi)
            );


    if (pid->docCmnt)
    {
       fprintf(htmlMachineData.htmlFile,"<p class=\"docCmnt\">%s</p>\n"
               , pid->docCmnt
               );
    }

    if (assignExternalEventValues(pmi))
    {
       fprintf(htmlMachineData.htmlFile,"<p class=\"internalDesignation\">Internal name: "
               );

       if (pid->complexInfo)
       {
          print_complex_event_ancestry(htmlMachineData.htmlFile, pid, false);
       }
       else
       {
          fprintf(htmlMachineData.htmlFile,"%s", pid->name);
       }

       fprintf(htmlMachineData.htmlFile,"</p>\n"
               );
    }

    if ((pmi->complex_event_count) && (pid->complexInfo))
    {
       fprintf(htmlMachineData.htmlFile
               , "<p class=\"externalComplexName\">External ancestry: "
               );

       print_complex_event_ancestry(htmlMachineData.htmlFile, pid, true);
	fprintf(htmlMachineData.htmlFile,"</p>\n");

    }

    if ((pid->externalDesignation && pid->externalDesignation->name_prefix) && !pid->complexInfo)
    {

       fprintf(htmlMachineData.htmlFile,"<p class=\"fullExternalName\">Full external name: %s%s</p>"
               , pid->externalDesignation->name_prefix
               , pid->externalDesignation->name
               );

    }

    fprintf(htmlMachineData.htmlFile
            , "<p class=\"occuringStates\">Occurs in these states, with these actions and transitions:\n<table class=\"elements\">\n"
            );

    bool table_header_needed = true;
    for (s = 0; s < pmi->state_count; s++)
    {
       if (pmi->actionArray[pid->seOrder][s])
       {
          if (table_header_needed)
          {
             fprintf(htmlMachineData.htmlFile,"<tr><th>state</th><th>Action</th></tr>"
                     );

             table_header_needed = false;
          }

          fprintf(htmlMachineData.htmlFile,"<tr>\n");

          fprintf(htmlMachineData.htmlFile,"<th>%s</th>"
                  , stateNameByIndex(pmi,s)
                  , strlen(pmi->actionArray[pid->seOrder][s]->action->name)
                     ? pmi->actionArray[pid->seOrder][s]->action->name 
                     : "none"
                  );

          _print_action_data_block(&htmlMachineData, pmi, pid->seOrder, s);
	fprintf(htmlMachineData.htmlFile,"</tr>\n"
                  );

       }
    }

    fprintf(htmlMachineData.htmlFile, "</table>\n"
		);

    closeHTMLWriter(1);

	}


	for (s = 0; s < pmi->state_count; s++) {
	
		pid = statePidByIndex(pmi,s);

    if (htmlMachineData.origBaseName)
    {
       snprintf(buf
                , sizeof(buf)
                , "%s_%s_%s"
                , htmlMachineData.origBaseName
                , pmi->name->name
                , pid->name
                );
       initHTMLWriter(buf);
    }
    else
    {
       initHTMLWriter(NULL);
    }
	
		fprintf(htmlMachineData.htmlFile,"<h2 class=\"stateName\">%s</h2>\n"
			, pid->name);

    if (pid->docCmnt)
    {
       fprintf(htmlMachineData.htmlFile,"<p class=\"docCmnt\">%s</p>\n"
               , pid->docCmnt
               );
    }

    fprintf(htmlMachineData.htmlFile
            , "<p class=\"occuringEvents\">These events occur with these actions or transitions:\n<table class=\"elements\">\n"
            );

    bool table_header_needed = true;

			for (e = 0; e < pmi->event_count; e++) {

				if (pmi->actionArray[e][pid->seOrder])
       {
          if (table_header_needed) {

					fprintf(htmlMachineData.htmlFile
                  ,"<tr><th>event</th><th>action</th></tr>\n"
                     );
             table_header_needed = false;
          }
          pID_INFO pid_e = eventPidByIndex(pmi, e);
          fprintf(htmlMachineData.htmlFile
                  , "<tr>"
                  );
          fprintf(htmlMachineData.htmlFile
                  , "<th>%s</th>"
                  , get_event_name(pid_e, pmi)
                  );

          _print_action_data_block(&htmlMachineData, pmi, e, pid->seOrder);
          fprintf(htmlMachineData.htmlFile
                  , "</tr>");
       }
    }

    fprintf(htmlMachineData.htmlFile, "</table>\n");

    closeHTMLWriter(1);

	}

  for (pid = pmi->action_list; pid; pid = pid->nextAction) {
	
     if (htmlMachineData.origBaseName)
     {
        snprintf(buf
                 , sizeof(buf)
                 , "%s_%s_%s"
                 , htmlMachineData.origBaseName
                 , pmi->name->name
                 , pid->name);
        initHTMLWriter(buf);
     }
     else
     {
        initHTMLWriter(NULL);
     }

		fprintf(htmlMachineData.htmlFile,"<h2 class=\"actionName\">%s</h2>\n"
			, strlen(pid->name) ? pid->name 
                           : "transition");
		fprintf(htmlMachineData.htmlFile
            ,"<p class=\"docCmnt\">%s</p>\n"
            , pid->docCmnt ? pid->docCmnt : ""
                  );

          if (pid->action_returns_decl
              )
          {
             fprintf(htmlMachineData.htmlFile
                     ,"\n<br/><br/>Returns:<ul class=\"return_decl\">"
                     );
             for (pasei = pid->action_returns_decl;
                  pasei;
                  pasei = pasei->next
                  )
             {
                fprintf(htmlMachineData.htmlFile
                        , "<li>%s</li>\n"
                        , pasei->se->name
                        );
             }
             fprintf(htmlMachineData.htmlFile
                     , "</ul>\n"
                     );
          }

    closeHTMLWriter(1);

	}

  if (
              pmi->transition_fn_count)
  {
     for (pid = pmi->transition_fn_list; pid; pid = pid->nextTransitionFn)
          {
        if (htmlMachineData.origBaseName)
        {
           snprintf(buf
                    , sizeof(buf)
                    , "%s_%s_%s"
                     , htmlMachineData.origBaseName
                    , pmi->name->name
                    , pid->name
                     );
           initHTMLWriter(buf);
        }
        else
        {
           initHTMLWriter(NULL);
        }

        fprintf(htmlMachineData.htmlFile,"<h2 class=\"transitionFnName\">%s</h2>\n"
          , pid->name);
        fprintf(htmlMachineData.htmlFile
                ,"<p class=\"docCmnt\">%s</p>\n"
                , pid->docCmnt ? pid->docCmnt : ""
                );
        if (pid->transition_fn_returns_decl)
             {
                fprintf(htmlMachineData.htmlFile
                        ,"\n<br/><br/>Returns:<ul class=\"return_decl\">"
                        );
                for (pasei = pid->transition_fn_returns_decl;
                     pasei;
                     pasei = pasei->next
                     )
                {
                   fprintf(htmlMachineData.htmlFile
                           , "<li>%s</li>\n"
                           , pasei->se->name
                           );
                }
                fprintf(htmlMachineData.htmlFile
                        , "</ul>\n"
                        );
        }
     }

     closeHTMLWriter(1);

  }
	
}

static void closeHTMLWriter(int good)
{

	if (good) {

		fprintf(htmlMachineData.htmlFile
                     , "</body>\n</html>\n"
                     );
             }

  if (htmlMachineData.origBaseName)
  {
     fclose(htmlMachineData.htmlFile);

     if (!good) {

       unlink(htmlMachineData.htmlName);

     }
  }

	CHECK_AND_FREE(htmlMachineData.htmlName);

}

static void _html_page_preamble(char *title)
{
   time_t		now;

   time(&now);

   fprintf(htmlMachineData.htmlFile
                  , "<!DOCTYPE html>\n<html>\n"
                  );

   fprintf(htmlMachineData.htmlFile
                  ,"<!--\n\t%s\n\n",htmlMachineData.htmlName);
   fprintf(htmlMachineData.htmlFile,"\tThis file automatically generated by FSMLang\n\n");
   fprintf(htmlMachineData.htmlFile,"\tOn %s\n\n-->\n",ctime(&now));

   fprintf(htmlMachineData.htmlFile,"<head>\n");

   fprintf(htmlMachineData.htmlFile,"<title>FSM Lang : %s</title>\n",title);

   fprintf(htmlMachineData.htmlFile,"<link REL=stylesheet type=\"text/css\" href=\"fsmlang.css\">\n");

   fprintf(htmlMachineData.htmlFile,"</head><body>\n");

}

static void _print_action_data_block(pHTMLMachineData phmd, pMACHINE_INFO pmi, int e, int s)
{
   pACTION_SE_INFO pasei;
   bool action_has_a_name = pmi->actionArray[e][s] 
                     ? (strlen(pmi->actionArray[e][s]->action->name) > 0)
                              : false
                              ;

   if (pmi->modFlags & mfActionsReturnStates) {

     fprintf(phmd->htmlFile
             ,"\t\t<td class=%s>"
             ,	pmi->actionArray[e][s]
                  ? (action_has_a_name
                     ? "action" 
                        : "noAction") 
                     : "nullAction"
             );

     if (action_has_a_name)
     {
        fprintf(phmd->htmlFile
                , "<a href=\"%s_%s_%s.html\">"
                , phmd->origBaseName
                , pmi->name->name
                , pmi->actionArray[e][s]->action->name
                );
     }

     fprintf(phmd->htmlFile
             , "%s%s"
             ,	pmi->actionArray[e][s] 
                     ? (action_has_a_name
                      ? pmi->actionArray[e][s]->action->name 
                      : "transition") 
                   : "Not Defined"
             ,	pmi->actionArray[e][s] 
                   ? (action_has_a_name
                      ? "</a>"
                      : "") 
                     : ""
                  );

          if (
              pmi->actionArray[e][s]
             && pmi->actionArray[e][s]->action->action_returns_decl
              )
          {
             fprintf(phmd->htmlFile
                     ,"<br/>returns:\n\t<ul class=\"return_decl\">\n"
                     );
             for (pasei = pmi->actionArray[e][s]->action->action_returns_decl;
                  pasei;
                  pasei = pasei->next
                  )
             {
                fprintf(phmd->htmlFile
                        , "\t\t<li>%s</li>\n"
                        , pasei->se->name
                        );
             }
             fprintf(phmd->htmlFile
                     , "\t</ul>\n"
                     );
          }

     if (
         pmi->actionArray[e][s]
        && pmi->actionArray[e][s]->transition
         )
     {
        fprintf(phmd->htmlFile
                , "<br/>%s"
                , pmi->actionArray[e][s]->transition->name
                );

          if (pmi->actionArray[e][s]->transition->transition_fn_returns_decl
              )
          {
             fprintf(phmd->htmlFile
                     ,"<br/>returns:\n\t<ul class=\"return_decl\">\n"
                     );
             for (pasei = pmi->actionArray[e][s]->transition->transition_fn_returns_decl;
                  pasei;
                  pasei = pasei->next
                  )
             {
                fprintf(phmd->htmlFile
                        , "\t\t<li>%s</li>\n"
                        , pasei->se->name
                        );
             }
             fprintf(phmd->htmlFile
                     , "\t</ul>\n"
                     );
          }
          fprintf(phmd->htmlFile
                  , "</td>\n"
                  );

				}

     fprintf(phmd->htmlFile,"</td>\n");

	}
   else {

     fprintf(phmd->htmlFile,"\t\t<td class=%s>"
             ,	pmi->actionArray[e][s] 
                ? (action_has_a_name
                   ? "action" 
                   : "noAction") 
                : "nullAction");

     if (action_has_a_name) {
        fprintf(phmd->htmlFile,"<a href=\"%s_%s_%s.html\">"
                , phmd->origBaseName
                , pmi->name->name
                , pmi->actionArray[e][s]->action->name
                );

	}

     fprintf(phmd->htmlFile,"%s%s"
             ,	pmi->actionArray[e][s] 
                ? (action_has_a_name
                   ? pmi->actionArray[e][s]->action->name 
                   : "noAction") 
                : "&nbsp;"
             ,	pmi->actionArray[e][s] 
                   ? (action_has_a_name
                      ? "</a>"
                      : "") 
                   : "");

     if (
         pmi->actionArray[e][s]
        && pmi->actionArray[e][s]->action->action_returns_decl)
    {
       fprintf(phmd->htmlFile,"<br/>returns:\n\t<ul class=\"return_decl\">\n");
       for (pasei = pmi->actionArray[e][s]->action->action_returns_decl; pasei; pasei = pasei->next)
       {
          fprintf(phmd->htmlFile,"\t\t<li>%s</li>\n"
                  ,pasei->se->name
                  );
       }
       fprintf(phmd->htmlFile,"\t</ul>\n"
               );
    }
    fprintf(phmd->htmlFile, "<br/><b>transition</b> : ");

     if (pmi->actionArray[e][s])
  {
        if (pmi->actionArray[e][s]->transition)
        {
           if (get_id_type(pmi->actionArray[e][s]->transition) == TRANSITION_FN)
           {
        fprintf(phmd->htmlFile,"<a href=\"%s_%s_%s.html\">"
                      , phmd->origBaseName
                      , pmi->name->name
                      , pmi->actionArray[e][s]->transition->name
                      );
           }
        }
     }

     fprintf(phmd->htmlFile,"%s%s"
          , pmi->actionArray[e][s] ? 
               (pmi->actionArray[e][s]->transition ? 
                 pmi->actionArray[e][s]->transition->name : "none")
               : "none"
                , pmi->actionArray[e][s]
               ? (pmi->actionArray[e][s]->transition
                  ? ((get_id_type(pmi->actionArray[e][s]->transition) == TRANSITION_FN)
                     ? "</a>" 
                     : ""
                     )
                  : ""
                )
               : ""
             );
        if (
         pmi->actionArray[e][s]
         && pmi->actionArray[e][s]->transition 
        && pmi->actionArray[e][s]->transition->transition_fn_returns_decl)
        {
           fprintf(phmd->htmlFile,"<br/>returns:\n\t<ul class=\"return_decl\">\n");
           for (pasei = pmi->actionArray[e][s]->transition->transition_fn_returns_decl; pasei; pasei = pasei->next)
           {
              fprintf(phmd->htmlFile,"\t\t<li>%s</li>\n"
                      ,pasei->se->name
                   );
     }
        fprintf(phmd->htmlFile, "\t</ul>\n");
}
     fprintf(phmd->htmlFile,"</td>\n"
             );

	}

}

