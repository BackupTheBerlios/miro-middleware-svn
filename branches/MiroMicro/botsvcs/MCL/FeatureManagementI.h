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
#ifndef MCL_FeatureManagementI_h
#define MCL_FeatureManagementI_h

#include "idl/LocalizationS.h"

namespace Miro
{
  namespace MCL
  {
    // forward declarations
    class MultiFeature;
    template <class T> class ObservationModelT;
    
    class  FeatureManagementI : public virtual POA_Miro::MCL::MultiFeatureLocalization
    {
    public:
      //Constructor 
      FeatureManagementI(ObservationModelT<MultiFeature>& _manager) throw();
      
      //Destructor 
      virtual ~FeatureManagementI();
           
      // Feature manager interface.
      
      virtual void addFeature(char const * _feature) throw(EUnknown);
      virtual void delFeature(char const * _feature) throw(EUnknown);
      virtual FeatureVector * currentFeatures() throw();
      virtual FeatureVector * availableFeatures() throw();
      
    protected:
      //--------------------------------------------------------------------------
      // protected data
      //--------------------------------------------------------------------------
      
      ObservationModelT<MultiFeature>& manager_;
    };
  }
}
#endif
