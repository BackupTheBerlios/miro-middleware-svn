// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef miro_Log_h
#define miro_Log_h

#include <ace/Log_Msg.h>
#include <sstream>

// Macro definitions for logging and debug information
//
// MIRO_ASSERT: the standart assert macro. 
// It is provided to enable disabling of assert macros in inline code
// of Miro without disabling them for user code too.
//
// MIRO_LOG: the standart ACE_LOG macro syntax.
// The first argument is the log level/priority.
// The second argument is (Category, Message, ...);
//
// MIRO_LOG_OSTR: a logging macro, providing an ostream.
// This enables the use of ostream operators for logging output.
// The data is first buffered in a stringstream.
//
// The MIRO_DBG* macros are used so we can separate
// debug and logging output.
//
// MIRO_DBG: the standart ACE_LOG macro syntax.
// The first argument is the log level/priority.
// The second argument is (Category, Message, ...);
//
// MIRO_DBG_OSTR: a logging macro, providing an ostream.
// This enables the use of ostream operators for logging output.
// The data is first buffered in a stringstream.
//
// MIRO_DBG_HEX_DUMP: logging of a hex dump.
// Untested up to now.
//
// MIRO_DBG_TRACE: trace macro.
// It uses the ctor/dtor of a local variable to 
// set/reset the trace depth.

#if defined (MIRO_NO_DEBUG)
#define MIRO_ASSERT(X) do {} while (0)
#elif !defined (MIRO_ASSERT)
#define MIRO_ASSERT(X) assert(X)
#endif  // MIRO_NO_DEBUG

#if defined (MIRO_NO_LOGGING)

#define MIRO_LOG(L, X) do {} while (0)
#define MIRO_LOG_OSTR(L, O) do {} while (0)
#define MIRO_DBG(L, C, X) do {} while (0)
#define MIRO_DBG_OSTR(L, C, O) do {} while (0)
#define MIRO_DBG_HEX_DUMP(N, X) do {} while (0)
#define MIRO_DBG_TRACE(X) do {} while (0)

#else // !MIRO_NO_LOGGING

#define MIRO_LOG(L, X) \
  do { \
    if (::Miro::Log::level() >= ::Miro::Log::L && \
	::Miro::Log::enabled(::Miro::Log::ll2LM(::Miro::Log::L))) { \
      ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
      ace___->conditional_set (__FILE__, __LINE__, 0, 0); \
      ace___->log(::Miro::Log::ll2LM(::Miro::Log::L), \
                  ::Miro::Log::format(), X); \
    } \
  } while (0)
#define MIRO_LOG_OSTR(L, O) \
  do { \
    if (::Miro::Log::level() >= ::Miro::Log::L && \
	::Miro::Log::enabled(::Miro::Log::ll2LM(::Miro::Log::L))) { \
      ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
      std::ostringstream ostr__; \
      ostr__ << O; \
      ace___->log(::Miro::Log::ll2LM(::Miro::Log::L), \
                  ::Miro::Log::format(), ostr__.str().c_str()); \
    } \
  } while (0)

#if defined (MIRO_NO_DEBUG)

#define MIRO_DBG(L, C, X) do {} while (0)
#define MIRO_DBG_OSTR(L, C, O) do {} while (0)
#define MIRO_DBG_TRACE(X) do {} while (0)

#else // !MIRO_NO_DEBUG

#define MIRO_DBG(L, C, X) \
  do { \
    if (::Miro::Log::level() >= L && \
	::Miro::Log::enabled(C)) { \
      ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
      ace___->conditional_set (__FILE__, __LINE__, 0, 0); \
      ace___->log(static_cast<ACE_Log_Priority>(C), \
		  ::Miro::Log::format(), X); \
    } \
  } while (0)
#define MIRO_DBG_OSTR(L, C, O) \
  do { \
    if (::Miro::Log::level() >= L && \
	::Miro::Log::enabled(C)) { \
      ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
      std::ostringstream ostr__; \
      ostr__ << O; \
      ace___->log(static_cast<ACE_Log_Priority>(C), \
		  ::Miro::Log::format(), ostr__.str().c_str()); \
    } \
  } while (0)
// Debug HexDump
#define MIRO_DBG_HEX_DUMP(L, X) \
  do { \
    unsigned int n__ = L; \
    if (::Miro::Log::level() >= n__) { \
     int __ace_error = ACE_OS::last_error (); \
      ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
      ace___->conditional_set (__FILE__, __LINE__, 0, __ace_error); \
      ace___->log_hexdump X; \
    } \
  } while (0)
// Constructor log message (Miro::Log::level() >= 1)
#define MIRO_LOG_CTOR(X) \
  do { \
    if (::Miro::Log::level() >= ::Miro::Log::LL_CTOR_DTOR) { \
      ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
      ace___->conditional_set (__FILE__, __LINE__, 0, 0); \
      ace___->log(LM_DEBUG, "Constructing " ## X ## ".\n"); \
    } \
  } while (0)
// Constructor log message (Miro::Log::level() >= 1)
#define MIRO_LOG_CTOR_END(X) \
  do { \
    if (::Miro::Log::level() >= ::Miro::Log::LL_CTOR_DTOR) { \
      ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
      ace___->conditional_set (__FILE__, __LINE__, 0, 0); \
      ace___->log(LM_DEBUG, "Construction of " ## X ## " fininshed.\n"); \
    } \
  } while (0)
// Destructor log message (Miro::Log::level() >= 1)
#define MIRO_LOG_DTOR(X) \
  do { \
    if (::Miro::Log::level() >= ::Miro::Log::LL_CTOR_DTOR) { \
      ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
      ace___->conditional_set (__FILE__, __LINE__, 0, 0); \
      ace___->log(LM_DEBUG, "Destructing " ## X ## ".\n"); \
    } \
  } while (0)
// Destructor log message (Miro::Log::level() >= 1)
#define MIRO_LOG_DTOR_END(X) \
  do { \
    if (::Miro::Log::level() >= ::Miro::Log::LL_CTOR_DTOR) { \
      ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
      ace___->conditional_set (__FILE__, __LINE__, 0, 0); \
      ace___->log(LM_DEBUG, "Destruction of " ## X ## " finished.\n"); \
    } \
  } while (0)

#define MIRO_DBG_TRACE(X) \
  ::Miro::Trace X


#endif // !MIRO_NO_DEBUG

#endif // !MIRO_NO_LOGGING

namespace Miro 
{
  class Log
  {
  public:
    static void init(int& argc, char * argv[]);
    static unsigned int level() throw();
    static void level(unsigned int _level) throw();
    static unsigned int mask() throw();
    static void mask(unsigned int _mask) throw();
    static bool enabled(int _prioriy);
    static bool compiledWithLog();
    static bool compiledWithDebug();
    static ACE_Log_Priority ll2LM(unsigned int _level);
    static void format(char const * _format);
    static char const * format();

    static int const MIRO =    0x00000800;
    static int const VIDEO =   0x00001000;
    static int const PSOS =    0x00002000;
    static int const SPHINX =  0x00004000;
    static int const PIONEER = 0x00010000;
    static int const CAN =     0x00020000;
    static int const FAUL =    0x00040000;
    static int const SPARROW = 0x00080000;
    static int const MCP =     0x00100000;
    static int const ABUS =    0x00200000;
    static int const MSP =     0x00300000;
    static int const SICK =    0x00400000;
    static int const DTLK =    0x00800000;
    static int const DP =      0x01000000;
    static int const B21 =     0x02000000;


    static unsigned int const LL_EMERGENCY = 0;
    static unsigned int const LL_ALERT = 1;
    static unsigned int const LL_CRITICAL = 2;
    static unsigned int const LL_ERROR = 3;
    static unsigned int const LL_WARNING = 4;
    static unsigned int const LL_NOTICE = 5;
    static unsigned int const LL_CTOR_DTOR = 6;
    static unsigned int const LL_DEBUG = 7;
    static unsigned int const LL_TRACE = 8;
    static unsigned int const LL_PRATTLE = 9;

    static unsigned int const MAX_LOG_LEVEL = LL_NOTICE;
    static unsigned int const MIN_DBG_LEVEL = LL_CTOR_DTOR;
    static unsigned int const MAX_DBG_LEVEL = LL_PRATTLE;

    class Trace
    {
    public:
      Trace(unsigned int _level = LL_TRACE,
	    int _priority = LM_DEBUG,
	    char const * _fun = __PRETTY_FUNCTION__);
      ~Trace();
    protected:
      unsigned int level_;
      int priority_;
      char const * const fun_;
    };

  protected:

    static unsigned int level_;
    static unsigned int mask_;
    static char const * format_;
    static ACE_Log_Priority aceLM_[MAX_LOG_LEVEL + 1];
  };

  inline
  Log::Trace::Trace(unsigned int _level, int _priority, char const * _fun) :
    level_(_level),
    priority_(_priority),
    fun_(_fun)
  {
//    MIRO_DBG(level_, priority_, std::string("Entering ") + fun_);
  }
  inline
  Log::Trace::~Trace() {
//    MIRO_DBG(level_, priority_, std::string("Leaving ") + fun_);
  }
  inline
  unsigned int
  Log::level() throw() {
    return level_;
  }
  inline
  void
  Log::level(unsigned int _level) throw() {
    level_ = _level;
  }
  inline
  unsigned int
  Log::mask() throw() {
    return mask_;
  }
  inline
  void
  Log::mask(unsigned int _mask) throw() {
    mask_ = _mask;
  }

  inline bool Log::compiledWithLog()
  {
#if defined (MIRO_NO_LOGGING)
     return false;
#else
     return true;
#endif
  }

  inline bool Log::compiledWithDebug()
  {
#if defined (MIRO_NO_DEBUG)
     return false;
#else
     return true;
#endif
  }

  inline
  bool
  Log::enabled(int _priority) {
    return 
      ACE_Log_Msg::instance()->
      log_priority_enabled(static_cast<ACE_Log_Priority>(_priority));
  }
  inline
  char const *
  Log::format() {
    return format_;
  }
  inline
  void
  Log::format(char const * _format) {
    format_ = _format;
  }
  inline
  ACE_Log_Priority
  Log::ll2LM(unsigned int _level) {
    assert(_level <= MAX_LOG_LEVEL);
    return aceLM_[_level];
  }

}
#endif // miro_Log_h
