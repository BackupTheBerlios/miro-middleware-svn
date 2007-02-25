// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 2005
// Department of Software Technology, Technical University of Graz, Austria
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

