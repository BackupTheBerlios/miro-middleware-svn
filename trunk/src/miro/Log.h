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

#include <string>
#include <sstream>

//
// a log facility, providing loglevels and log filters
//

#if defined (MIRO_NO_LOGGING)

#define MIRO_LOG(C, X) do {} while (0)
#define MIRO_LOG_OSTR(C, L, O) do {} while (0)
#define MIRO_DBG(C, X) do {} while (0)
#define MIRO_DBG_OSTR(C, L, O) do {} while (0)

#else // !MIRO_NO_LOGGING

#define MIRO_LOG(C, X) \
  do { \
    unsigned int n__ = C; \
    if (::Miro::Log::level() >= n__) { \
      ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
      ace___->conditional_set (__FILE__, __LINE__, 0, 0); \
      ace___->log X; \
    } \
  } while (0)
#define MIRO_LOG_OSTR(C, L, O) \
  do { \
    unsigned int n__ = C; \
    if (::Miro::Log::level() >= n__) { \
      ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
      std::ostringstream ostr__; \
      ostr__ << O; \
      ace___->conditional_set (__FILE__, __LINE__, 0, 0); \
      ace___->log (L, "%c", ostr__.str().c_str()); \
    } \
  } while (0)

#if defined (MIRO_NO_DEBUG)

#define MIRO_DBG(C, X) do {} while (0)
#define MIRO_DBG_OSTR(C, L, O) do {} while (0)

#else // !MIRO_NO_DEBUG

#define MIRO_DBG(C, X) \
  do { \
    unsigned int n__ = C; \
    if (::Miro::Log::level() >= n__) { \
      ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
      ace___->conditional_set (__FILE__, __LINE__, 0, 0); \
      ace___->log X; \
    } \
  } while (0)
#define MIRO_DBG_OSTR(C, L, O) \
  do { \
    unsigned int n__ = C; \
    if (::Miro::Log::level() >= n__) { \
      ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
      std::ostringstream ostr__; \
      ostr__ << O; \
      ace___->conditional_set (__FILE__, __LINE__, 0, 0); \
      ace___->log (L, "%c", ostr__.str().c_str()); \
    } \
  } while (0)


#endif // !MIRO_NO_DEBUG

#endif // !MIRO_NO_LOGGING

namespace Miro 
{
  /**
   * this class can be used to mix in every object that might 
   * be interested in logging messages, at the moment loggin 
   * is done to cerr, but this is further improvable, that is 
   * one reason, why one might want to use this. we define four log levels:
   *
   *  INFO        for all sorts of messages, 
   *  WARNING     for minor problems that may occur in normal operations
   *  ERROR       for major problems that need user attention 
   *  FATAL_ERROR for problems that do not allow further operation
   */
  class Log {
    /**
     * this defines what levels are visible 
     * every log call with logLevel < visibleLoglevel is ignored
     *
     */
    int visibleLoglevel;

    /**
     * to probide more transparancy this is prepended in front of 
     * every message printed
     */
    std::string objectName;

  public:
    static unsigned int level() throw();
    static void level(unsigned int _level) throw();
    static unsigned int mask() throw();
    static void mask(unsigned int _mask) throw();

    Log(int visibleLoglevel_, std::string objectName_);
    virtual ~Log();

    /**
     * log levels
     */
    typedef enum {INFO, WARNING, ERROR, FATAL_ERROR} LogType;

    /**
     * prints log message, also adds object name and time
     * this is virtual for the rare case s.o. wants to overwrite
     * with a variation
     */
    virtual
    void log(int logLevel, std::string logmsg) const;

    /**
     * prints log message and parameter, also adds object name and time
     * this is virtual for the rare case s.o. wants to overwrite
     * with a variation
     */
    virtual
    void log(int logLevel, std::string logmsg, int param) const;

    /**
     * prints log message and parameter, also adds object name and time
     * this is virtual for the rare case s.o. wants to overwrite
     * with a variation
     */
    virtual
    void log(int logLevel, std::string logmsg, double param) const;

    /**
     * prints log message and parameter, also adds object name and time
     * this is virtual for the rare case s.o. wants to overwrite
     * with a variation
     */
    virtual
    void log(int logLevel, std::string logmsg, size_t param) const;

  protected:
    static unsigned int mask_;
    static unsigned int level_;
  };

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
    mask_;
  }
  inline
  void
  Log::mask(unsigned int _mask) throw() {
    mask_ = _mask;
  }
}
#endif // miro_Log_h
