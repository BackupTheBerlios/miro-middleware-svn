#include "ConstraintArbiterViewer.h"
#include "ConstraintArbiterViewerTask.h"
#include <qpainter.h>
#include <qpixmap.h>
#include "VelocitySpace.h"

namespace Miro {

  ConstraintArbiterViewerTask::ConstraintArbiterViewerTask(VelocitySpace * _VelocitySpace) :
    Super(),
    temp(0),
    conArbApp_(temp, (char **)NULL),
    conArbView_(_VelocitySpace)
  {
    conArbView_.setCaption("Constraint Arbiter Viewer");
    conArbView_.setBackgroundColor(Qt::white);
    conArbApp_.setMainWidget(&conArbView_);
    conArbView_.show();
  }
  
  ConstraintArbiterViewerTask::~ConstraintArbiterViewerTask() {
    /* left empty */
  }
  
  int ConstraintArbiterViewerTask::svc() {
    return conArbApp_.exec();
  }
  
  int ConstraintArbiterViewerTask::open(void *) {
    return activate();
  }
    
};
