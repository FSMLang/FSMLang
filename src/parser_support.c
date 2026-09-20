/**
*  parser_support.c
*
*    Functions directly implementing parser rules.
*
*    FSMLang (fsm) - A Finite State Machine description language.
*    Copyright (C) 2026  Steven Stanton
*
*    This program is free software; you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation; either version 2 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program; if not, write to the Free Software
*    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
*    Steven Stanton
*    fsmlang@pesticidesoftware.com
*
*    For the latest on FSMLang: https://fsmlang.github.io
*
*    And, finally, your possession of this source code implies nothing.
*
*    File created by Steven Stanton
*
*  Long Description:
*
*/

#include "parser_support.h"
#include "y.tab.h"

#if defined (CYGWIN) || defined (LINUX)
	#include <stdio.h>
	#include <ctype.h>
	#include <unistd.h>
#endif
#if defined (LINUX) || defined (VS) || defined (CYGWIN)
	#include <time.h>
#endif
#include <string.h>
#include <stdlib.h>

extern char	         *yytext;
extern FILE	         *yyin, *yyout;
extern pLIST         id_list;
extern pMACHINE_INFO pmachineInfo;
extern pLIST         root_id_list;

static pMACHINE_PREFIX create_machine_prefix(pNATIVE_INFO,MOD_FLAGS,pID_INFO);
static void data_translator_common(pID_INFO);

/**
 * Capture the info for the state and the machine implementing
 * it.
 *
 * At this point, the machine is prospective, it will need to
 * be found later.
 * 
 * @author Steven Stanton (8/21/2026)
 * 
 * @param pstate   The state being implemented by a machine.
 * @param pmachine The name of the machine which will implement
 *  			   the state.
 * 
 * @return pID_INFO The state, as needed by the parsing tree.
 */
pID_INFO state_implemented_by(pID_INFO pstate, pID_INFO pmachine)
{
#ifdef PARSER_DEBUG
	fprintf(yyout
			, "state %s implemented by submachine %s\n"
			, pstate->name
			, pmachine->name
		   );
#endif

	if (pstate->type_data.state_data.state_flags & sfInibitSubMachines)
	{
		yyerror("a state inhibiting sub-machines cannot be implemented by such");
	}

	pstate->type_data.state_data.state_flags         |= sfImplementedBySubMachine;
	pstate->type_data.state_data.implementingMachine  = pmachine;

	set_id_type(pmachine, UNDEFINED_SI_MACHINE);
	pmachine->type_data.machine_pid_data.mpid_flags          = mpidf_implements_state;
	pmachine->type_data.machine_pid_data.implementedArtifact = pstate;

	return pstate;
}

static void data_translator_common(pID_INFO ptranslator)
{
	if (pmachineInfo->parent)
	{
		if (!pmachineInfo->parent->data) 
		{
			yyerror("data translator declared for sub-machine having parent with no data");
		}

		pmachineInfo->parent->submachines_wanting_parent_data_count++;
	}

	set_id_type(ptranslator, TRANSLATOR_FN);

#ifdef PARSER_DEBUG
fprintf(yyout,"found a data translator: %s\n", ptranslator->name);
#endif
}

pID_INFO data_translator(pID_INFO ptranslator)
{
	data_translator_common(ptranslator);
	return ptranslator;
}

pID_INFO consuming_data_translator(pID_INFO ptranslator)
{
	data_translator_common(ptranslator);
	ptranslator->type_data.translator_data.flags |= tf_consuming;
	return ptranslator;
}

/**
 * Capture the info for the translator and the machine implementing
 * it.
 *
 * At this point, the machine is prospective, it will need to
 * be found later.
 * 
 * @author Steven Stanton (8/21/2026)
 * 
 * @param ptranslator   The translator being implemented by a machine.
 * @param pmachine The name of the machine which will implement
 *  			   the translator.
 * 
 * @return pID_INFO The translator, as needed by the parsing tree.
 */
pID_INFO translator_implemented_by(pID_INFO ptranslator, pID_INFO pmachine)
{
#ifdef PARSER_DEBUG
	fprintf(yyout
			, "translator %s implemented by submachine %s\n"
			, ptranslator->name
			, pmachine->name
		   );
#endif

	ptranslator->type_data.translator_data.flags               |= tf_implemented_by_sub_machine;
	ptranslator->type_data.translator_data.implementingMachine  = pmachine;

	set_id_type(pmachine, UNDEFINED_TI_MACHINE);
	pmachine->type_data.machine_pid_data.mpid_flags          = mpidf_implements_translator;
	pmachine->type_data.machine_pid_data.implementedArtifact = ptranslator;

	return ptranslator;
}

/**
 * Create the machine prefix from the parsed stream.
 *
 * This function will swap the active namespace to that of the
 * new machine, so it is imperative that the machine name be
 * added to the existing namespace before this function is
 * called.
 * 
 * @author Steven Stanton (8/12/2026)
 * 
 * @param pnative          pointer to native block data
 * @param machine_modifier pointer to the machine modifier flags
 * @param pid              pointer to the pid representing the
 *  					   machine name
 * 
 * @return pMACHINE_PREFIX pointer to the created data
 *  	   structure.
 */
static pMACHINE_PREFIX create_machine_prefix(pNATIVE_INFO pnative, MOD_FLAGS machine_modifier,pID_INFO pid)
{
	pMACHINE_PREFIX pmachine_prefix;

	if ((pmachine_prefix = (pMACHINE_PREFIX)calloc(1, sizeof(MACHINE_PREFIX))) == NULL)
	{
		yyerror("out of memory");
	}

	if ((pmachine_prefix->pmachineInfo = (pMACHINE_INFO)calloc(1, sizeof(MACHINE_INFO))) == NULL)
	{
		yyerror("out of memory");
	}

	/* grab any native language stuff */
	if (pnative)
	{
		pmachine_prefix->pmachineInfo->native_prologue = pnative->prologue;
		pmachine_prefix->pmachineInfo->native_epilogue = pnative->epilogue;
	}

	/* grab any modifiers */
	pmachine_prefix->pmachineInfo->modFlags = machine_modifier;

	pid->powningMachine = pmachine_prefix->pmachineInfo;

	pmachine_prefix->pmachineInfo->name   = pid;
	pmachine_prefix->pmachineInfo->parent = pmachineInfo;


	/* now give ourselves our own id list */
	id_list = pmachine_prefix->pmachineInfo->id_list = init_list();

	/* set the global pointer to the current machine info struct */
	pmachineInfo = pmachine_prefix->pmachineInfo;

	return pmachine_prefix;
}

pMACHINE_PREFIX machine_declared_by_id(pNATIVE_INFO pnative, MOD_FLAGS machine_modifier, pID_INFO pid)
{
	/* grab our name */
	set_id_type(pid, MACHINE);

	return create_machine_prefix(pnative,machine_modifier,pid);
}

pMACHINE_PREFIX machine_declared_by_machine_pid(pNATIVE_INFO pnative, MOD_FLAGS machine_modifier, pID_INFO machine_pid)
{
	pMACHINE_PREFIX pmachine_prefix = NULL;

	if (machine_pid->type_data.machine_pid_data.mpid_flags & mpidf_implements_state)
	{
		set_id_type(machine_pid, SI_MACHINE);
		machine_modifier |= mfStateImplementing;
	}
	else
	{
		set_id_type(machine_pid, TI_MACHINE);
		machine_modifier |= mfTranslatorImplementing;
	}

	pmachine_prefix = create_machine_prefix(pnative
											, machine_modifier
											, machine_pid
											);

	if (pmachine_prefix == NULL)
	{
		yyerror("out of memory");
	}

	machine_pid->type_data.machine_pid_data.pmi = pmachine_prefix->pmachineInfo;
	pmachine_prefix->pmachineInfo->implemented_artifact = machine_pid->type_data.machine_pid_data.implementedArtifact;

	return pmachine_prefix;
}

void capture_machine_qualifier(pMACHINE_QUALIFIER pqualifier)
{
	if (!(pqualifier->modFlags & ACTIONS_RETURN_FLAGS))
	{
		pID_INFO pid_event;
		/* note that this is not added to the machine event list;
			it is here only to be found as an event id for return
			decls.
		*/
		add_id(id_list, EVENT,"noEvent",&pid_event);
		pid_event->powningMachine = pmachineInfo;
		pid_event->order          = NO_EVENT; // This makes it easier to detect.
	}

	/* as with 'noEvent', we need this to support return decls */
	pID_INFO pid_state;
	add_id(id_list, STATE, "noTransition",&pid_state);
	pid_state->powningMachine = pmachineInfo;
	pid_state->order          = NO_TRANSITION;  // This makes it easier to detect.

	pmachineInfo->modFlags |= pqualifier->modFlags;
}

pMACHINE_INFO capture_machine(pMACHINE_PREFIX pprefix
							  , pMACHINE_QUALIFIER pqualifier
							  , pSTATEMENT_DECL_LIST psdl
							 )
{
	pMACHINE_INFO retVal = pprefix->pmachineInfo;

	retVal->modFlags          |= pqualifier->modFlags;
	retVal->machineTransition  = pqualifier->machineTransition;
	retVal->native_impl_prologue = pqualifier->native_impl_prologue;
	retVal->native_impl_epilogue = pqualifier->native_impl_epilogue;

	/* harvest the lists */
	retVal->data               = psdl->data;
	retVal->state_list         = psdl->pstate_and_event_decls->state_decls;
	retVal->event_list         = psdl->pstate_and_event_decls->event_decls;
	retVal->action_list        = psdl->pactions_and_transitions->action_list;
	retVal->action_info_list   = psdl->pactions_and_transitions->action_info_list;
	retVal->transition_list    = psdl->pactions_and_transitions->transition_list;
	retVal->transition_fn_list = psdl->pactions_and_transitions->transition_fn_list;
	retVal->machine_list       = psdl->pactions_and_transitions->machine_list;
	retVal->sequences          = psdl->sequences;

	count_external_declarations(retVal->event_list, &(retVal->external_event_designation_count));
	count_parent_event_referenced(retVal->event_list, &(retVal->parent_event_reference_count));
	count_shared_events(retVal->event_list, &(retVal->shared_event_count));
	count_event_user_data_attributes(retVal->event_list
									 , &(retVal->data_translator_count)
									 , &(retVal->data_block_count)
									);

	/* sanity checks */
	if (retVal->parent && retVal->data_block_count && !output_generated_file_names_only)
	{
		yyerror("event user data not allowed in sub-machines");
	}


	if (retVal->data_translator_count && !retVal->data && !output_generated_file_names_only)
	{
		if (!(retVal->modFlags & mfTranslatorsReturnEvents))
		{
			yyerror("data translators returning void are "
					"not allowed for machines having no data"
				   );
		}
	}

	count_external_declarations(retVal->state_list
								, &(retVal->external_state_designation_count)
							   );
	count_states_with_entry_exit_fns(retVal->state_list
									 , &(retVal->states_with_entry_fns_count)
									 , &(retVal->states_with_exit_fns_count)
									);

	retVal->executes_fns_on_state_transitions = (
												 ((retVal->states_with_entry_fns_count + retVal->states_with_exit_fns_count) > 0)
												 || (retVal->machineTransition != NULL)
												);
	if (retVal->machine_list)
	{
		count_sub_machine_inhibitors(retVal->state_list, &(retVal->submachine_inhibitor_count));
	}

	if (allocateActionArray(retVal)) yyerror("out of memory");

	if (retVal->parent && !retVal->parent->heterogeneous_children)
	{
		retVal->parent->heterogeneous_children = ((retVal->parent->modFlags & ACTIONS_RETURN_FLAGS)
												  != (retVal->modFlags & ACTIONS_RETURN_FLAGS)
												 );
	}
	enumerate_pid_list(retVal->state_list);
	enumerate_pid_list(retVal->event_list);

	count_states_implemented_by_machine(retVal->state_list
										, &(retVal->states_implemented_by_machine)
									   );

	count_translators_implemented_by_machine(retVal->event_list
											 , &(retVal->translators_implemented_by_machine)
											);

	if (populate_action_array(retVal, yyout)) yyerror("Action array population failed");

	count_states_with_zero_events(retVal->state_list
								  , &(retVal->states_with_zero_events)
								 );
	count_states_with_one_event(retVal->state_list
								, &(retVal->states_with_one_event)
							   );
	count_states_with_no_way_in(retVal->state_list
								, &(retVal->states_with_no_way_in)
							   );
	count_states_with_no_way_out(retVal->state_list
								 , &(retVal->states_with_no_way_out)
								);
	count_events_with_zero_handlers(retVal->event_list
									, &(retVal->events_with_zero_handlers)
								   );
	count_events_with_one_handler(retVal->event_list
								  , &(retVal->events_with_one_handler)
								 );

	compute_event_and_state_density_pct(retVal);

/* reset context */
	pmachineInfo = retVal->parent;
	if (retVal->parent)
	{
		id_list = retVal->parent->id_list;
		pmachineInfo = retVal->parent;
	}
	else
	{
		id_list = root_id_list;
	}

#ifdef PARSER_DEBUG

	fprintf(yyout
			, "found a machine named %s\n"
			, retVal->name->name
		   );
	fprintf(yyout
			, "\twith %d events and %d states\n"
			, retVal->event_list->count
			, retVal->state_list->count
		   );

	if (retVal->modFlags & mfReentrant)
	{

		fprintf(yyout, "The machine is reentrant\n");

	}

	fprintf(yyout, "Actions return ");
	if (retVal->modFlags & mfActionsReturnStates)
	{

		fprintf(yyout, "states\n");

	}
	else if (retVal->modFlags & mfActionsReturnVoid)
	{

		fprintf(yyout, "void\n");

	}
	else
	{

		fprintf(yyout, "events\n");

	}

	fprintf(yyout, "Translators return ");
	if (retVal->modFlags & mfTranslatorsReturnEvents)
	{

		fprintf(yyout, "events\n");

	}
	else
	{

		fprintf(yyout, "void\n");

	}

	if (retVal->machineTransition)
	{
		fprintf(yyout
				, "on transition: %s\n"
				, retVal->machineTransition->name
			   );
	}

	fprintf(yyout, "The states :\n");
	parser_debug_print_state_list(retVal->state_list, yyout);

	fprintf(yyout, "The events :\n");
	parser_debug_print_event_list(retVal->event_list, yyout);

	if (retVal->parent_event_reference_count > 0)
	{
		fprintf(yyout
				, "%d events reference the parent machine.\n"
				, retVal->parent_event_reference_count);
	}

	if (retVal->sequences)
	{
		fprintf(yyout
				, "There %s %d event sequence%s given:\n"
				, retVal->sequences->count == 1 ? "is" : "are"
				, retVal->sequences->count
				, retVal->sequences->count == 1 ? "" : "s"
			   );

		parser_debug_print_event_sequences(retVal, yyout);

	}

	fprintf(yyout, "The actions :\n");
	parser_debug_print_action_list_deep(retVal->action_list, retVal, yyout);

	fprintf(yyout, "\nThe %d transitions :\n"
			, retVal->transition_list->count
		   );
	parser_debug_print_transition_list(retVal->transition_list, yyout);

	if (retVal->transition_fn_list->count)
	{
		fprintf(yyout, "\nThe %d transition functions :\n"
				, retVal->transition_fn_list->count
			   );

		parser_debug_print_transition_fn_list(retVal->transition_fn_list, yyout);
	}

	if (retVal->data)
	{
		fprintf(yyout
				, "this machine has data\n"
			   );

		parser_debug_print_data_block(retVal->data, yyout);

	}

	if (retVal->machine_list)
	{
		fprintf(yyout
				, "this machine has %u sub-machines\n"
				, retVal->machine_list->count
			   );

		fprintf(yyout
				, "the sub-machine depth is %u\n"
				, retVal->sub_machine_depth
			   );
	}

	fprintf(yyout, "\n");
#endif

	return retVal;
}

