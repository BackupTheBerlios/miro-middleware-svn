// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// 
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef FaulCanConnection_h
#define FaulCanConnection_h

#include "FaulControllerConnection.h"
#include "sparrow/SparrowConnection2003.h"



namespace FaulMotor
{
  class Connection;
};

namespace FaulController
{
  // forward declarations


  class FaulCanConnection : public Connection
  {
    typedef Connection Super;

  public:
    FaulCanConnection(Sparrow::Connection2003 * _connection2003, int _motor);
    virtual ~FaulCanConnection();

    void writeMessage(char const * const _message);
    Sparrow::Connection2003 * connection2003_;
    int motor_;


  };
};
#endif
