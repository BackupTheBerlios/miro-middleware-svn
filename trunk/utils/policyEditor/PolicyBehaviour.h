// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef PolicyBehaviour_h
#define PolicyBehaviour_h

#include <qstring.h> 
#include <qmap.h>
#include <qdom.h>


// forward declarations
class PolicyDocumentClass;

/** This class represents all internal data of the PolicyEditor */
class PolicyBehaviour
{
public:
  PolicyBehaviour(PolicyDocumentClass& _document, QDomNode _node);

  /** returns a stringmap with all parameters of the given behaviour
   *  in the given pattern.  */
  QMap<QString, QString> getParameters() const;

  /** sets a new set of parameters for the given behaviour in the given
   *  pattern. All existing parameters are deleted!  */
  void setParameters(const QMap<QString, QString>& paramMap);

protected:
  /** adds a parameter to the given behaviour in the given pattern */
  void addParameter(const QString& key, const QString& value);

  /** deletes all parameters of the given behaviour in the given
   *  pattern.  */
  void delParameters();


  //! The main document structure containing all patterns and behaviours
  PolicyDocumentClass& document_;
  QDomNode node_;
};

#endif
