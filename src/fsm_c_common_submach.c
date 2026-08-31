/**
*  fsm_c_common_submach.c
*
*    Submachine related common C stuff.
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

#include "fsm_c_common_submach.h"
#include "ancestry.h"
#include "list.h"

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

static void print_parent_event_sharer_signature(pCMachineData,pID_INFO,FILE*,DECLARE_OR_DEFINE);
static bool define_needed_parent_event_sharers(pLIST_ELEMENT, void *);
static bool declare_needed_parent_event_sharers(pLIST_ELEMENT, void *);

/**
 * Print the signature of the parent event sharing function,
 * suitable for a declaration or the start of the definition.
 * 
 * @author Steven Stanton (8/27/2026)
 * 
 * @param pcmd   Data from the governing Output Generator
 * @param fout   Destination file
 * @param dod    Print for declaration when dod == dod_declare;
 *  			 otherwise, print for start of definition.
 */
static void print_parent_event_sharer_signature(pCMachineData pcmd, pID_INFO pevent, FILE *fout, DECLARE_OR_DEFINE dod)
{
	fprintf(fout
			, "%s share_parent_event_%s(%s%s%s)%s\n"
			, eventType(pcmd)
			, pevent->name
			, generate_instance ? "unsigned" : "p"
			, generate_instance ? "" : fsmType(pcmd)
			, dod == dod_declare
			  ? ""
			  : generate_instance
				? " instance" : " pfsm"
			, dod == dod_declare ? ";" : "\n{"
			);
}

bool find_legitimate_sharer(pLIST_ELEMENT pelem, void *data)
{
	pMACHINE_INFO             pmi  = (pMACHINE_INFO) pelem->mbr;
	pITERATOR_CALLBACK_HELPER pich = (pITERATOR_CALLBACK_HELPER) data;

	return (
			(pmi != pich->ih.pmi)
			&& (!(pmi->modFlags & mfStateImplementing))
			&& (!(pmi->modFlags & ACTIONS_RETURN_FLAGS))
			);
}


/**
 * Create switch cases for each event that a parent shares to
 * other machines.
 * 
 * @author Steven Stanton (8/26/2026)
 * 
 * @param pelem  The event record
 * @param data   Must be a pointer to an
 *  			 ITERATOR_CALLBACK_HELPER.
 * 
 * @return bool Always "false."
 */
static bool define_needed_parent_event_sharers(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO                  pevent = (pID_INFO) pelem->mbr;
	pITERATOR_CALLBACK_HELPER pich   = (pITERATOR_CALLBACK_HELPER) data;

	pEVENT_DATA ped = &pevent->type_data.event_data;

	pich->ih.pid = pevent;

	if (ped->psharing_sub_machines
//		&& (pich->ih.pmi->modFlags & mfStateImplementing)
		&& iterate_list(ped->psharing_sub_machines, find_legitimate_sharer, pich)
		)
	{
		print_parent_event_sharer_signature(pich->pcmd, pevent, pich->ih.fout, dod_define);

		if (generate_instance)
		{
			fprintf(pich->ih.fout
					, "\tp%s pfsm = %s_INSTANCES[instance];\n"
					, fsmType(pich->pcmd)
					, machineName(pich->pcmd)
					);
		}

		fprintf(pich->ih.fout
				, "\treturn %s_pass_shared_event(pfsm, sharing_%s_%s);\n"
				, machineName(pich->pcmd)
				, machineName(pich->pcmd)
				, pevent->name
				);

		fprintf(pich->ih.fout
				, "}\n\n"
				);
	}

	return false;
}

static bool declare_needed_parent_event_sharers(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO                  pevent = (pID_INFO) pelem->mbr;
	pITERATOR_CALLBACK_HELPER pich   = (pITERATOR_CALLBACK_HELPER) data;

	pEVENT_DATA ped = &pevent->type_data.event_data;

	pich->ih.pid = pevent;

	if (ped->psharing_sub_machines
//		&& (pich->ih.pmi->modFlags & mfStateImplementing)
		&& iterate_list(ped->psharing_sub_machines, find_legitimate_sharer, pich)
		)
	{
		print_parent_event_sharer_signature(pich->pcmd
											, pevent
											, pich->ih.fout
											, dod_declare
											);

	}

	return false;
}

void define_needed_event_sharing_functions(pFSMCOutputGenerator pfsmcog)
{
	
	ITERATOR_CALLBACK_HELPER ich = {
		.ih = {
			.pmi = pfsmcog->pcmd->pmi
			, .fout = pfsmcog->pcmd->cFile
		}
		, .pcmd = pfsmcog->pcmd
	};

	iterate_list(pfsmcog->pcmd->pmi->event_list
				 , define_needed_parent_event_sharers
				 , &ich
				 );

}

void declare_needed_event_sharing_functions(pFSMCOutputGenerator pfsmcog)
{
	
	ITERATOR_CALLBACK_HELPER ich = {
		.ih = {
			.pmi = pfsmcog->pcmd->pmi
			, .fout = generate_instance
			          ? pfsmcog->pcmd->subMachineHFile
					  : pfsmcog->pcmd->pubHFile
		}
		, .pcmd = pfsmcog->pcmd
	};

	iterate_list(pfsmcog->pcmd->pmi->event_list
				 , declare_needed_parent_event_sharers
				 , &ich
				 );

}

