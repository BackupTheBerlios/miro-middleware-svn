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

#include "MainForm.h"

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
  QApplication a(argc, argv);
  Miro::Client client(argc, argv);

  if (int rc = parseArgs(argc, argv) != 0)
    return rc;

  MainForm * mainForm = new MainForm(client);

  a.setMainWidget(mainForm);
  mainForm->setCaption("QtVideo");
  mainForm->show();

  return a.exec();
}




