// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 1999-2005
// Department of Neuroinformatics, University of Ulm, Germany
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// $Id$
//
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "IO.h"

// need all types defined in idl, for io operators
#include "idl/ExceptionC.h"
#include "idl/TimeC.h"
#include "idl/WorldPointC.h"
#include "idl/WorldPoint3DC.h"
#include "idl/PositionC.h"
#include "idl/VelocityC.h"
#include "idl/MotionStatusC.h"
#include "idl/RangeEventC.h"
#include "idl/PanTiltC.h"
#include "idl/GpsDataC.h"
#include "idl/FieldStrengthC.h"
#include "idl/InclinationC.h"
#include "idl/CompassEventC.h"
#include "idl/InclinometerEventC.h"
#include "idl/MagnetometerEventC.h"
#include "idl/ThermometerEventC.h"

#ifdef MIRO_HAS_PIONEER
#include "idl/TCM2EventC.h"
#endif

#include "Angle.h"
#include "TimeHelper.h"

#include <ace/INET_Addr.h>
#include <ace/Sched_Params.h>

#include <iostream>
#include <sstream>
#include <vector>
#include <functional>
#include <algorithm>

namespace
{
  // Since the scan data comes in a bunch,
  // first sort the data by group and index.
  struct Scan {
    long group;
    long index;
    long range;
    Scan(long g, long i, long r) :
        group(g), index(i), range(r) {}
  };
  struct Less : public std::binary_function<Scan&, Scan&, bool> {
    bool operator()(const Scan& _lhs, const Scan& _rhs) {
      return (_lhs.group < _rhs.group ||
              _lhs.group == _rhs.group && _lhs.index < _rhs.index);
    }
  };
}


std::ostream &
operator<<(std::ostream &ostr, const ACE_TTY_IO::Serial_Params &rhs)
{
  ostr << "baudrate = " << rhs.baudrate << std::endl
  << "parityenb = " << rhs.parityenb << std::endl
  << "databits = " << rhs.databits << std::endl
  << "stopbits = " << rhs.stopbits << std::endl
  << "readtimeoutmsec = " << rhs.readtimeoutmsec << std::endl
  << "modem = " << rhs.modem << std::endl
  << "rcvenb = " << rhs.rcvenb << std::endl
  << "ctsenb = " << rhs.ctsenb << std::endl    // CTS & RTS are the same under unix
  << "rtsenb = " << rhs.rtsenb << std::endl    // enable & set rts mode (win32)
  << "xinenb = " << rhs.xinenb << std::endl    // enable xon/xoff  reception
  << "xoutenb = " << rhs.xoutenb << std::endl; // enable xon/xoff transmission
  return ostr;
}

std::ostream &
operator<<(std::ostream &ostr, ACE_Sched_Params const &rhs)
{
  std::ostringstream p;
  switch (rhs.policy()) {
    case ACE_SCHED_OTHER:
      p <<  "ACE_SCHED_OTHER";
      break;
    case ACE_SCHED_FIFO:
      p << "ACE_SCHED_FIFO";
      break;
    case ACE_SCHED_RR:
      p << "ACE_SCHED_RR";
      break;
    default:
      p << rhs.policy();
  }

  ostr << "policy = " << p.str() << std::endl
  << "priority = " << rhs.priority() << std::endl
  << "quantum = " << rhs.quantum() << " sec" << std::endl;

  return ostr;
}

std::ostream &
operator<<(std::ostream &ostr, ACE_INET_Addr const &rhs)
{
  ostr << rhs.get_host_addr() << std::endl;
  return ostr;
}

namespace Miro
{
  using ::operator<<;

  // output operator for TimeIDL
  std::ostream &
  operator<<(std::ostream &ostr, const TimeIDL &rhs)
  {
    ostr << rhs.sec << ".";
    ostr.width(6);
    ostr.fill('0');
    ostr << rhs.usec;
    return ostr;
  }
  // output operator for PositionIDL
  std::ostream &
  operator<<(std::ostream &ostr, const PositionIDL &rhs)
  {
    ostr << "(" << rhs.point << ",";
    ostr << rad2Deg(rhs.heading) << "°)";
    return ostr;
  }
  // output operator for WorldPointIDL
  std::ostream &
  operator<<(std::ostream &ostr, const WorldPointIDL &rhs)
  {
    ostr << rhs.x << " " << rhs.y;
    return ostr;
  }
  // output operator for WorldPoint3DIDL
  std::ostream &
  operator<<(std::ostream &ostr, const WorldPoint3DIDL &rhs)
  {
    ostr << rhs.x << " " << rhs.y << " " << rhs.z;
    return ostr;
  }
  // output operator for VelocityIDL
  std::ostream &
  operator<<(std::ostream &ostr, const VelocityIDL &rhs)
  {
    ostr << rhs.translation << "mm/s " << rad2Deg(rhs.rotation) << "°/s";
    return ostr;
  }
  // output operator for MotionStatusIDL
  std::ostream &
  operator<<(std::ostream &ostr, const MotionStatusIDL &rhs)
  {
    ostr << rhs.time << " ";
    ostr << rhs.position << " ";
    ostr << rhs.velocity;
    return ostr;
  }
  // output operator for PanTiltPositionIDL
  std::ostream &
  operator<<(std::ostream &ostr, const PanTiltPositionIDL &rhs)
  {
    ostr << rad2Deg(rhs.panValue) << "° ";
    ostr << rad2Deg(rhs.tiltValue) << "°";
    return ostr;
  }
  // output for exceptions which take arguments
  std::ostream &
  operator<<(std::ostream &ostr, const EDevIO &rhs)
  {
    ostr << static_cast<const CORBA::Exception&>(rhs)
    << "Exception :" << rhs.what;
    return ostr;
  }
  // output for exceptions which take arguments
  std::ostream &
  operator<<(std::ostream &ostr, const EOutOfBounds &rhs)
  {
    ostr << static_cast<const CORBA::Exception&>(rhs)
    << "Exception :" << rhs.what;
    return ostr;
  }
  // output for exceptions which take arguments
  std::ostream &
  operator<<(std::ostream &ostr, const ETimeOut &rhs)
  {
    ostr << static_cast<const CORBA::Exception&>(rhs)
    << "Exception :" << rhs.what;
    return ostr;
  }
  // output operator for @ref RangeGroupIDL
  std::ostream &
  operator<<(std::ostream &ostr, const RangeGroupIDL &rhs)
  {
    unsigned int l = rhs.length();
    for (unsigned int i = 0; i != l; ++i) {
      ostr.width(6);
      ostr.fill(' ');
      ostr << rhs[i];
    }
    return ostr;
  }
  // output operator for @ref RangeScanIDL
  std::ostream &
  operator<<(std::ostream &ostr, const RangeScanIDL &rhs)
  {
    unsigned int l = rhs.length();
    for (unsigned int i = 0; i != l; ++i) {
      ostr << rhs[i] << std::endl;
    }
    return ostr;
  }

  std::ostream &
  operator<<(std::ostream &ostr, const RangeBunchEventIDL &rhs)
  {

    std::vector<Scan> scan;
    for (unsigned int i = 0; i < rhs.sensor.length(); ++i)
      scan.push_back(Scan(rhs.sensor[i].group,
                          rhs.sensor[i].index,
                          rhs.sensor[i].range));
    std::sort(scan.begin(), scan.end(), Less());

    ostr << rhs.time << std::endl;
    int group = -1;
    int index = 0;
    std::vector<Scan>::const_iterator i, j = scan.end();
    for (i = scan.begin(); i != j; ++i) {
      if (i->group > group) {
        if (group != -1)
          ostr << std::endl;
        ostr << i->group << ": ";
        group = i->group;
        index = 0;
      }
      for (; index < i->index; ++index)
        ostr << "      ";

      ostr.width(6);
      ostr.fill(' ');
      ostr << i->range;
      index = i->index;
    }
    return ostr;
  }

  std::ostream &
  operator<<(std::ostream &ostr, const RangeGroupEventIDL &rhs)
  {
    ostr << rhs.time << " - "
    << rhs.group << ": "
    << rhs. range;
    return ostr;
  }

  std::ostream &
  operator<<(std::ostream &ostr, const RangeScanEventIDL &rhs)
  {
    ostr << rhs.time << std::endl
    << rhs. range;
    return ostr;
  }

  std::ostream &
  operator<<(std::ostream &ostr, const GpsPositionIDL &rhs)
  {
    ostr << "lat = " << rad2Deg(rhs.latitude) << "°, ";
    ostr << "lon = " << rad2Deg(rhs.longitude) << "°, ";
    ostr << "alt = " << rhs.altitude << "m";
    return ostr;
  }

  std::ostream &
  operator<<(std::ostream &ostr, const GpsDilutionIDL &rhs)
  {
    ostr << "hdop = " << rhs.hdop << ", ";
    ostr << "vdop = " << rhs.vdop << ", ";
    ostr << "pdop = " << rhs.pdop;
    return ostr;
  }

  std::ostream &
  operator<<(std::ostream &ostr, const GpsGlobalPositionEventIDL &rhs)
  {
    ostr << rhs.time << ": " << rhs.global;
    return ostr;
  }

  std::ostream &
  operator<<(std::ostream &ostr, const GpsRelativePositionEventIDL &rhs)
  {
    ostr << rhs.time << ": " << rhs.relative;
    return ostr;
  }

  std::ostream &
  operator<<(std::ostream &ostr, const GpsPositionEventIDL &rhs)
  {
    ostr << rhs.time << ": " << rhs.global << " / " << rhs.relative;
    return ostr;
  }

  std::ostream &
  operator<<(std::ostream &ostr, const GpsDilutionEventIDL &rhs)
  {
    ostr << rhs.time << ": " << rhs.dilution;
    return ostr;
  }

  std::ostream &
  operator<<(std::ostream &ostr, const GpsSentenceEventIDL &rhs)
  {
    ostr << rhs.time << ": " << rhs.sentence;
    return ostr;
  }

  std::ostream &
  operator<<(std::ostream &ostr, const FieldStrengthIDL &rhs)
  {
    ostr << "(" << rhs.x << "," << rhs.y << "," << rhs.z << ")";
    return ostr;
  }

  std::ostream &
  operator<<(std::ostream &ostr, const InclinationIDL &rhs)
  {
    ostr << "(" << rad2Deg(rhs.pitch) << "°," << rad2Deg(rhs.roll) << "°)";
    return ostr;
  }

  std::ostream &
  operator<<(std::ostream &ostr, const CompassEventIDL &rhs)
  {
    ostr << rhs.time << " " << rad2Deg(rhs.heading) << "°";
    return ostr;
  }

  std::ostream &
  operator<<(std::ostream &ostr, const InclinometerEventIDL &rhs)
  {
    ostr << rhs.time << " " << rhs.inclination;
    return ostr;
  }

  std::ostream &
  operator<<(std::ostream &ostr, const MagnetometerEventIDL &rhs)
  {
    ostr << rhs.time << " " << rhs.fieldstrength;
    return ostr;
  }

  std::ostream &
  operator<<(std::ostream &ostr, const ThermometerEventIDL &rhs)
  {
    ostr << rhs.time << " " << rhs.temperature << "°C";
    return ostr;
  }

#ifdef MIRO_HAS_PIONEER
  std::ostream &
  operator<<(std::ostream &ostr, const TCM2EventIDL &rhs)
  {
    ostr << rhs.time << " ";
    ostr << rad2Deg(rhs.heading) << "° ";
    ostr << rhs.inclination << " ";
    ostr << rhs.fieldstrength << "µT ";
    ostr << rhs.temperature << "°C";
    return ostr;
  }
#endif

}
