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
#ifndef VideoParameters_h
#define VideoParameters_h

#include "miro/DevParameters.h"

namespace Video
{
  class Parameters;

  std::ostream&
  operator << (std::ostream& ostr, const Parameters& desc);

  class Parameters : public Miro::DevParameters
  {
    typedef Miro::DevParameters Super;
  public:
    std::string	grabber;
    std::string	format;
    std::string	source;
    std::string	palette;
    std::string	subfield;
    
    int		width;
    int		height;
    int		connections;
    int		buffers;
    int		byteorder;
    int 	upsideDown; //for cameras mounted upside-down
    int 	pixelSize;  //in bytes

    Parameters();
    virtual void operator <<= (const QDomNode& node);

    friend
    std::ostream&
    operator << (std::ostream& ostr, const Parameters& desc);
  };
};

#endif
