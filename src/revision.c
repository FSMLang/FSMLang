/**
 * revision.c 
 *  
 * Revision information. 
 */

/* 
  Revision 1.1 separates the action/transition array from the machine structure.  Two
  benefits proceed from this: first, the array can be made constant; and second, the machine
  structure can now be instantiated multiple times.  A macro is provided by which machines may
  be easily declared.
*/ 

/*
   Revision 1.2 adds a new option: "-i0" will inhibit the creation of an instance of the machine.
   Any other argument to -i, or not having -i0 will cause the creation of an instance, which is
   the historical behavior.
*/

/*
   Revision 1.3 quiets a warning about  the shadowing of a global variable by a local variable; the warning
   was correct, but the shadowing was not consequential.  In the spirit of "no warnings are the only good warnings"....
*/

/*
   Revision 1.4 adds two new functions:
      * transitions may now be done through functions, rather than designated states.  As with actions, these
        are not previously declared; they are simply stated.  The function arguments are first a pointer to the
        machine, followed by the event causing the transition.
      * both actions and transitions may be given return declarations.  These are in the form of "name returns ...;"
        where "name" is a previously seen action or transition function name.  Presently, this information is used
        only to enhance the HTML page.
*/

/*
   Revision 1.5 adds a new output type modifier, 's'.  Specifying -ts will create a machine in which each state is
      handled by a function, which switches on the event to execute the appropriate action function and select the
      next state.  This type of machine uses no memory, as there is no event/state table; the machine is
      purely code.
 
      Also, the "actions return states" machine is fixed.
*/

/*
   Revision 1.6 adds a new command line option, -c, which will create a more compact event/state table.  The option is
   currently only meaningful when -tc is used with machines having actions which return events.
 
   Compaction is achieved by creating a table of pointers to the action functions along with an enum to index it.  This enum
   and the state enum are used to create the event/state table basic type; and, the enums are declared with the packed
   attribute.  Thus, for machines having less than 256 actions and states, the size of the basic event/state table type struct
   is only 2 bytes.
 
   When transition functions are used, a similar table/enum pair is constructed.
*/

/*
   Revision 1.7:
      * introduces the concept of transition functions to machines having actios which return states.
      * extends the -c option to machines having actions which return void.  This required no code changes.
      * introduces a new machine qualifier: 'on transition ID;' where ID names a function which will be called
        whenever the state changes.  The function takes a pointer to the machine, and the value of the new state.
*/

/*
   Revision 1.8:
      * introduces Hierarchical state machines.
*/

/*
   Revision 1.9:
 
   Removed the concept of an "active" sub state machine; all sub-machines can
   run based on the event given to the top level machine.
*/

/*
   Revision 1.10:
 
   Create weakly linked stubs for all user functions.  Cygwin
   seems to have trouble with this.  It works for Mingwsa. Have
   not tested Linux.
*/

/*
   Revision 1.11:
 
   Remove line-endings from generated DBG_PRINTF statements.
*/

/*
   Revision 1.12:
 
   "Action returns" statements can now reference sub-machine events 
   and "parent" events.
 
*/

/*
   Revision 1.13:
 
   RUN_STATE_MACHINE and DECLARE_%s_MACHINE now enclose parameters in parentheses. 
*/

/*
   Revision 1.14:
 
   returns statements with multiple entries can now end with a namespace event reference.
*/

/*
   Revision 1.15:
 
   Compact table generation correctly references states and state transition functions.
*/

/*
   Revision 1.16:
 
   C Switch Machines now generate any sub-machines.
*/

/*
   Revision 1.17:
 
   Data initialization is mediated through a macro.
*/

/*
   Revision 1.18:
 
   findAndRunSubMachine not declared when no sub-machines are present.
*/

/*
   Revision 1.19:
 
   --generate-weak-fns=false suppresses the generation of weak function stubs.
*/
const char rev_string[] = "version 1.19+0";

