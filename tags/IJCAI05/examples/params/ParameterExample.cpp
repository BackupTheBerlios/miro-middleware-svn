// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "Parameters.h"

#include "miro/Utils.h"
#include "miro/Exception.h"

using std::cout;
using std::cerr;
using std::endl;

int
main(int argc, char *argv[])
{
  int rc = 0;

  Test::MyParameters * params = Test::MyParameters::instance();

  try {
    // Config file processing
    Miro::ConfigDocument * config =
      new Miro::ConfigDocument(argc, argv, "Config.xml");
    config->setSection("test");
    config->getParameters("Test", *params);
    delete config;

    cout << "  test paramters:" << endl << *params << endl << endl;

    QDomDocument doc("MiroConfigDocument");
    QDomElement e = doc.createElement("config");
    doc.appendChild(e);
    QDomElement f = doc.createElement("group");
    f.setAttribute("name", "test");
    e.appendChild(f);

    QDomElement g = (*params) >>= f;
    g.setAttribute("name", "Test");

    QString s = doc.toString();
    std::cout << s;
  }
  catch (const Miro::CException& e) {
    cerr << "Miro exception: " << e << endl;
    rc = 1;
  }
  catch (const Miro::Exception& e) {
    cerr << "Miro exception: " << e << endl;
    rc = 1;
  }

  return rc;
}




