// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "miro/Log.h"
#include "miro/Exception.h"

#include <ace/Arg_Shifter.h>
#include <ace/Log_Record.h>
#include <ace/OS.h>

namespace
{
  struct Pair
  {
    int priority;
    char const * name;
  };

  unsigned int const NUM_CATHEGORIES = 15;
  Pair cathegories[NUM_CATHEGORIES] = {
    {Miro::Log::MIRO, "MIRO"},
    {Miro::Log::VIDEO, "VIDEO"},
    {Miro::Log::PSOS, "PSOS"},
    {Miro::Log::SPHINX, "SPHINX"},
    {Miro::Log::PIONEER, "PIONEER"},
    {Miro::Log::CAN, "CAN"},
    {Miro::Log::FAUL, "FAUL"},
    {Miro::Log::SPARROW, "SPARROW"},
    {Miro::Log::MCP, "MCP"},
    {Miro::Log::ABUS, "ABUS"},
    {Miro::Log::MSP, "MSP"},
    {Miro::Log::SICK, "SICK"},
    {Miro::Log::DTLK, "DTLK"},
    {Miro::Log::DP, "DP"},
    {Miro::Log::B21, "B21"}
  };

  char const * const MIRO_LOG_LEVEL = "-MiroLogLevel";
  char const * const MIRO_LOG_FILTER = "-MiroLogFilter";
  char const * const MIRO_LOG_DEVICE   = "-MiroLogDevice";

  std::string logDevice;
}

//
// a log facility, providing loglevels and output
//

namespace Miro 
{
  using std::string;

  unsigned int Log::level_;
  unsigned int Log::mask_;
  char const * Log::format_ = "%s\n";
  ACE_Log_Priority Log::aceLM_[MAX_DBG_LEVEL + 1] = {
    LM_EMERGENCY,
    LM_ALERT,
    LM_CRITICAL,
    LM_ERROR,
    LM_WARNING,
    LM_NOTICE,
    LM_DEBUG,
    LM_DEBUG,
    LM_DEBUG,
    LM_DEBUG
  };

  void
  Log::init(int& argc, char * argv[]) 
  {

    // register all logging cathegories at the log record
    for (unsigned int i = 0; i < NUM_CATHEGORIES; ++i) {
      ACE_Log_Record::priority_name((ACE_Log_Priority)cathegories[i].priority, 
				    cathegories[i].name);
    }

    // parse command line arguments
    ACE_Arg_Shifter arg_shifter (argc, argv);
    while (arg_shifter.is_anything_left ()) {
      const ACE_TCHAR *currentArg = arg_shifter.get_current ();

      if (ACE_OS::strcasecmp(MIRO_LOG_LEVEL, currentArg) == 0) {
	arg_shifter.consume_arg();
	if (arg_shifter.is_parameter_next()) {
	  level_ = strtol(arg_shifter.get_current (), (char**)NULL, 10);
	  if (level_ == 0 && errno == ERANGE)
	    throw Miro::CException(errno, "Error parsing -MiroLogLevel");
	  arg_shifter.consume_arg();
	}
      }
      else if (ACE_OS::strcasecmp(MIRO_LOG_FILTER, currentArg) == 0) {
	arg_shifter.consume_arg();

	mask_ = 0;
	while (arg_shifter.is_parameter_next()) {
	  for (unsigned int i = 0; i < NUM_CATHEGORIES; ++i) {
	    if (ACE_OS::strcasecmp(cathegories[i].name, currentArg) == 0) {
	      mask_ |= cathegories[i].priority;
	      break;
	    }
	  }

	  arg_shifter.consume_arg();
	}
      }
      else if (ACE_OS::strcasecmp(MIRO_LOG_DEVICE, currentArg) == 0) {
	arg_shifter.consume_arg();

	if (arg_shifter.is_parameter_next()) {
	  logDevice = arg_shifter.get_current ();
	}
      }
      else
	arg_shifter.ignore_arg ();
    }
  }
}
