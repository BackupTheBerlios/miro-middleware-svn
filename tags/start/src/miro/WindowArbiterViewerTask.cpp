#include "WindowArbiterViewer.h"
#include "WindowArbiterViewerTask.h"
#include <qpainter.h>
#include <qpixmap.h>
#include "DynamicWindow.h"

namespace Miro {

  WindowArbiterViewerTask::WindowArbiterViewerTask(DynamicWindow * _dynamicWindow) :
    Super(),
    temp(0),
    winArbApp_(temp, (char **)NULL),
    winArbView_(_dynamicWindow)
  {
    winArbView_.setCaption("Window Arbiter Viewer");
    winArbView_.setBackgroundColor(Qt::white);
    winArbApp_.setMainWidget(&winArbView_);
    winArbView_.show();
  }
  
  WindowArbiterViewerTask::~WindowArbiterViewerTask() {
    /* left empty */
  }
  
  int WindowArbiterViewerTask::svc() {
    return winArbApp_.exec();
  }
  
  int WindowArbiterViewerTask::open(void *) {
    return activate();
  }
    
};
