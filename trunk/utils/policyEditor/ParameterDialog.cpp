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

#include "ParameterDialog.h"
#include "PolicyConfig.h"

#include "../params/Class.h"
#include "../params/Generator.h"

#include <qgroupbox.h>
#include <qvgroupbox.h>
#include <qhgroupbox.h>
#include <qlayout.h>
#include <qhbox.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qtooltip.h>
#include <qmessagebox.h>

#include <climits>

MyIntValidator::MyIntValidator(QWidget * parent, const char *name) :
  Super(parent, name),
  accepted_(true)
{}

MyIntValidator::MyIntValidator(int bottom, int top,
			       QWidget * parent, const char *name) :
  Super(bottom, top, parent, name),
  accepted_(true)
{}

QValidator::State 
MyIntValidator::validate( QString & input, int & pos) const
{
  QValidator::State s = Super::validate(input, pos);
  bool accept = input.isEmpty() || (s == Acceptable);
  if (accept != accepted_) {
    accepted_ = accept;
    emit const_cast<MyIntValidator *>(this)->acceptable(accept);
  }
  return s;
}

MyDoubleValidator::MyDoubleValidator(QWidget * parent, const char *name) :
  Super(parent, name),
  accepted_(true)
{}

MyDoubleValidator::MyDoubleValidator(double bottom, double top, int decimals,
				     QWidget * parent, const char *name) :
  Super(bottom, top, decimals, parent, name),
  accepted_(true)
{}

QValidator::State 
MyDoubleValidator::validate( QString & input, int & pos) const
{
  QValidator::State s = Super::validate(input, pos);
  bool accept = input.isEmpty() || (s == Acceptable);
  if (accept != accepted_) {
    accepted_ = accept;
    emit const_cast<MyDoubleValidator *>(this)->acceptable(accept);
  }

  return s;
}

MyBoolValidator::MyBoolValidator(QWidget * parent, const char *name) :
  Super(parent, name),
  accepted_(true)
{}

QValidator::State 
MyBoolValidator::validate( QString & input, int &) const
{
  QValidator::State s = Invalid;

  if (input == "true" ||
      input == "false")
    s = Acceptable;
  else if (input.length() <= 5)
    s = Intermediate;
 
  bool accept = (s == Acceptable);
  if (accept != accepted_) {
    accepted_ = accept;
    emit const_cast<MyBoolValidator *>(this)->acceptable(accept);
  }

  return s;
}


ParameterDialog::ParameterDialog(const QString& _name, 
				 const QMap<QString, QString>& _stringMap,
				 QWidget* _parent) :
  QDialog(_parent, "ParamDialog", TRUE),
  config_(PolicyConfigClass::instance()),
  class_(config_->description().getClass(_name)),
  modified_(false),
  accept_(0)
{
  if (class_ == NULL) {
    throw QString("Parameter description for " + 
		  _name +
		  " not found.\nCheck whether the relevant description file is loaded.");
  }
  params_ = class_->parameterSet();

  const Class * parent = class_;
  while (!parent->parent().isEmpty()) {
    const Class * tmp = config_->description().getClass(parent->parent());
    if (tmp == NULL) {
      QMessageBox::warning(this, 
			   "Error constructing parameter dialog:",
			   QString("Parameter description for " + 
				   parent->parent() +
				   " not found.\nCheck whether the relevant description file is loaded."));
      break;
    }
    parent = tmp;
    params_.insert(parent->parameterSet().begin(), parent->parameterSet().end());
  }

  setCaption(_name);

  QVBoxLayout * topBox = new QVBoxLayout(this, 0, -1, "boxLayout");

  QHGroupBox * groupBox = new QHGroupBox(this, "groupbox");
  QWidget * frame = new QFrame(groupBox);
  QGridLayout * gridLayout = new QGridLayout(frame, params_.size(), 3, 2, 5, "gridLayout"); 

  QHBoxLayout * dialogButtonsBox = new QHBoxLayout(NULL, 0, -1, "hBoxLayout");
  QSpacerItem * dBSpace = new QSpacerItem(0, 0);
  QPushButton * okButton = new QPushButton("OK", this);
  QPushButton * cancelButton = new QPushButton("Cancel", this);
  
  // add parameter structs:
  QMap<QString, QString>::ConstIterator iter;
  Class::ParameterSet::const_iterator first, last = params_.end();
  unsigned long i = 0;
  for (first = params_.begin(); first != last; ++first, ++i) {

    // name
    QLabel * name = new QLabel(frame);
    QString n = first->name_;
    n[0] = n[0].upper();
    name->setText(n);
    gridLayout->addWidget(name, i, 0);

    // value edit field
    QLineEdit * value = new QLineEdit(frame, first->name_);
    iter = _stringMap.find(n);
    if (iter != _stringMap.end()) {
      value->setText(iter.data());
      value->setEdited(false);
    }
    if (first->default_)
      QToolTip::add(value, QString("default: ") + first->default_);
    lineEdits_.push_back(value);

    // type safe editing
    if (first->type_ == "std::string") {

    }
    if (first->type_ == "char") {
      value->setMaxLength(1);
    }
    else if (first->type_ == "bool") {
      MyBoolValidator *  v = new MyBoolValidator(frame);
      value->setValidator(v);
      connect(v, SIGNAL(acceptable(bool)), this, SLOT(accept(bool)));
    }
    else if (first->type_ == "int") {
      MyIntValidator *  v = new MyIntValidator(frame);
      v->setRange(INT_MIN + 1, INT_MAX - 1);
      value->setValidator(v);
      connect(v, SIGNAL(acceptable(bool)), this, SLOT(accept(bool)));
    }
    else if (first->type_ == "unsigned int") {
      MyIntValidator * v = new MyIntValidator(frame);
      v->setRange(0, INT_MAX - 1);
      value->setValidator(v);
      connect(v, SIGNAL(acceptable(bool)), this, SLOT(accept(bool)));
    }
    else if (first->type_ == "short") {
      MyIntValidator * v = new MyIntValidator(frame);
      v->setRange(SHRT_MIN, SHRT_MAX);
      value->setValidator(v);
      connect(v, SIGNAL(acceptable(bool)), this, SLOT(accept(bool)));
    }
    else if (first->type_ == "unsigned short") {
      MyIntValidator * v = new MyIntValidator(frame);
      v->setRange(0, USHRT_MAX);
      value->setValidator(v);
      connect(v, SIGNAL(acceptable(bool)), this, SLOT(accept(bool)));
    }
    else if (first->type_ == "long") {
      MyIntValidator * v = new MyIntValidator(frame);
      v->setRange(LONG_MIN + 1, LONG_MAX - 1);
      value->setValidator(v);
      connect(v, SIGNAL(acceptable(bool)), this, SLOT(accept(bool)));
    }
    else if (first->type_ == "unsigned long") {
      MyIntValidator * v = new MyIntValidator(frame);
      v->setRange(0, LONG_MAX - 1);
      value->setValidator(v);
      connect(v, SIGNAL(acceptable(bool)), this, SLOT(accept(bool)));
    }
    else if (first->type_ == "double") {
      MyDoubleValidator * v = new MyDoubleValidator(frame);
      value->setValidator(v);
      connect(v, SIGNAL(acceptable(bool)), this, SLOT(accept(bool)));
    }
    else if (first->type_ == "angle") {
      MyDoubleValidator * v = new MyDoubleValidator(frame);
      value->setValidator(v);
      connect(v, SIGNAL(acceptable(bool)), this, SLOT(accept(bool)));
    }
    else if (first->type_ == "Angle") {
      MyDoubleValidator * v = new MyDoubleValidator(frame);
      v->setRange(-180., 180.);
      value->setValidator(v);
      connect(v, SIGNAL(acceptable(bool)), this, SLOT(accept(bool)));
    }
    else if (first->type_ == "ACE_Time_Value") {
      MyDoubleValidator * v = new MyDoubleValidator(frame);
      v->setRange(0, ULONG_MAX, 6);
      value->setValidator(v);
      connect(v, SIGNAL(acceptable(bool)), this, SLOT(accept(bool)));
    }
    gridLayout->addWidget(value, i, 1);

    // measure
    QLabel * measure = new QLabel(frame);
    if (!first->measure_.isEmpty()) {
      measure->setText(first->measure_);
      QToolTip::add(measure, (first->type_ != "angle")? first->type_ : QString("double"));
    }
    else
      measure->setText(first->type_);
    gridLayout->addWidget(measure, i, 2);
  }

  topBox->addSpacing(10);
  topBox->addWidget(groupBox);
  topBox->addSpacing(10);
  topBox->addLayout(dialogButtonsBox);
  topBox->addSpacing(5);
  dialogButtonsBox->addItem(dBSpace);
  dialogButtonsBox->addWidget(okButton);
  dialogButtonsBox->addSpacing(5);
  dialogButtonsBox->addWidget(cancelButton);
  dialogButtonsBox->addSpacing(5);
  
  okButton->setDefault(true);

  groupBox->sizeHint(); // we need this to get the right size (hutz)
  groupBox->setTitle("Parameters");

  // connect the dialogs functionality  
  connect(this,         SIGNAL(okay(bool)), okButton, SLOT(setEnabled(bool)));
  connect(okButton,     SIGNAL(clicked()), SLOT(accept()));
  connect(cancelButton, SIGNAL(clicked()), SLOT(reject()));
}

QMap<QString, QString>
ParameterDialog::result() const
{
  QMap<QString, QString> stringMap;

  LineEditVector::const_iterator first, last = lineEdits_.end();
  for (first = lineEdits_.begin(); first != last; ++first) {
    QString key   = (*first)->name();
    key[0] = key[0].upper();
    QString value = (*first)->text();

    if (!value.isEmpty() && !key.isEmpty()) {
       stringMap.insert(key, value);
     }
  }
  return stringMap;
}

void
ParameterDialog::accept(bool _accept)
{
  accept_ += (_accept)? -1 : 1;

  assert(accept_ >= 0);

  bool a = accept_ == 0;
  emit okay(a);
}
