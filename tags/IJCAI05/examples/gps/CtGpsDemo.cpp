/*
 * This file is part of Miro (The Middleware for Robots)
 * Copyright (C) 2005 Ingo van Lil <ingo@vanlil.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 * $Id$
 */

#include "miro/Client.h"
#include "miro/IO.h"
#include "idl/CtGpsC.h"

#include <iostream>
#include <stdio.h>

using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::flush;

using Miro::Client;
using Miro::CtGps;
using Miro::CtGps_var;
using Miro::GpsGlobalPositionEventIDL;
using Miro::ETimeOut;

int
main(int argc, char *argv[])
{
  // Initialize server daemon.
  Client client(argc, argv);

  try {
    CtGps_var gps = client.resolveName<CtGps>("GPS");

    while (1) {
      char c;
      cout << "CtGps demo application:" << endl
	   << " 0 - Select DGPS source" << endl
	   << " 1 - Configure altitude aiding" << endl
	   << " 2 - Enable/disable NMEA sentences" << endl
	   << " 3 - Save configuration" << endl
	   << " q - Quit" << endl;
      cout << "> ";
      cin >> c;
      if (c == 'q')
	break;

      switch (c) {
	case '0': // Select DGPS source
	  cout << "Select DGPS source:" << endl
	       << " 0 - Turn off DGPS" << endl
	       << " 1 - This port (not supported by implementation)" << endl
	       << " 2 - Other port" << endl
	       << " 3 - Beacon port (RTCM)" << endl
	       << " 4 - L-Band" << endl
	       << " 5 - WAAS/EGNOS/MSAS" << endl;
	  cout << "> ";
	  cin >> c;
	  switch (c) {
	    case '0':
	      gps->setDifferentialSource(Miro::CtGps::NONE);
	      cout << "DGPS disabled." << endl;
	      break;
	    case '1':
	      gps->setDifferentialSource(Miro::CtGps::THIS);
	      cout << "Getting differential from this port." << endl;
	      break;
	    case '2':
	      gps->setDifferentialSource(Miro::CtGps::OTHER);
	      cout << "Getting differential from other port." << endl;
	      break;
	    case '3':
	      gps->setDifferentialSource(Miro::CtGps::BEACON);
	      cout << "Getting differential from beacon port." << endl;
	      break;
	    case '4':
	      gps->setDifferentialSource(Miro::CtGps::LBAND);
	      cout << "Getting differential from L-Band." << endl;
	      break;
	    case '5':
	      gps->setDifferentialSource(Miro::CtGps::WAAS);
	      cout << "Getting differential from WAAS/EGNOS/MSAS." << endl;
	      break;
	  }
	  break;

	case '1': // Configure altitude aiding
	  cout << "Configure altitude aiding:" << endl
	       << " 0 - Off (Disable altitude aiding)" << endl
	       << " 1 - Sometimes (if PDOP > threshold)" << endl
	       << " 2 - Always (Set fixed altitude)" << endl;
	  cout << "> ";
	  cin >> c;
	  switch (c) {
	    case '0':
	      gps->setAltitudeAidingNever();
	      cout << "Altitude aiding disabled." << endl;
	      break;
	    case '1':
	      float t;
	      cout << "Enter PDOP threshold: ";
	      cin >> t;
	      gps->setAltitudeAidingSometimes(t);
	      cout << "Set PDOP threshold to " << t << "." << endl;
	      break;
	    case '2':
	      float a;
	      cout << "Enter fixed altitude value (in m): ";
	      cin >> a;
	      gps->setAltitudeAidingAlways(a);
	      cout << "Set fixed altitude to " << a << " meters." << endl;
	      break;
	  }
	  break;

	case '2': // Enable/disable NMEA sentences
	  float r;
	  cout << "Select NMEA sentence to enable/disable:" << endl
	       << " 0 - GPGGA" << endl
	       << " 1 - GPGLL" << endl
	       << " 2 - GPGSA" << endl
	       << " 3 - GPGST" << endl
	       << " 4 - GPGSV" << endl
	       << " 5 - GPRMC" << endl
	       << " 6 - GPRRE" << endl
	       << " 7 - GPVTG" << endl
	       << " 8 - GPZDA" << endl
	       << " 9 - Turn off all sentences" << endl;
	  cout << "> ";
	  cin >> c;
	  if (c == '9') {
	    gps->setSentencesOff();
	    cout << "All NMEA sentences disabled." << endl;
	    break;
	  } else if (c=='2' || c=='3' || c=='4' || c=='6')
	    cout << "Enter new rate (0 (off) or 1 Hz): ";
	  else if (c=='0' || c=='1' || c=='5' || c=='7' || c=='8')
	    cout << "Enter new rate (0 (off), 0.2 Hz, 1 Hz or 5 Hz): ";
	  else break;
	  cin >> r;
	  {
	    Miro::CtGps::NmeaSentence s =
	      c == '0'? Miro::CtGps::GPGGA :
	      c == '1'? Miro::CtGps::GPGLL :
	      c == '2'? Miro::CtGps::GPGSA :
	      c == '3'? Miro::CtGps::GPGST :
	      c == '4'? Miro::CtGps::GPGSV :
	      c == '5'? Miro::CtGps::GPRMC :
	      c == '6'? Miro::CtGps::GPRRE :
	      c == '7'? Miro::CtGps::GPVTG : Miro::CtGps::GPZDA;
	    gps->setSentenceRate(s, r);
	  }
	  break;

	case '3': // Save configuration
	  cout << "This will make the GPS device's current configuration "
	       << "permanent!" << endl
	       << "Are you sure? [y/n]: ";
	  cin >> c;
	  if (c == 'y' || c == 'Y') {
	    gps->saveConfig();
	    cout << "Configuration successfully saved." << endl;
	  } else
	    cout << "Configuration NOT saved." << endl;
	  break;

	default:
	  cout << "Unknown command." << endl;
      }
    }
  }
  catch (ETimeOut) {
    cerr << "Timeout.\n";
    return 1;
  }
  catch (const CORBA::Exception & e) {
    cerr << "Uncaught CORBA exception: " << e << endl;
    return 1;
  }
  return 0;
}

