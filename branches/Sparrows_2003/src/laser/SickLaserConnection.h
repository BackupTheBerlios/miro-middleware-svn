// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef laserConnection_hh
#define laserConnection_hh

// #include "miro/DevConnection.h"
#include "miro/RangeSensorImpl.h"
#include "miro/LaserC.h"

#include "SickLaserMessage.h"
#include "SickLaserTask.h"
#include "SickLaserPollTask.h"
#include "SickLaserEvent.h"

#include <ace/Event_Handler.h>
#include <ace/Synch.h>

#include <exception>
#include <string>

#include <termios.h>

namespace Laser
{
  class Parameters;
};

namespace Miro 
{
  // forward declarations
  class RangeSensorImpl;

  class LaserTask;
  class LaserPollTask;
  class LaserMessage;
  class LaserEvent;


  /**
   * LaserConnection
   */
  class LaserConnection :  // public devConnection, 
    public Log
  {
  public:
    //! Constructor
    LaserConnection(ACE_Reactor * _reactor_,
		    RangeSensorImpl& _rangeSenorI,
		    LaserStatistic * _laserStatistic);

    //! Destructor
    virtual ~LaserConnection();

    /**
     * send sth to the laser, 
     * does not bother for ACK, but aquires syncMutex
     */
    void sendRaw(const LaserMessage &lm);

    /**
     * send sth to the laser, make at maximum <retries> retries
     * retries == -1 means, try forever
     * false means send failed, true means ok
     */
    bool send(const LaserMessage &lm, int retries = -1);

    /**
     * initialize and program baudrate of laser
     * has to be calles when the reactor is fully set up,
     * (from LaserServer::LaserServer, at the moment)
     */
    void initHardware();

    //! Stop all action of tasks and deregister event handlers.
    void stopTasks();

  protected:
    //! Make laser send every scan, blocks until mode is changed.
    void setContinousMode();

    //! Make laser send scans only on request, blocks until mode is changed.
    void setPollingMode();

    /**
     *  blocks until it gets an ACK or an NACK from the laser
     *  TODO add functionality
     *  Return value is true for ACK, false for NACK or timeout
     */
    bool waitAckNack(const ACE_Time_Value &timeout);
  
    /**
     * tries to send pakets to the laser, (status request)
     * if the paket is acknowledged it returns true,
     * does five retries at the moment
     */
    bool checkConnection();

    /**
     * programs the serial io device to the requested baudrate
     * (this does program the laser, but only the host computers
     * serial line), posssible are 500000, 38400, 19200, 9600
     */
    void setBaudrate(int baudrate);

    /**
     * this tries to program the laser to the requested baudrate
     * baudrate may be 9600, 19200, 38400, 50000
     */
    void setLaserBaudrate(int baudrate);
  
    /**
     * for event handling (file descriptors)
     * the reactor listens for data sent by the laser
     */
    ACE_Reactor* reactor_;   

    const ::Laser::Parameters& parameters_;
    //! prt to the central error statistic object
    LaserStatistic * laserStatistic_;


    /**
     * The laserTask is a group of threads (or only one) that handles 
     * complete pakets received by the laser. It does not have to bother 
     * for protocol issues. It is seperate thread, which discouples the
     * hardware and protocol handling from the actual work to do.
     */
    LaserTask * laserTask_;

    /**
     * The laserEvent decodes the communication protocol and reassembles the
     * pakets which maybe fragmented into several portions depending on
     * timing and buffers. This is the object which is called by the
     * reactor when there is activity on the filE descriptor
     */
    LaserEvent * laserEvent_;

    //! Poll the data
    LaserPollTask * laserPollTask_;

    /**
     * file descriptor of the laser interface
     */
    int fd;

    /**
     * id of the handler for the filedescriptor, 
     * when getting registered with the ACE_Reactor
     *
     */
    int selectHandlerId;

    /**
     * termio settings of the laser interface, the old settings are 
     * restored before
     * we exit
     */
    struct termios oldtio, newtio;

  public:
    /**
     * mutex for synchronization on access to mode with LaserTask
     */
    Mutex syncLaserScan;

    /**
     * condition variable for synchronization on update of mode by LaserTask
     */
    Condition syncLaserScanCond;


    /**
     * mutex for synchronization on access to mode with LaserTask
     */
    Mutex syncModeChange;

    /**
     * condition variable for synchronization on update of mode by LaserTask
     */
    Condition syncModeChangeCond;


    /**
     * set if the laser acknowledged the mode change
     */
    bool modeChanged;

    /**
     * mutex for synchronization on status requests
     */
    Mutex syncStatus;

    /**
     * condition variable for synchronization on status requests
     */
    Condition syncStatusCond;
  };
};
#endif




