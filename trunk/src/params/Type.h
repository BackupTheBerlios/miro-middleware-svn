// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2002, 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef Miro_CFG_Type_h
#define Miro_CFG_Type_h

#include "Parameter.h"

#include <vector>
#include <set>
#include <map>

namespace Miro
{
  namespace CFG
  {
    //! Class defining a parameter type.
    class Type
    {
    public:
      //------------------------------------------------------------------------
      // public types
      //------------------------------------------------------------------------

      //! Set holding the parameters of a parameter type.
      typedef std::set<Parameter> ParameterSet;

      //! Initializing constructor.
      Type();

      void setFinal(bool _final);
      bool isFinal() const;
      void setDummy(bool _dummy);
      bool isDummy() const;
      void setExtern(bool _extern);
      bool isExtern() const;
      void setName(const QString& _name);
      const QString& name() const;
      QString fullName() const;
      void setParent(const QString& _parent);
      const QString& parent() const;
      void setNameSpace(const QString& _nameSpace);
      const QString& nameSpace() const;
      bool instance() const;
      void setInstance();

      //! Add as static member.
      void addStatic(const QString& _type, const QString& _name);
      //! Add as parameter.
      void addParameter(const Parameter& _parameter);
      //! Add to constrctor.
      /** Workaround for specifying defaults for complex types. */
      void addToConstructor(const QString& _ctor);

      //! Generate header file code.
      void generateHeader(std::ostream& ostr, unsigned long _indent) const;
      //! Generate source file code.
      void generateSource(std::ostream& ostr, unsigned long _indent) const;

      //! Accessor to the parameters of the type.
      ParameterSet const& parameterSet() const;

    protected:
      //----------------------------------------------------------------------------
      // protected types
      //----------------------------------------------------------------------------

      typedef std::pair<QString, QString> QStringPair;
      typedef std::vector<QStringPair> QStringPairVector;
      typedef std::vector<QString> QStringVector;

      //----------------------------------------------------------------------------
      // protected methods
      //----------------------------------------------------------------------------
      void generateQDomOutOperator(std::ostream& ostr, 
				   const QString& classPrefix, 
				   unsigned long indent) const;
      void generateQDomInOperator(std::ostream& ostr, 
				  const QString& classPrefix,
				  unsigned long indent) const;

      //----------------------------------------------------------------------------
      // protected data
      //----------------------------------------------------------------------------
      QStringPairVector staticData_;

      QString name_;
      QString parent_;
      QString nameSpace_;

      bool final_;
      bool dummy_;
      bool extern_;
      bool instance_;

      QStringVector ctor_;
      ParameterSet parameter_;

      //----------------------------------------------------------------------------
      // protected static data
      //----------------------------------------------------------------------------
      static unsigned long const STEP = 2;
    };

    inline
    const Type::ParameterSet&
    Type::parameterSet() const {
      return parameter_;
    }
    
    inline
    void 
    Type::setFinal(bool _final) {
      final_ = _final;
    }
    
    inline
    bool 
    Type::isFinal() const {
      return final_;
    }
    
    inline
    void 
    Type::setDummy(bool _dummy) {
      dummy_ = _dummy;
    }
    
    inline
    bool 
    Type::isDummy() const {
      return dummy_;
    }
    
    inline
    void 
    Type::setExtern(bool _extern) {
      extern_ = _extern;
    }
    
    inline
    bool 
    Type::isExtern() const {
      return extern_;
    }
    
    inline
    void
    Type::setName(const QString& _name) {
      name_ = _name;
    }
    
    inline
    const QString&
    Type::name() const {
      return name_;
    }
    
    inline
    QString
    Type::fullName() const {
      QString full = nameSpace_ + name_;
      if (!dummy_)
	full += "Parameters";
      return full;
    }
    
    inline
    void
    Type::setParent(const QString& _parent) {
      parent_ = _parent;
    }
    
    inline
    const QString&
    Type::parent() const {
      return parent_;
    }
    
    inline
    void
    Type::setNameSpace(const QString& _nameSpace) {
      nameSpace_ = _nameSpace;
    }
    
    inline
    const QString&
    Type::nameSpace() const {
      return nameSpace_;
    }
    inline
    bool
    Type::instance() const {
      return instance_;
    }
    inline
    void
    Type::setInstance() {
      addStatic(QString("Miro::Singleton<") + 
		QString(name()) + "Parameters>",
		"instance");

      instance_ = true;
    }
  }
}
#endif // Miro_CFG_Type_h
