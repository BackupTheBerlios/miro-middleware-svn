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
#ifndef ConstraintArbiterViewerTask_h
#define ConstraintArbiterViewerTask_h

#include "ConstraintArbiterViewer.h"
#include "VelocitySpace.h"
#include "miro/Thread.h"

#include <qapplication.h>
#include <qwidget.h>

namespace Miro
{
  namespace BAP
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
}
#endif
