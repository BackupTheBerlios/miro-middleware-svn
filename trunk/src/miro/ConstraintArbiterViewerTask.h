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

#include "ConstraintArbiterViewer.h"
#include "VelocitySpace.h"
#include "Thread.h"

#include <qapplication.h>
#include <qwidget.h>

namespace Miro
{
  class ConstraintArbiterViewerTask : public Thread
  {
    //! Type defining the super class.
    typedef Thread Super;    

  public:
    ConstraintArbiterViewerTask(VelocitySpace const * _velocitySpace);
    ~ConstraintArbiterViewerTask();

    //--------------------------------------------------------------------------
    // inherited interface
    //--------------------------------------------------------------------------

    virtual int svc();
    virtual void cancel(bool _wait = true);
  
  protected:
    //! Dummy for command line parameters
    int temp;

    QApplication conArbApp_;
    ConstraintArbiterViewer conArbView_;
  };
}
#endif
