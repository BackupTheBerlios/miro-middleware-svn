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

#include "idl/KickerC.h"
#include "miro/Client.h"
#include "miro/TimeHelper.h"
#include "miro/Log.h"

#include <iostream>

int main(int argc, char *argv[])
{
  int rc = 1;

  // Initialize system log output.
  Miro::Log::init(argc, argv);
  // Initialize server daemon.
  Miro::Client client(argc, argv);

  try {
     // Reference to the server object
    Miro::Kicker_var kicker = client.resolveName<Miro::Kicker>("Kicker");

    std::cout << "Kicker test!" << std::endl
	      << "Kick duration in msec (0 to quit): " << std::endl;

    unsigned int duration;
    while(std::cin >> duration &&
	  duration > 0) {
      std::cout << "Kicker test!" << std::endl
		<< "Kick duration in msec (0 to quit): " << std::endl;
      
      ACE_Time_Value aT;
      aT.msec(duration);
      Miro::TimeIDL cT;
      Miro::timeA2C(aT, cT);
      MIRO_LOG_OSTR(LL_NOTICE, 
		    "Entered kick duration: " << 
		    cT.sec << "sec " << cT.usec << "usec");
      kicker->kick(cT);
      duration = 0;
    }
    rc = 0;
  }
  catch (const Miro::EOutOfBounds & e) {
    MIRO_LOG_OSTR(LL_CRITICAL, "Out of bounds exception on serve:" << std::endl << e);
  }
  catch (const Miro::EDevIO & e) {
    MIRO_LOG_OSTR(LL_CRITICAL, "Device IO exception on server:" << std::endl << e);
 }
  catch (const CORBA::Exception & e) {
    MIRO_LOG_OSTR(LL_CRITICAL, "Uncaught CORBA exception:" << std::endl << e);
  }
  return rc;
}




