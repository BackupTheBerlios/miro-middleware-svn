// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
//
//////////////////////////////////////////////////////////////////////////////
#ifndef MiroObjectVectorImpl_h
#define MiroObjectVectorImpl_h

#include "idl/ObjectVectorS.h"

namespace Miro
{
  //! Implementation of the Obst interface.
  /**
   * This class offers a generic implementation for the Obst
   * interface. Simply pass the lowlevel Obst sensor data through
   * the integrateData method and all the rest will be taken care for
   * you.
   *
   * @author Hans Utz
   */
  class ObjectVectorImpl : public virtual POA_Miro::ObjectVector
  {
  public:
    //! Default constructor .
    ObjectVectorImpl();

    //! Virtual destructor.
    virtual ~ObjectVectorImpl();

    //! Method to pass raw Obst data from the device into the ObstImpl class.
    void integrateData(long _voltage)  ACE_THROW_SPEC(());

    //! Obst interface method implementation.
    mirrorObject* getObjectVector(ObjectType _objectType)throw();

    //virtual CORBA::Long getAktion() ACE_THROW_SPEC(());
    void setObjectVector(mirrorObject const& _art , ObjectType _objectType) throw();


  protected:
    //! The current Obst voltage.
    long voltage_;
    static const int objectNumber = 2;
    mirrorObject_var mObj_[objectNumber];
  };
};

#endif // ObjectVectorImpl_h
