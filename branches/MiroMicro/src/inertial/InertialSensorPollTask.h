#ifndef InertialSensorPollTask_h
#define InertialSensorPollTask_h

#include "miro/Thread.h"
#include "InertialSensorImpl.h"
#include "XsensInertialSensorConnection.h"
#include "InertialSensorEventHandler.h"

#include "idl/InertialSensorEventC.h"

// xbus includes
#include <sys/ioctl.h>
#include "Xbus.h"

namespace Inertial
{
  class EventHandler;
  class XsensInertialSensorConnection;
  class InertialSensorImpl;
  class InertialParameters;

  class InertialSensorPollTask : public Miro::Thread
  {
    typedef Thread Super;
  public:
  InertialSensorPollTask( XsensInertialSensorConnection& _connection, EventHandler& _event, InertialSensorImpl& _i_impl );

    virtual ~InertialSensorPollTask();

    virtual int svc ();

  protected:

    XsensInertialSensorConnection&        inertial_;
    EventHandler&                         event_;
    InertialSensorImpl&                   i_impl_;
    const ::Inertial::InertialParameters& parameters_;
  };
}

#endif
