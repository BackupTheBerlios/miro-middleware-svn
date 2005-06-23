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
#ifndef laserMessage_hh
#define laserMessage_hh

#include "SickLaserOpcodes.h"
#include "miro/Exception.h"

namespace Miro 
{
  using std::cout;
  using std::cerr;
  using std::endl;

  /**
   * represents a message block sent to and received from the SICK 
   * laserscanner. 
   *
   * a message is structured as follows: (see also sick manual page 7/67)
   * STX (0x2)
   * Addr (1 byte)
   * Length (2 byte, including command byte, but excluding crc)
   * Cmd (1 byte)
   * Data (0..n bytes)
   * Status (1 byte, only in messages from the laserscanner)
   * Crc (2 bytes, computed over the whole message, including STX !)
   */
  class LaserMessage 
  {

    static const int SIZE = 1100;
    unsigned char buffer[SIZE];
  

  public:
    /**
     * constructs an empty LaserMessage object
     */
    LaserMessage()                     { memset(buffer,0,SIZE);} //size = 0; }

    /**
     * constructs a LaserMessage object
     */
    LaserMessage(char address_, char cmd_, unsigned short length_, const char * data_) {
      fillMessage(address_, cmd_, length_, data_);
    }


    /**
     * fills buffer with data at appropriate places
     */
    void fillMessage(char address_, char cmd_, unsigned short length_, const char * data_)  {
      unsigned short crc_;

      if ((length_+5+1) > SIZE)
	throw Miro::Exception(" LaserMessage::fillMessage: could not fill message, buffer too small");

      buffer[0] = 0x02;
      buffer[1] = address_;
      buffer[2] = (length_+1) & 0xff;  // +1 for cmd
      buffer[3] = (unsigned char)(((length_+1) >> 8 ) & 0xff); // +1 for cmd
      buffer[4] = cmd_;
      if (data_) memcpy(&buffer[5], data_, length_);

      crc_=createcrc();

      buffer[5+length_] = crc_ & 0xff;
      buffer[5+length_+1] = char((crc_ >> 8) & 0xff);

    }

    /**
     * returns the address of the buffer for the message
     * 
     */
    const unsigned char* bufferAddress() const { return &buffer[0]; }

    /**
     * returns the size of the buffer for the message
     * (including everything, also STX, and CRC)
     */
    int                  bufferSize()    const { return length()+6; }

    /**
     * returns the address of the current message
     * 
     */
    unsigned char      address() const { return buffer[1]; }

    /**
     * returns the length of the current message (more description!!)
     * 
     */
    unsigned short      length() const { return (*((unsigned short *)(&buffer[2]))); }

    /**
     * returns the length of the current message data (more description!!)
     * excluding crc, but includung the command byte !
     *
     */
    unsigned short  datalength() const { return (*((unsigned short *)(&buffer[2])))-2; }

    /**
     * returns the command of the current message
     * 
     */
    unsigned char          cmd() const { return buffer[4]; }

    /**
     * returns a pointer to the current message data
     * 
     */
    const unsigned char * data() const { return &buffer[5]; }

    /**
     * returns the status of the current message
     * 
     */
    unsigned char       status() const { return buffer[length()+3]; }

    /**
     * returns the CRC of the current message
     * ( attention: only packages sent by the laser have this !
     *   we do not send the to the laser! )
     * @return the current CRC
     */
    unsigned short         crc() const { return (*((unsigned short *)(&buffer[length()+4]))); }

    /**
     *  some useful cast methods for getting typed data
     *  out of the buffer, 
     *  ATTENTION, offset is always in bytes !
     */

    unsigned short getUnsignedShort(unsigned long offset) const { 
      if ( offset > (bufferSize()-sizeof(unsigned short)) )
	throw Miro::Exception("LaserMessage::getUnsignedShort: range check violation");
      return (unsigned short) *(data() + offset) |
	     ((unsigned short) *(data() + offset + 1)) << 8;
    }
    signed short getSignedShort(unsigned long offset) const { 
      if ( offset > (bufferSize()-sizeof(signed short)) )
	throw Miro::Exception("LaserMessage::getSignedShort: range check violation");
      return (signed short) *(data() + offset) |
	     ((signed short) *(data() + offset + 1)) << 8;
    }
    unsigned long getUnsignedLong(unsigned long offset) const { 
      if ( offset > (bufferSize()-sizeof(unsigned long)) )
	throw Miro::Exception("LaserMessage::getUnsignedLong: range check violation");
      return (unsigned long) *(data() + offset) |
	     ((unsigned long) *(data() + offset + 1)) << 8 |
	     ((unsigned long) *(data() + offset + 2)) << 16 |
	     ((unsigned long) *(data() + offset + 3)) << 24;
    }
    signed long getSignedLong(unsigned long offset) const { 
      if ( offset > (bufferSize()-sizeof(signed long)) )
	throw Miro::Exception("LaserMessage::getSignedLong: range check violation");
      return (signed long) *(data() + offset) |
	     ((signed long) *(data() + offset + 1)) << 8 |
	     ((signed long) *(data() + offset + 2)) << 16 |
	     ((signed long) *(data() + offset + 3)) << 24;
    }
    unsigned int getUnsignedByte(unsigned long offset) const { 
      if ( offset > (bufferSize()-sizeof(unsigned char)) )
	throw Miro::Exception("LaserMessage::getUnsignedByte: range check violation");
      return int(*((unsigned char *)(data() + offset)));
    }
    signed int getSignedByte(unsigned long offset) const { 
      if ( offset > (bufferSize()-sizeof(signed char)) )
	throw Miro::Exception("LaserMessage::getSignedByte: range check violation");
      return int(*((signed char *)(data() + offset)));
    }

    /**
     * checks the CRC of the message
     *
     * The code is taken from the sick laser scanner manual
     * page 12/67
     *
     * @return true if CRC is correct, else false
     */
    bool              checkCrc() const {
      return (crc() == createcrc());
    }

    /**
     * creates the CRC to the current message
     *
     * @return the created CRC
     */
    unsigned short createcrc() const;
  

    /**
     * prints a LaserMessage object on the given stream.
     */
    friend ostream &operator<<(ostream &ostr, const LaserMessage &lm);
    void PrintDataOn(ostream &ostr) const;
    void PrintDumpOn(ostream &ostr) const;
  };

  inline 
  void LaserMessage::PrintDataOn(ostream &ostr) const {
    for (long i=0; i<(datalength()); ++i) 
      ostr << data()[i];
    ostr << endl;
    ostr << "Status : " << int(status()) << endl;
  }

  inline 
  void LaserMessage::PrintDumpOn(ostream &ostr) const {
    for (long i=0; i<length()+6; ++i)
      ostr << short(bufferAddress()[i]) << " ";
    ostr << "}" << endl;
  }

  inline
  unsigned short LaserMessage::createcrc() const
  {
    unsigned short crcsum = 0;
    unsigned int len = length()+4; // everything in the package except the crc
    unsigned char  abData[2];
    const unsigned char *buf = bufferAddress();
    
    abData[0] = 0;
    
    while (len --) {
      abData[1] = abData[0];
      abData[0] = *buf++;

      if (crcsum & 0x8000) {
	crcsum <<= 1;
	crcsum ^= 0x8005;
      } 
      else {
	crcsum <<= 1;
      }
      crcsum ^= *(uint *)&abData;
    }

    return crcsum;
}


  inline 
  ostream &operator<<(ostream &ostr, const LaserMessage &lm) {
    ostr << "[address:" << int(lm.address()) << 
      " length:" << lm.length() << 
      " cmd:" << int(lm.cmd()) << 
      " status:" << int(lm.status()) <<
      " crc:" << lm.crc() << "]{";

    for (long i=0; i<lm.length(); ++i)
      ostr << short(lm.data()[i]) << " ";
    ostr << "}" << endl;

    return ostr;
  }
};
#endif






