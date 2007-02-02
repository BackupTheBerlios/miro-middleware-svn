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
#ifndef Miro_CFG_Parser_h
#define Miro_CFG_Parser_h

#include "Type.h"

#include <qxml.h>

#include <memory>

// forward declarations
class QString;

namespace Miro
{
  namespace CFG
  {
    // forward declarations
    class Generator;


    class Parser : public QXmlDefaultHandler
    {
    public:
      Parser(Generator& _generator);

      bool startDocument();
      bool startElement(QString const&, QString const&, QString const& , 
			QXmlAttributes const&);
      bool endElement(QString const&, QString const&, QString const&);

      QString errorString();
      bool characters(QString const& ch);
  
    private:
      void reset();

      Type type_;
      Generator& generator_;

      QString group_;

      bool parsing_;
      bool parameterParsing_;
      bool docuParsing_;
      bool ctorParsing_;

      bool staticConst_;
      bool instance_;
      bool string_;
      bool enumeration_;
      bool enumerationMultiple_;
      bool vector_;
      bool set_;
      bool angle_;
      bool timeValue_;
      bool inetAddr_;
      bool serialParams_;
      bool scanDescription_;
      bool schedParams_;

      QString docu_;
      QString ctor_;
      QString error_;
    };
  }
}
#endif // Miro_CFG_Parser_h
