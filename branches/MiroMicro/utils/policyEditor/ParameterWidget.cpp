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
