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
#ifndef ParameterSingleton_h
#define ParameterSingleton_h

#include "utils/widgets/CompoundParameter.h"

// forward declarations
class ConfigFile;
class QListViewItem;

class ParameterSingleton : public CompoundParameter
{
  Q_OBJECT
  
  typedef CompoundParameter Super;
public:
  //----------------------------------------------------------------------------
  // public methods
  //----------------------------------------------------------------------------
  ParameterSingleton(QListViewItem * _listViewItem, QListViewItem * _pre,
		     QDomNode const& _node,
		     QObject * _parent, const char * _name);
};

#endif
