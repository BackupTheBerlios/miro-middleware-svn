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

#include <qwidget.h>
#include <qpushbutton.h>
#include <qcheckbox.h> 
#include <qscrollbar.h>

// forward declarations
class PatternWidgetClass;

class PolicyViewClass : public QWidget
{
  Q_OBJECT

public:
  PolicyViewClass(QWidget *parent, PolicyDocumentClass& _document);

  void                 update();
  PolicyDocumentClass& getDocument();
  void                 startAddTransition(PatternWidgetClass* patternWidget);
  void                 endAddTransition(PatternWidgetClass* patternWidget);
  bool                 isAddTransitionMode() const;
  void		       setVerticalScrollValue(int value);
  void		       setHorizontalScrollValue(int value);
  void		       prepareFileClosing();

public slots:
  void getHorizontalScrollValue(int value); 
  void getVerticalScrollValue(int value);

protected slots:
  void onAddPattern();

protected:
  void resizeEvent(QResizeEvent* );
  void paintEvent(QPaintEvent* );
  void mousePressEvent(QMouseEvent* event);
  void mouseReleaseEvent(QMouseEvent* event);
  void mouseMoveEvent(QMouseEvent* event);

  PolicyDocumentClass& document;

  std::list<PatternWidgetClass*> PatternWidgetList;

  int                 picked_x;
  int                 picked_y;

  PatternWidgetClass* pickedPatternPtr;
  QString             pickedPattern;
  
  QScrollBar          *scrollBar,*verticalScrollBar;
  QWidget	      *parent;
  
 
  bool addTransitionMode;  // true while a signal arrow is dragged to destination
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
