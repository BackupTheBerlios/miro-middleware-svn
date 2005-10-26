// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2002, 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
//
//////////////////////////////////////////////////////////////////////////////

#include <ace/Date_Time.h>
#include <ace/Get_Opt.h>

#include <iostream>
#include <sstream>
#include <unistd.h>

using std::ostringstream;

// This is an attempt to determine the latency between vidoe capture
// of the camera and video capture of the device driver.  When it is
// started it just prints the current time on the screen.  Recording
// the monitor by the camera and capturing the video with LogVideo in
// streaming mode one can compare the log file time in the name with
// the last time printed on the screen.
//
// It gives a rough estimate, but is by far not as accurate as a
// latency test should be.
int main (int, char**)
{
  while (true) {
    ACE_Date_Time dt;
    dt.microsec(ACE_OS::gettimeofday().usec());
  
    ostringstream ostr;
  
    ostr << dt.year() << ".";
    ostr.width(2);
    ostr.fill('0');
    ostr << dt.month() << ".";
    ostr.width(2);
    ostr.fill('0');
    ostr << dt.day() << "-";
    ostr.width(2);
    ostr.fill('0');
    ostr << dt.hour() << ".";
    ostr.width(2);
    ostr.fill('0');
    ostr << dt.minute() << ".";
    ostr.width(2);
    ostr.fill('0');
    ostr << dt.second() << ".";
    ostr.width(2);
    ostr.fill('0');
    ostr << (dt.microsec() / 10000)
	 << ".ppm";
  
    std::cout << ostr.str() << std::endl;

    ACE_OS::sleep(ACE_Time_Value(0, 1));
  }
  return 0;
}

