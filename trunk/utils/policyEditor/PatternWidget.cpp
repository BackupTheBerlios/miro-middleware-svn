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

#include "PatternWidget.h"
#include "BehaviourWidget.h"
#include "ArbiterWidget.h"
#include "Transition.h"
#include "PolicyView.h"
#include "TransitionContainer.h"
#include "TransitionXML.h"
#include "TransitionWidget.h"

#include "PatternXML.h"
#include "ArbiterXML.h"
#include "BehaviourXML.h"

#include "utils/widgets/ConfigFile.h"

#include "params/Generator.h"

#include "miro/Log.h"

#include <qpainter.h>
#include <qpopupmenu.h>
#include <qdialog.h>
#include <qlabel.h>
#include <qtable.h>
#include <qheader.h>
#include <qlistbox.h>
#include <qcursor.h>
#include <qpushbutton.h>
#include <qinputdialog.h>
#include <qmessagebox.h>
#include <qobjectlist.h>
#include <qpoint.h>
#include <qlistview.h>
#include <qlayout.h>

#include <algorithm>
#include <cmath>


PatternWidget::PatternWidget(PatternXML * _pattern,
			     PolicyView * _view, char const * _name) :
  Super(_view->viewport(), _name), 
  pattern_(_pattern),
  view_(_view),
  title_(NULL),
  transitions_(NULL)
{
  QSizePolicy sp(QSizePolicy::Fixed, QSizePolicy::Fixed);

  //  setSizePolicy(sp);
  setFrameStyle(QFrame::WinPanel | QFrame::Raised);
  setBackgroundColor(red);
  setMouseTracking(true);    // enable mouse tracking for highlighting

  //  layout()->setResizeMode(QLayout::Fixed);

  pattern_->setWidget(this);
  init();
}


PatternWidget::~PatternWidget()
{
  if (pattern_ != NULL)
    pattern_->setWidget(NULL);

  if (parent()) {

    QWidget * p = dynamic_cast<QWidget *>(parent());
    MIRO_ASSERT(p != NULL);
    p->update();
  }
}

int
PatternWidget::numArbiters()
{
  // get the number of arbiters (0/1)
  // for calculating the size of the widget

  int arbiters = 0;
  QListViewItem * i = pattern_->listViewItem()->firstChild();
  while (i != NULL) {
    Item * item = Item::itemFromListViewItem(i);
    ArbiterXML * arbiter = dynamic_cast<ArbiterXML *>(item);
    if (arbiter != NULL) {
      ++arbiters;
    } 
    i = i->nextSibling();
  }

  return arbiters;
}

int
PatternWidget::numBehaviours()
{
  // get the number of behaviours (0...n)
  // for calculating the size of the widget

  int behaviours = 0;
  QListViewItem * i = pattern_->listViewItem()->firstChild();
  while (i != NULL) {
    Item * item = Item::itemFromListViewItem(i);
    BehaviourXML * behaviour = dynamic_cast<BehaviourXML *>(item);
    if (behaviour != NULL) {
      ++behaviours;
    }
    i = i->nextSibling();
  }

  return behaviours;
}

void 
PatternWidget::init()
{
  // delete all childs
  QObjectList const * childs = children();
  if (childs != NULL) {
    QObjectList l(*childs);
    while (l.count() > 0) {
      if (dynamic_cast<QLabel *>(l.first()) != NULL) {
	delete l.first();
      }
      l.removeFirst();
    }
  }

  // object title

#ifdef QLAYOUT_USED
  title_ = new QLabel(this);
  title_->setAlignment(title_->alignment() | Qt::AlignHCenter);
  title_->setBackgroundColor(red);
  QString patternText = pattern_->name();
  // if start pattern -> add "*" to name
  if (pattern_->startPattern()) 
    patternText = QString("* ") + pattern_->name() + " *";
  title_->setText(patternText);
#endif

  int behaviours = numBehaviours();
  int arbiters = numArbiters();
 
  // FIXME: use qlayout instead
  
  // resize widget //
  setFixedSize(100,
  	       PATTERN_NAME_HEIGHT + 
	       BEHAVIOUR_NAME_HEIGHT * behaviours + 
  	       ARBITER_NAME_HEIGHT * arbiters+
  	       2 * frameWidth());

  // get internal rect dimensions (inside the frame) //
  QRect cr = contentsRect();

  //----------------//
  // behaviour list //
  //----------------//

  behaviours = 0;
  QListViewItem * i = pattern_->listViewItem()->firstChild();
  while (i != NULL) {
    QWidget * widget = 0;
    Item * item = Item::itemFromListViewItem(i);
    BehaviourXML * behaviour = dynamic_cast<BehaviourXML *>(item);

    //--------------------------------------------------------------------------
    // add behaviours
    if (behaviour != NULL) {
      widget = new BehaviourWidget(behaviour, this, behaviour->name());
      widget->setGeometry(cr.x(), 
  			  cr.y() + PATTERN_NAME_HEIGHT +
  			  behaviours * BEHAVIOUR_NAME_HEIGHT,
  			  cr.width(), BEHAVIOUR_NAME_HEIGHT);  
      ++behaviours;
    }
    else {
      //------------------------------------------------------------------------
      // add arbiter
      ArbiterXML * arbiter = dynamic_cast<ArbiterXML *>(item);
      if (arbiter != NULL) {
	widget = new ArbiterWidget(arbiter, this, arbiter->name());
  	widget->setGeometry(cr.x(), cr.y() + cr.height() - ARBITER_NAME_HEIGHT, 
  			    cr.width(), ARBITER_NAME_HEIGHT);  
      }
      //#define ASDF
#ifdef ASDF
      else {
	//----------------------------------------------------------------------
	// add transition
	TransitionXML * transition = dynamic_cast<TransitionXML *>(item);
	if (transition != NULL) {
	  if (transitions_ == NULL) {
	    transitions_ = new TransitionContainer(this,
						   parentWidget(), "transition container");
	    transitions_->show();
	    view_->addChild(transitions_, 10, 10);
	  }
	  widget = new TransitionWidget(transition,
					transitions_, "transition");
	}
      }
#endif
    }
    if (widget != NULL)
      widget->show();
    i = i->nextSibling();
  }

  updateGeometry();
  view_->viewport()->update();
}


void 
PatternWidget::paintEvent(QPaintEvent* event)
{
  // draw the frame etc. //
  Super::paintEvent(event);

  QPainter p(this);
  
  //-------------------//
  // show pattern name //
  //-------------------//
  
  // get internal rect dimensions (inside the frame) //
  QRect cr = contentsRect();

  QString patternText = pattern_->name();
  // if start pattern -> add "*" to name
  if (pattern_->startPattern()) 
    patternText = QString("* ") + pattern_->name() + " *";

  p.drawText(cr.x(), cr.y(), cr.width(), PATTERN_NAME_HEIGHT,
	     AlignHCenter | AlignVCenter, patternText);
}

void
PatternWidget::drawArrows(QPainter * p)
{
  int behaviours = numBehaviours();

  p->setPen(darkGreen);
  
  // get list with transitions //
  TransitionVector transitions = pattern()->transitions();
  std::sort(transitions.begin(), transitions.end());

  const int Zeilenabstand=10;
  int DrawX1, DrawX2, DrawY1, DrawY2;
  int i;
  // show all transitions //
  QMap<PatternXML *, int> map;
  TransitionVector::iterator first, last = transitions.end(); 
  for (first = transitions.begin(); first != last; ++first ) {
    QString message = first->getMessage();
    PatternXML * target  = first->getTarget();
    map[target] = map[target] + 1;
    
    int x2 = target->x();
    int y2 = target->y();

    if (view_->childX(this) < x2) { 
      DrawX1 = view_->childX(this) + width(); 
      DrawY1 = view_->childY(this);  
      DrawX2 = x2;  
      DrawY2 = y2; 
    }

    else { 
      DrawX1 = view_->childX(this);
      DrawY1 = view_->childY(this)+height(); 
      DrawX2 = x2 + width();    
      DrawY2 = y2 + PATTERN_NAME_HEIGHT +
	BEHAVIOUR_NAME_HEIGHT * behaviours +
	2 * frameWidth(); 
    } 

    drawArrow(p, DrawX1, DrawY1, DrawX2, DrawY2, 10);
    i = map[target]-1;
    p->drawText((DrawX1 + DrawX2) / 2,
		(DrawY1 + DrawY2) /2 + i * Zeilenabstand, message);
  }
}

void
PatternWidget::drawArrow(QPainter * p,
			 int x1, int y1, 
			 int x2, int y2, int size)
{
  p->drawLine(x1, y1, x2, y2);

  float a=atan2(x2-x1, y2-y1);
  float a1=a-0.3;
  float a2=a+0.3;

  p->drawLine(x2, y2, int(x2 - size * sin(a1)), int(y2 - size * cos(a1)));
  p->drawLine(x2, y2, int(x2 - size * sin(a2)), int(y2 - size * cos(a2)));
}

void
PatternWidget::mouseReleaseEvent(QMouseEvent *)
{
  view_->updateContents(view_->contentsX(), 
			view_->contentsY(),
			view_->contentsWidth(),
			view_->contentsHeight());

}

void
PatternWidget::mousePressEvent(QMouseEvent* event) 
{
  // left button //
  if (event->button() == LeftButton) {

    // are we in the addTransitionMode ? //
    if (view_->isAddTransitionMode()) {
      view_->endAddTransition(this);
    }

    // else, save the click position for a possible move //
    else {
      pickedPos_ = event->pos();
    }
  }


  // right button -> popup menu //
  else if (event->button() == RightButton) {
    // create popup menu
    QPopupMenu menu;
    // populate menu
    pattern_->contextMenu(menu);
    // show popup menu
    menu.exec(QCursor::pos());
  }
}

void
PatternWidget::mouseMoveEvent(QMouseEvent* event)
{
  // if left key is pressed -> move widget //
  if (event->state() & LeftButton) {

    // map to parent:
    QPoint newPos(view_->childX(this), view_->childY(this));
    newPos += event->pos();
    newPos -= pickedPos_;

    if (newPos.x() < 0)
      newPos.setX(0);
    if (newPos.y() < 0)
      newPos.setY(0);

    view_->moveChild(this, newPos.x(), newPos.y());
    pattern_->setX(newPos.x());
    pattern_->setY(newPos.y());

    QRect r = view_->viewport()->childrenRect();
    QPoint s = view_->viewportToContents(r.bottomRight());

    view_->resizeContents(s.x(), s.y());
    view_->ensureVisible(newPos.x(), newPos.y());
  }
}

void
PatternWidget::enterEvent(QEvent*)
{
  setBackgroundColor(yellow);
  repaint();
}


void
PatternWidget::leaveEvent(QEvent*)
{
  setBackgroundColor(red);
  repaint();
}

//------------------------------------------------

#ifdef ASFASF
void 
PatternWidget::onAddTransition()
{
  view_->startAddTransition(this);
}
#endif
