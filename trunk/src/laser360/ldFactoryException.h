
//----------------------------------------------------------------
// Copyright (C) 2005
// Technische Universitaet Graz
//
// This file is part of the LD OEM Sick Scanner library
//----------------------------------------------------------------

//----------------------------------------------------------------
// project ....: Robot Processing Library
// file .......: ldFactoryException.h
// authors ....: Thomas Sega
// organization: TU - Graz
// creation ...: 02.12.2005 v1.0.0
// revisions ..:
//----------------------------------------------------------------

#ifndef _LD_FACTORY_EXCEPTION_H_
  #define _LD_FACTORY_EXCEPTION_H_

  #include <string>
  #include "ldBaseException.h"

namespace ldoem_
{

  //----------------------------------------------------------------------------
  ///
  /// Factory Exception class
  class TgmFactoryException : public BaseException
  {
  public:
    //----------------------------------------------------------------------------
    /// constructor
    ///
    /// @param msg Errortext that you can get afterwards with getMessage()
    TgmFactoryException( const std::string msg ) : BaseException( "TgmFactoryException: " + msg )
    {
    };

  };
}; // end namespace ldoem_

#endif // _LD_FACTORY_EXCEPTION_H_
