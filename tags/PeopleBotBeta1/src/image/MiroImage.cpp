// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////


#include "MiroImage.h"

namespace MiroImage 
{

shared::shared()
	{
	iSharedCount = 0;
	}

shared::~shared()
	{
	}

void shared::lock()
	{
	}

void shared::unlock()
	{
	}

void shared::attach()
	{
	iSharedCount++;
	}

void shared::detach()
	{
	if (iSharedCount > 0)
		iSharedCount--;
	if (iSharedCount == 0)
		delete this;
	}

uint32 shared::CountAttached()
	{
	return iSharedCount;
	}

}	// namespace
