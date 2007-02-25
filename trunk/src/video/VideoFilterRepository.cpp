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
	ostr << first->first << std::endl;
    }
  }

  std::ostream&
  operator << (ostream& ostr, FilterRepository const& _repository)
  {
    _repository.printToStream(ostr);
    return ostr;
  }

};
