// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// for details copyright, usage and credits to other groups see Miro/COPYRIGHT
// for documentation see Miro/doc
// 
// (c) 1999,2000
// Department of Neural Information Processing, University of Ulm, Germany
//
// Authors: 
//   Stefan Enderle, 
//   Stefan Sablatnoeg, 
//   Hans Utz
// 
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef miroLog_hh
#define miroLog_hh

#include <string>

//
// a log facility, providing loglevels and output
//

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
  };
};
#endif




