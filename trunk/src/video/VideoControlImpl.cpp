/*! \file VideoControlImpl.cpp
 *  \brief Implementation for the camera control interface
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
 * Revision 1.1  2003/06/11 09:23:13  muehlenf
 * Added camera control interface.
 *
 *
 *
 */

#ifdef HAVE_CONFIG_H
#   include "config.h"
#endif

#include "VideoControlImpl.h"

namespace Video
{
    //-------------------------------------------------------------------------
    ControlImpl::ControlImpl(Device1394 * device)
     :  device_(device)
    {
    }

    //-------------------------------------------------------------------------
    CORBA::Boolean ControlImpl::getFeatures(FeatureSet_out features)
        ACE_THROW_SPEC ((
            CORBA::SystemException
        ))
    {
        if (device_)
	    return device_->getFeatures(features);
        return false;
    }
    
    //-------------------------------------------------------------------------
    CORBA::Boolean ControlImpl::setFeatures(const FeatureSet & features)
        ACE_THROW_SPEC ((
            CORBA::SystemException
        ))
    {
        if (device_)
	    return device_->setFeatures(features);
        return false;
    }
};

