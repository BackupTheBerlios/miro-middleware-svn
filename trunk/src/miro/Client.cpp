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
#include "Client.h"

#include <ace/Arg_Shifter.h>
#include <ace/OS.h>

#include <tao/Strategies/advanced_resource.h>

#include <iostream>
#include <cstring>

// #undef DEBUG

#ifdef DEBUG
#define DBG(x) x
#else
#define DBG(x)
#endif

using std::string;
using std::ostream;

using CORBA::Object_var;
using CosNaming::NamingContext;
using CosNaming::Name;

namespace
{
  const char * const namingKey = "-MiroNamingContext";
  const char * const shortNamingKey = "-MNC";
  const char * const noNamingKey = "-MiroNoNaming";
  const char * const shortNoNamingKey = "-MNN";
}

namespace Miro
{
  Client::Client(int& argc, char* argv[],
                 const RobotParameters& _params) :
      orb_(CORBA::ORB_init(argc, argv)), // Initialize orb
      params_(_params),
      noNaming_(false),
      namingContextName(params_.name)
  {
    // MIRO_LOG_CTOR("Miro::Client\n");

    // resolve the used naming context
    ACE_Arg_Shifter arg_shifter(argc, argv);
    while (arg_shifter.is_anything_left()) {
      const ACE_TCHAR *current_arg = arg_shifter.get_current();

      if (ACE_OS::strcasecmp(current_arg, namingKey) == 0 ||
            ACE_OS::strcasecmp(current_arg, shortNamingKey) == 0) {
        arg_shifter.consume_arg();
        if (arg_shifter.is_parameter_next()) {
          namingContextName = arg_shifter.get_current();
          arg_shifter.consume_arg();
        }
      }
      else if (ACE_OS::strcasecmp(current_arg, noNamingKey) == 0 ||
               ACE_OS::strcasecmp(current_arg, shortNoNamingKey) == 0) {
        arg_shifter.consume_arg();
        noNaming_ = true;
      }
      else
        arg_shifter.ignore_arg();
    }

    if (!noNaming_) {
      // Get reference to NameService
      initialNamingContext = resolveInit<NamingContext>("NameService");

      // Attempt to create naming context.
      Name n;
      n.length(1);
      n[0].id = CORBA::string_dup(namingContextName.c_str());
      try {
        namingContext = initialNamingContext->bind_new_context(n);
      }
      catch (const NamingContext::AlreadyBound &) {
        // Fine, CCS context already exists.
        // get a reference for it
        namingContext = resolveName<NamingContext>(n);
      }
    }
  }

  Client::Client(const Client& _client) :
      orb_(_client.orb()), // Initialize orb
      // Get reference to NameService
      params_(_client.params_),
      noNaming_(_client.noNaming_),
      initialNamingContext(_client.initialNamingContext),
      namingContext(_client.namingContext),
      namingContextName(_client.namingContextName)
  {
    // MIRO_LOG_CTOR("Miro::Client copy ctor\n");
  }

  Client::~Client()
  {
    // MIRO_LOG_DTOR("Miro::Client\n");
  }

  // Return the corba orb reference.
  CORBA::ORB_ptr
  Client::orb() const
  {
    return CORBA::ORB::_duplicate(orb_.in());
  }

};
