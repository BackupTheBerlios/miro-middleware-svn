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
#ifndef WindowArbiterViewerTask_h
#define WindowArbiterViewerTask_h

#include <ace/Task.h>

#include "WindowArbiterViewer.h"
#include <qapplication.h>
#include <qwidget.h>
#include "DynamicWindow.h"

namespace Miro
{
  class WindowArbiterViewerTask : public ACE_Task<ACE_MT_SYNCH> {
    typedef ACE_Task<ACE_MT_SYNCH> Super;    
  public:
    WindowArbiterViewerTask(DynamicWindow *);
    ~WindowArbiterViewerTask();
    int svc();
    int open(void * = 0);
    int temp;
    QApplication winArbApp_;
    WindowArbiterViewer winArbView_;

  };
};
#endif
