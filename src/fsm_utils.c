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

typedef struct _complex_event_print_ complex_event_print_cb, *pcomplex_event_print_cb;
struct _complex_event_print_ 
{
   FILE          *fp;
   char          comma;
   pMACHINE_INFO pmi;
};

typedef struct _complex_event_state_list_ complex_event_state_list_cb, *pcomplex_event_state_list_cb;
struct _complex_event_state_list_ 
{
   pMACHINE_INFO pmi;
   bool          success;
};

typedef struct _print_complex_ancestry_ print_complex_ancestry_cb, *pprint_complex_ancestry_cb;
struct _print_complex_ancestry_
{
   FILE *fp;
   bool use_external;
};

/* the general use data */
char	*me = "I don't know who I am, but I'm";
bool generate_instance    = true;
bool compact_action_array = false;

pID_INFO id_list = NULL;

static void _print_ancestry     (pID_INFO,void*);
static void _print_complex_event(pID_INFO,void*);
static void _allocate_state_list(pID_INFO,void*);
static void _add_to_state_list  (pID_INFO,void*);

static void _add_state_to_complex_ancestors(pID_INFO ce, pID_INFO state);

int add_id(int type, char *name, unsigned namespace, pID_INFO *ppid_info)
{
   int ret_val;

  /* not already defined ? */
  if ((ret_val = lookup_id(name,namespace,ppid_info)) == LOOKUP)
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

     (*ppid_info)->nextID = id_list;
     (*ppid_info)->name = strdup(name);
     (*ppid_info)->type = type;
     (*ppid_info)->namespace = namespace;
     id_list = *ppid_info;

     #ifdef LEX_DEBUG
     printf("Added : %s of type %s in namespace %u\n",name,strings[type]
            , namespace
            );
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

int lookup_id(char *name, unsigned namespace, pID_INFO *ppid_info)
{

  for (*ppid_info = id_list; *ppid_info; *ppid_info = (*ppid_info)->nextID)
  {
     if ((*ppid_info)->namespace == namespace)
     {
        if (!strcmp((*ppid_info)->name,name))

      return (*ppid_info)->type;
     }
  }

  return LOOKUP;  /* i.e. not found */

}

void free_ids(void)
{

	pID_INFO	pid_info, pid_info1;
	pACTION_INFO		pai,pai1;
	pACTION_SE_INFO	pasi, pasi1;

	pid_info = id_list;
	while (pid_info) {

		pid_info1 = pid_info->nextID;

		free(pid_info->name);
		#ifdef MEM_DEBUG
		printf("freeing ID_INFO 0x%x\n",pid_info);
		#endif

		CHECK_AND_FREE(pid_info->docCmnt);

		/* free the action related stuff */
		for (pai = pid_info->actionInfo; pai; pai = pai1) {
	
				pai1 = pai->nextAction;
	
				/* events */
				for (pasi = pai->matrix->event_list; pasi; pasi = pasi1) {
					pasi1 = pasi->next;
					#ifdef MEM_DEBUG
					printf("freeing event ACTION_SE_INFO 0x%x\n",pasi);
					#endif
					free(pasi);
				}
	
				/* states */
				for (pasi = pai->matrix->state_list; pasi; pasi = pasi1) {
					pasi1 = pasi->next;
					#ifdef MEM_DEBUG
					printf("freeing state ACTION_SE_INFO 0x%x\n",pasi);
					#endif
					free(pasi);
				}
	
				/* the action info block itself */
				#ifdef MEM_DEBUG
				printf("freeing ACTION_INFO 0x%x\n",pai);
				#endif
				free(pai);
	
		}

		free(pid_info);
		pid_info = pid_info1;

	}

	id_list = NULL;

}

void freeMachineInfo(pMACHINE_INFO pmi)
{

	int							i;

	if (!pmi)

		return;

	/* and, the action array */
	if (pmi->actionArray) {

		for (i = 0; i < pmi->event_count; i++) {

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

	if (!pmi || !pmi->state_count || !pmi->event_count)
		return 1;

	if (!(pmi->actionArray = (pACTION_INFO **)calloc(pmi->event_count,sizeof(pACTION_INFO *))))
		return 1;

	for (i = 0; i < pmi->event_count; i++)

		if (!(pmi->actionArray[i] = (pACTION_INFO *)calloc(pmi->state_count,sizeof(pACTION_INFO))))

			return 1;

	return 0;

}

/**********************************************************************************************************************/
/**
 * @brief Allocate space for state lists for each complex event_count
 * 
 * @author Steven Stanton (9/19/2019)
 * 
 * @param pmi pointer to the machine info struct
 * 
 *
 * @ref_global none
 *
 * @mod_global none
 *
 * @return int 0 on success; 1 on failure
 *
 * 
 ***********************************************************************************************************************/
int  allocateComplexEventStateLists(pMACHINE_INFO pmi)
{
   complex_event_state_list_cb ceslcb = {.pmi = pmi, .success = true};

   for (pID_INFO ce = pmi->complex_event_list; ce && ceslcb.success; ce = ce->nextEvent)
   {
      _allocate_state_list(ce, &ceslcb);
      iterate_complex_event(ce, _allocate_state_list, &ceslcb);
   }
}

/**
	function: addToActionArray

	Add the given Action to the Action array,
		complaining if there is already something
		in the slot.

	Arguments : 
		pMACHINE_INFO	-	The completed machine info struct.

	returns: 0 on success
					 1 on failure
*/
int addToActionArray(pMACHINE_INFO pmi, pACTION_INFO pai)
{
	pACTION_SE_INFO	pasiE, pasiS;

	/* since we look at things from an event,state order */
	for (pasiE = pai->matrix->event_list; pasiE; pasiE = pasiE->next) {

		for (pasiS = pai->matrix->state_list; pasiS; pasiS = pasiS->next) {

			if (pmi->actionArray[pasiE->se->seOrder][pasiS->se->seOrder]) {
		
				fprintf(stderr,"Duplicate action: %s [%s, %s]:\n"
                 , pai->action->name
                 , pasiE->se->name
                 , pasiS->se->name
                 );
				fprintf(stderr,"Action already there : %s\n",
					pmi->actionArray[pasiE->se->seOrder][pasiS->se->seOrder]->action->name);
		
				return 1;
		
			}
	
			pmi->actionArray[pasiE->se->seOrder][pasiS->se->seOrder] = pai;

      if (get_id_type(pasiE->se) == COMPLEX_EVENT_MEMBER)
      {
         _add_state_to_complex_ancestors(pasiE->se, pasiS->se);

		}

      pasiE->se->actionCount++;
      pasiS->se->actionCount++;

      if (pai->transition)
      {
         pasiE->se->transitionCount++;
         pasiS->se->transitionCount++;
      }
		}

	}

	return 0;

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

	if (pmi && (index >= 0) && (index < pmi->event_count)) {

		for (pidi = pmi->event_list; pidi; pidi = pidi->nextEvent)

			if (pidi->seOrder == index) {

				cp = pidi->name;

				break;

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

	if (pmi && (index >= 0) && (index < pmi->event_count)) {

		for (pidi = pmi->event_list; pidi; pidi = pidi->nextEvent)

			if (pidi->seOrder == index) {

				break;

			}

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

	if (pmi && (index >= 0) && (index < pmi->state_count)) {

		for (pidi = pmi->state_list; pidi; pidi = pidi->nextState)

			if (pidi->seOrder == index) {

				cp = pidi->name;

				break;

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

	if (pmi && (index >= 0) && (index < pmi->state_count)) {

		for (pidi = pmi->state_list; pidi; pidi = pidi->nextState)

			if (pidi->seOrder == index) {

				break;

			}

	}

	return pidi;

}


/**
	function: transitionNameByIndex

* looks up the transition with the tOrder equal to the given 
* index. The pointer returned points to a field in the ID_INFO 
* struct and MUST NOT be freed. 

	Arguments:

			pMACHINE_INFO pmi	- the machine with which we are concerned.
			int	index	-	The index we want.

	Returns

		a pointer to the transition_fn name on success, or
		NULL on error.
*/
char *transitionNameByIndex(pMACHINE_INFO pmi, int index)
{

	char 			*cp = NULL;
	pID_INFO	pidi;

	if (pmi && (index >= 0) && (index < pmi->transition_count)) {

		for (pidi = pmi->transition_list; pidi; pidi = pidi->nextTransition)

			if (pidi->tOrder == index) {

				cp = pidi->name;

				break;

			}

	}

	return cp;

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

	if (pmi && (index >= 0) && (index < pmi->transition_count)) {

		for (pidi = pmi->transition_list; pidi; pidi = pidi->nextTransition)

			if (pidi->tOrder == index) {

				break;

			}

	}

	return pidi;

}

/**
	function: addToActionList

	Not to be confused with functions concerning the actionArray, this function
		uniquiely adds a pID_INFO to the machine's action_list.

	Arguments:

		pMACHINE_INFO pmi - the machine with which we are concerned.
		pID_INFO pid			-	the pID_INFO struct to add.

	Returns:	void
*/
void addToActionList(pMACHINE_INFO pmi, pID_INFO pid)
{

	pID_INFO p;

	/* Make certain we are not yet on the list. */
	for (p = pmi->action_list; p; p = p->nextAction) {

		if (p == pid)
    {
        return;
    }

	}

	/* Still here means we weren't on the list */
	pid->nextAction = pmi->action_list;
	pmi->action_list = pid;

}

/**
	return the length of the given string, or 0, if NULL is passed.
*/
int safe_strlen(char *s)
{
	return (s ? strlen(s) : 0);
}

/**
	returns a copy of the file portion of path

	memory is allocated.
*/
char *getFileNameNoDir(const char *path)
{
	char *cp,*cp1;

	for(cp = path; cp1 = strpbrk(cp,"\\/"); cp = ++cp1)
		;

	if ((cp1 = malloc(safe_strlen(cp)+1)) != NULL)
		strcpy(cp1,cp);

	return cp1;

}

/**
 * recursively print a complex event
 */
void print_complex_event(FILE *fp, pID_INFO pid, pMACHINE_INFO pmi)
{
   complex_event_print_cb cb = {fp, ' ' , pmi};

   _print_complex_event(pid, &cb);

   cb.comma = '\n';
   iterate_complex_event(pid, _print_complex_event, &cb);

   fprintf(fp
           , "}\n\n"
           );
}

/**********************************************************************************************************************/
/**
 * @brief print the actions return spec indicated by modFlags
 * 
 * @author Steven Stanton (9/5/2021)
 * 
 * @param fp pointer to the output fileName
 * @param modFlags flags containing the actions return spec
 * @param le the character to place at the end of the string.
 * 
 *
 * @ref_global none
 *
 * @mod_global none
 *
 * @thread_safe yes
 *
 * 
 ***********************************************************************************************************************/
void print_actions_return_spec(FILE *fp, MOD_FLAGS modFlags, char le)
{
   if (modFlags & mfActionsReturnStates) {

     fprintf(fp,"Actions return states%c",le);

   }
   else if (modFlags & mfActionsReturnVoid) {

     fprintf(fp,"Actions return void%c",le);

   }
   else {

     fprintf(fp,"Actions return events%c",le);

   }
}

static void _print_complex_event(pID_INFO ce, void *param)
{
   pcomplex_event_print_cb pcb = param;
   int                     i;
   bool                    comma;

   if (get_id_type(ce) == COMPLEX_EVENT_KEY)
   {
      fprintf(pcb->fp, "%c ", pcb->comma);

      fprintf(pcb->fp
              , "%s (namespace: %u, %u, parent: %s, prefix: %s; "
              , ce->name
              , ce->namespace
              , ce->complexInfo->namespace
              , ce->complexInfo->parent
                  ? ce->complexInfo->parent->name
                  : "None"
              , ce->complexInfo->name_prefix
                  ? ce->complexInfo->name_prefix 
                  : "None"
              );

      print_actions_return_spec(pcb->fp, ce->complexInfo->mod_flags,')');

      fprintf(pcb->fp
              , ":\n\tneeded by states: "
              );

      for (i = 0, comma = false; i < pcb->pmi->state_count; i++)
      {
         if (ce->complexInfo->states[i])
         {
            fprintf(pcb->fp
                    , "%s%s"
                    , comma ? ", " : ""
                    , ce->complexInfo->states[i]->name
                   );
            comma = true;
         }
      }
      fprintf(pcb->fp, "\n{");
      pcb->comma = '\n';

   }
   else
   {
      fprintf(pcb->fp
              , ce->externalDesignation 
                  ? "\t%c %s = %s; namespace: %u; parent: %s; seOrder: %u\n"
                  : "\t%c %s%s; namespace: %u; parent: %s; seOrder: %u\n"
              , pcb->comma
              , ce->name
              , ce->externalDesignation ? ce->externalDesignation->name : ""
              , ce->namespace
              , ce->complexInfo->parent
                  ? ce->complexInfo->parent->name
                  : "None"
              , ce->seOrder
              );
      pcb->comma = ',';
   }

}

void print_complex_event_ancestry(FILE *fp, pID_INFO ce, bool use_external)
{
   print_complex_ancestry_cb pcacb = {.fp = fp, .use_external = use_external};
   iterate_complex_event_ancestors(ce, _print_ancestry, &pcacb);
}

void print_external_event_name_with_prefix(FILE *fp, pID_INFO event)
{
   if (get_id_type(event) == EVENT)
   {
      fprintf(fp
              , "%s%s"
              , event->externalDesignation->name_prefix 
                  ? event->externalDesignation->name_prefix 
                  : ""
              , event->externalDesignation->name
              );
   }
   else
   {
      fprintf(fp
              , "%s%s"
              , event->complexInfo->parent
                  ? (event->complexInfo->parent->complexInfo->name_prefix
                        ? event->complexInfo->parent->complexInfo->name_prefix
                        : ""
                     )
                  : event->externalDesignation->name_prefix
                     ? event->externalDesignation->name_prefix 
                     : ""
              , event->externalDesignation->name
              );
   }
}

static void _print_ancestry(pID_INFO ce, void *p)
{
   pprint_complex_ancestry_cb ppcacb = p;

   fprintf(ppcacb->fp
           , "%s%s%s"
           , (ppcacb->use_external && ce->externalDesignation)
               ? (ce->complexInfo->parent
                  ? (ce->complexInfo->parent->complexInfo->name_prefix
                        ? ce->complexInfo->parent->complexInfo->name_prefix
                        : ""
                    )
                  : (ce->externalDesignation->name_prefix
                     ? ce->externalDesignation->name_prefix
                     : ""
                    )
                 )
               : ""
           , (ppcacb->use_external && ce->externalDesignation)
               ? ce->externalDesignation->name
               : ce->name
           , (get_id_type(ce) == COMPLEX_EVENT_KEY)
               ? "."
               : ""
           );
}

static void _allocate_state_list(pID_INFO ce, void *param)
{
   pcomplex_event_state_list_cb pceslcb = param;

   pceslcb->success = (get_id_type(ce) == COMPLEX_EVENT_KEY)
                        ? (NULL != (ce->complexInfo->states 
                                    = (pID_INFO*)calloc(pceslcb->pmi->event_count
                                                                , sizeof(pID_INFO)
                                                                )
                                    )
                           )
                        : true
                        ;
}

void iterate_complex_event(pID_INFO ce, generic_callback_fn cbfn, void *param)
{
   pID_INFO p;

   for (p = ce->complexInfo->members; p; p = p->complexInfo->nextEvent)
   {
      if (get_id_type(p) == COMPLEX_EVENT_KEY)
      {
         (*cbfn)(p,param);
         iterate_complex_event(p,cbfn,param);
      }
      else
      {
         (*cbfn)(p,param);
      }
   }
}

void iterate_complex_event_ancestors(pID_INFO ce, generic_callback_fn cbfn, void *param)
{
   if (ce->complexInfo)
   {
      if (ce->complexInfo->parent)
      {
         iterate_complex_event_ancestors(ce->complexInfo->parent, cbfn, param);
      }

      (*cbfn)(ce,param);
   }
}

static void _add_to_state_list(pID_INFO ce, void *param)
{
   pID_INFO state = param;

   if (get_id_type(ce) == COMPLEX_EVENT_KEY)
   {
      ce->complexInfo->states[state->seOrder] = state;
   }

}

static void _add_state_to_complex_ancestors(pID_INFO ce, pID_INFO state)
{
   iterate_complex_event_ancestors(ce, _add_to_state_list, state);
}

pID_INFO get_complex_event_ultimate_ancestor(pID_INFO pid)
{
   pID_INFO ua = NULL;

   if (pid->complexInfo)
   {
      ua = pid;
      while (ua->complexInfo->parent)
      {
         ua = ua->complexInfo->parent;
      }
   }

   return ua;
}

bool assignExternalEventValues(pMACHINE_INFO pmi)
{
   return (
       (pmi->modFlags & (mfActionsReturnStates | mfActionsReturnVoid))
       && (pmi->event_count == pmi->external_event_designation_count)
       && !compact_action_array
       );

}

