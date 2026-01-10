/**
*  util_file_inclusion.h
*
*    Utilities for tracking included files.
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

#ifndef UTIL_FILE_INCLUSION_H
#define UTIL_FILE_INCLUSION_H

#include "list.h"
									
/**
 * Add a filename to the list of included files.
 * 
 * @author Steven Stanton (1/5/2026)
 * 
 * @param filename Name of file being included.
 *
 * This function is intended to allow the lexer to keep a list
 * of the included files it opens during parsing a source file.
 *
 * An internal list anchor is prepared and the filename is
 * strdup'd and added to the list.  Only one list is maintained
 * at a time.  When file parsing is done, the list is obtained
 * by calling <i>get_included_files_list</i>.
 */
void track_included_file(char *filename);

/**
 * Print the list of included files
 * 
 * @author Steven Stanton (1/5/2026)
 * 
 * @param pLIST anchor   This must point to an initialized LIST.
 * 
 * The internal list becomes empty, preparing it for use in
 * parsing the next file.  As a consequence, this function
 * must only be called once at the end of parsing a file.
 */
void print_included_files_list(void);

#endif

