// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2002, 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "TextErrorHandler.h"

#include <sstream>

namespace Miro
{
  namespace CFG
  {
    TextErrorHandler::~TextErrorHandler()
    {
    }

    bool 
    TextErrorHandler::warning ( const QXmlParseException & exception )
    {
      createErrorMessage("Warning: ", exception);
      return false;
    }

    bool
    TextErrorHandler::error ( const QXmlParseException & exception )
    {
      createErrorMessage("Error: ", exception);
      return false;
    }

    bool
    TextErrorHandler::fatalError ( const QXmlParseException & exception ) 
    {
      createErrorMessage("Fatal error: ", exception);
      return false;
    }

    QString
    TextErrorHandler::errorString () {
      return error_;
    }

    void
    TextErrorHandler::createErrorMessage(const QString& type, const QXmlParseException & exception) 
    {
      std::stringstream ostr;
      ostr << type 
	   << " in line " << exception.lineNumber() << " "
	   << ", column " << exception.columnNumber() << std::endl
	//       << "public id: " << exception.publicId() << endl
	//       << "system id: " << exception.systemId() << endl
	   << exception.message() << std::endl;

      error_ = ostr.str().c_str();
    }
  }
}
