// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef miroDebug_h
#define miroDebug_h

#include <ace/Log_Msg.h>

// The following ASSERT macro is courtesy of Alexandre Karev
// <akg@na47sun05.cern.ch>.
#if defined (MIRO_NDEBUG)
#define MIRO_ASSERT(x)
#elif !defined (MIRO_ASSERT)
#define MIRO_ASSERT(X) \
  do { if(!(X)) { \
  int __ace_error = ACE_OS::last_error (); \
  ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
  ace___->set (__FILE__, __LINE__, -1, __ace_error, ace___->restart (), \
               ace___->msg_ostream (), ace___->msg_callback ()); \
  ace___->log (LM_ERROR, ACE_LIB_TEXT ("ACE_ASSERT: file %N, line %l assertion failed for '%s'.%a\n"), #X, -1); \
  } } while (0)
#endif  // MIRO_NDEBUG

#if defined (MIRO_NLOGGING)
#define MIRO_HEX_DUMP(X) do {} while (0)
#define MIRO_RETURN(Y) do { return (Y); } while (0)
#define MIRO_ERROR_RETURN(X, Y) return (Y)
#define MIRO_ERROR_BREAK(X) { break; }
#define MIRO_ERROR(X) do {} while (0)
#define MIRO_DEBUG(X) do {} while (0)
#define MIRO_ERROR_INIT(VALUE, FLAGS)
#else
#define MIRO_HEX_DUMP(X) \
  do { \
    int __ace_error = ACE_OS::last_error (); \
    ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
    ace___->conditional_set (__FILE__, __LINE__, 0, __ace_error); \
    ace___->log_hexdump X; \
  } while (0)
#define MIRO_RETURN(Y) \
  do { \
    int __ace_error = ACE_OS::last_error (); \
    ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
    ace___->set (__FILE__, __LINE__, Y, __ace_error, ace___->restart (), \
                 ace___->msg_ostream (), ace___->msg_callback ()); \
    return Y; \
  } while (0)
#define MIRO_ERROR_RETURN(X, Y) \
  do { \
    int __ace_error = ACE_OS::last_error (); \
    ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
    ace___->conditional_set (__FILE__, __LINE__, Y, __ace_error); \
    ace___->log X; \
    return Y; \
  } while (0)
#define MIRO_ERROR(X) \
  do { \
    int __ace_error = ACE_OS::last_error (); \
    ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
    ace___->conditional_set (__FILE__, __LINE__, -1, __ace_error); \
    ace___->log X; \
  } while (0)
#define MIRO_DEBUG(X) \
  do { \
    int __ace_error = ACE_OS::last_error (); \
    ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
    ace___->conditional_set (__FILE__, __LINE__, 0, __ace_error); \
    ace___->log X; \
  } while (0)
#define MIRO_ERROR_INIT(VALUE, FLAGS) \
  do { \
    ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
    ace___->set_flags (FLAGS); ace___->op_status (VALUE); \
  } while (0)
#endif // MIRO_NLOGGING

#define MIRO_LOG(X) \
  do { \
    int __ace_error = ACE_OS::last_error (); \
    ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
    ace___->conditional_set (__FILE__, __LINE__, 0, __ace_error); \
    ace___->log X; \
  } while (0)

#define MIRO_LOG_CTOR(X) \
  do { \
    ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
    ace___->conditional_set (__FILE__, __LINE__, 0, 0); \
    ace___->log((ACE_Log_Priority)1024, "%C", "Constructing " ## X); \
  } while (0)

#define MIRO_LOG_DTOR(X) \
  do { \
    ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
    ace___->conditional_set (__FILE__, __LINE__, 0, 0); \
    ace___->log((ACE_Log_Priority)1024, "%C", "Destructing " ## X); \
  } while (0)

#endif // miroDebug_h
