// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
//
//////////////////////////////////////////////////////////////////////////////


#include "SparrowBaseConnection.h"
#include "Parameters.h"

#include "can/CanMessage.h"

#include "miro/Exception.h"

#include <ace/Reactor.h>

#ifdef DEBUG
#define DBG(x) x
#define CSDBG(x) x
#else
#define DBG(x)
#define CSDBG(x)
#endif


namespace Sparrow
{
  using std::cout;
  using std::cerr;
  using std::endl;

  using Can::Message;

  //ACE_Time_Value maxTimeout(0, 500000);

  //------------------------//
  //----- constructors -----//
  //------------------------//

  BaseConnection::BaseConnection(ACE_Reactor* _reactor,
			 Miro::DevEventHandler* _devEventHandler):
			 //			 EventHandler _eventHandler,

    Super(_reactor, _devEventHandler, *Parameters::instance()),
    reactor(_reactor),
    params_(Parameters::instance())

  {
    DBG(cout << "Constructing SparrowConnection." << endl);
    std::cout << "Reactor" << (void*) reactor << endl;
    std::cout << "_Reactor" << (void*) _reactor << endl;
    

  }

  void
  BaseConnection::init()
  {

  }


  //----------------------//
  //----- destructor -----//
  //----------------------//

  BaseConnection::~BaseConnection()
  {
    DBG(cout << "Destructing SparrowConnection." << endl);



  }

};
