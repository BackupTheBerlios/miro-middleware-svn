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

#include "ConfigDialog.h"

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

ConfigDialog::ConfigDialog(QWidget* parent) :
  QDialog(parent, "ConfigDialog", TRUE),       // TRUE = modal dialog
  list_(NULL),
  delButton_(NULL),
  fileDialog_(NULL),
  modified_(false)
{
  resize(300, 200);
  setCaption("Config dialog");

  QVBoxLayout * topBox = new QVBoxLayout(this, 0, -1, "boxLayout");

  QVGroupBox * fileBox = new QVGroupBox(this, "fileBox");
  list_ = new QListBox(fileBox, "list");

  QHBox * fileButtonsBox = new QHBox(fileBox, "fileButtons");
  QPushButton * addButton = new QPushButton("Add...", fileButtonsBox);
  delButton_ = new QPushButton("Remove", fileButtonsBox);

  fileDialog_ = new QFileDialog(this, "config file dialog", TRUE);

  topBox->addSpacing(10);
  topBox->addWidget(fileBox);
  fileBox->setTitle("Behaviour description files");

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

  static const char * filters[3] = { "all files (*)", "config files (*.xml)", NULL };

  fileDialog_->setCaption("Config file dialog");
  fileDialog_->setFilters(filters);

  // connect the dialogs functionality  
  connect(okButton,     SIGNAL(clicked()), SLOT(accept()));
  connect(cancelButton, SIGNAL(clicked()), SLOT(reject()));
  connect(addButton,    SIGNAL(clicked()), SLOT(add()));
  connect(delButton_,   SIGNAL(clicked()), SLOT(del()));

  selectListItem();
}

ConfigDialog::~ConfigDialog()
{
}

void 
ConfigDialog::init(const QStringList& _list)
{
  // fill list with current parameters
  modified_ = false;
  list_->clear();
  list_->insertStringList(_list);
 
  selectListItem();
}

void
ConfigDialog::selectListItem()
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
ConfigDialog::result() 
{
  QStringList list;

  for (unsigned int i = 0; i < list_->count(); ++i)
    list.append(list_->text(i));

  return list;
}

void 
ConfigDialog::add()
{
  if (fileDialog_->exec() == QDialog::Accepted ) {

    if (list_->findItem(fileDialog_->selectedFile()) == NULL) {
      list_->insertItem(fileDialog_->selectedFile());
      modified_ = true;
    }
    else {
      QMessageBox::warning(this, "Duplicated file", "Selected file is already part of the configuration.");
    }
  }

  selectListItem();
}

void 
ConfigDialog::del()
{
  int item; 
  if ((item = list_->currentItem()) != -1) {
    list_->removeItem(item);
    modified_ = true;
  }
 
  selectListItem();
}
