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
#ifndef KeyValueList_h
#define KeyValueList_h

#include <ace/SString.h>
#include <ace/CDR_Stream.h>

#include <map>
#include <qstring.h>

namespace Miro
{
  class KeyValueList  
  {
  public:
    void addPair (const QString& _key, const QString& _value);
    QString  getValue(const QString& _key) const;
    void printToConsole();

  protected:
    typedef std::map<QString, QString> KeyValueListMap;
    KeyValueListMap parameters_;
  };
};
#endif
