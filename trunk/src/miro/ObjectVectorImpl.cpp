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
#include "ObjectVectorImpl.h"
#include "TimeHelper.h"

namespace Miro
{
  // Implementation skeleton constructor
  ObjectVectorImpl::ObjectVectorImpl() :
    voltage_(-1) // uninicialized value
  {
    for (int i=0; i<objectNumber; ++i) {
      mObj_[i] = new mirrorObject();
      mObj_[i]->data.length(10);
      mObj_[i]->data[0]=421;
    }
  }

  // Implementation skeleton destructor
  ObjectVectorImpl::~ObjectVectorImpl()
  {
  }

  void
  ObjectVectorImpl::integrateData(long _voltage) ACE_THROW_SPEC(())
  {
    voltage_ = _voltage;
  }



  mirrorObject *
  ObjectVectorImpl::getObjectVector(ObjectType _objectType) throw()
  {
    int num; 

    if (_objectType == FIRST) 
      num = 0;
    else
      num = 1;
    mirrorObject_var tmp = new mirrorObject(mObj_[num]);

//   What did this code do anyway?
//    mObj_[num]->data[0]= mObj_[num]->data[0];
//    tmp = mObj_[num];

    return tmp._retn();
  }




  /*CORBA::Long
  ObstImpl::getAktion() ACE_THROW_SPEC(())
  {
    return voltage_;
  }*/

  void
  ObjectVectorImpl::setObjectVector(mirrorObject const& _art, ObjectType _objectType) throw()
  {
    int num;
    if (_objectType == FIRST)
      num = 0;
    else
      num = 1;
	 
     ACE_Time_Value objTime(ACE_OS::gettimeofday());
     timeA2C(objTime,mObj_[num]->time);
     mObj_[num]->data = _art.data;

  }
  //ObstImpl::setObst(mirrorObject const& _art) ACE_THROW_SPEC(())
};
