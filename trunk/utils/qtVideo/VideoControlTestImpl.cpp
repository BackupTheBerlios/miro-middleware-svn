/*! \file VideoControlTestImpl.cpp
 *  \brief Dummy implementation for the camera control interface
 *
 * -----------------------------------------------------
 *
 * RoboCup Middle Size Team, Technische Universitaet Graz
 * "Mostly Harmless" 
 * http://www.robocup.tugraz.at
 *
 * Project: Miro::Video
 *
 * $Author$
 * $Date$
 * $Revision$
 *
 * $Log$
 * Revision 1.1  2004/02/09 17:28:56  graz
 * Added control-interface to VideoDevice1394 & Dialog.
 *
 *
 */

#ifdef HAVE_CONFIG_H
#   include "config.h"
#endif

#include "VideoControlTestImpl.h"

namespace Video
{
    //-------------------------------------------------------------------------
    ControlTestImpl::ControlTestImpl()
    {
    }

    //-------------------------------------------------------------------------
    CORBA::Boolean ControlTestImpl::getFeatures(FeatureSet_out features)
        ACE_THROW_SPEC ((
            CORBA::SystemException
        ))
    {
        features = features_;
        return true;
    }
    
    //-------------------------------------------------------------------------
    CORBA::Boolean ControlTestImpl::setFeatures(const FeatureSet & features)
        ACE_THROW_SPEC ((
            CORBA::SystemException
        ))
    {
	features_ = features;
        return true;
    }
};

