// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Nix (Nix Is eXtreme)
// 
// (c) 2001
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// $Date$
// 
//////////////////////////////////////////////////////////////////////////////

#include "miro/Server.h"
#include "miro/StructuredPushSupplier.h"
#include "miro/TimeHelper.h"
#include "miro/Exception.h"

#include <ace/Arg_Shifter.h>

#include <orbsvcs/CosNotifyChannelAdminS.h>
#include <orbsvcs/CosNotifyCommC.h>

#include <orbsvcs/Notify/Notify_EventChannelFactory_i.h>
#include <orbsvcs/Notify/Notify_Default_CO_Factory.h>
#include <orbsvcs/Notify/Notify_Default_POA_Factory.h>
#include <orbsvcs/Notify/Notify_Default_Collection_Factory.h>
#include <orbsvcs/Notify/Notify_Default_EMO_Factory.h>

#include <iostream>
#include <map>

#include <qfiledialog.h>
#include <qlayout.h>

#include "LogPlayer.h"


#ifdef DEBUG
#define DBG(x) x
#else
#define DBG(x)
#endif

using std::cout;
using std::cerr;

using CosNotifyChannelAdmin::EventChannelFactory;
using CosNotifyChannelAdmin::EventChannel;
using CosNotifyChannelAdmin::EventChannel_ptr;
using CosNotifyChannelAdmin::EventChannel_var;
using CosNotifyChannelAdmin::EventChannelFactory;
using CosNotifyChannelAdmin::EventChannelFactory_var;

#define NOTIFY_FACTORY_NAME "NotifyEventChannelFactory"

CosNaming::Name channelFactoryName;

MyWidget::MyWidget( EventChannel_ptr _ec, 
		    const std::string& _context,
		    QWidget *parent, const char *name ) :
  QWidget( parent, name ),
  supplier(_ec, _context),
  context( _context ) 
{
  QBoxLayout *topLayout = new QHBoxLayout(this, 5);

  // the menu
  QPopupMenu *fileMenu = new QPopupMenu( this );
  fileMenu->insertItem( "&Load", this, SLOT( load() ) );
  fileMenu->insertItem( "&Quit", qApp, SLOT( quit() ) );

  menuBar = new QMenuBar( this );
  menuBar->insertItem( "&File", fileMenu );    

  // ... add to layout
  topLayout->setMenuBar(menuBar);

  // the widgets
  playButton = new QPushButton( "Play", this, "play" );
  playButton->setMinimumSize(90, 30);
  stopButton = new QPushButton( "Stop", this, "stop" );
  stopButton->setMinimumSize(90, 30);
  pauseButton = new QPushButton( "Pause", this, "pause" );
  pauseButton->setMinimumSize(90, 30);

  nextButton = new QPushButton( "Next", this, "next" );
  nextButton->setMinimumSize(90, 30);
  prevButton = new QPushButton( "Prev", this, "prev" );
  prevButton->setMinimumSize(90, 30);
  
  secLabel = new QLCDNumber( 3, this, "sec" );
  centiSecLabel = new QLCDNumber( 2, this, "sec" );
  
  timeSlider = new QSlider( Horizontal, this, "time" );
  timeSlider->setRange(0, 1000);
  timeSlider->setValue(0);

  speedDial = new QDial( 10, 200, 10, 10, this, "speed" );
  speedDial->setWrapping(false);

  // ... add to the layout 
  QBoxLayout *layout1 = new QVBoxLayout(topLayout, 5);
  QGridLayout *layout2 = new QGridLayout(layout1, 2, 3, 5);

  layout2->addWidget(playButton, 0, 0);
  layout2->addWidget(stopButton, 0, 1);
  layout2->addWidget(pauseButton, 0, 2);
  layout2->addWidget(prevButton, 1, 0);
  QBoxLayout *layout3 = new QHBoxLayout(layout2);
  layout2->addWidget(nextButton, 1, 2);

  layout3->addWidget(secLabel);
  layout3->addWidget(centiSecLabel);

  layout1->addSpacing(10);
  layout1->addWidget(timeSlider);
  topLayout->addWidget(speedDial);

  // signals n slots
  connect( timeSlider, SIGNAL( valueChanged(int) ),
	   (MyWidget *)this, SLOT( changed(int) ) );
  connect( timeSlider, SIGNAL( sliderPressed() ),
	   (MyWidget *)this, SLOT( beginAction() ) );
  connect( timeSlider, SIGNAL( sliderReleased() ),
	   (MyWidget *)this, SLOT( endAction() ) );
  connect( timeSlider, SIGNAL( sliderMoved( int ) ),
	   (MyWidget *)this, SLOT( timeAction( int ) ) );

  connect( &timer, SIGNAL( timeout() ),
	   (MyWidget *)this, SLOT( step() ) );

  connect( playButton, SIGNAL( clicked() ),
	   (MyWidget *)this, SLOT( play() ) );
  connect( stopButton, SIGNAL( clicked() ),
	   (MyWidget *)this, SLOT( stop() ) );
  connect( pauseButton, SIGNAL( clicked() ),
	   (MyWidget *)this, SLOT( pause() ) );
  connect( prevButton, SIGNAL( clicked() ),
	   (MyWidget *)this, SLOT( prev() ) );
  connect( nextButton, SIGNAL( clicked() ),
	   (MyWidget *)this, SLOT( next() ) );

  connect( speedDial, SIGNAL( valueChanged( int ) ),
	   (MyWidget *)this, SLOT( speed( int ) ) );
  connect( speedDial, SIGNAL( dialPressed() ),
	   (MyWidget *)this, SLOT( beginAction() ) );
  connect( speedDial, SIGNAL( dialReleased() ),
	   (MyWidget *)this, SLOT( endAction() ) );

  action = false;
  timeSpeed = 10;
}

void
MyWidget::addExclude(const std::string& eventName)
{
  exclude.push_back(eventName);
}

void MyWidget::calcStartTime() {
  ACE_Time_Value delta = timeVector[ timeVectorPos ].first;
  delta *= timeSpeed / 10.0;
  startTime = ACE_OS::gettimeofday() - delta;
}

void MyWidget::changed( int v ) {
  if( v != (int)timeVectorPos ) {
    timeVectorPos = v;
    calcStartTime();
  }

  secLabel->display( int( timeVector[v].first.sec() ) );
  centiSecLabel->display( int( timeVector[v].first.usec() / 10000 ) );
}

void MyWidget::play() {
  calcStartTime();
  timer.start( 20 );
}

void MyWidget::stop() {
  timer.stop();
  timeSlider->setValue( 0 );  
}

void MyWidget::pause() {
  timer.stop();
}

void MyWidget::send() {
  TAO_InputCDR istr((char*)(timeVector[ timeVectorPos ].second), memoryMap.size());    
  CosNotification::StructuredEvent event;
  istr >> event;

  const char * s = event.header.fixed_header.event_type.type_name;
  std::string eventName = s;
  for (StringVector::const_iterator i = exclude.begin(); i != exclude.end(); ++i)
    if (*i == eventName)
      return;

  event.header.fixed_header.event_type.domain_name = CORBA::string_dup( context.c_str() );

  cout << timeVector[ timeVectorPos ].first.sec() << ":" 
       << timeVector[ timeVectorPos].first.usec() << "\t";    
  cout << "  Domain:  " << event.header.fixed_header.event_type.domain_name << "\t"
       << "  Type:    " << event.header.fixed_header.event_type.type_name << endl;

  supplier.sendEvent(event);
}

void MyWidget::next() {
  if( timer.isActive() )
    return;
  send();
  timeSlider->setValue( timeVectorPos+1 );
}

void MyWidget::prev() {
  if( timer.isActive() )
    return;
  timeSlider->setValue( timeVectorPos-1 );
  send();
}

void MyWidget::step() {
  if( action )
    return;
  ACE_Time_Value destTime = ACE_OS::gettimeofday() - startTime;
  while( 1 ) {
    if( timeVectorPos >= timeVector.size() )
      break;
    if( timeVector[ timeVectorPos ].first * ( timeSpeed / 10.0 ) > destTime )
      break;
    send();
    timeVectorPos++;
  }
  timeSlider->setValue( timeVectorPos );
}

void MyWidget::load() 
{
  QString s = QFileDialog::getOpenFileName( QString::null, "Logfiles (*.cdr *.log)", this );
  if ( s.isEmpty() )
    return;
  
  loadFile(s);
}

void MyWidget::beginAction() {
  action = true;
}

void MyWidget::endAction() {
  action = false;
}

void MyWidget::timeAction( int /* pos */ ) {
}

void MyWidget::speed( int s ) {
  timeSpeed = s;
  calcStartTime();
}

void MyWidget::loadFile( const char * const fileName ) {
  cerr << "loadFile: " << fileName << " " << flush;
  try {
    memoryMap.close();
    memoryMap.map( fileName );
    if (0 == memoryMap.handle())
      throw Miro::CException(errno, std::strerror(errno));    

    ACE_Time_Value timeStamp, timeBegin;
    Miro::TimeIDL timeIDL;

    TAO_InputCDR istr((char*)memoryMap.addr(), memoryMap.size());    
    
    istr >> timeIDL;
    Miro::timeC2A(timeIDL, timeBegin);

    timeVector.clear();
    int counter = 0;
    CosNotification::StructuredEvent event;
    while( istr.length() != 0 ) {
      Miro::timeC2A(timeIDL, timeStamp);
      if (timeStamp == ACE_Time_Value(0))
	break;
      timeVector.push_back(  TimePair( timeStamp - timeBegin, (char*)istr.rd_ptr() ) );

      istr >> event;
      
      if (istr.length() == 0)
	break;

      istr >> timeIDL;

      if (!(counter++ % 3000))
	cout << "." << flush;
    }
    cout << endl;

    cout << "timeVector.size(): " << timeVector.size() << endl;
  }
  catch (const CORBA::Exception & e) {
    cerr << "Uncaught CORBA exception: " << e << endl;
  }
  catch (...) {
    cerr << "Uncaught exception: " << endl;
  }  
  cerr << "done" << endl;

  timeVectorPos = 0;
  timeSlider->setRange( 0, timeVector.size() );
  timeSlider->setValue( 0 );

  this->setCaption(QString(fileName));
}

int main( int argc, char *argv[] )
{
  bool colocated = true;
  bool own = false;
  const char colocatedOpt[] = "-shared_ec";
 

  // parse optional args
 
  ACE_Arg_Shifter arg_shifter (argc, argv);
  while (arg_shifter.is_anything_left ()) {
    char *current_arg = (char *)arg_shifter.get_current ();

    if (ACE_OS::strcasecmp(current_arg, colocatedOpt) == 0) {
      arg_shifter.consume_arg();
      colocated = false;
      cout << "Using a shared event channel factory." << endl;
    } 
    else
      arg_shifter.ignore_arg ();
  }

  // Init TAO Factories
  // for the colocated event channel
  if (colocated) {
    TAO_Notify_Default_CO_Factory::init_svc();
    TAO_Notify_Default_POA_Factory::init_svc();
    TAO_Notify_Default_Collection_Factory::init_svc();
    TAO_Notify_Default_EMO_Factory::init_svc();
  }

  // Create Miro server
  DBG(cout << "Initialize server daemon." << endl);
  Miro::Server server(argc, argv);


  // parse mandatory args
  char * fileName;
  if (argc > 1) {
    fileName = argv[1];
    cout << "using file " << fileName << endl;
  }
  else {
    cout << "Please specify a file to play." << endl;
    return 1;
  }

  try {
    // For Naming Service communication
    CosNaming::Name n(1);
    n.length(1);

    // create factory and register it at the naming service
    n[0].id = CORBA::string_dup(NOTIFY_FACTORY_NAME);
    EventChannelFactory_var notifyFactory;

    if (colocated) {
      // Channel factory
      notifyFactory =
	TAO_Notify_EventChannelFactory_i::create(server.poa.in());

      if (server.rebind_) {
	// Force binding of references to make
	// sure they are always up-to-date.
	try {
	  server.initialNamingContext->unbind(n);

	  cerr << "Event channel factory was still bound in naming service." 
	       << "Unbound it." << endl;
	} catch (...) {
	  // catch anything in case there wasn't anything bound
	}
      }
      
      try {
	n[0].id = CORBA::string_dup(NOTIFY_FACTORY_NAME);
	server.initialNamingContext->bind(n, notifyFactory.in());
      }
      catch (CosNaming::NamingContext::AlreadyBound& ) {
	cerr << "Event channel factory was still bound in naming service: "
	     << n[0].id << endl
	     << "Use -MiroRebindIOR if you really want to rebind it." << endl
	     << "Use -shared_ec if you want to use it." << endl;

	throw 0;
      }
    }

    // just try to get it
    // The channel reference we need to get - somehow  
    EventChannel_var ec;
    // Channel identifer.
    CosNotifyChannelAdmin::ChannelID id;
    // Initial qos specified to the factory when creating the EC.
    CosNotification::QoSProperties initialQos;
    // Initial admin props specified to the factory when creating the EC.
    CosNotification::AdminProperties initialAdmin;
    
    n.length(2);
    n[0].id = CORBA::string_dup(server.namingContextName.c_str());
    n[1].id = CORBA::string_dup("EventChannel");

    if (server.rebind_) {
      // Force binding of references to make
      // sure they are always up-to-date.
      try {
	server.initialNamingContext->unbind(n);
	
	cerr << "EventChannel was still bound in naming service." 
	     << "Unbound it." << endl;
      } catch (...) {
	// catch anything in case there wasn't anything bound
      }
    }

    try {
      ec = server.resolveName<EventChannel>(n);

      // this shouldn't have worked, if we are colocated
      if (colocated) {
	cerr << "EventChannel already bound." << endl
	     << "Use -MiroRebindIOR if you really want to rebind it." << endl;
	throw 0;
      }
    }
    // get another
    catch (const CosNaming::NamingContext::NotFound & e) {
      // get the factory
      try {
	n.length(1);
	n[0].id = CORBA::string_dup(NOTIFY_FACTORY_NAME);
	notifyFactory = server.resolveName<EventChannelFactory>(n);
      }
      catch (const CosNaming::NamingContext::NotFound & e) {
	cerr << "No event channel factory in the naming service." << endl
	     << "Start the Notify_Service or do not use  the -shared_ec option." << endl;

	throw 0;
      }

      own = true;
      ec = notifyFactory->create_channel(initialQos, initialAdmin, id);

      try {
	n.length(1);
	n[0].id = CORBA::string_dup("EventChannel");
	server.namingContext->bind(n, ec.in());
      }
      catch (CosNaming::NamingContext::AlreadyBound& ) {
	cerr << "Event channel was still bound in naming service: "
	     << n[0].id << endl
	     << "Use -MiroRebindIOR if you really want to rebind it." << endl;
	throw(0);
      }
    }

    QApplication myApp(argc, argv);     // Create Qt application  

    // Scope operator to delimit the lifetime
    // of MyWidget, to prevent conflicts with CORBA cleanup
    {
      cerr << "construct mainwindow" << endl;
      MyWidget mainWindow(ec.in(), server.namingContextName);
      mainWindow.loadFile( fileName );

      cerr << "adding excluded events" << endl;
      for (int i = 2; i < argc; ++i) {
	cerr << argv[i] << endl;
	mainWindow.addExclude(std::string(argv[i]));
      }
    
      cerr << "set mainwidget" << endl;
      myApp.setMainWidget( &mainWindow );
      QObject::connect(&myApp, SIGNAL(lastWindowClosed()), &mainWindow, SLOT(quit()));
      cerr << "exec application" << endl;

      mainWindow.show();
      server.detach(4);
      myApp.exec();
    }

    if (own) {
      n.length(1);
      n[0].id = CORBA::string_dup("EventChannel");
      server.namingContext->unbind(n);
      ec->destroy();
    }
    
    if (colocated) {
      n.length(1);
      n[0].id = CORBA::string_dup(NOTIFY_FACTORY_NAME);
      server.initialNamingContext->unbind(n);
    }

    server.shutdown();
    server.wait();
  }
  catch (const CORBA::Exception& e) {
    cerr << "CORBA exception: " << e << endl;
    return 1;
  }
  return 0;
}
