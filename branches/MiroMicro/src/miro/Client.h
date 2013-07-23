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
#ifndef miroClient_h
#define miroClient_h

#include <tao/Version.h>
#if TAO_MAJOR_VERSION > 1 || \
	( TAO_MAJOR_VERSION == 1 && TAO_MINOR_VERSION > 4 ) || \
	( TAO_MAJOR_VERSION == 1 && TAO_MINOR_VERSION == 4 && TAO_BETA_VERSION >= 3 )
#include <orbsvcs/Naming/Naming_Client.h>
#include <orbsvcs/Naming/Naming_Server.h>
#else
#include <orbsvcs/Naming/Naming_Utils.h>
#endif

#include <orbsvcs/CosNamingC.h>

#include <string>
#include <iostream>

#include "Exception.h"
#include "miro/Parameters.h"


namespace Miro
{
  // forward declaration
  class Client;

  //! Global helper function for resolution of a CORBA object from the naming service.
  /** This function resolves a name from the CORBA naming service,
   * using the naming context of the client and narrows it to the specified
   * type.
   *
   * See "Advanced CORBA programming in C++" for details.
   */
  template<class T>
  typename T::_ptr_type resolve_name(const Client& client, const char * name)
  throw(CORBA::Exception);

  //! Base class for all Miro client applications.
  /**
   * It tries to encapsulate the CORBA stuff, so that you need
   * only to have a minimal idea of what CORBA is to use Miro.
   * Anyhow, it doesn't restrict you in the use of CORBA, since
   * your derived classes have full access to all the protected data
   * provided by Miro::Client.
   */
  class Client
  {
  public:

    //! Default constructor.
    /**
     * The ORB is initialized on construction of this class.  The argc
     * and argv are passed on the CORBA::ORB_init() global helper
     * function and the ORB specific command line options are removed
     * from the argument vector.
     */
    Client(int& argc, char *argv[],
           const RobotParameters& _params = *RobotParameters::instance());
    Client(const Client& _client);

    /**
     * Destructor.
     */
    virtual ~Client();

    //! Accessor which returns the ORB pointer.
    /**
     * Following the normal CORBA memory management rules of return
     * values from functions, this function duplicates the orb return
     * value before returning it.
     */
    CORBA::ORB_ptr orb() const;

    //! Helper functions to resolve initial contexts from the ORB.
    /**
     * See "Advanced CORBA programming in C++" for details.
     */
    template<class T>
    typename T::_ptr_type resolveInit(const char * id)
    const throw(CORBA::Exception, Miro::Exception);

    /**
     * Helper functions to resolve names from the name service,
     * using the clients naming context.
     *
     * See "Advanced CORBA programming in C++" for details.
     */
    template<class T>
    typename T::_ptr_type resolveName(const char * name)
    const throw(CORBA::Exception);

    /**
     * Helper functions to resolve names from the name service,
     * using the clients naming context.
     *
     * See "Advanced CORBA programming in C++" for details.
     */
    template<class T>
    typename T::_ptr_type tryResolveName(const char * name)
    const throw(CORBA::Exception);

    /**
     * Helper functions to resolve names from the name service,
     * using some user provided absolute naming path.
     *
     * See "Advanced CORBA programming in C++" for details.
     */
    template<class T>
    typename T::_ptr_type resolveName(const CosNaming::Name& name)
    const throw(CORBA::Exception);

    //   protected:
    /**
     * Remember our ORB.
     */
    CORBA::ORB_var orb_;

    const RobotParameters params_;
    bool noNaming_;

    /**
     * Reference to the initial naming context.
     */
    CosNaming::NamingContext_var initialNamingContext;

    /**
     * Reference to the Miro naming context.
     */
    CosNaming::NamingContext_var namingContext;

    std::string namingContextName;
  };

  //----------------------------------------------------------------------------
  // Template implementations
  //----------------------------------------------------------------------------
  template<class T>
  typename T::_ptr_type
  Client::resolveInit(const char * id) const throw(CORBA::Exception, Miro::Exception)

  {
    CORBA::Object_var obj;
    try {
      obj = orb_->resolve_initial_references(id);
    }
    catch (const CORBA::ORB::InvalidName & e) {
      throw;
    }
    catch (const CORBA::Exception & e) {
      std::cerr << "Cannot get initial reference for " << id << std::endl;
      throw 0;
    }

    if (CORBA::is_nil(obj.in())) {
      std::cerr << "Cannot get initial reference for " << id << std::endl;
      throw 0;
    }

    typename T::_var_type ref;
    try {
      ref = T::_narrow(obj.in());
    }
    catch (const CORBA::Exception & e) {
      std::cerr << "Cannot narrow reference for " << id << std::endl;
      throw 0;
    }
    if (CORBA::is_nil(ref.in())) {
      std::cerr << "Incorrect type of reference: " << id << std::endl;
      throw 0;
    }
    return ref._retn();
  }

  //----------------------------------------------------------------

  template<class T>
  typename T::_ptr_type
  Client::resolveName(const char* name)
  const throw(CORBA::Exception)
  {
    CosNaming::Name n;

    n.length(1);
    n[0].id = CORBA::string_dup(name);

    CORBA::Object_var obj;
    try {
      obj = namingContext->resolve(n);
    }
    catch (const CosNaming::NamingContext::NotFound & e) {
      std::cerr << "No " << name << " in Naming Service" << std::endl;
      throw 0;
    }
    if (CORBA::is_nil(obj.in())) {
      std::cerr << "Nil binding in Naming Service for " << name << std::endl;
      throw 0;
    }

    typename T::_var_type ref;
    try {
      ref = T::_narrow(obj.in());
    }
    catch (const CORBA::Exception & e) {
      std::cerr << "Cannot narrow reference for " << name << std::endl;
      throw 0;
    }
    if (CORBA::is_nil(ref.in())) {
      std::cerr << "Reference has incorrect type: " << name << std::endl;
      throw 0;
    }
    return ref._retn();
  }

  //----------------------------------------------------------------

  template<class T>
  typename T::_ptr_type
  Client::resolveName(const CosNaming::Name& name)
  const throw(CORBA::Exception)
  {
    CORBA::Object_var obj;
    obj = initialNamingContext->resolve(name);
    if (CORBA::is_nil(obj.in())) {
      std::cerr << "Nil binding in Naming Service for "
      << name[name.length() - 1].id
      << std::endl;
      throw 0;
    }

    typename T::_var_type ref;
    try {
      ref = T::_narrow(obj.in());
    }
    catch (const CORBA::Exception & e) {
      std::cerr << "Cannot narrow reference for"
      << name[name.length() - 1].id
      << std::endl;
      throw 0;
    }
    if (CORBA::is_nil(ref.in())) {
      std::cerr << "Reference has incorrect type: "
      << name[name.length() - 1].id
      << std::endl;
      throw 0;
    }
    return ref._retn();
  }

  template<class T>
  typename T::_ptr_type
  resolve_name(const Client& client, const char* name)
  throw(CORBA::Exception)
  {
    CosNaming::Name n;

    n.length(1);
    n[0].id = CORBA::string_dup(name);

    CORBA::Object_var obj;
    try {
      obj = client.namingContext->resolve(n);
    }
    catch (const CosNaming::NamingContext::NotFound & e) {
      std::cerr << "No " << name << " in Naming Service" << std::endl;
      throw 0;
    }
    catch (const CORBA::Exception & e) {
      std::cerr << "Cannot resolve binding for " << name << std::endl;
      throw 0;
    }
    if (CORBA::is_nil(obj.in())) {
      std::cerr << "Nil binding in Naming Service for " << name << std::endl;
      throw 0;
    }

    typename T::_var_type ref;
    try {
      ref = T::_narrow(obj.in());
    }
    catch (const CORBA::Exception & e) {
      std::cerr << "Cannot narrow reference for " << name << std::endl;
      throw 0;
    }
    if (CORBA::is_nil(ref.in())) {
      std::cerr << "Reference has incorrect type:" << name << std::endl;
      throw 0;
    }
    return ref._retn();
  }

  template<class T>
  typename T::_ptr_type
  Client::tryResolveName(const char* name)
  const throw(CORBA::Exception)
  {
    CosNaming::Name n;

    n.length(1);
    n[0].id = CORBA::string_dup(name);

    CORBA::Object_var obj;
    try {
      obj = namingContext->resolve(n);
    }
    catch (const CORBA::Exception & e) {
    }

    typename T::_var_type ref;
    try {
      ref = T::_narrow(obj.in());
    }
    catch (const CORBA::Exception & e) {
    }
    return ref._retn();
  }

};


#endif




