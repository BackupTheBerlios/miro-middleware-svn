// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// for details copyright, usage and credits to other groups see Miro/COPYRIGHT
// for documentation see Miro/doc
// 
// (c) 1999,2000
// Department of Neural Information Processing, University of Ulm, Germany
//
// Authors: 
//   Stefan Enderle, 
//   Stefan Sablatnoeg, 
//   Hans Utz
// 
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////


#undef DEBUG

#include "miro/Client.h"

#include <ace/Arg_Shifter.h>
#include <tao/Strategies/advanced_resource.h>


#include <iostream>
#include <cstring>

// #undef DEBUG

#ifdef DEBUG
#define DBG(x) x
#else
#define DBG(x)
#endif

using std::cerr;
using std::string;
using std::ostream;

using CORBA::Object_var;
using CosNaming::NamingContext;
using CosNaming::Name;

namespace 
{
  const char * const namingKey = "-MiroNamingContext";
  const char * const shortNamingKey = "-MNC";
  const char * const noNamingKey = "-MiroNoNaming";
};

namespace Miro
{
  Client::Client(int& argc, char* argv[],
		 const RobotParameters& _params) :
    orb_(CORBA::ORB_init(argc, argv)), // Initialize orb
    params_(_params),
    noNaming_(false),
    namingContextName(params_.name)
  {
    DBG(cout << "Constructing Miro::Client" << endl);

    // resolve the used naming context
    ACE_Arg_Shifter arg_shifter (argc, argv);
    while (arg_shifter.is_anything_left ()) {
      const ACE_TCHAR *current_arg = arg_shifter.get_current ();

      if (ACE_OS::strcasecmp(current_arg, namingKey) == 0 ||
	  ACE_OS::strcasecmp(current_arg, shortNamingKey) == 0) {
	arg_shifter.consume_arg();
	if (arg_shifter.is_parameter_next()) {
	  namingContextName = arg_shifter.get_current ();
	  arg_shifter.consume_arg();
	}
      }
      else if (ACE_OS::strcasecmp(current_arg, noNamingKey) == 0) {
	arg_shifter.consume_arg();
	noNaming_ = true;
      }
      else
	arg_shifter.ignore_arg ();
    }

    if (!noNaming_) {
      if (params_.nameServiceIOR.length() == 0) {
	// Get reference to NameService
	initialNamingContext = resolveInit<NamingContext>("NameService");
      }
      else {
	Object_var tmp = orb_->string_to_object(params_.nameServiceIOR.c_str());
	initialNamingContext = NamingContext::_narrow(tmp.in());
      }

      // Attempt to create naming context.
      Name n;
      n.length(1);
      n[0].id = CORBA::string_dup(namingContextName.c_str());
      try {
	namingContext = initialNamingContext->bind_new_context(n);
      } catch (const NamingContext::AlreadyBound &) {
	// Fine, CCS context already exists.
	// get a reference for it
	namingContext = resolveName<NamingContext>(n);
      }
    }
  }

  Client::Client(const Client& _client) :
    orb_(_client.orb()), // Initialize orb
	                             // Get reference to NameService
    params_(_client.params_),
    noNaming_(_client.noNaming_),
    initialNamingContext(_client.initialNamingContext),
    namingContext(_client.namingContext),
    namingContextName(_client.namingContextName)
  {
    DBG(cout << "Constructing Miro::Client copy" << endl);
  }

  Client::~Client()
  {
    DBG(cout << "Destructing Miro::Client" << endl);
  }
  
  // Return the corba orb reference.
  CORBA::ORB_ptr
  Client::orb() const
  {
    return CORBA_ORB::_duplicate(orb_.in());
  }

};

// template class std::string;
// template NamingContext::_ptr_type Miro::Client::resolveInit<NamingContext>(const char *);

// #pragma instantiate std::string;
// #pragma instantiate Miro::Client::resolveInit<CosNaming::NamingContext>

  
