#ifndef SphinxSpeech_h
#define SphinxSpeech_h

#include "miro/Server.h"
#include "miro/ReactorTask.h"

#include "SphinxSpeechImpl.h"
//#include "SphinxSpeechTask.h"

namespace Miro 
{

  class SphinxSpeechObject : public Miro::Server
  {
    typedef Miro::Server super;

  public:
    SphinxSpeechObject(int argc, char *argv[]);
    ~SphinxSpeechObject();

  protected:
    string miroRoot;

    //    SphinxSpeechTask speechTask;
    SphinxSpeechImpl speechImpl;
    Miro::SphinxSpeech_var pSpeech;
  };

};

#endif
