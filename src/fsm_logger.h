/**
 *    fsm_logger.h
 *
 *    Logging subsystem for FSMLang.
 *
 *    FSMLang (fsm) - A Finite State Machine description language.
 *    Copyright (C) 2023  Daniel Jasinski
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
 *    Daniel Jasinski
 *    jasinsky.me
 *
 *    For the latest on FSMLang: https://fsmlang.github.io
 *
 *    And, finally, your possession of this source code implies nothing.
 *
 *    File created by Daniel Jasinski
 *
 */

#ifndef FSM_LOGGER
#define FSM_LOGGER

#include <stdio.h>

#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_LVL_DBG
#endif

#ifndef CONFIG_LOG_NL
#define CONFIG_LOG_NL "\r\n"
#endif

#ifndef LOG_DETAIL
#define LOG_DETAIL LOG_DETAIL_NONE
#endif

#define LOG_LVL_NONE 0
#define LOG_LVL_CRT 1
#define LOG_LVL_ERR 2
#define LOG_LVL_WRN 3
#define LOG_LVL_INF 4
#define LOG_LVL_DBG 5

#define LOG_DETAIL_NONE 0
#define LOG_DETAIL_FULL 1

#define __FILENAME__ (__builtin_strrchr("/"__FILE__, '/') + 1)

#if LOG_DETAIL == LOG_DETAIL_FULL
#define LOG_IMPL(log_level, format, ...)                                                   \
    do                                                                                     \
    {                                                                                      \
        printf("<%s:%d:%s> " format "%s", log_level, __LINE__, __FILENAME__, __VA_ARGS__); \
    } while (0)
#elif LOG_DETAIL == LOG_DETAIL_NONE
#define LOG_IMPL(log_level, format, ...)  \
    do                                    \
    {                                     \
        (void)log_level;                  \
        printf(format "%s", __VA_ARGS__); \
    } while (0)
#else
#error "Erronous log detail configuration, check your build system!"
#endif

#if ((LOG_LEVEL) >= (LOG_LVL_CRT))
#undef LOG_CRT
#define LOG_CRT(...) LOG_IMPL("CRT", __VA_ARGS__, CONFIG_LOG_NL)
#endif
#if ((LOG_LEVEL) >= (LOG_LVL_ERR))
#undef LOG_ERR
#define LOG_ERR(...) LOG_IMPL("ERR", __VA_ARGS__, CONFIG_LOG_NL)
#endif
#if ((LOG_LEVEL) >= (LOG_LVL_WRN))
#undef LOG_WRN
#define LOG_WRN(...) LOG_IMPL("WRN", __VA_ARGS__, CONFIG_LOG_NL)
#endif
#if ((LOG_LEVEL) >= (LOG_LVL_INF))
#undef LOG_INF
#define LOG_INF(...) LOG_IMPL("INF", __VA_ARGS__, CONFIG_LOG_NL)
#endif
#if ((LOG_LEVEL) >= (LOG_LVL_DBG))
#undef LOG_DBG
#define LOG_DBG(...) LOG_IMPL("DBG", __VA_ARGS__, CONFIG_LOG_NL)
#endif

#ifndef LOG_CRT
#define LOG_CRT(...)
#endif

#ifndef LOG_ERR
#define LOG_ERR(...)
#endif

#ifndef LOG_WRN
#define LOG_WRN(...)
#endif

#ifndef LOG_INF
#define LOG_CRT(...)
#endif

#ifndef LOG_DBG
#define LOG_CRT(...)
#endif

#define TUI_IMPL(format, ...)             \
    do                                    \
    {                                     \
        printf(format "%s", __VA_ARGS__); \
    } while (0)

#define FSM_TUI(...) TUI_IMPL(__VA_ARGS__, CONFIG_LOG_NL)

#endif // !FSM_LOGGER