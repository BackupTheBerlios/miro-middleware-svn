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

#include <list>

class BehaviourWidget;
class ArbiterWidget;


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

  ArbiterWidget*         arbiterWidget;
  std::list<BehaviourWidget*> behaviourWidgetList;

  int picked_x;
  int picked_y;

private slots:
  void onAddBehaviour(int);
  void onDelete();
  void onAddTransition();
  void onSetStart();
  void onRenamePattern();
  void onRenameTransition();

protected:
  void paintEvent(QPaintEvent* event);
  void drawArrow(QPainter& p, int x1, int y1, int x2, int y2, int size);
  void mousePressEvent(QMouseEvent*);
  void mouseReleaseEvent(QMouseEvent*);
  void mouseMoveEvent(QMouseEvent* event);
  void enterEvent(QEvent*);
  void leaveEvent(QEvent*);
  void renamePattern(QString oldName);
  void renameTransition(QString pattern);

public:
  PatternWidgetClass(QWidget* parent, const QString& name);
  virtual ~PatternWidgetClass();

  /** returns the name of the associated pattern */
  const QString& getPatternName() const;

  /** returns a reference of the document. (called by BehaviourWidget) */
  PolicyDocumentClass& getDocument() const;

  /**updated PatternPosition*/
  void updatePos();

  /** updates internal representation and calls repaint(). (called by
   *  BehaviourWidget) */
  virtual void update();

  /** returns a reference of the main view. (called by BehaviourWidget) */
  PolicyViewClass& getView() const;
};

inline
const QString& 
PatternWidgetClass::getPatternName() const    
{
  return patternName; 
}

inline
PolicyDocumentClass&
PatternWidgetClass::getDocument() const
{
  return getView().getDocument(); 
}

#endif
