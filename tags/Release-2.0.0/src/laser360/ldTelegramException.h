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
#ifndef _LD_TELEGRAM_EXCEPTION_H_
  #define _LD_TELEGRAM_EXCEPTION_H_

  #include <string>
  #include "ldBaseException.h"

namespace ldoem_
{

  //---------------------------------------------------------------------------
  /// Telegram Exception class
  class TelegramException : public BaseException
  {
  public:
    //---------------------------------------------------------------------------
    /// constructor
    /// @param msg Errortext that you can get afterwards with getMessage()
    TelegramException( const std::string msg ) : BaseException( msg )
    {
    };

  };
}; // end namespace ldoem_

#endif // TELEGRAM_EXCEPTION_H_
