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
#ifndef FaulControllerConnection_h
#define FaulControllerConnection_h



namespace FaulController
{
  // forward declarations
  class Connection
  {


  public:
    Connection();
    virtual ~Connection();

    virtual void writeMessage(char const * const _message);

  };
};
#endif
