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

#include "VideoFilterRepository.h"
#include "VideoFilter.h"

namespace Video
{
  FilterFactoryBase::~FilterFactoryBase()
  {}

  /**
   * To get a pointer to the global FilterRepository instance,
   * simply call ArbiterRepository::instance();
   */
  Miro::Singleton<FilterRepository> FilterRepository::instance;

  FilterRepository::FilterRepository()
  {}

  Filter * 
  FilterRepository::getInstance(const std::string& _name, 
				Miro::ImageFormatIDL const& _inputFormat) throw(Miro::Exception)
  {
    FactoryMap::const_iterator i = factory_.find(_name);
    if (i == factory_.end())
      throw Miro::Exception("FilterFactory " + _name + " not registered at the Repository.");
    return i->second->instance(_inputFormat);
  }

  void
  FilterRepository::printToStream(std::ostream& ostr) const
  {
    FactoryMap::const_iterator first, last = factory_.end();	
    
    for (first = factory_.begin(); first != last; ++first) {		
	ostr << first->first << endl;
    }
  }

  std::ostream&
  operator << (ostream& ostr, FilterRepository const& _repository)
  {
    _repository.printToStream(ostr);
    return ostr;
  }

};
