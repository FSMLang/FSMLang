/**
*  ancestry.h
*
*    Manages names for variables and types.
*
*    FSMLang (fsm) - A Finite State Machine description language.
*    Copyright (C) 2023  Steven Stanton
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

#ifndef ANCESTRY_H
#define ANCESTRY_H

#include <stdio.h>

#include "fsm_priv.h"
#include "fsm_c_common.h"

char * actionFnType(pCMachineData);
char * actionReturnType(pCMachineData);
char * actionTransType(pCMachineData);
char * createAncestryFileName(pMACHINE_INFO);
char * dataTranslationFnType(pCMachineData);
char * eventType(pCMachineData);
char * fqMachineName(pCMachineData);
char * ufMachineName(pCMachineData);
char * ucMachineName(pCMachineData);
char * ucfqMachineName(pCMachineData);
char * fsmDataType(pCMachineData);
char * fsmFnEventType(pCMachineData);
char * subFsmFnEventType(pCMachineData);
char * subFsmFnReturnType(pCMachineData);
char * fsmFnType(pCMachineData);
char * fsmType(pCMachineData);
char * hungarianToUnderbarCaps(char *);
char * machineName(pCMachineData);
char * nfMachineName(pCMachineData);
char * ucnfMachineName(pCMachineData);
char * nfMachineNamePmi(pMACHINE_INFO,char**);
char * fqMachineNamePmi(pMACHINE_INFO,char**);
char * ucfqMachineNamePmi(pMACHINE_INFO,char**);
char * sharedEventStrType(pCMachineData);
char * stateFnType(pCMachineData);
char * stateType(pCMachineData);
void   streamHungarianToUnderbarCaps(FILE*,char *);
char * subFsmIfType(pCMachineData);
char * subMachineFnType(pCMachineData);
char * transitionFnType(pCMachineData);
char * instanceType(pCMachineData);

#endif

