#include "miro/Client.h"
#include "idl/SphinxSpeechC.h"
#include "miro/TimeHelper.h"

#include <string>

using std::string;

int
main(int argc, char **argv)
{

  try {
    //get the connection to the server
    Miro::Client client(argc,argv);
    Miro::SphinxSpeech_var speech=client.resolveName<Miro::SphinxSpeech>("Speech");

    //some variables...
    Miro::SentenceIDL_var sentence;
    Miro::TimeIDL timestamp;
    timestamp.sec=0; //initialization
    timestamp.usec=0;

    while (1) {
      //get the last recognized sentence
      sentence=speech->getLastSentence();
      //check whether there is a sentence
      //and it was said later than the last recognized one
      //      if (timestamp.sec==timestamp.usec==0) timestamp=sentence->timestamp;
      if (!sentence->valid || 
	  (1000000*sentence->timestamp.sec+sentence->timestamp.usec) 
	  == 
	  (1000000*timestamp.sec+timestamp.usec)) 
	{
	  usleep(1000000); //nothing heard
	  cout << sentence->timestamp.sec << "." << sentence->timestamp.usec << endl;
	} else {
	  cout << "sentence recognized" << endl;
	  //a new sentence was recognized
	  
	  //get new timestamp
	  timestamp=sentence->timestamp;
	  string str="";
	  
	  //build sentence from words
	  for (unsigned int i=0; i<sentence->sentence.length(); i++) {
	    str+=sentence->sentence[i].word;
	    str+=" ";
	  }
	  
	  //build the sentence to speak
	  if (str!="") {
	    str=string("You said ")+str;
	  } else {
	    //should never be the case
	    str="You said something I don't understand";
	  }
	  //say the sentence...
	  speech->speak(str.c_str());
	  //...and write it on the screen
	  cout << str << endl;
	  
	  //give some extra information for each word in the sentence
	  cout << "List of words said with latticeDensity, start frame, and end frame:" << endl;
	  for (unsigned int i=0; i<sentence->sentence.length(); i++) {
	    cout << sentence->sentence[i].word << " " 
		 << sentence->sentence[i].latticeDensity << " " 
		 << sentence->sentence[i].startFrame << " "
		 << sentence->sentence[i].endFrame << endl;
	  }
	}
    }
  }
  catch(Miro::Exception &e) {
    cout << "Miro Exception: " << e << endl;
  }
  catch (CORBA::Exception &e) {
    cout << "CORBA Exception: " << e << endl;
  }
  catch(...) {
    cout << "Uncaught exception" << endl;
  }
}
