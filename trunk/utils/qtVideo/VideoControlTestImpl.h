/*! \file VideoControlTestImpl.h
 *  \brief Dummy implementation for the camera control interface (interface)
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

#ifndef VIDEOCONTROLTESTIMPL_H
#define VIDEOCONTROLTESTIMPL_H

#include "idl/VideoControlS.h"

namespace Video
{
    class ControlTestImpl : public virtual POA_Video::Control
    {
    public:
        ControlTestImpl();
	
        virtual CORBA::Boolean getFeatures(FeatureSet_out features)
	        ACE_THROW_SPEC ((
                    CORBA::SystemException
                ));
	virtual CORBA::Boolean setFeatures(const FeatureSet & features)
	        ACE_THROW_SPEC ((
                    CORBA::SystemException
                ));
	
    private:
        FeatureSet features_;
    };
};

#endif /* VIDEOCONTROLTESTIMPL_H */

