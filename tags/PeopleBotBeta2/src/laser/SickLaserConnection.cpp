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


#include <ace/Reactor.h>

#include "SickLaserConnection.h"
#include "Parameters.h"

#include <termios.h>
#include <linux/serial.h>

#include <iostream>
#include <cmath>

#include "miro/Exception.h"

#undef DEBUG

namespace Miro
{
  using std::string;

  /***************************************************************************
   *
   *  Laser Implementation 
   *
   ***************************************************************************/

  LaserConnection::LaserConnection(ACE_Reactor * _reactor,
				   RangeSensorImpl& _rangeSenorI,
				   LaserStatistic * _laserStatistic) :
    Log(WARNING,"LaserConnection"),
    reactor_(_reactor),
    parameters_(*::Laser::Parameters::instance()),
    laserStatistic_(_laserStatistic),
    laserTask_(new LaserTask(*this, _rangeSenorI, _laserStatistic)),
    laserEvent_(new LaserEvent(laserTask_, _laserStatistic)),
    laserPollTask_(new LaserPollTask(*this, *laserEvent_, _laserStatistic)),
    fd(0),
    selectHandlerId(-1),
    syncLaserScan(),
    syncLaserScanCond(syncLaserScan),
    syncModeChange(),
    syncModeChangeCond(syncModeChange),
    syncStatus(),
    syncStatusCond(syncStatus)
  {
    // open the device
    fd = open(parameters_.device.c_str(), O_RDWR);
    if (fd < 0)
      throw Miro::Exception("LaserConnection::LaserConnection: Failed to open device.\nPropably running on the wrong machine?");

    // save current port settings 
    if (tcgetattr(fd,&oldtio) < 0) {
      throw Miro::CException(errno, "LaserConnection:LaserConnection:tcgetattr "+string(strerror(errno)) );
    }

#ifdef THREADEDWORKING
    // start the thread handling (complete) messages
    laserTask->open(NULL);
#endif

    // register handlers at the reactor
    // we need a handler to listen to the asynchronous file handler
    // of the laser device, this handles single characters and composes
    // messages which it passed on to laserTask
    //
    selectHandlerId = 
      reactor_->register_handler(fd, laserEvent_, ACE_Event_Handler::READ_MASK);
    if (selectHandlerId == -1)
      throw Miro::Exception("LaserConnection::LaserConnection: Failed to register handler for laser file descriptor.");
  }

  LaserConnection::~LaserConnection()
  {
    log(WARNING, "Destructing LaserConnection.");

    // before this destructor is called
    // do not delete this thread manager of ACE needs it
    //delete laserTask;
    //delete laserPollTask;

    log(WARNING, "destructor done.");
  }

  void 
  LaserConnection::stopTasks() 
  {
    serial_struct ss;

    log(WARNING, "entered stoptasks.");

    if (parameters_.continousMode) {
      // stop the laser from sending stuff all the time
      setPollingMode();
    } else {
      laserPollTask_->cancel();
      log(INFO, " laserPollTask exited.");
    }

    // Stop hardware triggered communication
    // no more calls to event, and therefore task
    if (selectHandlerId != -1)
      reactor_->remove_handler(selectHandlerId);
    log(WARNING, "removed fd handler.");

#ifdef THREADEDWORKING
    // Insert a HANGUP message block
    // to tell the thread to exit svc
    message = new ACE_Message_Block(0,0);
    message->msg_type (ACE_Message_Block::MB_HANGUP);
    laserTask->putq (message);
    log(WARNING, "sent hangup to laserTask.");
#endif

    syncModeChangeCond.broadcast();
    syncStatusCond.broadcast();

#ifdef THREADEDWORKING
    // Wait for the threads of the Task to exit.
    // It is rather rude to let the Task go out of scope without doing this first.  
    laserTask->wait ();
    log(WARNING, "laserTask exited.");
#endif

    // restore settings and 
    // close file descriptor
    // get serial info
    if ( ioctl(fd,TIOCGSERIAL,&ss) < 0 ) {
      log(ERROR,"stopTasks: " + string(strerror(errno)));
    }
    ss.custom_divisor = 0;
    ss.baud_base = 115200;
    ss.flags |= ASYNC_LOW_LATENCY;
    // set serial info
    if ( ioctl(fd,TIOCSSERIAL,&ss) < 0 ) {
      log(ERROR,"stopTasks: " + string(strerror(errno)));
    }
    
    if (tcsetattr(fd,TCSANOW,&oldtio) < 0) {
      throw Miro::CException(errno, "LaserConnection:~LaserConnection:tcsetattr "+string(strerror(errno)) );
    } 
    if (fd >= 0)
      ::close(fd);

    // signal everything we have, to make hanging threads run free
    syncModeChange.release();
    syncStatus.release();
    laserEvent_->syncMutex.release(); 
    laserEvent_->stateMutex.release();
    laserEvent_->ackNackCond.broadcast();
    laserEvent_->ackNackMutex.release();
 
  }

  /**
   *  blocks until it gets an Ack or an Nack from the laser
   *  TODO add timeout
   *  Return value is true for ACK, false for NACK or timeout
   */
  bool 
  LaserConnection::waitAckNack(const ACE_Time_Value &timeout) 
  {
    ACE_Time_Value abstimeout = ACE_OS::gettimeofday();
    Guard guard(laserEvent_->ackNackMutex);

    abstimeout += timeout;
    // if it times out wait returns -1 (and sets errno to ETIME
    // we return false, as the result is the same: resend message
    if (laserEvent_->ackNackCond.wait(&abstimeout)<0) {
      if (laserStatistic_)
	laserStatistic_->timeouts++;
      laserEvent_->ackNack = false;
      log(WARNING, "waitAckNack: timed out.");
    }

    return laserEvent_->ackNack;
  }

  void
  LaserConnection::sendRaw(const Miro::LaserMessage &lm) 
  {
    ACE_Time_Value now,end;
    static const ACE_Time_Value STXDELAY(0,50);
    int i,n;

    log(INFO, "LaserConnection::sendRaw: Sending message to laser. ");
#ifdef DEBUG
    cerr << lm << endl;
#endif

    i = 0;
    n = 0;
  
    //
    // send STX first
    //
    // but make first shure laser is not too busy sending us a paket
    //

    laserEvent_->syncMutex.acquire();
    do {
      n=write(fd, lm.bufferAddress(), 1);
    } while (n == 0);
  
    if (n<0) {
      log(ERROR, "sendRaw: error writing STX");
      throw Miro::CException(errno, "sendRaw: error writing STX");
    }
    //
    // then pause a short time (50usecs) but not longer than 6msecs !!
    end = ACE_OS::gettimeofday();
    end += STXDELAY; 
    now = ACE_OS::gettimeofday();
    while (now < end)
      now = ACE_OS::gettimeofday();
    
    //
    // and write the rest
    //
    while (n<lm.bufferSize()) {
      i = write(fd, lm.bufferAddress() + n, lm.bufferSize() - n);    
      if ( i<0 ) {
	laserEvent_->syncMutex.release();  
	throw Miro::CException(errno, "LaserClass::sendRaw: error in write");
      } else if (i==0) {
	laserEvent_->syncMutex.release();  
	throw Miro::CException(errno, "LaserClass::sendRaw: write wrote 0, should be blocking");
      }
      n+=i;
    }
    laserEvent_->syncMutex.release();  

#ifdef DEBUG
    cerr << "LaserClass::sendRaw: wrote "<< n << " bytes, awaiting ACK/NACK" << endl;
#endif
  
  }

  bool 
  LaserConnection::send(const Miro::LaserMessage &lm, int retries)
 {
    static const ACE_Time_Value TIMEFORACK(1,0);
    static const ACE_Time_Value RETRYDELAY(2,0);

    bool ok;

    do {

      sendRaw(lm);

      ok = waitAckNack( TIMEFORACK );

      if ( retries != -1 ) {
	--retries;
	if (retries < 0)
	  retries = 0;
      }
      if ((!ok) && (retries != 0)) {
	// we have to wait at least 30msecs before we are allowed to retry
	ACE_OS::sleep( RETRYDELAY );
	log(WARNING, "LaserConnection::send: retring...");
      }
    } while ((!ok) && (retries != 0));
  
    return ok;
  }

  void 
  LaserConnection::setLaserBaudrate(int baudrate) 
  {
    char c[10]=" SICK_LMS";
    Miro::LaserMessage lm;
    ACE_Guard<ACE_Thread_Mutex> guard(syncModeChange);
    ACE_Time_Value abstimeout;
    ACE_Time_Value timeout(5,0); // 5 seconds for mode change acknowledge
  
    c[0] = 0;
    lm.fillMessage(0,LO_CHANGEMODE,9,c);

    log(INFO, "setLaserBaudrate: Set scanner to admin mode.");
    if (!send(lm,5))
      throw Miro::Exception("LaserConnection::setLaserBaudrate: could not set setup mode");
  
    modeChanged = false;

    abstimeout = ACE_OS::gettimeofday();
    abstimeout += timeout;

    if (syncModeChangeCond.wait(&abstimeout) < 0 )
      throw Miro::Exception("LaserConnection::setLaserBaudrate: laser failed to acknowledge mode change at all");
    if (!modeChanged) 
      throw Miro::Exception("LaserConnection::setLaserBaudrate: laser failed to acknowledge mode change");

    if (baudrate == 9600) { 
      c[0]= 0x42; 
    }
    else if (baudrate == 19200) {
      c[0]= 0x41; 
    }
    else if (baudrate == 38400) {
      c[0]= 0x40; 
    }
    else if (baudrate == 500000)
    { 
      c[0]= 0x48; 
    }
    else {
      throw Miro::Exception("LaserConnection::setLaserBaudrate: baudrate not supported."); 
    }

    lm.fillMessage(0,LO_CHANGEMODE,9,c);
    log(INFO, "setLaserBaudrate: changing baudrate ");
    if (!send(lm,5))
      throw Miro::Exception("LaserConnection::setLaserBaudrate: could not send baudrate request");
    modeChanged = false;

    abstimeout = ACE_OS::gettimeofday();
    abstimeout += timeout;

    if (syncModeChangeCond.wait(&abstimeout) < 0 )
      throw Miro::Exception("LaserConnection::setLaserBaudrate: laser failed to acknowledge baudrate chagne change at all ");
    if (!modeChanged) 
      throw Miro::Exception("LaserConnection::setLaserBaudrate: laser failed to acknowledge mode change");
  
    // now the answer will be in the new baudrate
    // correct at this place !! although not intuitive !!
    setBaudrate(baudrate);

    // wait 5 seconds
    ACE_OS::sleep( ACE_Time_Value(5,0) ); 

    c[0]= 0x25; // polling mode
    lm.fillMessage(0,LO_CHANGEMODE,1,c); // no password needed
    log(INFO, "setLaserBaudrate: reset to normal operation");
    if (!send(lm,5))
      throw Miro::Exception("LaserConnection::setLaserBaudrate: could not set mode for normal operation ");
    modeChanged = false;

    abstimeout = ACE_OS::gettimeofday();
    abstimeout += timeout;
    if (syncModeChangeCond.wait(&abstimeout) < 0 )
      throw Miro::Exception("LaserConnection::setLaserBaudrate: laser failed to acknowledge mode change ");

    if (!modeChanged) 
      throw Miro::Exception("LaserConnection::setLaserBaudrate: laser failed to acknowledge mode change");
    log(INFO, "setLaserBaudrate: done.");
  }

  bool
  LaserConnection::checkConnection() 
  {
    Miro::LaserMessage lm(0,LO_REQ_STATUS,0,NULL);
    int failure;
    ACE_Guard<ACE_Thread_Mutex> guard(syncStatus);
    ACE_Time_Value abstimeout;
    ACE_Time_Value timeout(5,0); // 5 seconds for mode change acknowledge

    failure = 0;
    while (failure < 10) {
      if (send(lm,0)) {
	abstimeout = ACE_OS::gettimeofday();
	abstimeout += timeout;
	if (syncStatusCond.wait(&abstimeout) >= 0)
	  break;
      }

      ++failure;
    }
    return (failure < 10);
  }

  void 
  LaserConnection::setBaudrate(int baudrate) 
  {
    unsigned short br;
    serial_struct ss;

    if (baudrate == 500000) {

      if (parameters_.stdCrystal) 
	throw Miro::Exception("LaserConnection::setBaudrate: baudrate only supported with nonstandard crystal.");

      // get serial info
      if ( ioctl(fd,TIOCGSERIAL,&ss) < 0 ) {
	log(ERROR,"setBaudrate: " + string(strerror(errno)));
      }
      ss.custom_divisor = 0;
      ss.baud_base = 115200;
      ss.flags |= ASYNC_LOW_LATENCY;
      // set serial info
      if ( ioctl(fd,TIOCSSERIAL,&ss) < 0 ) {
	log(ERROR,"setBaudrate: " + string(strerror(errno)));
      }
    }

    if (baudrate == 500000) {
      br = B57600; 
    }
    else if (baudrate == 38400) {
      br = B38400; 
    }
    else if (baudrate == 19200) {
      br = B19200; 
    }
    else if (baudrate == 9600) {
      br = B9600; 
    }
    else
      throw Miro::CException(errno, "LaserConnection::setBaudrate: impossible baudrate attempted." );

    // set raw mode and correct blocking  
    memset(&newtio,0,sizeof(newtio));
    newtio.c_cflag = br | CS8 | CLOCAL | CREAD;
    newtio.c_iflag = 0;
    newtio.c_oflag = 0;
  
    // set input mode (non-canonical, no echo,...) 
    newtio.c_lflag = 0;
  
    // standard is 1 char minimum and blocking
    newtio.c_cc[VTIME]    = 1;   // inter-character timer unused 
    newtio.c_cc[VMIN]     = 1;   // blocking read until 5 chars received 
  
    if (tcflush(fd, TCIFLUSH) < 0)
      throw Miro::CException(errno, "LaserConnection::setBaudrate:tcflush "+string(strerror(errno)) );

    if (tcsetattr(fd,TCSANOW,&newtio) < 0)
      throw Miro::CException(errno, "LaserConnection::setBaudrate:tcsetattr " + string(strerror(errno)) );

    if ((baudrate != 500000) && (!parameters_.stdCrystal)) {
      // get serial info
      if (ioctl(fd,TIOCGSERIAL,&ss) < 0 ) {
	log(ERROR,"setBaudrate: " + string(strerror(errno)));
      }
      ss.flags = ss.flags | ASYNC_SPD_CUST;
      if (baudrate == 9600) 
	ss.custom_divisor = 104;
      else if (baudrate == 19200)
	ss.custom_divisor = 52;
      else if (baudrate == 38400)
	ss.custom_divisor = 26;
      
      ss.baud_base = 1000000;
      ss.flags |= ASYNC_LOW_LATENCY;
      // set serial info
      if (ioctl(fd,TIOCSSERIAL,&ss) < 0) {
	log(ERROR,"setBaudrate: " + string(strerror(errno)));
      }
    }
  }

  void 
  LaserConnection::initHardware() 
  {
    // actual baudrate set/detected
    int actual, destinationRate;
    int allowedRates[]={500000,9600,19200,38400};
    //
    // have called after all reactor stuff is set up
    // this means it CANNOT be done in our constructor 
    //

    destinationRate = parameters_.ttyParams.baudrate;

    int i=0;
    while (i<4) {
      actual = allowedRates[i];
      try {
	setBaudrate(actual);
      } catch (Miro::Exception& m) {
	// ignore this silently, as we will try to set 500000 baud even if stdcrystal is true
	++i;
	continue;
      }
      log(INFO, "initHardware: trying connect with ",actual);
      ACE_OS::sleep( ACE_Time_Value(2,0) ); 
      if (checkConnection()) {
	log(WARNING, "initHardware: connect succeeded for ",actual);
	break;
      } else {
	log(WARNING, "initHardware: connect failed for ",actual);
	++i;
      }

    }
    if (i==4) {
      log(FATAL_ERROR, "initHardware: without connection I can't program another baudrate");
      throw Miro::Exception("LaserConnection::initHardware could not determine laser baudrate.");
    }

    log(INFO, "initHardware: connect ok.");

    // change baudrate on laser if necessary ?
    // this will also adapt local baudrate !!
    if (destinationRate != actual) {
      log(INFO, "initHardware: Trying to program new baudrate.");
      setLaserBaudrate(destinationRate);
      log(INFO, "initHardware: New baudrate set.");
    }

    // wait 5 seconds
    ACE_OS::sleep( ACE_Time_Value(5,0) ); 

    log(INFO, "initHardware: Final check of connection.");
    // check whether we succeeded
    if (!checkConnection()) {
      throw Miro::Exception("LaserConnection::initHardware could not connect to laser.");
    }

    if (parameters_.continousMode) {
      setContinousMode();
    } else {
      setPollingMode();
      laserPollTask_->open(0);
    }
  }

  //
  // attention, exceptions are thrown to the corba client if called from there !!
  //
  void
  LaserConnection::setContinousMode() 
  {
    char c=0x24;
    Miro::LaserMessage lm( 0, LO_CHANGEMODE, 1, &c );
    ACE_Time_Value abstimeout;
    ACE_Time_Value timeout(5,0); // 5 seconds for mode change acknowledge
    ACE_Guard<ACE_Thread_Mutex> guard(syncModeChange);
    
    
    if (!send(lm,5))
      throw Miro::Exception("LaserConnection::setContinousMode: could not set mode in laser");
    
    log(INFO, "setContinousMode: mode change send, waiting for answer.");;
    
    modeChanged = false;
    
    abstimeout = ACE_OS::gettimeofday();
    abstimeout += timeout;
    
    if (syncModeChangeCond.wait(&abstimeout) < 0 )
      throw Miro::Exception("LaserConnection::setContinousMode: laser failed to acknowledge mode change");
    
  }

  void
  LaserConnection::setPollingMode() 
  {
    char c=0x25;
    Miro::LaserMessage lm( 0, LO_CHANGEMODE, 1, &c );
    ACE_Time_Value abstimeout;
    ACE_Time_Value timeout(5,0); // 5 seconds for mode change acknowledge
    ACE_Guard<ACE_Thread_Mutex> guard(syncModeChange);


    if (!send(lm,5))
      throw Miro::Exception("LaserConnection::setPollingMode: could not set mode in laser");

    log(INFO, "setPollingMode: mode change send, waiting for answer.");

    modeChanged = false;

    abstimeout = ACE_OS::gettimeofday();
    abstimeout += timeout;

    if (syncModeChangeCond.wait(&abstimeout) < 0 )
      throw Miro::Exception("LaserConnection::setPollingMode: laser failed to acknowledge mode change");
  }
};
