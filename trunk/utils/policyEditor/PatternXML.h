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
#ifndef PatternXML_h
#define PatternXML_h

#include "utils/widgets/ItemXML.h"

#include "Transition.h"

//forward declarations
class PolicyXML;

class PatternXML : public ItemXML
{
  Q_OBJECT;
  
  typedef ItemXML Super;
public:
  //----------------------------------------------------------------------------  
  // public methods
  //----------------------------------------------------------------------------  

  PatternXML(QDomNode const& _node,
	     QListViewItem * _parentItem, QListViewItem * _pre,
	     QObject * _parent, const char * _name);

  PolicyXML * policy();
  bool startPattern();
  void startPattern(bool _flag);
  bool hasTransition(QString const& _msg) const;
  void addTransition(QString const& _msg, QString const& _target);
  bool tryAddTransition(QString const& _msg, PatternXML * _target);
  void removeTransitionTo(QString const& _target);
  void renameTransitionTo(QString const& _oldTarget, QString const& _newTarget);

  // transition handling
  virtual TransitionVector transitions();
  virtual QStringVector unboundTransitionMessages();
  virtual QStringVector boundTransitionMessages() const;

  int x();
  void setX(int _x);
  int y();
  void setY(int _y);

  //----------------------------------------------------------------------------  
  // inherited public methods
  //----------------------------------------------------------------------------  

  virtual void contextMenu(QPopupMenu& _menu);
  virtual void rename(QString const& _name);
  virtual bool deletable();
  virtual void deleteItem();
  virtual QStringVector transitionMessages() const;

  virtual void removeChild(QObject * _obj);

  //----------------------------------------------------------------------------  
  // public constants
  //----------------------------------------------------------------------------  
  static const QString XML_ATTRIBUTE_START;
  static const QString XML_ATTRIBUTE_X;
  static const QString XML_ATTRIBUTE_Y;

protected slots:
  void setStartPattern();
  void addTransition(int _id);

protected:
  //----------------------------------------------------------------------------  
  // protected methods
  //----------------------------------------------------------------------------

  //----------------------------------------------------------------------------  
  // protected members
  //----------------------------------------------------------------------------
  QPopupMenu * menuAddTransition_;
};

#endif
