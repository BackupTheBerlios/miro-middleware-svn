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

#include "BehaviourWidget.h"
#include "ParamDialog.h"

#include <qpopupmenu.h>
#include <qmap.h>
#include <qcursor.h>

BehaviourWidget::BehaviourWidget(QWidget* parent, const QString& name) :
  Super(parent, "behaviourwidget"), behaviourName(name)
{
  setText(name);
  setBackgroundColor(lightGray);

  setMouseTracking(true);    // enable mouse tracking for highlighting
}


PatternWidgetClass& BehaviourWidget::getPatternWidget() const
{
  return (PatternWidgetClass&) *(parentWidget()); 
}


void BehaviourWidget::enterEvent(QEvent*)
{
  setBackgroundColor(cyan);
  repaint();
}

void BehaviourWidget::leaveEvent(QEvent*)
{
  setBackgroundColor(lightGray);
  repaint();
}

void BehaviourWidget::mousePressEvent(QMouseEvent* event) 
{
  // left button //
  if (event->button() == LeftButton) {

    // save the click position for a possible move //
    picked_x = event->pos().x();
    picked_y = event->pos().y();  
  }

  // right button -> popup menu //
  if (event->button() == RightButton) {
    QString patternName = getPatternWidget().getPatternName();

     // popup menu //
    QPopupMenu Menu;

    Menu.insertItem("Up",                this, SLOT(onUp()),   0, 1);
    Menu.insertItem("Down",              this, SLOT(onDown()), 0, 2);
    Menu.insertSeparator();
    Menu.insertItem("Set parameters",    this, SLOT(onSetParameters()));
    Menu.insertSeparator();
    Menu.insertItem("Delete behaviour",  this, SLOT(onDelete()));

    // grey up and down entry if not usable //
    Menu.setItemEnabled(1, 
	      getDocument().hasPrevBehaviour(patternName, behaviourName));

    Menu.setItemEnabled(2, 
	      getDocument().hasNextBehaviour(patternName, behaviourName));

    // show popup menu //
    Menu.exec(QCursor::pos());
  }
}


void BehaviourWidget::mouseMoveEvent(QMouseEvent* event)
{
  // if left key is pressed (and held) -> move widget //
  if (event->state() & LeftButton) {
    //    QPoint newPos = mapToParent(event->pos());
    //    move(newPos.x()-picked_x, newPos.y()-picked_y);
  }

  // if left key is not pressed -> highlight selection //
  else {
  }
}


void BehaviourWidget::onSetParameters()
{
  QString patternName = getPatternWidget().getPatternName();

  // fill dialog with current parameters //
  QMap<QString, QString> paramMap = getDocument().getParameters(patternName, 
								behaviourName);
  ParamDialog paramDialog(this,behaviourName,getDocument().getPolicyConfig(), paramMap);

  // OK pressed ? //
  if (paramDialog.exec()) {
    QMap<QString, QString> paramMap = paramDialog.getParameters();
    getDocument().setParameters(patternName, behaviourName, paramMap);
    getDocument().setModified(true);
  }
}


void BehaviourWidget::onDelete()
{
  QString patternName = getPatternWidget().getPatternName();

  getDocument().delBehaviour(patternName, behaviourName);
  getDocument().setModified(true);
  getPatternWidget().update();             // update pattern
  //  getPatternWidget().getView().repaint();  // repaint transition arrows (TODO)
}


void BehaviourWidget::onUp()
{
  QString patternName = getPatternWidget().getPatternName();

  getDocument().behaviourUp(patternName, behaviourName);
  getDocument().setModified(true);
  getPatternWidget().update();             // update pattern
}

void BehaviourWidget::onDown()
{
  QString patternName = getPatternWidget().getPatternName();

  getDocument().behaviourDown(patternName, behaviourName);
  getDocument().setModified(true);
  getPatternWidget().update();             // update pattern
}
