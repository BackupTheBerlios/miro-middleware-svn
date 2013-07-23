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
#ifndef Miro_Factory_h
#define Miro_Factory_h

namespace Miro
{
  //! Base class for all factories for default constructible types.
  /**
   * This allows factories for the descendands of a common base type
   * to be handled as a polymorphic type. This way it can be used in
   * conjuction with the Repository template.
   *
   * @param B The name of the base type of the factory.
   */
  template<class B>
  class FactoryBase
  {
  public:
    //! The typedef for the base type.
    typedef B BaseType;

    //! Virtual destructor.
    /** Noop implementation. */
    virtual ~FactoryBase() {};
    //! The actual factory method.
    /** @return An instance of a descendand of BaseType. */
    virtual BaseType * instance() const = 0;
  };

  //! Factory template for default constructible types.
  /**
   * @param T The concrete derived type of the factory.
   * @param B The common base type of the factory.
   */
  template < class T, class B = typename T::Base >
  class Factory : public FactoryBase<B>
  {
  public:
    //! The typedef for the concrete type.
    typedef T Type;

    //! Virtual destructor.
    /** Noop implementation. */
    virtual ~Factory() {};
    //! The actual factory method.
    virtual Type * instance() const;
  };

  /** @return An instance of a type Type. */
  template<class T, class B>
  T *
  Factory<T, B>::instance() const
  {
    return new Type();
  }

  //! Base class for all factories for one parameter constructible types.
  /**
   * This allows factories for the descendands of a common base type
   * to be handled as a polymorphic type. This way it can be used in
   * conjuction with the Repository template.
   *
   * @param B The name of the base type of the factory.
   * @param P The name of the parameter type for the ctor of B.
   */
  template<class P, class B>
  class OneParamFactoryBase
  {
  public:
    //! The typedef for the base type.
    typedef B BaseType;
    //! The typedef for the parameter type.
    typedef P ParamType;

    //! Virtual destructor.
    /** Noop implementation. */
    virtual ~OneParamFactoryBase() {};
    //! The actual factory method.
    /** @return An instance of a descendand of BaseType. */
    virtual BaseType * instance(ParamType const&) const = 0;
  };

  template < class T, class P, class B = typename T::Base >
  class OneParamFactory : public OneParamFactoryBase<P, B>
  {
  public:
    //! The typedef for the concrete type.
    typedef T Type;
    //! The typedef for the parameter type.
    typedef P ParamType;

    //! Virtual destructor.
    /** Noop implementation. */
    virtual ~OneParamFactory() {};
    //! The actual factory method.
    virtual Type * instance(ParamType const&) const;
  };

  /**
   * @param param To be passed to the constructor of Type.
   * @return An instance of a type Type.
   */
  template<class T, class P, class B>
  T *
  OneParamFactory<T, P, B>::instance(ParamType const& _param) const
  {
    return new Type(_param);
  }
}
#endif
