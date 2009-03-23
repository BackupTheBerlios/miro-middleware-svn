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
#ifndef MCL_Feature_h
#define MCL_Feature_h

#include "Sample.h"

#include <iosfwd>
#include <string>

namespace Miro
{
  namespace MCL
  {
    class Feature
    {
    public:
      static const bool logStatistics = true;
      
      Feature(std::string const& _name);
      virtual ~Feature();
      
      virtual bool validData() const;
      virtual double evaluate(const Sample& _sample) = 0;
      
      virtual void initStatistics();
      virtual void setGlobalMax();
      virtual void statistics(std::ostream& _ostr) const;
      virtual void printLast(std::ostream& _ostr) const;
      
      std::string const& name() const;
      
    protected:
      void name(std::string const& _name);
      
    private:
      //--------------------------------------------------------------------------
      // private members
      //--------------------------------------------------------------------------

      //! The name of the feature.
      std::string name_;
    };
    
    inline
    void
    Feature::name(std::string const& _name) {
      name_ = _name;
    }
    inline
    std::string const&
    Feature::name() const {
      return name_;
    }
  }
}
#endif // MCL_Feature_h
