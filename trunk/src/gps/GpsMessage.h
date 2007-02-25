// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 2005 Ingo van Lil <ingo@vanlil.de>
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
//
// Authors: 
//   Ingo van Lil
//
#ifndef GpsMessage_h
#define GpsMessage_h

#include "miro/DevMessage.h"
#include <string>
#include <vector>

namespace GPS
{
  class Message : public Miro::DevMessage
  {
    public:
      Message();
      Message(const std::string& msg);
      Message(const char * msg);
      ~Message();

      void set(const std::string& msg);
      void set(const char * msg);
      const std::string& get() const;

      bool valid() const;
      int fields() const;
      const std::string& field(int n) const;

    protected:
      bool verify();
      void parse();
      bool valid_;
      std::string msg_;
      std::vector<std::string> fields_;
  };
}

#endif // GpsMessage_h
