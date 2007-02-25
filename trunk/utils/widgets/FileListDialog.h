// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 1999-2005
// Department of Neuroinformatics, University of Ulm, Germany
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// $Id$
//
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
