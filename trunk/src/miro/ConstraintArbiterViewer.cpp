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

    int dim = 2*(VelocitySpace_->maxVelocity_/VelocitySpace_->spaceResolution_)+1;
	    
    startTimer(100);

    resize(dim + 20, dim + 20);
    pixmap_.resize(dim, dim);
  }

  void ConstraintArbiterViewer::timerEvent(QTimerEvent *) {
    paintVelocitySpace();
  }

  void ConstraintArbiterViewer::paintVelocitySpace() {

    int dim = 2*(VelocitySpace_->maxVelocity_/VelocitySpace_->spaceResolution_)+1;
    unsigned char pointValue;

    painter_.begin(&pixmap_);
    pixmap_.fill(Qt::white);
    QColor color;
    for(int l_index=VelocitySpace_->minDynWinLeft_; l_index<=VelocitySpace_->maxDynWinLeft_; l_index++) {
      for(int r_index=VelocitySpace_->minDynWinRight_; r_index<=VelocitySpace_->maxDynWinRight_; r_index++) {

        pointValue = VelocitySpace_->velocitySpace_[l_index][r_index];
	if(pointValue==0)
	  color.setRgb(255, 0, 0);
	else
	  color.setRgb(255 - pointValue, pointValue, 0);

	painter_.setPen(color);
	painter_.drawPoint(r_index, dim - l_index);

      }
    }

    painter_.setPen(Qt::black);
    painter_.drawRect(0, 0, dim, dim);
    painter_.moveTo(0, dim/2-1);
    painter_.lineTo(dim, dim/2-1);
    painter_.moveTo(dim/2-1, 0);
    painter_.lineTo(dim/2-1, dim);
	
    painter_.end();
    repaint(FALSE);

  }
  
  void ConstraintArbiterViewer::paintEvent(QPaintEvent*) {
    bitBlt(this, 10, 10, &pixmap_);
  }
  
};
