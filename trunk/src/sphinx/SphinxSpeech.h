#ifndef SphinxSpeech_h
#define SphinxSpeech_h

#include "miro/Server.h"
#include "miro/ReactorTask.h"

#include "SphinxSpeechImpl.h"

namespace Miro 
{

  //forward declaration
  class StructuredPushSupplier;

  class SphinxSpeechServer : public Miro::Server
  {
    typedef Miro::Server super;

  public:
    SphinxSpeechServer(int argc, char *argv[], StructuredPushSupplier * _supplier = NULL);
    ~SphinxSpeechServer();

  protected:
    std::string miroRoot;

    SphinxSpeechImpl speechImpl;
    Miro::SphinxSpeech_var pSpeech;
  };

};

#endif
