// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef VideoConsumer_h
#define VideoConsumer_h

#include "miro/Synch.h"
#include "miro/Thread.h"
#include "miro/TimeC.h"

namespace Video
{
  // forward declerations
  class Device;

  /**
   * Class for using the Video robot
   */
  class Consumer : public Miro::Thread
  {
    typedef Miro::Thread Super;
    
  public:
    Consumer(Device& _device, 
	     ACE_Sched_Params * pschedp = NULL);
    ~Consumer();

    virtual int svc();

    int getPaletteSize() const;
    void getWaitNextSubImage(unsigned char *, const int, const int);

  protected:
    void shrinkImageData(unsigned char*, unsigned char*, int, int);

    void copyImageData(void*, const void*);

    void copy(void*, const void*, const int);

    void swap3(void*, const void*);
    void swap4(void*, const void*);

    void swapLine3(void*, const void*, const int);
    void swapLine4(void*, const void*, const int);

    Device&     videoDevice;
    Miro::Mutex      mutex;
    Miro::Condition  cond;
    void *           pCurrentImageData;
    Miro::TimeIDL    timeStamp_;

    ACE_Sched_Params schedp_;

  private:
    static ACE_Time_Value maxWait;
  };
};
#endif

