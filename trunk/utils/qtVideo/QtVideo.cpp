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
//
// Authors: 
//   Hans Utz
//   Stefan Enderle
//   Stefan Sablatnoeg
//
#include "MainForm.h"

#include "miro/Log.h"

#include <ace/Get_Opt.h>

#include <qapplication.h>

using std::cout;
using std::cerr;
using std::endl;
using std::flush;
using std::cin;

using namespace Miro;

int parseArgs(int& argc, char* argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "rn:v?");
  
  int rc = 0;
  int c;
  
  while ((c = get_opts()) != -1) {
    switch (c) {
    case 'n':
      MainForm::interfaceName = get_opts.optarg;
      break;
    case 'r':
      MainForm::remote = true;
      break;
    case 'v':
      MainForm::verbose = true;
      break;
    case '?':
    default:
      cout << "usage: " << argv[0] << " [-r] [-n=interface name] [-?]" << endl
	     << "  -r using the remote video interface (default: false)" << endl
	     << "  -n interface name (default: Video)" << endl
	     << "  -v verbose mode" << endl
	     << "  -? help: emit this text and stop" << endl;
	rc = 1;
      }
    }

    if (MainForm::verbose)
      cout << "Remote: " << MainForm::remote << endl
	   << "Interface: " << MainForm::interfaceName << endl;

    return rc;
  }


int main(int argc, char *argv[])
{
  Miro::Log::init(argc, argv);
	  
  QApplication a(argc, argv);
  Miro::Client client(argc, argv);

  if (int rc = parseArgs(argc, argv) != 0)
    return rc;

  MainForm * mainForm = new MainForm(client);

  a.setMainWidget(mainForm);
  std::string caption = "QtVideo - Interface: "+MainForm::interfaceName;
  mainForm->setCaption(caption.c_str());
  mainForm->show();

  return a.exec();
}




