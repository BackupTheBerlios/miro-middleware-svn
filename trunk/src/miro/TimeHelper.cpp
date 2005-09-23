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

#include "TimeHelper.h"

#include <ace/Date_Time.h>

#include <iostream>
#include <sstream>

std::ostream& 
operator << (std::ostream& ostr, const ACE_Time_Value& rhs)
{
  ostr << rhs.sec() << ".";
  ostr.width(6);
  ostr.fill('0');
  ostr << rhs.usec();
  return ostr;
}

std::istream& 
operator >> (std::istream& istr, ACE_Time_Value& rhs) 
{
  char c;
  long t;

  istr >> t >> c;
  rhs.sec(t);

  if (c != '.')
    return istr;

  istr >> t;
  rhs.usec(t);
  return istr;
}

namespace Miro
{
  std::string
  timeString()
  {
    ACE_Date_Time dt;
    dt.microsec(ACE_OS::gettimeofday().usec());

    std::ostringstream ostr;

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
    ostr << (dt.microsec() / 10000);

    return ostr.str();
  }

  std::string
  timeStringSec()
  {
    ACE_Date_Time dt;

    std::ostringstream ostr;

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
    ostr << dt.second();

    return ostr.str();
  }

  std::string
  timeStringMinute()
  {
    ACE_Date_Time dt;

    std::ostringstream ostr;

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
    ostr << dt.minute();

    return ostr.str();
  }

#if ACE_MAJOR_VERSION > 5 || (ACE_MAJOR_VERSION == 5  && ACE_MINOR_VERSION >= 4)

  std::string
  timeString(ACE_Time_Value const& _stamp)
  {
    ACE_Date_Time dt(_stamp);

    std::ostringstream ostr;

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
    ostr << (dt.microsec() / 10000);

    return ostr.str();
  }

  std::string
  timeStringSec(ACE_Time_Value const& _stamp)
  {
    ACE_Date_Time dt(_stamp);

    std::ostringstream ostr;

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
    ostr << dt.second();

    return ostr.str();
  }

  std::string
  timeStringMinute(ACE_Time_Value const& _stamp)
  {
    ACE_Date_Time dt(_stamp);

    std::ostringstream ostr;

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
    ostr << dt.minute();

    return ostr.str();
  }
#endif
}


