#include "SphinxSpeechImpl.h"
#include "FestivalSpeech.h"
#include <string>

namespace Miro {
 
  /**
     @param dictFilename If the file given by this name doesn't exist
     then it puts the Aria directory at the beginning and hopes that
     that filename exists.
     
     @param halfDuplex this is to be true if a soundcard is half
     duplex...  All the pc104 sound cards are half duplex, almost all
     built on sound cards and/or PCI soundcards (for desktops) are full
     duplex but it shouldn't matter much to just let this use half
     duplex, but you may miss a few ms of speech if you are using half
     duplex instead of full duplex.
  **/
  
  SphinxSpeechImpl::SphinxSpeechImpl(string dictFileName, bool _halfDuplex) :
    speechTask(this),
    dict(dictFileName),
    halfDuplex(_halfDuplex),
    initialized(false),
    speaking(false),
    rec(false)
  {
    festival = new FestivalSpeechImpl();
    //    speechTask=new SphinxSpeechTask(this);
    speechTask.open(NULL);
    //    startRec();
    cout << "SphinxSpeechImpl initialized" << endl << flush;
  }
  
  SphinxSpeechImpl::~SphinxSpeechImpl()
  {
    stopRec();
    internalCleanup();
    delete festival;
    //    delete speechTask;
  }
  
  /**
     Adds a new LM to sphinx, all of the words in this LM _MUST_ be in
     the dict file that was passed to the constructor.  See the overview
     of this class for more details.
     
     @param dictFilename If the file given by this name doesn't exist
     then it puts the Aria directory at the beginning and hopes that
     that filename exists.
  **/
  void SphinxSpeechImpl::addLm(string lmFileName, string lmName)
  {
    speechTask.addLm(lmFileName,lmName);
  }
  /**
     This sets the language model for sphinx to use, only one of these
     can be active at once (ie each set simply overrides the last).  You
     will need to call this at the beginning (see the class overview for
     more details).
     
     @param lmName the name of the language model to set as active, this
     is the name given as lmName in addLm
  **/
  void SphinxSpeechImpl::setLm(string lmName)
  {
    speechTask.setLm(lmName);
  }

  /**
     This gets the language model that sphinx is using at the moment.
     @return the language model that sphinx is using at the moment
  **/
  string SphinxSpeechImpl::getLm()
  {
    return speechTask.getLm();
  }


  /**
     This will wait until the current recognition is through (if one is
     being recognized) and then say whatever speech is given, taking
     varargs like printf, if the class is in half duplex mode (set in
     the constructor) it'll turn off the microphone before
     speaking... even on a full duplex machine this is probably
     preferrable
   
     @param str the string to say
  **/
  void SphinxSpeechImpl::speak(const char *str) throw(EDevIO)
  {
    sounds.push_back(str);
    soundIsSpeech.push_back(true);
    festival->speak(str);
  }

  void SphinxSpeechImpl::skip() throw(EDevIO)
  {
    //unimplemented
    //needed for compatibility
  }
  void SphinxSpeechImpl::clear() throw(EDevIO)
  {
    //unimplemented
    //needed for compatibility
  }
  void SphinxSpeechImpl::reset() throw(EDevIO)
  {
    //unimplemented
    //needed for compatibility
  }
  void SphinxSpeechImpl::setVoice(char voice) throw(EDevIO)
  {
    //unimplemented
    //needed for compatibility
  }

  /**
     This will wait until the current recognition is through (if one is
     being recognized) and then play whatever sound file is given,
     taking varargs like printf, if the class is in half duplex mode
     (set in the constructor) it'll turn off the microphone before
     speaking... even on a full duplex machine this is probably
     preferrable
     
     @param str the sound file to play
  **/
  void SphinxSpeechImpl::play(const char *str, ...)
  {
    char buf[2048];
    va_list ptr;
    va_start(ptr, str);
    vsprintf(buf, str, ptr);
    sounds.push_back(buf);
    soundIsSpeech.push_back(false);
    va_end(ptr);
  }
  
  bool SphinxSpeechImpl::isPlaying()
  {
    return speaking;
  }

  bool SphinxSpeechImpl::startRec() //throw Miro::EDevIO
  {
    if ((ad = ad_open_sps(16000)) == NULL) {
      //      throw Miro::EDevIO("ad_open_sps failed");
      return false;
    }

    if (ad_start_rec(ad) < 0) {
      //      throw Miro::EDevIO("ad_start_rec failed");
      return false;
    }
    if (initialized && (cont_ad_attach(cont, ad, ad_read) < 0)) {
      //      throw Miro::EDevIO("cont_ad_attach failed");
      return false;
    }
    rec=true;
    return true;
  }

  bool SphinxSpeechImpl::stopRec()  //throw Miro::EDevIO
  {
    if (!rec) return true;
    if (initialized && (cont_ad_detach(cont) < 0)) {
      //throw Miro::EDevIO("cont_ad_detach failed");
      return false;
    }
    if (ad_stop_rec(ad) <0) {
      //      throw Miro::EDevIO("ad_stop_rec failed");
      return false;
    }
    /*    if (cont_ad_close(cont)) {
      //      throw Miro::EDevIO("cont_ad_close failed");
      return false;
      }*/
    if (ad_close(ad) < 0) {
      //      throw Miro::EDevIO("ad_close failed");
      return false;
    }
    rec=false;
    return true;
  }

  void SphinxSpeechImpl::internalCleanup() //throw Miro::EDevIO
  {
    stopRec();
    fbs_end();
  }

};

