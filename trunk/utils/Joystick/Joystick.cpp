#include "Joystick.h"

#include <miro/Client.h>
#include <miro/MotionC.h>
#include <miro/KickerC.h>
#include <ace/Reactor.h>
#include <tao/ORB_Core.h>



#include <linux/joystick.h>
#include <iostream>
#include <string.h>

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
	
	//Maximal zulässige Translation und Rotation holen
	
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
	
}

int EventHandler::handle_input(ACE_HANDLE){
    
    
    //Joystick-Event anlegen
    
    js_event e;

    
    //Geschwindigkeit auf vorherige Werte setzen
    
    Miro::VelocityIDL velocity;
    velocity.rotation = oldrot;
    velocity.translation = oldtrans;
    
    //Joystick-Device auslesen
    
    read(fd, &e, sizeof(struct js_event));
    
    
    
    //falls Button gedrückt
    
    if(e.type == JS_EVENT_BUTTON){
       
       //Button 0
       
       if(e.number == 0) {
       	oldrot = 0.0;
       	oldtrans = 0;
       	velocity.rotation = 0.0;
       	velocity.translation = 0;
     	motion_->setVelocity(velocity);
       	
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
          
	  velocity.translation = -(maxTranslation * (long)e.value * 4)/(32767*5);

	  oldtrans = velocity.translation;
	  cout << maxTranslation << " " << maxRotation << endl;
          motion_->setVelocity(velocity);
          cout << "Achse 1 " << e.value << " Geschw: " << velocity.translation
          << endl;	
       }
       
       //horizontale Richtung
       
       if (e.number == 0){
          
	  velocity.rotation = -0.8*(double)(e.value)*maxRotation/32767.0;
	  oldrot = velocity.rotation;
	  motion_->setVelocity(velocity);
	  cout << "Achse 0 " << e.value << " Geschw: " <<
          velocity.rotation << endl;
       }	
		
    }
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
    

    //über den Client Motion- und Kicker-Interface holen und Exception-Handling
    
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
    //EventHandler erzeugen, Motion und Kicker übergeben, Joystick-fd übergeben
    
    EventHandler eventHandler(motion, kicker, kickeron, (ACE_HANDLE) fd);
    
    
    //EventHandler beim reactor für Handle registrieren und den reactor starten
    
    reactor->register_handler((ACE_HANDLE) fd, &eventHandler, ACE_Event_Handler::READ_MASK);
    reactor->run_reactor_event_loop();
    return 0;
    
}
