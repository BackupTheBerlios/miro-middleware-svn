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
#ifndef ConstraintArbiterViewerTask_h
#define ConstraintArbiterViewerTask_h

#include <ace/Task.h>

#include "ConstraintArbiterViewer.h"
#include <qapplication.h>
#include <qwidget.h>
#include "VelocitySpace.h"

namespace Miro
{
  class ConstraintArbiterViewerTask : public ACE_Task<ACE_MT_SYNCH> {
    typedef ACE_Task<ACE_MT_SYNCH> Super;    
  public:
    ConstraintArbiterViewerTask(VelocitySpace *);
    ~ConstraintArbiterViewerTask();
    int svc();
    int open(void * = 0);
    int temp;
    QApplication winArbApp_;
    ConstraintArbiterViewer winArbView_;

  };
};
#endif
