// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
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
    virtual ~FactoryBase() throw () {};
    //! The actual factory method.
    /** @return An instance of a descendand of BaseType. */
    virtual BaseType * instance() const = 0;
  };

  //! Factory template for default constructible types.
  /**
   * @param T The concrete derived type of the factory.
   * @param B The common base type of the factory.
   */
  template<class T, class B>
  class Factory : public FactoryBase<B>
  {
  public:
    //! The typedef for the concrete type.
    typedef T Type;

    //! Virtual destructor.
    /** Noop implementation. */
    virtual ~Factory() throw () {};
    //! The actual factory method.
    virtual Type * instance() const;
  };

  /** @return An instance of a type Type. */
  template<class T, class B>
  T *
  Factory<T, B>::instance() const {
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
  template<class B, class P>
  class OneParamFactoryBase
  {
  public:
    //! The typedef for the base type.
    typedef B BaseType;
    //! The typedef for the parameter type.
    typedef P ParamType;

    //! Virtual destructor.
    /** Noop implementation. */
    virtual ~OneParamFactoryBase() throw () {};
    //! The actual factory method.
    /** @return An instance of a descendand of BaseType. */
    virtual BaseType * instance(ParamType const&) const = 0;
  };

  template<class T, class B, class P>
  class OneParamFactory : public OneParamFactoryBase<B, P>
  {
  public:
    //! The typedef for the concrete type.
    typedef T Type;

    //! Virtual destructor.
    /** Noop implementation. */
    virtual ~OneParamFactory() throw () {};
    //! The actual factory method.
    virtual Type * instance(ParamType const&) const;
  };

  /**
   * @param param To be passed to the constructor of Type.
   * @return An instance of a type Type. 
   */
  template<class T, class B, class P>
  T *
  OneParamFactory<T>::instance(ParamType const& _param) const {
    return new Type(_param);
  }
}
#endif
