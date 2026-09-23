/**
*  fsm_c_utils.c
*
*    Additional (cf ancestry.[ch]) ancestry functions.
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

#include "fsm_c_utils.h"

#include "ancestry.h"

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

pCMachineData ultimatePcmdAncestor(pCMachineData pcmd)
{
	return pcmd->parent_pcmd
			? ultimatePcmdAncestor(pcmd->parent_pcmd)
			: pcmd
			;
}

unsigned count_generations(pCMachineData pcmd)
{
	unsigned num_generations = 0;
	for (pCMachineData pcmd_iterator = pcmd->parent_pcmd;
		 pcmd_iterator;
		 pcmd_iterator = pcmd_iterator->parent_pcmd
		 )
	{
		num_generations++;
	}

	return num_generations;
}

pCMachineData nth_generation(pCMachineData pcmd, unsigned generation)
{
	pCMachineData nth_generation = pcmd;
	for (unsigned gen_count = generation; gen_count; gen_count--)
	{
		nth_generation = nth_generation->parent_pcmd;
	}
	return nth_generation;
}

void print_ancestor_macro(FILE *fout, pCMachineData pcmd)
{
	if (pcmd->parent_pcmd)
	{
		fprintf(fout, "PARENT");
		if (pcmd->parent_pcmd->parent_pcmd)
		{
			fprintf(fout, "_");
			print_ancestor_macro(fout, pcmd->parent_pcmd);
		}
	}
}

void define_ancestor_macros(FILE *fout, pCMachineData pcmd)
{
	pCMachineData pcmd_iterator;
	unsigned      num_generations;
	for (pcmd_iterator = pcmd, num_generations = count_generations(pcmd);
		 pcmd_iterator && pcmd_iterator->parent_pcmd;
		 pcmd_iterator = pcmd_iterator->parent_pcmd, num_generations--
		)
	{
		fprintf(fout
				, "#undef "
				);
		print_ancestor_macro(fout, pcmd_iterator);
		fprintf(fout
				, "\n#define "
				);
		print_ancestor_macro(fout, pcmd_iterator);
		fprintf(fout
				, "(A) %s_##A\n"
				, fqMachineName(nth_generation(pcmd, num_generations))
			   );
	}
}

void print_ancestor_case_statements(FILE *fout, pCMachineData pcmd, char *indent, char *name)
{
	for (pCMachineData pcmd_iterator = pcmd;
		 pcmd_iterator && pcmd_iterator->parent_pcmd;
		 pcmd_iterator = pcmd_iterator->parent_pcmd
		)
	{
		fprintf(fout
				, "%scase "
				, indent
				);
		print_ancestor_macro(fout, pcmd_iterator);
		fprintf(fout
				, "(%s):\n"
				, name
				);
	}
}

