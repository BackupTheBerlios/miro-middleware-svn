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
#ifndef PolicyPattern_h
#define PolicyPattern_h

#include "Transition.h"                // Transition

#include <qstring.h> 
#include <qstringlist.h>
#include <qdom.h>

// forward declarations
class QDomDocument;
class QDomNode;
class PolicyDocumentClass;

/** This class represents all internal data of the PolicyEditor */
class PolicyPattern
{
public:
  PolicyPattern(PolicyDocumentClass& _document, QDomNode _node);

  void setWindowSize(int width, int height);

  //-----------------//
  // pattern methods //
  //-----------------//

  /** deletes a given pattern */
  void remove();
  bool rename(const QString& newName);

  /** returns the x-coordinate of the given pattern */
  int getX();

  /** returns the y-coordinate of the given pattern */
  int getY();

  /** sets the x-coordinate of the given pattern */
  void setX(int x);

  /** sets the y-coordinate of the given pattern */
  void setY(int y);

  /** sets the given behaviour as start pattern */
  void setStartPattern();
 
  void setxModificator(int value);
  
  int getxModificator() const;

  void setyModificator(int value);
  
  int getyModificator() const;

  /** returns true if the pattern is the start pattern */
  bool isStartPattern();

  //-------------------//
  // behaviour methods //
  //-------------------//

  /** adds a new behaviour to a given pattern */
  QDomNode addBehaviour(const QString& behaviourName);

  /** returns true if the specified pattern hat the specified behaviour*/
  bool hasBehaviour(const QString& behaviourName);
  
  /** deletes a given behaviour */
  void delBehaviour(const QString& behaviourName);

  /** returns the number of behaviours for the given pattern */
  int getNumBehaviours() const;

  /** returns a list with the names of all behaviours of the given pattern */
  QStringList getBehaviourNames() const;

  /** moves the behaviour one step higher in the pattern */
  void behaviourUp(const QString& behaviourName);

  /** moves the behaviour one step lower in the pattern */
  void behaviourDown(const QString& behaviourName);

  /** returns true if the behaviour has a predecessor */
  bool hasPrevBehaviour(const QString& behaviourName);

  /** returns true if the behaviour has a successor */
  bool hasNextBehaviour(const QString& behaviourName);

  //--------------------//
  // transition methods //
  //--------------------//

  /** add a new transition from a pattern to a target */
  QDomNode addTransition(const QString& message, const QString& target);

  /** returns a list with all transitions of the given pattern */
  TransitionList getTransitions() const;
  
  /** deletes all transitions from the given pattern*/
  void delTransitionsFrom();

  /** deletes all transitions to a given pattern */
  void delTransitionsTo();


  //-----------------//
  // arbiter methods //
  //-----------------//

  /** set the arbiter for a given pattern */
  QDomNode setArbiter(const QString& arbiter);

  /** returns the arbiter of a given pattern. If no arbiter is given,
   *  "" is returned. 
   */
  QString getArbiter() const;

  /** deletes all arbiters of the given pattern */
  void delArbiter();
  
protected:
  // returns the DOM node of the given behaviour in a pattern
  QDomNode getBehaviourNode(const QString& behaviourName);

  // returns the DOM node of the next behaviour in a pattern
  QDomNode getNextBehaviourNode(const QString& behaviourName);

  // returns the DOM node of the previous behaviour in a pattern
  QDomNode getPrevBehaviourNode(const QString& behaviourName);

protected:
  PolicyDocumentClass& document_;
  QDomNode node_;

  int xModificator;
  int yModificator;

  int halfWindowWidth;
  int halfWindowHeight;
};

inline
bool
PolicyPattern::hasPrevBehaviour(const QString& behaviourName) {
  return (!getPrevBehaviourNode(behaviourName).isNull());
}

inline
bool
PolicyPattern::hasNextBehaviour(const QString& behaviourName) {
  return (!getNextBehaviourNode(behaviourName).isNull());
}

inline
void 
PolicyPattern::setxModificator(int value) {
  xModificator = value;
}
    
inline
int 
PolicyPattern::getxModificator() const {
  return xModificator;
}

inline
void
PolicyPattern::setyModificator(int value) {
  yModificator = value;
}
    

inline
int 
PolicyPattern::getyModificator() const {
  return yModificator;
}

#endif
