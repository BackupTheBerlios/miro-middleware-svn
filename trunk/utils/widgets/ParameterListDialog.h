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
#ifndef ParameterListDialog_h
#define ParameterListDialog_h

#include "DialogXML.h"
#include "ParameterList.h"

#include "params/Parameter.h"

// forward declarations
class QListView;
class QListViewItem;
class QPushButton;
class QString;
class QStringList;
class QParameterDialog;

class ConfigFile;

/**
 * This class shows a dialog for the behaviour parameters.
 */
class ParameterListDialog : public DialogXML
{
  Q_OBJECT;

  typedef DialogXML Super;
public:
  ParameterListDialog(ParameterList::Type _type,
		      Miro::CFG::Parameter const& _parameter,
		      QDomNode const& _parentNode, QDomNode const& _node,
		      ItemXML * _parentItem, ItemXML * _item,
		      QWidget * _parent, const char * _name = NULL);

  void setXML();

protected slots:
  void up();
  void down();
  void add();
  void del();
  void edit();

  void slotDoubleClick(QListViewItem *);
  void contextMenu(QListViewItem*,const QPoint&,int);

protected:
  void selectListItem();

  ConfigFile * config_;

  ParameterList::Type type_;
  Miro::CFG::Parameter parameter_;
  bool nestedCompound_;
  Miro::CFG::Type const * nestedType_;
  unsigned int index_;

  QListView * list_;
  QPushButton * delButton_;  
  QPushButton * editButton_;  

  QObject collector_;

  QDomDocument tmpDocument_;
  QDomNode tmpParentNode_;
  QDomNode tmpNode_;
};

#endif
