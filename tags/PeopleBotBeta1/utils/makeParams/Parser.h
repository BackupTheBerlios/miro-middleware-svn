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
#ifndef Parser_h
#define Parser_h

#include <qxml.h>

#include <memory>

class QString;
class Generator;

class Parser : public QXmlDefaultHandler
{
public:
  bool startDocument();
  bool startElement( const QString&, const QString&, const QString& , 
		     const QXmlAttributes& );
  bool endElement( const QString&, const QString&, const QString& );

private:
  Generator * generator_;

  bool instance_;

  bool string_;
  bool timeValue_;
  bool scanDescription_;
};
#endif
