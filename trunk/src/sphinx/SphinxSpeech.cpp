
#include "miro/Server.h"
#include "SphinxSpeech.h"

#include <string>

namespace Miro
{
  SphinxSpeechObject::SphinxSpeechObject(int argc, char *argv[]) :
    super(argc,argv),
    miroRoot(getenv("MIRO_ROOT")),
    speechImpl((miroRoot+"/etc/sphinx.dic").c_str()),
    pSpeech(speechImpl._this())
  {
    speechImpl.addLm((miroRoot+"/etc/sphinx.lm").c_str(),"default");
    speechImpl.setLm("default");
    
    CosNaming::Name n(1);
    n.length(1);
    n[0].id = CORBA::string_dup("Speech");

    if(rebind_) {
      // Force binding of references to make
      // sure they are always up-to-date.
      try {
	namingContext->unbind(n);
	cerr << "A Speech object was still registered at the NamingService.";
      }
      catch (...) {
      }      
    }
    try {
      namingContext->bind(n, pSpeech.in());
    }
    catch (CosNaming::NamingContext::AlreadyBound& ) {
      cerr << "Objects were still bound in naming service: " 
	   << n[0].id << endl
	   << "Use -MiroRebindIOR if you really want to rebind them." << endl;
      
      throw(0);
    }
  }

  SphinxSpeechObject::~SphinxSpeechObject()
  {
    try {
      CosNaming::Name n;
     
      n.length(1);
      n[0].id = CORBA::string_dup("Speech");
      namingContext->unbind(n);
      
#ifdef DEBUG
      cout << "Unbound objects from NamingSevice." << endl;
#endif
    }
    catch (const CORBA::Exception& e) {
      cerr << "Caught CORBA exception on unbind: " << e << endl;
      cerr << "Porbably the NameSevice went down while we ran." << endl;
    }
  }
};


int main(int argc, char* argv[]) 
{
  Miro::SphinxSpeechObject speech(argc,argv);
 
  try {
#ifdef DEBUG
    cout << "Loop forever handling events." << endl;
#endif
    speech.run();
#ifdef DEBUG
    cout << "Speech server ended, exiting." << endl;
#endif
  }
  catch (const Miro::EDevIO& e) {
    cerr << "DevIO excetpion: Device access failed." << endl;
    return 1;
  }
  catch (const CORBA::Exception & e) {
    cerr << "Uncaught CORBA exception: " << e << endl;
    return 1;
  }
  catch (const Miro::ACE_Exception& e) {
    cerr << "ACE exception: " << e << endl;
    return 1;
  }
  catch (...) {
    cerr << "Uncaught exception: " << endl;
    return 1;
  }

  return 0;  
}
