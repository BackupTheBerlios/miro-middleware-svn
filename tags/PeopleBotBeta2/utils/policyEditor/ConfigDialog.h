// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef ConfigDialog_h
#define ConfigDialog_h

#include <qdialog.h>

// forward declarations
class QListBox;
class QPushButton;
class QStringList;
class QFileDialog;

/**
 * This class shows a dialog for the behaviour parameters.
 */
class ConfigDialog : public QDialog
{
  Q_OBJECT

public:
  ConfigDialog(QWidget* parent);
  ~ConfigDialog();

  bool modified();

  void init(const QStringList& _list);
  QStringList result();

public slots:
  void add();
  void del();

protected:
  void selectListItem();

  QListBox * list_;
  QPushButton * delButton_;  
  QFileDialog * fileDialog_;

  bool modified_;
};

inline
bool
ConfigDialog::modified() {
  return modified_;
}

#endif
