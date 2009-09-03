// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 1999-2005
// Department of Neuroinformatics, University of Ulm, Germany
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// $Id$
//
#ifndef SPHINX_SPEECH_IMPL_H
#define SPHINX_SPEECH_IMPL_H

extern "C" {
#include <sphinx2/s2types.h>
#include <sphinx2/err.h>
#include <sphinx2/ad.h>
#include <sphinx2/cont_ad.h>
#include <sphinx2/fbs.h>
}

#include "idl/SphinxSpeechS.h"
#include "SphinxSpeechTask.h"

#include <orbsvcs/CosNotifyCommC.h>

#include <list>
#include <string>

namespace Miro 
{

  //forward declarations
  class FestivalSpeechImpl;
  class StructuredPushSupplier;

  class SphinxSpeechImpl : public POA_Miro::SphinxSpeech
  {
  public:
    SphinxSpeechImpl(std::string const & dictFileName, 
		     bool _halfDuplex = true,
		     Miro::StructuredPushSupplier * _supplier = NULL);
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
    Miro::StructuredPushSupplier *supplier;
    CosNotification::StructuredEvent notifyEvent;
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
}

#endif