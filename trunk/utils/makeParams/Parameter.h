// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef Parameter_h
#define Parameter_h

#include <qstring.h>

struct Parameter
{
  QString type_;
  QString name_;
  QString default_;
  QString measure_;
  QString description_;
  
  Parameter();
  Parameter(const QString& _type,
	    const QString& _name,
	    const QString& _default,
	    const QString& _measure,
	    const QString& _description);
};

bool operator < (const Parameter& lhs, const Parameter& rhs);

#endif
