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
#include "ParameterDialog.h"

#include <qpopupmenu.h>
#include <qmap.h>
#include <qcursor.h>
#include <qstring.h>

BehaviourWidget::BehaviourWidget(QWidget* parent, const QString& name) :
  Super(parent, "behaviourwidget")
{
  setText(name);
  setBackgroundColor(lightGray);

  setMouseTracking(true);    // enable mouse tracking for highlighting
}


PatternWidgetClass& 
BehaviourWidget::getPatternWidget() const
{
  PatternWidgetClass * ptr = dynamic_cast<PatternWidgetClass *>(parentWidget());
  assert(ptr != NULL);
  return *ptr; 
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
  // right button -> popup menu //
  if (event->button() == RightButton) {
    QString patternName = getPatternWidget().getPatternName();

     // popup menu //
    QPopupMenu menu;

    menu.insertItem("Up",                this, SLOT(onUp()),   0, 1);
    menu.insertItem("Down",              this, SLOT(onDown()), 0, 2);
    menu.insertSeparator();
    menu.insertItem("Set parameters",    this, SLOT(onSetParameters()));
    menu.insertSeparator();
    menu.insertItem("Delete behaviour",  this, SLOT(onDelete()));

    // grey up and down entry if not usable //
    menu.setItemEnabled(1, 
	      getDocument().hasPrevBehaviour(patternName, text()));

    menu.setItemEnabled(2, 
	      getDocument().hasNextBehaviour(patternName, text()));

    // show popup menu //
    menu.exec(QCursor::pos());
  }
}

void
BehaviourWidget::mouseDoubleClickEvent(QMouseEvent* event) 
{
  // left button //
  if (event->button() == LeftButton) {
    onSetParameters();
  }
}

void
BehaviourWidget::onSetParameters()
{
  QString patternName = getPatternWidget().getPatternName();

  // fill dialog with current parameters //
  QMap<QString, QString> paramMap = getDocument().getParameters(patternName, 
								text());
  ParameterDialog paramDialog(text(), paramMap, this);

  // OK pressed ? //
  if (paramDialog.exec()) {
    QMap<QString, QString> paramMap = paramDialog.result();
    getDocument().setParameters(patternName, text(), paramMap);
  }
}

void
BehaviourWidget::onDelete()
{
  QString patternName = getPatternWidget().getPatternName();

  getDocument().delBehaviour(patternName, text());
  getPatternWidget().init();             // update pattern
}


void
BehaviourWidget::onUp()
{
  QString patternName = getPatternWidget().getPatternName();

  getDocument().behaviourUp(patternName, text());
  getPatternWidget().init();             // update pattern
}

void
BehaviourWidget::onDown()
{
  QString patternName = getPatternWidget().getPatternName();

  getDocument().behaviourDown(patternName, text());
  getPatternWidget().init();             // update pattern
}
