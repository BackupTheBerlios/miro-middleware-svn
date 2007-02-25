// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 1999-2005
// Department of Neuroinformatics, University of Ulm, Germany
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// $Id$
//
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
