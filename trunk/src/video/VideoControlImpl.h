/*! \file VideoControlImpl.h
 *  \brief Implementation for the camera control interface (interface)
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
 * Revision 1.2  2003/06/12 09:30:56  gsteinba
 * Export of Video Control Interface Corrected
 *
 * Revision 1.1  2003/06/11 09:23:13  muehlenf
 * Added camera control interface.
 *
 *
 *
 */

#ifndef VIDEOCONTROLIMPL_H
#define VIDEOCONTROLIMPL_H

#include "idl/VideoControlS.h"
#include "VideoDevice1394.h"

namespace Video
{
    class ControlImpl : public virtual POA_Video::Control
    {
    public:
        ControlImpl(Device1394 * device);
	
        virtual CORBA::Boolean getFeatures(FeatureSet_out features)
	        ACE_THROW_SPEC ((
                    CORBA::SystemException
                ));
	virtual CORBA::Boolean setFeatures(const FeatureSet & features)
	        ACE_THROW_SPEC ((
                    CORBA::SystemException
                ));
	
    private:
        Device1394 * device_;
    };
};

#endif /* VIDEOCONTROLIMPL_H */

