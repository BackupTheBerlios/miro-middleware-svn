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
#ifndef MspParameters_h
#define MspParameters_h

#include "miro/DevParameters.h"
#include "miro/ScanDescriptionC.h"

namespace Msp
{
  class Parameters;

  std::ostream&
  operator << (std::ostream& ostr, const Parameters& desc);
  
  class Parameters : public Miro::DevParameters
  {
    typedef Miro::DevParameters Super;

  public:
    Miro::ScanDescriptionIDL sonarDescription;
    Miro::ScanDescriptionIDL infraredDescription;
    Miro::ScanDescriptionIDL tactileDescription;

    Parameters();
    virtual ~Parameters();
    virtual void operator <<= (const QDomNode& node);

    friend
    std::ostream&
    operator << (std::ostream& ostr, const Parameters& desc);
  };

};

#endif
