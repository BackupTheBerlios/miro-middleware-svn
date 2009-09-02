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
#ifndef miroServer_h
#define miroServer_h

#include "Client.h"
#include "Singleton.h"

#include <ace/Event_Handler.h>
#include <ace/Signal.h>
#include <ace/Task.h>

#include <tao/Version.h>
#if TAO_MAJOR_VERSION > 1 || \
	        ( TAO_MAJOR_VERSION == 1 && TAO_MINOR_VERSION > 4 ) || \
        ( TAO_MAJOR_VERSION == 1 && TAO_MINOR_VERSION == 4 && TAO_BETA_VERSION >= 3 )
#include <orbsvcs/Naming/Naming_Client.h>
#include <orbsvcs/Naming/Naming_Server.h>
#else
#include <orbsvcs/Naming/Naming_Utils.h>
#endif

#include <orbsvcs/CosNamingC.h>

#include <vector>

//! forward declaration
class ACE_Reactor;

namespace Miro
{
  class Server : public Client
  {
    typedef Client Super;
    typedef Server Self;

    //--------------------------------------------------------------------------
    // nested class definitions
    //--------------------------------------------------------------------------

    // MiroEvent shouldn´t be subclassed any further,
    // so we make it private to MiroServer

    //! Detached server thread pool.
    /**
     *   Uses the ACE_Task_Base class to run server threads
     */
  class Worker : public ACE_Task_Base
    {
      typedef ACE_Task_Base Super;
    public:
      //! Initializing constructor.
      Worker(ACE_Thread_Manager * _threadManager,
             CORBA::ORB_ptr _orb, bool& _shutdown);

      //! The thread entry point.
      virtual int svc();

      // Singleton instance of worker threads.
      //      static Snigleton<Worker> instance;

    protected:
      //! The orb.
      CORBA::ORB_var orb_;
      //! Cooperative shutdown indicator.
      bool& shutdown_;
    };

  public:

    //! Constructor.
    Server(int& argc, char *argv[],
           const RobotParameters& _params = *RobotParameters::instance());
    Server(const Server& _server);
    ~Server();

    //! Executes the server main routine.
    void run(unsigned int nthreads = 1);
    //! Detaches the server main routine.
    void detach(unsigned int nthreads = 1);
    //! Wait for server main loop completion.
    void wait();

    //! Accessor which returns the root poa.
    PortableServer::POA_ptr rootPoa() const;

    //! Returns the reactor instance used by the ORB core.
    ACE_Reactor * reactor();

    //! Shutdown the application.
    virtual void shutdown();

    //! Register an object reference at the CORBA naming service.
    void addToNameService(CORBA::Object_ptr _object, const std::string& _name);
    //! Register an object reference at the CORBA naming service.
    void addToNameService(CORBA::Object_ptr _object,
                          CosNaming::NamingContext_ptr _context,
                          const std::string& _name);

    bool rebind() const;

//  protected:
    //! Reference to Root POA.
    PortableServer::POA_var poa;

    //!  Reference to POA manager.
    PortableServer::POAManager_var poa_mgr;

    //! Force rebinding of references in the naming service
    bool rebind_;
    bool own_;

    typedef std::vector<std::string> NameVector;
    typedef std::vector<std::pair<CosNaming::NamingContext_ptr, std::string> > ContextNameVector;
    NameVector nameVector_;
    ContextNameVector contextNameVector_;

  private:
    bool shutdown_;

    //! Signal set to be handled by the event handler.
    ACE_Sig_Set signals_;

    ACE_Thread_Manager threadManager_;

    //! Shared worker thread pool.
    Worker worker_;
  };

  inline
  bool
  Server::rebind() const
  {
    return rebind_;
  }
};
#endif




