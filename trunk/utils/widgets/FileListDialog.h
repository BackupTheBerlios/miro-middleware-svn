// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2002, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef FileListDialog_h
#define FileListDialog_h

#include <qdialog.h>

// forward declarations
class QListBox;
class QPushButton;
class QString;
class QStringList;
class QFileDialog;

/**
 * This class shows a dialog for the behaviour parameters.
 */
class FileListDialog : public QDialog
{
  Q_OBJECT

public:
  FileListDialog(QWidget* parent,
		 QString const& _dialogTitle = QString("File list dialog"),
		 QString const& _listTitle = QString("Files"),
		 char const * _filters[] = NULL);
  ~FileListDialog();

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
FileListDialog::modified() {
  return modified_;
}

#endif
