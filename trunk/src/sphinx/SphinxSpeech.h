#ifndef SphinxSpeech_h
#define SphinxSpeech_h

#include "miro/Server.h"
#include "miro/ReactorTask.h"

#include "SphinxSpeechImpl.h"
//#include "SphinxSpeechTask.h"

namespace Miro 
{

  class SphinxSpeech : public Miro::Server
  {
    typedef Miro::Server super;

  public:
    SphinxSpeech(int argc, char *argv[]);
    ~SphinxSpeech();

  protected:
    string miroRoot;

    //    SphinxSpeechTask speechTask;
    SphinxSpeechImpl speechImpl;
    Miro::Speech_var pSpeech;
  };

};

#endif
