#include "JoystickService.h"
#include <miro/Server.h>
#include <miro/ReactorTask.h>
#include <miro/StructuredPushSupplier.h>
#include <idl/MotionC.h>
#include <ace/Reactor.h>
#include <tao/ORB_Core.h>
#include <linux/joystick.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>

using Miro::Server;
using Miro::ReactorTask;
using CosNotifyChannelAdmin::EventChannel;
using CosNotifyChannelAdmin::EventChannel_var;
using CosNotifyChannelAdmin::SupplierAdmin_var;

double EventHandler::oldrot;
long EventHandler::oldtrans;
int EventHandler::counter;

EventHandler::EventHandler(Miro::StructuredPushSupplier *_JoystickSupplier, ACE_HANDLE fd_): ACE_Event_Handler(){
	
	//Initialisierungen
	
	fd = fd_;
	oldrot = 0.0;
	oldtrans = 0;
	JoystickSupplier_ = _JoystickSupplier;
	counter = 1;
	time_ = ACE_OS::gettimeofday();
	OldStatus.translation = 0;
	OldStatus.rotation = 0.0;
	OldStatus.buttons = 0;
	CurStatus.translation = 0;
	CurStatus.rotation = 0.0;
	CurStatus.buttons = 0;
	
	//Maximal zulässige Translation und Rotation holen
	
	
	
	maxTranslation = 1000;
        maxRotation = 3.14;
	PropFaktor = 0.5;
	
	
	event_.header.fixed_header.event_type.domain_name =
        CORBA::string_dup(JoystickSupplier_->domainName().c_str());
        event_.header.fixed_header.event_type.type_name = 
        CORBA::string_dup("JoystickEvent");
        event_.header.fixed_header.event_name = CORBA::string_dup("");
        event_.header.variable_header.length(0);   // put nothing here
        event_.filterable_data.length(0);          // put nothing here
	
}


int EventHandler::handle_input(ACE_HANDLE){
    
    
    //Joystick-Event anlegen
    
    js_event e;
    
    
    //Geschwindigkeit auf vorherige Werte setzen
    

    CurStatus.rotation = oldrot;
    CurStatus.translation = oldtrans;
    
    //Joystick-Device auslesen
    
    read(fd, &e, sizeof(struct js_event));
    
    
    
    //falls Button gedrückt
    
    if(e.type == JS_EVENT_BUTTON){
       
       //Button 0
       
       if(e.number == 0) {
       	  CurStatus.buttons = 0x0001;
     	
       }
       if(e.number == 1) {
          CurStatus.buttons = 0X0002;
       }	  
       
       
    }
    
    
    //falls Joystick bewegt
    
    if(e.type == JS_EVENT_AXIS){
       
       //vertikale Richtung
       
       if (e.number == 1){
          
	  CurStatus.translation = -(maxTranslation * e.value * 4)/(32767*5);
	  oldtrans = CurStatus.translation;
	  
       	  
	
       }
       
       //horizontale Richtung
       
       if (e.number == 0){
          
	  CurStatus.rotation = -0.8*(double)(e.value)*maxRotation/32767.0;
	  CurStatus.rotation *= (1.0-PropFaktor*(double)abs(CurStatus.translation)/(0.8*double(maxTranslation)));
	  oldrot = CurStatus.rotation;
       }	
        
       
			
    }
    return 0;
}


int EventHandler::handle_timeout(const ACE_Time_Value&, const void *)
{
  
  if (OldStatus.translation != CurStatus.translation || OldStatus.rotation != CurStatus.rotation || CurStatus.buttons != 0) {
    event_.remainder_of_body <<= CurStatus;
    JoystickSupplier_->sendEvent(event_);  
    OldStatus.translation = CurStatus.translation;
    OldStatus.rotation = CurStatus.rotation;
    CurStatus.buttons = 0;
    cout << "Translation: " << CurStatus.translation << " Rotation: " << CurStatus.rotation << endl;
    
    if(counter == 0) {
        
	 ACE_Time_Value newtime = ACE_OS::gettimeofday(); 
         long sec = newtime.sec() - time_.sec();
         long msec = newtime.msec() - time_.msec();
         long t = sec * 1000 + msec;
         cout << sec << " " << msec << endl;
         double rate = 100.0*1000.0/(double)t;
         cout << "Rate: " << rate << endl;
         time_ = newtime;
          
    }
    counter = (counter + 1) % 100;
  }

  return 0;
}





int main(int argc, char *argv[]){

   Miro::Server server(argc, argv);
   

   
   
   ReactorTask * task = new ReactorTask(&server);
   EventChannel_var ec = server.resolveName<EventChannel>("EventChannel");
   Miro::StructuredPushSupplier joystickSupplier(ec.in(), server.namingContextName.c_str(), true);
   
   
   int fd = open("/dev/input/js0", O_RDONLY);
   EventHandler eventHandler(&joystickSupplier, (ACE_HANDLE) fd);  
   
   
    //Parameter-Parsing
    for(int i = 1; i < argc; i++){
      if(strcmp("-trans", argv[i])==0){
         if(i + 1 < argc){
	    eventHandler.maxTranslation = (long) atoi(argv[i+1]);
	 }
      }
      if(strcmp("-rot", argv[i])==0){
         if(i + 1 < argc){
            eventHandler.maxRotation = atof(argv[i+1]);
	 }
      }
      if(strcmp("-prop", argv[i])==0){
         if(i + 1 < argc){
            eventHandler.PropFaktor = atof(argv[i+1]);
	 }
      }
    }

   cout << "MaxTranslation: " << eventHandler.maxTranslation << endl;
   cout << "MaxRotation (trans = 0): " << eventHandler.maxRotation << endl;
   cout << "MaxRotation (trans = max): " << eventHandler.maxRotation*(1.0 - eventHandler.PropFaktor) << endl;



   ACE_Reactor  *reactor = server.reactor();
   reactor->register_handler((ACE_HANDLE) fd, &eventHandler, ACE_Event_Handler::READ_MASK);
   
   ACE_Time_Value delay;
   delay.set(0,0);
   
   ACE_Time_Value interval;
   interval.set(0, 40000);
   
   
   reactor->schedule_timer(&eventHandler, NULL, delay, interval);
   
   
   task->open(NULL);
   joystickSupplier.connect();
   
   server.run();
   
   server.detach();
   
   task->cancel();
   joystickSupplier.disconnect();

   
   
   return 0;

}
