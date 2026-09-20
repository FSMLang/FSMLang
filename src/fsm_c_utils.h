/**
*  ancestry2.h
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

#ifndef ANCESTRY2_H
#define ANCESTRY2_H

#include "fsm_c_common.h"

pCMachineData ultimatePcmdAncestor(pCMachineData);
unsigned count_generations(pCMachineData);
pCMachineData nth_generation(pCMachineData,unsigned);
void print_ancestor_macro(FILE*,pCMachineData);
void define_ancestor_macros(FILE*,pCMachineData);
void print_ancestor_case_statements(FILE*,pCMachineData,char*,char*);

#endif

