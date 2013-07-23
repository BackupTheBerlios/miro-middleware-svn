// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 1999-2005
// Department of Neuroinformatics, University of Ulm, Germany
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// $Id$
//
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
