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


#include "BaseStatus.h"

#include "mcp/McpMessage.h"
#include "miro/OdometryImpl.h" // BaseReportIDL

#include <iostream>
#include <cmath>

#undef DEBUG

// #define VERBOSE

namespace Base
{
  using std::cout;
  using std::cerr;
  using std::endl;
  using std::hex;
  using std::dec;

  std::ostream& 
  operator << (std::ostream& out, const ReportData& report) 
  {
    out << hex
	<< "request = 0x" << report.Request << endl
	<< "Clock = 0x" << report.Clock << endl
	<< "status = 0x" << report.GeneralStatus << endl
	<< "X = 0x" << report.Xpos << endl
	<< "Y = 0x" << report.Ypos << endl
	<< dec
	<< "heading = " << report.Heading << endl
	<< "relative heading = " << report.BaseRelativeHeading << endl
	<< "tranlate error = " << report.TranslateError << endl
	<< "translate velocity = " << report.TranslateVelocity << endl
	<< hex
	<< "translate status = 0x" << report.TranslateStatus << endl
	<< dec
	<< "tranlate error = " << report.RotateError << endl
	<< "translate velocity = " << report.RotateVelocity << endl
	<< hex
	<< "translate status = 0x" << report.RotateStatus << dec << endl;

    return out;
  }

  ReportData::ReportData() :
    Request(0),
    Clock(0),
    GeneralStatus(0),
    Xpos(0),
    Ypos(0),  
    Heading(0),
    BaseRelativeHeading(2048),
    TranslateError(0),
    TranslateVelocity(0),
    TranslateStatus(PHASE_STOP),
    RotateError(0),
    RotateVelocity(0),
    RotateStatus(PHASE_STOP)
  {
  }

  Status::Status(Miro::OdometryImpl * _odometry) :
    odometry_(_odometry),
    currentReport(),
    previousReport()
  {
    status.time.sec = 0;
    status.time.usec = 0;
    status.position.point.x = 0.;
    status.position.point.y = 0.;
    status.position.heading = 0.;
    status.velocity.translation = 0;
    status.velocity.rotation = 0.;
  }

  void
  Status::operator <<= (const Mcp::Message& report)
  {
    // set all the data

    parseReport(report);

    status.time.sec = report.time().sec();
    status.time.usec = report.time().usec();
    status.position.point.x = currentReport.Xpos;
    status.position.point.y = currentReport.Ypos;
    status.position.heading = -base2rad(currentReport.Heading);

    // shift velocities into the right metrics
    if (currentReport.RotateStatus & 0x800)
      status.velocity.rotation = base2rad(currentReport.RotateVelocity);
    else
      status.velocity.rotation = -base2rad(currentReport.RotateVelocity);

    if (currentReport.TranslateStatus & 0x800)
      status.velocity.translation = -currentReport.TranslateVelocity;
    else
      status.velocity.translation = currentReport.TranslateVelocity;

    if (odometry_) {
      odometry_->integrateData(status);
    }
  }

  void
  Status::parseReport(const Mcp::Message& report)
  {
    const unsigned char* string_position = report.message();

#ifdef DEBUG
    cerr << "parsing base status report: ";
#endif

    // save previous status report for interpolation, etc.
    previousReport = currentReport;

    // first long is bit flag for rest of items
    currentReport.Request = getULong(string_position);
    string_position += sizeof(unsigned long);

    // we use a fixed status report format, including everything the
    // base is able to get us. Therefor, we do not need to parse
    // the status report conditionally. But it's good to check
    // for correctness of the report.

    if ((currentReport.Request & REPORT_EVERYTHING) != REPORT_EVERYTHING) {
      cerr << "illegal status report!" << endl;
      exit(1);
      //    throw Miro::DevIO();
    }

    // these must remain in numerical order !!
    // as the base will pass them back that way

#ifdef DEBUG
    cout << "clock, ";
#endif
    currentReport.Clock = getULong(string_position);
    string_position += sizeof(unsigned long);

#ifdef DEBUG
    cout << "general, ";
#endif
    currentReport.GeneralStatus = getUShort(string_position);
    string_position += sizeof(unsigned short);

#ifdef DEBUG
    cout << "x, ";
#endif
    currentReport.Xpos = getUShort(string_position);
    string_position += sizeof(unsigned short);

    currentReport.Xpos = (unsigned long)((double)(currentReport.Xpos) 
					 / POS_PER_CM * 10.0);

#ifdef DEBUG
    cout << "y, ";
#endif
    currentReport.Ypos = getUShort(string_position);
    string_position += sizeof(unsigned short);

    currentReport.Ypos = (unsigned long)((double)(currentReport.Ypos) 
					 / POS_PER_CM * 10.0);

#ifdef DEBUG
    cout << "heading, ";
#endif
    currentReport.Heading = getUShort(string_position);
    string_position += sizeof(unsigned short);

#ifdef DEBUG
    cout << "rel heading, ";
#endif
    currentReport.BaseRelativeHeading = getUShort(string_position);
    string_position += sizeof(unsigned short);

#ifdef DEBUG
    cout << "trans error, ";
#endif
    currentReport.TranslateError = getUShort(string_position);
    string_position += sizeof(unsigned short);
    currentReport.TranslateError = (unsigned long) 
      ((double)currentReport.TranslateError / (ENC_PER_CM / 10.0));

#ifdef DEBUG
    cout << "trans vel, ";
#endif
    currentReport.TranslateVelocity = getUShort(string_position);
    string_position += sizeof(unsigned short);
    currentReport.TranslateVelocity = (unsigned long) 
      ((double)currentReport.TranslateVelocity / (ENC_PER_CM / 10.0));

#ifdef DEBUG
    cout << "trans status, ";
#endif
    currentReport.TranslateStatus = getUShort(string_position);
    string_position += sizeof(unsigned short);

#ifdef DEBUG
    cout << "rot error, ";
#endif
    currentReport.RotateError = getUShort(string_position);
    string_position += sizeof(unsigned short);

#ifdef DEBUG
    cout << "rot vel, ";
#endif
    currentReport.RotateVelocity = getUShort(string_position);
    string_position += sizeof(unsigned short);

#ifdef DEBUG
    cout << "rot status, ";
#endif
    currentReport.RotateStatus = getUShort(string_position);
    string_position += sizeof(unsigned short);

#ifdef DEBUG
    cout << "done" << endl;
#endif

#ifdef VERBOSE
    cout << "report data: " << endl << currentReport << endl;

    cout << "cosHeading = " << cosHeading << endl
	 << "sinHeading = " << sinHeading << endl
	 << "origin.point.x = " << origin.point.x << endl
	 << "origin.point.y = " << origin.point.y << endl;
#endif
  }
};
