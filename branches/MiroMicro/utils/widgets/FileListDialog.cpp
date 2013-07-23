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
#include "FileListDialog.h"

#include <qgroupbox.h>
#include <qvgroupbox.h>
#include <qhgroupbox.h>
#include <qhbox.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qmessagebox.h>
#include <qlistbox.h>
#include <qstringlist.h>
#include <qfiledialog.h>

FileListDialog::FileListDialog(QWidget* parent,
			       QString const& _dialogTitle,
			       QString const& _listTitle,
			       char const * _filters[]) :
  QDialog(parent, "FileListDialog", TRUE),       // TRUE = modal dialog
  list_(NULL),
  delButton_(NULL),
  fileDialog_(NULL),
  modified_(false)
{
  resize(300, 200);
  setCaption(_dialogTitle);

  QVBoxLayout * topBox = new QVBoxLayout(this, 0, -1, "boxLayout");

  QVGroupBox * fileBox = new QVGroupBox(this, "fileBox");
  list_ = new QListBox(fileBox, "list");

  QHBox * fileButtonsBox = new QHBox(fileBox, "fileButtons");
  QPushButton * addButton = new QPushButton("Add...", fileButtonsBox);
  delButton_ = new QPushButton("Remove", fileButtonsBox);

  fileDialog_ = new QFileDialog(this, "config file dialog", TRUE);

  topBox->addSpacing(10);
  topBox->addWidget(fileBox);
  fileBox->setTitle(_listTitle);

  topBox->addSpacing(10);
  QHBoxLayout * dialogButtonsBox = new QHBoxLayout(topBox, -1, "hBoxLayout");
  QSpacerItem * dBSpace = new QSpacerItem(0, 0);
  QPushButton * okButton = new QPushButton("OK", this);
  QPushButton * cancelButton = new QPushButton("Cancel", this);
  
  topBox->addSpacing(5);
  dialogButtonsBox->addItem(dBSpace);
  dialogButtonsBox->addWidget(okButton);
  dialogButtonsBox->addSpacing(5);
  dialogButtonsBox->addWidget(cancelButton);
  dialogButtonsBox->addSpacing(5);
  
  okButton->setDefault(true);

  static const char * filters[3] = { "all files (*)", NULL };

  fileDialog_->setCaption("File open dialog");
  fileDialog_->setFilters((_filters == NULL)? filters : _filters);

  // connect the dialogs functionality  
  connect(okButton,     SIGNAL(clicked()), SLOT(accept()));
  connect(cancelButton, SIGNAL(clicked()), SLOT(reject()));
  connect(addButton,    SIGNAL(clicked()), SLOT(add()));
  connect(delButton_,   SIGNAL(clicked()), SLOT(del()));

  selectListItem();
}

FileListDialog::~FileListDialog()
{
}

void 
FileListDialog::init(const QStringList& _list)
{
  // fill list with current parameters
  modified_ = false;
  list_->clear();
  list_->insertStringList(_list);
 
  selectListItem();
}

void
FileListDialog::selectListItem()
{
  if (list_->count() != 0) {
    int item = list_->currentItem();
    if (item == -1)
      item = 0;
    list_->setSelected(list_->item(item), true);
  }

  delButton_->setEnabled(list_->currentItem() != -1);
}

QStringList
FileListDialog::result() 
{
  QStringList list;

  for (unsigned int i = 0; i < list_->count(); ++i)
    list.append(list_->text(i));

  return list;
}

void 
FileListDialog::add()
{
  if (fileDialog_->exec() == QDialog::Accepted ) {

    if (list_->findItem(fileDialog_->selectedFile()) == NULL) {
      list_->insertItem(fileDialog_->selectedFile());
      modified_ = true;
    }
    else {
      QMessageBox::warning(this, "Duplicated file", "Selected file is already part of the file list.");
    }
  }

  selectListItem();
}

void 
FileListDialog::del()
{
  int item; 
  if ((item = list_->currentItem()) != -1) {
    list_->removeItem(item);
    modified_ = true;
  }
 
  selectListItem();
}
