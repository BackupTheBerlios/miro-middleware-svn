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

#include "PatternWidget.h"

#include "BehaviourWidget.h"
#include "ArbiterWidget.h"

#include <qpainter.h>
#include <qpopupmenu.h>
#include <qdialog.h>
#include <qlabel.h>
#include <qtable.h>
#include <qheader.h>
#include <qlineedit.h>
#include <qapplication.h>
#include <qlistbox.h>
#include <qcursor.h>

#include <cmath>

extern QApplication* qApp;

PatternWidgetClass::PatternWidgetClass(QWidget* parent, const QString& name) :
  Super(parent, "patternwidget"), 
  patternName(name),
  arbiterWidget(NULL)
{
  setFrameStyle(QFrame::WinPanel | QFrame::Raised);
  setBackgroundColor(lightGray);

  setMouseTracking(true);    // enable mouse tracking for highlighting

  update();
}


PatternWidgetClass::~PatternWidgetClass()
{
  delete arbiterWidget;
  // TODO: delete labels in behaviourLabelList
}


PolicyViewClass& PatternWidgetClass::getView() const
{
  return (PolicyViewClass&) *(parentWidget()); 
}

void PatternWidgetClass::updatePos()
{
  int numBehaviours = getDocument().getNumBehaviours(patternName);
  setGeometry(getDocument().getX(patternName), 
	      getDocument().getY(patternName), 
	      100,
	      PATTERN_NAME_HEIGHT + 
	      BEHAVIOUR_NAME_HEIGHT*numBehaviours + 
	      ARBITER_NAME_HEIGHT +
	      2*frameWidth());
}


void PatternWidgetClass::update()
{
  // clear current data //
  delete arbiterWidget;

  std::list<BehaviourWidget*>::iterator iter = behaviourWidgetList.begin();
  while (iter != behaviourWidgetList.end()) {
    delete(*iter);                    // delete widget
    iter++;
  }
  behaviourWidgetList.clear();  


  // resize widget //
  int numBehaviours = getDocument().getNumBehaviours(patternName);
  setGeometry(getDocument().getX(patternName), 
	      getDocument().getY(patternName), 
	      100,
	      PATTERN_NAME_HEIGHT + 
	      BEHAVIOUR_NAME_HEIGHT*numBehaviours + 
	      ARBITER_NAME_HEIGHT +
	      2*frameWidth());

  // get internal rect dimensions (inside the frame) //
  QRect cr = contentsRect();


  //----------------//
  // behaviour list //
  //----------------//

   // get string list with behaviour names //
   QStringList list = getDocument().getBehaviourNames(patternName);

   // show all behaviour names //
   int i = 0;
   for (QStringList::Iterator it = list.begin(); it != list.end(); ++it ) {
     QString behName = (*it);

     // create new label //
     BehaviourWidget* behWidget = new BehaviourWidget(this, behName);
     behWidget->setGeometry(cr.x(), 
			    cr.y()+PATTERN_NAME_HEIGHT+i*BEHAVIOUR_NAME_HEIGHT,
			    cr.width(), BEHAVIOUR_NAME_HEIGHT);  
     behWidget->show();

     // append label to list //
     behaviourWidgetList.push_back(behWidget);

     i++;
   }


  //---------//
  // arbiter //
  //---------//

  arbiterWidget = new ArbiterWidget(this, 
				    getDocument().getArbiter(patternName));
  arbiterWidget->setGeometry(cr.x(), cr.y()+cr.height()-ARBITER_NAME_HEIGHT, 
			     cr.width(), ARBITER_NAME_HEIGHT);  
  arbiterWidget->show();

  repaint();
}


void PatternWidgetClass::paintEvent(QPaintEvent* event)
{
  // draw the frame //
  Super::paintEvent(event);

  QPainter p(this);
  
  //-------------------//
  // show pattern name //
  //-------------------//
  
  // get internal rect dimensions (inside the frame) //
  QRect cr = contentsRect();

  QBrush brush(red);

  QString patternText = patternName;   // default
  // if start pattern -> add "*" to name //
  if (getDocument().isStartPattern(patternName)) 
    patternText = QString("* ") + patternText + " *";

  p.fillRect(cr.x(), cr.y(), cr.width(), PATTERN_NAME_HEIGHT, brush);
  p.drawText(cr.x(), cr.y(), cr.width(), PATTERN_NAME_HEIGHT,
	     AlignHCenter | AlignVCenter, patternText);
  
  p.end();

  
  //------------------//
  // show transitions //
  //------------------//
  
  p.begin(parentWidget());     // draw in the parent widget !!
  p.setPen(darkGreen);
  
  // get list with transitions //
  TransitionList transitionList = getDocument().getTransitions(patternName);
  const int Zeilenabstand=10;
  int DrawX1, DrawX2, DrawY1, DrawY2,OldDrawX1, OldDrawX2, OldDrawY1, OldDrawY2,i;
  // show all transitions //
  OldDrawX1=-1;
  OldDrawX2=-1;
  OldDrawY1=-1;
  OldDrawY2=-1;
  
  QMap<QString, int> map;
  for (TransitionList::iterator it = transitionList.begin(); 
       it != transitionList.end(); ++it ) {
    QString message = it->getMessage();
    QString target  = it->getTarget();
    map[target] = map[target] + 1;
    
    int x2 = getDocument().getX(target);
    int y2 = getDocument().getY(target);

    if (x() < x2) { 
      DrawX1 = x()+width(); 
      DrawY1 = y();  
      DrawX2 = x2;  
      DrawY2 = y2; 
    }

    else { 
      DrawX1 = x();
      DrawY1 = y()+height(); 
      DrawX2 = x2+width();    
      DrawY2 = y2+PATTERN_NAME_HEIGHT +
	BEHAVIOUR_NAME_HEIGHT * getDocument().getNumBehaviours(target) +
	2*frameWidth(); 
    } 
    
    drawArrow(p, DrawX1, DrawY1, DrawX2, DrawY2, 10);
    i=map[target]-1;
    p.drawText( (DrawX1+DrawX2)/2, (DrawY1+DrawY2)/2+i*Zeilenabstand, message);
    
  }
}



void PatternWidgetClass::drawArrow(QPainter& p, int x1, int y1, 
				 int x2, int y2, int size)
{
  p.drawLine(x1, y1, x2, y2);

  float a=atan2(x2-x1, y2-y1);
  float a1=a-0.3;
  float a2=a+0.3;

  p.drawLine(x2, y2, int(x2-size*sin(a1)), int(y2-size*cos(a1)));
  p.drawLine(x2, y2, int(x2-size*sin(a2)), int(y2-size*cos(a2)));
}


void PatternWidgetClass::mousePressEvent(QMouseEvent* event) 
{
  // left button //
  if (event->button() == LeftButton) {

    // are we in the addTransitionMode ? //
    if (getView().isAddTransitionMode()) {
      getView().endAddTransition(this);
    }

    // else, save the click position for a possible move //
    else {
      picked_x = event->pos().x();
      picked_y = event->pos().y();  
    }
  }


  // right button -> popup menu //
  if (event->button() == RightButton) {

    // popup menu //
    QPopupMenu Menu;
    QPopupMenu menuAddBehaviour;

    Menu.insertItem("Start pattern",  this, SLOT(onSetStart()));
    Menu.insertSeparator();
    Menu.insertItem("Add behaviour",  &menuAddBehaviour);
    Menu.insertItem("Add transition", this, SLOT(onAddTransition()));
    Menu.insertSeparator();
    Menu.insertItem("Rename pattern", this, SLOT(onRenamePattern()));
    Menu.insertItem("Rename transition", this, SLOT(onRenameTransition()));
    Menu.insertItem("Delete pattern", this, SLOT(onDelete()));

    // submenu: add all behaviour names //
    for (unsigned int id=0; id<getDocument().DatabaseVector.size(); id++) 
    {
      if (!getDocument().hasBehaviour(patternName,
          getDocument().DatabaseVector[id].getName()))
      {
        menuAddBehaviour.insertItem(getDocument().DatabaseVector[id].getName(),id);
      }
    }
    connect(&menuAddBehaviour, SIGNAL(activated(int)), 
	    this, SLOT(onAddBehaviour(int)));

    // show popup menu //
    Menu.exec(QCursor::pos());
  }
}


void PatternWidgetClass::mouseReleaseEvent(QMouseEvent* /*event*/) 
{
}


void PatternWidgetClass::mouseMoveEvent(QMouseEvent* event)
{
  // if left key is pressed -> move widget //
  if (event->state() & LeftButton) {
    QPoint newPos = mapToParent(event->pos());
    move(newPos.x()-picked_x, newPos.y()-picked_y);
    getDocument().setX(patternName, newPos.x()-picked_x);
    getDocument().setY(patternName, newPos.y()-picked_y);
    getDocument().setModified(true);
    getView().repaint();
  }
}


void PatternWidgetClass::enterEvent(QEvent*)
{
  QPainter p(this);
  
  // get internal rect dimensions (inside the frame) //
  QRect cr = contentsRect();

  QString patternText = patternName;   // default
  // if start pattern -> add "*" to name //
  if (getDocument().isStartPattern(patternName)) 
    patternText = QString("* ") + patternText + " *";

  QBrush brush(yellow);
  p.fillRect(cr.x(), cr.y(), cr.width(), PATTERN_NAME_HEIGHT, brush);
  p.drawText(cr.x(), cr.y(), cr.width(), PATTERN_NAME_HEIGHT,
	     AlignHCenter | AlignVCenter, patternText);
  
  p.end();
}


void PatternWidgetClass::leaveEvent(QEvent*)
{
  QPainter p(this);
  
  // get internal rect dimensions (inside the frame) //
  QRect cr = contentsRect();

  QBrush brush(red);

  QString patternText = patternName;   // default
  // if start pattern -> add "*" to name //
  if (getDocument().isStartPattern(patternName)) 
    patternText = QString("* ") + patternText + " *";

  p.fillRect(cr.x(), cr.y(), cr.width(), PATTERN_NAME_HEIGHT, brush);
  p.drawText(cr.x(), cr.y(), cr.width(), PATTERN_NAME_HEIGHT,
	     AlignHCenter | AlignVCenter, patternText);
}




//------------------------------------------------



void PatternWidgetClass::onAddBehaviour(int n)
{
  // add a behaviour to the current pattern //
  getDocument().addBehaviour(patternName, n);
  getDocument().setModified(true);
  update();
  getView().update();
}


void PatternWidgetClass::onDelete()
{
  // remove the current pattern //
  getDocument().delPattern(patternName);
  getDocument().setModified(true);
  getView().update();
}


void PatternWidgetClass::onAddTransition()
{
  getView().startAddTransition(this);
  getDocument().setModified(true);
}


void PatternWidgetClass::onSetStart()
{
  // set pattern to start pattern //
  getDocument().setStartPattern(patternName);
  getDocument().setModified(true);
  getView().update();
}

void PatternWidgetClass::onRenamePattern()
{
  renamePattern(patternName);
  getView().update();
}

void PatternWidgetClass::onRenameTransition()
{
  renameTransition(patternName);
  getView().update();
}

//-------------------------------------------------
void PatternWidgetClass::renamePattern(QString oldName)
{
  QDialog dialog(this, "addPatternDialog", true);
  dialog.setCaption("Rename Pattern");

  // line edit for pattern name //
  QLineEdit   lineEdit(&dialog);
  lineEdit.setText(oldName);
  lineEdit.selectAll();
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
    if ((lineEdit.text()!=oldName)&&(lineEdit.text()!=""))
    {
      QString newPatternName=lineEdit.text();
      getDocument().renamePattern(oldName, newPatternName);
      patternName=newPatternName;
      getDocument().setModified(true);
      update();
    }
  }  
}

void PatternWidgetClass::renameTransition(QString nameOfPattern)
{
  QDialog dialog(this, "RenameTransitionDialog", true);
  dialog.setCaption("Rename Transition");
  /*table.horizontalHeader().setLabel(0,"Message",-1);
  table.horizontalHeader().setLabel(1,"Target",-1);*/
  TransitionList transitionList=getDocument().getTransitions(nameOfPattern);
  QTable table(transitionList.size(),2,&dialog);
  int i=0;
  for (TransitionList::iterator it = transitionList.begin(); 
       it != transitionList.end(); ++it,i++ ) 
  {
     table.setText(i,1,it->getTarget());
     table.setText(i,0,it->getMessage());
  }
  table.setGeometry(10,10,400,200);
  // cancel button //
  QPushButton cancelButton("Cancel", &dialog);
  cancelButton.setGeometry(40,210,60,25);
  connect(&cancelButton, SIGNAL(clicked()), &dialog, SLOT(reject()));

  // OK button //
  QPushButton okButton("OK", &dialog);
  okButton.setGeometry(130,210,60,25);  
  connect(&okButton,     SIGNAL(clicked()), &dialog, SLOT(accept()));

  // OK pressed in dialog ? //
  if (dialog.exec()) {
    getDocument().delTransitionsFrom(nameOfPattern);
    for (int j=0; table.text(j,0).isNull()==false;j++)
    {
      if (table.text(j,0)!="") 
      {
        getDocument().addTransition(nameOfPattern,table.text(j,0),table.text(j,1));
      }; 
    }
    getDocument().setModified(true);
    update();
  } 
} 
