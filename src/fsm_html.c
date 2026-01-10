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
#include "event_sequences.h"
#include "util_file_inclusion.h"

#include <stdio.h>
#include <stdlib.h>

#include <time.h>
#if defined (LINUX) || defined (CYGWIN)
#include <string.h>
#include <unistd.h>
#endif

#include "y.tab.h"

/*
	Our interface to the outside world
*/
static int initHTMLWriter(pFSMOutputGenerator,char *);
static void writeHTMLWriter(pFSMOutputGenerator,pMACHINE_INFO);
static void closeHTMLWriter(pFSMOutputGenerator,int);

static int initHTMLFileNameWriter(pFSMOutputGenerator,char *);
static void writeHTMLFileName(pFSMOutputGenerator,pMACHINE_INFO);
static void closeHTMLFileNameWriter(pFSMOutputGenerator,int);

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

static FSMHTMLOutputGenerator HTMLMachineWriter = {
	{
     initHTMLWriter
	 , writeHTMLWriter
	 , closeHTMLWriter
	 , generateHTMLMachineWriter
  }
  , NULL
};

bool  css_content_internal = false;
char *css_content_filename = "fsmlang.css";


static void closeHTMLWriter(pFSMOutputGenerator pfsmog, int good);
static void closeHTMLFileNameWriter(pFSMOutputGenerator pfsmog, int good);
static void writeHTMLWriter(pFSMOutputGenerator pfsmog, pMACHINE_INFO pmi);
static void writeHTMLFileName(pFSMOutputGenerator pfsmog, pMACHINE_INFO pmi);
static int initHTMLWriter (pFSMOutputGenerator pfsmog, char *baseFileName);
static int initHTMLFileNameWriter (pFSMOutputGenerator pfsmog, char *baseFileName);
static bool print_sub_machine_row(pLIST_ELEMENT,void*);
static bool print_event_sequence_html(pLIST_ELEMENT,void*);
static bool print_event_sequence_puml(pLIST_ELEMENT,void*);
static bool print_action_table_row(pLIST_ELEMENT,void*);
static bool print_id_info_as_html_list_element(pLIST_ELEMENT,void*);
static bool print_machine_name_as_list_element(pLIST_ELEMENT,void*);
static bool print_state_table_state_row(pLIST_ELEMENT,void*);
static bool print_event_table_event_row(pLIST_ELEMENT,void*);
static bool print_state_chart_event_header_row(pLIST_ELEMENT,void*);
static bool print_state_chart_state_row(pLIST_ELEMENT,void*);
static bool print_state_chart_state_row_event(pLIST_ELEMENT,void*);
static bool print_transition_fn_table_transition_fn_row(pLIST_ELEMENT,void*);
static bool print_id_info_as_vector(pLIST_ELEMENT,void*);
static bool print_sequence_node_html(pLIST_ELEMENT,void*);
static void print_event_table(pFSMHTMLOutputGenerator);
static void print_state_table(pFSMHTMLOutputGenerator);
static void print_action_table(pFSMHTMLOutputGenerator);
static void print_transition_fn_table(pFSMHTMLOutputGenerator);
static void print_machine_table(pFSMHTMLOutputGenerator);
static void print_event_sequence_table(pFSMHTMLOutputGenerator);
static void print_state_chart(pFSMHTMLOutputGenerator);
static void print_machine_statistics(pFSMHTMLOutputGenerator);
static void print_vector(pLIST,pITERATOR_HELPER);
static void print_transition(pID_INFO,pITERATOR_HELPER);

static void print_machine_statistics(pFSMHTMLOutputGenerator pfsmhtmlog)
{
	FILE         *fout = pfsmhtmlog->pmd->htmlFile;
	pMACHINE_INFO pmi  = pfsmhtmlog->pmd->pmi;

	fprintf(fout
			, "<table class='machine_stats'>\n"
			);

	fprintf(fout
			, "\t<tbody>\n"
			);

	fprintf(fout
			, "\t\t<tr><th>%s</th><td>%u</td></tr>\n"
			, "Number of events"
			, pmi->event_list->count
			);

	fprintf(fout
			, "\t\t<tr><th>%s</th><td>%u</td></tr>\n"
			, "Events not handled"
			, pmi->events_with_zero_handlers
			);

	fprintf(fout
			, "\t\t<tr><th>%s</th><td>%u</td></tr>\n"
			, "Events handled in one state"
			, pmi->events_with_one_handler
			);

	fprintf(fout
			, "\t\t<tr><th>%s</th><td>%s</td></tr>\n"
			, "At least one event handled the same in all states?"
			, pmi->has_single_pai_events ? "yes" : "no"
			);

	fprintf(fout
			, "\t\t<tr><th>%s</th><td>%u</td></tr>\n"
			, "Number of states"
			, pmi->state_list->count
			);

	fprintf(fout
			, "\t\t<tr><th>%s</th><td>%u</td></tr>\n"
			, "Number of states with entry functions"
			, pmi->states_with_entry_fns_count
			);

	fprintf(fout
			, "\t\t<tr><th>%s</th><td>%u</td></tr>\n"
			, "Number of states with exit functions"
			, pmi->states_with_exit_fns_count
			);

	fprintf(fout
			, "\t\t<tr><th>%s</th><td>%u</td></tr>\n"
			, "States handling no events"
			, pmi->states_with_zero_events
			);

	fprintf(fout
			, "\t\t<tr><th>%s</th><td>%u</td></tr>\n"
			, "States handling exactly one event"
			, pmi->states_with_one_event
			);

	fprintf(fout
			, "\t\t<tr><th>%s</th><td>%u</td></tr>\n"
			, "States with no way in"
			, pmi->states_with_no_way_in
			);

	fprintf(fout
			, "\t\t<tr><th>%s</th><td>%u</td></tr>\n"
			, "States with no way out"
			, pmi->states_with_no_way_out
			);

	fprintf(fout
			, "\t</tbody>\n"
			);

	fprintf(fout
			, "</table>\n"
			);
}

static void print_transition(pID_INFO pid, pITERATOR_HELPER pih)
{
	fprintf(pih->fout
			, " transitions %s %s"
			, pid->type == STATE ? "to state" : "via function"
			, pid->name
			);
}

static bool print_id_info_as_vector(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO         pid = (pID_INFO)         pelem->mbr;
	pITERATOR_HELPER pih = (pITERATOR_HELPER) data;

	fprintf(pih->fout
			, "%s%s"
			, pih->first ? (pih->first = false, "") : ", "
			, pid->name
			);

	return false;
}

static void print_vector(pLIST plist, pITERATOR_HELPER pih)
{
	if (plist->count > 1)
	{
		fprintf(pih->fout, "(");
	}

	pih->first = true;
	iterate_list(plist, print_id_info_as_vector, pih);

	if (plist->count > 1)
	{
		fprintf(pih->fout, ")");
	}

}

static void print_state_chart(pFSMHTMLOutputGenerator pfsmhtmlog)
{
	ITERATOR_HELPER ih = {
		.fout = pfsmhtmlog->pmd->htmlFile
		, .pmi = pfsmhtmlog->pmd->pmi
	};

	fprintf(pfsmhtmlog->pmd->htmlFile, "<div class=\"scrollable\">\n<table class=\"machine\">\n");

	/* first row */
	fprintf(pfsmhtmlog->pmd->htmlFile, "\t<thead>\n\t\t<tr>\n");
	fprintf(pfsmhtmlog->pmd->htmlFile, "\t\t\t<th class=\"blankCorner\">&nbsp;</th>\n");

	/* event names */
	iterate_list(pfsmhtmlog->pmd->pmi->event_list
				 , print_state_chart_event_header_row
				 , &ih
				 );

	fprintf(pfsmhtmlog->pmd->htmlFile, "\t\t</tr>\n\t</thead>\n");

	/* State Label column */
	fprintf(pfsmhtmlog->pmd->htmlFile, "\t<tbody>\n");

	iterate_list(pfsmhtmlog->pmd->pmi->state_list
				 , print_state_chart_state_row
				 , &ih
				);

	fprintf(pfsmhtmlog->pmd->htmlFile, "\t</tbody>\n</table>\n</div>\n");

}

static void print_event_table(pFSMHTMLOutputGenerator pfsmhtmlog)
{
	ITERATOR_HELPER ih = {
		.fout = pfsmhtmlog->pmd->htmlFile
		, .pmi = pfsmhtmlog->pmd->pmi
	};
	
	fprintf(pfsmhtmlog->pmd->htmlFile,"<table class=\"elements\">\n");

	fprintf(pfsmhtmlog->pmd->htmlFile,"<thead><tr>\n");
	fprintf(pfsmhtmlog->pmd->htmlFile,"<th colspan='2' align='left'>Events</th>\n");
	fprintf(pfsmhtmlog->pmd->htmlFile,"</tr>\n</thead>\n");
	fprintf(pfsmhtmlog->pmd->htmlFile,"<tbody>\n");
	fprintf(pfsmhtmlog->pmd->htmlFile, "<tr>\n\t<td colspan='2' align='left'>\n");

	fprintf(pfsmhtmlog->pmd->htmlFile, "<table class='summary'>\n");
	fprintf(pfsmhtmlog->pmd->htmlFile
			, "<tr><th>Total number of events:</th><td>%u</td></tr>\n"
			, pfsmhtmlog->pmd->pmi->event_list->count
			);
	fprintf(pfsmhtmlog->pmd->htmlFile
			, "<tr><th>Events with no handlers:</th><td>%u</td></tr>\n"
			, pfsmhtmlog->pmd->pmi->events_with_zero_handlers
			);
	fprintf(pfsmhtmlog->pmd->htmlFile
			, "<tr><th>Events with one handler:</th><td>%u</td></tr>\n"
			, pfsmhtmlog->pmd->pmi->events_with_one_handler
			);
	fprintf(pfsmhtmlog->pmd->htmlFile
			, "<tr><th>Average event state density:</th><td>%u%%</td></tr>\n"
			, pfsmhtmlog->pmd->pmi->average_event_state_density_pct
			);
	fprintf(pfsmhtmlog->pmd->htmlFile, "</table>\n");
	fprintf(pfsmhtmlog->pmd->htmlFile, "\t</td>\n</tr>\n");

	iterate_list(pfsmhtmlog->pmd->pmi->event_list, print_event_table_event_row, &ih);
	fprintf(pfsmhtmlog->pmd->htmlFile,"</tbody>\n</table>\n");

}

static void print_state_table(pFSMHTMLOutputGenerator pfsmhtmlog)
{
	ITERATOR_HELPER ih = {
		.fout = pfsmhtmlog->pmd->htmlFile
		, .pmi = pfsmhtmlog->pmd->pmi
	};
	
	fprintf(pfsmhtmlog->pmd->htmlFile,"<table class=\"elements\">\n");

	fprintf(pfsmhtmlog->pmd->htmlFile,"<thead><tr>\n");
	fprintf(pfsmhtmlog->pmd->htmlFile,"<th colspan=2 align=left>States</th>\n");
	fprintf(pfsmhtmlog->pmd->htmlFile,"</tr>\n</thead>\n<tbody>\n");
	fprintf(pfsmhtmlog->pmd->htmlFile, "<tr><td colspan='2' align='left'>\n");
	fprintf(pfsmhtmlog->pmd->htmlFile, "<table class='summary'>\n");
	fprintf(pfsmhtmlog->pmd->htmlFile
			, "<tr><th>Total number of states:</th><td>%u</td></tr>\n"
			, pfsmhtmlog->pmd->pmi->state_list->count
			);
	fprintf(pfsmhtmlog->pmd->htmlFile
			, "<tr><th>States with no events:</th><td>%u</td></tr>\n"
			, pfsmhtmlog->pmd->pmi->states_with_zero_events
			);
	fprintf(pfsmhtmlog->pmd->htmlFile
			, "<tr><th>States with one event:</th><td>%u</td></tr>\n"
			, pfsmhtmlog->pmd->pmi->states_with_one_event
			);
	fprintf(pfsmhtmlog->pmd->htmlFile
			, "<tr><th>States with no way in:</th><td>%u</td></tr>\n"
			, pfsmhtmlog->pmd->pmi->states_with_no_way_in
			);
	fprintf(pfsmhtmlog->pmd->htmlFile
			, "<tr><th>States with no way out:</th><td>%u</td></tr>\n"
			, pfsmhtmlog->pmd->pmi->states_with_no_way_out
			);
	fprintf(pfsmhtmlog->pmd->htmlFile
			, "<tr><th>Average state event density:</th><td>%u%%</td></tr>\n"
			, pfsmhtmlog->pmd->pmi->average_state_event_density_pct
			);
	fprintf(pfsmhtmlog->pmd->htmlFile, "</table>\n");
	fprintf(pfsmhtmlog->pmd->htmlFile, "\t</td>\n</tr>\n");

	iterate_list(pfsmhtmlog->pmd->pmi->state_list, print_state_table_state_row, &ih);

	fprintf(pfsmhtmlog->pmd->htmlFile,"</tbody>\n</table>\n");

}

static void print_action_table(pFSMHTMLOutputGenerator pfsmhtmlog)
{
	ITERATOR_HELPER ih = {
		.fout = pfsmhtmlog->pmd->htmlFile
		, .pmi = pfsmhtmlog->pmd->pmi
	};
	
	fprintf(pfsmhtmlog->pmd->htmlFile,"<table class=\"elements\">\n");

	fprintf(pfsmhtmlog->pmd->htmlFile,"<thead><tr>\n");
	fprintf(pfsmhtmlog->pmd->htmlFile,"<th colspan=2 align=left>Actions</th>\n");
	fprintf(pfsmhtmlog->pmd->htmlFile,"</tr>\n</thead>\n<tbody>\n");
	
	iterate_list(pfsmhtmlog->pmd->pmi->action_list
				 , print_action_table_row
				 , &ih);

	fprintf(pfsmhtmlog->pmd->htmlFile,"</tbody>\n</table>\n");

}

static void print_transition_fn_table(pFSMHTMLOutputGenerator pfsmhtmlog)
{
	RETURN_IF_NULL(pfsmhtmlog->pmd->pmi->transition_fn_list);

	ITERATOR_HELPER ih = {
		.fout = pfsmhtmlog->pmd->htmlFile
		, .pmi = pfsmhtmlog->pmd->pmi
	};
	
	fprintf(pfsmhtmlog->pmd->htmlFile,"<table class=\"elements\">\n");

	fprintf(pfsmhtmlog->pmd->htmlFile,"<thead><tr>\n");
	fprintf(pfsmhtmlog->pmd->htmlFile,"<th colspan=2 align=left>Guard Functions</th>\n");
	fprintf(pfsmhtmlog->pmd->htmlFile,"</tr>\n</thead>\n<tbody>\n");
	
	iterate_list(pfsmhtmlog->pmd->pmi->transition_fn_list, print_transition_fn_table_transition_fn_row, &ih);

	fprintf(pfsmhtmlog->pmd->htmlFile,"</tbody>\n</table>\n");

}

static void print_machine_table(pFSMHTMLOutputGenerator pfsmhtmlog)
{
	RETURN_IF_NULL(pfsmhtmlog->pmd->pmi->transition_fn_list)

	fprintf(pfsmhtmlog->pmd->htmlFile,"<table class=\"elements\">\n");

	fprintf(pfsmhtmlog->pmd->htmlFile,"<thead><tr>\n");
	fprintf(pfsmhtmlog->pmd->htmlFile,"<th colspan=2 align=left>Sub Machines</th>\n");
	fprintf(pfsmhtmlog->pmd->htmlFile,"</tr>\n</thead>\n<tbody>\n");
	
	iterate_list(pfsmhtmlog->pmd->pmi->machine_list, print_sub_machine_row, pfsmhtmlog);

	fprintf(pfsmhtmlog->pmd->htmlFile,"</tbody>\n</table>\n");

}

static void print_event_sequence_table(pFSMHTMLOutputGenerator pfsmhtmlog)
{
	RETURN_IF_NULL(pfsmhtmlog->pmd->pmi->transition_fn_list)

	fprintf(pfsmhtmlog->pmd->htmlFile,"<table class=\"elements\">\n");

	fprintf(pfsmhtmlog->pmd->htmlFile
			,"<thead><tr>\n"
			);
	fprintf(pfsmhtmlog->pmd->htmlFile
			,"<th colspan=%u align=left>Event Sequences</th>\n"
			, include_svg_img ? 1 : 5
			);
	fprintf(pfsmhtmlog->pmd->htmlFile
			,"</tr>\n"
			);

	if (!include_svg_img)
	{
		fprintf(pfsmhtmlog->pmd->htmlFile
				, "<tr><th>Name</th><th>Comment</th>"
				);
		fprintf(pfsmhtmlog->pmd->htmlFile
				, "<th>Initial State</th><th>Sequence</th>"
				);
		fprintf(pfsmhtmlog->pmd->htmlFile
				, "<th>Final State</th></tr>\n"
				);
	}

	fprintf(pfsmhtmlog->pmd->htmlFile
			, "</thead>\n<tbody>\n"
			);
	
	iterate_list(pfsmhtmlog->pmd->pmi->sequences
				 , include_svg_img 
				   ? print_event_sequence_puml
				   : print_event_sequence_html
				 , pfsmhtmlog
				 );

	fprintf(pfsmhtmlog->pmd->htmlFile,"</tbody>\n</table>\n");

}

static bool print_state_chart_state_row(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO         pstate   = (pID_INFO)         pelem->mbr;
	pITERATOR_HELPER pih      = (pITERATOR_HELPER) data;

	fprintf(pih->fout
			, "\t<tr>\n\t\t<th class=stateName>%s</th>\n"
			, pstate->name
			);

	pih->state = pelem->ordinal;
	iterate_list(pih->pmi->event_list
				 , print_state_chart_state_row_event
				 , pih
				 );

	fprintf(pih->fout,"\t</tr>\n");

	return false;

}

static bool print_state_chart_state_row_event(pLIST_ELEMENT pelem, void *data)
{
	pITERATOR_HELPER pih    = (pITERATOR_HELPER) data;
	pMACHINE_INFO    pmi    = pih->pmi;
	pACTION_INFO     pai    = pmi->actionArray[pelem->ordinal][pih->state];


	fprintf(pih->fout
			,"\t\t<td class="
			);

	if (pmi->modFlags & mfActionsReturnStates) {

		fprintf(pih->fout
				, "%s>%s"
				, pai
				  ? (strlen(pai->action->name) 
			        ? "action" : "noAction") 
			      : "nullAction"
				, pai 
			      ? (strlen(pai->action->name) 
			        ? pai->action->name 
			        : "transition") 
			      : "Not Defined"
				);

		if (
			pai
		   && pai->action->type_data.action_data.action_returns_decl
			)
		{
		   fprintf(pih->fout
				   ,"<br/>returns:\n\t<ul class=\"return_decl\">\n"
				   );
		   iterate_list(pai->action->type_data.action_data.action_returns_decl
						, print_id_info_as_html_list_element
						, pih
						);
		   fprintf(pih->fout
				   , "\t</ul>\n"
				   );
		}

		if (
			pai
		   && pai->transition
			)
		{
		   fprintf(pih->fout
				   , "<br/>%s"
				   , pai->transition->name
				   );

		   if (pai->transition->transition_fn_returns_decl)
		   {
			  fprintf(pih->fout
					  ,"<br/>returns:\n\t<ul class=\"return_decl\">\n"
					  );
			  iterate_list(pai->transition->transition_fn_returns_decl
						   , print_id_info_as_html_list_element
						   , pih
						   );
			  fprintf(pih->fout
					  , "\t</ul>\n"
					  );
		   }
		}

	}
	else {

		fprintf(pih->fout
				, "%s>%s"
				, pai 
				  ? (strlen(pai->action->name) 
					? "action" 
			        : "noAction") 
				  : "nullAction"
				, pai 
				  ? (strlen(pai->action->name) 
			        ? pai->action->name 
			        : "noAction") 
		          : ""
				);

		if (
			pai
			)
		{
		   /* if this action is associated with a shared event, it will have exactly one event */
		   pID_INFO paction = pai->action;
		   pID_INFO pevent = (pID_INFO)find_nth_list_member(paction->type_data.action_data.actionInfo->matrix->event_list,0);

		   /* and, that event will have a list of sharing machines */
		   if (pevent->type_data.event_data.psharing_sub_machines)
		   {
			  fprintf(pih->fout
					  ,"<br/>shares event with:\n\t<ul class=\"return_decl\">\n"
					  );
			  iterate_list(pevent->type_data.event_data.psharing_sub_machines
						   , print_machine_name_as_list_element
						   , pih->fout
						   );
			  fprintf(pih->fout,"</ul>\n"
					  );
		   }
		   else
		   {
			  if (paction->type_data.action_data.action_returns_decl)
			  {
				 fprintf(pih->fout
						 ,"<br/>returns:\n\t<ul class=\"return_decl\">\n"
						 );
				 iterate_list(pai->action->type_data.action_data.action_returns_decl
							  , print_id_info_as_html_list_element
							  , pih
							  );
				 fprintf(pih->fout
						 , "\t</ul>\n"
						 );
			  }
		   }
		}

		fprintf(pih->fout
				, "<br/><b>%s</b> : %s"
				, pai && pai->transition && (pai->transition->type != STATE) 
				    ? "guard"
				    : "transition"
				, pai && pai->transition
				  ? pai->transition->name
				  : "none"
				);

		if (
			pai
			&& pai->transition 
			&& pai->transition->transition_fn_returns_decl
			)
		{
		   fprintf(pih->fout
				   ,"<br/>returns:\n\t<ul class=\"return_decl\">\n"
				   );
		   iterate_list(pai->transition->transition_fn_returns_decl
						, print_id_info_as_html_list_element
						, pih
						);
		   fprintf(pih->fout
				   , "\t</ul>\n"
				   );
		}

	}

	if (
		pai
		&& pai->docCmnt
		)
	{
	   fprintf(pih->fout
			   ,"<p class=\"transition_comment\">%s</p>"
			   , pai->docCmnt
			   );
	}

	fprintf(pih->fout
			, "</td>\n"
			);

	return false;

}

static bool print_state_chart_event_header_row(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO         pevent   = (pID_INFO)         pelem->mbr;
	pITERATOR_HELPER pih      = (pITERATOR_HELPER) data;
	pEVENT_DATA      ped      = &pevent->type_data.event_data;
	static char      *prefix  = "\t\t\t<th class='eventName'>";
	static char      *postfix = "</th>\n";

	fprintf(pih->fout
			, ped->shared_with_parent
				? "%s(%s::) %s%s"
				: "%s%s%s%s"
			, prefix
			, ped->shared_with_parent ? pih->pmi->parent->name->name : ""
			, pevent->name
			, postfix
			);

	return false;
}
	
static bool print_event_table_event_row(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO         pevent = (pID_INFO)         pelem->mbr;
	pITERATOR_HELPER pih    = (pITERATOR_HELPER) data;
	pEVENT_DATA      ped    = &pevent->type_data.event_data;
	
	fprintf(pih->fout, "<tr>\n");
	fprintf(pih->fout
			, ped->shared_with_parent
			  ? "<td class=\"label%s\">(%s::) %s</td>\n"
			  : "<td class=\"label%s\">%s%s</td>\n"
			, ped->phandling_states->count == 0
			  ? " eventWithNoHandler"
			  : ""
			, ped->shared_with_parent
			  ? pih->pmi->parent->name->name
			  : ""
			, pevent->name
		   );
	fprintf(pih->fout, "<td>%s\n"
			, pevent->docCmnt ? pevent->docCmnt : ""
			);

	if (ped->psharing_sub_machines)
	{
		fprintf(pih->fout
				, "<p>This event is shared with:</p><ul class=\"return_decl\">\n"
			   );
		iterate_list(ped->psharing_sub_machines
					 , print_machine_name_as_list_element
					 , pih->fout
					);
		fprintf(pih->fout, "</ul>\n"
			   );
	}

	if (ped->puser_event_data)
	{
		if (ped->puser_event_data->translator)
		{
			fprintf(pih->fout, "<p>\n");
			fprintf(pih->fout
					, "Data translator: %s\n"
					, ped->puser_event_data->translator->name
				   );
			fprintf(pih->fout, "</p>\n");
		}

		if (ped->puser_event_data->data_fields)
		{
			ITERATOR_HELPER ih = {
				.fout = pih->fout
				, .tab_level = 0
			};

			fprintf(pih->fout, "<p>\n");
			fprintf(pih->fout
					, "Event data:"
				   );
			fprintf(pih->fout, "</p>\n");

			fprintf(pih->fout, "<code>\n");
			iterate_list(ped->puser_event_data->data_fields
						 , print_data_field
						 , &ih
						);
			fprintf(pih->fout, "</code>\n");
		}
	}

	if (ped->phandling_states->count)
	{
		fprintf(pih->fout
				, "<p>Handled In %u of %u (%u %%) States:</p>\n\t<ul>\n"
				, ped->phandling_states->count
				, pih->pmi->state_list->count
				, ped->state_density_pct
				);

		iterate_list(ped->phandling_states
					 , print_id_info_as_html_list_element
					 , pih
					 );

		fprintf(pih->fout, "\t</ul>\n");
	}

	if (ped->pactions_list->count)
	{
		fprintf(pih->fout
				, "<p>Causes these actions to be taken:</p>\n\t<ul>\n"
				);

		iterate_list(ped->pactions_list
					 , print_id_info_as_html_list_element
					 , pih
					 );

		fprintf(pih->fout, "\t</ul>\n");
	}

	fprintf(pih->fout, "</td>\n");
	fprintf(pih->fout, "</tr>\n");

	return false;
}

static bool print_state_table_state_row(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO         pstate = (pID_INFO)pelem->mbr;
	pITERATOR_HELPER pih    = (pITERATOR_HELPER)data;
	pSTATE_DATA      psd    = &pstate->type_data.state_data;

	bool something_printed  = false;

	fprintf(pih->fout, "<tr>\n");

	fprintf(pih->fout
			,"<td class='label%s%s%s'>%s</td>\n<td>\n"
			, psd->pevents_handled->count == 0       ? " lazyState"        : ""
			, ((psd->pinbound_transitions->count == 0) && (pelem->ordinal != 0))
			    ? " stateWithNoEntry" : ""
			, psd->poutbound_transitions->count == 0 ? " deadEndState"     : ""
			, pstate->name
			);

	if (psd->pevents_handled->count == 0)
	{
		fprintf(pih->fout
				, "<p class='lazyState'>This state handles no events.</p>\n"
				);
		something_printed = true;
	}

	if (
		(psd->pinbound_transitions->count == 0)
		&& (pelem->ordinal != 0)
	   )
	{
		fprintf(pih->fout
				, "<p class='stateWithNoEntry'>This state has no entry points.</p>\n"
				);
		something_printed = true;
	}

	if (psd->poutbound_transitions->count == 0)
	{
		fprintf(pih->fout
				, "<p class='deadEndState'>This state has no way out.</p>\n"
				);
		something_printed = true;
	}

	if (pstate->docCmnt)
	{
		fprintf(pih->fout
				, "<p>%s</p>\n"
				, pstate->docCmnt
			   );
		something_printed = true;
	}

	if (psd->state_flags & sfHasEntryFn)
	{
		fprintf(pih->fout
				, "<p>On Entry: %s%s</p>\n"
				, psd->entry_fn
				? psd->entry_fn->name
				: "onEntryTo_"
				, psd->entry_fn
				? ""
				: pstate->name
			   );
		something_printed = true;
	}

	if (psd->state_flags & sfHasExitFn)
	{
		fprintf(pih->fout
				, "<p>On Exit: %s%s</p>\n"
				, psd->exit_fn
				? psd->exit_fn->name
				: "onExitTo_"
				, psd->exit_fn
				? ""
				: pstate->name
			   );
		something_printed = true;
	}

	if (psd->state_flags & sfInibitSubMachines)
	{
		fprintf(pih->fout
				, "<p>Inhibits sub-machines.</p>\n"
			   );
		something_printed = true;
	}

	if (psd->pinbound_transitions->count)
	{
		fprintf(pih->fout
				, "<p>Inbound Transitions:</p>\n\t<ul>\n"
				);
		iterate_list(psd->pinbound_transitions
					 , print_id_info_as_html_list_element
					 , pih
					 );
		fprintf(pih->fout
				, "</ul>\n"
				);
		something_printed = true;
	}

	if (psd->poutbound_transitions->count)
	{
		fprintf(pih->fout
				, "<p>Outbound Transitions:</p>\n\t<ul>\n"
				);
		iterate_list(psd->poutbound_transitions
					 , print_id_info_as_html_list_element
					 , pih
					 );
		fprintf(pih->fout
				, "</ul>\n"
				);
		something_printed = true;
	}

	if (psd->pevents_handled->count)
	{
		fprintf(pih->fout
				, "<p>Events Handled (%u of %u for %u%%):</p>\n\t<ul>\n"
				, psd->pevents_handled->count
				, pih->pmi->event_list->count
				, psd->event_density_pct
				);

		iterate_list(psd->pevents_handled
					 , print_id_info_as_html_list_element
					 , pih
					 );

		fprintf(pih->fout
				, "\t</ul>\n"
				);
		something_printed = true;
	}

	if (psd->pactions_list->count)
	{
		fprintf(pih->fout
				, "<p>Actions Taken:\n</p>\n\t<ul>\n"
				);

		iterate_list(psd->pactions_list
					 , print_id_info_as_html_list_element
					 , pih
					 );

		fprintf(pih->fout
				, "\t</ul>\n"
				);
		something_printed = true;
	}

	if (!something_printed)
	{
		fprintf(pih->fout, "&nbsp;");
	}

	fprintf(pih->fout, "</td></tr>\n");

	return false;

}

static bool print_transition_fn_table_transition_fn_row(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO         ptransition_fn = (pID_INFO)         pelem->mbr;
	pITERATOR_HELPER pih            = (pITERATOR_HELPER) data;

	fprintf(pih->fout
			,"\t<tr><td class='label'>%s</td><td>"
			, ptransition_fn->name
			);

	if (ptransition_fn->docCmnt)
	{
		fprintf(pih->fout
				, "%s"
				, ptransition_fn->docCmnt
				);

	}

	if (ptransition_fn->transition_fn_returns_decl)
	{
		fprintf(pih->fout
				, "<p>Returns:\n\t<ul>\n"
			   );

		iterate_list(ptransition_fn->transition_fn_returns_decl
					 , print_id_info_as_html_list_element
					 , pih
					);

		fprintf(pih->fout, "\t</ul>\n</p>\n");
	}

	fprintf(pih->fout, "</td>\n");

	return false;
}

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
   pITERATOR_HELPER pih = (pITERATOR_HELPER) data;
   pID_INFO         pid = (pID_INFO) pelem->mbr;

   fprintf(pih->fout
           , (pid->powningMachine && (pid->powningMachine != pih->pmi))
               ? "\t\t<li>%s::%s</li>\n"
               : "\t\t<li>%s%s</li>\n"
           , (pid->powningMachine && (pid->powningMachine != pih->pmi))
               ? pid->powningMachine->name->name
               : ""
           , pid->name
           );

   return false;
}

static bool print_action_table_row(pLIST_ELEMENT pelem, void *data)
{
   pID_INFO         paction = (pID_INFO)pelem->mbr;
   pITERATOR_HELPER pih     = (pITERATOR_HELPER) data;
   pACTION_DATA     pad     = &paction->type_data.action_data;

   fprintf(pih->fout,"<tr>\n");
   fprintf(pih->fout,"<td class=\"label\">%s</td>\n"
     , strlen(paction->name) ? paction->name : "transition");
   fprintf(pih->fout,"<td>\n");
   if (paction->docCmnt)
   {
	   fprintf(pih->fout, "%s"
			   , paction->docCmnt
			  );
   }

   if (pad->actionInfo)
   {
      /* if this action is associated with a shared event, it will have exactly one event */
      pID_INFO pevent = (pID_INFO)find_nth_list_member(pad->actionInfo->matrix->event_list,0);

      /* and, that event will have a list of sharing machines */
      if (pevent->type_data.event_data.psharing_sub_machines)
      {
         fprintf(pih->fout,"\n<br/><br/>Shares event to:<ul class=\"return_decl\">");
         iterate_list(pevent->type_data.event_data.psharing_sub_machines
                      , print_machine_name_as_list_element
                      , pih->fout
                      );
         fprintf(pih->fout,"</ul>\n"
                 );
      }
      else
      {
         if (pevent->type_data.event_data.shared_with_parent)
         {
            fprintf(pih->fout
                    ,"\n<p>Handles an event shared from %s</p>\n"
                    , pevent->powningMachine->parent->name->name
                    );
         }

         if (pad->action_returns_decl)
         {
            fprintf(pih->fout,"\n<br/><br/>Returns:<ul class=\"return_decl\">");
            iterate_list(pad->action_returns_decl
 						, print_id_info_as_html_list_element
 						, pih
 						);
            fprintf(pih->fout,"</ul>\n"
                    );
         }
      }

	  /* List the matrices. */
	  fprintf(pih->fout
			  , "<p>Matrices:</p>\n\t<ul>\n"
			  );

	  for (pACTION_INFO pai = pad->actionInfo; pai; pai = pai->nextAction)
	  {
		  fprintf(pih->fout, "\t\t<li>");
		  print_vector(pai->matrix->event_list, pih);
		  fprintf(pih->fout, ", ");
		  print_vector(pai->matrix->state_list, pih);

		  if (pai->transition)
		  {
			  print_transition(pai->transition, pih);
		  }
		  fprintf(pih->fout, "\t\t</li>");
	  }
	  fprintf(pih->fout, "\t</ul>\n");

      fprintf(pih->fout, "</td>\n</tr>\n");

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
   if (pmi->name->docCmnt)
   {
	   fprintf(pfsmhtmlog->pmd->htmlFile
			   , "%s"
			   , pmi->name->docCmnt
			  );
   }

   fprintf(pfsmhtmlog->pmd->htmlFile, "</td>\n</tr>\n");

   return false;
}

static bool print_event_sequence_html(pLIST_ELEMENT pelem, void *data)
{
   pEVENT_SEQUENCE         psequence  = ((pEVENT_SEQUENCE)pelem->mbr);
   pFSMHTMLOutputGenerator pfsmhtmlog = (pFSMHTMLOutputGenerator) data;

   fprintf(pfsmhtmlog->pmd->htmlFile
           ,"<tr>\n"
           );
   fprintf(pfsmhtmlog->pmd->htmlFile
           ,"<td class=\"label\">%s</td>\n"
		   , psequence->name->name
		   );

   fprintf(pfsmhtmlog->pmd->htmlFile,"<td>\n");
   if (psequence->docCmt)
   {
	   fprintf(pfsmhtmlog->pmd->htmlFile
			   , "%s"
			   , psequence->docCmt
			  );
   }
   fprintf(pfsmhtmlog->pmd->htmlFile, "</td>\n");

   fprintf(pfsmhtmlog->pmd->htmlFile,"<td>\n");
   if (psequence->initial_state)
   {
	   fprintf(pfsmhtmlog->pmd->htmlFile
			   , "%s"
			   , psequence->initial_state->name
			   );
   }
   fprintf(pfsmhtmlog->pmd->htmlFile, "</td>\n");

   fprintf(pfsmhtmlog->pmd->htmlFile,"<td>\n");
   SEQUENCE_ITERATOR_HELPER sih = {0};

   sih.ih.fout            = pfsmhtmlog->pmd->htmlFile;
   sih.ih.pmi             = pfsmhtmlog->pmd->pmi;
   sih.st.pcurr_state     = psequence->initial_state;

   fprintf(pfsmhtmlog->pmd->htmlFile,"<ul class=\"event_sequence\">\n");
   iterate_list(psequence->sequence
				, print_sequence_node_html
				, &sih
				);

   fprintf(pfsmhtmlog->pmd->htmlFile,"</ul>\n");
   fprintf(pfsmhtmlog->pmd->htmlFile, "</td>\n");

   fprintf(pfsmhtmlog->pmd->htmlFile,"<td>\n");
   fprintf(pfsmhtmlog->pmd->htmlFile
		   , "<p>Traced: %s.</p>"
		   , sih.st.pcurr_state->name
		   );
   if (psequence->final_state)
   {
	   fprintf(pfsmhtmlog->pmd->htmlFile
			   , "<p%s>%s was given in the sequence%s.</p>"
			   , psequence->final_state != sih.st.pcurr_state
			     ? " class=\"end_state_mismatch\""
			     : ""
			   , psequence->final_state->name
			   , psequence->final_state != sih.st.pcurr_state
			     ? "; this does not match the traced state"
			     : ""
			   );

   }
   fprintf(pfsmhtmlog->pmd->htmlFile, "</td>\n");

   fprintf(pfsmhtmlog->pmd->htmlFile, "</tr>\n");

   return false;
}

/* Main section */
static int initHTMLFileNameWriter (pFSMOutputGenerator pfsmog, char *baseFileName)
{
	pFSMHTMLOutputGenerator pfsmhtmlog = (pFSMHTMLOutputGenerator) pfsmog;
	if (NULL != (pfsmhtmlog->pmd = calloc(1, sizeof(HTMLMachineData))))
	{
		if (baseFileName)
		{
			pfsmhtmlog->pmd->htmlName = createFileName(baseFileName,".html");
		}
	}

	/* this may look funny, but it does the trick */
	return ((int) !pfsmhtmlog->pmd->htmlName);
}

static int initHTMLWriter (pFSMOutputGenerator pfsmog, char *baseFileName)
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
			int ret;
            if (0 != (ret = copyFileContents(pfsmhtmlog->pmd->htmlFile, css_content_filename)))
            {
                fprintf(stderr,"%s: Could not copy css file contents\n",me);
				fprintf(pfsmhtmlog->pmd->htmlFile
						,"<!-- ret: %d -->\n"
						, ret
						);
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

static void writeHTMLFileName(pFSMOutputGenerator pfsmog, pMACHINE_INFO pmi)
{
	pFSMHTMLOutputGenerator pfsmhtmlog = (pFSMHTMLOutputGenerator)pfsmog;

	if (output_make_recipe && !pmi->parent)
	{
		printf("%s: %s.fsm "
			   , pfsmhtmlog->pmd->htmlName
			   , inputFileName
			   );
		
		print_included_files_list();

		printf("\n");
	}

	if (!output_make_recipe || pmi->parent)
	{
		printf("%s ", pfsmhtmlog->pmd->htmlName);
	}

	if (pmi->machine_list)
	{
		write_machines(pmi->machine_list, generateHTMLMachineWriter, pfsmog);
	}

	if (output_make_recipe && !pmi->parent && pmi->machine_list)
	{
		printf(": %s\n"
			   , pfsmhtmlog->pmd->htmlName
			   );

	}
}

static void writeHTMLWriter(pFSMOutputGenerator pfsmog, pMACHINE_INFO pmi)
{

	RETURN_IF_NULL(pmi);

	pFSMHTMLOutputGenerator pfsmhtmlog = (pFSMHTMLOutputGenerator)pfsmog;

	pfsmhtmlog->pmd->pmi = pmi;

	fprintf(pfsmhtmlog->pmd->htmlFile, "<h2>");
	printAncestry(pmi, pfsmhtmlog->pmd->htmlFile, "::", alc_lower, ai_include_self);
	fprintf(pfsmhtmlog->pmd->htmlFile, "</h2>\n");

	if (pmi->name->docCmnt) fprintf(pfsmhtmlog->pmd->htmlFile, "<p>%s</p>\n", pmi->name->docCmnt);

	print_machine_statistics(pfsmhtmlog);

	if (include_svg_img)
	{
		fprintf(pfsmhtmlog->pmd->htmlFile
				, "<img src=\"%s.svg\" alt=\"PlantUML diagram separately generated.\"/>\n"
				, pfsmhtmlog->pmd->baseName
			   );
	}


	print_state_chart(pfsmhtmlog);
	print_event_table(pfsmhtmlog);
	print_state_table(pfsmhtmlog);
	print_action_table(pfsmhtmlog);

	if (pmi->transition_fn_list->count)
	{
		print_transition_fn_table(pfsmhtmlog);
	}

	if (pmi->sequences)
	{
		print_event_sequence_table(pfsmhtmlog);
	}

	if (pmi->machine_list)
	{
		print_machine_table(pfsmhtmlog);
		write_machines(pmi->machine_list, generateHTMLMachineWriter, pfsmog);
	}
}

static void closeHTMLFileNameWriter(pFSMOutputGenerator pfsmog, int good)
{
	pFSMHTMLOutputGenerator pfsmhtmlog = (pFSMHTMLOutputGenerator) pfsmog;

	(void) good;

	if (!pfsmhtmlog || !pfsmhtmlog->pmd)
	{
		return;
	}

	CHECK_AND_FREE(pfsmhtmlog->pmd->htmlName);

	free(pfsmhtmlog->pmd);
}

static void closeHTMLWriter(pFSMOutputGenerator pfsmog, int good)
{
   pFSMHTMLOutputGenerator pfsmhtmlog = (pFSMHTMLOutputGenerator) pfsmog;

   if (!pfsmhtmlog || !pfsmhtmlog->pmd)
   {
	   return;
   }

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

   free(pfsmhtmlog->pmd);

}

pFSMOutputGenerator generateHTMLMachineWriter(pFSMOutputGenerator parent)
{
	pFSMOutputGenerator pfsmog;

	if (parent)
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

	if (output_generated_file_names_only)
	{
		pfsmog->writeMachine = writeHTMLFileName;
		pfsmog->initOutput   = initHTMLFileNameWriter;
		pfsmog->closeOutput  = closeHTMLFileNameWriter;
	}

	return pfsmog;
}

static bool print_sequence_node_html(pLIST_ELEMENT pelem, void *data)
{
	pEVENT_SEQUENCE_NODE      pesn  = (pEVENT_SEQUENCE_NODE) pelem->mbr;
	pITERATOR_HELPER          pih   = (pITERATOR_HELPER) data;
	pSEQUENCE_ITERATOR_HELPER psih  = (pSEQUENCE_ITERATOR_HELPER) data;

	//Grab the current action before moving to the next state.
	pID_INFO paction = get_action(pih->pmi
								  , pesn->pevent->order
								  , psih->st.pcurr_state->order
								  );

	fprintf(pih->fout
			, "<li>%s: %s &rarr; "
			, pesn->pevent->name
			, psih->st.pcurr_state->name
			);

	TRANSITION_NOTE tn = determine_next_state(pih->pmi, pesn, &psih->st);

	fprintf(pih->fout
			, "%s (%s)"
			, psih->st.pcurr_state->name
			, paction ? paction->name : "No Action"
			);

	if (tn != tn_none)
	{

		fprintf(pih->fout
				, "\n<ul class=\"transition_note\">\n"
				);

		switch (tn)
		{
		case tn_no_fsm_transition:
			fprintf(pih->fout
					, "<li class=\"transition_warning\">The sequence indicates a EVENT_SEQUENCE, but none is given in the FSM."
					);
			break;
		case tn_state_mismatch:
			fprintf(pih->fout
					, "<li class=\"transition_warning\">The transition indicated in the sequence does not match the FSM."
					);
			break;
		case tn_fn_mismatch:
		case tn_fn_match:
			fprintf(pih->fout
					, "<li%s>State machine indicates transition is via function %s.<br/><br/>The transition given in the sequence is %sfound in the function's return declaration."
					, tn == tn_fn_match ? "" : " class=\"transition_warning\""
					, psih->st.pcurr_transition->name
					, tn == tn_fn_match ? "" : "not "
					);
			break;
		case tn_first_return:
			fprintf(pih->fout
					, "<li class=\"transition_warning\">State machine indicates transition is via function %s and no transition was indicated in the sequence."
					, psih->st.pcurr_transition->name
					);
			fprintf(pih->fout, "<br/><br/>The first indicated return value was chosen.");
			break;
		case tn_no_fn_return_list:
			fprintf(pih->fout
					, "<li class=\"transition_warning\">State machine indicates transition is via function %s and no transition was indicated in the sequence."
					, psih->st.pcurr_transition->name
					);
			fprintf(pih->fout, "<br/><br/>Furthermore, no return list was given for the function.");
			break;
		default:
			break;
		}

		fprintf(pih->fout
				, "\n</li>\n</ul>\n"
				);

	}

	fprintf(pih->fout
			, "</li>\n"
			);

	return false;
}

static bool print_event_sequence_puml(pLIST_ELEMENT pelem, void *data)
{
   pEVENT_SEQUENCE         psequence  = ((pEVENT_SEQUENCE)pelem->mbr);
   pFSMHTMLOutputGenerator pfsmhtmlog = (pFSMHTMLOutputGenerator) data;
   char                    *fn;

   fprintf(pfsmhtmlog->pmd->htmlFile
		   , "<tr><td><img src=\"%s.svg\" alt=\"PlantUML diagram separately generated.\"/></td></tr>\n"
		   , (fn = generate_sequence_file_name(psequence
											   , pfsmhtmlog->pmd->pmi
											   ))
		   );

   CHECK_AND_FREE(fn);

   return false;
}
