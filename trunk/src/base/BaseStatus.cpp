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
#include "miro/Log.h"

#include <cmath>

namespace Base
{
  std::ostream& 
  operator << (std::ostream& out, const ReportData& report) 
  {
    out << std::hex
	<< "request = 0x" << report.Request << std::endl
	<< "Clock = 0x" << report.Clock << std::endl
	<< "status = 0x" << report.GeneralStatus << std::endl
	<< "X = 0x" << report.Xpos << std::endl
	<< "Y = 0x" << report.Ypos << std::endl
	<< std::dec
	<< "heading = " << report.Heading << std::endl
	<< "relative heading = " << report.BaseRelativeHeading << std::endl
	<< "tranlate error = " << report.TranslateError << std::endl
	<< "translate velocity = " << report.TranslateVelocity << std::endl
	<< std::hex
	<< "translate status = 0x" << report.TranslateStatus << std::endl
	<< std::dec
	<< "tranlate error = " << report.RotateError << std::endl
	<< "translate velocity = " << report.RotateVelocity << std::endl
	<< std::hex
	<< "translate status = 0x" << report.RotateStatus << std::dec << std::endl;

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
    
    /**********
     * Quick dirty patch to "correct" coordinates
     * TODO: Check WHY!!!!!!
     **********/
    //    status.position.point.x = currentReport.Xpos;
    //    status.position.point.y = currentReport.Ypos;
    status.position.point.x = currentReport.Ypos;
    status.position.point.y = -currentReport.Xpos;

    /**********
     * EndPatch
     **********/

    status.position.heading = -base2rad(currentReport.Heading);

    while (status.position.heading <= -M_PI) {
      status.position.heading+=2*M_PI;
    }
    while (status.position.heading > M_PI) {
      status.position.heading-=2*M_PI;
    }
    

    // shift velocities into the right metrics
    if (currentReport.RotateStatus & 0x800)
      status.velocity.rotation = base2rad(currentReport.RotateVelocity);
    else
      status.velocity.rotation = -base2rad(currentReport.RotateVelocity);

    if (currentReport.TranslateStatus & 0x800)
      status.velocity.translation = -currentReport.TranslateVelocity;
    else
      status.velocity.translation = currentReport.TranslateVelocity;

    while (status.velocity.rotation <= -M_PI) {
      status.velocity.rotation+=2*M_PI;
    }
    while (status.velocity.rotation > M_PI) {
      status.velocity.rotation-=2*M_PI;
    }

    if (odometry_) {
      odometry_->integrateData(status);
    }
  }

  void
  Status::parseReport(const Mcp::Message& report)
  {
    const unsigned char* string_position = report.message();
    MIRO_LOG(LL_NOTICE , "parsing base status report: " );

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
      MIRO_LOG( LL_ERROR , "illegal status report!" );
      exit(1);
    }

    // these must remain in numerical order !!
    // as the base will pass them back that way

    MIRO_LOG( LL_NOTICE , "clock, " );
    currentReport.Clock = getULong(string_position);
    string_position += sizeof(unsigned long);

    MIRO_LOG( LL_NOTICE , "general, ");
    currentReport.GeneralStatus = getUShort(string_position);
    string_position += sizeof(unsigned short);

    MIRO_LOG( LL_NOTICE , "x, ");
    currentReport.Xpos = getUShort(string_position);
    string_position += sizeof(unsigned short);

    MIRO_LOG( LL_NOTICE , "x, ");
    currentReport.Xpos = (unsigned long)((double)(currentReport.Xpos) 
					 / POS_PER_CM * 10.0);

    currentReport.Ypos = getUShort(string_position);
    string_position += sizeof(unsigned short);

    currentReport.Ypos = (unsigned long)((double)(currentReport.Ypos) 
					 / POS_PER_CM * 10.0);

    MIRO_LOG( LL_NOTICE , "heading, ");
    currentReport.Heading = getUShort(string_position);
    string_position += sizeof(unsigned short);

    MIRO_LOG( LL_NOTICE , "rel. heading, ");
    currentReport.BaseRelativeHeading = getUShort(string_position);
    string_position += sizeof(unsigned short);

    MIRO_LOG( LL_NOTICE , "trans. error, ");
    currentReport.TranslateError = getUShort(string_position);
    string_position += sizeof(unsigned short);
    currentReport.TranslateError = (unsigned long) 
      ((double)currentReport.TranslateError / (ENC_PER_CM / 10.0));

    MIRO_LOG( LL_NOTICE , "trans. vel., ");
    currentReport.TranslateVelocity = getUShort(string_position);
    string_position += sizeof(unsigned short);
    currentReport.TranslateVelocity = (unsigned long) 
      ((double)currentReport.TranslateVelocity / (ENC_PER_CM / 10.0));

    MIRO_LOG( LL_NOTICE , "trans. status, ");
    currentReport.TranslateStatus = getUShort(string_position);
    string_position += sizeof(unsigned short);

    MIRO_LOG( LL_NOTICE , "rot. error, ");
    currentReport.RotateError = getUShort(string_position);
    string_position += sizeof(unsigned short);

    MIRO_LOG( LL_NOTICE , "rot. vel., ");
    currentReport.RotateVelocity = getUShort(string_position);
    string_position += sizeof(unsigned short);

    MIRO_LOG( LL_NOTICE , "rot. status, ");
    currentReport.RotateStatus = getUShort(string_position);
    string_position += sizeof(unsigned short);

    MIRO_LOG( LL_NOTICE , "done");

#ifdef VERBOSE
    MIRO_DBG_OSTR( MIRO , LL_PRATTLE , "report data: " << endl << currentReport
                << "\ncosHeading = " << currentReport.cosHeading 
	        << "\nsinHeading = " << sinHeading
	        << "\norigin.point.x = " << origin.point.x
	        << "origin.point.y = " << origin.point.y );
#endif
  }
};
