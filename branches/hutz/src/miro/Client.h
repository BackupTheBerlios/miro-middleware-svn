// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// for details copyright, usage and credits to other groups see Miro/COPYRIGHT
// for documentation see Miro/doc
// 
// (c) 1999,2000
// Department of Neural Information Processing, University of Ulm, Germany
//
// Authors: 
//   Stefan Enderle, 
//   Stefan Sablatnoeg, 
//   Hans Utz
// 
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef miroClient_hh
#define miroClient_hh

#include <orbsvcs/Naming/Naming_Utils.h>
#include <orbsvcs/CosNamingC.h>

#include <string>
#include <iostream>

#include "Exception.h"
#include "RobotParameters.h"

/**
 * The namespace of the Miro project.
 *
 * To not pollute the global namespace of users of Miro, all classes, structs
 * and helper functions are encapsulated into the namespace Miro. See the
 * hard copy manual for a short description of namespaces and Stroustrup's
 * C++ for a longer one.
 *
 * If you don't want to know anything about namespaces, just say:
 *
 * using namespace Miro;
 *
 * at the beginning of all of you .cc files to map everything of the Miro
 * namespace into the global one. This way you won't need any prefixes.
 */
namespace Miro
{
  // forward declaration
  class Client;

  /**
   * Helper functions to resolve names from the name service,
   * using the clients naming context.
   *
   * See "Advanced CORBA programming in C++" for details.
   */
    template<class T>
    typename T::_ptr_type resolve_name(const Client& client, const char * name) 
      throw(CORBA::Exception);

  /**
   * Base class for all Miro client applications.
   *
   * It tries to encapsulate the CORBA stuff, so that you need
   * only to have a minimal idea of what CORBA is to use Miro.
   * Anyhow, it doesn't restrict you in the use of CORBA, since
   * your derived classes have full access to all the protected data
   * provided by Miro::Client.
   */
  class Client
  {
  public:

    /**
     * Constructor
     */
    Client(int& argc, char *argv[], 
	   const RobotParameters& _params = RobotParameters());
    Client(const Client& _client);
    
    /**
     * Destructor.
     */
    virtual ~Client();

    CORBA::ORB_ptr orb() const;
    // Accessor which returns the ORB pointer.  Following the normal
    // CORBA memory management rules of return values from functions,
    // this function duplicates the orb return value before returning
    // it.

    /**
     * Helper functions to resolve initial contexts from the ORB.
     *
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
      cerr << "Cannot get initial reference for " << id << ": " << e << endl;
      throw 0;
    }

    if (CORBA::is_nil(obj.in())) {
      cerr << "Cannot get initial reference for " << id << endl;
      throw 0;
    }

    typename T::_var_type ref;
    try {
      ref = T::_narrow(obj.in());
    }
    catch (const CORBA::Exception & e) {
      cerr << "Cannot narrow reference for " << id << ": " << e << endl;
      throw 0;
    }
    if (CORBA::is_nil(ref.in())) {
      cerr << "Incorrect type of reference: " << id << endl;
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
      cerr << "No " << name << " in Naming Service" << endl;
      throw 0;
    }
    catch (const CORBA::Exception & e) {
      cerr << "Cannot resolve binding for " << name << ": " << e << endl;
      throw 0;
    }
    if (CORBA::is_nil(obj.in())) {
      cerr << "Nil binding in Naming Service for " << name << endl;
      throw 0;
    }

    typename T::_var_type ref;
    try {
      ref = T::_narrow(obj.in());
    }
    catch (const CORBA::Exception & e) {
      cerr << "Cannot narrow reference for " << name << ": " << e << endl;
      throw 0;
    }
    if (CORBA::is_nil(ref.in())) {
      cerr << "Reference has incorrect type: " << name << endl;
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
      cerr << "Nil binding in Naming Service for " 
	   << name[name.length() - 1].id
	   << endl;
      throw 0;
    }

    typename T::_var_type ref;
    try {
      ref = T::_narrow(obj.in());
    }
    catch (const CORBA::Exception & e) {
      cerr << "Cannot narrow reference for"
	   << name[name.length() - 1].id
	   << ": " << e << endl;
      throw 0;
    }
    if (CORBA::is_nil(ref.in())) {
      cerr << "Reference has incorrect type: "
	   << name[name.length() - 1].id
	   << endl;
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
      cerr << "No " << name << " in Naming Service" << endl;
      throw 0;
    }
    catch (const CORBA::Exception & e) {
      cerr << "Cannot resolve binding for " << name << ": " << e << endl;
      throw 0;
    }
    if (CORBA::is_nil(obj.in())) {
      cerr << "Nil binding in Naming Service for " << name << endl;
      throw 0;
    }

    typename T::_var_type ref;
    try {
      ref = T::_narrow(obj.in());
    }
    catch (const CORBA::Exception & e) {
      cerr << "Cannot narrow reference for " << name << ": " << e << endl;
      throw 0;
    }
    if (CORBA::is_nil(ref.in())) {
      cerr << "Reference has incorrect type:" << name << endl;
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




