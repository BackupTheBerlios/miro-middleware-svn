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

#include <tao/Any_Impl.h>
#include "tao/Any_Unknown_IDL_Type.h"

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
    eof_(false)
  {
    if (memMap_.addr() == MAP_FAILED)
      throw CException(errno, std::strerror(errno));

    try {
      std::cout << "looking for log file type" << std::endl;
      LogHeader::READ r;
      header_ = new (memMap_.addr()) LogHeader(r);
      version_ = header_->version;
      // version 2 log file
      istr_ = new TAO_InputCDR((char*)memMap_.addr() + sizeof(LogHeader),
			       memMap_.size() - sizeof(LogHeader),
			       (int)header_->byteOrder);
      typeRepository_ = new LogTypeRepository(*istr_);
    }
    catch (Miro::LogHeader::EFileType const&) {

      // version 1 log file
      istr_ = new TAO_InputCDR((char*)memMap_.addr(), memMap_.size());
    }
    std::cout << "version : " << version_ << std::endl;
  }

  bool
  LogReader::parseTimeStamp(ACE_Time_Value& _stamp) throw ()
  {
    if (eof_ ||
	istr_->length() == 0 ||
	istr_->rd_ptr() == 0)
      return false;

    // version 2 parsing
    if (version() == 2) {

      // get the time stamp
      TimeBase::TimeT t;
      if (!(istr_->read_ulonglong(t))) {
	std::cout << "eof 1" << std::endl;
	eof_ = true;
	return false;
      }

      if (next_ != NULL &&
	  next_ != istr_->rd_ptr() - sizeof(TimeBase::TimeT)) {
	std::cout << "mismatch: " << (void*)next_ 
		  << "\t " << (void *)(istr_->rd_ptr() - sizeof(TimeBase::TimeT))
		  << std::endl;
      }

      //      std::cout << "prt: 0x" << (void*) istr_->rd_ptr()
      //		<< "  time: 0x" << std::hex << t << std::dec << std::endl;

      ORBSVCS_Time::TimeT_to_Time_Value(_stamp, t);
    }
    // version 1 parsing
    else {
      TimeIDL timeStamp;
      if (!((*istr_) >> timeStamp)) {
	std::cout << "eof 2" << std::endl;
	eof_ = true;
	return false;
      }

      timeC2A(timeStamp, _stamp);
    }
    
    if (_stamp == ACE_Time_Value::zero) {
      std::cout << "eof 3" << std::endl;
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

    // version 2 parsing
    if (version() == 2) {
      //      std::cout << "h" << std::flush;
      // get address of next event
      char const * here = istr_->start()->rd_ptr();
      unsigned int len;
      if (!istr_->read_ulong(len)) {
	std::cout << "eof h1" << std::flush;
	eof_ = true;
	return false;
      }
      if (len == 0)
	eof_ = true;

      next_ = here + len;

      /*
      std::cout << std::endl
		<< "rd: " << (void *)istr_->start()->rd_ptr() << std::endl
		<< "wr: " << (void *)istr_->start()->wr_ptr() << std::endl
		<< "len: " << len << std::endl;
      */

      // get header
      if (!((*istr_) >> _header)) {
	eof_ = true;
	return false;
      }
      //      std::cout << "h" << std::endl;
    }
    // version 1 parsing
    else {
      if (!((*istr_) >> _header)) {
	std::cout << "-" << std::endl;
	eof_ = true;
	return false;
      }
    }
    return true;
  }
  
  bool
  LogReader::parseEventBody(CosNotification::StructuredEvent& _event) throw ()
  {
    //    std::cout << "parse event body" << std::endl;

    if (eof_) {
      std::cout << "eof 1" << std::endl;
      return false;
    }

    // get the rest of the header (variable header) 
    // get the filterable data
    if (!((*istr_) >> _event.header.variable_header) ||
	!((*istr_) >> _event.filterable_data)) {
      std::cout << "eof 2" << std::endl;
      eof_ = true;
      return false;
    }

    //    std::cout << "." << std::flush;

    // parse the any
    if (version() == 2) {


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

      try {

	// direct copy from TAO sources
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
    }
    // version 1 parsing
    else {
      //      std::cout << "parse event body" << std::endl;
      if (!((*istr_) >> _event.remainder_of_body)) {
	eof_ = true;
	return false;
      }
    }

    return true;
  }
}
