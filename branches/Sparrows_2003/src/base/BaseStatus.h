// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef BaseStatus_h
#define BaseStatus_h

#include "BaseDevice.h"

#include "miro/MotionStatusC.h"

namespace Miro 
{
  class OdometryImpl;
};

namespace Mcp {
  class Message;
};

namespace Base
{
  // forward decleration
  class Parameters;

  /**
   * raw data of a base status report
   */
  struct ReportData
  {
    ReportData();

    unsigned long Request;             // the data contained in this report
    unsigned long Clock;               // base time (1/256 s)
    unsigned long GeneralStatus;       // gerneral status bits 
    unsigned long Xpos;                // x position in mm (16 bit * 10)
    unsigned long Ypos;                // y position in mm (16 bit * 10)
    unsigned short Heading;            // (pi / 512) -> 0 = -pi, 1024 = pi
    unsigned long BaseRelativeHeading; // (pi / 512) -> 0 = -pi, 1024 = pi
    unsigned long TranslateError;      // 1/mm
    unsigned long TranslateVelocity;   // 1 mm/s
    unsigned long TranslateStatus;
    unsigned long RotateError;
    unsigned long RotateVelocity;
    unsigned long RotateStatus;
  };

  /**
   * output operator
   *
   * for debugging purposes only
   */
  ostream& operator << (ostream& out, const ReportData& report);

  /**
   * holds the current status of the base
   */
  class Status
  {
  public:
    //-------------------------------------------------------------------------
    // public methods
    //-------------------------------------------------------------------------

    Status(Miro::OdometryImpl * _odometry);

    void operator <<= (const Mcp::Message& report);

    bool rotateHalt() const;
    bool translateHalt() const;
    bool passiveRotation() const;
    bool passiveTranslation() const;

  protected:
    //-------------------------------------------------------------------------
    // protected methods
    //-------------------------------------------------------------------------

    void parseReport(const Mcp::Message& report);

    //-------------------------------------------------------------------------
    // protected static methods
    //-------------------------------------------------------------------------

    static const unsigned long PHASE_MASK = 0xE000;

    // methods for parsing the phase of the base
    static unsigned long phase(unsigned long status);
    static bool directionPositive(unsigned long status);
    static bool phaseMotionless(unsigned long status);
    static bool phaseAccel(unsigned long status);

    // copies 2 bytes in net order into an unsigned long in host order
    static unsigned long getUShort(const void * src);
    // copies 4 bytes in net order into an unsigned long in host order
    static unsigned long getULong(const void * src);

    //-------------------------------------------------------------------------
    // protected object data
    //-------------------------------------------------------------------------

    Miro::OdometryImpl * odometry_;
    Miro::MotionStatusIDL  status;

    ReportData currentReport;
    ReportData previousReport;
  };

  //---------------------------------------------------------------------------
  // inlines
  //---------------------------------------------------------------------------


  inline
  bool
  Status::rotateHalt() const {
    return (phaseMotionless(currentReport.RotateStatus) && 
	    !phaseMotionless(previousReport.RotateStatus));
  }
  inline
  bool 
  Status::translateHalt() const {
    return (phaseMotionless(currentReport.TranslateStatus) && 
	    !phaseMotionless(previousReport.TranslateStatus));
  }
  inline
  bool 
  Status::passiveRotation() const {
    return ((!phaseMotionless(previousReport.RotateStatus)) && 
	    (phaseAccel(currentReport.RotateStatus)) &&
	    (currentReport.RotateVelocity == 0) &&
	    (previousReport.RotateVelocity == 0));
  }
  inline
  bool 
  Status::passiveTranslation() const {
    return ((!phaseMotionless(previousReport.TranslateStatus)) && 
	    (phaseAccel(currentReport.TranslateStatus)) &&
	    (currentReport.TranslateVelocity == 0) &&
	    (previousReport.TranslateVelocity == 0));
  }

  //---------------------------------------------------------------------------
  // protected static methods
  //---------------------------------------------------------------------------
  inline
  unsigned long
  Status::getUShort(const void * src) {
    return ACE_NTOHS(*((const unsigned short *) src));
  }
  // copies 4 bytes in net order into an unsigned long in host order
  inline
  unsigned long
  Status::getULong(const void * src) {
    return ACE_NTOHL(*((const unsigned long *) src));
  }

  // methods for parsing the phase of the base
  inline
  unsigned long 
  Status::phase(unsigned long status)  {
    return ((status & PHASE_MASK) >> 13); 
  }
  inline
  bool 
  Status::directionPositive(unsigned long status) {
    return (!((status >> 11) & 1));
  }
  inline
  bool 
  Status::phaseMotionless(unsigned long status) {
    unsigned long x = phase(status);
    return ((x==PHASE_STOP)||(x==PHASE_HALT)||(x==PHASE_LIMP));
  }
  inline
  bool 
  Status::phaseAccel(unsigned long status) {
    unsigned long x = phase(status);
    return (x==PHASE_ACCEL);
  }
};
#endif




