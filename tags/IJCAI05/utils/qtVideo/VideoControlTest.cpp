// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// for details copyright, usage and credits to other groups see Miro/COPYRIGHT
// for documentation see Miro/doc
// 
// (c) 1999, 2000, 2003
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

#include "VideoControlTestImpl.h"
#include "miro/Server.h"

int main(int argc, char *argv[])
{
    Miro::Server server(argc, argv);
    
    Video::ControlTestImpl * p_control = new Video::ControlTestImpl();
    Video::Control_var control = p_control->_this();
    server.addToNameService(control.in(), "VideoControl");

    server.run();
    
    return EXIT_SUCCESS;
}




