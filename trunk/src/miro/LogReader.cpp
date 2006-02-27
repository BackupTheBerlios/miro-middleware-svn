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

#include "LogReader.h"
#include "LogHeader.h"
#include "LogTypeRepository.h"
#include "Log.h"
#include "Exception.h"
#include "TimeHelper.h"

#include "idl/TimeC.h"

#include <orbsvcs/Time_Utilities.h>
#include <orbsvcs/CosNotificationC.h>

#include <tao/Version.h>
#if (TAO_MAJOR_VERSION > 1) || \
  ( (TAO_MAJOR_VERSION == 1) && (TAO_MINOR_VERSION > 4) ) || \
  ( (TAO_MAJOR_VERSION == 1) && (TAO_MINOR_VERSION == 4) && (TAO_BETA_VERSION > 7) )
#  include <tao/AnyTypeCode/Any_Impl.h>
#  include <tao/AnyTypeCode/Any_Unknown_IDL_Type.h>
#elif ( (TAO_MAJOR_VERSION == 1) && (TAO_MINOR_VERSION == 4) )
#  include <tao/Any_Impl.h>
#  include <tao/Any_Unknown_IDL_Type.h>
#else
#  include <tao/Marshal.h>
#  include <tao/Typecode.h>
#  include <tao/Environment.h>
#endif

#include <cstdio>

namespace Miro
{
  LogReader::LogReader(std::string const& _fileName) throw (Exception) :
    memMap_(_fileName.c_str(), -1, O_RDONLY),
    header_(NULL),
    istr_(NULL),
    typeRepository_(NULL),
    next_(NULL),
    version_(1),
    tcrOffset_(sizeof(LogHeader)),
    events_(0),
    eof_(false)
  {
    if (memMap_.addr() == MAP_FAILED)
      throw CException(errno, std::strerror(errno));

    try {
      MIRO_DBG(MIRO, LL_DEBUG, "Looking for log file type.");
      LogHeader::READ r;
      header_ = new (memMap_.addr()) LogHeader(r);
      version_ = header_->version;

      istr_ = new TAO_InputCDR((char*)memMap_.addr() + sizeof(LogHeader),
			       memMap_.size() - sizeof(LogHeader),
			       (int)header_->byteOrder);
      // version 2 log file
      if (version() == 2) {
	istr_ = new TAO_InputCDR((char*)memMap_.addr() + sizeof(LogHeader),
				 memMap_.size() - sizeof(LogHeader),
				 (int)header_->byteOrder);
	typeRepository_ = new LogTypeRepository(*istr_);
      }
      // version 3 log file
      else if (version() == 3) {
	istr_ = new TAO_InputCDR((char*)memMap_.addr() + sizeof(LogHeader),
				 memMap_.size() - sizeof(LogHeader),
				 (int)header_->byteOrder);

	istr_->read_ulong(tcrOffset_);
	istr_->read_ulong(events_);

	MIRO_DBG_OSTR(MIRO, LL_DEBUG, 
		      "LogReader - TCR Offset: 0x" << 
		      std::hex << tcrOffset_ << std::dec << std::endl <<
		      "LogReader - Number of events: " << 
		      events_ << std::dec);

	TAO_InputCDR tcrIStream((char*)memMap_.addr() + tcrOffset_,
				memMap_.size() - tcrOffset_,
				(int)header_->byteOrder);
	typeRepository_ = new LogTypeRepository(tcrIStream);


	MIRO_DBG_OSTR(MIRO, LL_PRATTLE, "LogReader - Good bit : " << istr_->good_bit());
      }
      else 
	throw Exception("Unknown log format version.");
    }
    catch (Miro::LogHeader::EFileType const&) {

      // version 1 log file
      istr_ = new TAO_InputCDR((char*)memMap_.addr(), memMap_.size());
    }
    MIRO_DBG_OSTR(MIRO, LL_DEBUG,  "version : " << version_);
  }

  bool
  LogReader::parseTimeStamp(ACE_Time_Value& _stamp) throw ()
  {
    if (eof_ ||
	istr_->length() == 0 ||
	istr_->rd_ptr() == 0)
      return false;

    // version >= 2 parsing
    if (version() >= 2) {

      // get the time stamp
      TimeBase::TimeT t;
      if (!(istr_->read_ulonglong(t))) {
	MIRO_DBG(MIRO, LL_DEBUG, "eof 1");
	eof_ = true;
	return false;
      }

      if (next_ != NULL &&
	  next_ != istr_->rd_ptr() - sizeof(TimeBase::TimeT)) {
	MIRO_LOG_OSTR(LL_ERROR, 
		      "mismatch: " << (void*)next_ <<
		      "\t " << (void *)(istr_->rd_ptr() - sizeof(TimeBase::TimeT)));
      }

      ORBSVCS_Time::TimeT_to_Time_Value(_stamp, t);
    }
    // version 1 parsing
    else {
      TimeIDL timeStamp;
      if (!((*istr_) >> timeStamp)) {
	MIRO_DBG(MIRO, LL_DEBUG, "eof 2");
	eof_ = true;
	return false;
      }

      timeC2A(timeStamp, _stamp);
    }
    
    if (_stamp == ACE_Time_Value::zero) {
      MIRO_DBG(MIRO, LL_DEBUG, "eof 3");
      eof_ = true;
      return false;
    }
    return true;
  }
  
  bool
  LogReader::parseEventHeader(CosNotification::FixedEventHeader& _header) throw ()
  {
    if (eof_)
      return false;

    // version >= 2 parsing
    if (version() >= 2) {
      // get address of next event
      char const * here = istr_->start()->rd_ptr();
      unsigned int len;
      if (!istr_->read_ulong(len)) {
	MIRO_DBG(MIRO, LL_DEBUG, "eof h1");
	eof_ = true;
	return false;
      }
      if (len == 0)
	eof_ = true;

      next_ = here + len;

      // get header
      if (!((*istr_) >> _header)) {
	eof_ = true;
	return false;
      }
    }
    // version 1 parsing
    else {
      if (!((*istr_) >> _header)) {
	eof_ = true;
	return false;
      }
    }
    return true;
  }
  
  bool
  LogReader::parseEventBody(CosNotification::StructuredEvent& _event) throw ()
  {
    if (eof_) {
      MIRO_DBG(MIRO, LL_DEBUG, "eof e1");
      return false;
    }

    // get the rest of the header (variable header) 
    // get the filterable data
    if (!((*istr_) >> _event.header.variable_header) ||
	!((*istr_) >> _event.filterable_data)) {
      MIRO_DBG(MIRO, LL_DEBUG, "eof e2");
      eof_ = true;
      return false;
    }

    // parse the any
    if (version() >= 2) {

      // get type code from repository
      int id;
      if (!istr_->read_long(id)) {
	eof_ = true;
	return false;
      }

      CORBA::TypeCode_ptr tc = typeRepository_->typeCode(id);
    
      if (tc == CORBA::_tc_null) {
	_event.remainder_of_body = CORBA::Any();
	return true;

      }

      // direct copies from TAO sources
      // read the any payload

#if (TAO_MAJOR_VERSION > 1) || \
  ( (TAO_MAJOR_VERSION == 1) && (TAO_MINOR_VERSION > 4) ) || \
  ( (TAO_MAJOR_VERSION == 1) && (TAO_MINOR_VERSION == 4) && (TAO_BETA_VERSION > 3) )

      try {

	TAO::Unknown_IDL_Type *impl = new TAO::Unknown_IDL_Type (tc);
	_event.remainder_of_body.replace (impl);
	impl->_tao_decode (*istr_);
      }
      catch (CORBA::Exception const&) {
	eof_ = true;
	return false;
      }

#elif \
  ( (TAO_MAJOR_VERSION == 1) && (TAO_MINOR_VERSION == 4) ) || \
  ( (TAO_MAJOR_VERSION == 1) && (TAO_MINOR_VERSION == 3) && (TAO_BETA_VERSION > 4) ) 

      try {

	TAO::Unknown_IDL_Type *impl = new TAO::Unknown_IDL_Type (tc,
								 0,
								 istr_->byte_order ());
	_event.remainder_of_body.replace (impl);
	impl->_tao_decode (*istr_);
      }
      catch (CORBA::Exception const&) {
	eof_ = true;
	return false;
      }
#else
      ACE_TRY_NEW_ENV {

	// This will be the start of a new message block.
	char *begin = istr_->rd_ptr ();

	// Skip over the next aregument.
#if \
  ( (TAO_MAJOR_VERSION == 1) && (TAO_MINOR_VERSION == 3) ) || \
  ( (TAO_MAJOR_VERSION == 1) && (TAO_MINOR_VERSION == 2) && (TAO_BETA_VERSION > 1) )
	CORBA::TypeCode::traverse_status status =
	  TAO_Marshal_Object::perform_skip (tc,
					    istr_
					    ACE_ENV_ARG_PARAMETER);
#else // TAO_MINOR_VERSION <= 1
	CORBA::TypeCode::traverse_status status =
	  TAO_Marshal_Object::perform_skip (tc, istr_, ACE_TRY_ENV);
#endif
	ACE_TRY_CHECK;

	if (status != CORBA::TypeCode::TRAVERSE_CONTINUE) {
	  eof_ = true;
	  return false;
        }

	// This will be the end of the new message block.
	char *end = istr_->rd_ptr ();

	// The ACE_CDR::mb_align() call can shift the rd_ptr by up to
	// ACE_CDR::MAX_ALIGNMENT-1 bytes. Similarly, the offset adjustment
	// can move the rd_ptr by up to the same amount. We accommodate
	// this by including 2 * ACE_CDR::MAX_ALIGNMENT bytes of additional
	// space in the message block.
	size_t size = end - begin;
	ACE_Message_Block mb (size + 2 * ACE_CDR::MAX_ALIGNMENT);
	ACE_CDR::mb_align (&mb);
	ptr_arith_t offset = ptr_arith_t (begin) % ACE_CDR::MAX_ALIGNMENT;
	mb.rd_ptr (offset);
	mb.wr_ptr (offset + size);
	ACE_OS::memcpy (mb.rd_ptr (), begin, size);
	
	// Stick it into the Any. It gets duplicated there.
	_event.remainder_of_body._tao_replace (tc,
					       istr_->byte_order (),
					       &mb);
      }
      ACE_CATCH (CORBA_Exception, ex) {
	eof_ = true;
	return false;
      }
      ACE_ENDTRY;

#endif
    }
    // version 1 parsing
    else {
      if (!((*istr_) >> _event.remainder_of_body)) {
	eof_ = true;
	return false;
      }
    }

    return true;
  }
}
