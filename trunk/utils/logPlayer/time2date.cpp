// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "miro/TimeHelper.h"

#include <ace/Time_Value.h>
#include <ace/Date_Time.h>
#include <ace/Version.h>

#include <string>
#include <iostream>

int main (int argc, char * argv[]) 
{
#if ACE_MAJOR_VERSION > 5 || (ACE_MAJOR_VERSION == 5  && ACE_MINOR_VERSION >= 4)
  if (argc < 3) {
    std::cout << "usage: " << argv[0] << " <sec> <usec>\n"; 
    return 1;
  }

  unsigned int sec = atoi(argv[1]);
  unsigned int usec = atoi(argv[2]);

  ACE_Time_Value t(sec, usec);

  ACE_Date_Time d(t);

  std::cout << Miro::timeString(d) << std::endl;

  return 0;
#else
  std::cout << "This is supported only by ACE Version 5.4 or higher!\n";

  return 1;
#endif

}
