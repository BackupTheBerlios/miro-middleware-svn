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
#ifndef PolicyDocument_h
#define PolicyDocument_h

#include "BehaviourDescription.h"      // BehaviourDescription
#include "Transition.h"                // Transition
#include "PolicyConfig.h"

#include <qstring.h> 
#include <qdom.h>
#include <qmap.h>

#include <list>            // STL list
#include <vector>          // STL vector
#include <string> 
#include <fstream>


const QString XML_TAG_PATTERN    = "actionpattern";
const QString XML_TAG_BEHAVIOUR  = "behaviour";
const QString XML_TAG_TRANSITION = "transition";
const QString XML_TAG_ARBITER    = "arbiter";
const QString XML_TAG_PARAMETER  = "parameter";
const QString XML_PARAM_KEY      = "name";  // <parameter name=".." value="..">
const QString XML_PARAM_VALUE    = "value";


/** This class represents all internal data of the PolicyEditor */
class PolicyDocumentClass
{
public:
  typedef std::vector<BehaviourDescription> DatabaseVector;

  const DatabaseVector& databaseVector() const;

  std::vector<QString>               arbiterVector;
  
public:
  PolicyDocumentClass();
  ~PolicyDocumentClass();
  
  /** returns policyConfig*/
  PolicyConfigClass& getPolicyConfig();
  const PolicyConfigClass& getPolicyConfig() const;

  //------------------//
  // document methods //
  //------------------//

  /** loads a new document from a given XML file */
  void loadXML(const QString& filename);

  /** saves the document to a given XML file */
  void saveXML(const QString& filename) const;

  /** loads the behaviour description database */
  void loadDatabase(const std::string& filename);
  
  void setModified(bool wert);
  
  bool getModified();
  
  void setWindowSize(int width, int height);

  QDomDocument& getDomDocument()  { return domDocument; }
  
  void setNewBehaviourDescriptionFileName(const QString& file);
  
  QString getBehaviourDescriptionFileName();


  //-----------------//
  // pattern methods //
  //-----------------//

  /** adds a new pattern to the policy */
  void addPattern(const QString& name, int x, int y);

  /** deletes a given pattern */
  void delPattern(const QString& name);

  void renamePattern(const QString& oldName, const QString& newName);

  /** returns a list with the names of all behaviours of the given pattern */
  QStringList getPatternNames() const;

  /** returns the x-coordinate of the given pattern */
  int getX(const QString& patternName) const;

  /** returns the y-coordinate of the given pattern */
  int getY(const QString& patternName) const;

  /** sets the x-coordinate of the given pattern */
  void setX(const QString& patternName, int x);

  /** sets the y-coordinate of the given pattern */
  void setY(const QString& patternName, int y);

  /** sets the given behaviour as start pattern */
  void setStartPattern(const QString& patternName);
 
  void setxModificator(int value);
  
  int getxModificator();

  void setyModificator(int value);
  
  int getyModificator();

  /** returns true if the pattern is the start pattern */
  bool isStartPattern(const QString& patternName) const;

  //-------------------//
  // behaviour methods //
  //-------------------//

  /** adds a new behaviour to a given pattern */
  void addBehaviour(const QString& patternName, int n);

  /** returns true if the specified pattern hat the specified behaviour*/
  bool hasBehaviour(const QString& patternName, const QString& behaviourName);
  
  /** deletes a given behaviour */
  void delBehaviour(const QString& patternName, const QString& behaviourName);

  /** returns the number of behaviours for the given pattern */
  int getNumBehaviours(const QString& patternName) const;

  /** returns a list with the names of all behaviours of the given pattern */
  QStringList getBehaviourNames(const QString& patternName) const;

  /** moves the behaviour one step higher in the pattern */
  void behaviourUp(const QString& patternName, const QString& behaviourName);

  /** moves the behaviour one step lower in the pattern */
  void behaviourDown(const QString& patternName, const QString& behaviourName);

  /** returns true if the behaviour has a predecessor */
  bool hasPrevBehaviour(const QString& patternName, 
			const QString& behaviourName) const
  {
    return (!getPrevBehaviourNode(patternName, behaviourName).isNull());
  }

  /** returns true if the behaviour has a successor */
  bool hasNextBehaviour(const QString& patternName, 
			const QString& behaviourName) const
  {
    return (!getNextBehaviourNode(patternName, behaviourName).isNull());
  }

  //-------------------//
  // parameter methods //
  //-------------------//

  /** returns a stringmap with all parameters of the given behaviour
   *  in the given pattern.  */
  QMap<QString, QString> getParameters(const QString& patternName,
				       const QString& behaviourName) const;

  /** adds a parameter to the given behaviour in the given pattern */
  void addParameter(const QString& patternName, 
		    const QString& behaviourName,
		    const QString& key,
		    const QString& value);

  /** sets a new set of parameters for the given behaviour in the given
   *  pattern. All existing parameters are deleted!  */
  void setParameters(const QString& patternName,
		     const QString& behaviourName, 
		     const QMap<QString, QString>& paramMap);

  /** deletes all parameters of the given behaviour in the given
   *  pattern.  */
  void delParameters(const QString& patternName, const QString& behaviourName);

  //--------------------//
  // transition methods //
  //--------------------//

  /** add a new transition from a pattern to a target */
  void addTransition(const QString& patternName, const QString& message, 
		     const QString& target);

  /** returns a list with all transitions of the given pattern */
  TransitionList getTransitions(const QString& patternName) const;
  
  /** deletes all transitions from the given pattern*/
  void delTransitionsFrom(const QString& name);

  /** deletes all transitions to a given pattern */
  void delTransitionsTo(const QString& name);


  //-----------------//
  // arbiter methods //
  //-----------------//

  /** set the arbiter for a given pattern */
  void setArbiter(const QString& patternName, const QString& arbiter);

  /** set the arbiter for a given pattern */
  void setArbiter(const QString& patternName, int arbiterIndex);

  /** returns the arbiter of a given pattern. If no arbiter is given,
   *  "" is returned. 
   */
  QString getArbiter(const QString& patternName) const;

  /** deletes all arbiters of the given pattern */
  void delArbiter(const QString& patternName);
  
protected:
  // returns the DOM node of the given pattern
  QDomNode getPatternNode(const QString& patternName) const;

  // returns the DOM node of the given behaviour in a pattern
  QDomNode getBehaviourNode(const QString& patternName, 
			    const QString& behaviourName) const;

  // returns the DOM node of the next behaviour in a pattern
  QDomNode getNextBehaviourNode(const QString& patternName, 
				const QString& behaviourName) const;

  // returns the DOM node of the previous behaviour in a pattern
  QDomNode getPrevBehaviourNode(const QString& patternName, 
				const QString& behaviourName) const;

protected:
  PolicyConfigClass policyConfig;
  
  
  //! The main document structure containing all patterns and behaviours
  QDomDocument domDocument;

  DatabaseVector databaseVector_;

  bool modified;

  int xModificator;
  int yModificator;

  int halfWindowWidth;
  int halfWindowHeight;
};

inline
const PolicyDocumentClass::DatabaseVector& 
PolicyDocumentClass::databaseVector() const {
  return databaseVector_;
}


inline
PolicyConfigClass&
PolicyDocumentClass::getPolicyConfig() {
  return policyConfig;
}

inline
const PolicyConfigClass&
PolicyDocumentClass::getPolicyConfig() const {
  return policyConfig;
}


#endif
