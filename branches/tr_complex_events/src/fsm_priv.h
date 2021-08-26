/*
	fsm_priv.h

	Contains definitions, &c. private to the fsmlang compiler


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

#ifndef _FSM_PRIV_H
#define _FSM_PRIV_H

#include <stdio.h>
#include <stdbool.h>

#define DEFAULT_NAME_SPACE (0)

/*
	These flags track the modifiers
		of "machine" and also the action return specifiers.
*/
typedef enum {
	mfReentrant             = 1
	, mfActionsReturnStates = 2
  , mfActionsReturnVoid   = 4
  , mfMachineTransition   = 8
} MOD_FLAGS;

typedef struct _id_info_				ID_INFO, *pID_INFO;
typedef struct _action_se_info_	ACTION_SE_INFO, *pACTION_SE_INFO;
typedef struct _matrix_info_	  MATRIX_INFO, *pMATRIX_INFO;
typedef struct _action_info_		ACTION_INFO, *pACTION_INFO;
typedef struct _machine_info_		MACHINE_INFO, *pMACHINE_INFO;
typedef struct _complex_event_    COMPLEX_EVENT, *pCOMPLEX_EVENT;
typedef struct _complex_event_cb_ complex_event_cb, *pcomplex_event_cb;

struct _id_info_ {
  char    			  *name;
  char    			  *name_prefix;
  int     			  type;
  unsigned        namespace;
  pID_INFO			  nextID;
  pID_INFO			  nextState;
  pID_INFO			  nextEvent;
  pID_INFO			  nextAction;
  pID_INFO			  nextTransition;
  pID_INFO			  nextTransitionFn;
  pID_INFO        externalDesignation;
  pID_INFO        parentEvent;
	int						  seOrder;
  int             tOrder;
  int             actionCount; //!< records the number af actions associated with this state or event.
  int             transitionCount; //!< records the number af actions associated with this state or event.
	char					  *docCmnt;
	pACTION_INFO	  actionInfo;
  pACTION_SE_INFO	action_returns_decl;
  pACTION_SE_INFO	transition_fn_returns_decl;
  pCOMPLEX_EVENT  complexInfo;
};

struct _action_se_info_ {
  pID_INFO  			se;
  pACTION_SE_INFO next;
};

struct _matrix_info_ {
  pACTION_SE_INFO	state_list;
  pACTION_SE_INFO	event_list;
};

struct _action_info_ {
	pID_INFO				action;
  pMATRIX_INFO    matrix;
  pID_INFO  			transition;
	pACTION_INFO		nextAction;
};

struct _machine_info_ {
   unsigned     namespaces;
   pID_INFO			state_list;
	int						state_count;
  int           external_state_designation_count;
  pID_INFO			event_list;
	int						event_count;
  int           external_event_designation_count;
	pID_INFO			transition_list;
  int           transition_count;
	pID_INFO			transition_fn_list;
  int           transition_fn_count;
	pID_INFO     complex_event_list;
   int          complex_event_count;
   pID_INFO     action_list;
	pID_INFO			name;
	pACTION_INFO	**actionArray;
	char					*data;
	char					*native;
	MOD_FLAGS			modFlags;
  pID_INFO      machineTransition;
};

struct _complex_event_
{
   pID_INFO	nextEvent;
   pID_INFO members;
   unsigned member_count;
   pID_INFO parent;
   unsigned external_event_designation_count;
   unsigned namespace;
   pID_INFO *states;
   char     *name_prefix;
};

struct _complex_event_cb_
{
   FILE          * fp;
   pMACHINE_INFO   pmi;
   char          * ucMachineName;
   pID_INFO        state;
};

typedef void (*generic_callback_fn)(pID_INFO,void*);
void iterate_complex_event(pID_INFO, generic_callback_fn,void*);
void iterate_complex_event_ancestors(pID_INFO, generic_callback_fn,void*);
void print_complex_event_state_handler_name(pID_INFO,pcomplex_event_cb);
void print_complex_event_discriminator_name(pID_INFO,pcomplex_event_cb);

/* lexer id list handlers */
int add_id(int, char *, unsigned, pID_INFO *);
int lookup_id(char *, unsigned, pID_INFO *);
void free_ids(void);

/** _A is pID_INFO; _B is new type (int)   */
#define set_id_type(_A,_B) (_A)->type = _B
#define get_id_type(_A) ((_A)->type)

/** _A is an event pID_INFO, _B is pMACHINE_INFO   */
#define get_event_name(_A, _B) ((assignExternalEventValues((_B)))\
 ? (_A)->externalDesignation->name\
 : (_A)->name\
  )

/** _A is an event pID_INFO, _B is pMACHINE_INFO   */
#define get_event_name_prefix(_A, _B) ((assignExternalEventValues((_B)) && (_A)->externalDesignation->name_prefix)\
                                       ? (_A)->externalDesignation->name_prefix\
                                       : ""\
                                       )

/** _A is an event or state pID_INFO   */
#define has_actions(_A) ((((pID_INFO)_A)->actionCount > 0))
#define has_transitions(_A) ((((pID_INFO)_A)->transitionCount > 0))
#define has_actions_or_transitions(_A) (has_actions(_A) || has_transitions(_A))

/* parser info */
void freeMachineInfo(pMACHINE_INFO);

/* other general utilities */
FILE *openFile(char *, char *);
char *createFileName(char *,char *);
char *hungarianToUnderbarCaps(char *);
char *eventNameByIndex(pMACHINE_INFO,int);
pID_INFO eventPidByIndex(pMACHINE_INFO,int);
char *stateNameByIndex(pMACHINE_INFO,int);
pID_INFO statePidByIndex(pMACHINE_INFO,int);
char *transitionNameByIndex(pMACHINE_INFO,int);
pID_INFO transitionPidByIndex(pMACHINE_INFO,int);
int  allocateActionArray(pMACHINE_INFO);
int      allocateComplexEventStateLists(pMACHINE_INFO);
int  addToActionArray(pMACHINE_INFO,pACTION_INFO);
void addToActionList(pMACHINE_INFO, pID_INFO);
char *getFileNameNoDir(const char *);
void     print_complex_event(FILE*,pID_INFO,pMACHINE_INFO);
void     print_complex_event_ancestry(FILE*,pID_INFO,bool);
void     print_external_event_name_with_prefix(FILE*,pID_INFO);
pID_INFO get_complex_event_ultimate_ancestor(pID_INFO);
bool     assignExternalEventValues(pMACHINE_INFO);

/* general use data */
extern char *me;	/* main will set this to the program name (argv[0]) */
extern bool generate_instance;
extern bool compact_action_array;
extern unsigned namespace;
extern pID_INFO complex_parent;


#define LOOKUP	0	/* default - not defined in the parser */

/* other useful macros */
#define CHECK_AND_FREE(A)	if ( A ) { free( A ); A = NULL; }
#define FREE_AND_CLEAR(A) free( A ); A = NULL;
#define FCLOSE_AND_CLEAR(A) fclose( A ); A = NULL;

/*
	Output generation.

	Once the machine is parsed, we need to generate some output.

	The output object will have three functions:
		1) initOutput - called to initialize the outputer
		2) writeMachine - called each time a machine is successfully parsed
		3) closeOutput - called to close the outputer.

	Only one instance of an output device will be run at a time.
*/

typedef struct _fsm_output_generator_      FSMOutputGenerator, *pFSMOutputGenerator;

typedef int (*InitOutput)(char *);
typedef void (*WriteMachine)(pMACHINE_INFO);
typedef void (*CloseOutput)(int);

struct _fsm_output_generator_
{
	InitOutput		initOutput;
	WriteMachine	writeMachine;
	CloseOutput		closeOutput;
};

extern void yyerror(char*);


#endif /* ----------- nothing below this line ---------------- */
