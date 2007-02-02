// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef SimpleParameter_h
#define SimpleParameter_h

#include "ParameterXML.h"
#include "params/Parameter.h"

#include <qstring.h>

// forward declarations
class ConfigFile;
class QListView;
class QListViewItem;

class SimpleParameter : public ParameterXML
{
  Q_OBJECT
  
  //----------------------------------------------------------------------------
  // private types
  //----------------------------------------------------------------------------

  //! Base class type.
  typedef ParameterXML Super;
public:

  //----------------------------------------------------------------------------
  // public types
  //----------------------------------------------------------------------------

  enum Type {
    NONE,
    BOOL, CHAR, SHORT, USHORT, INT, UINT, LONG, ULONG, 
    DOUBLE,
    ANGLE, MIRO_ANGLE, STRING, ACE_TIME_VALUE, ACE_INET_ADDR,
    ENUMERATION, ENUMERATIONMULTIPLE
  };

  //----------------------------------------------------------------------------
  // public methods
  //----------------------------------------------------------------------------

  SimpleParameter(Miro::CFG::Parameter const& _param,
		  QDomNode const& _node,
		  QListViewItem * _parentItem, QListViewItem * _pre,
		  QObject * _parent, const char * _name);

  SimpleParameter(Miro::CFG::Parameter const& _param,
		  QDomNode const& _node,
		  QListView * _list, QListViewItem * _pre,
		  QObject * _parent, const char * _name);

  //----------------------------------------------------------------------------
  // public inherited methods
  //----------------------------------------------------------------------------

  virtual void setParameters();

  //----------------------------------------------------------------------------
  // public static methods
  //----------------------------------------------------------------------------

  static Type typeFromName(QString const& _name);

  //----------------------------------------------------------------------------
  // public constants
  //----------------------------------------------------------------------------

  static QString const XML_ATTRIBUTE_VALUE;

protected:
  Miro::CFG::Parameter param_;
};

#endif
