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
#ifndef ConstraintArbiterViewer_h
#define ConstraintArbiterViewer_h

#include "VelocitySpace.h"

#include <qwidget.h>
#include <qpixmap.h>
#include <qpainter.h>
#include <qtimer.h>

namespace Miro
{
  namespace BAP
  {
    class ConstraintArbiterViewer : public QWidget 
    {
    
    public:
      ConstraintArbiterViewer(VelocitySpace const * _velocitySpace,
			      QWidget *parent=0, const char *name=0);
      void paintVelocitySpace();
      
    protected:
      void paintEvent(QPaintEvent*);
      void timerEvent(QTimerEvent*);
      
      VelocitySpace const * velocitySpace_;
      
      QPixmap pixmap_;
      QPainter painter_;
    };
  }
}
#endif
