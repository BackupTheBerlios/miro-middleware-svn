// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef miroParameters_h
#define miroParameters_h

#include <iostream>

// forward declarations
class QDomNode;

namespace Miro
{
  // forward declaration
  class Parameters;

  //! Debug output stream operator.
  std::ostream& operator<<(std::ostream& _ostr, const Parameters& _rhs);

  //! Base class for all Parameter classes.
  class Parameters 
  {
  public:
    //! Initializing default constructor.
    virtual ~Parameters() {};
    //! XML parsing operator.
    virtual void operator <<= (const QDomNode& node) = 0;
  protected:
    virtual void printToStream(std::ostream&) const {};
    friend
    std::ostream& operator<<(std::ostream& _ostr, const Parameters& _rhs);
  };

  inline
  std::ostream&
  operator<<(std::ostream& _ostr, const Parameters& _rhs) {
    _rhs.printToStream(_ostr);
    return _ostr;
  }

};

#endif
 
