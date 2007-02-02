// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef miroEnumeration_h
#define miroEnumeration_h

#include <string>
#include <vector>

namespace Miro
{
  class Enumeration;

  std::ostream& operator<<(std::ostream& ostr, Enumeration _enum);
  std::istream& operator>>(std::istream& istr,  Enumeration& _enum);

  class Enumeration
  {
  public:
    Enumeration() {}
    Enumeration(std::string _enum, std::string _values);
    Enumeration(std::string _enum, std::vector<std::string> _values);

    void value(std::string _value);
    const std::string& value() const;
    const std::vector<std::string>& assortment() const;

    friend std::ostream& operator << (std::ostream& ostr, Enumeration _enum);
    friend std::istream& operator >> (std::istream& istr,  Enumeration& _enum);

  private:
    std::string enum_;
    std::vector<std::string> values_;
  };
}


#endif
