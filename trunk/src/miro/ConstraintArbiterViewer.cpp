#include "ConstraintArbiterViewer.h"
#include "VelocitySpace.h"
#include <qpainter.h>
#include <qpen.h>
#include <qcolor.h>
#include <qpixmap.h>
#include <qtimer.h>
#include <iostream>

namespace Miro {

  ConstraintArbiterViewer::ConstraintArbiterViewer(VelocitySpace * _VelocitySpace, QWidget *parent, const char *name)
    : QWidget(parent, name), VelocitySpace_(_VelocitySpace) {

    startTimer(100);

    resize(221,221);
    pixmap_.resize(201, 201);
  }

  void ConstraintArbiterViewer::timerEvent(QTimerEvent *) {
    paintVelocitySpace();
  }

  void ConstraintArbiterViewer::paintVelocitySpace() {
    int right, left;
    unsigned char pointValue;

    painter_.begin(&pixmap_);
    pixmap_.fill(Qt::white);
    QColor color;
    for(left=VelocitySpace_->minLeft_; left<=VelocitySpace_->maxLeft_; left++) {
      for(right=VelocitySpace_->minRight_; right<=VelocitySpace_->maxRight_; right++) {

        pointValue = VelocitySpace_->velocitySpace_[left+100][right+100];
	if(pointValue==0)
	  color.setRgb(255, 0, 0);
	else
	  color.setRgb(255 - pointValue, pointValue, 0);

	painter_.setPen(color);
	painter_.drawPoint(100 + right, 100 - left);

      }
    }

    painter_.setPen(Qt::black);
    painter_.drawRect(0, 0, 201, 201);
    painter_.moveTo(0, 100);
    painter_.lineTo(200, 100);
    painter_.moveTo(100, 0);
    painter_.lineTo(100, 200);
	
    painter_.end();
    repaint(FALSE);

  }
  
  void ConstraintArbiterViewer::paintEvent(QPaintEvent*) {
    bitBlt(this, 10, 10, &pixmap_);
  }
  
};
