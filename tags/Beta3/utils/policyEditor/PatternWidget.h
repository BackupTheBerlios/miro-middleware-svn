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
#ifndef PatternWidget_h
#define PatternWidget_h

#include "PolicyView.h"
#include "PolicyDocument.h"

#include <qwidget.h>
#include <qframe.h>
#include <qlabel.h>
#include <qpoint.h>

#include <list>

// forward declarations
class BehaviourWidget;
class ArbiterWidget;
class QPopupMenu;


/**
 * This class views the contents of a pattern description in the document. 
 */
class PatternWidgetClass : public QFrame
{
  Q_OBJECT

  typedef QFrame Super;

  static const int PATTERN_NAME_HEIGHT   = 18;
  static const int BEHAVIOUR_NAME_HEIGHT = 11;
  static const int ARBITER_NAME_HEIGHT   = 18;

private:
  QString patternName;

private slots:
  void onAddBehaviour(int);
  void onDelete();
  void onAddTransition();
  void onSetStart();
  void onRenamePattern();
  void onRenameTransition();

protected:
  void paintEvent(QPaintEvent* event);
  void drawArrow(QPainter * p, int x1, int y1, int x2, int y2, int size);
  void mousePressEvent(QMouseEvent*);
  void mouseReleaseEvent(QMouseEvent* event);
  void mouseMoveEvent(QMouseEvent* event);
  void enterEvent(QEvent*);
  void leaveEvent(QEvent*);
  void renamePattern(const QString& oldName);
  void renameTransition(const QString& pattern);

public:
  PatternWidgetClass(PolicyViewClass * view, QWidget* parent, const QString& name);
  virtual ~PatternWidgetClass();

  /** returns the name of the associated pattern */
  const QString& getPatternName() const;

  /** returns a reference of the document. (called by BehaviourWidget) */
  PolicyDocumentClass& getDocument();

  /** updates internal representation and calls repaint(). (called by
   *  BehaviourWidget) */
  void init();

  /** returns a reference of the main view. (called by BehaviourWidget) */
  PolicyViewClass& getView();

  void drawArrows(QPainter * p);

  PolicyViewClass * view_;
  QPopupMenu * menuAddBehaviour_;

  QPoint pickedPos;
};

inline
PolicyViewClass&
PatternWidgetClass::getView() {
  return *view_;
}

inline
const QString& 
PatternWidgetClass::getPatternName() const    
{
  return patternName; 
}

inline
PolicyDocumentClass&
PatternWidgetClass::getDocument()
{
  return getView().getDocument(); 
}

#endif
