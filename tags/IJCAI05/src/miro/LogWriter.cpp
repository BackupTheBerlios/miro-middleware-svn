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

#include "LogWriter.h"
#include "LogTypeRepository.h"
#include "Log.h"
#include "Exception.h"

#include <orbsvcs/Time_Utilities.h>

#include <tao/Version.h>
#if (TAO_MAJOR_VERSION > 1) || \
  ((TAO_MAJOR_VERSION == 1) && (TAO_MINOR_VERSION >= 4))
#include <tao/Any_Impl.h>
#else
#include <tao/Marshal.h>
#endif

#include <ace/FILE_Connector.h>

#include <cstdio>

namespace Miro
{
  LogWriter::LogWriter(std::string const& _fileName,
		       LogNotifyParameters const& _parameters) :
    parameters_(_parameters),
    fileName_(_fileName),
    memMap_((fileName_).c_str(), parameters_.maxFileSize,
	    O_RDWR | O_CREAT | O_TRUNC, ACE_DEFAULT_FILE_PERMS, PROT_RDWR, 
	    ACE_MAP_SHARED),
    header_(new(memMap_.addr()) LogHeader(w_)),
    ostr_((char*)memMap_.addr() + sizeof(LogHeader), 
	  memMap_.size() - sizeof(LogHeader) - parameters_.tCRFileSize),
    tcrOstr_((char*)memMap_.addr() + memMap_.size() - parameters_.tCRFileSize,
	     parameters_.tCRFileSize),
    typeRepository_(&tcrOstr_, parameters_.tCRFileSize),
    tcrOffsetSlot_(NULL),
    numEventsSlot_(NULL),
    numEvents_(0UL),
    lengthSlot_(NULL),
    totalLength_(0),
    full_(false)
  {
    if (memMap_.addr() == MAP_FAILED)
      throw CException(errno, std::strerror(errno));

    // The alignement is okay as we wrote 8 bytes of LogHeader
    tcrOffsetSlot_ = ostr_.current()->wr_ptr();
    ostr_.write_ulong(memMap_.size() - parameters_.tCRFileSize);

    // The allignement is okay as we write now a ulong.
    numEventsSlot_ = ostr_.current()->wr_ptr();
    ostr_.write_ulong(0);
  }

  LogWriter::~LogWriter()
  {
    //--------------------------------------------------------------------------
    // place type codes at the end of the event stream
    //--------------------------------------------------------------------------

    packTCR();

    //--------------------------------------------------------------------------
    // log file size cleanup
    //--------------------------------------------------------------------------

    // close the memory mapped file
    memMap_.close();

    if (ACE_OS::truncate((fileName_).c_str(), totalLength_) == -1) {
      // We shouldn't throw in a destructor...
      MIRO_LOG_OSTR(LL_WARNING, 
		    "Error " << errno << 
		    " truncating log file " << fileName_  << std::endl
		    << std::strerror(errno));
    }
  }

  bool 
  LogWriter::logEvent(ACE_Time_Value const& _stamp,
		      CosNotification::StructuredEvent const& _event)
  {
    // if there is place in the log file
    if (!full_) {

      // set the time stamp
      TimeBase::TimeT t;
      ORBSVCS_Time::Time_Value_to_TimeT(t, _stamp);
      
      // write time stamp
      if (ostr_.write_ulonglong(t)) {
	
	// set the length entry of the previous event
	if (lengthSlot_ != NULL) {
	  
	  // calculate length
	  char * here = ostr_.current()->wr_ptr() - sizeof(TimeBase::TimeT);
	  CORBA::ULong length = here - lengthSlot_;
	  
	  // write the length of the previous event
	  // direct writing is allowed, 
	  // as the alignement is correct and we write in host byte order
	  * reinterpret_cast<ACE_INT32 *>(lengthSlot_) = length;
	}
	
	
	// The alignement is okay as we wrote an ulonglong before
	lengthSlot_ = ostr_.current()->wr_ptr();

	// write the length slot
	if (ostr_.write_ulong(0) &&
	    // write the header
	    ostr_ << _event.header && 
	    // write the filterable data
	    ostr_ << _event.filterable_data) {
	  
	  
	  // serialize remainder_of_body

	  // obtain type code id
	  CORBA::Long typeId = -1;
	  CORBA::TypeCode_var tc = _event.remainder_of_body.type();
	  if (tc.in() != CORBA::_tc_null) {
	    typeId = typeRepository_.typeID(tc.in());
	  }
	  
#if (TAO_MAJOR_VERSION > 1) || \
  ((TAO_MAJOR_VERSION == 1) && (TAO_MINOR_VERSION >= 4)) || \
  ((TAO_MAJOR_VERSION == 1) && (TAO_MINOR_VERSION == 3 && TAO_BETA_VERSION >= 5)) 

	      //----------------------------------------------------------------
	      // we have Any_Impl
	      //----------------------------------------------------------------
	      
	  // if not type code repository full
	  if (typeId != -2 && 
	      // write type code id
	      ostr_.write_long(typeId) &&
	      // write any value if existent
	      ( _event.remainder_of_body.impl() == NULL ||
		_event.remainder_of_body.impl()->marshal_value(ostr_)) &&
	      // not max file size reached
	      (ostr_.total_length() <= parameters_.maxFileSize)) {

	    // write number of events
	    // direct writing is allowed, 
	    // as the alignement is correct and we write in host byte order
	    *reinterpret_cast<ACE_INT32 *>(numEventsSlot_) = ++numEvents_;

	    totalLength_ = ostr_.total_length();
	    return true;
	  }
#else
	  // if not type code repository full
	  if (typeId != -2 && 
	      // write type code id
	      ostr_.write_long(typeId)) {

	    ACE_TRY_NEW_ENV {
	      TAO_InputCDR input(_event.remainder_of_body._tao_get_cdr (),
				 _event.remainder_of_body._tao_byte_order ());
	      
#if ((TAO_MAJOR_VERSION == 1) && (TAO_MINOR_VERSION == 3))
	      //----------------------------------------------------------------
	      // we have ACE_ENV_ARG_PARAMETER
	      //----------------------------------------------------------------
	         
	      CORBA::TypeCode::traverse_status status =
		TAO_Marshal_Object::perform_append (tc.in(),
						    &input,
						    &ostr_
						    ACE_ENV_ARG_PARAMETER);
	      ACE_TRY_CHECK;
	      
	      if (status != CORBA::TypeCode::TRAVERSE_CONTINUE) {
		full_ = true;
		return false;
	      }
#else

#if ((TAO_MAJOR_VERSION == 1) && (TAO_MINOR_VERSION == 2) && (TAO_BETA_VERSION >= 2))


	      TAO_Marshal_Object::perform_append (tc.in (),
						  &input,
						  &ostr_
						  ACE_ENV_ARG_PARAMETER);
#else // TAO_MINOR_VERSION <= 2
	      
	      TAO_Marshal_Object::perform_append (tc.in (),
						  &input,
						  &ostr_,
						  ACE_TRY_ENV);
#endif

	      ACE_TRY_CHECK;
#endif
	    }
	    ACE_CATCH (CORBA_Exception, ex) {
	      full_ = true;
	      return false;
	    }
	    ACE_ENDTRY;

	    if (ostr_.total_length() <= parameters_.maxFileSize) {

	      // write number of events
	      // direct writing is allowed, 
	      // as the alignement is correct and we write in host byte order
	      *reinterpret_cast<ACE_INT32 *>(numEventsSlot_) = ++numEvents_;

	      totalLength_ = ostr_.total_length();
	      return true;
	    }
	  }
#endif
	}
      }
    
      MIRO_LOG_OSTR(Log::LL_ERROR, 
		    "Event log data - max file size reached:" << 
		    totalLength_ <<
		    " - Event logging stopped.");
      full_ = true;
    }
    return false;
  }

  void
  LogWriter::packTCR() throw (CException)
  {
    char * dest = (char *)memMap_.addr();
    ACE_UINT32 offset = sizeof(LogHeader) + totalLength_;

    // 8 byte alignement
    offset += (0x08 - (totalLength_ & 0x07)) & 0x07;

    dest += offset;
    memcpy(dest, tcrOstr_.current()->base(), typeRepository_.totalLength());

    // note new location of tcr
    TAO_OutputCDR o_(tcrOffsetSlot_, 8);
    o_.write_ulong(offset);

    MIRO_DBG_OSTR(MIRO, LL_DEBUG,
		  "Offset" << std::hex << offset <<
		  "Events" << numEvents_ << std::dec);

    // get total length of the file
    totalLength_ = offset + typeRepository_.totalLength();
    MIRO_DBG_OSTR(MIRO, LL_DEBUG,
		  "TCR length: " <<  typeRepository_.totalLength());
  }
}
