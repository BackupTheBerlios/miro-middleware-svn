// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Nix (Nix Is eXtreme)
//
// (c) 2001, 2002, 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
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
