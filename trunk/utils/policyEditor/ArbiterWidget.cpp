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

#include "PolicyConfig.h"
#include "../params/Generator.h"

#include <qpainter.h>
#include <qpopupmenu.h>
#include <qapplication.h>
#include <qlistbox.h>
#include <qcursor.h>

#include <cmath>
#include <iostream>

extern QApplication* qApp;

ArbiterWidget::ArbiterWidget(QWidget* parent, const QString& name) :
  Super(parent, "arbiterwidget")
{
  setText(name);
  setBackgroundColor(darkGreen);

  setMouseTracking(true);    // enable mouse tracking for highlighting
}

PatternWidgetClass& 
ArbiterWidget::getPatternWidget() const
{
  return dynamic_cast<PatternWidgetClass&>(* parentWidget()); 
}

void 
ArbiterWidget::mousePressEvent(QMouseEvent* event) 
{
  // right button -> popup menu //
  if (event->button() == RightButton) {

    // popup menu //
    QPopupMenu menu;
    menuSetArbiter_ = new QPopupMenu(&menu);

    menu.insertItem("Set arbiter", menuSetArbiter_);

    // submenu: add all arbiter names //
    Generator::GroupMap::const_iterator first, last;
    PolicyConfigClass::instance()->description().getGroupedClasses("arbiter", first, last);
    for (; first != last; ++first) {
      if (first->second.isFinal()) {
        menuSetArbiter_->insertItem(first->second.name());
      }
    }
    connect(menuSetArbiter_, SIGNAL(activated(int)), this, SLOT(onSetArbiter(int)));

    // show popup menu //
    menu.exec(QCursor::pos());
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

void ArbiterWidget::onSetArbiter(int n)
{
  // set arbiter for the current pattern //
  getDocument().setArbiter(getPatternWidget().getPatternName(), menuSetArbiter_->text(n));
  setText(menuSetArbiter_->text(n));
}

