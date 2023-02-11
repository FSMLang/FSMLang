%{         
	/* parser.y

		the FSM Description Language Parser.

	*/

#include <stdio.h>
#include <string.h>

#include <cwalk.h>

#if defined (LINUX) || defined (CYGWIN)
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#endif

#if defined (LINUX)
#include <unistd.h>
#include <getopt.h>
#endif

#include "revision.h"
#include "fsm_priv.h"

#include "fsm_c.h"
#include "fsm_cswitch.h"
#include "fsm_html.h"
#include "fsm_plantuml.h"

#include "list.h"

int lineno=1;
pLIST id_list = NULL;

//did we parse correctly?
int good=1;

extern char	*yytext;
extern FILE	*yyin, *yyout;

extern int yylex(void);

pMACHINE_INFO        pmachineInfo = NULL;
pFSMOutputGenerator	pfsmog;

void yyerror(char *);

%}

%union {
	pID_INFO				         pid_info;
	pACTION_SE_INFO	         se_info;
	pACTION_INFO		         action_info;
	pMATRIX_INFO		         matrix_info;
	char *					         charData;
 MOD_FLAGS                mod_flags;
 pMACHINE_INFO            pmachineInfo;
 pLIST                    plist;
 pSTATE_AND_EVENT_DECLS   pstate_and_event_decls;
 pSTATEMENT_DECL_LIST     pstatement_decl_list;
 pACTIONS_AND_TRANSITIONS pactions_and_transitions;
 pACTION_DECL						 paction_decl;
 pMACHINE_QUALIFIER       pmachine_qualifier;
 pMACHINE_PREFIX          pmachine_prefix;
}

%token MACHINE_KEY TRANSITION_KEY STATE_KEY EVENT_KEY ACTION_KEY ON PARENT NAMESPACE
%token REENTRANT ACTIONS RETURN STATES EVENTS RETURNS EXTERNAL EQUALS VOID TRANSLATOR_KEY
%token IMPLEMENTATION_KEY INHIBITS SUBMACHINES
%token <charData>	NATIVE_KEY
%token <charData>	NATIVE_BLOCK
%token <charData>	DATA_KEY
%token <charData>	DOC_COMMENT
%token <pid_info> MACHINE
%token <pid_info> STATE
%token <pid_info> EVENT
%token <pid_info> ACTION
%token <pid_info> TRANSITION_FN
%token <pid_info> ID

%type <plist>                    returns_comma_list
%type <plist>                    event_comma_list
%type <plist>                    event_vector
%type <plist>                    state_comma_list
%type <plist>                    state_vector
%type <matrix_info>              matrix
%type <plist>                    state_decl
%type <plist>                    event_decl
%type <plist>                    state_decl_list
%type <plist>                    event_decl_list
%type <pstate_and_event_decls>   state_and_event_decls
%type <pid_info>                 transition
%type <pid_info>                 external_designation
%type <action_info>              action_matrix
%type <action_info>              action
%type <paction_decl>             action_decl
%type <paction_decl>             action_decl_list
%type <action_info>              transition_matrix
%type <action_info>              transition_matrix_list
%type <charData>                 data
%type <charData>                 native
%type <charData>                 native_impl
%type <mod_flags>                machine_modifier
%type <pmachineInfo>             machine
%type <pstatement_decl_list>     statement_decl_list
%type <pactions_and_transitions> actions_and_transitions
%type <mod_flags>                action_return_spec
%type <pid_info>                 machine_transition_decl
%type <pmachine_qualifier>       machine_qualifier      
%type <plist>                    machine_list
%type <pmachine_prefix>          machine_prefix
%type <pid_info>                 namespace_event_ref
%type <pid_info>                 event_data
%type <pid_info>                 state

%%

fsmlang: machine 
					{ 

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found a single machine\n"); 

						#else

						/* write the machine */
						(*pfsmog->writeMachine)(pfsmog,$1);

						#endif

						/* get ready for the next machine */
						freeMachineInfo($1);

						free_ids(id_list);

					}
	| fsmlang machine	
					{ 

						#ifdef PARSER_DEBUG
						fprintf(yyout,"making a list of machines\n"); 
						#else

						/* write the machine */
						(*pfsmog->writeMachine)(pfsmog,$2);

						#endif

						/* get ready for the next machine */
						freeMachineInfo($2);

						free_ids(id_list);

					}
	;

machine_prefix: native machine_modifier MACHINE_KEY 
   {

				if (($$ = (pMACHINE_PREFIX)calloc(1,sizeof(MACHINE_PREFIX))) == NULL)
						yyerror("out of memory");

				if (($$->pmachineInfo = (pMACHINE_INFO)calloc(1,sizeof(MACHINE_INFO))) == NULL)
						yyerror("out of memory");

				/* grab any native language stuff */
				$$->pmachineInfo->native = $1;

       /* grab any modifiers */
 			$$->pmachineInfo->modFlags = $2;

       id_list = $$->pmachineInfo->id_list = init_list();

       $$->pmachineInfo->parent = pmachineInfo;
				pmachineInfo             = $$->pmachineInfo;

   }
 	;

machine:	machine_prefix ID machine_qualifier 
         {
            if (!($3->modFlags & ACTIONS_RETURN_FLAGS))
            {
   						pID_INFO pid_info;
              /* note that this is not added to the machine event list; it is here only to be
              found as an event id for return decls.
              */
   						add_id(id_list, EVENT,"noEvent",&pid_info);
              pid_info->powningMachine = pmachineInfo;
   
            }
         } 
        '{' data statement_decl_list '}'
					{

						$$ = $1->pmachineInfo;

				    $$->name              = $2;
 			    $$->modFlags          |= $3->modFlags;
 			    $$->machineTransition = $3->machineTransition;
           $$->native_impl       = $3->native_impl;

 					$$->data = $6;

						/* harvest the lists */
 					$$->state_list         = $7->pstate_and_event_decls->state_decls;
 					$$->event_list         = $7->pstate_and_event_decls->event_decls;
 					$$->action_list        = $7->pactions_and_transitions->action_list;
 					$$->action_info_list   = $7->pactions_and_transitions->action_info_list;
 					$$->transition_list    = $7->pactions_and_transitions->transition_list;
 					$$->transition_fn_list = $7->pactions_and_transitions->transition_fn_list;
 					$$->machine_list       = $7->pactions_and_transitions->machine_list;

						count_external_declarations  ($$->event_list,&($$->external_event_designation_count));
						count_parent_event_referenced($$->event_list,&($$->parent_event_reference_count));
						count_shared_events          ($$->event_list,&($$->shared_event_count));
						count_data_translators       ($$->event_list,&($$->data_translator_count));

						count_external_declarations  ($$->state_list,&($$->external_state_designation_count));
 					if ($$->machine_list)
						{
						    count_sub_machine_inhibitors  ($$->state_list,&($$->submachine_inhibitor_count));
						}

						if (allocateActionArray($$))
 						yyerror("out of memory");

 					enumerate_pid_list($$->state_list);
 					enumerate_pid_list($$->event_list);

						if (populate_action_array($$, yyout))
 						yyerror("Action array population failed");

           free($1);

           $2->powningMachine = pmachineInfo;

           /* reset context */
           pmachineInfo = $$->parent;
           if ($$->parent)
           {
            pID_INFO pid;

            id_list = $$->parent->id_list;

            add_id(id_list,MACHINE,$2->name,&pid);
            pid->powningMachine = $$;

           }

						#ifdef PARSER_DEBUG
						pID_INFO	pid_info;

						fprintf(yyout
                   ,"found a machine named %s\n"
                   , $$->name->name
                   );
						fprintf(yyout
								,"\twith %d events and %d states\n"
								,$$->event_list->count
								,$$->state_list->count
								);

						if ($$->modFlags & mfReentrant) {

							fprintf(yyout,"The machine is reentrant\n");

						}

						if ($$->modFlags & mfActionsReturnStates) {

							fprintf(yyout,"Actions return states\n");

						}
						else if ($$->modFlags & mfActionsReturnVoid) {

							fprintf(yyout,"Actions return void\n");

						}
						else {

							fprintf(yyout,"Actions return events\n");

						}

           if ($$->machineTransition)
           {
               fprintf(yyout
                       ,"on transition: %s\n"
                       ,$$->machineTransition->name
                       );
           }

						fprintf(yyout,"The states :\n");
 					parser_debug_print_state_list($$->state_list,yyout);

						fprintf(yyout,"The events :\n");
 					parser_debug_print_event_list($$->event_list,yyout);

           if ($$->parent_event_reference_count > 0)
           {
						   fprintf(yyout
                      ,"%d events reference the parent machine.\n"
                      , $$->parent_event_reference_count);
           }

						fprintf(yyout,"The actions :\n");
 					parser_debug_print_action_list_deep($$->action_list,$$,yyout);

           fprintf(yyout,"\nThe %d transitions :\n"
                   , $$->transition_list->count
                   );
					  parser_debug_print_transition_list($$->transition_list,yyout);

           if ($$->transition_fn_list->count)
           {
		           fprintf(yyout,"\nThe %d transition functions :\n"
                      , $$->transition_fn_list->count
                      );

								parser_debug_print_transition_fn_list($$->transition_fn_list,yyout);
           }

            if ($$->machine_list)
            {
                fprintf(yyout
                     ,"this machine has %u sub-machines\n"
                     ,$$->machine_list->count
                     );
            }

						fprintf(yyout,"\n");
						#endif

					}
	;

machine_qualifier:
        {
    					if (NULL == ($$ = (pMACHINE_QUALIFIER)calloc(1, sizeof(MACHINE_QUALIFIER))))
    						yyerror("Out of memory");
        }
    | machine_transition_decl
		     {
 					if (NULL == ($$ = (pMACHINE_QUALIFIER)calloc(1, sizeof(MACHINE_QUALIFIER))))
 						yyerror("Out of memory");

 					$$->machineTransition = $1;
 				
		     }
    | action_return_spec
		     {
 					if (NULL == ($$ = (pMACHINE_QUALIFIER)calloc(1, sizeof(MACHINE_QUALIFIER))))
 						yyerror("Out of memory");

 					$$->modFlags = $1;
		     }
    | native_impl
		     {
 					if (NULL == ($$ = (pMACHINE_QUALIFIER)calloc(1, sizeof(MACHINE_QUALIFIER))))
 						yyerror("Out of memory");

 					$$->native_impl = $1;
		     }
    | machine_qualifier machine_transition_decl
		     {
           if ($1->machineTransition)
             yyerror("only one machine transition declaration allowed per machine");

 					$1->machineTransition = $2;

           $$ = $1;
		     }
    | machine_qualifier action_return_spec
		     {
           if ($1->modFlags != mfNone)
             yyerror("only one action return spec allowed per machine");

 					$1->modFlags          = $2;

           $$ = $1;
		     }
    | machine_qualifier native_impl
		     {
           if ($1->native_impl)
             yyerror("only one native implementation allowed per machine");

           $1->native_impl = $2;

           $$ = $1;
		     }
    ;

machine_transition_decl: ON TRANSITION_KEY ID ';'
    {
 	 		$$ = $3;
    }
    ;

action_return_spec: 
	ACTIONS RETURN EVENTS ';'
        {
						pID_INFO pid_info;
           /* note that this is not added to the machine event list; it is here only to be
           found as an event id for return decls.
           */
						add_id(id_list, EVENT,"noEvent",&pid_info);
           pid_info->powningMachine = pmachineInfo;

 					$$ = 0;
        }
	| ACTIONS RETURN STATES ';'
					{
 					$$ = mfActionsReturnStates;
					}
	| ACTIONS RETURN VOID ';'
					{
						$$ = mfActionsReturnVoid;
					}
	;

machine_modifier:
	{
 		$$ = 0;
	}
	| machine_modifier REENTRANT
					{
						$$ = ($1 | mfReentrant);
					}
	;

statement_decl_list:	state_and_event_decls actions_and_transitions
	{
 	if (NULL == ($$ = (pSTATEMENT_DECL_LIST)calloc(1, sizeof(STATEMENT_DECL_LIST))))
 		yyerror("Out of memory");

 	$$->pstate_and_event_decls   = $1;
 	$$->pactions_and_transitions = $2;

	}
	;

machine_list:
    machine
    {
        if (NULL == ($$ = init_list()))
            yyerror("out of memory");

        if (NULL == add_to_list($$,$1))
            yyerror("out of memory");

    }
    | machine_list machine
    {

        $$ = $1;

        if (NULL == add_to_list($$,$2))
            yyerror("out of memory");

    }
    ;

actions_and_transitions: 
   machine_list
		{
 		if (NULL == ($$ = (pACTIONS_AND_TRANSITIONS)calloc(1,sizeof(ACTIONS_AND_TRANSITIONS))))
 			yyerror("out of memory");


 		if (NULL == ($$->action_list = init_list()))
	 			yyerror("out of memory");

 		if (NULL == ($$->action_info_list = init_list()))
 			yyerror("out of memory");

 		if (NULL == ($$->transition_list = init_list()))
 			yyerror("out of memory");

 		if (NULL == ($$->transition_fn_list = init_list()))
 			yyerror("out of memory");

    $$->machine_list = $1;

		}
   | action_decl
		{
 		if (NULL == ($$ = (pACTIONS_AND_TRANSITIONS)calloc(1,sizeof(ACTIONS_AND_TRANSITIONS))))
 			yyerror("out of memory");

			$$->action_list        = $1->action_list        ? $1->action_list        : init_list();
			$$->action_info_list   = $1->action_info_list   ? $1->action_info_list   : init_list();
			$$->transition_list    = $1->transition_list    ? $1->transition_list    : init_list();
			$$->transition_fn_list = $1->transition_fn_list ? $1->transition_fn_list : init_list();

			if (
 				   (NULL == $$->action_list)
 				|| (NULL == $$->action_info_list)
 				|| (NULL == $$->transition_list)
 				|| (NULL == $$->transition_fn_list)
					)
 				yyerror("out of memory");

 		free($1);
		}
	| transition_matrix_list
	  {
 		if (NULL == ($$ = (pACTIONS_AND_TRANSITIONS)calloc(1,sizeof(ACTIONS_AND_TRANSITIONS))))
 			yyerror("out of memory");

 		if (NULL == ($$->action_list = init_list()))
	 			yyerror("out of memory");

 		if (NULL == ($$->action_info_list = init_list()))
 			yyerror("out of memory");

 		if (NULL == ($$->transition_list = init_list()))
 			yyerror("out of memory");

 		if (NULL == ($$->transition_fn_list = init_list()))
 			yyerror("out of memory");

 		if (NULL == ($$->machine_list = init_list()))
 			yyerror("out of memory");

			if (NULL == add_unique_to_list($$->action_list, $1->action))
 				yyerror("out of memory");

			if (NULL == add_to_list($$->action_info_list, $1))
 			yyerror("out of memory");

			if ($1->transition)
			{
				switch ($1->transition->type)
				{
 				case STATE:
						if (NULL == add_unique_to_list($$->transition_list, $1->transition))
 						yyerror("out of memory");
 					break;

 				case TRANSITION_FN:
						if (NULL == add_unique_to_list($$->transition_fn_list, $1->transition))
 						yyerror("out of memory");
 					break;

					//there are no other choices
				}
			}
	  }
 | action_return_decl
	  {
 		if (NULL == ($$ = (pACTIONS_AND_TRANSITIONS)calloc(1,sizeof(ACTIONS_AND_TRANSITIONS))))
 			yyerror("out of memory");

 		if (NULL == ($$->action_list = init_list()))
	 			yyerror("out of memory");

 		if (NULL == ($$->action_info_list = init_list()))
 			yyerror("out of memory");

 		if (NULL == ($$->transition_list = init_list()))
 			yyerror("out of memory");

 		if (NULL == ($$->transition_fn_list = init_list()))
 			yyerror("out of memory");

 		if (NULL == ($$->machine_list = init_list()))
 			yyerror("out of memory");

   }
 | transition_fn_return_decl
   {
 		if (NULL == ($$ = (pACTIONS_AND_TRANSITIONS)calloc(1,sizeof(ACTIONS_AND_TRANSITIONS))))
 			yyerror("out of memory");

 		if (NULL == ($$->action_list = init_list()))
	 			yyerror("out of memory");

 		if (NULL == ($$->action_info_list = init_list()))
 			yyerror("out of memory");

 		if (NULL == ($$->transition_list = init_list()))
 			yyerror("out of memory");

 		if (NULL == ($$->transition_fn_list = init_list()))
 			yyerror("out of memory");

 		if (NULL == ($$->machine_list = init_list()))
 			yyerror("out of memory");

   }
 | actions_and_transitions action_decl
	  {
 		$$ = $1;

 		move_list_unique($1->action_list, $2->action_list);
 		free_list($2->action_list);

 		move_list($1->action_info_list, $2->action_info_list);
 		free($2->action_info_list);

 		move_list_unique($1->transition_list, $2->transition_list);
 		free_list($2->transition_list);

 		move_list_unique($1->transition_fn_list, $2->transition_fn_list);
 		free_list($2->transition_fn_list);

 		free($2);
	  }
 | actions_and_transitions transition_matrix_list
	  {
 		$$=$1;

			if (NULL == add_unique_to_list($$->action_list, $2->action))
 				yyerror("out of memory");

			if (NULL == add_to_list($$->action_info_list, $2))
 			yyerror("out of memory");

			if ($2->transition)
			{
				switch ($2->transition->type)
				{
 				case STATE:
						if (NULL == add_unique_to_list($$->transition_list, $2->transition))
 						yyerror("out of memory");
 					break;

 				case TRANSITION_FN:
						if (NULL == add_unique_to_list($$->transition_fn_list, $2->transition))
 						yyerror("out of memory");
 					break;

					//there are no other choices
				}
			}
	  }
 | actions_and_transitions action_return_decl
	  {
			$$ = $1;
	  }
 | actions_and_transitions transition_fn_return_decl
	  {
			$$ = $1;
	  }
   /* note that machines must precede actions and transitions */
	;

transition_matrix_list: transition_matrix
					{
						$$ = $1;
					}
	;

transition_matrix:	TRANSITION_KEY matrix STATE ';'
					{

						pID_INFO pid_info;

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found a transition matrix\n");
						#endif

						//first, we have to add an id_info struct to the id list
						//we treat it as a "null action"
						add_id(id_list, ACTION,"",&pid_info);
           pid_info->powningMachine = pmachineInfo;

						//second, we grab a struct to hold the info
						if (($$ = (pACTION_INFO)calloc(1,sizeof(ACTION_INFO))) == NULL)

							yyerror("out of memory");

						$$->action     = pid_info;
						$$->matrix     = $2;
						$$->transition = $3;

						$$->nextAction = pid_info->actionInfo;
						pid_info->actionInfo = $$;

					}
   | TRANSITION_KEY matrix ID ';'
					{

						pID_INFO pid_info;

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found a transition matrix with new transition function\n");
						#endif

           set_id_type($3,TRANSITION_FN);

						//first, we have to add an id_info struct to the id list
						//we treat it as a "null action"
						add_id(id_list, ACTION,"",&pid_info);
           pid_info->powningMachine = pmachineInfo;

						//second, we grab a struct to hold the info
						if (($$ = (pACTION_INFO)calloc(1,sizeof(ACTION_INFO))) == NULL)

							yyerror("out of memory");

						$$->action     = pid_info;
						$$->matrix     = $2;
						$$->transition = $3;

						$$->nextAction = pid_info->actionInfo;
						pid_info->actionInfo = $$;

					}
   | TRANSITION_KEY matrix TRANSITION_FN ';'
					{

						pID_INFO pid_info;

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found a transition matrix with known transition function\n");
						#endif

						//first, we have to add an id_info struct to the id list
						//we treat it as a "null action"
						add_id(id_list, ACTION,"",&pid_info);
           pid_info->powningMachine = pmachineInfo;

						//second, we grab a struct to hold the info
						if (($$ = (pACTION_INFO)calloc(1,sizeof(ACTION_INFO))) == NULL)

							yyerror("out of memory");

						$$->action     = pid_info;
						$$->matrix     = $2;
						$$->transition = $3;

						$$->nextAction = pid_info->actionInfo;
						pid_info->actionInfo = $$;

        }

	;

action_decl:	action_decl_list ';'
					{

						#ifdef PARSER_DEBUG
						/* we should now be able to print a list of the actions */
						pID_INFO	pid;

						fprintf(yyout,"The actions in this list:\n");
						parser_debug_print_id_list_names($1->action_list,pmachineInfo,yyout,"noAction");
						#endif

						$$ = $1;

					}
	;

action_decl_list: ACTION_KEY action 	
					{

						#ifdef PARSER_DEBUG
						fprintf(yyout,"started an action declaration\n");
						#endif

 					if (NULL == ($$ = ((pACTION_DECL) calloc(1,sizeof(ACTION_DECL)))))
 						yyerror("out of memory");

 					if (NULL == ($$->action_list = init_list()))
 						yyerror("out of memory");

 					if (NULL == ($$->action_info_list = init_list()))
 						yyerror("out of memory");

 					if (NULL == ($$->transition_list = init_list()))
 						yyerror("out of memory");

 					if (NULL == ($$->transition_fn_list = init_list()))
 						yyerror("out of memory");

						if (NULL == add_unique_to_list($$->action_list, $2->action))
 						yyerror("out of memory");

						if (NULL == add_to_list($$->action_info_list, $2))
 						yyerror("out of memory");

						if ($2->transition)
						{

							switch ($2->transition->type)
							{
 							case STATE:
									if (NULL == add_unique_to_list($$->transition_list, $2->transition))
 									yyerror("out of memory");
 								break;

 							case TRANSITION_FN:
									if (NULL == add_unique_to_list($$->transition_fn_list, $2->transition))
 									yyerror("out of memory");
 								break;

								//there are no other choices
							}

						}

					}

	| action_decl_list ',' action 
					{

						#ifdef PARSER_DEBUG
						fprintf(yyout,"added another action declarations\n");
						#endif

						$$ = $1;

						if (NULL == add_unique_to_list($$->action_list, $3->action))
 						yyerror("out of memory");

						if (NULL == add_to_list($$->action_info_list, $3))
 						yyerror("out of memory");

						if ($3->transition)
						{
							switch ($3->transition->type)
							{
 							case STATE:
									if (NULL == add_unique_to_list($$->transition_list, $3->transition))
 									yyerror("out of memory");
 								break;

 							case TRANSITION_FN:
									if (NULL == add_unique_to_list($$->transition_fn_list, $3->transition))
 									yyerror("out of memory");
 								break;

								//there are no other choices
							}

						}

					}
	;

action: action_matrix 
					{

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found an action without transition\n");
						#endif

						$$ = $1;

					}
	| action_matrix transition 	
					{

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found an action with transition\n");
						#endif

						$1->transition = $2;

						$$ = $1;

					}
	;

transition: TRANSITION_KEY STATE	
					{

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found a transition to known state\n");
						#endif

						$$ = $2;

					}
  | TRANSITION_KEY ID
					{

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found a transition with new function\n");
						#endif

           set_id_type($2,TRANSITION_FN);
						$$ = $2;

					}
  | TRANSITION_KEY TRANSITION_FN
					{

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found a transition with known function\n");
						#endif

						$$ = $2;

					}
	;

action_matrix: ID matrix
					{

						#ifdef PARSER_DEBUG
						volatile pID_INFO pid_info = $1;

						fprintf(yyout,"found an action matrix\n");
						#endif

           set_id_type($1,ACTION);
           $1->powningMachine = pmachineInfo;

						/* 
							grab an ACTION_INFO struct
						*/
						if (($$ = (pACTION_INFO)calloc(1,sizeof(ACTION_INFO))) == NULL)

							yyerror("out of memory");

						$$->action     = $1;
						$$->matrix     = $2;
						$$->transition = NULL;

						$$->nextAction = $1->actionInfo;
						$1->actionInfo = $$;

					}
   | ACTION matrix
					{

						#ifdef PARSER_DEBUG
						volatile pID_INFO pid_info = $1;

						fprintf(yyout,"found an action matrix\n");
						#endif

						/* 
							grab an ACTION_INFO struct
						*/
						if (($$ = (pACTION_INFO)calloc(1,sizeof(ACTION_INFO))) == NULL)

							yyerror("out of memory");

						$$->action     = $1;
						$$->matrix     = $2;
						$$->transition = NULL;

						$$->nextAction = $1->actionInfo;
						$1->actionInfo = $$;

					}
	;

matrix: '[' event_vector ',' state_vector ']'
     {
         #ifdef PARSER_DEBUG
         fprintf(yyout,"found a matrix\n");
         #endif

         /*
            grab a MATRIX_INFO struct
         */
						if (($$ = (pMATRIX_INFO)malloc(sizeof(MATRIX_INFO))) == NULL)

							yyerror("out of memory");

            $$->event_list = $2;
            $$->state_list = $4;

     }
 ;

state_vector: 
		'(' state_comma_list STATE ')' 

					{

						$$ = $2;

 					if (add_to_list($$,$3) == NULL)
							yyerror("out of memory");

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found a state vector\n");
 					parser_debug_print_id_list_names($$,pmachineInfo,yyout,"");
						#endif

					}
	| STATE
					{

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found a state scalar\n");
						#endif

						if (($<plist>$ = init_list()) == NULL) 
							yyerror("out of memory");

 					if (add_to_list($$,$1) == NULL)
							yyerror("out of memory");

					}
	;

state_comma_list:	STATE ','
					{

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found the beginning of a state comma list\n");
						#endif

						if (($$ = init_list()) == NULL) 
							yyerror("out of memory");

 					if (add_to_list($$,$1) == NULL)
							yyerror("out of memory");

					}
	| state_comma_list STATE ','
					{

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found the continuation of a state comma list\n");
						#endif

						$$ = $1;

 					if (add_to_list($$,$2) == NULL)
							yyerror("out of memory");

					}
	;

event_vector: '(' event_comma_list EVENT ')' 
					{

						$$ = $2;

 					if (add_to_list($$,$3) == NULL)
							yyerror("out of memory");

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found an event vector\n");
 					parser_debug_print_id_list_names($$,pmachineInfo,yyout,"");
						#endif

					}
	| EVENT
					{

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found a scalar event : %s\n",$1->name);
						#endif

						if (($<plist>$ = init_list()) == NULL) 
							yyerror("out of memory");

 					if (add_to_list($$,$1) == NULL)
							yyerror("out of memory");

					}
	;


event_comma_list:	EVENT ',' 	
					{

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found the begining of an event comma list: %s\n",$1->name);
						#endif

           if ($1->type_data.event_data.psharing_sub_machines)
             yyerror("events which are shared by sub-machines may not be part of a vector");

						/* start a list */
						if (($$ = init_list()) == NULL) 
							yyerror("out of memory");

 					if (add_to_list($$,$1) == NULL)
							yyerror("out of memory");

					}
	| event_comma_list EVENT ','
					{

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found the continuation of an event comma list. adding %s\n",$2->name);
						#endif

           if ($2->type_data.event_data.psharing_sub_machines)
             yyerror("events which are shared by sub-machines may not be part of a vector");

 					$$ = $1;

 					if (add_to_list($$,$2) == NULL)
							yyerror("out of memory");

					}
	;

state_and_event_decls: 
	  state_decl
		{
 	   if (NULL == ($$ = ((pSTATE_AND_EVENT_DECLS)calloc(1,sizeof(STATE_AND_EVENT_DECLS)))))
 		    yyerror("Out of memory");

 		 $$->state_decls = $1;
 		 $$->event_decls = init_list();
		}
 | event_decl
		{
 	   if (NULL == ($$ = ((pSTATE_AND_EVENT_DECLS)calloc(1,sizeof(STATE_AND_EVENT_DECLS)))))
 		    yyerror("Out of memory");

 		 $$->event_decls = $1;
 		 $$->state_decls = init_list();
		}
	| state_and_event_decls state_decl
	  {
 		$$ = $1;

			$$->state_decls = move_list($$->state_decls,$2);
	  }
	| state_and_event_decls event_decl
	  {
 		$$ = $1;

			$$->event_decls = move_list($$->event_decls,$2);

	  }
	;

state_decl:	state_decl_list ';'
					{

						#ifdef PARSER_DEBUG
						/* we should now be able to print a list of the states */
						fprintf(yyout,"The %d states in this list :\n",$1->count);

 					parser_debug_print_state_list($1,yyout);
						#endif

						$$ = $1;

					}
	;

state: ID
         {
 				  $$ = $1;
           set_id_type($$,STATE);
           $$->powningMachine = pmachineInfo;
         }
 	| state INHIBITS SUBMACHINES
					{
         	#ifdef PARSER_DEBUG
         	fprintf(yyout
										, "state %s inhibits submachines\n"
 									, $1->name
										);
         	#endif

 					$$ = $1;

 					$$->type_data.state_data.state_flags |= sfInibitSubMachines;

		      }
 				;

state_decl_list:	STATE_KEY state	
					{

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found the start of a state declaration list\n");
						#endif

 					if (NULL == ($$ = init_list()))
 						yyerror("Out of memory");


 					if (NULL == (add_to_list($$,$2)))
 						yyerror("Out of memory");

					}
	| state_decl_list ',' state 
					{

						#ifdef PARSER_DEBUG
						fprintf(yyout,"added another id to the state declaration list\n");
						#endif

						$$ = $1;

           set_id_type($3,STATE);
           $3->powningMachine = pmachineInfo;

 					if (NULL == (add_to_list($$,$3)))
 						yyerror("Out of memory");

					}
	;
 
event_decl:	event_decl_list ';'
					{

						$$ = $1;

						#ifdef PARSER_DEBUG
						/* we should now be able to print a list of the events */
						fprintf(yyout,"The %d events in this list:\n",$$->count);
 					parser_debug_print_event_list($$,yyout);
						#endif

					}
	;

parent_namespace: PARENT NAMESPACE
  {
    if (!pmachineInfo->parent)
        yyerror("parent namespace invoked in top-level machine");

    id_list = pmachineInfo->parent->id_list;
  }
  ;

event_data: { $$ = NULL; }
  | TRANSLATOR_KEY ID
     {
        $$ = $2;
        
        #ifdef PARSER_DEBUG
        fprintf(yyout,"found a data translator: %s\n", $2->name);
        #endif
     }
    
event_decl_list:	EVENT_KEY ID external_designation
					{

 					if (NULL == ($$ = init_list()))
 						yyerror("Out of memory");

           set_id_type($2,EVENT);
           $2->type_data.event_data.externalDesignation = $3;
           $2->powningMachine = pmachineInfo;

 					if (NULL == (add_to_list($$,$2)))
 						yyerror("Out of memory");

					}
 | EVENT_KEY parent_namespace EVENT event_data
					{
            #ifdef PARSER_DEBUG
            fprintf(yyout,"Found a namespace event reference\n");
            #endif

 					if (NULL == ($$ = init_list()))
 						yyerror("Out of memory");

           pID_INFO pid;

            id_list = pmachineInfo->id_list;

           if (!add_unique_id(id_list,EVENT,$3->name,&pid))
 						yyerror("Cannot reference parent event twice");

           pid->type_data.event_data.data_translator     = $4;
           pid->type_data.event_data.shared_with_parent  = true;
           pid->powningMachine = pmachineInfo;

 					if (NULL == (add_to_list($$,pid)))
 						yyerror("Out of memory");

           /* let the parent machine event know about us */
           if (NULL == $3->type_data.event_data.psharing_sub_machines)
           {
 					    if (NULL == ($3->type_data.event_data.psharing_sub_machines = init_list()))
 						      yyerror("Out of memory");
           }

           if (NULL == add_to_list($3->type_data.event_data.psharing_sub_machines, pmachineInfo))
               yyerror("Out of memory");

					}
	| event_decl_list ',' ID external_designation
					{

						#ifdef PARSER_DEBUG
						fprintf(yyout,"added another id to the event declaration list\n");
						#endif

						$$ = $1;

           set_id_type($3,EVENT);
           $3->type_data.event_data.externalDesignation = $4;
           $3->powningMachine = pmachineInfo;

 					if (NULL == (add_to_list($$,$3)))
 						yyerror("Out of memory");

					}
 | event_decl_list ',' parent_namespace EVENT event_data
					{
            #ifdef PARSER_DEBUG
            fprintf(yyout,"added another namespace event reference to the declaration list\n");
            #endif

            $$ = $1;

           pID_INFO pid;

            id_list = pmachineInfo->id_list;

           if (!add_unique_id(id_list,EVENT,$4->name,&pid))
 						yyerror("Cannot reference parent event twice");

           pid->type_data.event_data.data_translator     = $5;
           pid->type_data.event_data.shared_with_parent  = true;
           pid->powningMachine = pmachineInfo;

 					if (NULL == (add_to_list($$,pid)))
 						yyerror("Out of memory");

           /* let the parent machine event know about us */
           if (NULL == $4->type_data.event_data.psharing_sub_machines)
           {
 					    if (NULL == ($4->type_data.event_data.psharing_sub_machines = init_list()))
 						      yyerror("Out of memory");
           }

           if (NULL == add_to_list($4->type_data.event_data.psharing_sub_machines, pmachineInfo))
               yyerror("Out of memory");

					}
	;

external_designation : {
           $$ = NULL;
     }
 | '=' ID
 {
           #ifdef PARSER_DEBUG
           fprintf(yyout,"External designation = %s\n",$2->name);
           #endif
            $$ = $2;
 }
 ;

native:	{
						$$ = NULL;
			}
	| NATIVE_KEY NATIVE_BLOCK
					{
						#ifdef PARSER_DEBUG
						fprintf(yyout,"Native\n%s\n",$2);
						#else
						$$ = $2;
						#endif
					}

	;
 
native_impl: NATIVE_KEY IMPLEMENTATION_KEY NATIVE_BLOCK
					{
						#ifdef PARSER_DEBUG
						fprintf(yyout,"Native implementation\n%s\n",$3);
						#else
						$$ = $3;
						#endif
					}

	;
 
data:	{
						$$ = NULL;
			}
	| DATA_KEY
					{
						#ifdef PARSER_DEBUG
						fprintf(yyout,"Data\n%s\n",$1);
						#else
						$$ = $1;
						#endif
					}

	;
 
namespace: parent_namespace
  | MACHINE NAMESPACE
  {
    if (pmachineInfo->parent)
        yyerror("sub-machine namespace invoked in sub-machine");

    id_list = $1->powningMachine->id_list;
  }
  ;

namespace_event_ref: namespace EVENT
  {
    #ifdef PARSER_DEBUG
    fprintf(yyout,"Found a namespace event reference\n");
    #endif

    $$ = $2;
    id_list = pmachineInfo->id_list;
  }
  ;

returns_comma_list: namespace_event_ref ','
    {
						/* start a list */
						if (($$ = init_list()) == NULL) 
							yyerror("out of memory");

 					if (add_to_list($$,$1) == NULL)
							yyerror("out of memory");

    }
    | EVENT ','
    {
						/* start a list */
						if (($$ = init_list()) == NULL) 
							yyerror("out of memory");

 					if (add_to_list($$,$1) == NULL)
							yyerror("out of memory");

    }
    | returns_comma_list namespace_event_ref ','
    {
           $$ = $1;

 					if (add_to_list($$,$2) == NULL)
							yyerror("out of memory");

    }
    | returns_comma_list EVENT ','
    {
           $$ = $1;

 					if (add_to_list($$,$2) == NULL)
							yyerror("out of memory");

    }
    ;

action_return_decl: 
  ACTION RETURNS returns_comma_list EVENT ';'
  {
    #ifdef PARSER_DEBUG
    fprintf(yyout,"Found an action return declaration\n");
    #endif

 	 if (!$1->action_returns_decl)
		 {
		    if (($1->action_returns_decl = init_list()) == NULL) 
				   yyerror("out of memory");
		 }

			if (add_to_list($3,$4) == NULL)
				 yyerror("out of memory");

			move_list_unique($1->action_returns_decl, $3);
 		free_list($3);

  }
  | ACTION RETURNS returns_comma_list namespace_event_ref ';'
  {
    #ifdef PARSER_DEBUG
    fprintf(yyout,"Found an action return declaration\n");
    #endif

 	 if (!$1->action_returns_decl)
		 {
		    if (($1->action_returns_decl = init_list()) == NULL) 
				   yyerror("out of memory");
		 }

			if (add_to_list($3,$4) == NULL)
				 yyerror("out of memory");

			move_list_unique($1->action_returns_decl, $3);
 		free_list($3);

  }
  | ACTION RETURNS EVENT ';'
  {
    #ifdef PARSER_DEBUG
    fprintf(yyout,"Found an action return declaration\n");
    #endif

 	 if (!$1->action_returns_decl)
		 {
		    if (($1->action_returns_decl = init_list()) == NULL) 
				   yyerror("out of memory");
		 }

		 if (add_unique_to_list($1->action_returns_decl,$3) == NULL)
				yyerror("out of memory");

  }
  | ACTION RETURNS namespace_event_ref ';'
  {
    #ifdef PARSER_DEBUG
    fprintf(yyout,"Found an action return declaration\n");
    #endif

 	 if (!$1->action_returns_decl)
		 {
		    if (($1->action_returns_decl = init_list()) == NULL) 
				   yyerror("out of memory");
		 }

		 if (add_unique_to_list($1->action_returns_decl,$3) == NULL)
				yyerror("out of memory");

  }
  | ACTION RETURNS state_comma_list STATE ';'
  {
    #ifdef PARSER_DEBUG
    fprintf(yyout,"Found an action return declaration\n");
    #endif

			if (add_to_list($3, $4) == NULL)
				yyerror("out of memory");

 	 if (!$1->action_returns_decl)
		 {
		    if (($1->action_returns_decl = init_list()) == NULL) 
				   yyerror("out of memory");
		 }

			move_list_unique($1->action_returns_decl, $3);
 		free_list($3);

  }
  | ACTION RETURNS STATE ';'
  {
    #ifdef PARSER_DEBUG
    fprintf(yyout,"Found an action return declaration\n");
    #endif

		 if (add_to_list($1->action_returns_decl,$3) == NULL)
				yyerror("out of memory");

 	 if (!$1->action_returns_decl)
		 {
		    if (($1->action_returns_decl = init_list()) == NULL) 
				   yyerror("out of memory");
		 }

			add_unique_to_list($1->action_returns_decl, $3);

  }
  ;
 
transition_fn_return_decl:
  TRANSITION_FN RETURNS state_comma_list STATE ';'
  {
			if (add_unique_to_list($3, $4) == NULL)
 			yyerror("out of memory");

 	 if (!$1->transition_fn_returns_decl)
		 {
		    if (($1->transition_fn_returns_decl = init_list()) == NULL) 
				   yyerror("out of memory");
		 }

			move_list_unique($1->transition_fn_returns_decl, $3);
 		free_list($3);

    #ifdef PARSER_DEBUG
    fprintf(yyout,"Found a transition_fn return declaration\n");
    #endif

  }
  | TRANSITION_FN RETURNS STATE ';'
  {
    #ifdef PARSER_DEBUG
    fprintf(yyout,"Found a transition_fn return declaration\n");
    #endif

 	 if (!$1->transition_fn_returns_decl)
		 {
		    if (($1->transition_fn_returns_decl = init_list()) == NULL) 
				   yyerror("out of memory");
		 }

			add_unique_to_list($1->transition_fn_returns_decl, $3);

  }
  ;
 
%%

#if defined(CYGWIN) || defined (LINUX)
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#endif

#ifdef VS
#include "vsstd.h"
#include "vsstring.h"
#endif

void usage(void);
char *dotfsm = ".fsm";

/*
  use these as needed for the val param of the long options
  when flag is set to &longval.  Otherwise, set the val param to the
  short option character and set flag to NULL.
*/
typedef enum {
    lo_css_content_filename
    , lo_css_content_internal
    , lo_weak_fns
    , lo_core_logging
    , lo_include_svg_img
} LONG_OPTIONS;

int longindex = 0;
int longval;
const struct option longopts[] =
{
    {
        .name      = "help"
        , .has_arg = no_argument
        , .flag    = NULL
        , .val     = '?'
    }
    , {
        .name      = "css-content-filename"
        , .has_arg = required_argument
        , .flag    = &longval
        , .val     = lo_css_content_filename
    }
    , {
        .name      = "css-content-internal"
        , .has_arg = required_argument
        , .flag    = &longval
        , .val     = lo_css_content_internal
    }
    , {
        .name      = "generate-weak-fns"
        , .has_arg = required_argument
        , .flag    = &longval
        , .val     = lo_weak_fns
    }
    , {
        .name      = "core-logging-only"
        , .has_arg = required_argument
        , .flag    = &longval
        , .val     = lo_core_logging
    }
    , {
        .name      = "include-svg-img"
        , .has_arg = required_argument
        , .flag    = &longval
        , .val     = lo_include_svg_img
    }
    , {0}
};

int main(int argc, char **argv)
{

	int		c;
	char	*cp,*cp1;
	char  *outFileBase = 0;

	me = argv[0];

	/* special case the single '?' */
	if (argc == 2 && argv[1][0] == '?') {

		usage();
		return (1);

	}

#ifndef PARSER_DEBUG
	/* default to writing a c machine */
	pfsmog = pCMachineWriter;

	while ((c = getopt_long(argc,argv,"vht:o:i:c", longopts, &longindex)) != -1) {

		switch(c) {

     case 0:

        switch (longval)
        {
            case lo_weak_fns:
                generate_weak_fns 
                    = !strcmp(optarg,"true") ? true : false;
                break;
            case lo_core_logging:
                core_logging_only 
                    = !strcmp(optarg,"true") ? true : false;
                break;
            case lo_include_svg_img:
                include_svg_img
                    = !strcmp(optarg,"true") ? true : false;
                break;
            case lo_css_content_filename:
                css_content_filename = optarg;
                break;
            case lo_css_content_internal:
                css_content_internal
                    = !strcmp(optarg,"true") ? true : false;
                break;
            default:
                usage();
                return(0);
                break;
        }
        break;

			case 'h':
				usage();
				return (1);

			case 't':

				switch (optarg[0]) {

					case 'c':

						pfsmog = pCMachineWriter;
						break;

					case 'h':

						pfsmog = pHTMLMachineWriter;
						break;

         case 's':
           pfsmog = pCSwitchMachineWriter;
						break;

         case 'p':
           pfsmog = pPlantUMLMachineWriter;
						break;

					default:

						usage();
						return (1);

				}
				break;

     case 'i':
        if ('0' == optarg[0])
        {
            generate_instance = false;
        }
        break;

     case 'c':
        compact_action_array = true;
        break;

			case 'o':

				outFileBase = optarg;
				break;

     case 'v':

        fprintf(stdout,"version %s\n",rev_string);
        return (0);

			case '?':
			case ':':

				usage();
				return (1);

		}

	}
#endif

	if (argv[optind]) {

		cp = strdup(argv[optind]);

		/* find the extension */
		cp1 = rindex(cp,'.');
		if (!cp1) {

			usage();
			exit(1);

		}
		else {

			if (strcmp(cp1,dotfsm))	{

				usage();
				return (1);

			}

		}

		if ((yyin = openFile(argv[optind],"r")) == NULL) {

			return 1;

		}

		/* get the base file name */
		if (!outFileBase) {
			/* use the base input file name */
			*cp1 = 0;
			cwk_path_get_basename(cp, (const char **)&outFileBase, NULL);
		}

		#ifndef PARSER_DEBUG
		if (!(*pfsmog->initOutput)(pfsmog,outFileBase)) {
		#endif

			yyparse();

		#ifndef PARSER_DEBUG
			(*pfsmog->closeOutput)(pfsmog,good);

		}
		#endif

		fclose(yyin);

		return (!good);

	}
	else {

		usage();
		return (!good);

	}

}

void yyerror(char *s)
{

  fprintf(stderr,"%s: %s\n",me,s);
	fprintf(stderr,"\tline %d : %s\n",lineno,yytext);

 #ifndef PARSER_DEBUG
	good = 0;
	#else
	//always return good so that the makefile can pick up stderr
	//the makefile will detect differences between actual and expected outcome;
	good = 1;
	#endif

}

void usage(void)
{

	fprintf(stdout,"Usage : %s [-tc|s|h|p] filename, where filename ends with '.fsm'\n",me);
	fprintf(stdout,"\t and where 'c' gets you c code output based on an event/state table,\n");
	fprintf(stdout,"\t 's' gets you c code output with individual state functions using switch constructions,\n");
	fprintf(stdout,"\t and 'h' gets you html output\n");
	fprintf(stdout,"\t and 'p' gets you PlantUML output\n");
 fprintf(stdout,"\t-i0 inhibits the creation of a machine instance\n");
 fprintf(stdout,"\t\tany other argument to 'i' allows the creation of an instance;\n");
 fprintf(stdout,"\t\tthis is the default\n");
 fprintf(stdout,"\t-c will create a more compact event/state table when -tc is used\n");
 fprintf(stdout,"\t\twith machines having actions which return states\n");
 fprintf(stdout,"\t--generate-weak-fns=false suppresses the generation of weak function stubs.\n");
 fprintf(stdout,"\t--core-logging-only=true suppresses the generation of debug log messages in all but the core FSM function.\n");
 fprintf(stdout,"\t--include-svg-img=true adds <img/> tag referencing <filename>.svg to include an image at the top of the web page.\n");
 fprintf(stdout,"\t--css-content-internal=true puts the CSS directly into the html.\n");
 fprintf(stdout,"\t--css-content-filename=<filename> uses the named file for the css citation, or\n");
 fprintf(stdout,"\t\tfor the content copy.\n");
 fprintf(stdout,"\t-v prints the version and exits\n");
	
}

