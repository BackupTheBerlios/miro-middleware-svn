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

#include <ace/Date_Time.h>
#include <ace/Get_Opt.h>

#include <iostream>
#include <strstream>

int main (int, char**)
{
  while (true) {
    ACE_Date_Time dt;
    dt.microsec(ACE_OS::gettimeofday().usec());
  
    char buffer[256];
    memset(buffer, 0, 256);
    ostrstream ostr(buffer, 255);
  
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
  
    cout << buffer << endl;

    ACE_OS::sleep(ACE_Time_Value(0, 1));
  }
  return 0;
}

