// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////


#include "SphinxSpeechTask.h"
#include "SphinxSpeechImpl.h"
#include "FestivalSpeech.h"

#include "miro/Exception.h"

#include "miro/TimeHelper.h"

#include <iostream>

#include <ace/ARGV.h>

// #undef DEBUG

namespace Miro
{
  //////////////////////////////////////////////////////////////////////
  // SphinxSpeechTask
  //
  SphinxSpeechTask::SphinxSpeechTask(SphinxSpeechImpl * _speechImpl) :
    Super(),
    Log(INFO,"SphinxSpeechTask"),
    speechImpl(_speechImpl)
  {}

  //////////////////////////////////////////////////////////////////////
  // ~SphinxSpeechTask
  SphinxSpeechTask::~SphinxSpeechTask() 
  {
    cout << "Destructing SpeechTask" << endl;
  }

  
  ////////////////////////////////////////////////////////////////////////
  // Now the svc() method where everything interesting happens.
  //
  int 
  SphinxSpeechTask::svc()
  {
#ifdef DEBUG
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) Task 0x%x starts in thread %u\n", (void *) this, ACE_Thread::self ()));
#endif
    
    ACE_ARGV args;
    int error;
    int16 adbuf[4097];
    int32 ts;
    string strtmp;
    string hmmDir=getenv("SPHINX_ROOT");
    hmmDir+="/model/hmm/6k";
    search_hyp_t * result;
    SentenceIDL sentence;

    char * argvtmp;

    args.add("SphinxSpeech");
    args.add("-live TRUE");
    args.add("-ctloffset 0");
    args.add("-ctlcount 100000000");    
    args.add("-agcemax TRUE");
    args.add("-langwt 6.5");
    args.add("-fwdflatlw 8.5");
    args.add("-rescorelw 9.5");
    args.add("-ugwt 0.5");
    args.add("-fillpen 1e-10");
    args.add("-silpen 0.005");
    args.add("-inspen 0.65");
    args.add("-top 1");
    args.add("-topsenfrm 3");
    args.add("-topsenthresh -70000");
    args.add("-beam 2e-06");
    args.add("-npbeam 2e-06");
    args.add("-lpbeam 2e-05");
    args.add("-lponlybeam 0.0005");
    args.add("-nwbeam 0.0005");
    args.add("-fwdflat FALSE");
    args.add("-fwdflatbeam 1e-08");
    args.add("-fwdflatnwbeam 0.0003");
    args.add("-bestpath TRUE");
    
    args.add("-dictfn");
    argvtmp=(char*)malloc(255*sizeof(char));
    sprintf(argvtmp,"%s",speechImpl->dict.c_str());
    args.add(argvtmp);

    args.add("-noisedict");
    argvtmp=(char*)malloc(255*sizeof(char));
    sprintf(argvtmp,"%s/noisedict",hmmDir.c_str());
    args.add(argvtmp);

    args.add("-phnfn");
    argvtmp=(char*)malloc(255*sizeof(char));
    sprintf(argvtmp,"%s/phone",hmmDir.c_str());
    args.add(argvtmp);

    args.add("-mapfn");
    argvtmp=(char*)malloc(255*sizeof(char));
    sprintf(argvtmp,"%s/map",hmmDir.c_str());
    args.add(argvtmp);
    
    args.add("-hmmdir");
    argvtmp=(char*)malloc(255*sizeof(char));
    sprintf(argvtmp,"%s/",hmmDir.c_str());
    args.add(argvtmp);
    
    args.add("-hmmdirlist");
    argvtmp=(char*)malloc(255*sizeof(char));
    sprintf(argvtmp,"%s",hmmDir.c_str());
    args.add(argvtmp);
    
    args.add("-8bsen TRUE");

    args.add("-sendumpfn");
    argvtmp=(char*)malloc(255*sizeof(char));
    sprintf(argvtmp,"%s/sendump",hmmDir.c_str());
    args.add(argvtmp);

    args.add("-cbdir");
    argvtmp=(char*)malloc(255*sizeof(char));
    sprintf(argvtmp,"%s",hmmDir.c_str());
    args.add(argvtmp);
    
    fbs_init(args.argc(), args.argv());

    std::list<std::string>::iterator fit, lmit; //file iterator; lm iterator
     
    for (fit = newLmFileNames.begin(), lmit = newLmNames.begin();
	 fit != newLmFileNames.end();
	 fit = newLmFileNames.begin(), lmit = newLmNames.begin())
    {
      if (lm_read((*fit).c_str(), (*lmit).c_str(), 7.0, .5, 6.5) != 0)
      {
	cerr << "[SphinxSpeechTask] Error: Could not read lm file " << (*fit) << endl;
      } else {
	lmNames.push_front((*lmit));
	newLmFileNames.pop_front();
	newLmNames.pop_front();
      }
    }    
    
    currentLm=newLm;
    if (currentLm.size() >0) uttproc_set_lm(currentLm.c_str());
    newLm="";

    speechImpl->startRec();

    speechImpl->cont = cont_ad_init(speechImpl->ad, ad_read);
    cont_ad_calib(speechImpl->cont);


    while (true) {
      //test if the LM must be changed
      if (newLm.size() > 0 && newLm != currentLm)
      {
	currentLm=newLm;
	uttproc_set_lm(currentLm.c_str());
	newLm="";
      }

      if (!speechImpl->rec) { //soundcard being used to speak
	usleep(1000);
	continue;
      }

      //Queue might not be needed (it is not used at the moment)
      if (speechImpl->halfDuplex) {
 	if (speechImpl->sounds.size()>0) {
	  string str;
	  speechImpl->stopRec();
	  str=*(speechImpl->sounds.begin());
	  speechImpl->sounds.pop_front();
	  if (*(speechImpl->soundIsSpeech.begin())) {
	    speechImpl->festival->speak(str.c_str());
	  }
	  speechImpl->soundIsSpeech.pop_front();
	  speechImpl->startRec();

	  //go to next iteration
	  continue;
	}
      }


      //Await data for next utterance
      error = cont_ad_read(speechImpl->cont, adbuf, 4096);
      if (error==0) {
	//nothing heard
	usleep(10000); //sleep 10 msec
	continue; 
      } else if (error<0) {
	cerr << "[SphinxSpeechTask] Error: cont_ad_read failed" << endl;
	sleep(1); 
	continue;
      }
      
      //if we got here, there is data to process...
      uttproc_begin_utt(NULL);
      uttproc_rawdata(adbuf,error,0);

      //get timestamp
      ts=speechImpl->cont->read_ts;
      Miro::timeA2C(ACE_OS::gettimeofday(),sentence.timestamp);

      while (1) {//will break inside...
	
	error = cont_ad_read(speechImpl->cont, adbuf, 4096);
	if (error <0) break;
	if (error == 0) {
	  //No sound recorded. Check the silence length.
	  //If greater than 1 sec, end of utterance

	  if ((speechImpl->cont->read_ts - ts) > DEFAULT_SAMPLES_PER_SEC)
	    break;
	} else {
	  //renew timestamp
	  ts = speechImpl->cont->read_ts;
	  Miro::timeA2C(ACE_OS::gettimeofday(),sentence.timestamp);
	}

	// decode the last heard data
	//non-blocking mode
	//rem=remainin to be decoded
	int32 rem=uttproc_rawdata(adbuf, error, 0);
	
	if ((rem == 0) && (error == 0)) usleep(10000); 
	//if there is no work to do, just sit there and relax ;-)
      }

      //Utterance ended.
      //Stop listening while decoding
      ad_stop_rec(speechImpl->ad);
      while (ad_read(speechImpl->ad,adbuf, 4096) >= 0); //flush buffer
      cont_ad_reset(speechImpl->cont);

      uttproc_end_utt();
      if (uttproc_result_seg(&error, &result, 1) < 0) {
	cerr << "uttproc_result failed" << endl;
      }
//      string resultStr="";
      sentence.sentence.length(10);
      int i=0;
      while (result!=NULL) {
//	if (resultStr.size()!=0) resultStr+=" ";
//	resultStr+=result->word;
	sentence.sentence[i].word=result->word;
	sentence.sentence[i].startFrame=result->sf;
	sentence.sentence[i].endFrame=result->ef;
	sentence.sentence[i].aScore=result->ascr;
	sentence.sentence[i].lScore=result->lscr;
	sentence.sentence[i].confidence=result->conf;
	sentence.sentence[i].latticeDensity=result->latden;
	sentence.sentence[i].phonePerplexity=result->phone_perp;
	if ((i%10)==0) { //each 10 words, increase result length by 10
	  sentence.sentence.length(i+10); 
	}
	i++;
	result=result->next;
      }
      sentence.sentence.length(i);
      //sentence.timestamp should already be there
      sentence.valid=true;	
      speechImpl->integrateData(sentence);
      //      if (resultStr!="") cout << resultStr << endl << std::flush;
      
      ad_start_rec(speechImpl->ad);
	

    }
    
    log(INFO, "left service.");
    
    return (0);
  }

  /**
     Adds a new LM to sphinx, all of the words in this LM _MUST_ be in
     the dict file that was passed to the constructor.  See the overview
     of this class for more details.
     
     @param dictFilename If the file given by this name doesn't exist
     then it puts the Aria directory at the beginning and hopes that
     that filename exists.
  **/
  void SphinxSpeechTask::addLm(string lmFileName, string lmName)
  {
    newLmFileNames.push_front(lmFileName);
    newLmNames.push_front(lmName);
  }
  /**
     This sets the language model for sphinx to use, only one of these
     can be active at once (ie each set simply overrides the last).  You
     will need to call this at the beginning (see the class overview for
     more details).
     
     @param lmName the name of the language model to set as active, this
     is the name given as lmName in addLm
  **/
  void SphinxSpeechTask::setLm(string lmName)
  {
    newLm = lmName;
  }

  /**
     This gets the language model that sphinx is using at the moment.
     @return the language model that sphinx is using at the moment
  **/
  string SphinxSpeechTask::getLm()
  {
    return currentLm;
  }

  
};
