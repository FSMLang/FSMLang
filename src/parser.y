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
#include "fsm_c_event_table.h"
#include "fsm_html.h"
#include "fsm_plantuml.h"
#include "fsm_statistics.h"
#include "fsm_c_event_xref.h"
#include "fsm_rst.h"

#include "list.h"

int lineno=1;
pLIST id_list = NULL;

//did we parse correctly?
int good=1;

extern char	*yytext;
extern FILE	*yyin, *yyout;

extern int yylex(void);

char *rindex(const char *str,int c);

bool html_help = false;

pMACHINE_INFO               pmachineInfo = NULL;
pFSMOutputGenerator         pfsmog       = NULL;
fpFSMOutputGeneratorFactory fpfsmogf     = NULL;
pSTATE_AND_EVENT_DECLS      psedecls     = NULL;

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
 pDATA_FIELD              pdata_field;
 pDATA_TYPE_STRUCT        pdata_type_struct;
 pUSER_EVENT_DATA         puser_event_data;
 pNATIVE_INFO             pnative_info;
}

%token ON NAMESPACE STATE_KEY EVENT_KEY PROLOGUE_KEY EPILOGUE_KEY
%token DATA_KEY TRANSLATOR_KEY MACHINE_KEY
%token REENTRANT ACTIONS RETURN STATES EVENTS RETURNS EXTERNAL VOID
%token IMPLEMENTATION_KEY INHIBITS SUBMACHINES ALL ENTRY EXIT STRUCT_KEY UNION_KEY
%token SEQUENCE_KEY

%token <charData> ACTION_KEY 
%token <charData> TRANSITION_KEY 

%token <charData>	PARENT
%token <charData> NATIVE_KEY
%token <charData> NATIVE_BLOCK
%token <pid_info> MACHINE
%token <pid_info> STATE
%token <pid_info> EVENT
%token <pid_info> ACTION
%token <pid_info> TRANSITION_FN
%token <pid_info> ID
%token <charData> NUMERIC_STRING
%token <pid_info> TYPE_NAME

%type <pnative_info>             native
%type <pnative_info>             native_impl
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
%type <action_info>              transition_matrix_start
%type <action_info>              transition_matrix_list
%type <plist>                    machine_data
%type <plist>                    data_block
%type <pdata_field>              data_field
%type <plist>                    data_fields
%type <charData>                 data_field_dimension
%type <charData>                 native_prologue
%type <charData>                 native_impl_prologue
%type <charData>                 native_epilogue
%type <charData>                 native_impl_epilogue
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
%type <puser_event_data>         user_event_data
%type <pid_info>                 state
%type <charData>                 parent_namespace
%type <pdata_type_struct>        data_type
%type <plist>                    sequence
%type <plist>                    sequences

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
				if ($1)
				{
					$$->pmachineInfo->native_prologue = $1->prologue;
					$$->pmachineInfo->native_epilogue = $1->epilogue;
				}

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
   						pID_INFO pid_event;
							/* note that this is not added to the machine event list;
								 it is here only to be found as an event id for return
								 decls.
              */
   						add_id(id_list, EVENT,"noEvent",&pid_event);
              pid_event->powningMachine = pmachineInfo;
            }

						/* as with 'noEvent', we need this to support return decls */
						pID_INFO pid_state;
						add_id(id_list, STATE, "noTransition",&pid_state);
						pid_state->powningMachine = pmachineInfo;

         } 
        '{' statement_decl_list '}'
					{

						$$                     = $1->pmachineInfo;

				    $$->name               = $2;
 			      $$->modFlags          |= $3->modFlags;
 			      $$->machineTransition  = $3->machineTransition;
            $$->native_impl_prologue = $3->native_impl_prologue;
            $$->native_impl_epilogue = $3->native_impl_epilogue;


						/* harvest the lists */
 					$$->data               = $6->data;
 					$$->state_list         = $6->pstate_and_event_decls->state_decls;
 					$$->event_list         = $6->pstate_and_event_decls->event_decls;
 					$$->action_list        = $6->pactions_and_transitions->action_list;
 					$$->action_info_list   = $6->pactions_and_transitions->action_info_list;
 					$$->transition_list    = $6->pactions_and_transitions->transition_list;
 					$$->transition_fn_list = $6->pactions_and_transitions->transition_fn_list;
 					$$->machine_list       = $6->pactions_and_transitions->machine_list;
					$$->sequences          = $6->sequences;

						count_external_declarations     ($$->event_list,&($$->external_event_designation_count));
						count_parent_event_referenced   ($$->event_list,&($$->parent_event_reference_count));
						count_shared_events             ($$->event_list,&($$->shared_event_count));
						count_event_user_data_attributes($$->event_list
																						 ,&($$->data_translator_count)
																						 ,&($$->data_block_count)
																						 );

 					/* sanity checks */
 					if ($$->parent && $$->data_block_count && !output_generated_file_names_only)
					{
 					   yyerror("event user data not allowed in sub-machines");
					}


					count_external_declarations($$->state_list
																				,&($$->external_state_designation_count)
																				);
 					count_states_with_entry_exit_fns($$->state_list
																					 ,&($$->states_with_entry_fns_count)
																					 ,&($$->states_with_exit_fns_count)
																					 );

          $$->executes_fns_on_state_transitions = (
                                                   (($$->states_with_entry_fns_count + $$->states_with_exit_fns_count) > 0)
                                                   || ($$->machineTransition != NULL)
                                                   );
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

					 count_states_with_zero_events($$->state_list
																				 , &($$->states_with_zero_events)
																				 );
					 count_states_with_one_event($$->state_list
																			 , &($$->states_with_one_event)
																			 );
					 count_states_with_no_way_in($$->state_list
																			 , &($$->states_with_no_way_in)
																			 );
					 count_states_with_no_way_out($$->state_list
																			 , &($$->states_with_no_way_out)
																			 );
					 count_events_with_zero_handlers($$->event_list
																					 , &($$->events_with_zero_handlers)
																					 );
					 count_events_with_one_handler($$->event_list
																				 , &($$->events_with_one_handler)
																				 );

					 compute_event_and_state_density_pct($$);

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

					 if ($$->sequences)
					 {
						 fprintf(yyout
										 , "There %s %d event sequence%s given:\n"
										 , $$->sequences->count == 1 ? "is" : "are"
										 , $$->sequences->count
										 , $$->sequences->count == 1 ? "" : "s"
										 );

						 parser_debug_print_event_sequences($$->sequences, yyout);

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

           if ($$->data)
           {
             fprintf(yyout
                     , "this machine has data\n"
                     );

             parser_debug_print_data_block($$->data, yyout);

           }

            if ($$->machine_list)
            {
                fprintf(yyout
                     ,"this machine has %u sub-machines\n"
                     ,$$->machine_list->count
                     );

								fprintf(yyout
												, "the sub-machine depth is %u\n"
												, $$->sub_machine_depth
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

 					$$->native_impl_prologue = $1->prologue;
 					$$->native_impl_epilogue = $1->epilogue;
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
           if ($1->native_impl_prologue)
             yyerror("only one native_prologue implementation allowed per machine");

           if ($1->native_impl_epilogue)
             yyerror("only one native_epilogue implementation allowed per machine");

           $1->native_impl_prologue = $2->prologue;
           $1->native_impl_epilogue = $2->epilogue;

           $$ = $1;
		     }
    ;

machine_transition_decl: ON TRANSITION_KEY ID ';'
    {
 	 		$$          = $3;
 			$3->docCmnt = $2;
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
	| REENTRANT
					{
						$$ = mfReentrant;
					}
	;

statement_decl_list:	state_and_event_decls actions_and_transitions
   	{
    	if (NULL == ($$ = (pSTATEMENT_DECL_LIST)calloc(1, sizeof(STATEMENT_DECL_LIST))))
    		yyerror("Out of memory");
   
    	$$->pstate_and_event_decls   = $1;
    	$$->pactions_and_transitions = $2;
   
   	}
 | machine_data state_and_event_decls actions_and_transitions
   	{
    	if (NULL == ($$ = (pSTATEMENT_DECL_LIST)calloc(1, sizeof(STATEMENT_DECL_LIST))))
    		yyerror("Out of memory");
   
      $$->data                     = $1;
    	$$->pstate_and_event_decls   = $2;
    	$$->pactions_and_transitions = $3;
   
   	}
 | statement_decl_list sequences
 {
	#ifdef PARSER_DEBUG
	fprintf(yyout
					,"Found %d sequence%s.\n"
					, $2->count
					, $2->count == 1 ? "" : "s"
					);
	#endif

	$$ = $1;
	$$->sequences = $2;

 }
	;

sequence: SEQUENCE_KEY event_comma_list EVENT ';'
	{
		#ifdef PARSER_DEBUG
		fprintf(yyout, "Found an event sequence\n");
		#endif

		add_to_list($2, $3);

		$$ = $2;
	}
	;

sequences: 
	sequence 
	{
		if (($$ = init_list()) == NULL)
    		yyerror("Out of memory");

		add_to_list($$, $1);
	}
	| sequences sequence
	{
		#ifdef PARSER_DEBUG
		fprintf(yyout, "Found another event sequence\n");
		#endif

		add_to_list($1, $2);

		$$ = $1;

	}
	;


machine_list:
    machine
    {
        if (NULL == ($$ = init_list()))
            yyerror("out of memory");

        if (NULL == add_to_list($$,$1))
            yyerror("out of memory");

				/* Bump the depth count. The function is a no-op if we're at the top. */
				increase_sub_machine_depth($1->parent);

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

transition_matrix_start: matrix TRANSITION_KEY
        {
						pID_INFO pid_info;

						//first, we have to add an id_info struct to the id list
						//we treat it as a "null action"
						add_id(id_list, ACTION,"",&pid_info);
           pid_info->powningMachine = pmachineInfo;

						//second, we grab a struct to hold the info
						if (($$ = (pACTION_INFO)calloc(1,sizeof(ACTION_INFO))) == NULL)

							yyerror("out of memory");

						$$->matrix     = $1;
						$$->action     = pid_info;
 					$$->docCmnt    = $2;

						$$->nextAction = pid_info->type_data.action_data.actionInfo;
						pid_info->type_data.action_data.actionInfo = $$;

        }
    | TRANSITION_KEY matrix
        {
						pID_INFO pid_info;

						//first, we have to add an id_info struct to the id list
						//we treat it as a "null action"
						add_id(id_list, ACTION,"",&pid_info);
           pid_info->powningMachine = pmachineInfo;

						//second, we grab a struct to hold the info
						if (($$ = (pACTION_INFO)calloc(1,sizeof(ACTION_INFO))) == NULL)

							yyerror("out of memory");

						$$->matrix     = $2;
						$$->action     = pid_info;
 					$$->docCmnt    = $1;

						$$->nextAction = pid_info->type_data.action_data.actionInfo;
						pid_info->type_data.action_data.actionInfo = $$;

        }
    ;

transition_matrix:	transition_matrix_start STATE ';'
					{

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found a transition matrix\n");
						#endif

           $$ = $1;

						$$->transition = $2;

					}
   | transition_matrix_start ID ';'
					{

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found a transition matrix with new transition function\n");
						#endif

           set_id_type($2,TRANSITION_FN);

						$$->transition = $2;

					}
   | transition_matrix_start TRANSITION_FN ';'
					{

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found a transition matrix with known transition function\n");
						#endif

						$$->transition = $2;

        }

	;

action_decl:	action_decl_list ';'
					{

						#ifdef PARSER_DEBUG
						/* we should now be able to print a list of the actions */
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

 					$2->docCmnt = $1;

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

						$$->nextAction = $1->type_data.action_data.actionInfo;
						$1->type_data.action_data.actionInfo = $$;

					}
   | ACTION matrix
					{

						#ifdef PARSER_DEBUG
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

						$$->nextAction = $1->type_data.action_data.actionInfo;
						$1->type_data.action_data.actionInfo = $$;

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
		'(' ALL ')' 
		      {
 					if (($$ = init_list()) == NULL)
							yyerror("out of memory");

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found an all state vector\n");
 					#endif
		      }
 	| '(' state_comma_list STATE ')' 
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

event_vector: 
		'(' ALL ')' 
		      {
 					if (($$ = init_list()) == NULL)
							yyerror("out of memory");

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found an all event vector\n");
 					#endif
		      }
	| '(' event_comma_list EVENT ')' 
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
      psedecls = $$;
		}
 | event_decl
		{
 	   if (NULL == ($$ = ((pSTATE_AND_EVENT_DECLS)calloc(1,sizeof(STATE_AND_EVENT_DECLS)))))
 		    yyerror("Out of memory");

 		 $$->event_decls = $1;
 		 $$->state_decls = init_list();
      psedecls = $$;
		}
	| state_and_event_decls state_decl
	  {
 		$$ = $1;

			$$->state_decls = move_list($$->state_decls,$2);
      psedecls = $$;
	  }
	| state_and_event_decls event_decl
	  {
 		$$ = $1;

			$$->event_decls = move_list($$->event_decls,$2);
      psedecls = $$;

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

					 if (NULL == ($$->type_data.state_data.pinbound_transitions = init_list()))
					 {
					    yyerror("out of memory");
					 }

					 if (NULL == ($$->type_data.state_data.poutbound_transitions = init_list()))
					 {
					    yyerror("out of memory");
					 }

					 if (NULL == ($$->type_data.state_data.pevents_handled = init_list()))
					 {
					    yyerror("out of memory");
					 }

					 if (NULL == ($$->type_data.state_data.pactions_list = init_list()))
					 {
					    yyerror("out of memory");
					 }

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
   | state ON ENTRY
					{
         	#ifdef PARSER_DEBUG
         	fprintf(yyout
										, "state %s has anonymous entry function\n"
 									, $1->name
										);
         	#endif

 					$$ = $1;

 					$$->type_data.state_data.state_flags |= sfHasEntryFn;

		      }
   | state ON ENTRY ID
					{
         	#ifdef PARSER_DEBUG
         	fprintf(yyout
										, "state %s has entry function %s\n"
 									, $1->name
 									, $4->name
										);
         	#endif

 					$$ = $1;

 					$$->type_data.state_data.state_flags |= sfHasEntryFn;
 					$$->type_data.state_data.entry_fn     = $4;

 					set_id_type($4, ENTRY);

		      }
   | state ON EXIT
					{
         	#ifdef PARSER_DEBUG
         	fprintf(yyout
										, "state %s has anonymous exit function\n"
 									, $1->name
										);
         	#endif

 					$$ = $1;

 					$$->type_data.state_data.state_flags |= sfHasExitFn;

		      }
   | state ON EXIT ID
					{
         	#ifdef PARSER_DEBUG
         	fprintf(yyout
										, "state %s has exit function %s\n"
 									, $1->name
 									, $4->name
										);
         	#endif

 					$$ = $1;

 					$$->type_data.state_data.state_flags |= sfHasExitFn;
 					$$->type_data.state_data.exit_fn     = $4;

 					set_id_type($4, EXIT);

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

    /* this picks up any doc comment */
 	 $$ = $1;
  }
  ;

user_event_data: { $$ = NULL; }
  | DATA_KEY TRANSLATOR_KEY ID
   {
		if (pmachineInfo->parent && !pmachineInfo->parent->data) 
		{
			yyerror("data translator declared for sub-machine having parent with no data");
		}

  		if (NULL == ($$ = ((pUSER_EVENT_DATA) calloc(1, sizeof(USER_EVENT_DATA)))))
  		   yyerror("out of memory");
 
      $$->translator = $3;
      
      #ifdef PARSER_DEBUG
      fprintf(yyout,"found a data translator: %s\n", $3->name);
      #endif
   }
  | DATA_KEY TRANSLATOR_KEY ID data_block
   {
  		if (NULL == ($$ = ((pUSER_EVENT_DATA) calloc(1, sizeof(USER_EVENT_DATA)))))
  		   yyerror("out of memory");
 
      $$->translator = $3;
 		 $$->data_fields = $4;
      
      #ifdef PARSER_DEBUG
      fprintf(yyout,"found a data translator: %s\n", $3->name);
      fprintf(yyout,"found data fields\n");
 		 parser_debug_print_data_block($4,yyout);
      #endif
   }
  | DATA_KEY data_block
   {
  		if (NULL == ($$ = ((pUSER_EVENT_DATA) calloc(1, sizeof(USER_EVENT_DATA)))))
  		   yyerror("out of memory");
 
 		 $$->data_fields = $2;
      
      #ifdef PARSER_DEBUG
      fprintf(yyout,"found data fields\n");
 		 parser_debug_print_data_block($2,yyout);
      #endif
   }
   ;
    
event_decl_list:	EVENT_KEY ID external_designation user_event_data
					{

 					if (NULL == ($$ = init_list()))
 						yyerror("Out of memory");

           set_id_type($2,EVENT);
           $2->type_data.event_data.externalDesignation = $3;
           $2->type_data.event_data.puser_event_data    = $4;
           $2->powningMachine                           = pmachineInfo;

 					if (NULL == ($2->type_data.event_data.phandling_states = init_list()))
 						yyerror("Out of memory");

 					if (NULL == ($2->type_data.event_data.pactions_list = init_list()))
 						yyerror("Out of memory");

 					if (NULL == (add_to_list($$,$2)))
 						yyerror("Out of memory");

					}
 | EVENT_KEY parent_namespace EVENT user_event_data
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

           pid->type_data.event_data.puser_event_data   = $4;
           pid->type_data.event_data.shared_with_parent = true;
           pid->powningMachine                          = pmachineInfo;
 					pid->docCmnt                                 = $2;

 					if (NULL == (pid->type_data.event_data.phandling_states = init_list()))
 						yyerror("Out of memory");

 					if (NULL == (pid->type_data.event_data.pactions_list = init_list()))
 						yyerror("Out of memory");

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
	| event_decl_list ',' ID external_designation user_event_data
					{

						#ifdef PARSER_DEBUG
						fprintf(yyout,"added another id to the event declaration list\n");
						#endif

						$$ = $1;

           set_id_type($3,EVENT);
           $3->type_data.event_data.externalDesignation = $4;
           $3->type_data.event_data.puser_event_data    = $5;
           $3->powningMachine                           = pmachineInfo;

 					if (NULL == ($3->type_data.event_data.phandling_states = init_list()))
 						yyerror("Out of memory");

 					if (NULL == ($3->type_data.event_data.pactions_list = init_list()))
 						yyerror("Out of memory");

 					if (NULL == (add_to_list($$,$3)))
 						yyerror("Out of memory");

					}
 | event_decl_list ',' parent_namespace EVENT user_event_data
					{
            #ifdef PARSER_DEBUG
            fprintf(yyout,"added another namespace event reference to the declaration list\n");
            #endif

            $$ = $1;

           pID_INFO pid;

            id_list = pmachineInfo->id_list;

           if (!add_unique_id(id_list,EVENT,$4->name,&pid))
 						yyerror("Cannot reference parent event twice");

           pid->type_data.event_data.puser_event_data    = $5;
           pid->type_data.event_data.shared_with_parent  = true;
           pid->powningMachine                           = pmachineInfo;
 					 pid->docCmnt                                  = $3;

 					if (NULL == (pid->type_data.event_data.phandling_states = init_list()))
 						yyerror("Out of memory");

 					if (NULL == (pid->type_data.event_data.pactions_list = init_list()))
 						yyerror("Out of memory");

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


native:  
  {
	$$ = NULL;
  }
  | native native_prologue
  {
	if ($1 == NULL)
	{
		if (($$ = (pNATIVE_INFO) calloc(1, sizeof(NATIVE_INFO))) == NULL)
			yyerror("out of memory");
	}
	else
	{
		$$ = $1;
	}

	if ($$->prologue)
	{
		yyerror("cannot declare two native prologues");
	}

	$$->prologue = $2;

  }
  | native native_epilogue
  {
	if ($1 == NULL)
	{
		if (($$ = (pNATIVE_INFO) calloc(1, sizeof(NATIVE_INFO))) == NULL)
			yyerror("out of memory");
	}
	else
	{
		$$ = $1;
	}

	if ($$->epilogue)
	{
		yyerror("cannot declare two native epilogues");
	}

	$$->epilogue = $2;
  }
  ;

native_prologue:
	 NATIVE_KEY NATIVE_BLOCK
					{
						#ifdef PARSER_DEBUG
						fprintf(yyout,"Native\n%s\n",$2);
						#else
						$$ = $2;
						#endif
					}
	| NATIVE_KEY PROLOGUE_KEY NATIVE_BLOCK
					{
						#ifdef PARSER_DEBUG
						fprintf(yyout,"Native prologue\n%s\n",$3);
						#else
						$$ = $3;
						#endif
					}

	;
 
native_epilogue:
	NATIVE_KEY EPILOGUE_KEY NATIVE_BLOCK
					{
						#ifdef PARSER_DEBUG
						fprintf(yyout,"Native epilogue\n%s\n",$3);
						#else
						$$ = $3;
						#endif
					}

	;
 
native_impl:
	native_impl_prologue
	{
		if (($$ = (pNATIVE_INFO) calloc(1, sizeof(NATIVE_INFO))) == NULL)
			yyerror("out of memory");

		$$->prologue = $1;
	}
	| native_impl_epilogue
	{
		if (($$ = (pNATIVE_INFO) calloc(1, sizeof(NATIVE_INFO))) == NULL)
			yyerror("out of memory");

		$$->epilogue = $1;
	}
	| native_impl native_impl_prologue
	{
		if ($1->prologue != NULL)
		{
			yyerror("only one native implementation prologue is allowed");
		}

		$$ = $1;
		$$->prologue = $2;
	}
	| native_impl native_impl_epilogue
	{
		if ($1->epilogue != NULL)
		{
			yyerror("only one native implementation epilogue is allowed");
		}

		$$ = $1;
		$$->epilogue = $2;
	}
	;

native_impl_prologue: 
	NATIVE_KEY IMPLEMENTATION_KEY NATIVE_BLOCK
					{
						#ifdef PARSER_DEBUG
						fprintf(yyout,"Native implementation\n%s\n",$3);
						#else
						$$ = $3;
						#endif
					}
	| NATIVE_KEY IMPLEMENTATION_KEY PROLOGUE_KEY NATIVE_BLOCK
					{
						#ifdef PARSER_DEBUG
						fprintf(yyout,"Native implementation prologue\n%s\n",$4);
						#else
						$$ = $4;
						#endif
					}
	;
 
native_impl_epilogue: NATIVE_KEY IMPLEMENTATION_KEY EPILOGUE_KEY NATIVE_BLOCK
					{
						#ifdef PARSER_DEBUG
						fprintf(yyout,"Native implementation\n%s\n",$4);
						#else
						$$ = $4;
						#endif
					}

	;
 
machine_data: DATA_KEY data_block { $$ = $2; pmachineInfo->data = $$; };

data_block:	'{' data_fields '}'
	{
			#ifdef PARSER_DEBUG
			fprintf(yyout,"Data block done\n");
			#endif

			$$ = $2;

	}
	;

data_fields : data_field
   {
      #ifdef PARSER_DEBUG
    	fprintf(yyout,"Starting data_fields list\n");
    	#endif
   
      if (NULL == ($$ = init_list()))
    		yyerror("out of memory");
   
    	add_to_list($$, $1);
   
   }
   | data_fields data_field
   {
      #ifdef PARSER_DEBUG
    	fprintf(yyout,"Continuing data_fields list\n");
    	#endif
   
 	  $$ = $1;
    	add_to_list($$, $2);
   
   }
   ;

data_type : ID
   {
		   #ifdef PARSER_DEBUG
		   fprintf(yyout, "found simple data type id\n");
 		 #endif

 		 if (($$ = (pDATA_TYPE_STRUCT) calloc(1, sizeof(DATA_TYPE_STRUCT))) == NULL)
 		    yyerror("out of memory");

    	 set_id_type($1, TYPE_NAME);

 		 $$->dtt      = dtt_simple;
    	 $$->dtu.name = $1;

   }
   | TYPE_NAME
   {
		   #ifdef PARSER_DEBUG
		   fprintf(yyout, "found simple data type name\n");
 		 #endif

 		 if (($$ = (pDATA_TYPE_STRUCT) calloc(1, sizeof(DATA_TYPE_STRUCT))) == NULL)
 		    yyerror("out of memory");

 		 $$->dtt      = dtt_simple;
    	 $$->dtu.name = $1;

   }
   | STRUCT_KEY '{' data_fields '}' 
   {
		   #ifdef PARSER_DEBUG
		   fprintf(yyout, "found struct data type\n");
 		 #endif

 		 if (($$ = (pDATA_TYPE_STRUCT) calloc(1, sizeof(DATA_TYPE_STRUCT))) == NULL)
 		    yyerror("out of memory");

      $$->dtt         = dtt_struct;
 		 $$->dtu.members = $3;

   }
   | UNION_KEY  '{' data_fields '}' 
   {
		   #ifdef PARSER_DEBUG
		   fprintf(yyout, "found union data type\n");
 		 #endif

 		 if (($$ = (pDATA_TYPE_STRUCT) calloc(1, sizeof(DATA_TYPE_STRUCT))) == NULL)
 		    yyerror("out of memory");

      $$->dtt         = dtt_union;
 		 $$->dtu.members = $3;

   }
   | data_type '*'
   {
    $$ = $1;
    ($$->indirection_level)++;

		 #ifdef PARSER_DEBUG
		 fprintf(yyout
            ,"found pointer data type: TYPE: %s; indirection_level: %u\n"
            , $1->dtt == dtt_simple ? "simple"
 					   : $1->dtt == dtt_struct ? "struct"
 					     : "union"
            , $1->indirection_level
            );
		 #endif
   }
   ;

data_field : 
   data_type ID data_field_dimension ';'
  {
		 #ifdef PARSER_DEBUG
		 fprintf(yyout
            ,"found data field: TYPE: %s; NAME: %s; dimension: %s\n"
            , $1->dtt == dtt_simple ? "simple"
 					   : $1->dtt == dtt_struct ? "struct"
 					     : "union"
            , $2->name
            , $3 ? $3 : "none"
            );
		 #endif

 	 if (($$ = calloc(1, sizeof(DATA_FIELD))) == NULL)
 	    yyerror("out of memory");

   $$->pdts            = $1;
   $$->pdts->is_array  = true;
 	 $$->pdts->dimension = $3;
 	 $$->data_field_name = $2;

  }
  | data_type ID ';'
  {
		 #ifdef PARSER_DEBUG
		 fprintf(yyout
            ,"found data field: TYPE: %s; NAME: %s\n" 
            , $1->dtt == dtt_simple ? "simple"
 					   : $1->dtt == dtt_struct ? "struct"
 					     : "union"
            , $2->name
            );
		 #endif

 	 if (($$ = calloc(1, sizeof(DATA_FIELD))) == NULL)
 	    yyerror("out of memory");

    $$->pdts            = $1;
 	 $$->data_field_name = $2;

  }
  ;

data_field_dimension: '[' ']'
   {
       $$ = NULL;
   }
   | '[' NUMERIC_STRING ']'
   {
       $$ = $2;
   }
   | '[' ID ']'
   {
       $$ = $2->name;
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

 	 if (!$1->type_data.action_data.action_returns_decl)
		 {
		    if (($1->type_data.action_data.action_returns_decl = init_list()) == NULL) 
				   yyerror("out of memory");
		 }

			if (add_to_list($3,$4) == NULL)
				 yyerror("out of memory");

			move_list_unique($1->type_data.action_data.action_returns_decl, $3);
 		free_list($3);

  }
  | ACTION RETURNS returns_comma_list namespace_event_ref ';'
  {
    #ifdef PARSER_DEBUG
    fprintf(yyout,"Found an action return declaration\n");
    #endif

 	 if (!$1->type_data.action_data.action_returns_decl)
		 {
		    if (($1->type_data.action_data.action_returns_decl = init_list()) == NULL) 
				   yyerror("out of memory");
		 }

			if (add_to_list($3,$4) == NULL)
				 yyerror("out of memory");

			move_list_unique($1->type_data.action_data.action_returns_decl, $3);
 		free_list($3);

  }
  | ACTION RETURNS EVENT ';'
  {
    #ifdef PARSER_DEBUG
    fprintf(yyout,"Found an action return declaration\n");
    #endif

 	 if (!$1->type_data.action_data.action_returns_decl)
		 {
		    if (($1->type_data.action_data.action_returns_decl = init_list()) == NULL) 
				   yyerror("out of memory");
		 }

		 if (add_unique_to_list($1->type_data.action_data.action_returns_decl,$3) == NULL)
				yyerror("out of memory");

  }
  | ACTION RETURNS namespace_event_ref ';'
  {
    #ifdef PARSER_DEBUG
    fprintf(yyout,"Found an action return declaration\n");
    #endif

 	 if (!$1->type_data.action_data.action_returns_decl)
		 {
		    if (($1->type_data.action_data.action_returns_decl = init_list()) == NULL) 
				   yyerror("out of memory");
		 }

		 if (add_unique_to_list($1->type_data.action_data.action_returns_decl,$3) == NULL)
				yyerror("out of memory");

  }
  | ACTION RETURNS state_comma_list STATE ';'
  {
    #ifdef PARSER_DEBUG
    fprintf(yyout,"Found an action return declaration\n");
    #endif

			if (add_to_list($3, $4) == NULL)
				yyerror("out of memory");

 	 if (!$1->type_data.action_data.action_returns_decl)
		 {
		    if (($1->type_data.action_data.action_returns_decl = init_list()) == NULL) 
				   yyerror("out of memory");
		 }

			move_list_unique($1->type_data.action_data.action_returns_decl, $3);
 		free_list($3);

  }
  | ACTION RETURNS STATE ';'
  {
    #ifdef PARSER_DEBUG
    fprintf(yyout,"Found an action return declaration\n");
    #endif

		 if (add_to_list($1->type_data.action_data.action_returns_decl,$3) == NULL)
				yyerror("out of memory");

 	 if (!$1->type_data.action_data.action_returns_decl)
		 {
		    if (($1->type_data.action_data.action_returns_decl = init_list()) == NULL) 
				   yyerror("out of memory");
		 }

			add_unique_to_list($1->type_data.action_data.action_returns_decl, $3);

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
#include <stdio.h>
#include <stdlib.h>

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
	, lo_short_dbg_names
 , lo_force_generation_of_event_passing_actions
 , lo_add_machine_name
 , lo_generate_run_function
 , lo_add_event_cross_reference
 , lo_add_plantuml_title
 , lo_add_plantuml_legend
 , lo_exclude_states_from_plantuml_legend
 , lo_exclude_events_from_plantuml_legend
 , lo_exclude_actions_from_plantuml_legend
 , lo_add_plantuml_prefix_string
 , lo_add_plantuml_prefix_file
 , lo_short_user_fn_names
 , lo_event_cross_ref_only
 , lo_event_cross_ref_format
 , lo_convenience_macro_in_public_header
 , lo_add_profiling_macros
 , lo_profile_sub_fsms
 , lo_empty_cell_fn
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
        , .has_arg = optional_argument
        , .flag    = &longval
        , .val     = lo_include_svg_img
    }
    , {
        .name      = "short-debug-names"
        , .has_arg = no_argument
        , .flag    = &longval
        , .val     = lo_short_dbg_names
    }
    , {
        .name      = "force-generation-of-event-passing-actions"
        , .has_arg = no_argument
        , .flag    = &longval
        , .val     = lo_force_generation_of_event_passing_actions
    }
    , {
        .name      = "add-machine-name"
        , .has_arg = no_argument
        , .flag    = &longval
        , .val     = lo_add_machine_name
    }
    , {
        .name      = "generate-run-function"
        , .has_arg = optional_argument
        , .flag    = &longval
        , .val     = lo_generate_run_function
    }
    , {
        .name      = "add-event-cross-reference"
        , .has_arg = optional_argument
        , .flag    = &longval
        , .val     = lo_add_event_cross_reference
    }
    , {
        .name      = "add-plantuml-title"
        , .has_arg = optional_argument
        , .flag    = &longval
        , .val     = lo_add_plantuml_title
    }
    , {
        .name      = "add-plantuml-legend"
        , .has_arg = optional_argument
        , .flag    = &longval
        , .val     = lo_add_plantuml_legend
    }
    , {
        .name      = "exclude-events-from-plantuml-legend"
        , .has_arg = optional_argument
        , .flag    = &longval
        , .val     = lo_exclude_events_from_plantuml_legend
    }
    , {
        .name      = "exclude-states-from-plantuml-legend"
        , .has_arg = optional_argument
        , .flag    = &longval
        , .val     = lo_exclude_states_from_plantuml_legend
    }
    , {
        .name      = "exclude-actions-from-plantuml-legend"
        , .has_arg = optional_argument
        , .flag    = &longval
        , .val     = lo_exclude_actions_from_plantuml_legend
    }
    , {
        .name      = "add-plantuml-prefix-string"
        , .has_arg = required_argument
        , .flag    = &longval
        , .val     = lo_add_plantuml_prefix_string
    }
    , {
        .name      = "add-plantuml-prefix-file"
        , .has_arg = required_argument
        , .flag    = &longval
        , .val     = lo_add_plantuml_prefix_file
    }
    , {
        .name      = "add-plantuml-prefix-file"
        , .has_arg = required_argument
        , .flag    = &longval
        , .val     = lo_add_plantuml_prefix_file
    }
    , {
        .name      = "short-user-fn-names"
        , .has_arg = optional_argument
        , .flag    = &longval
				, .val     = lo_short_user_fn_names
    }
    , {
        .name      = "event-cross-ref-only"
        , .has_arg = optional_argument
        , .flag    = &longval
				, .val     = lo_event_cross_ref_only
    }
    , {
        .name      = "event-cross-ref-format"
        , .has_arg = required_argument
        , .flag    = &longval
				, .val     = lo_event_cross_ref_format
		}
		, {
        .name      = "convenience-macros-in-public-header"
        , .has_arg = optional_argument
        , .flag    = &longval
				, .val     = lo_convenience_macro_in_public_header
    }
		, {
        .name      = "add-profiling-macros"
        , .has_arg = optional_argument
        , .flag    = &longval
				, .val     = lo_add_profiling_macros
    }
		, {
        .name      = "profile-sub-fsms"
        , .has_arg = optional_argument
        , .flag    = &longval
				, .val     = lo_profile_sub_fsms
    }
		, {
        .name      = "empty-cell-fn"
        , .has_arg = required_argument
        , .flag    = &longval
				, .val     = lo_empty_cell_fn
    }
    , {0}
};
      
int main(int argc, char **argv)
{

	char	*cp1;
	char    *outFileBase = NULL;

 #ifndef PARSER_DEBUG
 int   c;
 #endif

	me = argv[0];

#ifdef YYDEBUG
// yydebug = 1;
#endif

	/* special case the single '?' */
	if (argc == 2 && argv[1][0] == '?') {

		usage();
		return (1);

	}

#ifndef PARSER_DEBUG

	while ((c = getopt_long(argc,argv,"vh:t:o:i:csM::", longopts, &longindex)) != -1) {

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
                if (!optarg || !strcmp(optarg, "true"))
                {
                  include_svg_img = true;
                }
                break;
            case lo_css_content_filename:
                css_content_filename = optarg;
                break;
            case lo_css_content_internal:
                css_content_internal
                    = !strcmp(optarg,"true") ? true : false;
                break;
            case lo_short_dbg_names:
                short_dbg_names = true;
                break;
            case lo_add_machine_name:
                add_machine_name = true;
                break;
            case lo_force_generation_of_event_passing_actions:
                force_generation_of_event_passing_actions = true;
                break;
            case lo_generate_run_function:
                if (optarg && !strcmp(optarg,"false"))
                    generate_run_function = false;
                break;
 			   case lo_add_event_cross_reference:
 					 if (!optarg || !strcmp(optarg,"true"))
 						add_event_cross_reference = true;
 					 break;
         case lo_event_cross_ref_only:
 					 if (!optarg || !strcmp(optarg,"true"))
						fpfsmogf = generateCEventXRefWriter;
           break;
         case lo_event_cross_ref_format:
           if (!check_requested_xref_format(optarg))
             yyerror("invalid event cross reference format");
					 fpfsmogf = generateCEventXRefWriter;
           break;
 			   case lo_add_plantuml_title:
 					 if (!optarg || !strcmp(optarg,"true"))
 						add_plantuml_title = true;
 					 break;
 			   case lo_add_plantuml_legend:
 					 if (!optarg)
						 {
 					    add_plantuml_legend = true;
						 }
 					 else
						 {
								 if (strstr(optarg, "top"))
   						 {
    					    add_plantuml_legend = true;
    							plantuml_legend_vertical_placement = vp_top;
   						 }
								 if (strstr(optarg, "bottom"))
   						 {
    					    add_plantuml_legend = true;
    							plantuml_legend_vertical_placement = vp_bottom;
   						 }
								 if (strstr(optarg, "center"))
   						 {
    					    add_plantuml_legend = true;
    							plantuml_legend_horizontal_placement = hp_center;
   						 }
								 if (strstr(optarg, "left"))
   						 {
    					    add_plantuml_legend = true;
    							plantuml_legend_horizontal_placement = hp_left;
   						 }
								 if (strstr(optarg, "right"))
   						 {
    					    add_plantuml_legend = true;
    							plantuml_legend_horizontal_placement = hp_right;
   						 }
						 }

 					 if (!add_plantuml_legend)
						 {
 							 yyerror("unrecognized legend placement option");
 							 usage();
 							 return (good);
						 }
 					 break;
 			   case lo_exclude_events_from_plantuml_legend:
 					 if (!optarg || !strcmp(optarg,"true"))
 						exclude_events_from_plantuml_legend = true;
 					 break;
 			   case lo_exclude_states_from_plantuml_legend:
 					 if (!optarg || !strcmp(optarg,"true"))
 						exclude_states_from_plantuml_legend = true;
 					 break;
 			   case lo_exclude_actions_from_plantuml_legend:
 					 if (!optarg || !strcmp(optarg,"true"))
 						exclude_actions_from_plantuml_legend = true;
 					 break;
 					 case lo_add_plantuml_prefix_string:
 					     if (!pplantuml_prefix_strings_list)
 							    pplantuml_prefix_strings_list = init_list();
 							 add_to_list(pplantuml_prefix_strings_list, optarg);
 							 break;
 					 case lo_add_plantuml_prefix_file:
 					     if (!pplantuml_prefix_files_list)
 							    pplantuml_prefix_files_list = init_list();
 							 add_to_list(pplantuml_prefix_files_list, optarg);
 							 break;
			    case lo_short_user_fn_names:
		            if (!optarg || !strcmp(optarg, "true"))
			            short_user_fn_names=true;
		            break;
						case lo_convenience_macro_in_public_header:
						   if (optarg && !strcmp(optarg, "false"))
							 {
								 convenience_macros_in_public_header = false;
							 }
							 break;
      			case lo_add_profiling_macros:
							if (optarg && !strcmp(optarg, "true"))
							{
								add_profiling_macros = true;
							}
							break;
      			case lo_profile_sub_fsms:
							if (optarg && !strcmp(optarg, "true"))
							{
								profile_sub_fsms = true;
							}
							break;
            case lo_empty_cell_fn:
                empty_cell_fn = optarg;
                break;
            default:
                usage();
                return(0);
                break;
        }
        break;

		case 'h':
      if (optarg[0])
      {
        html_help = true;
      }
			usage();
			return (1);

 		case 's':
 			pfsmog = pMachineStatisticsWriter;
 			break;

			case 't':

				switch (optarg[0]) {

					case 'c':
            fpfsmogf = generateCMachineWriter;
						break;

					case 'h':
						fpfsmogf = generateHTMLMachineWriter;
						break;

					case 's':
						fpfsmogf = generateCSwitchMachineWriter;
						break;

					case 'p':
						fpfsmogf = generatePlantUMLMachineWriter;
						break;

					case 'e':
						fpfsmogf = generateCEventTableMachineWriter;
						break;
          		case 'r':
            		fpfsmogf = generateRSTMachineWriter;
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

     case 'M':
		output_generated_file_names_only = true;
		if (optarg)
		{
			switch (optarg[0])
			{
				case 'd':
					output_make_recipe = true;
					break;

				case 'h':
					output_header_files = true;
				break;

				default:
					break;
			}
		}
		break;

     case '?':
     case ':':

        usage();
        return (0);

		}

	}
#endif

	if (optind >= argc)
	{
		fprintf(stdout,"need a file name to work with.\n");
		return (!good);
	}

	for (int fnind = optind; fnind < argc && good; fnind++) {

		inputFileName = strdup(argv[fnind]);

		/* find the extension */
		cp1 = rindex(inputFileName,'.');
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

		if ((yyin = openFile(argv[fnind],"r")) == NULL) {

			return 1;

		}

		/* get the base file name */
		if (!outFileBase) {
			/* use the base input file name */
			*cp1 = 0;
			cwk_path_get_basename(inputFileName, (const char**)&outFileBase, NULL);
		}

		#ifndef PARSER_DEBUG

		if (NULL == pfsmog)
		{

    	    /* default to writing a c machine */
    	    if (NULL == fpfsmogf)
    	    {
    			fpfsmogf = generateCMachineWriter;
    	    }
   
			pfsmog = fpfsmogf(NULL);
		}

		if (!(*pfsmog->initOutput)(pfsmog,outFileBase)) {

		#endif

			yyparse();

		#ifndef PARSER_DEBUG
			(*pfsmog->closeOutput)(pfsmog,good);

      //prepare for next input file, if any
      outFileBase = NULL;

		}
		#endif

		fclose(yyin);

	}

	return (good == 1 ? 0 : 1);

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
	char *list_start       = html_help ? "\n<ul class=\"syntax\">\n" : "\n";
	char *list_end         = html_help ? "</ul>\n"                   : "";
	char *item_start       = html_help ? "<li>"                      : "\t";
	char *inner_item_start = html_help ? "<li>"                      : "\t\t";
	char *item_end         = html_help ? "</li>\n"                   : "\n";
	char *list_item_end    = html_help ? "</li>\n\t</ul>\n</li>\n"   : "\n";
	char *lt               = html_help ? "&lt;"                      : "<";
	char *gt               = html_help ? "&gt;"                      : ">";

	fprintf(stdout
			, "%s%sUsage : %s [-tc|s|e|h|p|r] [-o outfile] [-s] filename, where filename ends with '.fsm'%s"
			, list_start
			, item_start
			, me
			, list_start
			);
	fprintf(stdout
			, "%sand where 'c' gets you c code output based on an event/state table,%s"
			, inner_item_start
			, item_end
			);
	fprintf(stdout
			,"%s's' gets you c code output with individual state functions using switch constructions,%s"
			, inner_item_start
			, item_end
			);
	fprintf(stdout
			,"%s'e' gets you c code output with a table of functions for each event using switch constructions,%s"
			, inner_item_start
			, item_end
			);
	fprintf(stdout
			,"%s'h' gets you html output%s"
			, inner_item_start
			, item_end
			);
	fprintf(stdout
			,"%s'p' gets you PlantUML output%s"
			, inner_item_start
			, item_end
			);
	fprintf(stdout
			,"%s'r' gets you reStructuredText output%s"
			, inner_item_start
			, list_item_end
			);
	fprintf(stdout
			,"%s-i0 inhibits the creation of a machine instance%s"
			, item_start
			, list_start
			);
	fprintf(stdout
			,"%sany other argument to 'i' allows the creation of an instance;%s"
			, inner_item_start
			, item_end
			);
	fprintf(stdout
			,"%sthis is the default%s"
			, inner_item_start
			, list_item_end
			);
	fprintf(stdout
			,"%s-c will create a more compact event/state table when -tc is used%s"
			, item_start
			, list_start
			);
	fprintf(stdout
			,"%swith machines having actions which return states%s"
			, inner_item_start
			, list_item_end
			);
	fprintf(stdout
			,"%s-s prints some useful statistics and exits%s"
			, item_start
			, item_end
			);
	fprintf(stdout
			,"%s-o  %soutfile%s will use %soutfile%s as the filename for the top-level machine output.%s"
			, item_start
      , lt
      , gt
      , lt
      , gt
			, list_start
			);
	fprintf(stdout
			,"%sAny sub-machines will be put into files based on the sub-machine names.%s"
			, inner_item_start
			, list_item_end
			);
	fprintf(stdout
			,"%s--generate-weak-fns=false suppresses the generation of weak function stubs.%s"
			, item_start
			, item_end
			);
	fprintf(stdout
			,"%s--short-user-fn-names=true causes user functions (such as action functions to use only the machine name when the sub-machine depth is 1).%s"
			, item_start
			, item_end
			);
	fprintf(stdout
			,"%s--force-generation-of-event-passing-actions forces the generation of actions which pass events%s"
			, item_start
			, list_start
			);
 fprintf(stdout
		 ,"%swhen weak function generation is disabled.%s"
		 , inner_item_start
		 , item_end
		 );
 fprintf(stdout
		 ,"%sThe generated functions are not weak.%s"
		 , inner_item_start
		 , list_item_end
		 );
	fprintf(stdout
			,"%s--core-logging-only=true suppresses the generation of debug log messages in all but the core FSM function.%s"
			, item_start
			, item_end
			);
 fprintf(stdout
		 ,"%s--generate-run-function%s=true|false%s this option is deprecated.  The run function is always generated;%s"
		 , item_start
     , lt
     , gt
		 , list_start
		 );
	fprintf(stdout
			,"%sno RUN_STATE_MACHINE macro is provided.%s"
			, inner_item_start
			, list_item_end
			);
	fprintf(stdout
			,"%s--include-svg-img%s=*true|false%s adds %simg/%s tag referencing %sfilename%s.svg to include an image at%s"
			, item_start
      , lt
      , gt
      , lt
      , gt
      , lt
      , gt
			, list_start
			);
  fprintf(stdout
		  ,"%sthe top of the web page.%s"
		  , inner_item_start
		  , list_item_end
		  );
	fprintf(stdout
			,"%s--css-content-internal=true puts the CSS directly into the html.%s"
			, item_start
			, item_end
			);
	fprintf(stdout
			,"%s--css-content-filename=%sfilename%s uses the named file for the css citation, or%s"
			, item_start
      , lt
      , gt
			, list_start
			);
	fprintf(stdout
			,"%sfor the content copy.%s"
			, inner_item_start
			, list_item_end
			);
 fprintf(stdout
		 ,"%s--add-plantuml-title=%s*true|false%s adds the machine name as a title to the plantuml.%s"
		 , item_start
      , lt
      , gt
		 , item_end
		 );
 fprintf(stdout
		 ,"%s--add-plantuml-legend=%s*center|left|right|top|*bottm%s adds a legend to the plantuml.%s"
		 , item_start
      , lt
      , gt
		 , list_start
		 );
 fprintf(stdout
		 ,"%sCenter, bottom are the defaults.  Horizontal and vertial parameters can be added in a quoted string.%s"
		 , inner_item_start
		 , item_end
		 );
 fprintf(stdout
		 ,"%sCenter is a horizontal parameter.%s"
		 , inner_item_start
		 , item_end
		 );
 fprintf(stdout
		 ,"%sBy default, event, state, and action lists are%s"
		 , inner_item_start
		 , item_end
		 );
 fprintf(stdout
		 ,"%sincluded in the legend, and event descriptions are removed%s"
		 , inner_item_start
		 , item_end
		 );
 fprintf(stdout
		 ,"%sfrom the body of the diagram.%s"
		 , inner_item_start
		 , list_item_end
		 );
 fprintf(stdout
		 ,"%s--exclude-states-from-plantuml-legend=%s*true|false%s excludes state information from the plantuml legend.%s"
		 , item_start
      , lt
      , gt
		 , list_start
		 );
 fprintf(stdout
		 ,"%sWhen excluded from legend, state comments are included in the diagram body.%s"
		 , inner_item_start
		 , list_item_end
		 );
 fprintf(stdout
		 ,"%s--exclude-events-from-plantuml-legend=%s*true|false%s excludes event information from the plantuml legend.%s"
		 , item_start
      , lt
      , gt
		 , item_end
		 );
 fprintf(stdout
		 ,"%s--exclude-actions-from-plantuml-legend=%s*true|false%s excludes action information from the plantuml legend.%s"
		 , item_start
      , lt
      , gt
		 , item_end
		 );
 fprintf(stdout
		 ,"%s--convenience-macros-in-public-header[=%s*true|false%s] includes convenience macros%s"
		 , item_start
      , lt
      , gt
		 , list_start
		 );
 fprintf(stdout
		 ,"%s(THIS, UFMN, e.g.) in the public header of the top-level machine;%s"
		 , inner_item_start
		 , item_end
		 );
 fprintf(stdout
		 ,"%sotherwise, they are placed in the private header.%s"
		 , inner_item_start
		 , list_item_end
		 );
 fprintf(stdout
		 ,"%s--add-machine-name adds the machine name when using the --short-debug-names option%s"
		 , item_start
		 , item_end
		 );
 fprintf(stdout
		 ,"%s--add-event-cross-reference%s=true|*false%s adds a cross-reference list as a comment block%s"
		 , item_start
      , lt
      , gt
		 , list_start
		 );
 fprintf(stdout
		 ,"%sin front of the machine event enumeration. Omitting the optional argument is equivalent%s"
		 , inner_item_start
		 , item_end
		 );
 fprintf(stdout
		 ,"%sto specifying \"true\"%s"
		 , inner_item_start
		 , list_item_end
		 );
 fprintf(stdout
		 ,"%s--event-cross-ref-only%s=*true|false%s creates a cross-reference list as a separate file.%s"
		 , item_start
      , lt
      , gt
		 , list_start
		 );
 fprintf(stdout
		 ,"%sWhen the format is not specified by --event-cross-ref-format, json is provided.%s"
		 , inner_item_start
		 , item_end
		 );
 fprintf(stdout
		 ,"%sThe file created is %sfilename%s.[json|csv|tab|xml]%s"
		 , inner_item_start
      , lt
      , gt
		 , list_item_end
		 );
 fprintf(stdout
		 ,"%s--event-cross-ref-format=[json|csv|tab|xml] specifies the output format for --event-cross-ref-only.%s"
		 , item_start
		 , list_start
		 );
 fprintf(stdout
		 ,"%sSpecifying this option obviates --event-cross-ref-only.%s"
		 , inner_item_start
		 , list_item_end
		 );
 fprintf(stdout
		 ,"%s--add-plantuml-prefix-string=%stext%s will add the specified text to the plantuml output before%s"
		 , item_start
      , lt
      , gt
		 , list_start
		 );
 fprintf(stdout
		 ,"%sany generated output.  This option can be specified multiple times; all text will be%s"
		 , inner_item_start
		 , item_end
		 );
 fprintf(stdout
		 ,"%sadded in the order given%s"
		 , inner_item_start
		 , item_end
		 );
 fprintf(stdout
		 ,"%sfor the content copy.%s"
		 , inner_item_start
		 , list_item_end
		 );
 fprintf(stdout
		 ,"%s--add-plantuml-prefix-file=%stext%s will add the text in the specified file%s"
		 , item_start
      , lt
      , gt
		 , list_start
		 );
 fprintf(stdout
		 ,"%sto the plantuml output before any generated output.%s"
		 , inner_item_start
		 , item_end
		 );
	fprintf(stdout
			,"%sThis option can be specified multiple times; all text will be%s"
			, inner_item_start
			, item_end
			);
 fprintf(stdout
		 ,"%sadded in the order given%s"
		 , inner_item_start
		 , item_end
		 );
 fprintf(stdout
		 ,"%sfor the content copy.%s"
		 , inner_item_start
		 , list_item_end
		 );
 fprintf(stdout
		 ,"%s-M prints the file name(s) of the source files that would have been created to stdout.%s"
		 , item_start
		 , list_start
		 );
 fprintf(stdout
		 ,"%sThis is useful in Makefiles for getting the list of files%s"
		 , inner_item_start
		 , item_end
		 );
 fprintf(stdout
		 ,"%sthat will be generated %s"
		 , inner_item_start
		 , item_end
		 );
 fprintf(stdout
		 ,"%s(e.g. GENERATED_SRC=$(shell $(FSM) -M -tc $(FSM_SRC))).%s"
		 , inner_item_start
		 , item_end
		 );
 fprintf(stdout
		 ,"%sThis option must preceed the -t option.%s"
		 , inner_item_start
		 , list_item_end
		 );
 fprintf(stdout
		 ,"%s-Mh prints the file name(s) of the headers that would have been created to stdout.%s"
		 , item_start
		 , list_start
		 );
 fprintf(stdout
		 ,"%sThis is useful in Makefiles for getting the list of files%s"
		 , inner_item_start
		 , item_end
		 );
 fprintf(stdout
		 ,"%sthat will be generated %s"
		 , inner_item_start
		 , item_end
		 );
 fprintf(stdout
		 ,"%s(e.g. GENERATED_HDRS=$(shell $(FSM) -M -tc $(FSM_SRC))).%s"
		 , inner_item_start
		 , item_end
		 );
 fprintf(stdout
		 ,"%sThis option must preceed the -t option.  And, only tc or ts are applicable.%s"
		 , inner_item_start
		 , list_item_end
		 );
 fprintf(stdout
		 ,"%s-Md print a line suitable for inclusion in a Makefile giving the recipe for%s"
		 , item_start
		 , list_start
		 );
 fprintf(stdout
		 ,"%screating dependent files.%s"
		 , inner_item_start
		 , item_end
		 );
 fprintf(stdout
		 ,"%sThis option must preceed the -t option.%s"
		 , inner_item_start
		 , list_item_end
		 );
 fprintf(stdout
		 ,"%s--add-profiling-macros%s=true|*false%s adds profiling macros at the beginning%s"
		 , item_start
      , lt
      , gt
		 , list_start
		 );
 fprintf(stdout
		 ,"%sand end of the FSM function, and before and after invocation of action functions.%s"
		 , inner_item_start
		 , list_item_end
		 );
 fprintf(stdout
		 ,"%s--profile-sub-fsms%s=true|*false%s adds profiling macros at the beginning%s"
		 , item_start
      , lt
      , gt
		 , list_start
		 );
 fprintf(stdout
		 ,"%sand end of the FSM function in sub-machines.  Profiling macros%s"
		 , inner_item_start
		 , item_end
		 );
 fprintf(stdout
		 ,"%smust also be enabled.%s"
		 , inner_item_start
		 , list_item_end
		 );
 fprintf(stdout
		 ,"%s--empty-cell-fn=%sname%s designates a function to be called when%s"
		 , item_start
      , lt
      , gt
		 , list_start
		 );
 fprintf(stdout
		 ,"%san event/state cell is empty.%s"
		 , inner_item_start
		 , list_item_end
		 );
 fprintf(stdout
		 ,"%s-v prints the version and exits%s%s"
		 , item_start
		 , item_end
		 , list_end
		 );
	
}

