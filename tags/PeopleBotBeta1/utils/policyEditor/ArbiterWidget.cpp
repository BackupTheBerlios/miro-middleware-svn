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

#include "ArbiterWidget.h"

#include <qpainter.h>
#include <qpopupmenu.h>
#include <qapplication.h>
#include <qlistbox.h>
#include <qcursor.h>

#include <cmath>
#include <iostream>

extern QApplication* qApp;

ArbiterWidget::ArbiterWidget(QWidget* parent, const QString& name) :
  Super(parent, "behaviourwidget"), arbiterName(name)
{
  setText(name);
  setBackgroundColor(darkGreen);

  setMouseTracking(true);    // enable mouse tracking for highlighting
}


PatternWidgetClass& ArbiterWidget::getPatternWidget() const
{
  return (PatternWidgetClass&) *(parentWidget()); 
}


void ArbiterWidget::mousePressEvent(QMouseEvent* event) 
{
  // left button //
  if (event->button() == LeftButton) {
    std::cout << "ArbiterWidget: left button pressed" << std::endl;
  }


  // right button -> popup menu //
  if (event->button() == RightButton) {

    // popup menu //
    QPopupMenu Menu;
    QPopupMenu menuSetArbiter;

    Menu.insertItem("Set arbiter",    &menuSetArbiter);
    Menu.insertSeparator();
    Menu.insertItem("Delete arbiter",         this, SLOT(onDelete()));

    // submenu: add all arbiter names //
    for (unsigned int id=0; id<getDocument().arbiterVector.size(); id++) {
      menuSetArbiter.insertItem(getDocument().arbiterVector[id], id);
    }
    connect(&menuSetArbiter, SIGNAL(activated(int)), 
 	    this, SLOT(onSetArbiter(int)));

    // show popup menu //
    Menu.exec(QCursor::pos());
  }
}



void ArbiterWidget::enterEvent(QEvent*)
{
  setBackgroundColor(green);
  repaint();
}

void ArbiterWidget::leaveEvent(QEvent*)
{
  setBackgroundColor(darkGreen);
  repaint();
}


void ArbiterWidget::mouseMoveEvent(QMouseEvent* event)
{
  // if left key is pressed -> move widget //
  if (event->state() & LeftButton) {
    //     QPoint newPos = mapToParent(event->pos());
    //     move(newPos.x()-picked_x, newPos.y()-picked_y);
    //     getDocument().setX(name, newPos.x());
    //     getDocument().setY(name, newPos.y());
    //     getView().repaint();
  }

  // if left key is not pressed -> highlight selection //
  else {
  }
}


void ArbiterWidget::onSetArbiter(int n)
{
  // set arbiter for the current pattern //
  getDocument().setArbiter(getPatternWidget().getPatternName(), n);
  getDocument().setModified(true);
  getPatternWidget().update();             // update pattern
}



void ArbiterWidget::onDelete()
{
  std::cout << "ArbiterWidget::onDelete: not implemented!" << std::endl;
}
