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
#include "ConstraintArbiterViewerTask.h"
#include "VelocitySpace.h"

#include <qpainter.h>
#include <qpixmap.h>

namespace Miro 
{
  namespace BAP
  {
    ConstraintArbiterViewerTask::
    ConstraintArbiterViewerTask(VelocitySpace const * _velocitySpace) :
      Super(),
      temp(0),
      conArbApp_(temp, (char **)NULL),
      conArbView_(_velocitySpace)
    {
      conArbView_.setCaption("Constraint Arbiter Viewer");
      conArbView_.setBackgroundColor(Qt::white);
      conArbApp_.setMainWidget(&conArbView_);
      conArbView_.show();
    }
  
    ConstraintArbiterViewerTask::~ConstraintArbiterViewerTask() 
    {
      /* left empty */
    }
    
    int ConstraintArbiterViewerTask::svc() {
      return conArbApp_.exec();
    }
    
    void 
    ConstraintArbiterViewerTask::cancel(bool _wait)
    {
      conArbApp_.exit();
      Super::cancel(_wait);
    }   
  }
}
