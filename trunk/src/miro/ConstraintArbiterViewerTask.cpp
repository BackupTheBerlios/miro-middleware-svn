#include "ConstraintArbiterViewer.h"
#include "ConstraintArbiterViewerTask.h"
#include <qpainter.h>
#include <qpixmap.h>
#include "VelocitySpace.h"

namespace Miro {

  ConstraintArbiterViewerTask::ConstraintArbiterViewerTask(VelocitySpace * _VelocitySpace) :
    Super(),
    temp(0),
    winArbApp_(temp, (char **)NULL),
    winArbView_(_VelocitySpace)
  {
    winArbView_.setCaption("Window Arbiter Viewer");
    winArbView_.setBackgroundColor(Qt::white);
    winArbApp_.setMainWidget(&winArbView_);
    winArbView_.show();
  }
  
  ConstraintArbiterViewerTask::~ConstraintArbiterViewerTask() {
    /* left empty */
  }
  
  int ConstraintArbiterViewerTask::svc() {
    return winArbApp_.exec();
  }
  
  int ConstraintArbiterViewerTask::open(void *) {
    return activate();
  }
    
};
