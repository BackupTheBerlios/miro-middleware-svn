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
#include "Server.h"
#include "Exception.h"

#include <ace/Synch.h>
#include <ace/Arg_Shifter.h>
#include <ace/OS.h>

#include <tao/ORB_Core.h>
#include <miro/Log.h>

static Miro::Server * pServer;

extern "C" void handler(int signum)
{
  // check of sigint and sigterm
  if (signum == SIGINT || signum == SIGTERM) {
    MIRO_LOG_OSTR(LL_CRITICAL, "Signal received: " << signum);
    pServer->shutdown();
  }
}


namespace Miro
{
  using ::operator<<;

  /**
   * @param orb      The Object request broker.
   * @param shutdown Cooperative shutdown indicator.
   */
  Server::Worker::Worker(ACE_Thread_Manager * _threadManager,
                         CORBA::ORB_ptr orb, bool& shutdown) :
      Super(_threadManager),
      orb_(CORBA::ORB::_duplicate(orb)),
      shutdown_(shutdown)
  {
  }

  int
  Server::Worker::svc()
  {
    MIRO_LOG(LL_NOTICE, "Entering (detached) server loop.");
    while (!shutdown_) {
      ACE_Time_Value timeSlice(0, 200000);
      orb_->perform_work(timeSlice);
    }
    MIRO_LOG(LL_NOTICE, "Leaving (detached) server loop.");
    return 0;
  }

  Server::Server(int& argc, char *argv[],
                 const RobotParameters& _params) :
      Super(argc, argv, _params),
      rebind_(true),
      own_(true),
      shutdown_(false),
      signals_(),
      worker_(&threadManager_, orb_.in(), shutdown_)
  {
    MIRO_DBG(MIRO, LL_CTOR_DTOR, "Constructing Miro::Server.\n");

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
        arg_shifter.ignore_arg();
    }

    // Get reference to Root POA.
    poa = resolveInit<PortableServer::POA>("RootPOA");

    // Get POA manager
    poa_mgr = poa->the_POAManager();

    // register Signal handler for Ctr+C
    signals_.sig_add(SIGINT);
    signals_.sig_add(SIGTERM);

    pServer = this;
    ACE_Sig_Action sa((ACE_SignalHandler) handler, SIGINT);
    ACE_Sig_Action sa2((ACE_SignalHandler) handler, SIGTERM);

    // Activate the POA manager.
    poa_mgr->activate();

    MIRO_DBG(MIRO, LL_CTOR_DTOR, "Miro::Server. constructed\n");
  }

  Server::Server(const Server& _server) :
      Super(_server),
      poa(_server.rootPoa()),
      poa_mgr(_server.poa_mgr),
      rebind_(_server.rebind_),
      own_(false),
      shutdown_(true),
      signals_(),
      worker_(&threadManager_, orb_.in(), shutdown_)
  {
    MIRO_DBG(MIRO, LL_NOTICE, "Copy constructing Miro::Server.\n");
  }

  Server::~Server()
  {
    if (own_) {
      MIRO_DBG(MIRO, LL_CTOR_DTOR, "Destructing Miro::Server.\n");
    }
    else {
      MIRO_DBG(MIRO, LL_CTOR_DTOR, "Destructing cloned Miro::Server.\n");
    }

    if (!noNaming_) {
      CosNaming::Name n(1);
      n.length(1);
      try {
        NameVector::const_iterator i;
        for (i = nameVector_.begin(); i != nameVector_.end(); ++i) {
          n[0].id = CORBA::string_dup(i->c_str());
          namingContext->unbind(n);
        }
        ContextNameVector::const_iterator j;
        for (j = contextNameVector_.begin(); j != contextNameVector_.end(); ++j) {
          n[0].id = CORBA::string_dup(j->second.c_str());
          j->first->unbind(n);
          CORBA::release(j->first);
        }
      }
      catch (const CORBA::Exception& e) {
        MIRO_LOG_OSTR(LL_ERROR, "Caught CORBA exception on unbinding: " << n[0].id
                      << "\nProbably the NameSevice went down while we run:\n" << e);
      }
    }

    if (own_) {
      MIRO_LOG(LL_NOTICE, "Deactivating POA manager.\n");
      //poa_mgr->deactivate(0, 0);
      MIRO_LOG(LL_NOTICE, "Destroying the POA.\n");
      //poa->destroy(0, 0);
      MIRO_LOG(LL_NOTICE, "Performing remaining work in POA\n");
      ACE_Time_Value timeSlice(0, 200000);
      //orb_->perform_work(timeSlice);
      MIRO_LOG(LL_NOTICE, "Destroying the ORB.\n");
      //orb_->shutdown(1);
    }
  }

  // Return the root POA reference
  /**
   * Following the normal CORBA memory management rules of return
   * values from functions, this function duplicates the poa return
   * value before returning it.
   */
  PortableServer::POA_ptr
  Server::rootPoa() const
  {
    return PortableServer::POA::_duplicate(poa.in());
  }

  /**
   * Starts the server main loop as a parallel threadpool. Returns
   * emediately.
   *
   * @param nthreads Number of threads in the threadpool.
   */
  void
  Server::detach(unsigned int nthreads)
  {
    if (worker_.activate(THR_NEW_LWP | THR_JOINABLE, nthreads) != 0)
      throw Miro::Exception("Miro::Server: Cannot activate client threads");
  }


  void
  Server::wait()
  {
    worker_.wait();
  }

  /**
   * Starts the server main loop, waits for shutdown
   * of the server.
   *
   * @param nthreads Number of threads in the threadpool.
   */
  void
  Server::run(unsigned int nthreads)
  {
    if (nthreads > 1) {
      detach(nthreads - 1);
    }
    worker_.svc();
    worker_.wait();
    // Dectivate the POA manager.
    // poa_mgr->deactivate();
  }

  /**
   * Adds an IOR at the naming service in the servers
   * naming context.
   */
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
          MIRO_LOG_OSTR(LL_ERROR, "Object still bound in naming service: " << _name
                        << "\nRebinding it.");
        }
        catch (...) {
        }
      }

      try {
        namingContext->bind(n, _object);
      }
      catch (CosNaming::NamingContext::AlreadyBound&) {
        MIRO_LOG_OSTR(LL_ERROR, "Object is already bound in naming service: "
                      << n[0].id
                      << "\nUse -MiroRebindIOR if you really want to rebind it.");
        throw(0);
      }

      nameVector_.push_back(_name);
    }
  }

  /**
   * Adds an IOR at the naming service in a specified
   * naming context.
   */
  void
  Server::addToNameService(CORBA::Object_ptr _object,
                           CosNaming::NamingContext_ptr _context,
                           const std::string& _name)
  {
    if (!noNaming_) {
      CosNaming::Name n(1);
      n.length(1);
      n[0].id = CORBA::string_dup(_name.c_str());

      if (rebind_) {
        // Force binding of references to make
        // sure they are always up-to-date.
        try {
          _context->unbind(n);
          MIRO_LOG_OSTR(LL_ERROR, "Object still bound in naming service: " << _name  << "\nRebinding it.");
        }
        catch (...) {
        }
      }

      try {
        _context->bind(n, _object);
      }
      catch (CosNaming::NamingContext::AlreadyBound&) {
        MIRO_LOG_OSTR(LL_ERROR,  "Object is already bound in naming service: "
                      << n[0].id
                      << "\nUse -MiroRebindIOR if you really want to rebind it.");
        throw(0);
      }

      contextNameVector_.push_back(std::make_pair(CosNaming::NamingContext::_duplicate(_context), _name));
    }
  }

  /**
   * Signals the server main loop to stop. The call returns
   * emediately.  Use @ref wait() to wait for completion.
   */
  void
  Server::shutdown()
  {
    // tell the orb to return from run()
    shutdown_ = true;
  }

  /**
   * Be careful to use this reactor with any mutex protected servant
   * code. It's easy to deadlock. Create your own reactor task
   * instead.  Anyhow, as long as you just want to use some timers
   * etc. go ahead.
   */
  ACE_Reactor *
  Server::reactor()
  {
    // @@ Please see if there's a way to get to the Reactor without
    // using the TAO_ORB_Core_instance().
    return TAO_ORB_Core_instance()->reactor();
  }
};
