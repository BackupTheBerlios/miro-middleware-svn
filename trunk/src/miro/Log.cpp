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

  struct Pair2
  {
    char const * name;
    int priority;
  };

  char const * const MIRO_LOG_LEVEL = "-MiroLogLevel";
  char const * const MIRO_LOG_FILTER = "-MiroLogFilter";
  char const * const MIRO_LOG_LEVEL_SHORT = "-MLL";
  char const * const MIRO_LOG_FILTER_SHORT = "-MLF";
  char const * const MIRO_LOG_DEVICE   = "-MiroLogDevice";

  std::string logDevice;
}

//
// a log facility, providing loglevels and output
//

namespace Miro 
{
  unsigned int Log::level_ = LL_WARNING;
  unsigned int Log::mask_ = MIRO;
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


  /**
   * Adds the log cathegory clear names corresponding to the cathegory
   * integer constants to the ACE_Log_Record.
   *
   * Parses the argument vector for the Miro command line option:
   * -MiroLogLevel, -MiroLogFilter and -MiroLogDevice.
   *
   * @TODO -MiroLogDevice has no effect. - Needs to be implemented.
   */
  void
  Log::init(int& argc, char * argv[]) 
  {
#if (GCC_MAJOR_VERSION >= 3)
    std::set_terminate (__gnu_cxx::__verbose_terminate_handler);
#endif

    mask_ = 0;
   // register all logging cathegories at the log record
    for (unsigned int i = 0; i < NUM_CATHEGORIES; ++i) {
      ACE_Log_Record::priority_name((ACE_Log_Priority)cathegories[i].priority, 
				    cathegories[i].name);
    }

    // parse command line arguments
    ACE_Arg_Shifter arg_shifter (argc, argv);
    while (arg_shifter.is_anything_left ()) {
      const ACE_TCHAR *currentArg = arg_shifter.get_current ();

      if (ACE_OS::strcasecmp(MIRO_LOG_LEVEL, currentArg) == 0 ||
	  ACE_OS::strcasecmp(MIRO_LOG_LEVEL_SHORT, currentArg) == 0 ) {
	arg_shifter.consume_arg();
	if (arg_shifter.is_parameter_next()) {
	  level_ = strtol(arg_shifter.get_current (), (char**)NULL, 10);
	  if (level_ == 0 && errno == ERANGE)
	    throw Miro::CException(errno, "Error parsing -MiroLogLevel");
	  arg_shifter.consume_arg();
	}
      }
      else if (ACE_OS::strcasecmp(MIRO_LOG_FILTER, currentArg) == 0 ||
	       ACE_OS::strcasecmp(MIRO_LOG_FILTER_SHORT, currentArg) == 0 ) {
	arg_shifter.consume_arg();

	while (arg_shifter.is_parameter_next()) {
	  unsigned int i;
	  for (i = 0; i < NUM_CATHEGORIES; ++i) {
	    if (ACE_OS::strcasecmp(cathegories[i].name, arg_shifter.get_current ()) == 0) {
	      unsigned long p;
	      ACE_Log_Msg *l = ACE_Log_Msg::instance ();
	      p = l->priority_mask(ACE_Log_Msg::PROCESS);
	      p |= cathegories[i].priority;
	      l->priority_mask(p, ACE_Log_Msg::PROCESS);

	      break;
	    }
	  }
	  if (i == NUM_CATHEGORIES)
	    throw Miro::Exception(std::string("Unknown parameter for -MiroLogFilter: ") + 
				  arg_shifter.get_current ());

	  arg_shifter.consume_arg();
	}
      }
      else if (ACE_OS::strcasecmp(MIRO_LOG_DEVICE, currentArg) == 0) {
	arg_shifter.consume_arg();

	if (arg_shifter.is_parameter_next()) {
	  logDevice = arg_shifter.get_current ();
	}
      }
      else {
	arg_shifter.ignore_arg ();
      }
    }

    mask_ = ACE_Log_Msg::instance()->priority_mask(ACE_Log_Msg::PROCESS);
  
//     for (unsigned int i = 0; i < 32; ++i) {
//       unsigned long p = 1UL << i;
//       if (mask_ & p) {
// 	std::cout << "logged cathegory: " 
// 		  << ACE_Log_Record::priority_name((ACE_Log_Priority) p) 
// 		  << std::endl;
//       }
//    }
  }
}
