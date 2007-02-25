// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 1999-2005
// Department of Neuroinformatics, University of Ulm, Germany
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// $Id$
//
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
