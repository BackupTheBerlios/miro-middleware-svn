// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2002, 2003, 2004, 2005
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "PolicyView.h"
#include "PatternWidget.h"
#include "PolicyXML.h"

#include "miro/Log.h"

#include "miro/Log.h"

#include <qpainter.h>
#include <qpopupmenu.h>
#include <qdialog.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qmainwindow.h>
#include <qstatusbar.h>
#include <qcursor.h>
#include <qpushbutton.h>
#include <qcheckbox.h> 
#include <qscrollbar.h>
#include <qinputdialog.h>
#include <qmessagebox.h>
#include <qobjectlist.h>
#include <qlistview.h>

#include <algorithm>
#include <iostream>

using namespace std;

PolicyView::PolicyView(QWidget * _parent) :
  Super(_parent, "policy view"), 
  policy_(NULL),
  addTransitionMode_(false)
{
  enableClipper(true);
  viewport()->setBackgroundMode(NoBackground);
  // viewport()->setBackgroundColor(white);
}

PolicyView::~PolicyView()
{
  if (policy_ != NULL)
    policy_->setWidget(NULL);
}

void 
PolicyView::contentsMousePressEvent(QMouseEvent* event) 
{
  // jump out of add transition mode
  if (addTransitionMode_) {
    // repaint last arrow
    QRect rect(arrowFrom_, arrowTo_);
    rect = rect.normalize();
    repaintContents(rect);
    addTransitionMode_ = false;
  }

  // right button -> popup menu
  if (policy_ != NULL &&
      event->button() == RightButton) {
    pickedX_ = event->pos().x();               // save this for onAddPattern
    pickedY_ = event->pos().y();

    // popup menu //
    QPopupMenu menu;
    policy_->contextMenu(menu);
    menu.exec(QCursor::pos());
  }
}

void 
PolicyView::contentsMouseMoveEvent(QMouseEvent* event)
{
  if (isAddTransitionMode()) {
    
    // repaint last arrow
    QRect rect(arrowFrom_, arrowTo_);
    rect = rect.normalize();
    repaintContents(rect);

    // update arrow coordinates
    arrowTo_ = event->pos();

    // draw new arrow
    QPainter p(this->viewport());
    p.drawLine(contentsToViewport(arrowFrom_), contentsToViewport(arrowTo_));
  }
}

void 
PolicyView::addPatternWidget(PatternXML * _pattern)
{
    PatternWidget * widget = new PatternWidget(_pattern, this, _pattern->name());
    widget->show();
    int x = std::max(0, _pattern->x());
    int y = std::max(0, _pattern->y());
    addChild(widget, x, y);
    viewport()->update();
}

void 
PolicyView::init(PolicyXML * _policy)
{
  if (policy_ == _policy)
    return;

  // delete all childs
  QObjectList * childs;
  while((childs =  const_cast<QObjectList *>(viewport()->children())) != NULL && 
	!childs->isEmpty()) {
    delete childs->first();
  }

  policy_ = _policy;

  if (policy_ == NULL)
    return;

  policy_->setWidget(this);

  //-------------------------
  // rebuild the widget list

  QListViewItem * i = policy_->listViewItem()->firstChild();
  while (i != NULL) {
    Item * item = policy_->itemFromListViewItem(i);
    PatternXML * pattern = dynamic_cast<PatternXML *>(item);
    if (pattern != NULL)
      addPatternWidget(pattern);

    i = i->nextSibling();
  }

  QRect r = viewport()->childrenRect();
  QPoint s = viewportToContents(r.bottomRight());

  resizeContents(s.x(), s.y());
  setContentsPos(0, 0);
}

void 
PolicyView::drawContents(QPainter * p, int clipx, int clipy, int clipw, int cliph)
{
  p->fillRect(clipx, clipy, clipw, cliph, white);

  const QObjectList * childs = viewport()->children();

  if (childs) {
    QObjectListIt iter(*childs);
    while (iter.current() != NULL) {
      PatternWidget * pattern = 
	dynamic_cast<PatternWidget *>(iter.current());

      if (pattern != NULL) {
	pattern->drawArrows(p);
      }
	++iter;
    }
    this->drawArrows(p);
  }
  Super::drawContents(p, clipx, clipy, clipw, cliph);
}

void
PolicyView::drawArrows(QPainter * p)
{
  p->setPen(darkGreen);
  
  // get list with transitions //
  TransitionVector transitions = policy_->transitions();

  const int Zeilenabstand=10;
  int DrawX1 = std::max(contentsWidth(), visibleWidth());
  int DrawY1 = 0;
  int i;

  // show all transitions //
  QMap<PatternXML *, int> map;
  TransitionVector::iterator first, last = transitions.end(); 
  for (first = transitions.begin(); first != last; ++first ) {
    QString message = first->getMessage();
    PatternXML * target  = first->getTarget();

    map[target] = map[target] + 1;
    
    int DrawX2 = childX(target->widget()) + target->widget()->width();
    int DrawY2 = childY(target->widget());

    PatternWidget::drawArrow(p, DrawX1, DrawY1, DrawX2, DrawY2, 10);
    i = map[target]-1;
    p->drawText((DrawX1 + DrawX2) / 2,
		(DrawY1 + DrawY2) /2 + i * Zeilenabstand, message);
  }
}

void 
PolicyView::startAddTransition(PatternWidget * _pattern)
{
  pickedPattern_ = _pattern;
  arrowFrom_.setX(childX(pickedPattern_));
  arrowFrom_.setY(childY(pickedPattern_));

  addTransitionMode_ = true;
  viewport()->setMouseTracking(true);
}

// this is called from the clicked PatternWidget when in signal mode //
void 
PolicyView::endAddTransition(PatternWidget * _target)
{
  // if addTransition mode is on, finish it and add the signal //
  if (isAddTransitionMode()) {
    viewport()->setMouseTracking(false);
    addTransitionMode_ = false;

    bool ok = false;
    QString message = QInputDialog::getText(tr( "Add Transition" ),
					    tr( "Transition name:" ),
					    QLineEdit::Normal, QString::null, &ok, this );
    if ( ok && !message.isEmpty() ) {
      if (pickedPattern_->pattern()->tryAddTransition(message, _target->pattern()))
	updateContents(contentsX(), contentsY(),
		       contentsWidth(), contentsHeight());
      else
	QMessageBox::warning(this, 
			     "Add Transition", 
			     "Transition " + message + " allready exists.\n" + 
			     "No transition added.");

    }
  }
}
