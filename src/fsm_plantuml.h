/**
  fsm_html.h

    Declarations, etc. for fsm_plantuml.c

    FSMLang (fsm) - A Finite State Machine description language.
    Copyright (C) 2022  Steven Stanton

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

    Steven Stanton fsmlang@pesticidesoftware.com

    For the latest on FSMLang: http://fsmlang.pesticidesoftware.com

    And, finally, your possession of this source code implies nothing.

  Creation: 	sstanton		Nov-26-2022
*/
#ifndef _FSM_PLANTUML_H
#define _FSM_PLANTUML_H

#include "fsm_priv.h"

pFSMOutputGenerator generatePlantUMLMachineWriter(FSMOGF_TYPE);

#endif  /*  ----------------------------- Nothing below this line  -----------------  */
