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

#include "PolicyView.h"
#include "PatternWidget.h"

#include <qpainter.h>
#include <qpopupmenu.h>
#include <qdialog.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qmainwindow.h>
#include <qstatusbar.h>
#include <qcursor.h>

PolicyViewClass::PolicyViewClass(QWidget *parent, 
				 PolicyDocumentClass& _document) :
  QWidget(parent, "view"), 
  document(_document)
{
  this->parent=parent;
  scrollBar=new QScrollBar(-1000,1000,1,20,0,QScrollBar::Horizontal,this,"Scrollbar");
  scrollBar->setGeometry(0, parent->height()-70,parent->width()-20,20);
  connect (scrollBar, SIGNAL(valueChanged(int)),this,
	   SLOT(getHorizontalScrollValue(int)));
    
  verticalScrollBar= new QScrollBar(-1000,1000,1,20,0,QScrollBar::Vertical,this,"VerticalScrollbar");
  verticalScrollBar->setGeometry(parent->width()-20,20,
				 20,parent->height()-90);
  connect (verticalScrollBar, SIGNAL(valueChanged(int)),this,
	   SLOT(getVerticalScrollValue(int)));
 
  setBackgroundColor(white);
  addTransitionMode = false;

  // update the view //
  update(); 
}


void PolicyViewClass::resizeEvent(QResizeEvent* )
{
  scrollBar->setGeometry(0, parent->height()-70,parent->width()-20,20);
  verticalScrollBar->setGeometry(parent->width()-20,20,
				 20,parent->height()-90);
  getDocument().setWindowSize(parent->width(),parent->height());
  update();
}


void PolicyViewClass::mousePressEvent(QMouseEvent* event) 
{
  // right button -> popup menu //
  if (event->button() == RightButton) {
    picked_x = event->pos().x();               // save this for onAddPattern
    picked_y = event->pos().y();
    // popup menu //
    QPopupMenu Menu;
    Menu.insertItem("Add pattern", this, SLOT(onAddPattern()));
    Menu.exec(QCursor::pos());
  }
}


void PolicyViewClass::mouseReleaseEvent(QMouseEvent* /*event*/) 
{
}


void PolicyViewClass::mouseMoveEvent(QMouseEvent* event)
{
  if (isAddTransitionMode()) {
    QPainter p;
    p.begin(this);
    p.drawLine(pickedPatternPtr->pos(), event->pos());
    p.end();
  }
}

void PolicyViewClass::getHorizontalScrollValue(int value)
{
  getDocument().setxModificator(value);
  setBackgroundColor(white);
  std::list<PatternWidgetClass*>::iterator iter = PatternWidgetList.begin();
  while (iter != PatternWidgetList.end()) {
    (*iter)->updatePos();                    
    iter++;
  }
  QString message;
  message.sprintf("Value of horizontal Scrollbar: %d",value);
  ((QMainWindow*) parent)->statusBar()->message(message, 3000);
}  

void PolicyViewClass::getVerticalScrollValue(int value)
{
  getDocument().setyModificator(value);
  setBackgroundColor(white);
  std::list<PatternWidgetClass*>::iterator iter = PatternWidgetList.begin();
  while (iter != PatternWidgetList.end()) {
    (*iter)->updatePos();                    
    iter++;
  }
  QString message;
  message.sprintf("Value of vertical Scrollbar: %d",value);
  ((QMainWindow*) parent)->statusBar()->message(message, 3000);
}  

void PolicyViewClass::setVerticalScrollValue(int value)
{
  verticalScrollBar->setValue(value);
  getVerticalScrollValue(value);
}

void PolicyViewClass::setHorizontalScrollValue(int value)
{
  scrollBar->setValue(value);
  getHorizontalScrollValue(value);
}

void PolicyViewClass::prepareFileClosing()
{
  std::list<PatternWidgetClass*>::iterator iter = PatternWidgetList.begin();
  while (iter != PatternWidgetList.end()) {
    delete(*iter);                    // delete widget
    iter++;
  }
  PatternWidgetList.clear();
}

void PolicyViewClass::update()
{
  //----------------------------------------------//
  // delete all widgets and clear the widget list //
  //----------------------------------------------//

  std::list<PatternWidgetClass*>::iterator iter = PatternWidgetList.begin();
  while (iter != PatternWidgetList.end()) {
    delete(*iter);                    // delete widget
    iter++;
  }

  PatternWidgetList.clear();

  //-------------------------//
  // rebuild the widget list //
  //-------------------------//

  QStringList list = document.getPatternNames();

  QStringList::Iterator patternIter = list.begin();
  while (patternIter !=list.end()) {
    // generate pattern widget //
    QString patternName = *patternIter;
    PatternWidgetClass* patternWidget = new PatternWidgetClass(this, patternName);
    patternWidget->show();             // this is important !

    // append widget to list //
    PatternWidgetList.push_back(patternWidget);

    patternIter++;
  }

  repaint();
}

void PolicyViewClass::paintEvent(QPaintEvent* )
{
  //  cout << "PolicyViewClass::paintEvent" << endl;

  // repaint all pattern widgets //
  // (TODO: This is needed because the transition arrows are no widgets 
  // and so they are not repainted automatically.)
  std::list<PatternWidgetClass*>::iterator iter = PatternWidgetList.begin();
  while (iter != PatternWidgetList.end()) {
    (*iter)->repaint();
    iter++;
  }
}


void PolicyViewClass::onAddPattern()
{
  QDialog dialog(this, "addPatternDialog", true);
  dialog.setCaption("Add Pattern");

  // line edit for pattern name //
  QLineEdit   lineEdit(&dialog);
  lineEdit.setGeometry(120,20,100,20);  
  lineEdit.setFocus();
  connect(&lineEdit, SIGNAL(returnPressed()), &dialog, SLOT(accept()));

  // label //
  QLabel      label(&lineEdit, "&Pattern Name:", &dialog);
  label.setGeometry(10,20,100,20);  

  // cancel button //
  QPushButton cancelButton("Cancel", &dialog);
  cancelButton.setGeometry(40,80,60,25);
  connect(&cancelButton, SIGNAL(clicked()), &dialog, SLOT(reject()));

  // OK button //
  QPushButton okButton("OK", &dialog);
  okButton.setGeometry(130,80,60,25);  
  connect(&okButton,     SIGNAL(clicked()), &dialog, SLOT(accept()));

  // OK pressed in dialog ? //
  if (dialog.exec()) {
    document.addPattern(lineEdit.text(), picked_x, picked_y);
    document.setModified(true);
    update();
  }  
}


void PolicyViewClass::startAddTransition(PatternWidgetClass* patternWidget)
{
  pickedPatternPtr = patternWidget;
  pickedPattern    = patternWidget->getPatternName();
  addTransitionMode = true;
  setMouseTracking(true);
}



// this is called from the clicked PatternWidget when in signal mode //
void PolicyViewClass::endAddTransition(PatternWidgetClass* patternWidget)
{
  // if addTransition mode is on, finish it and add the signal //
  if (isAddTransitionMode()) {
    setMouseTracking(false);
    addTransitionMode = false;

    // dialog //
    QDialog dialog(this, "addTransitionDialog", true);
    dialog.setCaption("Add Transition");
    // line edit for pattern name //
    QLineEdit   lineEdit(&dialog);
    lineEdit.setGeometry(120,20,100,20);  
    lineEdit.setFocus();
    connect(&lineEdit, SIGNAL(returnPressed()), &dialog, SLOT(accept()));
    // label //
    QLabel      label(&lineEdit, "&Transition Name:", &dialog);
    label.setGeometry(10,20,100,20);  
    // cancel button //
    QPushButton cancelButton("Cancel", &dialog);
    cancelButton.setGeometry(40,80,60,25);
    connect(&cancelButton, SIGNAL(clicked()), &dialog, SLOT(reject()));
    // OK button //
    QPushButton okButton("OK", &dialog);
    okButton.setGeometry(130,80,60,25);  
    connect(&okButton,     SIGNAL(clicked()), &dialog, SLOT(accept()));
    
    // OK pressed in dialog ? //
    if (dialog.exec()) {
      QString message = lineEdit.text();
      QString target  = patternWidget->getPatternName();

      // TODO add signal //
      getDocument().addTransition(pickedPattern, message, target);
      getDocument().setModified(true);
      update();
    }
  }
}
