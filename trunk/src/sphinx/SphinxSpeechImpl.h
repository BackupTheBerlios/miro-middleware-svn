#ifndef SPHINX_SPEECH_IMPL_H
#define SPHINX_SPEECH_IMPL_H

extern "C" {
#include <sphinx2/s2types.h>
#include <sphinx2/err.h>
#include <sphinx2/ad.h>
#include <sphinx2/cont_ad.h>
#include <sphinx2/fbs.h>
}

#include "miro/SphinxSpeechS.h"
#include "SphinxSpeechTask.h"

#include <list>
#include <string>

namespace Miro 
{

  //forward declaration
  class FestivalSpeechImpl;

  class SphinxSpeechImpl :public POA_Miro::SphinxSpeech
  {
  public:
    SphinxSpeechImpl(std::string dictFileName, bool _halfDuplex=true);
    ~SphinxSpeechImpl();

    //sphinx
    virtual SentenceIDL* getLastSentence() throw(Miro::EDevIO);

    void integrateData(const SentenceIDL& data);

    void addLm(std::string lmFileName, std::string lmName);
    void setLm(std::string lmName);
    std::string getLm();

    //festival
    virtual void speak(const char *str) throw(Miro::EDevIO);

    //Not implemented
    virtual void skip() throw(Miro::EDevIO);
    //Not implemented
    virtual void clear() throw(Miro::EDevIO);
    //Not implemented
    virtual void reset() throw(Miro::EDevIO);

    //Not implemented
    virtual void setVoice(char voice) throw(Miro::EDevIO);

    //Not implemented
    void play(const char *str);

    //other
    bool isPlaying();

  protected:
    //sphinx
    bool startRec() throw (Miro::EDevIO);
    bool stopRec() throw (Miro::EDevIO);

    //other
    void internalCleanup() throw(Miro::EDevIO);

    //sphinx
    SphinxSpeechTask speechTask;
    SentenceIDL lastSentence;
    
    std::string dict;
    ad_rec_t * ad;
    cont_ad_t * cont;
   

    //festival
    std::list<std::string> sounds;
    std::list<bool> soundIsSpeech;
    FestivalSpeechImpl *festival;
    
    //sphinx
    bool halfDuplex;

    //other
    bool initialized;
    bool speaking; 
    bool rec;

    friend class SphinxSpeechTask;
  };

};

#endif
