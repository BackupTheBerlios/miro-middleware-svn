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
      Parameter(QString const& _type,
		QString const& _name,
		QString const& _default,
		QString const& _fullDefault,
		QString const& _measure,
		QString const& _description);
    };
    
    bool operator < (Parameter const & lhs, Parameter const& rhs);
    bool operator == (Parameter const & lhs, Parameter const& rhs);

    typedef std::vector<Parameter> ParameterVector;
  }
}

#endif // Miro_CFG_Parameter_h
