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
#ifndef miroEnumerationMultiple_h
#define miroEnumerationMultiple_h

#include <string>
#include <vector>

namespace Miro
{
  class EnumerationMultiple;

  std::ostream& operator<<(std::ostream& ostr, EnumerationMultiple _enum);
  std::istream& operator>>(std::istream& istr,  EnumerationMultiple& _enum);

  class EnumerationMultiple
  {
  public:
    EnumerationMultiple() {}
    EnumerationMultiple(std::string _enum, std::string _values);
    EnumerationMultiple(std::vector<std::string> _enum, std::vector<std::string> _values);

    void value(std::string _value);
    void value(std::vector<std::string> _value);
    const std::vector<std::string>& value() const;
    const std::vector<std::string>& assortment() const;

    friend std::ostream& operator << (std::ostream& ostr, EnumerationMultiple _enum);
    friend std::istream& operator >> (std::istream& istr,  EnumerationMultiple& _enum);

  private:
    std::vector<std::string> tokenizer(std::string _values);
    void checkAvailability();

    std::vector<std::string> enum_;
    std::vector<std::string> values_;
  };
}


#endif
