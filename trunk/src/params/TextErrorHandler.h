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
#ifndef Miro_CFG_TextErrorHandler_h
#define Miro_CFG_TextErrorHandler_h

#include <qxml.h>

namespace Miro
{
  namespace CFG
  {
    class TextErrorHandler : public QXmlErrorHandler
    {
      typedef QXmlErrorHandler Super;

    public:
      
      virtual ~TextErrorHandler();
      
      virtual bool warning ( const QXmlParseException & exception );
      virtual bool error ( const QXmlParseException & exception );
      virtual bool fatalError ( const QXmlParseException & exception );
      virtual QString errorString ();
      
    protected:
      void createErrorMessage(const QString& type,
			      const QXmlParseException & exception);
      
      QString error_;
    };
  }
}

#endif // Miro_CFG_TextErrorHandler_h
