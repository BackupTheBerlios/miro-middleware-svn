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
		   bool _tmpFile,
		   LogNotifyParameters const& _parameters) :
    parameters_(_parameters),
    fileName_(_fileName),
    tcrMap_((fileName_ + ".tcr").c_str(), parameters_.tCRFileSize,
	    O_RDWR | O_CREAT | O_TRUNC, ACE_DEFAULT_FILE_PERMS, PROT_RDWR, 
	    ACE_MAP_SHARED),
    tcrOstr_((char*)tcrMap_.addr(), tcrMap_.size()),
    typeRepository_(&tcrOstr_, parameters_.tCRFileSize),
    memMap_((fileName_ + ".dta").c_str(), parameters_.maxFileSize,
	    O_RDWR | O_CREAT | O_TRUNC, ACE_DEFAULT_FILE_PERMS, PROT_RDWR, 
	    ACE_MAP_SHARED),
    ostr_((char*)memMap_.addr(), memMap_.size()),
    lengthSlot_(NULL),
    tmpFile_(_tmpFile),
    totalLength_(0),
    full_(false)
  {
    if (memMap_.addr() == MAP_FAILED)
      throw CException(errno, std::strerror(errno));

  }

  LogWriter::~LogWriter()
  {
    //--------------------------------------------------------------------------
    // write the actual log file
    //--------------------------------------------------------------------------

    if (!tmpFile_) {
      try {
	writeFile();
      }
      catch (CException const& e) {
	MIRO_LOG_OSTR(LL_ERROR, "Error writing log file: " << e);
      }
    }

    //--------------------------------------------------------------------------
    // type code repository cleanup
    //--------------------------------------------------------------------------

    // close the memory mapped file
    tcrMap_.close();
    if (tmpFile_) {
      remove((fileName_+".tcr").c_str());
    }
    // truncate the file to the actual length
    else if (ACE_OS::truncate((fileName_ + ".tcr").c_str(), 
			      typeRepository_.totalLength()) == -1) {
      // We shouldn't throw in a destructor...
      MIRO_LOG_OSTR(LL_WARNING, 
		    "Error " << errno << 
		    " truncating log file " << fileName_ << ".tcr : " << std::endl
		    << std::strerror(errno));
    }

    //--------------------------------------------------------------------------
    // type code repository cleanup
    //--------------------------------------------------------------------------

    // close the memory mapped file
    memMap_.close();

    if (tmpFile_) {
      remove((fileName_ + ".dta").c_str());
    }
    // truncate the file to the actual length
    else if (ACE_OS::truncate((fileName_ + ".dta").c_str(), totalLength_) == -1) {
      // We shouldn't throw in a destructor...
      MIRO_LOG_OSTR(LL_WARNING, 
		    "Error " << errno << 
		    " truncating log file " << fileName_ << ".dta : " << std::endl
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
	
	//	std::cout << "." << std::flush;
	// set the length entry of the previous event
	if (lengthSlot_ != NULL) {
	  
	  //	  std::cout << "+" << std::flush;
	  // calculate length
	  char * here = ostr_.current()->wr_ptr() - sizeof(TimeBase::TimeT);
	  CORBA::ULong length = here - lengthSlot_;
	  
	  // dirty tricks on cdr stream...
	  ACE_Message_Block * mblock = 
	    const_cast<ACE_Message_Block *>(ostr_.current());
	  // safe the write ptr
	  char * tmp = mblock->wr_ptr();
	  // set the length
	  mblock->wr_ptr(lengthSlot_);
	  ostr_.write_ulong(length);
	  // restore write ptr of the cdr stream
	  mblock->wr_ptr(tmp);
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
	  //	  std::cout << "." << std::flush;

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
	    //	    std::cout << "." << std::endl;
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
#else // TAO_MINOR_VERSION <= 2
	      
	      TAO_Marshal_Object::perform_append (tc.in (),
						  &input,
						  &ostr_,
						  ACE_TRY_ENV);
	      ACE_TRY_CHECK;
#endif
	    }
	    ACE_CATCH (CORBA_Exception, ex) {
	      full_ = true;
	      return false;
	    }
	    ACE_ENDTRY;

	    if (ostr_.total_length() <= parameters_.maxFileSize) {
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
  LogWriter::writeFile() throw (CException)
  {
    ACE_FILE_Addr fileName(fileName_.c_str());
    ACE_FILE_IO buffer;
    ACE_FILE_Connector connector;

    if (connector.connect(buffer, 
			  fileName, 
			  NULL,
			  ACE_Addr::sap_any, 
			  0,
			  O_WRONLY | O_CREAT | O_TRUNC) == -1) {
      throw CException(errno, std::strerror(errno));
    }

    LogHeader::WRITE w;
    LogHeader header(w);

    char alignementBuffer[0x08];
    ACE_OS::memset(alignementBuffer, 0, 0x08);

    iovec iov[4];
    iov[0].iov_base = &header;
    iov[0].iov_len = sizeof(LogHeader);

    iov[1].iov_base = tcrMap_.addr();
    iov[1].iov_len = typeRepository_.totalLength();

    // cdr stream alignement
    iov[2].iov_base = alignementBuffer;
    iov[2].iov_len = (0x08 - (typeRepository_.totalLength() & 0x07)) & 0x07;

    iov[3].iov_base = memMap_.addr();
    iov[3].iov_len = totalLength_;

    if (buffer.send(iov, 4) == -1)
      throw CException(errno, std::strerror(errno)); 

    buffer.close();

    setTmp();
  }
}
