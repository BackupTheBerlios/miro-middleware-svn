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
  {}

  
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
      //TODO: add ACE timestamp to result
      ts=speechImpl->cont->read_ts;

      cout << "Listening..." << endl;
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

  
  ////////////////////////////////////////////////////////////////////////
  // handleMessage
  //
  /*
    void 
    SphinxSpeechTask::handleMessage(const LaserMessage& message) 
    {
    // we do not need to chack for crc here, we are only passed crc
    // correct packets
    switch (message.cmd()) {
    
    case LR_LMS_TYPE:
    log(INFO, "received LMS_TYPE MESSAGE");
    message.PrintDataOn(std::cerr);
    break;
    
    case LR_POWER_ON: 
    log(INFO, "received POWER_ON MESSAGE");
    message.PrintDataOn(std::cerr);
    break;
    
    case LR_ACK_RESET: 
    log(INFO, "received SW-RESET ACKNOWLEDGE MESSAGE");
    break;
    
    case LR_NOT_ACK: 
    log(WARNING, "received NOT ACKNOWLEDGE MESSAGE");
    break;
    
    case LR_MODE_CHANGE: 
    log(INFO, "received MODE CHANGE ACK");
    laser_.syncModeChange.acquire();
    switch (message.getUnsignedByte(0)) {
    case 0: 
    log(INFO, "Mode changed successfully");
    laser_.modeChanged = true;
    break;
    case 1:
    log(INFO, "Mode not changed, wrong password");
    laser_.modeChanged = false;
    break;
    case 2:
    log(INFO, "Mode not changed, error in LMI/LMS");
    laser_.modeChanged = false;
    break;
    default:
    log(INFO, "Mode not changed, unknown error");
    laser_.modeChanged = false;
    break;
    }
    laser_.syncModeChange.release();
    laser_.syncModeChangeCond.broadcast();
    break;
    
    case LR_REQ_DATA: 
    {
    //log(INFO, "received sensor data");
    //message.PrintDumpOn(std::cerr);
    int vals = (message.getUnsignedShort(0) & 0x3fff);
    
    // throw exception if not the expected number of data arrived
    if (vals != 361)
    throw Miro::Exception("SphinxSpeechTask::handleMessage: was expecting 361 values");
    
    Guard guard(laser_.syncLaserScan);
    RangeGroupEventIDL * data = new RangeGroupEventIDL();
    
    ACE_Time_Value now = ACE_OS::gettimeofday();
    timeA2C(now, data->time);
    data->group = 0;
    data->range.length(vals);
    
    for (long i = vals - 1; i >= 0; --i) {
    data->range[i] = message.getUnsignedShort((i+1)*2) & 0x1fff;
    }
    
    laserI_.integrateData(data);
    
    laser_.syncLaserScanCond.broadcast();
    break;
    }
    case LR_LMS_STATUS: 
    log(INFO, "received sensor/LMI status");
    laser_.syncStatusCond.broadcast();
    // TODO further decode
    break;
    
    case LR_ERROR_TEST: 
    log(INFO, "received error/test");
    switch (message.getUnsignedByte(0)) {
    case 0: 
    log(INFO, "No Error, Test OK");
    break;
    case 1:
    log(INFO, "Info");
    break;
    case 2:
    log(WARNING, "Warning");
    break;
    case 3:
    log(ERROR, "Error");
    break;
    case 4:
    log(ERROR, "Fatal Error");
    break;
    }
    // TODO further decode
    
    break;
    
    case LR_MEMORY_DUMP: 
    log(INFO, "received memory dump");
    // TODO further decode
    break;
    
    case LR_AVERAGE_DATA: 
    log(INFO, "received average data");
    // TODO further decode
    break;
    
    case LR_DATA_RANGE: 
    log(INFO, "received data sub range");
    // TODO further decode
    break;
    
    case LR_LMI_CONF_DATA: 
    log(INFO, "received lmi configuration");
    // TODO further decode
    break;
    
    case LR_LMI_HANDLE_DEFS: 
    log(INFO, "received data definition");
    // TODO further decode
    break;
    
    case LR_LMS_VARIANT_CHANGE: 
    log(INFO, "received variant change");
    switch (message.getUnsignedByte(0)) {
    case 0: 
    log(INFO, "variant change aborted");
    break;
    case 1:
    log(INFO, "variant change done");
    break;
    }
    
    log(INFO, "angular scan range    :");
    cerr << message.getUnsignedShort(1) << endl;
    log(INFO, "single shot resolution:");
    cerr << message.getUnsignedShort(3) << endl;
    
    break;
    
    case LR_FIELD_CONF: 
    log(INFO, "received field configuration :");
    switch (message.getUnsignedByte(0)) {
    case 0: 
    log(INFO, "field conf aborted");
    break;
    case 1:
    log(INFO, "field conf done");
    break;
    }
    //  TODO further decode
    break;
    
    case LR_PASSWORD_CHANGE: 
    log(INFO, "received password change ack:");
    switch (message.getUnsignedByte(0)) {
    case 0: 
    log(INFO, "password change aborted");
    break;
    case 1:
    log(INFO, "password change done");
    break;
    case 2:
    log(INFO, "acknowledge of user expected");
    break;
    }
    switch (message.getUnsignedByte(1)) {
    case 0: 
    log(INFO, "password for SICK-SERVICE and authorized customers");
    break;
    case 1:
    log(INFO, "password for maintenance");
    break;
    break;
    }
    break;
    case LR_FIELD_CONF_DATA: 
    log(INFO, "received field configuration data:");
    //  TODO further decode
    break;
    
    case LR_LEARN_FIELD_CONF: 
    log(INFO, "received learn field configuration :");
    switch (message.getUnsignedByte(0)) {
    case 0: 
    log(INFO, "learning aborted");
    break;
    case 1:
    log(INFO, "done, verification can start");
    break;
    case 3:
    log(INFO, "learning active");
    break;
    }
    log(INFO,"learning from LMS #:");
    cerr << message.getUnsignedByte(1) << endl;
    break;
    
    case LR_CONF_DYN_RECT: 
    log(INFO, "received configuration for dynamic rectangle:");
    //  TODO further decode
    break;
    
    case LR_CONF_DYN_SEGMENT: 
    log(INFO, "received dynamic segment configuration:");
    //  TODO further decode
    break;
    
    case LR_DYN_FIELD_INDEX_OR_SPEED: 
    log(INFO, "received field index or speed configuration :");
    switch (message.getUnsignedByte(0)) {
    case 0: 
    log(INFO, "switch aborted");
    break;
    case 1:
    log(INFO, "switch done.");
    break;
    }
    log(INFO,"Index of activated field #:");
    cerr << message.getUnsignedShort(1) << endl;
    break;
    
    case LR_SET_OUTPUT: 
    log(INFO, "received set output active/inactive :");
    switch (message.getUnsignedByte(0)) {
    case 0: 
    log(INFO, "output switch aborted");
    break;
    case 1:
    log(INFO, "output switch done.");
    break;
    }
    log(INFO,"output #:");
    cerr << message.getUnsignedShort(1) << endl;
    log(INFO,"active (1) or inactive (0) :");
    cerr << message.getUnsignedShort(2) << endl;
    
    break;

    case LR_LMI_INPUT: 
    log(INFO, "received input data (bit 0..3:A..D, 4:restart):");
    cerr << message.getUnsignedShort(0) << endl;
    break;
    
    case LR_SIMULATE_INPUT: 
    log(INFO, "received simulate input:");
    //  TODO further decode
    break;
    
    case LR_CALIBRATE: 
    log(INFO, "received calibrate:");
    switch (message.getUnsignedByte(0)) {
    case 0: 
    log(INFO, "calibration not ok");
    break;
    case 1:
    log(INFO, "calibration ok");
    break;
    }
    break;
    
    case LR_CHANGE_PERMANENT_BAUDRATE: 
    log(INFO, "received acknowledge of permanent baudrate change:");
    switch (message.getUnsignedByte(0)) {
    case 0: 
    log(INFO, "change of baudrate not accepted");
    break;
    case 1:
    log(INFO, "change of baudrate accepted");
    break;
    }
    switch (message.getUnsignedByte(1)) {
    case 0: 
    log(INFO, "baudrate will be 9600 on poweron");
    break;
    case 1:
    log(INFO, "baudrate will be unchanged on poweron");
    break;
    }
    break;
    
    case LR_CHANGE_ADDRESS: 
    log(INFO, "received acknowledge of address definition :");
    switch (message.getUnsignedByte(0)) {
    case 0: 
    log(INFO, "change of address not accepted");
    break;
    case 1:
    log(INFO, "change of address accepted");
    break;
    }
    log(INFO,"new address is :");
    cerr << message.getUnsignedByte(1) << endl;
    break;
    
    case LR_ACTIVATE_SENDER: 
    log(INFO, "received activate sender :");
    switch (message.getUnsignedByte(0)) {
    case 0: 
    log(INFO, "activation of sender not accepted");
    break;
    case 1:
    log(INFO, "activation of sender accepted");
    break;
    }
    log(INFO,"activation is :");
    cerr << message.getUnsignedByte(1) << endl;
    break;
    
    case LR_LMI_CONF: 
    log(INFO, "received LMI conf :");
    switch (message.getUnsignedByte(0)) {
    case 0: 
    log(INFO, "LMI conf not accepted");
    break;
    case 1:
    log(INFO, "LMI conf accepted");
    break;
    }
    log(INFO,"number of attached sensors :");
    cerr << message.getUnsignedByte(1) << endl;
    // TODO further decode 
    break;
    
    case LR_HANDLE_DEFS: 
    log(INFO, "received case definition :");
    // TODO further decode 
    break;
    case LR_PIXEL_ORIENTED: 
    log(INFO, "received pixel oriented mode acknowledge:");
    switch (message.getUnsignedByte(0)) {
    case 0: 
    log(INFO, "state change not accepted");
    break;
    case 1:
    log(INFO, "state change accepted");
    break;
    }
    switch (message.getUnsignedByte(1)) {
    case 0: 
    log(INFO, "state : not pixel oriented");
    break;
    case 1:
    log(INFO, "state : pixel oriented");
    break;
    }
    break;
    case LR_CARTESIAN: 
    log(INFO, "received cartesian scan:");
    break;
    
    case LR_CONF_LMS: 
    log(INFO, "received LMS configuration:");
    switch (message.getUnsignedByte(0)) {
    case 0: 
    log(INFO, "conf not accepted");
    break;
    case 1:
    log(INFO, "conf accepted");
    break;
    }
    break;
    
    
    default:
    cerr << __FILE__  << " : " 
    << __LINE__ << ":" 
    << __FUNCTION__ ; 
    log(WARNING, "() - Unhandled opcode in :");
    message.PrintDumpOn(cerr);
    }
    }*/
};
