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
#ifndef BehaviourWidget_h
#define BehaviourWidget_h

#include "PatternWidget.h"
#include "PolicyDocument.h"

#include <qlabel.h>
#include <qstring.h>

/**
 * This class shows a behaviour name for use in PatternWidget
 */
class BehaviourWidget : public QLabel
{
  Q_OBJECT

  typedef QLabel Super;

private:
  int picked_x;
  int picked_y;

  QString behaviourName;

  /** returns a reference of the parent pattern widget */
  PatternWidgetClass& getPatternWidget() const;

  /** returns a reference of the document */
  PolicyDocumentClass& getDocument() const;

private slots:
  void onUp();
  void onDown();
  void onSetParameters();
  void onDelete();

protected:
  void mousePressEvent(QMouseEvent*);
  void mouseMoveEvent(QMouseEvent* event);
  void enterEvent(QEvent*);
  void leaveEvent(QEvent*);

public:
  BehaviourWidget(QWidget* parent, const QString& patternName);
};

/** returns a reference of the document */
inline
PolicyDocumentClass& 
BehaviourWidget::getDocument() const  
{
  return getPatternWidget().getDocument(); 
}

#endif
