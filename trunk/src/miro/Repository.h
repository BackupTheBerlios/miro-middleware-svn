// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2001, 2002, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef Miro_Repository_h
#define Miro_Repository_h

#include "Singleton.h"
#include "Exception.h"

#include <map>
#include <string>
#include <iostream>

namespace Miro
{
  // forward declarations
  template<class T>
  class Repository;

  //! Ostream operator for debug purposes.
  template<class T>
  std::ostream&
  operator << (std::ostream& ostr, Repository<T> const& factory);

  //! Base class for repository templates.
  /**
   * @author Hans Utz
   */
  class RepositoryBase
  {
  public:
    //--------------------------------------------------------------------------
    // pulic types
    //--------------------------------------------------------------------------

    MIRO_EXCEPTION_TYPE(EAlreadyRegistered);
    MIRO_EXCEPTION_TYPE(ENotRegistered);
  };

  //! Repository of available instances of a type.
  /**
   * @author Hans Utz
   */
  template<class T>
  class Repository : public RepositoryBase
  {
    //--------------------------------------------------------------------------
    // private types
    //--------------------------------------------------------------------------
    typedef T Type;
    typedef Repository<Type> Self;
    typedef RepositoryBase Super;

  public:
    //--------------------------------------------------------------------------
    // public methods
    //--------------------------------------------------------------------------

    //! Clean up the repository.
    ~Repository();
    //! Empty the repository.
    void clear() throw ();
    //! Register an instance at the repository.
    void add( std::string const& _name, Type * _type) throw (EAlreadyRegistered);
    //! Look up an instance by name.
    Type * get(std::string const& _name) throw (ENotRegistered);
    //! Remove instance from the repository, deleting it.
    void remove(std::string const& _name) throw (ENotRegistered);

    //--------------------------------------------------------------------------
    // public data
    //--------------------------------------------------------------------------

    //! Accessor to the global instance of the Repository.
    static Singleton<Self> instance;

  protected:
    //--------------------------------------------------------------------------
    // protected types
    //--------------------------------------------------------------------------

    //! Map of the added instances, sorted by name.
    typedef std::map<std::string, Type *> InstanceMap;

    //--------------------------------------------------------------------------
    // protected methods
    //--------------------------------------------------------------------------

    //! Dump the Repository to the specified output stream.
    void printToStream(std::ostream& ostr) const;

    //--------------------------------------------------------------------------
    // protected data
    //--------------------------------------------------------------------------

    //! Map to associate an instance name with an instance.
    InstanceMap instances_;

  private:
    //--------------------------------------------------------------------------
    // private/hidden methods
    //--------------------------------------------------------------------------

    //! There is only one Repository instance.
    Repository() throw ();
    //! Copy construction is prohibited
    Repository(const Repository<T>&) throw () {}
    Repository& operator=(Repository const&) {}

    //--------------------------------------------------------------------------
    // friend declarations
    //--------------------------------------------------------------------------
    //! Allow Singleton to create a Repository
    friend std::ostream& operator << <Type>(std::ostream&, const Self&);
    //! Allow Singleton to create a Repository
    friend class ACE_Singleton<Self, ACE_Recursive_Thread_Mutex>;
  };

  /**
   * To get a pointer to the global Repository instance,
   * simply call Repository::instance();
   */
  template<class T>
  Singleton<Repository<T> > Repository<T>::instance;

  template<class T>
  Repository<T>::Repository() throw ()
  {}

  template<class T>
  Repository<T>::~Repository()
  {
    clear();
  }

  /** Deletes all objects registered at the repository. */
  template<class T>
  void
  Repository<T>::clear() throw ()
  {
    typename InstanceMap::iterator first, last = instances_.end();
    for (first = instances_.begin(); first != last; ++first)
      delete first->second;
    instances_.clear();
  }

  /** The repository takes ownership of the instance. */
  template<class T>
  void
  Repository<T>::add(std::string const& _name, T * _instance) 
    throw (EAlreadyRegistered)
  {
    if (instances_.find(_name) == instances_.end()) {
      instances_.insert(std::make_pair(_name, _instance));	
      return;
    }
    throw EAlreadyRegistered(_name);
  }

  /** 
   * @return Instance registered under the specified name.
   * @throw ENotRegistered 
   * if no instance is registered under the specified name.
   */
  template<class T>
  T * 
  Repository<T>::get(const std::string& _name) throw (ENotRegistered)
  {
    typename InstanceMap::const_iterator i = instances_.find(_name);
    if (i != instances_.end())
      return i->second;
    throw ENotRegistered(_name);
  }

  /** 
   * @throw ENotRegistered 
   * if no instance is registered under the specified name.
   */
  template<class T>
  void
  Repository<T>::remove(const std::string& _name) throw (ENotRegistered)
  {
    typename InstanceMap::iterator i = instances_.find(_name);
    if (i != instances_.end()) {
      delete i->second;
      instances_.erase(i);
      return;
    }
    throw ENotRegistered(_name);
  }

  /**
   * Prints the names of all registered instances to the stream
   */
  template<class T>
  void
  Repository<T>::printToStream(std::ostream& _ostr) const
  {
    typename InstanceMap::const_iterator first, last = instances_.end();	
    for(first = instances_.begin(); first != last; ++first) {		
      _ostr << first->first << std::endl;
    }
  }

  template<class T>
  std::ostream&
  operator << (ostream& _ostr, Repository<T> const& _repository)
  {
    _repository.printToStream(_ostr);
    return _ostr;
  }
}
#endif

