// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2002, 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
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
