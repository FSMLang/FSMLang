
#ifndef UNIT_TEST_H
#define UNIT_TEST_H

#include <stdbool.h>

/** TEST_FNs know whether or not they pass */
typedef bool (*TEST_FN)(void);

/** VOID_TEST_FNs do not know whether or not they pass */
typedef void (*VOID_TEST_FN)(void);

#endif
