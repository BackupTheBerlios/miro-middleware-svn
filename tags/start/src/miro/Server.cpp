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


#include <ace/Synch.h>
#include <ace/Arg_Shifter.h>

#include <tao/ORB_Core.h>

#include "miro/Server.h"
#include "miro/Exception.h"

#include <iostream>

// #undef DEBUG


#ifdef DEBUG
#define DBG(x) x
#else
#define DBG(x)
#endif

namespace Miro
{
  Server::Event::Event(Server& server_) :
    server(server_)
  {
    DBG(cout << "Constructing Miro::Server::Event." << endl);
  }

  Server::Event::~Event()
  {
    DBG(cout << "Destructing Miro::Server::Event." << endl);
  }

  int
  Server::Event::handle_signal(int signum, siginfo_t *, ucontext_t *)
  {
    DBG(cerr << "Signal received: " << signum << endl);

    server.shutdown();

    return 0;
  }

  Server::Worker::Worker (CORBA::ORB_ptr orb, bool& shutdown) :
    orb_ (CORBA::ORB::_duplicate (orb)),
    shutdown_(shutdown)
  {
  }

  int
  Server::Worker::svc()
  {
    while (!shutdown_) {
      ACE_Time_Value timeSlice(0, 200000);
      orb_->perform_work(timeSlice);
    }
    return 0;
  }

  Server::Server(int& argc, char *argv[],
		 const RobotParameters& _params) :
    super(argc, argv, _params),
    rebind_(false),
    own_(true),
    shutdown_(false),
    event_(new Event(*this)),
    signals_(),
    worker_(orb_.in(), shutdown_)
  {
    DBG(cerr << "Constructing Miro::Server." << endl);

    // parse arguments
    ACE_Arg_Shifter arg_shifter(argc, argv);
    while (arg_shifter.is_anything_left()) {
      const ACE_TCHAR *current_arg = arg_shifter.get_current();

      const char rebindOpt[] = "-MiroRebindIOR";
      if (ACE_OS::strcasecmp(current_arg, rebindOpt) == 0) {
	arg_shifter.consume_arg();
	rebind_ = true;
      }
      else
	arg_shifter.ignore_arg ();
    }

    // Get reference to Root POA.
    poa = resolveInit<PortableServer::POA>("RootPOA");

    // Get POA manager
    poa_mgr = poa->the_POAManager();
        
    // register Signal handler for Ctr+C
    signals_.sig_add(SIGINT);
    signals_.sig_add(SIGTERM);

    if (reactor()->register_handler(signals_, event_) == -1) {
      throw ACE_Exception(errno, "Failed to register signal handler.");
    }

    // Activate the POA manager.
    poa_mgr->activate();

    DBG(cerr << "Miro::Server. constructed" << endl);
  }

  Server::Server(const Server& _server) :
    super(_server),
    poa(_server.root_poa()),
    poa_mgr(_server.poa_mgr),
    rebind_(_server.rebind_),
    own_(false),
    shutdown_(true),
    event_(NULL),
    signals_(),
    worker_(orb_.in(), shutdown_)
  {
    DBG(cerr << "Copy constructing Miro::Server." << endl);
  }

  Server::~Server()
  {
#ifdef DEBUG
    if (own_) {
      cout << "Destructing Miro::Server." << endl;
    }
    else {
      cout << "Destructing cloned Miro::Server." << endl;
    }
#endif

    if (!noNaming_) {
      CosNaming::Name n(1);
      n.length(1);
      try {
	NameVector::const_iterator i;
	for (i = nameVector_.begin(); i != nameVector_.end(); ++i) {
	  n[0].id = CORBA::string_dup(i->c_str());
	  namingContext->unbind(n);
	}
      }
      catch (const CORBA::Exception& e) {
	cerr << "Caught CORBA exception on unbinding: " << n[0].id << endl;
	cerr << "Porbably the NameSevice went down while we run:" << endl;
	cerr << e << endl;
      }
    }

    if (own_) {
      cout << "Deactivating POA manager." << endl;
      poa_mgr->deactivate(0, 0);
      cout << "Destroying the POA." << endl;
      poa->destroy(0, 0);
      cout << "Performing remaining work in POA" << endl;
      ACE_Time_Value timeSlice(0, 200000);
      orb_->perform_work(timeSlice);
      cout << "Destroying the ORB." << endl;
      orb_->shutdown(1);
      
      // remove signal handler from reactor
      reactor()->remove_handler(signals_);
      // since it is only registered for signals, 
      // handel_close isn't called
      delete event_;
    }
  }

  // Return the root POA reference
  PortableServer::POA_ptr
  Server::root_poa() const
  {
    return PortableServer::POA::_duplicate(poa.in());
  }

  void 
  Server::detach(int nthreads)
  {
    if (worker_.activate(THR_NEW_LWP | THR_JOINABLE, nthreads) != 0)
      throw Miro::Exception("Miro::Server: Cannot activate client threads");
  }


  void 
  Server::wait()
  {
    worker_.wait();
  }

  void
  Server::run(int nthreads)
  {
    if (nthreads > 1) {
      detach(nthreads);
      wait();
    }
    else
      worker_.svc();
  
    // Dectivate the POA manager.
    // poa_mgr->deactivate();
  }

  void
  Server::addToNameService(CORBA::Object_ptr _object, const std::string& _name)
  {
    if (!noNaming_) {
      CosNaming::Name n(1);
      n.length(1);
      n[0].id = CORBA::string_dup(_name.c_str());

      if (rebind_) {
	// Force binding of references to make
	// sure they are always up-to-date.
	try {
	  namingContext->unbind(n);

	  cerr << "Object still bound in naming service: " << _name << endl
	       << "Rebinding it." << endl;
	} catch (...) {
	}
      }

      try {
	namingContext->bind(n, _object);
      }
      catch (CosNaming::NamingContext::AlreadyBound& ) {
	cerr << "Object is already bound in naming service: "
	     << n[0].id << endl
	     << "Use -MiroRebindIOR if you really want to rebind it." << endl;
	throw(0);
      }

      nameVector_.push_back(_name);
    }      
  }

  void
  Server::shutdown()
  {
    // tell the orb to return from run()
    shutdown_ = true;
  }

  ACE_Reactor *
  Server::reactor()
  {
    // @@ Please see if there's a way to get to the Reactor without
    // using the TAO_ORB_Core_instance().
    return TAO_ORB_Core_instance()->reactor();
  }
};
