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

#include "VideoControlDialogImp.h"

#include "miro/Client.h"

#include <qapplication.h>

using namespace Miro;

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  Miro::Client client(argc, argv);

  VideoControlDialogImp * dialog = new VideoControlDialogImp(client);

  a.setMainWidget(dialog);
  dialog->setCaption("QtVideoControl");
  dialog->show();

  return a.exec();
}




