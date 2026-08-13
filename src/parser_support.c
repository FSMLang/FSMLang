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

static pMACHINE_PREFIX create_machine_prefix(pNATIVE_INFO,MOD_FLAGS,pID_INFO);

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
	pmachine->type_data.machine_pid_data.implementedState = pstate;

	return pstate;
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

pMACHINE_PREFIX machine_declared_by_machine_pid(pNATIVE_INFO pnative, MOD_FLAGS machine_modifier, pID_INFO pid)
{
	pMACHINE_PREFIX pmachine_prefix = NULL;

	set_id_type(pid, SI_MACHINE);

	machine_modifier |= mfStateImplementing;

	pmachine_prefix = create_machine_prefix(pnative
											, machine_modifier
											, pid
											);

	if (pmachine_prefix == NULL)
	{
		yyerror("out of memory");
	}

	pid->type_data.machine_pid_data.pmi = pmachine_prefix->pmachineInfo;

	return pmachine_prefix;
}

