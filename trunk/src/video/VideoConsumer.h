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
#ifndef VideoConsumer_h
#define VideoConsumer_h

#include "miro/Synch.h"
#include "miro/Task.h"
//#include "miro/DevConsumer.h"


namespace Video
{
  // forward declerations
  class Connection;
  class VideoImpl;

  /**
   * Class for using the Video robot
   */
  class Consumer : public Miro::Task
  {
    typedef Miro::Task Super;
    
  public:
    Consumer(Connection& _connection, 
	     VideoImpl * _pGrabber = NULL);
    ~Consumer();

    virtual int svc();

    virtual int open(void *);
    virtual void cancel();

    int getImageSize() const;
    void getCurrentImage(void*);
    void getWaitNextImage(void*);

  protected:
  	void copyImageData(void*, const void*);

	void copy(void*, const void*, const int);

	void swap3(void*, const void*);
 	void swap4(void*, const void*);

	void swapLine3(void*, const void*, const int);
	void swapLine4(void*, const void*, const int);

	Connection&	connection;
	VideoImpl*       pGrabber;
	Miro::Mutex      mutex;
	Miro::Condition  cond;
	bool             running;
	void*           pCurrentImageData;

  private:
    static ACE_Time_Value maxWait;
  };
};
#endif

