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

struct _id_info_ {
  char    			  *name;
  int     			  type;
  pID_INFO			  nextID;
  pID_INFO			  nextState;
  pID_INFO			  nextEvent;
  pID_INFO			  nextAction;
  pID_INFO			  nextTransition;
  pID_INFO			  nextTransitionFn;
  pID_INFO        externalDesignation;
	int						  seOrder;
  int             tOrder;
	char					  *docCmnt;
	pACTION_INFO	  actionInfo;
  pACTION_SE_INFO	action_returns_decl;
  pACTION_SE_INFO	transition_fn_returns_decl;
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
  pID_INFO			state_list;
	int						state_count;
  pID_INFO			event_list;
	int						event_count;
	pID_INFO			transition_list;
  int           transition_count;
	pID_INFO			transition_fn_list;
  int           transition_fn_count;
	pID_INFO			action_list;
	pID_INFO			name;
	pACTION_INFO	**actionArray;
	char					*data;
	char					*native;
	MOD_FLAGS			modFlags;
  pID_INFO      machineTransition;
};

/* lexer id list handlers */
int add_id(int, char *, pID_INFO *);
int lookup_id(char *, pID_INFO *);
void free_ids(void);

/** _A is pID_INFO; _B is new type (int)   */
#define set_id_type(_A,_B) (_A)->type = _B

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
int  addToActionArray(pMACHINE_INFO,pACTION_INFO);
void addToActionList(pMACHINE_INFO, pID_INFO);
char *getFileNameNoDir(const char *);

/* general use data */
extern char *me;	/* main will set this to the program name (argv[0]) */
extern bool generate_instance;
extern bool compact_action_array;


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


#endif /* ----------- nothing below this line ---------------- */
