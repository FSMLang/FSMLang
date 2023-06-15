/**
 * @file fsm_unused.h
 * @author jasinsky.me
 * @brief Provide unused macro
 * 
 * Unused macro is useful when compiling in -Wall, -Wextra modes. Some of the 
 * function API contracts require argument passing - but then argument is left
 * unused.
 * 
 * The same applies for functions, variables, which are not used in different 
 * build variants
 * 
 * @version 0.1
 * @date 2023-06-14
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef FSM_UNUSED
#define FSM_UNUSED

/**
 * @brief Unused macro handling single argument.
 * 
 */
#define FSMLANG_MAYBE_UNUSED(token) (void)(token)

#endif