#ifndef XsensInertialSensorConnection_h
#define XsensInertialSensorConnection_h


#include "InertialSensorEventHandler.h"
#include "InertialSensorImpl.h"
#include "InertialSensorConnection.h"
#include "Parameters.h"
#include "idl/InertialSensorEventC.h"

#include <exception>
#include <string>

// xbus includes
#include <sys/ioctl.h>
#include "Xbus.h"

using std::string;

namespace Inertial
{

  class InertialSensorImpl;
  class InertialSensorPollTask;
  class EventHandler;
  class InertialParameters;

  class XsensInertialSensorConnection
  {

  public:
    XsensInertialSensorConnection(ACE_Reactor & _reactor, InertialSensorImpl & _i_impl );

    virtual ~XsensInertialSensorConnection();

    bool setSensorMode();

    virtual bool readData( Miro::InertialSensorEventIDL &inertial_data );

  private:
    static const string OUTPUT_MODE_CALIB_ORIENT;
    static const string OUTPUT_MODE_ORIENT;
    static const string OUTPUT_MODE_CALIB;
    static const string OUTPUT_SETTINGS_ACC_EULER;
    static const string OUTPUT_SETTINGS_ACC_GYR_EULER;
    static const string OUTPUT_SETTINGS_EULER;
    static const string OUTPUT_SETTINGS_CALIBRATED;
    static const string OUTPUT_SETTINGS_ACC;

    CXbus xbus;

    std::string deviceName_;

    int outputMode_;

    int outputSettings_;

    float fdata[18];

    unsigned char data[MAXMSGLEN];

    float orient;
  protected:

    /**
     * for event handling (file descriptors)
     * the reactor listens for data sent by the laser
     */
     ACE_Reactor* reactor_;

     EventHandler* inertial_event_;

     InertialSensorPollTask* inertial_poll_task_;

     const InertialParameters& parameters_;

  public:

    Miro::Mutex     syncInertialSensorScan;
    Miro::Condition syncInertialSensorScanCond;

  };
}

#endif
