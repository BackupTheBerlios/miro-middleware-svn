
//----------------------------------------------------------------
// Copyright (C) 2005
// Technische Universitaet Graz
//
// This file is part of the LD OEM Sick Scanner library
//----------------------------------------------------------------

//----------------------------------------------------------------
// project ....: Robot Processing Library
// file .......: ldObject.h
// authors ....: Thomas Sega
// organization: TU - Graz
// creation ...: 02.12.2005 v1.0.0
// revisions ..:
//----------------------------------------------------------------


#ifndef _LD_OBJECT_H_
  #define _LD_OBJECT_H_

namespace ldoem_
{
  //---------------------------------------------------------------------------
  /// base class for all objects
  class Object
  {
  public:

    //---------------------------------------------------------------------------
    /// default constructor
    Object();

    //---------------------------------------------------------------------------
    /// destructor
    virtual ~Object();

    //---------------------------------------------------------------------------
    /// returns the name of this class. Each class has to
    /// overload this function and return its name. The returned
    /// string is system-independent; however, template classes
    /// do not consider the template argument.
    virtual const char * getTypeName() const;

  protected:

    //---------------------------------------------------------------------------
    /// this flag indicates if any ldoem-object has been constructed
    static bool initialized_;

    //---------------------------------------------------------------------------
    /// member which initialize everything for the ldoem object
    void initialize();
  };

  inline bool isNull( const void * p )
  {
    return p == 0;
  };
  inline bool notNull( const void * p )
  {
    return p != 0;
  };
}
#endif //_LD_OBJECT_H_
