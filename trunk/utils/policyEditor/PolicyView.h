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
