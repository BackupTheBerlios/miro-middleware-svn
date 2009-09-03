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
#ifndef FilterBusy_h
#define FilterBusy_h

#include "Parameters.h"
#include "video/VideoFilter.h"

class FilterBusy : public Video::Filter
{
  typedef Video::Filter Super;
public:
  FilterBusy(const Miro::ImageFormatIDL& _inputFormat);
  virtual void process();
  FILTER_PARAMETERS_FACTORY(FilterBusy);

protected:
  unsigned int imageSize_;
};

#endif // FilterBusy_h