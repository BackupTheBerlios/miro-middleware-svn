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

// forward declarations
class QPopupMenu;
class QString;

/**
 * This class shows a behaviour name for use in PatternWidget
 */
class ArbiterWidget : public QLabel
{
  Q_OBJECT

  typedef QLabel Super;

public:
  ArbiterWidget(QWidget* parent, const QString& patternName);

protected slots:
  void onSetArbiter(int);

protected:
  void mousePressEvent(QMouseEvent * event);
  void enterEvent(QEvent*);
  void leaveEvent(QEvent*);

  //! returns a reference of the parent pattern widget
  PatternWidgetClass& getPatternWidget() const;

  //! returns a reference of the document
  PolicyDocumentClass& getDocument() const;

private:
  QPopupMenu * menuSetArbiter_;
};

inline
PolicyDocumentClass& 
ArbiterWidget::getDocument() const {
  return getPatternWidget().getDocument(); 
}

#endif
