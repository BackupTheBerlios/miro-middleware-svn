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

      //! Initializing constructor.
      Type();

      //! Mark a type to be final.
      /** 
       * None final types are expected to be not instanciated.  They
       * will be therfore omitted by the graphical editing tools like
       * ConfigEditor and PolicyEditor.
       */
      void setFinal(bool _final);
      //! Predicate indicating the type is final.
      bool isFinal() const;
      //! Mark a type as dummy.
      /** 
       * The type implementation is not autogenerated, but hand crafted.
       * The type description is therfore only for editing purposes.
       */
      void setDummy(bool _dummy);
      //! Predicate indicating the type is dummy.
      bool isDummy() const;
      void setExtern(bool _extern);
      bool isExtern() const;
      //! Set the name of the type.
      void setName(QString const& _name);
      //! Query the name of the type.
      /** This excludes the namespace as well as the Parameters suffix. */
      QString const& name() const;
      //! Query the fully qualified name of the type.
      /** This includes the namespaces as well as the Parameters suffix. */
      QString fullName() const;
      void setParent(QString const& _parent);
      //! Set the parent class.
      /** Parameter types support single public inheritance. */
      QString const& parent() const;
      //! Set the namespace for the type.
      /** Namespace can be actually a nested namespace. */
      void setNameSpace(QString const& _nameSpace);
      //! Query the namespace of the type.
      QString const& nameSpace() const;
      //! Mark a type as having a singel global instance.
      void setInstance();
      //! Predicate inidcating the type has a single global instance.
      bool instance() const;

      //! Add as static member.
      void addStatic(QString const& _type, QString const& _name);
      //! Add as parameter.
      void addParameter(Parameter const& _parameter);
      //! Add as static const parameter.
      void addStaticConstParameter(Parameter const& _parameter);
      //! Add to constrctor.
      /** Workaround for specifying defaults for complex types. */
      void addToConstructor(QString const& _ctor);

      //! Generate header file code.
      void generateHeader(std::ostream& ostr, unsigned long _indent) const;
      //! Generate source file code.
      void generateSource(std::ostream& ostr, unsigned long _indent) const;

      //! Accessor to the parameters of the type.
      ParameterVector const& parameterSet() const;

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
				   QString const& classPrefix, 
				   unsigned long indent) const;
      void generateQDomInOperator(std::ostream& ostr, 
				  QString const& classPrefix,
				  unsigned long indent) const;

      //----------------------------------------------------------------------------
      // protected static methods
      //----------------------------------------------------------------------------
      static bool isIntegralType(QString const& _type);

      //----------------------------------------------------------------------------
      // protected data
      //----------------------------------------------------------------------------

      QString name_;
      QString parent_;
      QString nameSpace_;

      bool final_;
      bool dummy_;
      bool extern_;
      bool instance_;

      QStringVector ctor_;
      ParameterVector parameter_;
      QStringPairVector staticData_;
      ParameterVector staticConstParameter_;

      //----------------------------------------------------------------------------
      // protected static data
      //----------------------------------------------------------------------------
      static unsigned long const STEP = 2;
    };

    inline
    ParameterVector const&
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
    Type::setName(QString const& _name) {
      name_ = _name;
    }
    
    inline
    QString const&
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
    Type::setParent(QString const& _parent) {
      parent_ = _parent;
    }
    
    inline
    QString const&
    Type::parent() const {
      return parent_;
    }
    
    inline
    void
    Type::setNameSpace(QString const& _nameSpace) {
      nameSpace_ = _nameSpace;
    }
    
    inline
    QString const&
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
