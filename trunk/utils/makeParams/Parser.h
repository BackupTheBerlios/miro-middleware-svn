// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2000, 2001, 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef Parser_h
#define Parser_h

#include "Class.h"

#include <qxml.h>

#include <memory>

class QString;
class Generator;

class Parser : public QXmlDefaultHandler
{
public:
  Parser(Generator& _generator);

  bool startDocument();
  bool startElement( const QString&, const QString&, const QString& , 
		     const QXmlAttributes& );
  bool endElement( const QString&, const QString&, const QString& );

private:
  Class class_;
  Generator& generator_;

  bool parsing_;

  bool instance_;

  bool string_;
  bool timeValue_;
  bool serialParams_;
  bool scanDescription_;
};
#endif
