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

#include "PolicyPattern.h"
#include "PolicyDocument.h"

PolicyPattern::PolicyPattern(PolicyDocumentClass& _documnet,
			     QDomNode _node) :
  document_(_documnet),
  node_(_node)
{
}

void 
PolicyPattern::setWindowSize(int width, int height)
{
  halfWindowWidth = width/2;
  halfWindowHeight = height/2;
}

//------------------------------------------------------------------//
//---------------------- protected methods ---------------------------//
//------------------------------------------------------------------//


// returns the DOM node of the given behaviour in a pattern //
QDomNode 
PolicyPattern::getBehaviourNode(const QString& behaviourName)
{
  // check for each child: if it is a behaviour -> add to list //
  QDomNode n = node_.firstChild();
  while(!n.isNull()) {
    QDomElement e = n.toElement();
    // if tagName == "behaviour" -> check name //
    if (e.tagName() == XML_TAG_BEHAVIOUR) {

      QString name =e.attribute("name", "");
      
      if (name == behaviourName) 
	return n;
    }

    n = n.nextSibling();
  }

  // behaviour node not found //
  throw QString("PolicyDocumentClass::getBehaviourNode: behaviour '" + 
		behaviourName + "' not found in pattern '" + 
		node_.toElement().attribute("name") + "'");
}

// returns the DOM node of the previous behaviour in a pattern //
QDomNode 
PolicyPattern::getPrevBehaviourNode(const QString& behaviourName)
{
  QDomNode behaviourNode = getBehaviourNode(behaviourName);
  QDomNode previousNode = behaviourNode.previousSibling();

  // get previous node until a behaviour is found //
  while(!previousNode.isNull()) {
    if (previousNode.toElement().tagName() == XML_TAG_BEHAVIOUR)
      break;
    
    previousNode = previousNode.previousSibling();
  } 
  return previousNode;
}


// returns the DOM node of the next behaviour in a pattern //
QDomNode
PolicyPattern::getNextBehaviourNode(const QString& behaviourName)
{
  QDomNode behaviourNode = getBehaviourNode(behaviourName);
  QDomNode nextNode = behaviourNode.nextSibling();

  // get next node until a behaviour is found //
  while(!nextNode.isNull()) {
    if (nextNode.toElement().tagName() == XML_TAG_BEHAVIOUR) 
      break;

    nextNode = nextNode.nextSibling();
  } 
  return nextNode;
}

//-----------------------------------------------------------------//
//---------------------- public methods ---------------------------//
//-----------------------------------------------------------------//

void 
PolicyPattern::remove()
{
  // remove DOM node //
  node_.ownerDocument().documentElement().removeChild(node_);

  delTransitionsTo();
  document_.setModified();
}

bool 
PolicyPattern::rename(const QString& newName)
{
  QDomElement element = node_.toElement();
  QString name = element.attribute("name");
  // identity allway succeedes
  if (name == newName)
    return true;

  // make sure that the new name is unique
  QDomElement docElem = node_.ownerDocument().documentElement();
  QDomNode n = docElem.firstChild();
  bool unique = true;

  while(!n.isNull()) {
    // if tagName == "pattern" -> check name //
    QDomElement e = n.toElement();
    if (e.tagName() == XML_TAG_PATTERN &&
	e.attribute("name") == newName) {
      unique = false;
      break;
    }
    n = n.nextSibling();
  }

  if (unique) {
    // rename node
    element.setAttribute("name", newName);
 
    // rename all transition targets
    n = docElem.firstChild();
    while(!n.isNull()) {

      // if tagName == "pattern" -> check name //
      if (n.toElement().tagName() == XML_TAG_PATTERN) {
	QDomNode node = n.firstChild();
	while(!node.isNull()) {

	  // if tagName == "transition" -> add a transition object to the list //
	  QDomElement e = node.toElement();
	  if (e.tagName() == XML_TAG_TRANSITION &&
	      e.attribute("target", "-") == name) {
	    e.setAttribute("target", newName);
	  }
	  node = node.nextSibling();
	}
      }
      n = n.nextSibling();
    }
    
    document_.setModified();
  }
  return unique;
}

int 
PolicyPattern::getX() 
{
  QDomElement e = node_.toElement();
  return (e.attribute("x", "0").toInt() - xModificator + halfWindowWidth);
}


int 
PolicyPattern::getY() 
{
  QDomElement e = node_.toElement();
  return (e.attribute("y", "0").toInt() - yModificator + halfWindowHeight);
}


void 
PolicyPattern::setX(int x)
{
  QDomElement e = node_.toElement();
  e.setAttribute("x", x + xModificator-halfWindowWidth);
  document_.setModified();
}


void 
PolicyPattern::setY(int y)
{
  QDomElement e = node_.toElement();
  e.setAttribute("y", y + yModificator - halfWindowHeight);
  document_.setModified();
}

void
PolicyPattern::setStartPattern()
{
  //  for all patterns: set "start" attributes to "false" //
  QDomElement docElem = node_.ownerDocument().documentElement();

  // check for each child: if it is a pattern //
  QDomNode n = docElem.firstChild();
  while(!n.isNull()) {

    // if tagName == "pattern" -> set attribute //
    QDomElement e = n.toElement();
    if (e.tagName() == XML_TAG_PATTERN) {
      e.setAttribute("start", "false");
    }
    n = n.nextSibling();
  }


  // for given pattern: set start attribute to "true" //
  node_.toElement().setAttribute("start", "true");

  document_.setModified();
}


bool 
PolicyPattern::isStartPattern()
{
  return node_.toElement().attribute("start") == "true";
}

//---------------------- behaviour methods ---------------------------//


QDomNode
PolicyPattern::addBehaviour(const QString& behaviourName)
{
  document_.setModified();

  // create new DOM element for the behaviour //
  QDomElement element = node_.ownerDocument().createElement(XML_TAG_BEHAVIOUR);

  // set attributes //
  element.setAttribute("name", behaviourName);

  // append element to pattern node //
  return node_.appendChild(element);
}

bool 
PolicyPattern::hasBehaviour(const QString& behaviourName)
{
  bool rc = true;
  QDomNode node = node_.firstChild();
  
  while (!node.isNull()) {
    QDomElement e = node.toElement();
    if (e.tagName() == XML_TAG_BEHAVIOUR &&
	e.attribute("name") == behaviourName) {
      rc = true;
      break;
    }
    node = node.nextSibling();
  }

  return rc;
}

// deletes a given behaviour //
void 
PolicyPattern::delBehaviour(const QString& behaviourName)
{
  // get DOM node of the given pattern and behaviour //
  QDomNode behaviourNode = getBehaviourNode(behaviourName);
  
  // remove DOM node //
  node_.removeChild(behaviourNode);

  document_.setModified();
}


int 
PolicyPattern::getNumBehaviours() const
{
  int behaviourCounter = 0;

  // check for each child: if it is a behaviour -> increase counter //
  QDomNode node = node_.firstChild();
  while(!node.isNull()) {

    // if tagName == "behaviour" -> increase counter //
    if (node.toElement().tagName() == XML_TAG_BEHAVIOUR) {
      ++behaviourCounter;
    }

    node = node.nextSibling();
  }
  
  return behaviourCounter;
}


QStringList 
PolicyPattern::getBehaviourNames() const
{
  QStringList list;

  // check for each child: if it is a behaviour -> add to list //
  QDomNode node = node_.firstChild();
  while(!node.isNull()) {
    QDomElement e = node.toElement();
    // if tagName == "behaviour" -> add behaviour name to string list //
    if (e.tagName() == XML_TAG_BEHAVIOUR) {
      list += e.attribute("name");
    }

    node = node.nextSibling();
  }
  
  return list;
}


void
PolicyPattern::behaviourUp(const QString& behaviourName)
{
  // get DOM node of the given behaviour //
  QDomNode behaviourNode     = getBehaviourNode(behaviourName);
  QDomNode prevBehaviourNode = getPrevBehaviourNode(behaviourName);

  if (prevBehaviourNode.isNull()) 
    return;

  // remove DOM node //
  node_.insertBefore(behaviourNode, prevBehaviourNode);

  document_.setModified();
}


void
PolicyPattern::behaviourDown(const QString& behaviourName)
{
  // get DOM node of the given behaviour //
  QDomNode behaviourNode     = getBehaviourNode(behaviourName);
  QDomNode nextBehaviourNode = getNextBehaviourNode(behaviourName);

  if (nextBehaviourNode.isNull()) 
    return;

  // remove DOM node //
  node_.insertAfter(behaviourNode, nextBehaviourNode);

  document_.setModified();
}



//---------------------- transition functions ---------------------------//

TransitionList 
PolicyPattern::getTransitions() const
{
  TransitionList list;

  // check for each child: if it is a transition -> add to list //
  QDomNode node = node_.firstChild();
  while(!node.isNull()) {
    QDomElement e = node.toElement();
    // if tagName == "transition" -> add a transition object to the list //
    if (e.tagName() == XML_TAG_TRANSITION) {

      QString message = e.attribute("message");
      QString target  = e.attribute("target");

      Transition transition(message, target);
      list.push_back(transition);
    }

    node = node.nextSibling();
  }

  return list;
}


QDomNode 
PolicyPattern::addTransition(const QString& message, const QString& target)
{
  QDomNode rc;
  bool unique = true;

  // check for each child: if it is a transition -> add to list //
  QDomNode node = node_.firstChild();
  while(!node.isNull()) {
    QDomElement e = node.toElement();
    // if tagName == "transition" -> add a transition object to the list //
    if (e.tagName() == XML_TAG_TRANSITION &&
	e.attribute("message") == message) {
      unique = false;
      break;
    }
    node = node.nextSibling();
  }

  if (unique) {
    document_.setModified();
    // create new DOM element for the behaviour //
    QDomElement element = node_.ownerDocument().createElement(XML_TAG_TRANSITION);
    
    // set attributes //
    element.setAttribute("message", message);
    element.setAttribute("target",  target);
    
    // append element to pattern node //
    rc = node_.appendChild(element);
    
  }
  return rc;
}

void 
PolicyPattern::delTransitionsFrom()
{
  QDomNode node = node_.firstChild();
  while(!node.isNull()) {
    QDomNode tmp = node;
    node = node.nextSibling();

    if (tmp.toElement().tagName() == XML_TAG_TRANSITION) {
      node_.removeChild(tmp);
    }
  }
}


void 
PolicyPattern::delTransitionsTo()
{
  QString name = node_.toElement().attribute("name");
  QDomElement docElem = node_.ownerDocument().documentElement();

  // for all actionPatterns ... //
  QDomNode patternNode = docElem.firstChild();
  while(!patternNode.isNull()) {
    
    // for all children (behaviours, transitions, arbiters) ... //
    QDomNode node = patternNode.firstChild();
    while(!node.isNull()) {

      QDomNode tmp = node;
      QDomElement element = tmp.toElement();
      node = node.nextSibling();

      // if tagName == "transition" -> check target //
      // if target == pattern name -> delete transition //
      if (element.tagName() == XML_TAG_TRANSITION &&
	  element.attribute("target") == name) {
	patternNode.removeChild(tmp);
	document_.setModified();
      }
    }

    patternNode = patternNode.nextSibling();
  }
}


//---------------------- arbiter functions ---------------------------//


QDomNode 
PolicyPattern::setArbiter(const QString& arbiter)
{
  document_.setModified();

  // deletes current arbiter
  delArbiter();

  // create new DOM element for the behaviour
  QDomElement element = node_.ownerDocument().createElement(XML_TAG_ARBITER);

  // set attributes
  element.setAttribute("name", arbiter);

  // append element to pattern node
  return node_.appendChild(element);
}


QString 
PolicyPattern::getArbiter() const
{
  // check for each child: if it is an arbiter //
  QDomNode node = node_.firstChild();
  while(!node.isNull()) {
    QDomElement e = node.toElement();
    // if tagName == "arbiter" -> return arbiter name //
    if (e.tagName() == XML_TAG_ARBITER) {
      return e.attribute("name", "");
    }

    node = node.nextSibling();
  }

  return QString("");
}


void 
PolicyPattern::delArbiter()
{
  // COMMENT: this removes ALL arbiters (although only one should be set!)

  // check for each child: if it is an arbiter //
  QDomNode n = node_.firstChild();
  while(!n.isNull()) {
    QDomNode tmp = n;
    n = n.nextSibling();
    
    // if tagName == "arbiter" -> remove node //
    if (tmp.toElement().tagName() == XML_TAG_ARBITER) {
      node_.removeChild(tmp);
    }
  }
}
