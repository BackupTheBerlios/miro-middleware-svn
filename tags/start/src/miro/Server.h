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
#ifndef miroServer_hh
#define miroServer_hh

#include "miro/Client.h"

#include <ace/Event_Handler.h>
#include <ace/Signal.h>
#include <ace/Task.h>

#include <orbsvcs/Naming/Naming_Utils.h>
#include <orbsvcs/CosNamingC.h>

#include <vector>

// forward declarations
class ACE_Reactor;

namespace Miro
{
  class Server : public Client
  {
    typedef Client super;
    typedef Server self;

    //--------------------------------------------------------------------------
    // nested class definitions
    //--------------------------------------------------------------------------

    // MiroEvent shouldn´t be subclassed any further,
    // so we make it private to MiroServer

    class Event : public ACE_Event_Handler
    {
    public:
      Event(Server& server_);
      //  private:
      virtual ~Event();
    public:
      
      virtual int handle_signal (int signum, siginfo_t *, ucontext_t *);
      virtual int handle_close (ACE_HANDLE, ACE_Reactor_Mask) {
	cout << "Miro server handle_close called." << endl;
	return 0;
      }
      // Called when object is removed from the <ACE_Reactor>.

    private:
      Server& server;
    };

    class Worker : public ACE_Task_Base
    {
      // = TITLE
      //   Run a server thread
      //
      // = DESCRIPTION
      //   Use the ACE_Task_Base class to run server threads
      //
    public:
      Worker(CORBA::ORB_ptr orb, bool& shutdown);
      // ctor

      virtual int svc();
      // The thread entry point.

    private:
      CORBA::ORB_var orb_;
      // The orb
      bool& shutdown_;
    };

  public:

    /**
     * Constructor.
     */
    Server(int& argc, char *argv[],
	   const RobotParameters& _params = RobotParameters());
    Server(const Server& _server);
    /**
     * Destructor.
     */
    ~Server();

    /**
     * Executes the server main routine.
     */
    void run(int nthreads = 1);
    void detach(int nthreads = 1);
    void wait();

    PortableServer::POA_ptr root_poa() const;
    // Accessor which returns the root poa. Following the normal CORBA
    // memory management rules of return values from functions, this
    // function duplicates the poa return value before returning it.

    /**
     * Returns the reactor instance used by the ORB core.
     *
     * Be careful to use this reactor with any mutex protected servant
     * code. It's easy to deadlock. Create your own reactor task instead.
     * Anyhow, as long as you just want to use some timers etc. go ahead.
     */
    ACE_Reactor * reactor();

    /**
     * Shutdown the application. 
     *
     * This method is called on SIGINT to
     * cause the orb to return from its event loop.
     *
     * If you have stuff that has to be executed just before the orb is
     * returning from orb->run(), overwrite this method and call 
     * super::close() in the end.
     */
    virtual void shutdown();

    void addToNameService(CORBA::Object_ptr _object, const std::string& _name);

    bool rebind() const;

//  protected:
    /**
     * Reference to Root POA.
     */
    PortableServer::POA_var poa;

    /**
     * Reference to POA manager.
     */
    PortableServer::POAManager_var poa_mgr;

    bool rebind_;
    bool own_;

    typedef std::vector<std::string> NameVector;
    NameVector nameVector_;

//  private:
    bool shutdown_;

    // Event handler for SIGINT.
    // Triggers clean shutdown of the server.
    Event *event_;

    // Signal set to be handled by the event handler.
    ACE_Sig_Set signals_;

    Worker worker_;
  };

  inline
  bool 
  Server::rebind() const {
    return rebind_;
  }
};
#endif




