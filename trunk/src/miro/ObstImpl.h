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
#ifndef MiroObstImpl_h
#define MiroObstImpl_h

#include "idl/ObstS.h"

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
  class ObstImpl : public virtual POA_Miro::Obst
  {
  public:
    //! Default constructor .
    ObstImpl();

    //! Virtual destructor.
    virtual ~ObstImpl();

    //! Method to pass raw Obst data from the device into the ObstImpl class.
    void integrateData(long _voltage)  ACE_THROW_SPEC(());

    //! Obst interface method implementation.
    mirrorObject* getObst(ObjectType _objectType)throw();

    //virtual CORBA::Long getAktion() ACE_THROW_SPEC(());
    void setObst(mirrorObject const& _art , ObjectType _objectType) throw();


  protected:
    //! The current Obst voltage.
    long voltage_;
    static const int objectNumber = 2;
    mirrorObject_var mObj_[objectNumber];
  };
};

#endif // ObstImpl_h
