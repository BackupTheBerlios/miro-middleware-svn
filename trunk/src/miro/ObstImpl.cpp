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

#include "ObstImpl.h"
#include "TimeHelper.h"

namespace Miro
{
  // Implementation skeleton constructor
  ObstImpl::ObstImpl() :
    voltage_(-1) // uninicialized value
  {
    for (int i=0; i<objectNumber; ++i) {
      mObj_[i] = new mirrorObject();
      mObj_[i]->data.length(10);
      mObj_[i]->data[0]=421;
    }
  }

  // Implementation skeleton destructor
  ObstImpl::~ObstImpl()
  {
  }

  void
  ObstImpl::integrateData(long _voltage) ACE_THROW_SPEC(())
  {
    voltage_ = _voltage;
  }



  mirrorObject *
  ObstImpl::getObst(ObjectType _objectType) throw()
  {
    int num; 
    if (_objectType == FIRST) 
      num = 0;
    else
      num = 1;
    mirrorObject_var tmp = new mirrorObject();
    mObj_[num]->data[0]= mObj_[num]->data[0];
    tmp = mObj_[num];
    return tmp._retn();
  }




  /*CORBA::Long
  ObstImpl::getAktion() ACE_THROW_SPEC(())
  {
    return voltage_;
  }*/

  void
  ObstImpl::setObst(mirrorObject const& _art, ObjectType _objectType) throw()
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
