// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2002, 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "Parameter.h"

namespace Miro
{
  namespace CFG
  {
    Parameter::Parameter() 
    {}

    Parameter::Parameter(QString const& _type,
			 QString const& _name,
			 QString const& _default,
			 QString const& _fullDefault,
			 QString const& _measure,
			 QString const& _description) :
      type_(_type),
      name_(_name),
      default_(_default),
      fullDefault_(_fullDefault),
      measure_(_measure),
      description_(_description)
    {}

    bool operator < (Parameter const& lhs, Parameter const& rhs) 
    {
      return (!(QString::compare(lhs.name_, rhs.name_) >= 0 &&
		QString::compare(lhs.type_, rhs.type_) >= 0 &&
		QString::compare(lhs.default_, rhs.default_) >= 0 &&
		QString::compare(lhs.measure_, rhs.measure_) >= 0 &&
		QString::compare(lhs.description_, rhs.description_) >= 0));
    }

    bool operator == (Parameter const& lhs, Parameter const& rhs)
    {
      return (QString::compare(lhs.name_, rhs.name_) == 0 &&
	      QString::compare(lhs.type_, rhs.type_) == 0 &&
	      QString::compare(lhs.default_, rhs.default_) == 0 &&
	      QString::compare(lhs.measure_, rhs.measure_) == 0 &&
	      QString::compare(lhs.description_, rhs.description_) == 0);
    }
  }
}

