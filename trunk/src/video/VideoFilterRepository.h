// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef VideoFilterRepository_h
#define VideoFilterRepository_h

#include "VideoFilterFactory.h"

#include "miro/Singleton.h"
#include "miro/Exception.h"

#include <map>
#include <string>
#include <iostream>

namespace Miro
{
  class ImageFormatIDL;
}

namespace Video
{
  // forward declarations
  class Filter;
  class FilterFactoryBase;
  class FilterRepository;

  //! Ostream operator for debug purposes.
  std::ostream&
  operator << (std::ostream& ostr, FilterRepository const& rhs);

  //! Repository of all available filters.
  /**
   * Filters have to register a factory at the repository before
   * they can be initialized.
   *
   * @author Hans Utz
   */
  class FilterRepository  
  {
    typedef FilterRepository Self;
  public:
    //! Register an Filter at the repository.
    template<class T>
    void registerFilter(std::string const& _name) throw(Miro::Exception);
    //! Lookup an Behaviour by name.
    Filter * getInstance(std::string const& _name, 
			 Miro::ImageFormatIDL const& _inputFormat) throw(Miro::Exception);

    //! Accessor to the global instance of the BehaviourRepository.
    static Miro::Singleton<Self> instance;

  protected:
    //! Dump the BehaviourRepository to the specified output stream.
    void printToStream(std::ostream& _ostr) const;

    //! Map of the registerd Behaviours, sorted by name.
    typedef std::map<std::string, FilterFactoryBase *> FactoryMap;
    //! Map to associate an Behaviour name with an Behaviour instance.
    FactoryMap factory_;

    friend std::ostream& operator << (std::ostream&, FilterRepository const&);

  private:
    //! There is only one BehaviourRepository instance.
    FilterRepository();
    //! Copy construction is prohibited
    FilterRepository(FilterRepository const&) {}
    friend class Miro::Singleton<FilterRepository>::ACE_Singleton_Type;
  };

  template<class T>
  void
  FilterRepository::registerFilter(std::string const& _name)  throw(Miro::Exception)
  {
    if (factory_.find(_name) != factory_.end())
      throw Miro::Exception("FilterFactory " + _name + " already registered at the Repository.");
    factory_[_name] =  new FilterFactory<T>;	
  }
};
#endif

