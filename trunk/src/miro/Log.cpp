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


#include <ace/OS.h>
#include "miro/Exception.h"
#include "miro/Log.h"

#include <iostream>

//
// a log facility, providing loglevels and output
//

using std::string;

namespace Miro {
  Log::Log(int visibleLoglevel_, std::string objectName_) :
    visibleLoglevel(visibleLoglevel_),
    objectName(objectName_)
  {
  }

  Log::~Log()
  {}

  void Log::log(int logLevel, std::string logmsg) const 
  { 
    ACE_Time_Value time;
    time_t tt;
    
    if (logLevel >= visibleLoglevel) {

      if ((logLevel<INFO) || (logLevel>FATAL_ERROR))
	throw Exception("Log: invalid loglevel");

      time = ACE_OS::gettimeofday();
      tt = time.sec();
	
      string label[]={"[INFO]","[WARNING]","[ERROR]","[FATAL ERROR]"};
      cerr << "[" << objectName << "] " << label[logLevel] << " " << logmsg << " " << ctime(&tt);
    }
  }

  void Log::log(int logLevel, std::string logmsg, int param) const 
  { 
    ACE_Time_Value time;
    time_t tt;
    
    if (logLevel >= visibleLoglevel) {

      if ((logLevel<INFO) || (logLevel>FATAL_ERROR))
	throw Exception("Log: invalid loglevel");

      time = ACE_OS::gettimeofday();
      tt = time.sec();

      string label[]={"[INFO]","[WARNING]","[ERROR]","[FATAL ERROR]"};
      cerr << "[" << objectName << "] " << label[logLevel] << " " << logmsg << " " << param << " " << ctime(&tt);
    }
  }

  void Log::log(int logLevel, std::string logmsg, double param) const 
  { 
    ACE_Time_Value time;
    time_t tt;
    
    if (logLevel >= visibleLoglevel) {

      if ((logLevel<INFO) || (logLevel>FATAL_ERROR))
	throw Exception("Log: invalid loglevel");

      time = ACE_OS::gettimeofday();
      tt = time.sec();

      string label[]={"[INFO]","[WARNING]","[ERROR]","[FATAL ERROR]"};
      cerr << "[" << objectName << "] " << label[logLevel] << " " << logmsg << " " << param << " " << ctime(&tt);
    }
  }
  
  void Log::log(int logLevel, std::string logmsg, size_t param) const 
  { 
    ACE_Time_Value time;
    time_t tt;
    
    if (logLevel >= visibleLoglevel) {

      if ((logLevel<INFO) || (logLevel>FATAL_ERROR))
	throw Exception("Log: invalid loglevel");

      time = ACE_OS::gettimeofday();
      tt = time.sec();

      string label[]={"[INFO]","[WARNING]","[ERROR]","[FATAL ERROR]"};
      cerr << "[" << objectName << "] " 
	   << label[logLevel] << " " 
	   << logmsg << " " << param << " " << ctime(&tt);
    }
  }
};
