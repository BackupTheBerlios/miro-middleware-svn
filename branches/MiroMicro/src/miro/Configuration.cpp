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
#include "Configuration.h"

// for string compare
#if ACE_MAJOR_VERSION > 5 || \
    ACE_MAJOR_VERSION == 5 && ACE_MINOR_VERSION >= 4
#include <ace/OS_NS_strings.h>
#else
#include <ace/OS.h>
#endif

#include <ace/Arg_Shifter.h>

#include <string>

namespace
{
  const char configFileOpt[] = "-MiroConfigFile";
  const char shortFileOpt[] = "-MCF";
}

namespace Miro
{
  Singleton<ConfigDocument> Configuration::document;

  void Configuration::init(int& argc, char * argv[]) throw(Exception)
  {
    const char *name;
    char host[256];
    ACE_OS::hostname(host, 256);
    name = host;

    ACE_Arg_Shifter arg_shifter(argc, argv);
    while (arg_shifter.is_anything_left()) {
      const ACE_TCHAR *current_arg = arg_shifter.get_current();

      if ((ACE_OS::strcasecmp(configFileOpt, current_arg) == 0) ||
            (ACE_OS::strcasecmp(shortFileOpt, current_arg) == 0)) {
        arg_shifter.consume_arg();
        if (arg_shifter.is_parameter_next()) {
          name = arg_shifter.get_current();
          arg_shifter.consume_arg();
        }
      }
      else
        arg_shifter.ignore_arg();
    }

    std::string fileName = name;
    if (name == host)
      fileName += std::string(".xml");

    document()->init(fileName);
  }
}
