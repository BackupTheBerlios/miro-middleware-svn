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

#include "PolicyView.h"
#include "PatternWidget.h"

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

#include <algorithm>

PolicyViewClass::PolicyViewClass(QWidget * _parent, 
				 PolicyDocumentClass& _document) :
  Super(_parent, "view"), 
  document(_document),
  addTransitionMode(false)
{
  enableClipper(true);
  viewport()->setBackgroundMode(NoBackground);
  // viewport()->setBackgroundColor(white);

  // update the view //
  init(); 
}

void 
PolicyViewClass::contentsMousePressEvent(QMouseEvent* event) 
{
  // jump out of add transition mode
  if (addTransitionMode) {
    // repaint last arrow
    QRect rect(arrowFrom_, arrowTo_);
    rect = rect.normalize();
    repaintContents(rect);
    addTransitionMode = false;
  }

  // right button -> popup menu //
  if (event->button() == RightButton) {
    picked_x = event->pos().x();               // save this for onAddPattern
    picked_y = event->pos().y();
    // popup menu //
    QPopupMenu menu;
    menu.insertItem("Add pattern", this, SLOT(onAddPattern()));
    menu.exec(QCursor::pos());
  }
}

void 
PolicyViewClass::contentsMouseMoveEvent(QMouseEvent* event)
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
PolicyViewClass::init()
{
  // delete all childs
  QObjectList * childs;
  while((childs =  const_cast<QObjectList *>(viewport()->children())) != NULL && 
	!childs->isEmpty()) {
    delete childs->first();
  }

  //-------------------------//
  // rebuild the widget list //
  //-------------------------//

  QStringList list = document.getPatternNames();

  QStringList::Iterator patternIter = list.begin();
  while (patternIter !=list.end()) {
    // generate pattern widget //
    QString patternName = *patternIter;

    PatternWidgetClass* patternWidget = new PatternWidgetClass(this, viewport(), patternName);
    patternWidget->show(); 
    int x = std::max(0, document.getX(patternName));
    int y = std::max(0, document.getY(patternName));
    addChild(patternWidget, x, y);

    patternIter++;
  }

  QRect r = viewport()->childrenRect();
  QPoint s = viewportToContents(r.bottomRight());

  resizeContents(s.x(), s.y());
  setContentsPos(0, 0);
}

void 
PolicyViewClass::drawContents(QPainter * p, int clipx, int clipy, int clipw, int cliph)
{
  p->fillRect(clipx, clipy, clipw, cliph, white);

  const QPtrList<QObject> * childs = viewport()->children();

  if (childs) {
    QPtrListIterator<QObject> iter(*childs);
    while (iter.current() != NULL) {
      PatternWidgetClass * pattern = 
	dynamic_cast<PatternWidgetClass *>(iter.current());

      assert (pattern != NULL);

      pattern->drawArrows(p);
      ++iter;
    }
  }
  Super::drawContents(p, clipx, clipy, clipw, cliph);
}

void 
PolicyViewClass::onAddPattern()
{
  bool ok = false;
  QString text = QInputDialog::getText(
                    tr( "Add Pattern" ),
                    tr( "Pattern name:" ),
                    QLineEdit::Normal, QString::null, &ok, this );
  if ( ok && !text.isEmpty() ) {
    if (document.addPattern(text, picked_x, picked_y)) {
      PatternWidgetClass* patternWidget = new PatternWidgetClass(this, viewport(), text);
      addChild(patternWidget, picked_x, picked_y);
      patternWidget->show();  
      
      QRect r = viewport()->childrenRect();
      QPoint s = viewportToContents(r.bottomRight());

      resizeContents(s.x(), s.y());
      ensureVisible(picked_x, picked_y);

    }
    else
      QMessageBox::warning(this, 
			   "Add Pattern", 
			   "Pattern " + text + " allready exists.\n" + 
			   "No pattern added.");

  }
}

void 
PolicyViewClass::startAddTransition(PatternWidgetClass* patternWidget)
{
  pickedPatternPtr = patternWidget;
  pickedPattern    = patternWidget->getPatternName();
  addTransitionMode = true;
  arrowFrom_.setX(childX(pickedPatternPtr));
  arrowFrom_.setY(childY(pickedPatternPtr));

  viewport()->setMouseTracking(true);
}

// this is called from the clicked PatternWidget when in signal mode //
void 
PolicyViewClass::endAddTransition(PatternWidgetClass* patternWidget)
{
  // if addTransition mode is on, finish it and add the signal //
  if (isAddTransitionMode()) {
    viewport()->setMouseTracking(false);
    addTransitionMode = false;

    bool ok = false;
    QString message = QInputDialog::getText(tr( "Add Transition" ),
					    tr( "Transition name:" ),
					    QLineEdit::Normal, QString::null, &ok, this );
    if ( ok && !message.isEmpty() ) {
      QString target  = patternWidget->getPatternName();
      if (document.addTransition(pickedPattern, message, target))
	updateContents();
      else
	QMessageBox::warning(this, 
			     "Add Transition", 
			     "Transition " + message + " allready exists.\n" + 
			     "No transition added.");

    }
  }
}
