// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
//  NotifyMulticastDefines
//
//
//  (c) 2002
//  Department of Neural Information Processing, University of Ulm, Germany
//
//
//  Authors:
//    Philipp Baer <phbaer@openums.org>
//
//
//  Version:
//    1.0.3
//
//
//  Description:
//
//    Defines/macros for NotifyMulticast
//
//
//  $Id$
//
//////////////////////////////////////////////////////////////////////////////

#ifndef notifyMulticastHeader_hh
#define notifyMulticastHeader_hh

#include <string>

#define HEADER_SIZE 32
#define DEFAULT_MTU 1500

/* Debug output macros */
#define DEBUG_LEVEL 32

#define DBG_TOOMUCH 64
#define DBG_VERBOSE 32
#define DBG_MORE    16
#define DBG_INFO    8
#define DBG_ERROR   0
#define DBG_ALWAYS  0

/* Debug macros */
#define _PRINT_X(l, fd, x) if (DEBUG_LEVEL >= (l)) fd << DBG_CLASSNAME << ": " << x << std::endl;

#define PRINT(x) _PRINT_X(DBG_ALWAYS, std::cout, x)
#define PRINT_ERR(x) _PRINT_X(DBG_ERROR, std::cerr, x)

#ifdef DEBUG
#  define PRINT_DBG(l, x) _PRINT_X(l, std::cout, x)
#else
#  define PRINT_DBG(l, x)
#endif

/* needed for QT/ACE compatibility */
//#define QT_GENUINE_STR

#undef RRC_DEBUG

#define LOG(cfg, x) { \
    if ((cfg)->logfile() != NULL) { \
    (*((cfg)->logfile())) << (cfg)->datetime() << x << std::endl; \
    } \
    }

#endif
