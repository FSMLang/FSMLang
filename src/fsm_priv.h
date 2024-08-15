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

#include "list.h"

#ifdef FSMLANG_DEVELOP
#define FSMLANG_DEVELOP_PRINTF(A, ...) if (A) fprintf(A, __VA_ARGS__)
#else
#define FSMLANG_DEVELOP_PRINTF(...)
#endif

#define RETURN_IF_NULL(A) \
if (!(A))                 \
{                         \
	return;               \
}                         \


/**
	These flags track the modifiers
		of "machine" and also the action return specifiers.
*/
typedef enum {
  mfNone
	, mfReentrant           = 1
	, mfActionsReturnStates = 2
  , mfActionsReturnVoid   = 4
  , mfMachineTransition   = 8
} MOD_FLAGS;

typedef enum {
   sfNone
   , sfInibitSubMachines = 1
   , sfHasEntryFn        = 2
   , sfHasExitFn         = 4
} STATE_FLAGS;

typedef enum HORIZONTAL_PLACEMENT {
   hp_none_given
   , hp_left
   , hp_center
   , hp_right
} HORIZONTAL_PLACEMENT;

typedef enum VERTICAL_PLACEMENT {
   vp_none_given
   , vp_top
   , vp_center
   , vp_bottom
} VERTICAL_PLACEMENT;

/**
 * Set the letter case for outputting
 * ancestry.
 * 
 * @author sstan (10/20/2023)
 */
typedef enum ANCESTRY_LETTER_CASE {
	alc_upper
	, alc_lower
} ANCESTRY_LETTER_CASE;

typedef enum ANCESTRY_INCLUSION
{
	ai_omit_self        = 1
	, ai_include_self   = 2
	, ai_omit_ultimate  = 4
	, ai_stop_at_parent = 8
} ANCESTRY_INCLUSION;

#define ACTIONS_RETURN_FLAGS (mfActionsReturnStates | mfActionsReturnVoid)

typedef struct _id_info_				 ID_INFO,                 *pID_INFO;
typedef struct _action_se_info_	         ACTION_SE_INFO,          *pACTION_SE_INFO;
typedef struct _matrix_info_	         MATRIX_INFO,             *pMATRIX_INFO;
typedef struct _action_info_		     ACTION_INFO,             *pACTION_INFO;
typedef struct _machine_info_		     MACHINE_INFO,            *pMACHINE_INFO;
typedef struct _state_and_event_decls_   STATE_AND_EVENT_DECLS,   *pSTATE_AND_EVENT_DECLS;
typedef struct _statement_decl_list_     STATEMENT_DECL_LIST,     *pSTATEMENT_DECL_LIST;   
typedef struct _actions_and_transitions_ ACTIONS_AND_TRANSITIONS, *pACTIONS_AND_TRANSITIONS;
typedef struct _action_decl_             ACTION_DECL,             *pACTION_DECL;
typedef struct _machine_qualifier_       MACHINE_QUALIFIER,       *pMACHINE_QUALIFIER;
typedef struct _machine_prefix_          MACHINE_PREFIX,          *pMACHINE_PREFIX;
typedef struct _iterator_helper_         ITERATOR_HELPER,         *pITERATOR_HELPER;

typedef struct _event_data_              EVENT_DATA,              *pEVENT_DATA;
typedef struct _state_data_              STATE_DATA,              *pSTATE_DATA;
/* The following must not be confused with ACTION_INFO */
typedef struct _action_data_             ACTION_DATA,             *pACTION_DATA;
typedef struct _data_field_              DATA_FIELD,              *pDATA_FIELD;
typedef union  _data_type_union_         DATA_TYPE_UNION,         *pDATA_TYPE_UNION;
typedef struct _data_type_struct_        DATA_TYPE_STRUCT,        *pDATA_TYPE_STRUCT;
typedef struct _data_field_name_         DATA_FIELD_NAME,         *pDATA_FIELD_NAME;
typedef struct _user_event_data_         USER_EVENT_DATA,         *pUSER_EVENT_DATA;
typedef struct _native_info_             NATIVE_INFO,             *pNATIVE_INFO;


typedef union  _pid_type_data_           PID_TYPE_DATA,           *pPID_TYPE_DATA;

struct _user_event_data_
{
   pLIST    data_fields;
   pID_INFO translator;
};

struct _state_data_
{
   STATE_FLAGS state_flags;
   pID_INFO    entry_fn;
   pID_INFO    exit_fn;
   unsigned    event_density_pct;
   pLIST       pevents_handled;
   pLIST       pinbound_transitions;
   pLIST       poutbound_transitions;
   pLIST       pactions_list;
};

struct _event_data_
{
   pID_INFO         externalDesignation;
   pLIST            psharing_sub_machines;
   bool             shared_with_parent;
   unsigned         single_pai_state_count;
   pACTION_INFO     psingle_pai;
   bool             single_pai_for_all_states;
   pUSER_EVENT_DATA puser_event_data;
   pLIST            phandling_states;
   pLIST            pactions_list;
   unsigned         state_density_pct;
};

struct _action_data_
{
	pACTION_INFO    actionInfo;
	pLIST           action_returns_decl;
};

union _pid_type_data_
{
   EVENT_DATA    event_data;
   STATE_DATA    state_data;
   ACTION_DATA   action_data;
};

typedef enum
{
   dtt_simple
   , dtt_union 
   , dtt_struct 
} DATA_TYPE_TYPE;
union _data_type_union_
{
   pID_INFO         name;
   pLIST            members;
};

struct _data_type_struct_
{
   DATA_TYPE_TYPE  dtt;
   DATA_TYPE_UNION dtu;
};

struct _data_field_              
{
   pID_INFO            data_field_name;
   pDATA_TYPE_STRUCT   pdts;
   bool                isPointer;
   char              * dimension;
};

struct _iterator_helper_
{
   FILE          *fout;
   pMACHINE_INFO pmi;
   pID_INFO      pid;
   pACTION_INFO  pai;
   bool          error;
   bool          first;
   int           event;
   int           state;
   unsigned      *counter0;
   unsigned      *counter1;
   unsigned      tab_level;
   bool          found;
   LIST_ITERATOR_FN pfn_sub_iterator;
};


struct _machine_prefix_ 
{
   pMACHINE_INFO pmachineInfo;
};

struct _action_decl_            
{
   pLIST action_list;
   pLIST action_info_list;
   pLIST transition_list;
   pLIST transition_fn_list;
};

struct _actions_and_transitions_
{
   pLIST action_list;
   pLIST action_info_list;
   pLIST transition_list;
   pLIST transition_fn_list;
   pLIST machine_list;
};

struct _statement_decl_list_
{
   pLIST                    data;
   pSTATE_AND_EVENT_DECLS   pstate_and_event_decls;
   pACTIONS_AND_TRANSITIONS pactions_and_transitions;
};

struct _state_and_event_decls_
{
   pLIST state_decls;
   pLIST event_decls;
};

struct _id_info_ {
  char           *name;
  int             type;
  unsigned        order;
  pID_INFO        nextID;
  char           *docCmnt;
  PID_TYPE_DATA   type_data;
  pMACHINE_INFO   powningMachine;
  pID_INFO        pfield_type;
  pDATA_FIELD     pdata_field;
  pLIST           transition_fn_returns_decl;
};

struct _action_se_info_ {
  pID_INFO  			se;
  pACTION_SE_INFO next;
};

struct _matrix_info_ {
  pLIST	state_list;
  pLIST	event_list;
};

struct _action_info_
{
	pID_INFO     action;
	pMATRIX_INFO matrix;
	pID_INFO     transition;
	pACTION_INFO nextAction;
	char         *docCmnt;
}; 


struct _native_info_ {
   char *prologue;
   char *epilogue;
};
struct _machine_qualifier_
{
   MOD_FLAGS			modFlags;
   pID_INFO      machineTransition;
   char          *native_impl_prologue;
   char          *native_impl_epilogue;
};

struct _machine_info_ {
  pMACHINE_INFO parent;
  pLIST         state_list;
  unsigned      external_state_designation_count;
  unsigned      parent_event_reference_count;
  unsigned      data_translator_count;
  unsigned      data_block_count;
  unsigned      submachine_inhibitor_count;
  pLIST         event_list;
  unsigned      external_event_designation_count;
  pLIST         transition_list;
  pLIST         transition_fn_list;
  pLIST         action_list;
  pID_INFO      name;
  pLIST         action_info_list;
  pACTION_INFO  **actionArray;
  pLIST          data;
  char          *native_prologue;
  char          *native_epilogue;
  char          *native_impl_prologue;
  char          *native_impl_epilogue;
  MOD_FLAGS      modFlags;
  pID_INFO      machineTransition;
  pLIST         machine_list;
  unsigned      shared_event_count;
  pLIST         id_list;
  bool          has_single_pai_events;
  unsigned      states_with_entry_fns_count;
  unsigned      states_with_exit_fns_count;
  unsigned      sub_machine_depth;
  unsigned      events_with_zero_handlers;
  unsigned      events_with_one_handler;
  unsigned      states_with_zero_events;
  unsigned      states_with_one_event;
  unsigned      states_with_no_way_in;
  unsigned      states_with_no_way_out;
  unsigned      average_state_event_density_pct;
  unsigned      average_event_state_density_pct;
};

/* lexer id list handlers */
extern pLIST id_list;
int  add_id(pLIST, int, char *, pID_INFO *);
bool add_unique_id(pLIST, int, char *, pID_INFO *);
int  lookup_id(pLIST, char *, pID_INFO *);
void free_ids(pLIST);

/** _A is pID_INFO; _B is new type (int)   */
#define set_id_type(_A,_B) (_A)->type = _B

/* parser info */
void freeMachineInfo(pMACHINE_INFO);

/* other general utilities */
FILE *openFile(char *, char *);
char *createFileName(char *,char *);
char *eventNameByIndex(pMACHINE_INFO,int);
pID_INFO eventPidByIndex(pMACHINE_INFO,int);
char *stateNameByIndex(pMACHINE_INFO,int);
pID_INFO statePidByIndex(pMACHINE_INFO,int);
pID_INFO transitionPidByIndex(pMACHINE_INFO,int);
int  allocateActionArray(pMACHINE_INFO);
char *getFileNameNoDir(const char *);
void enumerate_pid_list(pLIST);
void count_external_declarations(pLIST,unsigned*);
void count_sub_machine_inhibitors(pLIST,unsigned*);
void count_parent_event_referenced(pLIST,unsigned*);
void count_shared_events(pLIST,unsigned*);
void count_event_user_data_attributes(pLIST,unsigned*,unsigned*);
void count_states_with_entry_exit_fns(pLIST,unsigned*,unsigned*);
void count_states_with_zero_events(pLIST,unsigned*);
void count_states_with_one_event(pLIST,unsigned*);
void count_states_with_no_way_in(pLIST,unsigned*);
void count_states_with_no_way_out(pLIST,unsigned*);
void count_events_with_zero_handlers(pLIST,unsigned*);
void count_events_with_one_handler(pLIST,unsigned*);
void compute_event_and_state_density_pct(pMACHINE_INFO);
bool populate_action_array(pMACHINE_INFO,FILE*);
int  copyFileContents(const FILE*,const char*);
bool printAncestry(pMACHINE_INFO,FILE*,char*,ANCESTRY_LETTER_CASE,ANCESTRY_INCLUSION);
void printNameWithAncestry(char*,pMACHINE_INFO,FILE*,char*,ANCESTRY_LETTER_CASE,ANCESTRY_INCLUSION);
pMACHINE_INFO ultimateAncestor(pMACHINE_INFO);
unsigned maxDepth(pMACHINE_INFO);
void print_tab_levels(FILE*,unsigned);
void streamStrCaseAware(FILE*,char*,ANCESTRY_LETTER_CASE);
void increase_sub_machine_depth(pMACHINE_INFO);

#ifdef PARSER_DEBUG
void parser_debug_print_state_list(pLIST,FILE*);
void parser_debug_print_event_list(pLIST,FILE*);
void parser_debug_print_id_list_names(pLIST,pMACHINE_INFO,FILE*,char*);
void parser_debug_print_action_list_deep(pLIST,pMACHINE_INFO,FILE*);
void parser_debug_print_transition_list(pLIST,FILE*);
void parser_debug_print_transition_fn_list(pLIST,FILE*);
void parser_debug_print_data_block(pLIST,FILE*);
#endif

/* general use data */
extern char                 *me;	/* main will set this to the program name (argv[0]) */
extern char                 *inputFileName;
extern bool                 generate_instance;
extern bool                 compact_action_array;
extern bool                 generate_weak_fns;
extern bool                 force_generation_of_event_passing_actions;
extern bool                 core_logging_only;
extern bool                 include_svg_img;
extern bool                 short_dbg_names;
extern bool                 add_machine_name;
extern bool                 generate_run_function;
extern bool                 add_event_cross_reference;
extern bool                 add_plantuml_title;
extern bool                 add_plantuml_legend;
extern bool                 exclude_events_from_plantuml_legend;
extern bool                 exclude_states_from_plantuml_legend;
extern bool                 exclude_actions_from_plantuml_legend;
extern HORIZONTAL_PLACEMENT plantuml_legend_horizontal_placement;
extern VERTICAL_PLACEMENT   plantuml_legend_vertical_placement;
extern pLIST                pplantuml_prefix_strings_list;
extern pLIST                pplantuml_prefix_files_list;
extern bool                 output_generated_file_names_only;
extern bool                 output_header_files;
extern bool                 output_make_recipe;
extern bool                 short_user_fn_names;
extern bool                 print_action_array;
extern bool                 convenience_macros_in_public_header;

#define LOOKUP	0	/* default - not defined in the parser */

/* other useful macros */
#define CHECK_AND_FREE(A)	if ( A ) { free( A ); A = NULL; }
#define FREE_AND_CLEAR(A) free( A ); A = NULL;
#define FCLOSE_AND_CLEAR(A) if (A) { fclose( A ); A = NULL; }

/*
	Output generation.

	Once the machine is parsed, we need to generate some output.

	The output object will have three functions:
		1) initOutput - called to initialize the outputer
		2) writeMachine - called each time a machine is successfully parsed
		3) closeOutput - called to close the outputer.

	Only one instance of an output device will be run at a time.
*/

typedef struct _fsm_output_generator_         FSMOutputGenerator
                                              , *pFSMOutputGenerator;
typedef struct _fsm_output_generator_factory_ FSMOutputGeneratorFactoryStr
                                                , *pFSMOutputGeneratorFactoryStr;

typedef int (*fpInitOutput)(pFSMOutputGenerator,char *);
typedef void (*fpWriteMachine)(pFSMOutputGenerator,pMACHINE_INFO);
typedef void (*fpCloseOutput)(pFSMOutputGenerator,int);

typedef pFSMOutputGenerator (*fpFSMOutputGeneratorFactory)(pFSMOutputGenerator);

struct _fsm_output_generator_
{
	fpInitOutput	            initOutput;
	fpWriteMachine	            writeMachine;
	fpCloseOutput	            closeOutput;
	fpFSMOutputGeneratorFactory fsmogFactory;
};

struct _fsm_output_generator_factory_
{
	fpFSMOutputGeneratorFactory fsmogf;
	pFSMOutputGenerator         parent_fsmog;
};

extern void yyerror(char*);

void write_machines(pLIST,fpFSMOutputGeneratorFactory, pFSMOutputGenerator);
bool print_machine_component(pLIST_ELEMENT,void*);
bool print_sub_machine_event(pLIST_ELEMENT,void*);
bool print_sub_machine_component_name(pLIST_ELEMENT,void*);
bool print_sub_machine_events(pLIST_ELEMENT,void*);
bool print_sub_machine_event_names(pLIST_ELEMENT,void*);
bool print_data_field(pLIST_ELEMENT,void*);

#endif /* ----------- nothing below this line ---------------- */
