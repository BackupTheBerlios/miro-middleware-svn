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

#include "PatternWidget.h"

#include "BehaviourWidget.h"
#include "ArbiterWidget.h"

#include "PolicyConfig.h"
#include "params/Generator.h"

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

#include <cmath>

using namespace Miro::CFG;

PatternWidgetClass::PatternWidgetClass(PolicyViewClass * view,
				       QWidget* parent, const QString& name) :
  Super(parent, "patternwidget"), 
  patternName(name),
  view_(view)
{
  setFrameStyle(QFrame::WinPanel | QFrame::Raised);
  setBackgroundColor(red);

  setMouseTracking(true);    // enable mouse tracking for highlighting

  init();
}


PatternWidgetClass::~PatternWidgetClass()
{
}

void 
PatternWidgetClass::init()
{
  // delete all childs
  QObjectList * childs;
  while((childs =  const_cast<QObjectList *>(this->children())) != NULL && 
	!childs->isEmpty()) {
    delete childs->first();
  }

  // resize widget //
  int numBehaviours = getDocument().getNumBehaviours(patternName);
  setFixedSize(100,
	       PATTERN_NAME_HEIGHT + 
	       BEHAVIOUR_NAME_HEIGHT * numBehaviours + 
	       ARBITER_NAME_HEIGHT +
	       2*frameWidth());

  // get internal rect dimensions (inside the frame) //
  QRect cr = contentsRect();


  //----------------//
  // behaviour list //
  //----------------//

   // get string list with behaviour names //
   QStringList list = getDocument().getBehaviourNames(patternName);

   // show all behaviour names //
   int i = 0;
   for (QStringList::Iterator it = list.begin(); it != list.end(); ++it ) {
     QString behName = (*it);

     // create new label //
     BehaviourWidget* behWidget = new BehaviourWidget(this, behName);
     behWidget->setGeometry(cr.x(), 
			    cr.y()+PATTERN_NAME_HEIGHT+i*BEHAVIOUR_NAME_HEIGHT,
			    cr.width(), BEHAVIOUR_NAME_HEIGHT);  
     behWidget->show();

     i++;
   }


  //---------//
  // arbiter //
  //---------//

  ArbiterWidget * arbiterWidget = 
    new ArbiterWidget(this, getDocument().getArbiter(patternName));
  arbiterWidget->setGeometry(cr.x(), cr.y()+cr.height()-ARBITER_NAME_HEIGHT, 
			     cr.width(), ARBITER_NAME_HEIGHT);  
  arbiterWidget->show();

  update();
}


void 
PatternWidgetClass::paintEvent(QPaintEvent* event)
{
  // draw the frame etc. //
  Super::paintEvent(event);

  QPainter p(this);
  
  //-------------------//
  // show pattern name //
  //-------------------//
  
  // get internal rect dimensions (inside the frame) //
  QRect cr = contentsRect();

  QString patternText = patternName;   // default
  // if start pattern -> add "*" to name //
  if (getDocument().isStartPattern(patternName)) 
    patternText = QString("* ") + patternText + " *";

  p.drawText(cr.x(), cr.y(), cr.width(), PATTERN_NAME_HEIGHT,
	     AlignHCenter | AlignVCenter, patternText);
}

void
PatternWidgetClass::drawArrows(QPainter * p)
{
  PolicyViewClass& daddy = getView();

  p->setPen(darkGreen);
  
  // get list with transitions //
  TransitionList transitionList = getDocument().getTransitions(patternName);

  const int Zeilenabstand=10;
  int DrawX1, DrawX2, DrawY1, DrawY2,OldDrawX1, OldDrawX2, OldDrawY1, OldDrawY2,i;
  // show all transitions //
  OldDrawX1=-1;
  OldDrawX2=-1;
  OldDrawY1=-1;
  OldDrawY2=-1;
  
  QMap<QString, int> map;
  for (TransitionList::iterator it = transitionList.begin(); 
       it != transitionList.end(); ++it ) {
    QString message = it->getMessage();
    QString target  = it->getTarget();
    map[target] = map[target] + 1;
    
    int x2 = getDocument().getX(target);
    int y2 = getDocument().getY(target);

    if (daddy.childX(this) < x2) { 
      DrawX1 = daddy.childX(this)+width(); 
      DrawY1 = daddy.childY(this);  
      DrawX2 = x2;  
      DrawY2 = y2; 
    }

    else { 
      DrawX1 = daddy.childX(this);
      DrawY1 = daddy.childY(this)+height(); 
      DrawX2 = x2+width();    
      DrawY2 = y2+PATTERN_NAME_HEIGHT +
	BEHAVIOUR_NAME_HEIGHT * getDocument().getNumBehaviours(target) +
	2*frameWidth(); 
    } 
    
    drawArrow(p, DrawX1, DrawY1, DrawX2, DrawY2, 10);
    i=map[target]-1;
    p->drawText( (DrawX1+DrawX2)/2, (DrawY1+DrawY2)/2+i*Zeilenabstand, message);
    
  }
}

void
PatternWidgetClass::drawArrow(QPainter * p,
			      int x1, int y1, 
			      int x2, int y2, int size)
{
  p->drawLine(x1, y1, x2, y2);

  float a=atan2(x2-x1, y2-y1);
  float a1=a-0.3;
  float a2=a+0.3;

  p->drawLine(x2, y2, int(x2-size*sin(a1)), int(y2-size*cos(a1)));
  p->drawLine(x2, y2, int(x2-size*sin(a2)), int(y2-size*cos(a2)));
}

void
PatternWidgetClass::mouseReleaseEvent(QMouseEvent *)
{
  getView().updateContents(getView().contentsX(), 
			   getView().contentsY(),
			   getView().contentsWidth(),
			   getView().contentsHeight());

}

void
PatternWidgetClass::mousePressEvent(QMouseEvent* event) 
{
  // left button //
  if (event->button() == LeftButton) {

    // are we in the addTransitionMode ? //
    if (getView().isAddTransitionMode()) {
      getView().endAddTransition(this);
    }

    // else, save the click position for a possible move //
    else {
      pickedPos = event->pos();
    }
  }


  // right button -> popup menu //
  else if (event->button() == RightButton) {

    // popup menu //
    QPopupMenu menu;
    menuAddBehaviour_ = new QPopupMenu(&menu);

    menu.insertItem("Start pattern",  this, SLOT(onSetStart()));
    menu.insertSeparator();
    menu.insertItem("Add behaviour",  menuAddBehaviour_);
    menu.insertItem("Add transition", this, SLOT(onAddTransition()));
    menu.insertSeparator();
    menu.insertItem("Rename pattern", this, SLOT(onRenamePattern()));
    menu.insertItem("Rename transition", this, SLOT(onRenameTransition()));
    menu.insertItem("Delete pattern", this, SLOT(onDelete()));

    // submenu: add all behaviour names //
    Generator::GroupMap::const_iterator first, last;
    PolicyConfigClass::instance()->description().getGroupedTypes("behaviour", first, last);
    for (; first != last; ++first) {
      if (first->second.isFinal() &&
	  !getDocument().hasBehaviour(patternName, first->second.name())) {
        menuAddBehaviour_->insertItem(first->second.name());
      }
    }
    connect(menuAddBehaviour_, SIGNAL(activated(int)), this, SLOT(onAddBehaviour(int)));

    // show popup menu
    menu.exec(QCursor::pos());
  }
}

void
PatternWidgetClass::mouseMoveEvent(QMouseEvent* event)
{
  // if left key is pressed -> move widget //
  if (event->state() & LeftButton) {

    // map to parent:
    QPoint newPos(getView().childX(this), getView().childY(this));
    newPos += event->pos();
    newPos -= pickedPos;

    if (newPos.x() < 0)
      newPos.setX(0);
    if (newPos.y() < 0)
      newPos.setY(0);

    getView().moveChild(this, newPos.x(), newPos.y());
    getDocument().setX(patternName, newPos.x());
    getDocument().setY(patternName, newPos.y());

    QRect r = getView().viewport()->childrenRect();
    QPoint s = getView().viewportToContents(r.bottomRight());

    getView().resizeContents(s.x(), s.y());
    getView().ensureVisible(newPos.x(), newPos.y());
  }
}

void
PatternWidgetClass::enterEvent(QEvent*)
{
  setBackgroundColor(yellow);
  repaint();
}


void
PatternWidgetClass::leaveEvent(QEvent*)
{
  setBackgroundColor(red);
  repaint();
}

//------------------------------------------------



void 
PatternWidgetClass::onAddBehaviour(int n)
{
  // add a behaviour to the current pattern //
  getDocument().addBehaviour(patternName, menuAddBehaviour_->text(n));
  init();
}


void 
PatternWidgetClass::onDelete()
{
  // remove the current pattern //
  getDocument().delPattern(patternName);
  delete this;
}


void 
PatternWidgetClass::onAddTransition()
{
  getView().startAddTransition(this);
}


void 
PatternWidgetClass::onSetStart()
{
  // set pattern to start pattern //
  getDocument().setStartPattern(patternName);
}

void 
PatternWidgetClass::onRenamePattern()
{
  renamePattern(patternName);
}

void 
PatternWidgetClass::onRenameTransition()
{
  renameTransition(patternName);
}

//-------------------------------------------------
void 
PatternWidgetClass::renamePattern(const QString& oldName)
{
  bool ok = false;
  QString name = QInputDialog::getText(tr( "Rename Pattern" ),
					    tr( "Pattern name:" ),
					    QLineEdit::Normal, oldName, &ok, this );
  if ( ok && !name.isEmpty() && name != oldName) {
    if (getDocument().renamePattern(oldName, name)) {
      patternName = name;
      init();
    }
    else {
      QMessageBox::warning(this, 
			   "Rename Pattern", 
			   "Pattern name " + name + " allready exists.\n" + 
			   "Pattern not renamed.");

    }
  }  
}

void
PatternWidgetClass::renameTransition(const QString& nameOfPattern)
{
  QDialog dialog(this, "RenameTransitionDialog", true);
  dialog.setCaption("Rename Transition");
  TransitionList transitionList=getDocument().getTransitions(nameOfPattern);

  QTable table(transitionList.size(),2,&dialog);
  table.horizontalHeader()->setLabel(0,"Message",-1);
  table.horizontalHeader()->setLabel(1,"Target",-1);

  int i=0;
  TransitionList::iterator first, last = transitionList.end();
  for (first = transitionList.begin(); first != last; ++first, ++i ) 
  {
     table.setText(i, 1, first->getTarget());
     table.setText(i, 0, first->getMessage());
  }
  table.setGeometry(10,10,400,200);
  // cancel button //
  QPushButton cancelButton("Cancel", &dialog);
  cancelButton.setGeometry(40,210,60,25);
  connect(&cancelButton, SIGNAL(clicked()), &dialog, SLOT(reject()));

  // OK button //
  QPushButton okButton("OK", &dialog);
  okButton.setGeometry(130,210,60,25);  
  connect(&okButton,     SIGNAL(clicked()), &dialog, SLOT(accept()));

  // OK pressed in dialog ? //
  if (dialog.exec()) {
    getDocument().delTransitionsFrom(nameOfPattern);
    for (int j = 0; j < table.numRows(); ++j)
    {
      QString message  = table.text(j, 0);
      QString target = table.text(j, 1);
      if (!message.isEmpty() && 
	  !target.isEmpty()) {
	if (!getDocument().addTransition(nameOfPattern, message, target))
	  QMessageBox::warning(this, 
			   "Rename Transition", 
			   "Target " + target + " does not exist.\n" + 
			   "Transition " + message + " dropped.");
      }
    }
    getView().updateContents(getView().contentsX(), 
			     getView().contentsY(),
			     getView().contentsWidth(), 
			     getView().contentsHeight());

  } 
} 
