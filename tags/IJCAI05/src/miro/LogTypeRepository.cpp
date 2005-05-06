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

#include "LogTypeRepository.h"
#include "Log.h"
#include "Exception.h"

#include <tao/CDR.h>

#include <algorithm>
#include <sstream>

namespace 
{

}

namespace Miro
{
  LogTypeRepository::LogTypeRepository(TAO_OutputCDR * _ostr,
				       size_t _maxLength) throw (CException):
    ostr_(_ostr),
    maxLength_(_maxLength),
    totalLength_(ostr_->total_length()),
    full_(false)
  {
    types_.reserve(64);

    pNumTypes_ = ostr_->current()->wr_ptr();
    ostr_->write_ulong(0x01235678);
  }

  LogTypeRepository::LogTypeRepository(TAO_InputCDR& _istr) throw (CException) :
    ostr_(NULL),
    maxLength_(),
    totalLength_(),
    full_(true)
  {
    //--------------------------------------------------------------------------
    // parse log repository
    //--------------------------------------------------------------------------

    // get number of types in cdr stream
    unsigned int numTypes;
    _istr.read_ulong(numTypes);
    types_.reserve(numTypes);

    MIRO_DBG_OSTR(MIRO, LL_DEBUG, "types: " << numTypes);

    // read each type from cdr stream
    for (unsigned int i = 0; i < numTypes; ++i) {
      
      // read type code from the mmapped file
      CORBA::TypeCode_ptr type;
      _istr >> type;

      // search for type code. fail, if already known
      MIRO_ASSERT(std::find(types_.begin(), types_.end(), type) == types_.end());

      // add type to our repository
      types_.push_back(CORBA::TypeCode::_duplicate(type));
    }
  }

  LogTypeRepository::~LogTypeRepository() throw ()
  {
    // free type codes
    TypeCodeVector::const_iterator first, last = types_.end();
    for (first = types_.begin(); first != last; ++first) {
      CORBA::release (*first);
    }
  }

  CORBA::Long
  LogTypeRepository::addType(CORBA::TypeCode_ptr _type)
  {
    // add type code to the mmapped file
    // FIXME: test for overrun
    (*ostr_) << _type;

    // if memory is used up, fail.
    if (ostr_->total_length() > maxLength_) {
      MIRO_LOG(Log::LL_ERROR, "Event log type code repository full.");
      full_ = true;
      return -2;
    }

    // add type to our repository
    types_.push_back(CORBA::TypeCode::_duplicate(_type));

    // increase type code counter
    TAO_OutputCDR len(pNumTypes_, 32);
    len.write_ulong(types_.size());

    // remember file length (for cleanup)
    totalLength_ = ostr_->total_length();

    return types_.size() - 1;
  }

}
