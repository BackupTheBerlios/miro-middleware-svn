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
#ifndef LogWriter_h
#define LogWriter_h

#include "LogHeader.h"
#include "LogTypeRepository.h"
#include "StructuredPushConsumer.h"
#include "SvcParameters.h"

#include <ace/Mem_Map.h>
#include <ace/High_Res_Timer.h>
#include <tao/CDR.h>

#include <string>

namespace Miro
{
  class LogWriter
  {
  public:
    //--------------------------------------------------------------------------
    // public methods
    //--------------------------------------------------------------------------

    //! Initializating constructor.
    LogWriter(std::string const& _fileName,
	    bool _tmpFile = false,
	    LogNotifyParameters const& _parameters = *LogNotifyParameters::instance());
    //! Disconnect from the supplier.
    ~LogWriter();

    //! Set file to be temporary.
    void setTmp();

    //! Inherited IDL interface: StructuredPushSupplier method
    bool logEvent(ACE_Time_Value const& _stamp,
		  CosNotification::StructuredEvent const& _event);
    //! Report the protocol version.
    unsigned short version() const;

  protected:
    //--------------------------------------------------------------------------
    // protected methods
    //--------------------------------------------------------------------------

    void writeFile() throw (CException);

    //--------------------------------------------------------------------------
    // protected data
    //--------------------------------------------------------------------------

    //! Reference to the parameters.
    LogNotifyParameters const& parameters_;
    //! The name of the log file.
    std::string const fileName_;
    //! Memory mapped file, holding the tyoe code repository.
    ACE_Mem_Map tcrMap_;
    //! CDR stream to log type codes to.
    TAO_OutputCDR tcrOstr_;
    //! Instance of the type repository.
    LogTypeRepository typeRepository_;
    //! Memory mapped file, holding the log.
    ACE_Mem_Map memMap_;
    //! CDR stream to log to.
    TAO_OutputCDR ostr_;
    //! Slot to write the length of the serialized structured event.
    /** This is used for skipped parsing of the file. */
    char * lengthSlot_;
    //! Flag indicating the file should be removed on cleanup.
    bool tmpFile_;
    //! The length of the cdr stream
    size_t totalLength_;
    //! Flag indicating that the file is full.
    bool full_;
  };

  inline
  void
  LogWriter::setTmp() {
    tmpFile_ = true;
  }
  inline
  unsigned short
  LogWriter::version() const {
    return LogHeader::PROTOCOL_VERSION;
  }
}
#endif