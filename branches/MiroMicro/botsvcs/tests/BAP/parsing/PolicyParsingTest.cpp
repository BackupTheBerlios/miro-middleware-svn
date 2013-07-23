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
#include "../lib/DummyFactory.h"

#include "idl/BehaviourEngineC.h"
#include "botsvcs/BAP/BehaviourRepository.h"
#include "botsvcs/BAP/ArbiterRepository.h"
#include "botsvcs/BAP/ActionPattern.h"
#include "botsvcs/BAP/Policy.h"

using std::cout;
using std::cerr;
using std::endl;
using std::flush;
using std::cin;

int
main(int argc, char* argv[])
{
  // command line argument parsing
  if (argc <= 1) {
    std::cout << "usage: " << argv[0] << " <policy file>" << endl;
    return 1;
  }

  try {
    // the available behaviours and arbiters
    DummyFactory factory;
    factory.init();
    
    // parsing the policy
    Miro::BAP::Policy policy;
    Miro::BAP::Policy::loadPolicyFile(policy, argv[1]);

    cout << "Policy: " << endl 
	 << policy;

    cout << "closing down policy" << std::endl;
    factory.fini();
  }
  catch(const Miro::BehaviourEngine::EMalformedPolicy& e) {
    std::cerr << "Error parsing policy: " << e.what << endl;
  }
  catch(const Miro::BehaviourEngine::EMalformedXML& ) {
    std::cerr << "Malformed XML in file: " << argv[1] << endl;
  }
  catch(const Miro::BehaviourEngine::ENoPolicy& ) {
    std::cerr << "No policy loaded" << endl;
  }
  catch(const Miro::BehaviourEngine::EFile& ) {
    std::cerr << "Error loading file: " << argv[1] << endl;
  }
  catch (const Miro::Exception& e) {
    std::cout << endl
	      << "Miro::Exception:" << endl
	      << e << endl;
  }

  return 0;
}
