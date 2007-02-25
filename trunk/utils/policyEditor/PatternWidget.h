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
#ifndef PatternWidget_h
#define PatternWidget_h

#include <qframe.h>
#include <qvbox.h>
#include <qlabel.h>

// forward declarations
class BehaviourWidget;
class ArbiterWidget;
class PatternXML;
class PolicyView;
class TransitionContainer;

/**
 * This class views the contents of a pattern description in the document. 
 */
class PatternWidget : public QLabel //QVBox
{
  Q_OBJECT

  typedef QLabel Super;

  static const int PATTERN_NAME_HEIGHT   = 18;
  static const int BEHAVIOUR_NAME_HEIGHT = 11;
  static const int ARBITER_NAME_HEIGHT   = 18;

public:
  PatternWidget(PatternXML * _pattern,
		PolicyView * _view, char const * name = NULL);
  virtual ~PatternWidget();

  PatternXML * pattern();

  /** updates internal representation and calls repaint(). (called by
   *  BehaviourWidget) */
  void init();

  void drawArrows(QPainter * p);

  static void drawArrow(QPainter * p, int x1, int y1, int x2, int y2, int size);

  void removeTransitionContainer();

protected:
  void paintEvent(QPaintEvent* event);
  void mousePressEvent(QMouseEvent*);
  void mouseReleaseEvent(QMouseEvent* event);
  void mouseMoveEvent(QMouseEvent* event);
  void enterEvent(QEvent*);
  void leaveEvent(QEvent*);
  void renamePattern(const QString& oldName);
  void renameTransition(const QString& pattern);

  int numBehaviours();
  int numArbiters();

  PatternXML * pattern_;
  PolicyView * view_;
  QLabel * title_;
  TransitionContainer * transitions_;

  QPoint pickedPos_;
};

inline
void
PatternWidget::removeTransitionContainer() {
  transitions_ = NULL;
}

inline
PatternXML *
PatternWidget::pattern() {
  return pattern_;
}
#endif
