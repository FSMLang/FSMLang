/*
	fsm_utils.c

	Contains utility functions for the fsmlang compiler


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

*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "fsm_priv.h"
#include "list.h"
#include "ancestry.h"

#ifndef LEX_DEBUG

#if defined (CYGWIN) || defined (LINUX)
#include "y.tab.h"
#endif 
#ifdef VS
#include "parser.h"
#endif

#else
#include "lexer_debug.h"
#endif

typedef struct _action_array_population_helper_ ACTION_ARRAY_POPULATION_HELPER, *pACTION_ARRAY_POPULATION_HELPER;
struct _action_array_population_helper_ 
{
   FILE          *fout;
   pMACHINE_INFO pmi;
   pACTION_INFO  pai;
   pID_INFO      pevent;
   pID_INFO      pstate;
   bool          error;
};


static bool recursePrintAncestry(pMACHINE_INFO,FILE*,char*,ANCESTRY_LETTER_CASE,ANCESTRY_INCLUSION);
static bool write_machine(pLIST_ELEMENT,void*);
static bool count_entry_and_exit_handlers(pLIST_ELEMENT,void*);
static bool count_data_xlate(pLIST_ELEMENT,void*);
static bool count_shared_evts(pLIST_ELEMENT,void*);
static bool count_parent_event_refs(pLIST_ELEMENT,void*);
static bool count_inhibitors(pLIST_ELEMENT,void*);
static bool count_external(pLIST_ELEMENT,void*);
static bool count_zero_event_states(pLIST_ELEMENT,void*);
static bool count_one_event_states(pLIST_ELEMENT,void*);
static bool count_zero_event_handlers(pLIST_ELEMENT,void*);
static bool count_one_event_handlers(pLIST_ELEMENT,void*);
static bool count_no_way_in_states(pLIST_ELEMENT,void*);
static bool count_no_way_out_states(pLIST_ELEMENT,void*);
static bool enumerate_pid(pLIST_ELEMENT,void*);
static bool process_action_info(pLIST_ELEMENT,void*);
static bool iterate_matrix_states(pLIST_ELEMENT,void*);
static bool add_to_action_array(pLIST_ELEMENT,void*);
static bool find_id_by_name(pLIST_ELEMENT,void*);
static bool compute_state_density_pct_and_average(pLIST_ELEMENT,void*);
static bool compute_event_density_pct_and_average(pLIST_ELEMENT,void*);
static bool add_inbound_state_wrapper(pLIST_ELEMENT,void*);
#ifdef PARSER_DEBUG
static bool print_state_id_info(pLIST_ELEMENT,void*);
static bool print_transition_info(pLIST_ELEMENT,void*);
static bool print_full_action_info(pLIST_ELEMENT,void*);
static bool print_pid_name(pLIST_ELEMENT,void*);
static bool print_event_id_info(pLIST_ELEMENT,void*);
static bool print_sharing_sub_machine(pLIST_ELEMENT,void*);
#endif

/* the general use data */
char  *me = "I don't know who I am, but I'm";
char  *inputFileName = "";
bool  generate_instance                         = true;
bool  compact_action_array                      = false;
bool  short_dbg_names                           = false;
bool  force_generation_of_event_passing_actions = false;
bool  add_machine_name                          = false;
bool  generate_run_function                     = true;
bool  add_event_cross_reference                 = false;
pLIST pplantuml_prefix_strings_list             = NULL;
pLIST pplantuml_prefix_files_list               = NULL;
bool  output_generated_file_names_only          = false;
bool  output_header_files                       = false;
bool  output_make_recipe                        = false;
bool  short_user_fn_names                       = false;
char  *empty_cell_fn                            = NULL;

void print_tab_levels(FILE *output, unsigned levels)
{
   for (unsigned level = 0; level < levels; level++)
   {
      fprintf(output, "\t");
   }
}

int add_id(pLIST id_list, int type, char *name, pID_INFO *ppid_info)
{
   int ret_val;

  /* not already defined ? */
  if ((ret_val = lookup_id(id_list, name, ppid_info)) == LOOKUP)
  {
     /* allocate new entry and link to list */
     if ((*ppid_info = (pID_INFO) malloc(sizeof(ID_INFO))) == NULL) {

       printf("!!!! error : no memory to add name\n");
       return (0);

     }

     memset(*ppid_info,0,sizeof(ID_INFO));

     #ifdef MEM_DEBUG
     printf("Adding ID_INFO 0x%x\n",*ppid_info);
     #endif

     (*ppid_info)->name = strdup(name);
     (*ppid_info)->type = type;

     add_to_list(id_list,*ppid_info);

     #ifdef LEX_DEBUG
     printf("Added : %s of type %s\n",name,strings[type]);
     #endif

     ret_val = type;
  }
  #ifdef LEX_DEBUG
  else
  {
     printf("Found : %s of type %s\n",name,strings[type]);
  }
  #endif

  return (ret_val);

}

bool add_unique_id(pLIST id_list, int type, char *name, pID_INFO *ppid_info)
{
   bool ret_val = false;

  /* not already defined ? */
  if ((ret_val = lookup_id(id_list, name, ppid_info)) == LOOKUP)
  {
     /* allocate new entry and link to list */
     if ((*ppid_info = (pID_INFO) malloc(sizeof(ID_INFO))) == NULL) {

       printf("!!!! error : no memory to add name\n");
       return (0);

     }

     memset(*ppid_info,0,sizeof(ID_INFO));

     #ifdef MEM_DEBUG
     printf("Adding ID_INFO 0x%x\n",*ppid_info);
     #endif

     (*ppid_info)->name = strdup(name);
     (*ppid_info)->type = type;

     add_to_list(id_list,*ppid_info);

     #ifdef LEX_DEBUG
     printf("Added : %s of type %s\n",name,strings[type]);
     #endif

     ret_val = true;
  }
  #ifdef LEX_DEBUG
  else
  {
     printf("Error : %s of type %s already exists\n",name,strings[type]);
  }
  #endif

  return (ret_val);

}

static bool find_id_by_name(pLIST_ELEMENT pelem, void *data)
{
   pID_INFO pid   = (pID_INFO) pelem->mbr;
   char     *name = (char *) data;

   /*
   printf("looking for: %s\n",name);
   printf("\texamining %s\n", pid->name);
   */

   return !strcmp(pid->name, name);
}

int lookup_id(pLIST id_list, char *name, pID_INFO *ppid_info)
{

   pLIST_ELEMENT pelem = iterate_list(id_list,find_id_by_name,name);

   if (pelem)
   {
      *ppid_info = (pID_INFO)pelem->mbr;
      return ((pID_INFO)pelem->mbr)->type;
   }

   return LOOKUP;  /* i.e. not found */

}

void free_ids(pLIST id_list)
{
   (void) id_list;
}

void freeMachineInfo(pMACHINE_INFO pmi)
{

	unsigned							i;

	if (!pmi)

		return;

	/* and, the action array */
	if (pmi->actionArray) {

		for (i = 0; i < pmi->event_list->count; i++) {

			CHECK_AND_FREE(pmi->actionArray[i]);

		}

		FREE_AND_CLEAR(pmi->actionArray);

	}

	memset(pmi,0,sizeof(MACHINE_INFO));

}

FILE *openFile(char *fileName, char *mode)
{

  FILE *fp;

  if ((fp = fopen(fileName,mode)) == NULL) {

    fprintf(stderr,"%s : unable to open file %s\n",
      me , fileName);

  }

  return fp;

}

char *createFileName(char *base, char *ext)
{

	char	*cp;

	if (base) {

		if ((cp = (char *) malloc(strlen(base)+strlen(ext)+1)) != NULL) {

			strcpy(cp,base);
			strcat(cp,ext);

		}
		else {

	    fprintf(stderr,"%s : unable to create filename from %s and %s\n",
 	     me , base, ext);

		}


	}
	else {

		fprintf(stderr,"%s : why should I create a filename from nothing?\n",
			me);

	}

	return cp;

}

/**
	function : allocateActionArray

	Memory is allocated for the ActionArray, which must be freed
	when the machine info struct is freed.

	Arguments : 
		pMACHINE_INFO	-	The completed machine info struct.

	returns:

		0	on success,
		1 on failure.
*/
int allocateActionArray(pMACHINE_INFO pmi)
{

	unsigned i;

	if (!pmi || !pmi->state_list->count || !pmi->event_list->count)
		return 1;

	if (!(pmi->actionArray = (pACTION_INFO **)calloc(pmi->event_list->count,sizeof(pACTION_INFO *))))
		return 1;

	for (i = 0; i < pmi->event_list->count; i++)

		if (!(pmi->actionArray[i] = (pACTION_INFO *)calloc(pmi->state_list->count,sizeof(pACTION_INFO))))

			return 1;

	return 0;

}

static bool add_to_action_array(pLIST_ELEMENT pelem, void *data)
{
   pACTION_ARRAY_POPULATION_HELPER paaph      = (pACTION_ARRAY_POPULATION_HELPER) data;
   pID_INFO                        pstate     = (pID_INFO)      pelem->mbr;
   pSTATE_DATA                     psd        = &pstate->type_data.state_data;
   pEVENT_DATA                     ped        = &paaph->pevent->type_data.event_data;
   pID_INFO                        action     = paaph->pai->action;
   pID_INFO                        transition = paaph->pai->transition;


   #ifdef PARSER_DEBUG
   fprintf(paaph->fout, "\t\tadd_to_action_array: state: %s\n"
           , pstate->name
           );
   #endif

   if (paaph->pmi->actionArray[paaph->pevent->order][pstate->order])
   {
      fprintf(stderr
              ,"Machine %s: Won't insert action %s into slot: event %s, state %s because it is already occupied by %s\n"
              , paaph->pmi->name->name
              , action->name ? action->name : "transition"
              , paaph->pevent->name
              , pstate->name
              , paaph->pmi->actionArray[paaph->pevent->order][pstate->order]->action->name
			    ? paaph->pmi->actionArray[paaph->pevent->order][pstate->order]->action->name 
				: "transition"
              );

      paaph->error = true;
   }
   else
   {
      paaph->pmi->actionArray[paaph->pevent->order][pstate->order] = paaph->pai;

	  if (action && strlen(action->name))
	  {
		  add_unique_to_list(psd->pactions_list, action);
		  add_unique_to_list(ped->pactions_list, action);
	  }

	  if (ped->psingle_pai == paaph->pai)
      {
         ped->single_pai_state_count++;
      }

	  if (paaph->pai)
	  {
		  add_unique_to_list(psd->pevents_handled, paaph->pevent);
		  add_unique_to_list(ped->phandling_states
							 , pstate
							 );
	  }


	  if (transition)
	  {  
		  if (transition->type == STATE)
		  {
			  if (transition != pstate)
			  {
				  add_unique_to_list(transition->type_data.state_data.pinbound_transitions
									 , pstate
									 );

				  add_unique_to_list(psd->poutbound_transitions
									 , transition
									 );
			  }
		  }
		  else
		  {
			  copy_list_unique_with_exception(psd->poutbound_transitions
											  , transition->transition_fn_returns_decl
											  , pstate
											  );

			  if (transition->transition_fn_returns_decl)
			  {
				  iterate_list(transition->transition_fn_returns_decl
							   , add_inbound_state_wrapper
							   , pstate
							   );
			  }
		  }
	  }

   }

   return paaph->error;
}

static bool iterate_matrix_states(pLIST_ELEMENT pelem, void *data)
{
   pACTION_ARRAY_POPULATION_HELPER paaph = (pACTION_ARRAY_POPULATION_HELPER) data;
   
   paaph->pevent = (pID_INFO) pelem->mbr;
   pEVENT_DATA ped = &paaph->pevent->type_data.event_data;

   /* look for events which are handled identically in all states */
   paaph->pevent->type_data.event_data.single_pai_state_count = 0;
   if (!paaph->pevent->type_data.event_data.psingle_pai)
   {
      paaph->pevent->type_data.event_data.psingle_pai            = paaph->pai;
   }

   #ifdef PARSER_DEBUG
   fprintf(paaph->fout, "\titerate_matrix_states: event: %s\n", paaph->pevent->name);
   #endif

   paaph->error = false;
   iterate_list(paaph->pai->matrix->state_list,add_to_action_array,paaph);

   if (ped->single_pai_state_count == paaph->pmi->state_list->count)
   {
      ped->single_pai_for_all_states    = true;
      paaph->pmi->has_single_pai_events = true;
   }

   return paaph->error;
}

static bool process_action_info(pLIST_ELEMENT pelem, void *data)
{
   pACTION_ARRAY_POPULATION_HELPER paaph = (pACTION_ARRAY_POPULATION_HELPER) data;

   paaph->pai = (pACTION_INFO)pelem->mbr;

   #ifdef PARSER_DEBUG
   fprintf(paaph->fout
           , "process_action_info: %s\n"
           , strlen(paaph->pai->action->name) ? paaph->pai->action->name : "noAction"
           );
   #endif

   /* first, handle any (ALL) constructs.
    
      These cannot be resolved when parsed, because the parser
      cannot see the machine's state and event lists at that time.
  */
   if (!paaph->pai->matrix->event_list->count)
   {
      copy_list(paaph->pai->matrix->event_list, paaph->pmi->event_list);
   }
   if (!paaph->pai->matrix->state_list->count)
   {
      copy_list(paaph->pai->matrix->state_list, paaph->pmi->state_list);
   }

   /* then, fill in the action array */
   iterate_list(paaph->pai->matrix->event_list, iterate_matrix_states, paaph);

   return paaph->error;

}

bool populate_action_array(pMACHINE_INFO pmi, FILE *fout)
{
   ACTION_ARRAY_POPULATION_HELPER aaph;

   aaph.pmi   = pmi;
   aaph.fout  = fout;
   aaph.error = false;

   iterate_list(pmi->action_info_list,process_action_info,&aaph);

   #ifdef PARSER_DEBUG
   fprintf(fout,"populate_action_array returning %s\n", aaph.error ? "true" : "false");
   #endif

   return aaph.error;
}

/**
	function: eventNameByIndex

	looks up the event with the seOrder equal to the given index.
	The pointer returned points to a field in the ID_INFO struct and
	MUST NOT be freed.

	Arguments:

			pMACHINE_INFO pmi	- the machine with which we are concerned.
			int	index	-	The index we want.

	Returns

		a pointer to the event name on success, or
		NULL on error.
*/
char *eventNameByIndex(pMACHINE_INFO pmi, int index)
{

	char 			*cp = NULL;
	pID_INFO	pidi;

	if (pmi && index >= 0 && ((unsigned)index < pmi->event_list->count)) {

     pidi = (pID_INFO)find_nth_list_member(pmi->event_list,(unsigned)index);
     if (pidi)
     {
        cp = pidi->name;
     }

	}

	return cp;

}

/**
	function: eventPidByIndex

	looks up the event with the seOrder equal to the given index.
	The pointer returned points to the ID_INFO struct and
	MUST NOT be freed.

	Arguments:

			pMACHINE_INFO pmi	- the machine with which we are concerned.
			int	index	-	The index we want.

	Returns

		a pointer to the event pid on success, or
		NULL on error.
*/
pID_INFO eventPidByIndex(pMACHINE_INFO pmi, int index)
{

	pID_INFO	pidi = NULL;

	if (pmi && (index >= 0) ) {

     pidi = (pID_INFO) find_nth_list_member(pmi->event_list, (unsigned) index);
	}

	return pidi;

}

/**
	function: stateNameByIndex

	looks up the state with the seOrder equal to the given index.
	The pointer returned points to a field in the ID_INFO struct and
	MUST NOT be freed.

	Arguments:

			pMACHINE_INFO pmi	- the machine with which we are concerned.
			int	index	-	The index we want.

	Returns

		a pointer to the state name on success, or
		NULL on error.
*/
char *stateNameByIndex(pMACHINE_INFO pmi, int index)
{

	char 			*cp = NULL;
	pID_INFO	pidi;

	if (pmi && (index >= 0)) {

     pidi = (pID_INFO)find_nth_list_member(pmi->state_list,(unsigned) index);
     if (pidi)
     {
        cp = pidi->name;
     }

	}

	return cp;

}

/**
	function: statePidByIndex

	looks up the state with the seOrder equal to the given index.
	The pointer returned points to the ID_INFO struct and
	MUST NOT be freed.

	Arguments:

			pMACHINE_INFO pmi	- the machine with which we are concerned.
			int	index	-	The index we want.

	Returns

		a pointer to the state pid on success, or
		NULL on error.
*/
pID_INFO statePidByIndex(pMACHINE_INFO pmi, int index)
{

	pID_INFO	pidi = NULL;

	if (pmi && (index >= 0)) {
     pidi = (pID_INFO)find_nth_list_member(pmi->state_list,(unsigned) index);
	}

	return pidi;

}


/**
	function: transitionPidByIndex

* looks up the transition with the tOrder equal to the given 
* index. The pointer returned points to the ID_INFO struct and 
	MUST NOT be freed.

	Arguments:

			pMACHINE_INFO pmi	- the machine with which we are concerned.
			int	index	-	The index we want.

	Returns

		a pointer to the transition_fn pid on success, or
		NULL on error.
*/
pID_INFO transitionPidByIndex(pMACHINE_INFO pmi, int index)
{

	pID_INFO	pidi = NULL;

  if (pmi && (index >= 0) && ((unsigned)index < pmi->transition_list->count))
  {
     pidi = (pID_INFO)find_nth_list_member(pmi->transition_list,(unsigned)index);
	}

	return pidi;

}

/*
   require a pointer to a list element which has a pID_INFO as a member
      and a pointer to an unsigned integer.
 
   Assign the value of the unsigned integer to the order field of the ID_INFO, then
      increment the unsigned integer.
*/
static bool enumerate_pid(pLIST_ELEMENT pelem, void *data)
{
   ((pID_INFO)pelem->mbr)->order = (*((unsigned*)data))++;
   return false;
}

/**********************************************************************************************************************/
/**
 * @brief assign ordinal values to the members of a list which contains pID_INFO members
 * 
 * @author Steven Stanton (7/8/2022)
 * 
 * @param plist the list to sequentially number
 * 
 *
 * @ref_global none
 *
 * @mod_global none
 *
 * @thread_safe yes
 *
 * counting starts at 0.
 ***********************************************************************************************************************/
void enumerate_pid_list(pLIST plist)
{
   unsigned counter = 0;
   iterate_list(plist,enumerate_pid,&counter);
}

static bool count_external(pLIST_ELEMENT pelem, void *data)
{
   if (((pID_INFO)pelem->mbr)->type_data.event_data.externalDesignation)
   {
      (*((unsigned*)data))++;
   }
   return false;
}

static bool count_inhibitors(pLIST_ELEMENT pelem, void *data)
{
   if (((pID_INFO)pelem->mbr)->type_data.state_data.state_flags & sfInibitSubMachines)
   {
      (*((unsigned*)data))++;
   }
   return false;
}

static bool count_parent_event_refs(pLIST_ELEMENT pelem, void *data)
{
   if (((pID_INFO)pelem->mbr)->type_data.event_data.psharing_sub_machines)
   {
      (*((unsigned*)data))++;
   }
   return false;
}

static bool count_shared_evts(pLIST_ELEMENT pelem, void *data)
{
   if (((pID_INFO)pelem->mbr)->type_data.event_data.shared_with_parent)
   {
      (*((unsigned*)data))++;
   }
   return false;
}

static bool count_data_xlate(pLIST_ELEMENT pelem, void *data)
{
   pID_INFO pevent      = (pID_INFO)pelem->mbr;
   pITERATOR_HELPER pih = (pITERATOR_HELPER) data;

   if (pevent->type_data.event_data.puser_event_data)
   {
      if (pevent->type_data.event_data.puser_event_data->translator)
      {
         (*((unsigned*)pih->counter0))++;
      }
      if (pevent->type_data.event_data.puser_event_data->data_fields)
      {
         (*((unsigned*)pih->counter1))++;
      }
   }
   return false;
}

static bool count_entry_and_exit_handlers(pLIST_ELEMENT pelem, void *data)
{
   pID_INFO         pstate = (pID_INFO) pelem->mbr;
   pITERATOR_HELPER pih    = (pITERATOR_HELPER) data;

   if (pstate->type_data.state_data.state_flags & sfHasEntryFn)
   {
      (*pih->counter0)++;
   }

   if (pstate->type_data.state_data.state_flags & sfHasExitFn)
   {
      (*pih->counter1)++;
   }

   return false;

}

static bool count_zero_event_states(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO         pstate = (pID_INFO)   pelem->mbr;
	unsigned        *count  = (unsigned *) data;

	if (pstate->type_data.state_data.pevents_handled->count == 0)
	{
		(*count)++;
	}

	return false;
}

void count_states_with_zero_events(pLIST plist, unsigned *count)
{
	iterate_list(plist, count_zero_event_states, count);
}

static bool count_one_event_states(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO         pstate = (pID_INFO)   pelem->mbr;
	unsigned        *count  = (unsigned *) data;

	if (pstate->type_data.state_data.pevents_handled->count == 1)
	{
		(*count)++;
	}

	return false;
}

void count_states_with_one_event(pLIST plist, unsigned *count)
{
	iterate_list(plist, count_one_event_states, count);
}

static bool count_zero_event_handlers(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO         pevent = (pID_INFO)   pelem->mbr;
	unsigned        *count  = (unsigned *) data;

	if (pevent->type_data.event_data.phandling_states->count == 0)
	{
		(*count)++;
	}

	return false;
}

void count_events_with_zero_handlers(pLIST plist, unsigned *count)
{
	iterate_list(plist, count_zero_event_handlers, count);
}

static bool count_one_event_handlers(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO         pevent = (pID_INFO)   pelem->mbr;
	unsigned        *count  = (unsigned *) data;

	if (pevent->type_data.event_data.phandling_states->count == 1)
	{
		(*count)++;
	}

	return false;
}

void count_events_with_one_handler(pLIST plist, unsigned *count)
{
	iterate_list(plist, count_one_event_handlers, count);
}

static bool count_no_way_in_states(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO         pstate = (pID_INFO)   pelem->mbr;
	unsigned        *count  = (unsigned *) data;

	/* the first state has a way in, by default */
	if (pelem->ordinal)
	{
		if (pstate->type_data.state_data.pinbound_transitions->count == 0)
		{
			(*count)++;
		}
	}

	return false;
}

void count_states_with_no_way_in(pLIST plist, unsigned *data)
{
	iterate_list(plist, count_no_way_in_states, data);
}

static bool count_no_way_out_states(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO         pstate = (pID_INFO)   pelem->mbr;
	unsigned        *count  = (unsigned *) data;

	if (pstate->type_data.state_data.poutbound_transitions->count == 0)
	{
		(*count)++;
	}

	return false;
}

void count_states_with_no_way_out(pLIST plist, unsigned *data)
{
	iterate_list(plist, count_no_way_out_states, data);
}

void count_external_declarations(pLIST plist, unsigned *counter)
{
   iterate_list(plist, count_external, counter);
}

void count_sub_machine_inhibitors(pLIST plist, unsigned *counter)
{
   iterate_list(plist, count_inhibitors, counter);
}

void count_parent_event_referenced(pLIST plist, unsigned *counter)
{
   iterate_list(plist, count_parent_event_refs, counter);
}

void count_shared_events(pLIST plist, unsigned *counter)
{
   iterate_list(plist, count_shared_evts, counter);
}

void count_event_user_data_attributes(pLIST plist, unsigned *translators, unsigned *blocks)
{
   ITERATOR_HELPER ih;

   ih.counter0 = translators;
   ih.counter1 = blocks;
   iterate_list(plist, count_data_xlate, &ih);
}

void count_states_with_entry_exit_fns(pLIST pstate_list, unsigned *entry_counter, unsigned *exit_counter)
{
   ITERATOR_HELPER ih;

   *entry_counter = *exit_counter = 0;

   ih.counter0 = entry_counter;
   ih.counter1 = exit_counter;
   
   iterate_list(pstate_list, count_entry_and_exit_handlers, &ih);
}

/**
	return the length of the given string, or 0, if NULL is passed.
*/
int safe_strlen(const char *s)
{
	return (s ? strlen(s) : 0);
}

/**
	returns a copy of the file portion of path

	memory is allocated.
*/
char *getFileNameNoDir(const char *path)
{
  char *cp1 = NULL;
  const char *cp;

  if (path)
  {
	  for (cp = path; (cp1 = strpbrk(cp, "\\/")); cp = ++cp1)
			;

		if ((cp1 = malloc(safe_strlen(cp)+1)) != NULL)
			strcpy(cp1,cp);
  }

  return cp1;

}

/**********************************************************************************************************************/
/**
 * @brief write a machine
 * 
 * @author Steven Stanton (7/10/2022)
 * 
 * @param pelem pointer to a list element; the member is expected to be a pMACHINE_INFO.
 * @param data expected to be a pFSMOutputGenerator.
 * 
 *
 * @ref_global None
 *
 * @mod_global None
 *
 * @thread_safe Yes
 * 
 * @return bool always returns false, indicating that list processing should continue
 *
 * Require a list member pointing to a valid MACHINE_INFO struct, and data pointing to a valid FSMOutputGeneratorFactoryStr.
 * Call the generator's writeMachine function, passing the pointer to the MACHINE_INFO struct.
 ***********************************************************************************************************************/
static bool write_machine(pLIST_ELEMENT pelem, void *data)
{
   pFSMOutputGeneratorFactoryStr pfsmogf = (pFSMOutputGeneratorFactoryStr) data;
   pMACHINE_INFO                 pmi     = (pMACHINE_INFO) pelem->mbr;

   pFSMOutputGenerator pfsmog = pfsmogf->fsmogf(pfsmogf->parent_fsmog);
   if (pfsmog)
   {
	   pfsmog->initOutput(pfsmog, pmi->name->name);
	   pfsmog->writeMachine(pfsmog, pmi);
	   pfsmog->closeOutput(pfsmog,1);

	   free(pfsmog);
   }

   return false;
}

/**********************************************************************************************************************/
/**
 * @brief write each member of the list using the given pFSMOutputGenerator.
 * 
 * @author Steven Stanton (7/10/2022)
 * 
 * @param plist list of pMACHINE_INFOs
 * @param pfsmog ouput generator to use
 * 
 *
 * @ref_global None
 *
 * @mod_global None
 *
 * @thread_safe Yes
 *
 * Require a valid list of pointers to valid MACHINE_INFO structure and a pointer to a valid FSMOutputGenerator.
 * Use the output generator to write each member of the list.
 ***********************************************************************************************************************/
void write_machines(pLIST plist, fpFSMOutputGeneratorFactory fpfsmogg, pFSMOutputGenerator parent_fsmog)
{
   FSMOutputGeneratorFactoryStr fsmogg;

   fsmogg.fsmogf       = fpfsmogg;
   fsmogg.parent_fsmog = parent_fsmog;

   iterate_list(plist, write_machine, &fsmogg);
}

bool print_machine_component(pLIST_ELEMENT pelem, void *data)
{
   pID_INFO pid         = (pID_INFO) pelem->mbr;
   pITERATOR_HELPER pih = (pITERATOR_HELPER) data;

   fprintf(pih->fout
           , "%s%s_%s\n"
           , pih->first ? "" : ","
           , pih->pmi->name->name
           , pid->name
           );

   return false;
}

bool print_sub_machine_event(pLIST_ELEMENT pelem, void *data)
{
   pID_INFO pid         = (pID_INFO) pelem->mbr;
   pITERATOR_HELPER pih = (pITERATOR_HELPER) data;

   fprintf(pih->fout, "\t, ");

   if (pelem->ordinal == 0)
   {
       printNameWithAncestry("firstEvent"
                             , pih->pmi
                             , pih->fout
                             , "_"
                             , alc_lower
                             , ai_include_self
                             );
       fprintf(pih->fout, "\n\t, ");
       printNameWithAncestry(pid->name
                             ,pih->pmi
                             , pih->fout
                             , "_"
                             , alc_lower
                             , ai_include_self
                             );
       fprintf(pih->fout, " = ");
       printNameWithAncestry("firstEvent"
                             , pih->pmi
                             , pih->fout
                             , "_"
                             , alc_lower
                             , ai_include_self
                             );
   }
   else
   {
       printNameWithAncestry(pid->name
                             ,pih->pmi
                             , pih->fout
                             , "_"
                             , alc_lower
                             , ai_include_self
                             );
   }

   fprintf(pih->fout, "\n");

   return false;
}

bool print_sub_machine_component_name(pLIST_ELEMENT pelem, void *data)
{
   pID_INFO pid         = (pID_INFO) pelem->mbr;
   pITERATOR_HELPER pih = (pITERATOR_HELPER) data;

   if(short_dbg_names)
   {
	   fprintf(pih->fout
			   , "\t, \"%s\"\n"
			   , pid->name
			   );
   }
   else
   {
	   fprintf(pih->fout, "\t, \"");
	   printAncestry(pih->pmi, pih->fout, "_", alc_lower, ai_include_self);
	   fprintf(pih->fout
			   , "_%s\"\n"
			   , pid->name
			   );
   }

   return false;
}

/**
 * prints the events for the current event list, followed by
 * events for any sub-machines.
 * 
 * @author sstan (12/9/2023)
 * 
 * @param pelem  this must have a mbr of pMACHINE_INFO type
 * @param data   this must point to an ITERATOR_HELPER structure
 * 
 * @return bool always "false"
 */
bool print_sub_machine_events(pLIST_ELEMENT pelem, void *data)
{
   pITERATOR_HELPER pih = (pITERATOR_HELPER) data;

   pih->pmi   = (pMACHINE_INFO) pelem->mbr;
   pih->first = false;

   iterate_list(pih->pmi->event_list,print_sub_machine_event,pih);

   fprintf(pih->fout, "\t, ");
   printNameWithAncestry("noEvent"
                         , pih->pmi
                         , pih->fout
                         , "_"
                         , alc_lower
                         , ai_include_self
                         );

   fprintf(pih->fout, "\n");

   if (pih->pmi->machine_list)
   {
	   iterate_list(pih->pmi->machine_list
                    , print_sub_machine_events
                    , pih
                    );

       pih->pmi   = (pMACHINE_INFO) pelem->mbr;
       fprintf(pih->fout , "\t, ");
       printNameWithAncestry("lastEvent"
                             , pih->pmi
                             , pih->fout
                             , "_"
                             , alc_lower
                             , ai_include_self
                             );
       fprintf(pih->fout," = ");
       printNameWithAncestry("noEvent"
                             , (pMACHINE_INFO)LAST_LIST_MEMBER(pih->pmi->machine_list)
                             , pih->fout
                             , "_"
                             , alc_lower
                             , ai_include_self
                             );
       fprintf(pih->fout, "\n");

   }

   return false;
}

bool print_sub_machine_event_names(pLIST_ELEMENT pelem, void *data)
{
   pITERATOR_HELPER pih = (pITERATOR_HELPER) data;

   pih->pmi   = (pMACHINE_INFO) pelem->mbr;
   pih->first = false;

   iterate_list(pih->pmi->event_list, print_sub_machine_component_name, pih);

   if(!short_dbg_names)
   {
      fprintf(pih->fout, "\t, \"");
	  printAncestry(pih->pmi, pih->fout, "_", alc_lower, ai_include_self);
	  fprintf(pih->fout, "_noEvent\"\n");
   }
   else
   {
      fprintf(pih->fout
            , "\t, \"noEvent\"\n");
   }

   if (pih->pmi->machine_list)
   {
	   iterate_list(pih->pmi->machine_list, print_sub_machine_event_names, pih);
   }


   return false;
}

/**
 * Opens the file indicated by src and copies the contents into 
 * the file indicated by dest (which must be open).
 */
int copyFileContents(const FILE *fDest, const char *src)
{
    FILE *fSrc;
    char buf[256];
    int numBytes, retVal;

    if (NULL == (fSrc = openFile((char*)src,"r")))
    {
        return 1;
    }

    while (!feof(fSrc) && !ferror(fSrc) && !ferror((FILE*)fDest))
    {
        if (0 < (numBytes = fread(buf,sizeof(char), sizeof(buf), fSrc)))
        {
            numBytes = fwrite(buf, sizeof(char), numBytes, (FILE*)fDest);
        }
    }

    retVal = ferror(fSrc) + ferror((FILE*)fDest);

    fclose(fSrc);

	return retVal;

}

void streamStrCaseAware(FILE *fout, char *str, ANCESTRY_LETTER_CASE alc)
{
	if (alc == alc_upper)
	{
		streamHungarianToUnderbarCaps(fout, str);
	}
	else
	{
		fputs(str, fout);
	}
}

static bool recursePrintAncestry(pMACHINE_INFO pmi, FILE *fout, char *separator, ANCESTRY_LETTER_CASE alc, ANCESTRY_INCLUSION ai)
{
   bool something_was_printed = false;

   if (pmi->parent
	   && !(ai & ai_stop_at_parent)
	   )
   {
	   if ((something_was_printed = recursePrintAncestry(pmi->parent, fout, separator, alc, ai))) 
		   fputs(separator, fout);
	   streamStrCaseAware(fout, pmi->name->name, alc);
   }
   else
   {
	   if (!(ai & ai_omit_ultimate))
	   {
		   streamStrCaseAware(fout, pmi->name->name, alc);
		   something_was_printed = true;
	   }
   }

   return something_was_printed;

}

bool printAncestry(pMACHINE_INFO pmi, FILE *fout, char *separator, ANCESTRY_LETTER_CASE alc, ANCESTRY_INCLUSION ai)
{
	bool something_was_printed = false;

	if (pmi->parent)
	{
		something_was_printed = recursePrintAncestry(pmi->parent,fout,separator,alc, ai);
	}

	if (ai & ai_include_self)
	{
		if (something_was_printed)
		{
			fputs(separator, fout);
		}

		streamStrCaseAware(fout, pmi->name->name, alc);
		something_was_printed = true;
	}

	return something_was_printed;
}

void printNameWithAncestry(char *name, pMACHINE_INFO pmi, FILE *fout, char *separator, ANCESTRY_LETTER_CASE alc, ANCESTRY_INCLUSION ai)
{
	printAncestry(pmi, fout, separator, alc, ai);

	fputs(separator, fout);

	if (alc == alc_upper)
	{
		streamHungarianToUnderbarCaps(fout, name);
	}
	else
	{
		fputs(name, fout);
	}
}

pMACHINE_INFO ultimateAncestor(pMACHINE_INFO pmi)
{
	return !pmi->parent ? pmi : ultimateAncestor(pmi->parent);
}

/**
 * Get the maximum sub-machine depth, which is the sub-machine
 * depth of the top-level machine.
 * 
 * @author Steven Stanton (1/23/2024)
 * 
 * @param pmi    Pointer to machine info object
 * 
 * @return unsigned The sub-machine depth of the top-level
 *  	   machine.
 */
unsigned maxDepth(pMACHINE_INFO pmi)
{
	return ultimateAncestor(pmi)->sub_machine_depth;
}

/**
 * prints a data field
 * 
 * @author Steven Stanton (12/9/2023)
 * 
 * @param pelem  must have a mbr of type pDATA_FIELD
 * @param data   must point to an ITERATOR_HELPER struct
 * 
 * @return bool always "false."
 */
bool print_data_field(pLIST_ELEMENT pelem, void *data)
{
   pITERATOR_HELPER pih       = (pITERATOR_HELPER) data;
   pDATA_FIELD pdf            = (pDATA_FIELD)      pelem->mbr;

   print_tab_levels(pih->fout, pih->tab_level);
   switch (pdf->pdts->dtt)
   {
   case dtt_simple:
      fprintf(pih->fout, "%s", pdf->pdts->dtu.name->name);
      break;
   case dtt_struct:
      pih->tab_level++;
      fprintf(pih->fout , "struct {\n" );
      iterate_list(pdf->pdts->dtu.members, print_data_field, pih);
      pih->tab_level--;
      print_tab_levels(pih->fout, pih->tab_level);
      fprintf(pih->fout, "}");
      break;
   case dtt_union:
      pih->tab_level++;
      fprintf(pih->fout , "union {\n" );
      iterate_list(pdf->pdts->dtu.members, print_data_field, pih);
      pih->tab_level--;
      print_tab_levels(pih->fout, pih->tab_level);
      fprintf(pih->fout, "}");
      break;
   }

   for (unsigned level = 0; level < pdf->pdts->indirection_level; level++)
   {
       fprintf(pih->fout
               , "%s*"
               , level ? "" : " "
               );
   }
   fprintf(pih->fout, " %s", pdf->data_field_name->name);

   if (pdf->pdts->is_array)
   {
       fprintf(pih->fout
               , "[%s]"
               , pdf->pdts->dimension ? pdf->pdts->dimension : ""
               );
   }

   fprintf(pih->fout, ";\n");

   return false;
}

void increase_sub_machine_depth(pMACHINE_INFO pmi)
{
	if (pmi)
	{
		pmi->sub_machine_depth++;
		increase_sub_machine_depth(pmi->parent);
	}
}

void compute_event_and_state_density_pct(pMACHINE_INFO pmi)
{
	iterate_list(pmi->state_list, compute_state_density_pct_and_average, pmi);
	iterate_list(pmi->event_list, compute_event_density_pct_and_average, pmi);

	pmi->average_state_event_density_pct /= pmi->state_list->count;
	pmi->average_event_state_density_pct /= pmi->event_list->count;
}

static bool compute_state_density_pct_and_average(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO      pstate = (pID_INFO)pelem->mbr;
	pMACHINE_INFO pmi    = (pMACHINE_INFO) data;
	pSTATE_DATA   psd    = &pstate->type_data.state_data;

	/* Integral math. */
	psd->event_density_pct = 100 * psd->pevents_handled->count;
	
	/* Now, do the division for this state. */
	psd->event_density_pct /= pmi->event_list->count;

	/* Accumulate into machine record. */
	pmi->average_state_event_density_pct += psd->event_density_pct;

	return false;
}

static bool compute_event_density_pct_and_average(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO      pevent = (pID_INFO)pelem->mbr;
	pMACHINE_INFO pmi    = (pMACHINE_INFO) data;
	pEVENT_DATA   ped    = &pevent->type_data.event_data;

	/* Integral math. */
	ped->state_density_pct = 100 * ped->phandling_states->count;
	
	/* Now, do the division for this event. */
	ped->state_density_pct /= pmi->state_list->count;

	/* Accumulate into machine record. */
	pmi->average_event_state_density_pct += ped->state_density_pct;

	return false;
}

static bool add_inbound_state_wrapper(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO ptransition_state = (pID_INFO) pelem->mbr;
	pID_INFO state_to_add      = (pID_INFO) data;

	if (state_to_add != ptransition_state)
	{
		add_unique_to_list(ptransition_state->type_data.state_data.pinbound_transitions
						   , state_to_add
						  );
	}

	return false;
}

/**
 * Creates a string comprising the contents of the provided
 * file.  The file is closed after reading.  Memory is
 * allocated, which must be freed by the caller.
 * 
 * @author Steven Stanton (2/10/2024)
 * 
 * @param file   FILE* pointer to the file containing the
 *  			 desrired string contents
 * 
 * @return char* pointer to the created string.
 */
char *create_string_from_file(FILE *file, unsigned long *str_len)
{
	char *cp = NULL;

	if (file)
	{
		fseek(file, 0, SEEK_END);
		const unsigned long file_size = ftell(file);

		fseek(file, 0, SEEK_SET);

		if ((cp = (char *) malloc(file_size + 1)) != NULL)
		{
			fread(cp, 1, file_size, file);
			cp[file_size] = 0;
		}

		fclose(file);

		if (str_len)
		{
			*str_len = file_size;
		}
	}

	return cp;
}

/**
 * Allow compacting only when actions return events.  Set the
 * compacting request flag to false so that code generation will
 * proceed without compacting.
 * 
 * @author Steven Stanton (2/2/2025)
 * 
 * @param pmi    
 * 
 * @return bool	true iff compact structures are requested and
 *  	   allowed.
 */
bool compacting(pMACHINE_INFO pmi)
{
	if (compact_action_array && (pmi->modFlags & ACTIONS_RETURN_FLAGS))
	{
		fprintf(stderr
				, "Warning: Ignoring compact array request because actions do not return events.\n"
				);
		compact_action_array = false;
	}

	return compact_action_array
		   && !(pmi->modFlags & ACTIONS_RETURN_FLAGS)
	       ;
}

#ifdef PARSER_DEBUG

typedef struct _debug_list_helper_ DEBUG_LIST_HELPER, *pDEBUG_LIST_HELPER;
struct _debug_list_helper_
{
   FILE          *fout;
   unsigned      counter;
   char          *nullName;
   char          *indenture;
   pMACHINE_INFO pmi;
};

static bool print_state_id_info(pLIST_ELEMENT pelem, void *data)
{
   pDEBUG_LIST_HELPER phelper = (pDEBUG_LIST_HELPER) data;
   pID_INFO pid               = (pID_INFO) pelem->mbr;

   fprintf(phelper->fout
       ,"\t%d:\t%s"
       ,phelper->counter++
       ,pid->name
       );

   fprintf(phelper->fout
       ,"\n%s\n"
       ,pid->docCmnt ? pid->docCmnt : ""
       );

   return false;
}

static bool print_sharing_sub_machine(pLIST_ELEMENT pelem, void *data)
{
   pDEBUG_LIST_HELPER phelper = (pDEBUG_LIST_HELPER) data;
   pMACHINE_INFO      pmi     = (pMACHINE_INFO) pelem->mbr;

   fprintf(phelper->fout
           , "\t\t\t%s\n"
           , pmi->name->name
           );

   return false;
}

static bool print_event_id_info(pLIST_ELEMENT pelem, void *data)
{
   pDEBUG_LIST_HELPER phelper = (pDEBUG_LIST_HELPER) data;
   pID_INFO pid               = (pID_INFO) pelem->mbr;

   fprintf(phelper->fout
       ,"\t%d:\t%s"
       ,phelper->counter++
       ,pid->name
       );

   if (pid->type_data.event_data.externalDesignation)
   {
      fprintf(phelper->fout," = %s", pid->type_data.event_data.externalDesignation->name);
   }

   if (pid->type_data.event_data.psharing_sub_machines)
   {
      fprintf(phelper->fout
              ,"; This event is shared by %d sub-machine%s:\n"
              , pid->type_data.event_data.psharing_sub_machines->count
              , pid->type_data.event_data.psharing_sub_machines->count != 1 ? "s" : ""
              );

      iterate_list(pid->type_data.event_data.psharing_sub_machines
                   , print_sharing_sub_machine
                   , phelper
                   );
   }

   fprintf(phelper->fout
       ,"\n%s\n"
       ,pid->docCmnt ? pid->docCmnt : ""
       );

   return false;
}

void parser_debug_print_event_list(pLIST plist, FILE *file)
{
   DEBUG_LIST_HELPER helper;

   helper.fout    = file;
   helper.counter = 0;

   iterate_list(plist,print_event_id_info,&helper);
}

void parser_debug_print_state_list(pLIST plist, FILE *file)
{
   DEBUG_LIST_HELPER helper;

   helper.fout    = file;
   helper.counter = 0;

   iterate_list(plist,print_state_id_info,&helper);
}

static bool print_pid_name(pLIST_ELEMENT pelem, void *data)
{
   pDEBUG_LIST_HELPER phelper = (pDEBUG_LIST_HELPER) data;
   pID_INFO           pid     = (pID_INFO) pelem->mbr;

   char *name = strlen(pid->name)
                   ? pid->name 
                   : phelper->nullName;

   fprintf(phelper->fout
           , "\t%s%s%s\n"
           , phelper->indenture ? phelper->indenture : ""
           , (pid->powningMachine 
              && phelper->pmi
              && (pid->powningMachine != phelper->pmi)
              )
               ? "namespace::"
               : ""
           , name
           );

   return false;
}

/**********************************************************************************************************************/
/**
 * @brief prints the names of the ID_INFOs comprising the given list
 * 
 * @author Steven Stanton (7/9/2022)
 * 
 * @param plist Must point to a list whose members are pID_INFOs
 * @param file the output filename
 * @param nullName A string to use when the name field of the ID_INFO struct is null
 * 
 *
 * @ref_global None
 *
 * @mod_global None
 *
 * @thread_safe Yes
 *
 * all input parameters must be valid.  The format used is simply "%s\n".
 ***********************************************************************************************************************/
void parser_debug_print_id_list_names(pLIST plist
                                      , pMACHINE_INFO pmi
                                      , FILE *file
                                      , char *nullName)
{
   DEBUG_LIST_HELPER helper = {0};

   helper.fout     = file;
   helper.nullName = nullName;
   helper.pmi      = pmi;

   iterate_list(plist,print_pid_name,&helper);
}

static bool print_full_action_info(pLIST_ELEMENT pelem, void *data)
{
   pID_INFO pid               = (pID_INFO)pelem->mbr;
   pDEBUG_LIST_HELPER phelper = (pDEBUG_LIST_HELPER) data;

   pACTION_INFO	    pai;

   fprintf(phelper->fout
       ,"\t%s\n"
       ,strlen(pid->name) ? pid->name : "noAction"
       );

   phelper->indenture = "\t\t\t";
   for (pai = pid->type_data.action_data.actionInfo;
         pai;
         pai = pai->nextAction) {

     fprintf(phelper->fout,"\t\t%swhich occurs in these events\n"
             , pai == pid->type_data.action_data.actionInfo ? "" : "and "
             );
     phelper->nullName = "noEvent";
     iterate_list(pai->matrix->event_list, print_pid_name, phelper);

     fprintf(phelper->fout,"\t\tand states\n");
     phelper->nullName = "noState";
     iterate_list(pai->matrix->state_list, print_pid_name, phelper);

     if (pai->transition)
    {
        switch (pai->transition->type)
        {
            case STATE:
                fprintf(phelper->fout,"\t\tand transitions to state %s\n"
               ,pai->transition->name);
               break;
            case TRANSITION_FN:
                fprintf(phelper->fout,"\t\tand transitions using function %s\n"
               ,pai->transition->name);
               break;
        }
    }

   }

   if (pid->type_data.action_data.action_returns_decl)
  {
    fprintf(phelper->fout,"\t\tand which returns\n");
    phelper->indenture = "\t\t\t";
    iterate_list(pid->type_data.action_data.action_returns_decl, print_pid_name, phelper);
  }

   if (pid->docCmnt)
     fprintf(phelper->fout,"Doc Comments:\n%s\n"
           ,pid->docCmnt);

   return false;
}

void parser_debug_print_action_list_deep(pLIST plist, pMACHINE_INFO pmi, FILE *file)
{
   DEBUG_LIST_HELPER helper = {0};

   helper.fout = file;
   helper.pmi  = pmi;

   iterate_list(plist,print_full_action_info,&helper);
}

void parser_debug_print_transition_list(pLIST plist, FILE *file)
{
   DEBUG_LIST_HELPER helper = {0};

   helper.fout = file;

   iterate_list(plist,print_pid_name,&helper);
}

static bool print_transition_info(pLIST_ELEMENT pelem, void *data)
{
   pID_INFO pid               = (pID_INFO)pelem->mbr;
   pDEBUG_LIST_HELPER phelper = (pDEBUG_LIST_HELPER) data;

   fprintf(phelper->fout,"\t%s\n"
           , pid->name
           );

    if (pid->transition_fn_returns_decl)
   {
       fprintf(phelper->fout,"\t\twhich returns\n");
       phelper->indenture = "\t\t\t";
       phelper->nullName  = "";
       iterate_list(pid->transition_fn_returns_decl, print_pid_name, phelper);
   }

    return false;

}

void parser_debug_print_transition_fn_list(pLIST plist, FILE *file)
{
   DEBUG_LIST_HELPER helper = {0};

   helper.fout = file;

   iterate_list(plist,print_transition_info,&helper);
}

void parser_debug_print_data_block(pLIST plist, FILE *file)
{
   ITERATOR_HELPER ih;

   ih.fout      = file;
   ih.tab_level = 0;

   iterate_list(plist,print_data_field,&ih);

   fprintf(file, "\n");
}
#endif

