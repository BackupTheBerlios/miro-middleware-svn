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
#ifndef PolicyView_h
#define PolicyView_h

#include "PolicyDocument.h"

#include <qstring.h>
#include <qscrollview.h>

// forward declarations
class PatternWidgetClass;

class PolicyViewClass : public QScrollView
{
  Q_OBJECT
  typedef QScrollView Super;

public:
  PolicyViewClass(QWidget * _parent, PolicyDocumentClass& _document);

  void                 init();
  PolicyDocumentClass& getDocument();
  void                 startAddTransition(PatternWidgetClass* patternWidget);
  void                 endAddTransition(PatternWidgetClass* patternWidget);
  bool                 isAddTransitionMode() const;

protected slots:
  void onAddPattern();

protected:
  void drawContents(QPainter * p, int clipx, int clipy, int clipw, int cliph);
  void contentsMousePressEvent(QMouseEvent* event);
  void contentsMouseMoveEvent(QMouseEvent* event);

  PolicyDocumentClass& document;

  int                 picked_x;
  int                 picked_y;

  PatternWidgetClass* pickedPatternPtr;
  QString             pickedPattern;

  bool addTransitionMode;  // true while a signal arrow is dragged to destination

  QPoint arrowFrom_;
  QPoint arrowTo_;
};

inline
PolicyDocumentClass&
PolicyViewClass::getDocument() {
  return document; 
}

inline
bool  
PolicyViewClass::isAddTransitionMode() const { 
  return addTransitionMode;
}
#endif
