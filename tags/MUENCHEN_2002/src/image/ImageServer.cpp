// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////


#include "ImageServer.h"
#include "ImageImpl.h"

#include <iostream>

#include <sched.h>

using namespace Miro;

ImageServer::ImageServer(int argc, char *argv[]) :
  Miro::Server(argc, argv)
{
  CosNaming::Name n;

  image      = new ImageImpl  ( positionStamps, NULL );

  Miro::Image_var pImage(image->_this());

  // Force binding of controller reference to make
  // unbind first, if already bound
  // sure it is always up-to-date.
  n.length(1);
  n[0].id = CORBA::string_dup("Image");
  try {
    namingContext->unbind(n);
  } catch (...) {
   }
  try {
    namingContext->bind(n, pImage.in());
  }
  catch (CosNaming::NamingContext::AlreadyBound& ) {
    cerr << "Objects were still bound in naming service: " 
	 << n[0].id << endl
	 << "Use -MiroRebindIOR if you really want to rebind them." << endl;
    
    throw(0);
  }

  // get reference to corba object Odometry
  // if it fails, we wont use position stamps
  positionStamps = true;
  try {
    odometry = Miro::resolve_name<Miro::Odometry>((*this), "Motion");
  } catch (const CORBA::Exception & e) {
    cerr << "got CORBA exception will not use positionStamps " << e << endl;
    positionStamps = false;
  }
}

ImageServer::~ImageServer()
{
  CosNaming::Name n;
  n.length(1);
  n[0].id = CORBA::string_dup("Image");
  namingContext->unbind(n);
}

void ImageServer::shutdown() {
  // remove imageImpl, this unregisters imageEvent from reactorTask,
  // leaving it free for suicide after this
   Server::shutdown();
}

int
main(int argc, char *argv[])
{
  try {
    ImageServer imageServer(argc, argv);
    imageServer.run();
  }
  catch (const CORBA::Exception & e) {
    cerr << e << endl;
    return 1;
  }
  catch (...) {
     return 1;
  }
  return 0;
}




