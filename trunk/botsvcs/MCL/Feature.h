// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Nix (Nix Is eXtreme)
//
// (c) 2002, 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
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
