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
#include "Parameters.h"

#include "miro/Log.h"
#include "miro/Configuration.h"
#include "miro/Exception.h"

using namespace std;

int
main(int argc, char *argv[])
{
  int rc = 0;

  // command line paramter parsing
  // -> increase log level for verbosity (-MLL 7)
  Miro::Log::init(argc, argv);
  // -> specify the config file (-MCF Config.xml)
  Miro::Configuration::init(argc, argv);
  // Get global configuration document
  Miro::ConfigDocument * config = Miro::Configuration::document();

  // Read parameters from document...
  try {
    // global parameters
    // -> those are accessed through singletons
    Test::MyParameters * params = Test::MyParameters::instance();
    Test::MoreParameters * more_params = Test::MoreParameters::instance();

    // individual parameters
    // -> those are locally allocated
    Test::FriendsParameters tom;
    Test::FriendsParameters cherry;

    // selecting the paramter section
    config->setSection("My_test_section");

    // reading global parameters (if present)
    // -> no name required
    config->getParameters("Test::MyParameters", *params);
    config->getParameters("Test::MoreParameters", *more_params);

    // reading individual parameter settings (if present)
    // -> referred by name
    config->getType("Test::FriendsParameters", "Tom", tom);
    config->getType("Test::FriendsParameters", "Cherry", cherry);

    // debug output 
    cout << "  my paramters:" << endl << *params << endl << endl;
    cout << "  more paramters:" << endl << *more_params << endl << endl;
    cout << "  tom's paramters:" << endl << tom << endl << endl;
    cout << "  cherry's paramters:" << endl << cherry << endl << endl;

  }
  catch (const Miro::CException& e) {
    cerr << "Miro exception: " << e << endl;
    rc = 1;
  }
  catch (const Miro::Exception& e) {
    cerr << "Miro exception: " << e << endl;
    rc = 1;
  }

  return rc;
}




