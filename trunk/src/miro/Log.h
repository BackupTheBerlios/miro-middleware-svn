// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002, 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef miro_Log_h
#define miro_Log_h

#include <ace/Log_Msg.h>
#include <sstream>
#include <cassert>

// Macro definitions for logging and debug information
//
// MIRO_ASSERT: the standart assert macro. 
// It is provided to enable disabling of assert macros in inline code
// of Miro without disabling them for user code too.
//
// MIRO_LOG: 
// The first argument is the log level/priority.
// The second argument is (Category, Message);
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#if defined (MIRO_NO_DEBUG)
#define MIRO_ASSERT(X) do {} while (0)
#elif !defined (MIRO_ASSERT)
#define MIRO_ASSERT(X) assert(X)
#endif  // MIRO_NO_DEBUG

#if defined (MIRO_NO_LOGGING)

#define MIRO_LOG(L, X) do {} while (0)
#define MIRO_LOG_OSTR(L, O) do {} while (0)
#define MIRO_DBG(Category, Loglevel, X) do {} while (0)
#define MIRO_DBG_OSTR(Category, Loglevel, O) do {} while (0)
#define MIRO_DBG_HEX_DUMP(N, X) do {} while (0)
#define MIRO_DBG_TRACE(X) do {} while (0)

#else // !MIRO_NO_LOGGING

#define MIRO_LOG(Loglevel, Output) \
  do { \
    if (::Miro::Log::level() >= ::Miro::Log::Loglevel) { \
      ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
      ace___->conditional_set (__FILE__, __LINE__, 0, 0); \
      ace___->log(::Miro::Log::ll2LM(::Miro::Log::Loglevel), \
                  ::Miro::Log::format(), Output); \
    } \
  } while (0)
#define MIRO_LOG_OSTR(Loglevel, Output) \
  do { \
    if (::Miro::Log::level() >= ::Miro::Log::Loglevel) { \
      ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
      std::ostringstream ostr__; \
      ostr__ << Output; \
      ace___->log(::Miro::Log::ll2LM(::Miro::Log::Loglevel), \
                  ::Miro::Log::format(), ostr__.str().c_str()); \
    } \
  } while (0)

#if defined (MIRO_NO_DEBUG)

#define MIRO_DBG(L, C, X) do {} while (0)
#define MIRO_DBG_OSTR(L, C, O) do {} while (0)
#define MIRO_DBG_TRACE(X) do {} while (0)

#else // !MIRO_NO_DEBUG

#define MIRO_DBG(Category, Loglevel, X) \
  do { \
    if (::Miro::Log::level() >= ::Miro::Log::Loglevel && \
	::Miro::Log::enabled(::Miro::Log::Category)) { \
      ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
      ace___->log(::Miro::Log::ll2LM(::Miro::Log::Loglevel), \
		  ::Miro::Log::format(), X); \
    } \
  } while (0)
#define MIRO_DBG_OSTR(Category, Loglevel, O) \
  do { \
    if (::Miro::Log::level() >= ::Miro::Log::Loglevel && \
	::Miro::Log::enabled(::Miro::Log::Category)) { \
      ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
      std::ostringstream ostr__; \
      ostr__ << O; \
      ace___->log(static_cast<ACE_Log_Priority>(::Miro::Log::ll2LM(::Miro::Log::Loglevel)), \
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
      ace___->log(LM_DEBUG, "Constructing " X ".\n"); \
    } \
  } while (0)
// Constructor log message (Miro::Log::level() >= 1)
#define MIRO_LOG_CTOR_END(X) \
  do { \
    if (::Miro::Log::level() >= ::Miro::Log::LL_CTOR_DTOR) { \
      ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
      ace___->conditional_set (__FILE__, __LINE__, 0, 0); \
      ace___->log(LM_DEBUG, "Construction of " X " fininshed.\n"); \
    } \
  } while (0)
// Destructor log message (Miro::Log::level() >= 1)
#define MIRO_LOG_DTOR(X) \
  do { \
    if (::Miro::Log::level() >= ::Miro::Log::LL_CTOR_DTOR) { \
      ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
      ace___->conditional_set (__FILE__, __LINE__, 0, 0); \
      ace___->log(LM_DEBUG, "Destructing "  X ".\n"); \
    } \
  } while (0)
// Destructor log message (Miro::Log::level() >= 1)
#define MIRO_LOG_DTOR_END(X) \
  do { \
    if (::Miro::Log::level() >= ::Miro::Log::LL_CTOR_DTOR) { \
      ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
      ace___->conditional_set (__FILE__, __LINE__, 0, 0); \
      ace___->log(LM_DEBUG, "Destruction of " X " finished.\n"); \
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
    //! Initializethe logging module.
    static void init(int& argc, char * argv[]);
    //! Query the current logging level.
    static int level() throw();
    //! Set the current logging level.
    static void level(int _level) throw();
    //! Query the current logging maks.
    static unsigned int mask() throw();
    //! Set the current logging level.
    static void mask(unsigned int _mask) throw();
    //! Query if selected priority is currently enabled.
    static bool enabled(int _prioriy);
    //! Query if Miro was compiled with logging support.
    static bool compiledWithLog();
    //! Query if Miro was compiled with debug support.
    /** This requires logging support. */
    static bool compiledWithDebug();
    //! Helper method to convert a Miro LogLevel into an ACE LogMessage
    static ACE_Log_Priority ll2LM(int _level);
    //! Setting the log format.
    static void format(char const * _format);
    //! Accessor returning a pointer to the current log format.
    static char const * format();

    //! Log cathegory of the miro core components.
    static unsigned int const MIRO =    0x00000800;
    //! Log cathegory of the video components.
    static unsigned int const VIDEO =   0x00001000;
    //! Log cathegory of the Sphinx Speech components.
    static unsigned int const SPHINX =  0x00002000;
    //! Log cathegory of the Pioneer components.
    static unsigned int const PIONEER = 0x00004000;
    //! Log cathegory of the faulhaber components.
    static unsigned int const FAUL =    0x00008000;
    //! Log cathegory of the Sparrow components.
    static unsigned int const SPARROW = 0x00010000;
    //! Log cathegory of the Sick laser range finder components.
    static unsigned int const SICK =    0x00020000;
    //! Log cathegory of the DoubleTalk components.
    static unsigned int const DTLK =    0x00040000;
    //! Log cathegory of the DirectedPerception components.
    /** The pantilt unit of the B21. */
    static unsigned int const DP =      0x00080000;
    //! Log cathegory of the B21 components
    static unsigned int const B21 =     0x00100000;

    //! Log level of messages reporting an emergency.
    /** 
     * Your robot is on fire etc. This log level is not maskable,
     * except if you turn of logging at configure time. 
     */
    static signed int const LL_EMERGENCY = 0;
    //! Log level of messages reporting an alert.
    static signed int const LL_ALERT = 1;
    //! Log level of messages reporting a critical condition.
    /**
     * This usually is an unrecoverable error, that leads to
     * the termination of reporting program.
     */
    static signed int const LL_CRITICAL = 2;
    //! Log level of messages reporting an error.
    /**
     * This indicates a real error, but the program will usually
     * try to contiue anyway.
     */
    static signed int const LL_ERROR = 3;
    //! Log level of messages reporting a warning.
    /**
     * A warning should be fixed, but the program is likely work
     * anyways.
     */
    static signed int const LL_WARNING = 4;
    //! Log level of messages reporting a notice.
    /** Make a postit and add it to the other 500 ones. */
    static signed int const LL_NOTICE = 5;
    //! Debug level of messages reporting a constructor/destructor entry.
    /**
     * This debug level is designed to hunt segfaults on startup and
     * exit. - This is when all the big ctors/dtors are run.
     */
    static signed int const LL_CTOR_DTOR = 6;
    //! Log level of messages reporting debug output.
    static signed int const LL_DEBUG = 7;
    //! Log level of messages reporting program trace output.
    static signed int const LL_TRACE = 8;
    //! Log level of messages reporting really verbose comments on the progam execution.
    static signed int const LL_PRATTLE = 9;

    //! The highest log level.
    static signed int const MAX_LOG_LEVEL = LL_NOTICE;
    //! The lowest deug level.
    static signed int const MIN_DBG_LEVEL = LL_CTOR_DTOR;
    //! The highest debug level.
    static signed int const MAX_DBG_LEVEL = LL_PRATTLE;

    //! Helper class for method traces.
    class Trace
    {
    public:
      Trace(unsigned int _level = LL_TRACE,
	    int _priority = LM_DEBUG,
	    char const * _fun = __PRETTY_FUNCTION__);
      ~Trace();
    protected:
      //! Remember the log level for the dtor.
      unsigned int level_;
      //! Remember the cathegory for the dtor.
      int priority_;
      //! Remember the method name for the dtor.
      char const * const fun_;
    };

  protected:

    //! Default is LL_WARNING - 4.
    static unsigned int level_;
    //! Default is MIRO.
    static unsigned int mask_;
    static char const * format_;
    static ACE_Log_Priority aceLM_[MAX_DBG_LEVEL + 1];
  };

  /**
   * It reports to the logging framework in its ctor and dtor.
   * Instance this class via the MIRO_TRACE macro on the beginning
   * of your method and you'll have trace log messages on every
   * execution of the method.
   */
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
  /** The default logging level is @ref LL_WARNING (4). */
  inline
  int
  Log::level() throw() {
    return level_;
  }
  /** This is usually done by the command line option -MiroLogLevel. */
  inline
  void
  Log::level(int _level) throw() {
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
      (mask_ & _priority)>0;
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
  Log::ll2LM(int _level) {
    assert(_level <= MAX_DBG_LEVEL);
    return aceLM_[_level];
  }

}
#endif // miro_Log_h
