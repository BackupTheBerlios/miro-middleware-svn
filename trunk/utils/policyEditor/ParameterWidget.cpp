// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "ParameterWidget.h"
#include "utils/widgets/ParameterXML.h"

#include <qpopupmenu.h>
#include <qcursor.h>

ParameterWidget::ParameterWidget(QColor _cNormal, QColor _cEntered,
				 ParameterXML * _parameter,
				 QWidget * _parent, char const * _name) :
  Super(_parent, _name),
  cNormal_(_cNormal),
  cEntered_(_cEntered),
  parameter_(_parameter)
{
  setText(_name);
  setBackgroundColor(cNormal_);

  setMouseTracking(true);    // enable mouse tracking for highlighting

  parameter_->setWidget(this);
}

ParameterWidget::~ParameterWidget()
{
  if (parameter_ != NULL)
    parameter_->setWidget(NULL);
}

void
ParameterWidget::enterEvent(QEvent *)
{
  setBackgroundColor(cEntered_); // highlighting on
  repaint();
}

void 
ParameterWidget::leaveEvent(QEvent *)
{
  setBackgroundColor(cNormal_); // highliting off
  repaint();
}

void
ParameterWidget::mousePressEvent(QMouseEvent * _event) 
{
  // right button -> popup menu
  if (_event->button() == RightButton) {
    // popup menu
    QPopupMenu menu;
    // populate menu
    parameter_->contextMenu(menu);
    // show popup menu //
    menu.exec(QCursor::pos());
  }
}

void
ParameterWidget::mouseDoubleClickEvent(QMouseEvent * _event) 
{
  // left button -> edit parameters
  if (_event->button() == LeftButton) {
    parameter_->setParameters();
  }
}
