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
#ifndef ArbiterWidget_h
#define ArbiterWidget_h

#include "PolicyDocument.h"
#include "PatternWidget.h"

#include <qlabel.h>

/**
 * This class shows a behaviour name for use in PatternWidget
 */
class ArbiterWidget : public QLabel
{
  Q_OBJECT

  typedef QLabel Super;

private:
  QString arbiterName;

  /** returns a reference of the parent pattern widget */
  PatternWidgetClass& getPatternWidget() const;

  /** returns a reference of the document */
  PolicyDocumentClass& getDocument() const;

private slots:
  void onSetArbiter(int);
  void onDelete();

protected:
  void mousePressEvent(QMouseEvent*);
  void mouseMoveEvent(QMouseEvent* event);
  void enterEvent(QEvent*);
  void leaveEvent(QEvent*);

public:
  ArbiterWidget(QWidget* parent, const QString& patternName);
};

inline
PolicyDocumentClass& 
ArbiterWidget::getDocument() const  
{
  return getPatternWidget().getDocument(); 
}

#endif
