// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "idl/BehaviourEngineC.h"
#include "miro/Client.h"

#include <qfile.h>

#include <iostream>

int main(int argc, char * argv[])
{
  Miro::Client client(argc, argv);
  Miro::BehaviourEngine_var engine;

  std::string transition;
  std::string file;
  char * policy = NULL;

  char c = 'R';
  while (c != 'q') {
    try {
      switch (c) {
      case 'R':
	engine = client.resolveName<Miro::BehaviourEngine>("BehaviourEngine");
	break;
	
      case '0':
	engine->openPolicy();
	break;
	
      case '1':
	engine->closePolicy();
	break;
	
      case 't':
	std::cout << "Transition: " << flush;
	std::cin >> transition;
	//	engine->sendTransition(transition.c_str());
	break;
	
      case 'f':
	std::cout << "Policy file: " << flush;
	std::cin >> file;
	engine->loadPolicyFile(file.c_str());
	break;
	
      case 'p': {
	std::cout << "Local policy file: " << flush;
	std::cin >> file;

	delete[] policy;
	policy = NULL;

	QFile f(file.c_str());
	if (f.exists()) {
	  f.open(IO_ReadOnly);
	  policy = new char[f.size() + 1];
	  memset(policy, 0, f.size() + 1);
	  f.readBlock(policy, f.size());

	  engine->loadPolicy(policy);
	}
	else {
	  std::cerr << "File not found error." << endl;
	}
	break;
      }
	
      default:
	std::cerr << "Unknown command." << endl;
	break;
      }
    }
    catch(const Miro::BehaviourEngine::EMalformedPolicy& e) {
      std::cerr << "Error parsing policy: " << e.what << endl;
    }
    catch(const Miro::BehaviourEngine::EMalformedXML& ) {
      std::cerr << "Malformed XML in file: " << file << endl;
    }
    catch(const Miro::BehaviourEngine::ENoPolicy& ) {
      std::cerr << "No policy loaded." << endl;
    }
    catch(const Miro::BehaviourEngine::EFile& ) {
      std::cerr << "Error loading file: " << file << endl;
    }
    catch(const CORBA::Exception& e) {
      std::cerr << "Communication Failed. CORBA exception: " << endl
		<< e << endl
		<< "Try R!" << endl;
    }

    std::cout << endl
	      << "Possible Commands:" << endl
	      << " 0: OPEN" << endl
	      << " 1: CLOSE" << endl
	      << " t: send transtion" << endl
	      << " f: load policy from file" << endl
	      << " p: load policy from local file" << endl
	      << " q: quit" << endl
	      << "Choice: " << flush;

    std::cin >> c;
    std::cout << c << endl;
  }
  return 0;
}
