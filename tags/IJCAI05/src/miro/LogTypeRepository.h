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
#ifndef LogTypeRepository_h
#define LogTypeRepository_h

#include "Exception.h"

#include <ace/Mem_Map.h>
#include <tao/Typecode.h>
#include <tao/CDR.h>

#include <string>
#include <vector>
#include <functional>
#include <algorithm>

// forward declaration
class TAO_OutputCDR;
class TAO_InputCDR;

namespace Miro
{
  class LogTypeRepository 
  {
  public:
    //--------------------------------------------------------------------------
    // public methods
    //--------------------------------------------------------------------------
    //! Initializing constructor for output stream.
    LogTypeRepository(TAO_OutputCDR * _ostr, size_t _maxLength = 1024*1024) throw (CException);
    //! Initializing constructor for input stream.
    LogTypeRepository(TAO_InputCDR& _istr) throw (CException);

    //! Cleaning up.
    virtual ~LogTypeRepository() throw ();

    //! Return the type id for the the type code.
    /**
     * If the type code already exits in the repository the id is returned.
     * Otherwise, the type code is added to the repository first.
     *
     * -1 is returned if the type code repository if full.
     */
    CORBA::Long typeID(CORBA::TypeCode_ptr _type);

    //! Return the type code corresponding to the type id.
    /** NULL if the type id is unknown. */
    CORBA::TypeCode_ptr typeCode(long _typeID);

    //! Maximum size of the repository backing storage.
    size_t totalLength() const;

  private:
    //--------------------------------------------------------------------------
    // private types
    //--------------------------------------------------------------------------

    typedef std::vector<CORBA::TypeCode_ptr> TypeCodeVector;
    struct TCEq : public std::unary_function<char const *, bool>
    {
      TCEq(CORBA::TypeCode_ptr _tc) : tc_(_tc) {}
      
      bool operator()(CORBA::TypeCode_ptr _tc) const {
	return tc_->equal(_tc);
      }
    protected:
      CORBA::TypeCode_ptr tc_;
    };


    //--------------------------------------------------------------------------
    // private methods
    //--------------------------------------------------------------------------

    //! Add type to the repository
    CORBA::Long addType(CORBA::TypeCode_ptr _type);

    //--------------------------------------------------------------------------
    // private data
    //--------------------------------------------------------------------------

    //! CDR stream to log to.
    TAO_OutputCDR * ostr_;
    size_t maxLength_;
    size_t totalLength_;
    bool full_;
    char * pNumTypes_;

    TypeCodeVector types_;
  };

  inline
  size_t
  LogTypeRepository::totalLength() const {
    return totalLength_;
  }

  inline
  CORBA::Long
  LogTypeRepository::typeID(CORBA::TypeCode_ptr _type) {
    // search for type code
    TypeCodeVector::const_iterator where = 
      std::find_if(types_.begin(), types_.end(), TCEq(_type));
    // return its index, if already known
    if (where != types_.end()) {
      return where - types_.begin();
    }
    // add type code to the repository, and return the index
    return addType(_type);
  }

  inline
  CORBA::TypeCode_ptr
  LogTypeRepository::typeCode(long _id) {
    CORBA::TypeCode_ptr tc =  CORBA::_tc_null;
    if (0 <= _id && _id < (int)types_.size()) {
      tc = types_[_id];
    }
    return tc;
  }
}
#endif
