// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// for details copyright, usage and credits to other groups see Miro/COPYRIGHT
// for documentation see Miro/doc
//
// (c) 1999,2000
// Department of Neural Information Processing, University of Ulm, Germany
//
// Authors:
//   Stefan Enderle,
//   Stefan Sablatnoeg,
//   Hans Utz
//
// $Id$
//
//////////////////////////////////////////////////////////////////////////////
#ifndef CanMessage_h
#define CanMessage_h

#include "miro/DevMessage.h"

// extern "C"
// {
#include "canmsg.h"
// }

namespace Can
{
  typedef enum 
  {
	OLD,PCAN,UNINITIALIZED
  }drivertype;

  typedef struct
  {
	CanId ID;              // 11/29 bit code
	unsigned char  MSGTYPE;         // bits of MSGTYPE_*
	unsigned char  LEN;             // count of data bytes (0..8)
	unsigned char  DATA[8];         // data bytes, up to 8
  } TPCANMsg;              // for PCAN_WRITE_MSG

  typedef struct
  {
	TPCANMsg Msg;          // the above message
	unsigned long dwTime;       // a timestamp in msec, read only
  } pcanmsg;            // for PCAN_READ_MSG

  // forward declarations
  class Message;

  std::ostream& operator<< (std::ostream& ostr, const Message& rhs);

  class Message : public Miro::DevMessage
  {
  public:
    Message();
    int            length() const;
    void           length(int _len);

    char           charData(int i) const;
    void           charData(int i, char d);
    unsigned char  byteData(int i) const;
    void           byteData(int i, unsigned char d);
    short shortData(int i) const;
    void           shortData(int i, unsigned short d); // sets two chars !
    long  longData(int i) const;
    void           longData(int i, unsigned long d);   // sets four chars !

    void           setBuffer(int pos, const char * buffer, int length ) ;

    CanId          id() const;
    void           id(CanId _id);

    static drivertype driver;

    void canMessage(canmsg ** msg_ )  { *msg_= message_; }
    void canMessage(pcanmsg ** msg_)  { *msg_=messagep_; std::cout << "messagep_ " << (void *) messagep_ << endl; }
  protected:
    canmsg * message_;
    pcanmsg * messagep_;
  };

  inline
  CanId
  Message::id() const { 
	  if(driver==OLD)
	  	return message_->id; 
	  else
		return messagep_->Msg.ID;
  }

  inline
  void
  Message::id(CanId _id) { 
	  if(driver==OLD)
	  	message_->id = _id; 
	  else
		messagep_->Msg.ID=_id;  
  }

  inline
  int
  Message::length() const {
	  if(driver==OLD)
		return message_->len;
	  else
		return messagep_->Msg.LEN;
  }

  inline
  void 
  Message::length(int _len) { 
	if(driver==OLD)
		message_->len = _len; 
	else
		messagep_->Msg.LEN=_len;
  }

  inline
  char
  Message::charData(int i) const { 
	if(driver==OLD)
		return message_->d[i]; 
	else
		return messagep_->Msg.DATA[i];
  }

  inline
  void
  Message::charData(int i, char d) { 
	if(driver==OLD)
		  message_->d[i] = d; 
	else
		  messagep_->Msg.DATA[i]=d;
  }

  inline
  unsigned char
  Message::byteData(int i) const { 
	if(driver==OLD)
		return message_->d[i]; 
	else
		return messagep_->Msg.DATA[i];
  }

  inline
  void
  Message::byteData(int i, unsigned char d) { 
	if(driver==OLD)
		message_->d[i] = d; 
	else
		messagep_->Msg.DATA[i]=d;
  }

  inline
  short
  Message::shortData(int i) const { 
	if(driver==OLD)
		return (short) ACE_NTOHS(*((const unsigned short *) (&(message_->d[i]))));
	else
		return (short) ACE_NTOHS(*((const unsigned short *) (&(messagep_->Msg.DATA[i]))));
  }
  
  inline
  void 
  Message::shortData(int i, unsigned short d) 
  { 
	if(driver==OLD)
		*((unsigned short *) (&(message_->d[i]))) = ACE_HTONS(d);
	else
		*((unsigned short *) (&(messagep_->Msg.DATA[i]))) = ACE_HTONS(d);
  }

  inline
  long
  Message::longData(int i) const { 
	if(driver==OLD)
		return (long) ACE_NTOHL(*((const unsigned long *) (&(message_->d[i]))));
	else
		return (long) ACE_NTOHL(*((const unsigned long *) (&(messagep_->Msg.DATA[i]))));
  }

  inline
  void 
  Message::longData(int i, unsigned long d) 
  { 
	if(driver==OLD)
		*((unsigned long *) (&(message_->d[i]))) = ACE_HTONL(d);
	else
		*((unsigned long *) (&(messagep_->Msg.DATA[i]))) = ACE_HTONL(d);
  }
};
#endif 
