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
   bool          error;
};

/* the general use data */
char	*me = "I don't know who I am, but I'm";
bool generate_instance    = true;
bool compact_action_array = false;

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

	int							i;

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
	This function will take a string in hungarian notation
		and change it into "capitalized underbar" notation.
		For example, "newMachine" becomes "NEW_MACHINE", but
		testFSM will become TEST_FSM

	Moreover, escape characters are replaced by underbars.

	Memory is allocated, which must be freed by the calling 
		program.

	returns :
		pointer to the string on success,
		NULL									on failure.

*/
char *hungarianToUnderbarCaps(char *str)
{

	int 	i,consecutive;
	char	*cp, *cp1, *cp2;

	/* first, use i and cp1 to figure out how much memory to get */
	i = strlen(str);
	i++;

	for (cp = str; *cp; cp++)

		if (!(*cp & 0x20))

			i++;

	/* now, cp becomes the pointer to the new memory */
	if ((cp = (char *)malloc(i))) {

		consecutive = 0;
		for (cp1 = str, cp2 = cp; *cp1; cp1++) {

			//deal with the escapes first
			if (*cp1 == '\\') {
				*cp2++ = '_';
				continue;
			}

			if (!(*cp1 & 0x20)) {

				if (!consecutive) {

					*cp2++ = '_';

					consecutive = 1;

				}

			}
			else

				consecutive = 0;

			*cp2++ = isalpha(*cp1) ? toupper(*cp1) : *cp1;

		}

		*cp2 = 0;

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

	int i;

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
   pACTION_ARRAY_POPULATION_HELPER paaph  = (pACTION_ARRAY_POPULATION_HELPER) data;
   pID_INFO                        pstate = (pID_INFO)      pelem->mbr;

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
              , paaph->pai->action->name
              , paaph->pevent->name
              , pstate->name
              , paaph->pmi->actionArray[paaph->pevent->order][pstate->order]->action->name
              );

      paaph->error = true;
   }
   else
   {
      paaph->pmi->actionArray[paaph->pevent->order][pstate->order] = paaph->pai;
   }

   return paaph->error;
}

static bool iterate_matrix_states(pLIST_ELEMENT pelem, void *data)
{
   pACTION_ARRAY_POPULATION_HELPER paaph = (pACTION_ARRAY_POPULATION_HELPER) data;
   
   paaph->pevent = (pID_INFO) pelem->mbr;

   #ifdef PARSER_DEBUG
   fprintf(paaph->fout, "\titerate_matrix_states: event: %s\n", paaph->pevent->name);
   #endif

   iterate_list(paaph->pai->matrix->state_list,add_to_action_array,paaph);

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

   iterate_list(paaph->pai->matrix->event_list,iterate_matrix_states,paaph);

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

	if (pmi && (index >= 0) && (index < pmi->event_list->count)) {

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

  if (pmi && (index >= 0) && (index < pmi->transition_list->count))
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
   if (((pID_INFO)pelem->mbr)->externalDesignation)
   {
      (*((unsigned*)data))++;
   }
   return false;
}
void count_external_declarations(pLIST plist, unsigned *counter)
{
   iterate_list(plist, count_external, counter);
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
	const char *cp;
  char *cp1;

	for(cp = path; cp1 = strpbrk(cp,"\\/"); cp = ++cp1)
		;

	if ((cp1 = malloc(safe_strlen(cp)+1)) != NULL)
		strcpy(cp1,cp);

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
 * Require a list member pointing to a valid MACHINE_INFO struct, and data pointing to a valid FSMOutputGenerator struct.
 * Call the generator's writeMachine function, passing the pointer to the MACHINE_INFO struct.
 ***********************************************************************************************************************/
static bool write_machine(pLIST_ELEMENT pelem, void *data)
{
   pFSMOutputGenerator pfsmog = (pFSMOutputGenerator) data;
   pMACHINE_INFO       pmi    = (pMACHINE_INFO) pelem->mbr;

   (*pfsmog->initOutput)(pfsmog, pmi->name->name);
   (*pfsmog->writeMachine)(pfsmog, pmi);
   (*pfsmog->closeOutput)(pfsmog,1);

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
void write_machines(pLIST plist, pFSMOutputGenerator pfsmog)
{
   iterate_list(plist, write_machine, pfsmog);
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

bool print_sub_machine_component(pLIST_ELEMENT pelem, void *data)
{
   pID_INFO pid         = (pID_INFO) pelem->mbr;
   pITERATOR_HELPER pih = (pITERATOR_HELPER) data;

   fprintf(pih->fout
           , "\t%s%s_%s_%s\n"
           , pih->first ? "" : ","
           , pih->pparent->name->name
           , pih->pmi->name->name
           , pid->name
           );

   return false;
}

bool print_sub_machine_component_name(pLIST_ELEMENT pelem, void *data)
{
   pID_INFO pid         = (pID_INFO) pelem->mbr;
   pITERATOR_HELPER pih = (pITERATOR_HELPER) data;

   fprintf(pih->fout
           , "\t, \"%s_%s_%s\"\n"
           , pih->pparent->name->name
           , pih->pmi->name->name
           , pid->name
           );

   return false;
}

bool print_sub_machine_events(pLIST_ELEMENT pelem, void *data)
{
   pITERATOR_HELPER pih = (pITERATOR_HELPER) data;

   pih->pmi   = (pMACHINE_INFO) pelem->mbr;
   pih->first = false;

   iterate_list(pih->pmi->event_list,print_sub_machine_component,pih);

   return false;
}

bool print_sub_machine_event_names(pLIST_ELEMENT pelem, void *data)
{
   pITERATOR_HELPER pih = (pITERATOR_HELPER) data;

   pih->pmi   = (pMACHINE_INFO) pelem->mbr;
   pih->first = false;

   iterate_list(pih->pmi->event_list,print_sub_machine_component_name,pih);

   return false;
}

#ifdef PARSER_DEBUG

typedef struct _debug_list_helper_ DEBUG_LIST_HELPER, *pDEBUG_LIST_HELPER;
struct _debug_list_helper_
{
   FILE     *fout;
   unsigned counter;
   char     *nullName;
   char     *indenture;
};

static bool print_state_or_event_id_info(pLIST_ELEMENT pelem, void *data)
{
   pDEBUG_LIST_HELPER phelper = (pDEBUG_LIST_HELPER) data;
   pID_INFO pid               = (pID_INFO) pelem->mbr;

   fprintf(phelper->fout
       ,"\t%d:\t%s"
       ,phelper->counter++
       ,pid->name
       );

   if (pid->externalDesignation)
   {
      fprintf(phelper->fout," = %s", pid->externalDesignation->name);
   }

   fprintf(phelper->fout
       ,"\n%s\n"
       ,pid->docCmnt ? pid->docCmnt : ""
       );

   return false;  //do not quit
}

void parser_debug_print_state_or_event_list(pLIST plist,FILE *file)
{
   DEBUG_LIST_HELPER helper;

   helper.fout = file;
   helper.counter = 0;

   iterate_list(plist,print_state_or_event_id_info,&helper);
}

static bool print_pid_name(pLIST_ELEMENT pelem, void *data)
{
   pDEBUG_LIST_HELPER phelper = (pDEBUG_LIST_HELPER) data;

   char *name = strlen(((pID_INFO)pelem->mbr)->name)
                   ? ((pID_INFO)pelem->mbr)->name 
                   : phelper->nullName;

   fprintf(phelper->fout,"\t%s%s\n"
           , phelper->indenture ? phelper->indenture : ""
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
void parser_debug_print_id_list_names(pLIST plist, FILE *file, char *nullName)
{
   DEBUG_LIST_HELPER helper = {0};

   helper.fout = file;
   helper.nullName = nullName;

   iterate_list(plist,print_pid_name,&helper);
}

static bool print_full_action_info(pLIST_ELEMENT pelem, void *data)
{
   pID_INFO pid               = (pID_INFO)pelem->mbr;
   pDEBUG_LIST_HELPER phelper = (pDEBUG_LIST_HELPER) data;

   pACTION_INFO	    pai;
   pACTION_SE_INFO	pase_info;

   fprintf(phelper->fout
       ,"\t%s\n"
       ,strlen(pid->name) ? pid->name : "noAction"
       );

   phelper->indenture = "\t\t\t";
   for (pai = pid->actionInfo;
         pai;
         pai = pai->nextAction) {

     fprintf(phelper->fout,"\t\t%swhich occurs in these events\n"
             , pai == pid->actionInfo ? "" : "and "
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

   if (pid->action_returns_decl)
  {
    fprintf(phelper->fout,"\t\tand which returns\n");
    phelper->indenture = "\t\t\t";
    iterate_list(pid->action_returns_decl, print_pid_name, phelper);
  }

   if (pid->docCmnt)
     fprintf(phelper->fout,"Doc Comments:\n%s\n"
           ,pid->docCmnt);

   return false;
}

void parser_debug_print_action_list_deep(pLIST plist, FILE *file)
{
   DEBUG_LIST_HELPER helper = {0};

   helper.fout = file;

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

   pACTION_SE_INFO	pase_info;

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

}

void parser_debug_print_transition_fn_list(pLIST plist, FILE *file)
{
   DEBUG_LIST_HELPER helper = {0};

   helper.fout = file;

   iterate_list(plist,print_transition_info,&helper);
}
#endif

