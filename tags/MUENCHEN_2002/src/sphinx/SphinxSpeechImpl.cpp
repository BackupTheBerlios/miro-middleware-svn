#include "SphinxSpeechImpl.h"
#include "FestivalSpeech.h"

#include "miro/StructuredPushSupplier.h"

#include <string>

namespace Miro {
 
  /**
     @param dictFileName Name (with full path) of the dictionary file for Sphinx
     
     @param _halfDuplex this is to be true if a soundcard is half
     duplex...  All the pc104 sound cards are half duplex, almost all
     built on sound cards and/or PCI soundcards (for desktops) are full
     duplex but it shouldn't matter much to just let this use half
     duplex, but you may miss a few ms of speech if you are using half
     duplex instead of full duplex.
  **/
  
  SphinxSpeechImpl::SphinxSpeechImpl(string dictFileName, bool _halfDuplex, StructuredPushSupplier * _supplier) :
    speechTask(this),
    supplier(_supplier),
    dict(dictFileName),
    halfDuplex(_halfDuplex),
    initialized(false),
    speaking(false),
    rec(false)
  {
    festival = new FestivalSpeechImpl();
    lastSentence.sentence.length(0);
    lastSentence.valid=false;
    if (supplier) {
      notifyEvent.header.fixed_header.event_type.domain_name = 
	CORBA::string_dup(supplier->domainName().c_str());
      notifyEvent.header.fixed_header.event_type.type_name = 
	CORBA::string_dup("Speech");
      notifyEvent.header.fixed_header.event_name = CORBA::string_dup("");
      notifyEvent.header.variable_header.length(0); // put nothing here
      notifyEvent.filterable_data.length(0);        // put nothing here
    } else {
      cout << "Speech notification will not be available" << endl;
    }
    speechTask.open(NULL);
    cout << "SphinxSpeechImpl initialized" << endl << flush;
  }
  
  SphinxSpeechImpl::~SphinxSpeechImpl()
  {
    stopRec();
    internalCleanup();
    if (festival !=NULL) delete festival;
  }
  
  SentenceIDL *SphinxSpeechImpl::getLastSentence() throw(EDevIO)
  {
    SentenceIDL* result=new SentenceIDL();
    result->sentence=lastSentence.sentence;
    result->timestamp=lastSentence.timestamp;
    result->valid=lastSentence.valid;
    return result;
  }

  void SphinxSpeechImpl::integrateData(const SentenceIDL& data)
  {
    //while copying, the sentence is not valid
    lastSentence.valid=false;

    lastSentence.sentence=data.sentence;
    lastSentence.timestamp=data.timestamp;
    lastSentence.valid=data.valid;
    
    if (supplier) {
      notifyEvent.remainder_of_body <<= lastSentence;
      supplier->sendEvent(notifyEvent);
    }


  }

  /**
     Adds a new LM to sphinx, all of the words in this LM _MUST_ be in
     the dict file that was passed to the constructor.  
  **/
  void SphinxSpeechImpl::addLm(string lmFileName, string lmName)
  {
    speechTask.addLm(lmFileName,lmName);
  }
  /**
     This sets the language model for sphinx to use, only one of these
     can be active at once (ie each set simply overrides the last).  You
     will need to call this at the beginning 
     
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
     being recognized) and then say whatever speech is given; 
     if the class is in half duplex mode (set in
     the constructor) it'll turn off the microphone before
     speaking... even on a full duplex machine this is probably
     preferrable
   
     @param str the string to say
  **/
  void SphinxSpeechImpl::speak(const char *str) throw(EDevIO)
  {
    bool recording;
    recording=rec;
    if (recording&&halfDuplex) {
      //if it is a half duplex card we must stop the recognition while speaking
      stopRec();
    }
    if (festival!=NULL) festival->speak(str);
    if (recording&&halfDuplex) {
      //if it is a half duplex card, start the recognition again
      usleep(1000);
      startRec();
    }


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
     if the class is in half duplex mode
     (set in the constructor) it'll turn off the microphone before
     speaking... even on a full duplex machine this is probably
     preferrable
     
     @param str the sound file to play
  **/
  void SphinxSpeechImpl::play(const char *str)
  {
    sounds.push_back(str);
    soundIsSpeech.push_back(false);
  }
  
  bool SphinxSpeechImpl::isPlaying()
  {
    return speaking;
  }

  bool SphinxSpeechImpl::startRec() throw (Miro::EDevIO)
  {
    if ((ad = ad_open_sps(16000)) == NULL) {
      throw Miro::EDevIO("ad_open_sps failed");
    }

    if (ad_start_rec(ad) < 0) {
      throw Miro::EDevIO("ad_start_rec failed");
    }
    if (initialized && (cont_ad_attach(cont, ad, ad_read) < 0)) {
      throw Miro::EDevIO("cont_ad_attach failed");
    }
    rec=true;
    return true;
  }

  bool SphinxSpeechImpl::stopRec()  throw (Miro::EDevIO)
  {
    if (!rec) return true;
    if (initialized && (cont_ad_detach(cont) < 0)) {
      throw Miro::EDevIO("cont_ad_detach failed");
    }
    if (ad_stop_rec(ad) <0) {
      throw Miro::EDevIO("ad_stop_rec failed");
    }
    //    if (cont_ad_close(cont)) {
    //      throw Miro::EDevIO("cont_ad_close failed");
    //    }
    if (ad_close(ad) < 0) {
      throw Miro::EDevIO("ad_close failed");
    }
    rec=false;
    return true;
  }

  void SphinxSpeechImpl::internalCleanup() throw (Miro::EDevIO)
  {
    stopRec();
    fbs_end();
  }

};

