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
#ifndef PolicyView_h
#define PolicyView_h

#include <qstring.h>
#include <qscrollview.h>

// forward declarations
class PolicyXML;
class PatternXML;
class PatternWidget;

class PolicyView : public QScrollView
{
  Q_OBJECT
  typedef QScrollView Super;

public:
  PolicyView(QWidget * _parent);
  virtual ~PolicyView();

  void init(PolicyXML * _policy);
  void startAddTransition(PatternWidget* patternWidget);
  void endAddTransition(PatternWidget* patternWidget);
  bool isAddTransitionMode() const;

  void addPatternWidget(PatternXML * _pattern);
  
protected:
  //  using QScrollView::drawContents;
  virtual void drawContents(QPainter * p, int clipx, int clipy, int clipw, int cliph);
  void drawArrows(QPainter * p);
  void contentsMousePressEvent(QMouseEvent* event);
  void contentsMouseMoveEvent(QMouseEvent* event);

  PolicyXML * policy_;

  int pickedX_;
  int pickedY_;

  PatternWidget * pickedPattern_;

  bool addTransitionMode_;  // true while a signal arrow is dragged to destination

  QPoint arrowFrom_;
  QPoint arrowTo_;
};

inline
bool  
PolicyView::isAddTransitionMode() const { 
  return addTransitionMode_;
}
#endif
