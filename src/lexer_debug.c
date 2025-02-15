/*
	lexer_debug.c


	Contains code, &c. necessary for debugging the stand-alone lexer.

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

#include "lexer_debug.h"


char *strings[]  = {
	"LOOKUP"
	, "MACHINE"
	, "STATE"
	, "EVENT"
	, "TRANSITION "
	, "ACTION"
	, "MACHINE_KEY"
	, "STATE_KEY"
	, "EVENT_KEY"
	, "ACTION_KEY"
	, "TRANSITION_KEY"
	, "ID"
	, "REENTRANT"
	, "ACTIONS"
	, "RETURN"
	, "STATES"
	, "EVENTS"
  , "RETURNS"
  , "EXTERNAL"
  , "ON"
  , "VOID"
  , "PARENT"
  , "NAMESPACE"
  , "DATA_KEY"
  , "STRING"
  , "TYPE_NAME"
  , "FIELD_NAME"
  , "TRANSLATOR_KEY"
  , "NATIVE_KEY"
  , "IMPLEMENTATION_KEY"
  , "INHIBITS"
  , "SUBMACHINES"
  , "ALL"
  , "ENTRY"
  , "EXIT"
  , "NUMERIC_STRING"
  , "STRUCT_KEY"
  , "UNION_KEY"
  , "PROLOGUE_KEY"
  , "EPILOGUE_KEY"
  , "SEQUENCE_KEY"
};

