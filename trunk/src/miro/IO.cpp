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

#include "IO.h"

// need all types defined in idl, for io operators
#include "idl/ExceptionC.h"
#include "idl/TimeC.h"
#include "idl/WorldPointC.h"
#include "idl/PositionC.h"
#include "idl/VelocityC.h"
#include "idl/MotionStatusC.h"
#include "idl/RangeEventC.h"
#include "idl/PanTiltC.h"

#include "Angle.h"

#include <ace/INET_Addr.h>

#include <iostream>
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
      group(g), index(i), range(r)
    {}
  };
  struct Less : public std::binary_function<Scan&, Scan&, bool> {
    bool operator () (const Scan& _lhs, const Scan& _rhs) {
      return (_lhs.group < _rhs.group || 
	      _lhs.group == _rhs.group && _lhs.index < _rhs.index);
    }
  };
};


std::ostream &
operator<<(std::ostream &ostr, const ACE_TTY_IO::Serial_Params &rhs) {
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
operator<<(std::ostream &ostr, const ACE_INET_Addr &rhs) {
  ostr << rhs.get_host_addr() << std::endl;
  return ostr; 
}
namespace Miro
{
  using ::operator<<;

//   // output operator for LaserScanIDL
//   std::ostream &
//   operator<<(std::ostream &ostr, const LaserScanIDL &rhs) {
//     ostr << rhs.time << " ";
//     ostr << rhs.position << " ";
//     ostr << LASER_NUM << " ";
//     for (long i=0; i<LASER_NUM; ++i)
//       ostr << rhs.value[i] << " ";

//     return ostr;
//   }
  // output operator for TimeIDL
  std::ostream &
  operator<<(std::ostream &ostr, const TimeIDL &rhs) {
    ostr << rhs.sec << ".";
    ostr.width(6);
    ostr.fill('0');
    ostr << rhs.usec;
    return ostr;
  }
  // output operator for PositionIDL
  std::ostream &
  operator<<(std::ostream &ostr, const PositionIDL &rhs) {
    ostr << "(" << rhs.point << ",";
    ostr << rad2Deg(rhs.heading) << "°";
    return ostr;
  }
  // output operator for WorldPointIDL
  std::ostream &
  operator<<(std::ostream &ostr, const WorldPointIDL &rhs) {
    ostr << rhs.x << " " << rhs.y;
    return ostr;
  }
  // output operator for VelocityIDL
  std::ostream &
  operator<<(std::ostream &ostr, const VelocityIDL &rhs) {
    ostr << rhs.translation << "mm/s " << rad2Deg(rhs.rotation) << "°/s";
    return ostr;
  }
  // output operator for MotionStatusIDL
  std::ostream &
  operator<<(std::ostream &ostr, const MotionStatusIDL &rhs) {
    ostr << rhs.time << " ";
    ostr << rhs.position << " ";
    ostr << rhs.velocity;
    return ostr;
  }
  // output operator for PanTiltPositionIDL
  std::ostream &
  operator<<(std::ostream &ostr, const PanTiltPositionIDL &rhs) {
    ostr << rad2Deg(rhs.panvalue) << "° ";
    ostr << rad2Deg(rhs.tiltvalue) << "°";
    return ostr;
  }
  // output for exceptions which take arguments
  std::ostream &
  operator<<(std::ostream &ostr, const EDevIO &rhs) {
    ostr << static_cast<const CORBA::Exception&>(rhs)
	 <<"Exception :"<< rhs.what;
    return ostr;
  }
  // output for exceptions which take arguments
  std::ostream &
  operator<<(std::ostream &ostr, const EOutOfBounds &rhs) {
    ostr << static_cast<const CORBA::Exception&>(rhs)
	 <<"Exception :"<< rhs.what;
    return ostr;
  }
  // output for exceptions which take arguments
  std::ostream &
  operator<<(std::ostream &ostr, const ETimeOut &rhs) {
    ostr << static_cast<const CORBA::Exception&>(rhs)
	 <<"Exception :"<< rhs.what;
    return ostr;
  }
  // output operator for @ref RangeGroupIDL
  std::ostream &
  operator<<(std::ostream &ostr, const RangeGroupIDL &rhs) {
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
};


//   // output operator for TactileScanIDL
//   std::ostream &
//   operator<<(std::ostream &ostr, const TactileScanIDL &rhs) {
//     ostr << TACTILE_ROWS << " ";
//     ostr << TACTILE_COLS << " ";
//     for (long i=0; i< TACTILE_ROWS; ++i)
//       for (long j=0; j< TACTILE_COLS; ++j){
// 	ostr << rhs.value[i][j] << " ";
//       }
//     return ostr;
//   }
//   // output operator for SonarScanIDL
//   std::ostream &
//   operator<<(std::ostream &ostr, const SonarScanIDL &rhs) {
//     ostr << rhs.length() << " ";
//     for (unsigned long i = 0; i < rhs.length(); ++i) {
//       ostr << rhs[i] << " ";
//     }
//     return ostr;
//   }
//  // output operator for IrScanIDL
//   std::ostream &
//   operator<<(std::ostream &ostr, const InfraredScanIDL &rhs) {
//     ostr << IR_ROWS << " ";
//     ostr << IR_NUM << " ";
//     for (long i=0; i< IR_ROWS; ++i)
//       for (long j=0; j< IR_NUM; ++j){
// 	ostr << rhs.value[i][j] << " ";
//       }
//     return ostr;
//   }
