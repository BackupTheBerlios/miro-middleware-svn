// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2001, 2002, 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "ConstraintArbiterViewer.h"
#include "VelocitySpace.h"

#include <qpainter.h>
#include <qpen.h>
#include <qcolor.h>
#include <qpixmap.h>
#include <qtimer.h>

namespace Miro 
{
  namespace BAP
  {
    
    ConstraintArbiterViewer::
    ConstraintArbiterViewer(VelocitySpace const * _velocitySpace, 
			    QWidget *parent, const char *name) :
      QWidget(parent, name),
      velocitySpace_(_velocitySpace) 
    {
      
      int dim = 2*(velocitySpace_->maxVelocity_/velocitySpace_->spaceResolution_)+1;
      
      startTimer(100);
      
      resize(dim + 20, dim + 20);
      pixmap_.resize(dim, dim);
    }
    
    void
    ConstraintArbiterViewer::timerEvent(QTimerEvent *) 
    {
      paintVelocitySpace();
    }
    
    void
    ConstraintArbiterViewer::paintVelocitySpace() 
    {
      int dim = 2*(velocitySpace_->maxVelocity_/velocitySpace_->spaceResolution_)+1;
      unsigned char pointValue;
      
      painter_.begin(&pixmap_);
      pixmap_.fill(Qt::white);
      QColor color;
      for(int l_index=velocitySpace_->minDynWinLeft_; l_index<=velocitySpace_->maxDynWinLeft_; l_index++) {
	for(int r_index=velocitySpace_->minDynWinRight_; r_index<=velocitySpace_->maxDynWinRight_; r_index++) {
	  
	  pointValue = velocitySpace_->velocitySpace_[l_index][r_index];
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
    
    void
    ConstraintArbiterViewer::paintEvent(QPaintEvent*)
    {
      bitBlt(this, 10, 10, &pixmap_);
    }
  }
}
