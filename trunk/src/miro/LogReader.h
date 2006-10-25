// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2001, 2002, 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef LogReader_h
#define LogReader_h

#include "LogHeader.h"
#include "LogTypeRepository.h"
#include "miro/SvcParameters.h"

#include <orbsvcs/CosNotificationC.h>
#include <tao/CDR.h>
#include <ace/Mem_Map.h>

#include <string>

namespace Miro
{

  class LogReader
  {
  public:
    //--------------------------------------------------------------------------
    // public methods
    //--------------------------------------------------------------------------

    //! Initializating constructor.
    LogReader(std::string const& _fileName) throw (Exception);

    TAO_InputCDR * istr() { return istr_; }
    char const * rdPtr() const throw ();
    void rdPtr(char const * _rdPtr) throw ();
    bool parseTimeStamp(ACE_Time_Value& _stamp) throw ();
    bool parseEventHeader(CosNotification::FixedEventHeader& _header) throw ();
    bool parseEventBody(CosNotification::StructuredEvent& _event) throw ();
    bool skipEventBody() throw ();

    //! Report the protocol version.
    unsigned short version() const throw ();
    //! Report the number of events in the log file.
    unsigned long events() const throw ();
    //! Flag indicating end of file.
    bool eof() const throw ();

    unsigned int progress() const throw ();
  protected:
    //--------------------------------------------------------------------------
    // protected data
    //--------------------------------------------------------------------------

    //! Memory mapped file, holding the log.
    ACE_Mem_Map memMap_;
    //! File header.
    LogHeader * header_;
    //! CDR stream to read from.
    TAO_InputCDR * istr_;
    //! Instance of the type repository.
    LogTypeRepository * typeRepository_;
    //! Next log entry in stream.
    char const * next_;

    //! Version number of the log file protocol.
    ACE_UINT16 version_;
    //! Offset of the type code repository in log file (version >= 3).
    ACE_UINT32 tcrOffset_;
    //! Number of events in log (version >= 3).
    ACE_UINT32 events_;

    //! Flag inidcating end of file.
    bool eof_;
  };

  inline
  char const * 
  LogReader::rdPtr() const throw () {
    return istr_->rd_ptr();
  }
  inline
  void
  LogReader::rdPtr(char const * _rdPtr) throw () {
    eof_ = false;

    delete istr_;
    if (version() == 2) {
      if (_rdPtr == NULL) {
	eof_ = true;
      }
	

      // TODO: solve this without operator new
      istr_ = new TAO_InputCDR(_rdPtr,
			       ((char *)memMap_.addr() + memMap_.size()) - _rdPtr,
			       (int)header_->byteOrder);
    }
    else {
      istr_ = new TAO_InputCDR(_rdPtr,
			       ((char *)memMap_.addr() + memMap_.size()) - _rdPtr);
    }

    return; 

    ACE_Message_Block const * mblock =
      istr_->start();
    char * rdPtr = const_cast<char *>(_rdPtr);

    const_cast<ACE_Message_Block *>(mblock)->rd_ptr(rdPtr);
    const_cast<ACE_Message_Block *>(mblock)->wr_ptr(rdPtr);
  }
  inline
  bool
  LogReader::eof() const throw () {
    return eof_;
  }
  inline
  unsigned short
  LogReader::version() const throw () {
    return version_;
  }
  inline
  unsigned long
  LogReader::events() const throw () {
    return events_;
  }
  inline
  bool
  LogReader::skipEventBody() throw ()
  {
    //    CosNotification::StructuredEvent event;
    //    return parseEventBody(event);

    if (eof_)
      return false;

    if (version() > 1) {
      rdPtr(next_);
    }
    else {
      CosNotification::OptionalHeaderFields variable_header;
      CosNotification::FilterableEventBody filterable_data;
      CORBA::Any remainder_of_body;
      if (!((*istr_) >> variable_header) ||
	  !((*istr_) >> filterable_data) ||
	  !((*istr_) >> remainder_of_body)) {
	eof_ = true;
	return false;
      }
    }

    return true;
  }
  inline
  unsigned int
  LogReader::progress() const throw () {
    return (unsigned int)((double) ((char *) istr_->rd_ptr() - 
				    (char *) memMap_.addr()) * 100. / 
			  (double) memMap_.size());
  }
}
#endif
