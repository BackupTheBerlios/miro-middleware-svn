#include "Joystick.h"

#include <miro/Client.h>
#include <idl/MotionC.h>
#include <idl/KickerC.h>
#include <ace/Reactor.h>
#include <tao/ORB_Core.h>



#include <linux/joystick.h>
#include <iostream>
#include <string.h>

using std::cout;
using std::cerr;
using std::endl;
using std::flush;
using std::cin;

double EventHandler::oldrot;
long EventHandler::oldtrans;


EventHandler::EventHandler(Miro::Motion_var _motion, Miro::Kicker_var _kicker, bool _kickeron, ACE_HANDLE fd_): ACE_Event_Handler(){
	
	//Initialisierungen
	
	motion_ = _motion;
	kicker_ = _kicker;
	fd = fd_;
	oldrot = 0.0;
	oldtrans = 0;
        kickeron_ = _kickeron;
	maxRotation = 3.14; 
	maxTranslation = 1000;

	//Maximal zul,Ad(Bssige Translation und Rotation holen

	CORBA::Long minTrans;
	CORBA::Long maxTrans;
	CORBA::Double minRot;
	CORBA::Double maxRot;
	motion_->getMinMaxVelocity(minTrans, maxTrans, minRot, maxRot);
	minTranslation = minTrans;
	maxTranslation = maxTrans;
	minRotation = minRot;
	maxRotation = maxRot;
        cout << "MaxTranslation: " << maxTranslation << endl;
        cout << "MaxRotation: " << maxRotation << endl;
	OldVelocity.translation = 0;
	OldVelocity.rotation = 0.0;

	CurVelocity.translation = 0;
	CurVelocity.rotation = 0.0;


}

int EventHandler::handle_input(ACE_HANDLE){


    //Joystick-Event anlegen

    js_event e;


    //Geschwindigkeit auf vorherige Werte setzen


    CurVelocity.rotation = oldrot;
    CurVelocity.translation = oldtrans;

    //Joystick-Device auslesen

    read(fd, &e, sizeof(struct js_event));



    //falls Button gedr,A|(Bckt

    if(e.type == JS_EVENT_BUTTON){

       //Button 0

       if(e.number == 0) {
       	oldrot = 0.0;
       	oldtrans = 0;
       	CurVelocity.rotation = 0.0;
       	CurVelocity.translation = 0;
     	//motion_->setVelocity(velocity);

       }

       //Button 1

       if(e.number == 1) {
         if(kickeron_){
	    Miro::TimeIDL time;
	    time.sec = 0;
	    time.usec = 50000;
	    kicker_->kick(time);
	 }
       }

    }


    //falls Joystick bewegt

    if(e.type == JS_EVENT_AXIS){

       //vertikale Richtung

       if (e.number == 1){
          
	  CurVelocity.translation = -(maxTranslation * (long)e.value)/(32767);

	  oldtrans = CurVelocity.translation;
	  cout << maxTranslation << " " << maxRotation << endl;
          //motion_->setVelocity(velocity);
          cout << "Achse 1 " << e.value << " Geschw: " << CurVelocity.translation
          << endl;
       }

       //horizontale Richtung

       if (e.number == 0){

	  CurVelocity.rotation = -0.8*(double)(e.value)*maxRotation/32767.0;
	  oldrot = CurVelocity.rotation;
	  //motion_->setVelocity(velocity);
	  cout << "Achse 0 " << e.value << " Geschw: " <<
          CurVelocity.rotation << endl;
       }

    }
    return 0;
}

int EventHandler::handle_timeout(const ACE_Time_Value&, const void *)
{
   if (OldVelocity.translation != CurVelocity.translation || OldVelocity.rotation != CurVelocity.rotation){
      OldVelocity.translation = CurVelocity.translation;
      OldVelocity.rotation = CurVelocity.rotation;
      motion_->setVelocity(CurVelocity);

   }

  cout << "********************************handle timeout*********************" << endl;
  return 0;
}



int
main(int argc, char *argv[])
{
    //Client erzeugen

    Miro::Client client(argc, argv);


    //Standard Joystick-Device
    
    char device[500] = "/dev/input/js0";
    
    bool kickeron = true;
    

    //Parameter-Parsing
    for(int i = 1; i < argc; i++){
      if(strcmp("-dev", argv[i])==0){
         if(i + 1 < argc){
	    strcpy(device, argv[i+1]);
	 }
      }
      if(strcmp("-nokicker", argv[i]) == 0){
         kickeron = false;
	 
      
      }
   }
    
    
    //Interface-Deklaration
    Miro::Kicker_var kicker;
    Miro::Motion_var motion;
    

    //,A|(Bber den Client Motion- und Kicker-Interface holen und Exception-Handling
    
    try {
       motion = Miro::resolve_name<Miro::Motion>(client, "Motion");
    }
    catch (const CORBA::Exception& e){
    
       cout << "Motion-Device not found" << endl;
    
    }
    
    if(kickeron){
       try {
          kicker = Miro::resolve_name<Miro::Kicker>(client, "Kicker");
       }
       catch (const CORBA::Exception& e) {
          cout << "Kicker-Device not found" << endl;
          kickeron = false;
       }
    }    
    
    //reactor instantiieren
    
    ACE_Reactor *reactor = ACE_Reactor::instance();
    
    
    //Joystick-fd holen
    int fd = open(device, O_RDONLY);    
    if (fd == -1){
       cout << "Joystick-Device not found" << endl;
       exit(1);
    }
    cout << "Joystick Device gefunden" << endl;
    //EventHandler erzeugen, Motion und Kicker ,A|(Bbergeben, Joystick-fd ,A|(Bbergeben
    
    EventHandler eventHandler(motion, kicker, kickeron, (ACE_HANDLE) fd);
    
    
    //EventHandler beim reactor f,A|(Br Handle registrieren und den reactor starten
    
    reactor->register_handler((ACE_HANDLE) fd, &eventHandler, ACE_Event_Handler::READ_MASK);

    ACE_Time_Value delay;
    delay.set(0,0);

    ACE_Time_Value interval;
    interval.set(0, 100000);
    reactor->schedule_timer(&eventHandler, NULL, delay, interval);
    reactor->run_reactor_event_loop();
    return 0;

}
