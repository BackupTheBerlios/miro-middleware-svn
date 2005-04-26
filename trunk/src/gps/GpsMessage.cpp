/*
 * This file is part of Miro (The Middleware for Robots)
 * Copyright (C) 2005 Ingo van Lil <ingo@vanlil.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 * $Id$
 */

#include "GpsMessage.h"
#include <string>
#include <vector>

namespace GPS
{
  Message::Message() :
    msg_("")
  {
    fields_.clear();
    valid_ = false;
  }

  Message::Message(const std::string &msg) :
    msg_(msg)
  {
    fields_.clear();
    valid_ = verify();
    if (valid_)
      parse();
  }

  Message::Message(const char *msg) :
    msg_(msg)
  {
    fields_.clear();
    valid_ = verify();
    if (valid_)
      parse();
  }

  Message::~Message()
  {
  }

  void Message::set(const std::string& msg)
  {
    msg_ = msg;
    fields_.clear();
    valid_ = verify();
    if (valid_)
      parse();
  }

  void Message::set(const char * msg)
  {
    msg_ = (std::string)msg;
    fields_.clear();
    valid_ = verify();
    if (valid_)
      parse();
  }

  const std::string& Message::get() const
  {
    return msg_;
  }

  bool Message::valid() const
  {
    return valid_;
  }

  int Message::fields() const
  {
    return fields_.size();
  }

  const std::string& Message::field(int n) const
  {
    return fields_[n];
  }

  bool Message::verify()
  {
    const char *c = msg_.c_str();
    unsigned char chksum = 0;

    if (*(c++) != '$')
      return false;
    while (*c != '*' && *c != '\0')
      chksum ^= *(c++);
    if (*c == '\0')
      return true;
    return (chksum == strtol(c+1, NULL, 16));
  }

  void Message::parse()
  {
    unsigned int c1 = 1;
    unsigned int c2;

    while ((c2 = msg_.find(",", c1)) != std::string::npos) {
      fields_.push_back(msg_.substr(c1, c2-c1));
      c1 = c2+1;
    }

    c2 = msg_.rfind("*", msg_.length());
    if (c2 == std::string::npos)
      c2 = msg_.length() + 1;
    fields_.push_back(msg_.substr(c1, c2-c1));
  }
}

