// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef ConstraintArbiterViewer_h
#define ConstraintArbiterViewer_h

#include <qwidget.h>
#include "VelocitySpace.h"
#include <qpixmap.h>
#include <qpainter.h>
#include <qtimer.h>

namespace Miro
{
  class ConstraintArbiterViewer : public QWidget {
    
  public:
    ConstraintArbiterViewer(VelocitySpace *, QWidget *parent=0, const char *name=0);
    void paintVelocitySpace();
  protected:
    VelocitySpace * VelocitySpace_;
    void paintEvent(QPaintEvent*);
    void timerEvent(QTimerEvent*);
    QPixmap pixmap_;
    QPainter painter_;
  };
};
#endif
