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
#ifndef Miro_CFG_Parameter_h
#define Miro_CFG_Parameter_h

#include <qstring.h>

#include <vector>

namespace Miro
{
  namespace CFG
  {
    struct Parameter
    {
      QString type_;
      QString name_;
      QString default_;
      QString fullDefault_;
      QString measure_;
      QString description_;
      
      Parameter();
      Parameter(const QString& _type,
		const QString& _name,
		const QString& _default,
		const QString& _fullDefault,
		const QString& _measure,
		const QString& _description);
    };
    
    bool operator < (const Parameter& lhs, const Parameter& rhs);
    bool operator == (const Parameter& lhs, const Parameter& rhs);

    typedef std::vector<Parameter> ParameterVector;
  }
}

#endif // Miro_CFG_Parameter_h
