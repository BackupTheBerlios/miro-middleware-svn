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
#ifndef WindowArbiterViewer_h
#define WindowArbiterViewer_h

#include <qwidget.h>
#include "DynamicWindow.h"
#include <qpixmap.h>
#include <qpainter.h>
#include <qtimer.h>

namespace Miro
{
  class WindowArbiterViewer : public QWidget {
    
  public:
    WindowArbiterViewer(DynamicWindow *, QWidget *parent=0, const char *name=0);
    void paintDynamicWindow();
  protected:
    DynamicWindow * dynamicWindow_;
    void paintEvent(QPaintEvent*);
    void timerEvent(QTimerEvent*);
    QPixmap pixmap_;
    QPainter painter_;
  };
};
#endif
