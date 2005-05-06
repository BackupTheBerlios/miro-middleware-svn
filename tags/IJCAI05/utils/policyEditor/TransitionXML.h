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
#ifndef TransitionXML_h
#define TransitionXML_h

#include "utils/widgets/ItemXML.h"

// forward declarations
class PatternXML;

class TransitionXML : public ItemXML
{
  Q_OBJECT;
  
  typedef ItemXML Super;

public:
  //----------------------------------------------------------------------------  
  // public methods
  //----------------------------------------------------------------------------

  TransitionXML(QDomNode& _node,
		QListViewItem * _parentItem, QListViewItem * _pre,
		QObject * _parent, const char * _name);

  PatternXML * pattern() const;

  QString target();
  void setTarget(QString const& _target);
  QString message();

  //----------------------------------------------------------------------------  
  // inherited public methods
  //----------------------------------------------------------------------------

  void contextMenu(QPopupMenu& _menu);

  //----------------------------------------------------------------------------  
  // public constants
  //----------------------------------------------------------------------------  
  static const QString XML_TAG;
  static const QString XML_ATTRIBUTE_MSG;
  static const QString XML_ATTRIBUTE_TARGET;

public slots:
  void setMessage();
  void setTarget(int _id);

protected:
  //----------------------------------------------------------------------------  
  // protected members
  //----------------------------------------------------------------------------
  virtual void updateView();
  
  //----------------------------------------------------------------------------  
  // protected members
  //----------------------------------------------------------------------------
  QPopupMenu * menuSetTarget_;

private:
  //----------------------------------------------------------------------------  
  // hidden members
  //----------------------------------------------------------------------------
  TransitionXML(TransitionXML const&);
};

#endif
