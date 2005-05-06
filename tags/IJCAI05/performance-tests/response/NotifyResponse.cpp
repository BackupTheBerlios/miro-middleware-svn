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


#include <ace/Stats.h>
#include <ace/Sample_History.h>
#include <ace/High_Res_Timer.h>

#include "idl/OdometryC.h"
#include "idl/SparrowMotionC.h"
#include "miro/StructuredPushConsumer.h"
#include "miro/IO.h"
#include "miro/Server.h"
#include "miro/TimeHelper.h"
#include "miro/Task.h"

#include <vector>
#include <cmath>

#include <stdlib.h>
#include <sys/time.h>

using std::cout;
using std::cerr;
using std::endl;
using std::flush;
using std::cin;
using std::vector;
using CosNotification::EventTypeSeq;
using CosNotification::StructuredEvent;
using CosNotifyChannelAdmin::EventChannel;
using CosNotifyChannelAdmin::EventChannel_ptr;
using CosNotifyChannelAdmin::EventChannel_var;
using Miro::Server;
using Miro::VelocityIDL;
using Miro::PositionIDL;
using Miro::MotionStatusIDL;
using Miro::Odometry;
using Miro::Odometry_ptr;
using Miro::Odometry_var;
using Miro::SparrowMotion;
using Miro::SparrowMotion_var;
using Miro::SparrowMotion_ptr;
using Miro::Guard;
using Miro::Mutex;
using Miro::Condition;

int niterations = 10;
int ticks = 0;

PositionIDL startPos;
PositionIDL current;

bool running = false;

Server * server = 0;
ACE_Sample_History * history = 0;

ACE_hrtime_t test_start;
ACE_hrtime_t start;
ACE_hrtime_t now;

double rand1()
{
  return (double)rand() / (RAND_MAX + 1.0);
}

class OdometryConsumer : public Miro::StructuredPushConsumer
{
  typedef Miro::StructuredPushConsumer Super;

public:
  /** 
   *Initialization
   *
   * Connect the Consumer to the EventChannel.
   * Creates a new proxy supplier and connects to it.
   *
   * Registers for the events, that it wants to get pushed.
   */
  OdometryConsumer(CosNotifyChannelAdmin::EventChannel_ptr _ec,
		   const char * domainName);
  /** Disconnect from the supplier. */
  virtual ~OdometryConsumer();

  /** inherited IDL interface: StructuredPushSupplier methods */
  virtual void push_structured_event(const CosNotification::StructuredEvent & notification
				     ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    throw(CORBA::SystemException, CosEventComm::Disconnected);

  Mutex mutex;
  Condition condition;
};

OdometryConsumer::OdometryConsumer(EventChannel_ptr _consumerAdmin,
				   const char * domainName) :
  Super(_consumerAdmin),
  mutex(),
  condition(mutex)
{
  EventTypeSeq added;
  added.length(1);

  added[0].domain_name =  CORBA::string_dup(domainName);
  added[0].type_name = CORBA::string_dup("Odometry");

  setSubscriptions(added);
}

OdometryConsumer::~OdometryConsumer()
{
}

ACE_Time_Value t(0);
int counter = 0;


void
OdometryConsumer::push_structured_event(const StructuredEvent & notification
					ACE_ENV_ARG_DECL_NOT_USED)
  throw(CORBA::SystemException, CosEventComm::Disconnected)
{
  Guard guard(mutex);

  if (running) {
    cout << "." << flush;
    ++ticks;

    MotionStatusIDL * pMotionStatus;
    if (notification.remainder_of_body >>= pMotionStatus) {
      
      current = pMotionStatus->position;
      
      if (pMotionStatus->position.point.x != startPos.point.x ||
	  pMotionStatus->position.point.y != startPos.point.y ||
	  pMotionStatus->position.heading != startPos.heading) {
	running = false;
	now = ACE_OS::gethrtime ();
	history->sample (now - start);
	condition.broadcast();
      }
    }
    else {
      cerr << "Invalid message type." << endl;
    }
  }
}

class Task : public Miro::Task 
{
public:
  Task(OdometryConsumer * _pConsumer, 
       Odometry_ptr _pOdometry,
       SparrowMotion_ptr _pMotion);

  int svc();

protected:
  OdometryConsumer * pConsumer_;
  Odometry_var odometry_;
  SparrowMotion_var motion_;
};

Task::Task(OdometryConsumer * _pConsumer, 
	   Odometry_ptr _odometry, 
	   SparrowMotion_ptr _motion) :
  pConsumer_(_pConsumer),
  odometry_(Odometry::_duplicate(_odometry)),
  motion_(SparrowMotion::_duplicate(_motion))
{}

int
Task::svc()
{
  cout << "Task started." << endl;

  test_start = ACE_OS::gethrtime ();
  
  for (int i = 0; i < niterations; ++i) {

    motion_->setLRPower(0, 0);
    
    ACE_OS::sleep(ACE_Time_Value(1, (int)floor(rand1() * 100000.)));
    startPos = odometry_->getPosition();
    cout << "start position:" << startPos << endl;
    
    Guard guard(pConsumer_->mutex);
    cout << "Got mutex." << endl;
    
    running = true;
    start = ACE_OS::gethrtime ();
    
    motion_->setLRPower(10000, 10000);
    pConsumer_->condition.wait();
  }
  motion_->setLRPower(0, 0);

  cout << "client ended" << endl;
  server->shutdown();

  return 0;
}

int main(int argc, char *argv[])
{
  cout << "Initialize server daemon." << flush;
  server = new Server(argc, argv);

  if (argc > 1)
    niterations = ACE_OS::atoi(argv[1]);

  history = new ACE_Sample_History(niterations);
    
  try {
    // The odometry interface
    Odometry_var odometry = server->resolveName<Odometry>("Odometry");
    SparrowMotion_var motion = server->resolveName<SparrowMotion>("Motion");

    // The one channel that we create using the factory.
    EventChannel_var ec = server->resolveName<EventChannel>("EventChannel");
    
    // The consumer, that gets the events
    OdometryConsumer pushConsumer(ec.in(), server->namingContextName.c_str());

    Task task(&pushConsumer, odometry.in(), motion.in());
    
    cout << "Loop forever handling events." << endl;
    task.open(NULL);
    server->run();
    cout << "Server stoped, exiting." << endl;

    ACE_hrtime_t test_end = ACE_OS::gethrtime ();

    ACE_DEBUG ((LM_DEBUG, "test finished\n"));
    
    ACE_DEBUG ((LM_DEBUG, "High resolution timer calibration...."));
    ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();
    ACE_DEBUG ((LM_DEBUG, "done\n"));

    history->dump_samples ("HISTORY", gsf);

    ACE_Basic_Stats stats;
    history->collect_basic_stats (stats);
    stats.dump_results ("Total", gsf);
    
    ACE_Throughput_Stats::dump_throughput ("Total", gsf,
					   test_end - test_start,
					   stats.samples_count ());

    // data evaluation
    cout << "ticks: " << ticks << endl;
    cout << "end position: " << current << endl;
  }
  catch (const CORBA::Exception & e) {
    cerr << "Uncaught CORBA exception: " << e << endl;
    return 1;
  }
  catch (...) {
    cerr << "Uncaught exception!" << endl;
    return 1;
  }

  delete history;
  delete server;
  
  return 0;
}
