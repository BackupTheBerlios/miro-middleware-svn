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

#include "PolicyDocument.h"
#include "PolicyConfig.h"

#include <qfile.h>
#include <qstringlist.h>
#include <qdom.h>

#include <iostream>

using std::cout;
using std::endl;

PolicyDocumentClass::PolicyDocumentClass() :
  policyConfig(*PolicyConfigClass::instance()),
  document_(NULL),
  modified(false),
  xModificator(0),
  yModificator(0)
{
  PolicyDocumentClass::init();
}

PolicyDocumentClass::~PolicyDocumentClass() 
{
  delete document_;
}

void
PolicyDocumentClass::init()
{
  delete document_;
  document_ = new QDomDocument("MiroPolicyConfig");
  // init default document
  QDomElement root = document_->createElement( "policy" );
  document_->appendChild( root );
  documentName_ = "";
  modified = false;

  //  cout << document_->toString() << std::endl;
}

void 
PolicyDocumentClass::setWindowSize(int width, int height)
{
  halfWindowWidth = width/2;
  halfWindowHeight = height/2;
}

//------------------------------------------------------------------//
//---------------------- protected methods ---------------------------//
//------------------------------------------------------------------//


// returns the DOM node of the given pattern //
QDomNode
PolicyDocumentClass::getPatternNode(const QString& patternName) const
{
  QDomElement docElem = document_->documentElement();

  // check for each child: if it is a pattern //
  QDomNode node = docElem.firstChild();
  while(!node.isNull()) {

    // if tagName == "pattern" -> check name //
    if (node.toElement().tagName() == XML_TAG_PATTERN) {
    
      QString name = node.toElement().attribute("name", "");

      if (name == patternName) 
	return node;
    }

    node = node.nextSibling();
  }
  
  // pattern node not found //
  throw QString("PolicyDocumentClass::getPatternNode: pattern '" + 
		    patternName + "' not found");
}


// returns the DOM node of the given behaviour in a pattern //
QDomNode 
PolicyDocumentClass::getBehaviourNode(const QString& patternName, 
				      const QString& behaviourName) const
{
  QDomNode patternNode = getPatternNode(patternName);

  // check for each child: if it is a behaviour -> add to list //
  QDomNode node = patternNode.firstChild();
  while(!node.isNull()) {

    // if tagName == "behaviour" -> check name //
    if (node.toElement().tagName() == XML_TAG_BEHAVIOUR) {

      QString name = node.toElement().attribute("name", "");
      
      if (name == behaviourName) 
	return node;
    }

    node = node.nextSibling();
  }

  // behaviour node not found //
  throw QString("PolicyDocumentClass::getBehaviourNode: behaviour '" + 
		    behaviourName + "' not found in pattern '" + patternName + "'");
}


// returns the DOM node of the previous behaviour in a pattern //
QDomNode PolicyDocumentClass::getPrevBehaviourNode(const QString& patternName, 
			const QString& behaviourName) const
{
  QDomNode behaviourNode = getBehaviourNode(patternName, behaviourName);

  QDomNode previousNode = behaviourNode.previousSibling();

  // get previous node until a behaviour is found //
  while(previousNode.toElement().tagName() != XML_TAG_BEHAVIOUR) {
    
    previousNode = previousNode.previousSibling();
    
    if (previousNode.isNull()) return previousNode;
  } 

  return previousNode;
}


// returns the DOM node of the next behaviour in a pattern //
QDomNode PolicyDocumentClass::getNextBehaviourNode(const QString& patternName, 
			const QString& behaviourName) const
{
  QDomNode behaviourNode = getBehaviourNode(patternName, behaviourName);

  QDomNode nextNode = behaviourNode.nextSibling();

  // get next node until a behaviour is found //
  while(nextNode.toElement().tagName() != XML_TAG_BEHAVIOUR) {

    nextNode = nextNode.nextSibling();
    
    if (nextNode.isNull()) return nextNode;
  } 

  return nextNode;
}



//-----------------------------------------------------------------//
//---------------------- public methods ---------------------------//
//-----------------------------------------------------------------//



//---------------------- document methods ---------------------------//

void
PolicyDocumentClass::setDocumentName(const QString& _name)
{
  documentName_ = _name;
  modified = true;
}

void 
PolicyDocumentClass::loadXML(const QString& filename) 
{
  modified = false;
  document_->clear();

  QFile f(filename);
  if (!f.open(IO_ReadOnly)) {
    throw QString("PolicyDocumentClass::loadXML: Error: file not found!");
  }
  if (!document_->setContent(&f)) {
    throw QString("PolicyDocumentClass::loadXML: Error: XML parsing error!");
  }
  documentName_ = filename;
}


void
PolicyDocumentClass::saveXML()
{
  QFile f(documentName_ );
  if (!f.open(IO_WriteOnly)) {
    throw QString("PolicyDocumentClass::loadXML: Error: file error!");
  }
  QTextStream ts(&f);
  document_->save(ts, 0);
  modified = false;
}


//---------------------- pattern methods ---------------------------//

bool 
PolicyDocumentClass::addPattern(const QString& name, int x, int y)
{
  // The pattern name has to be unique.
  // This pattern has to be Start-Pattern, 
  // if it's the first pattern at all
  QDomElement docElem = document_->documentElement();
  QDomNode n = docElem.firstChild();
  bool hasPatterns = false;
  bool unique = true;

  while(!n.isNull()) {
    // if tagName == "pattern" -> check name //
    QDomElement e = n.toElement();
    if (e.tagName() == XML_TAG_PATTERN) {
      hasPatterns = true;
      if (e.attribute("name") == name) {
	unique = false;
	break;
      }
    }
    n = n.nextSibling();
  }

  if (unique) {
    // create new DOM element //
    QDomElement element = document_->createElement(XML_TAG_PATTERN);

    // set attributes //
    element.setAttribute("name", name);

    element.setAttribute("x", x + xModificator - halfWindowWidth);
    element.setAttribute("y", y + yModificator - halfWindowHeight);

    if (hasPatterns)
      element.setAttribute("start", "false");
    else 
      element.setAttribute("start", "true");

    // append element to main node //
    docElem.appendChild(element);
  
    modified = true;
  }
  return unique;
}



void 
PolicyDocumentClass::delPattern(const QString& name)
{
  // get DOM node of the given pattern //
  QDomNode patternNode = getPatternNode(name);
  
  // remove DOM node //
  document_->documentElement().removeChild(patternNode);

  delTransitionsTo(name);
  modified = true;
}




bool 
PolicyDocumentClass::renamePattern(const QString& oldName, const QString& newName)
{
  if (oldName == newName)
    return true;

  // make sure that the new name is unique
  QDomElement docElem = document_->documentElement();
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
    QDomNode patternNode = getPatternNode(oldName);
    patternNode.toElement().setAttribute("name", newName);
    // get DOM node of the given pattern //
 
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
	      e.attribute("target", "-") == oldName) {
	    e.setAttribute("target", newName);
	    cout <<"change transition: OK" <<endl;
	  }
	  node = node.nextSibling();
	}
      }
      n = n.nextSibling();
    }
    
    modified = true;
  }
  return unique;
}




QStringList PolicyDocumentClass::getPatternNames() const
{
  QStringList list;

  QDomElement docElem = document_->documentElement();

  // all nodes on the second level are actionPatterns! //
  QDomNode node = docElem.firstChild();
  while(!node.isNull()) {
    
    QDomElement element = node.toElement();
    QString name = element.attribute("name", "");

    list += name;

    node = node.nextSibling();
  }
  
  return list;
}


int 
PolicyDocumentClass::getX(const QString& patternName) const
{
  return (getPatternNode(patternName).toElement().attribute("x",
  "0").toInt() - xModificator + halfWindowWidth);
}


int 
PolicyDocumentClass::getY(const QString& patternName) const
{
  return (getPatternNode(patternName).toElement().attribute("y",
      "0").toInt()-yModificator+halfWindowHeight);
}


void 
PolicyDocumentClass::setX(const QString& patternName, int x)
{
  getPatternNode(patternName).toElement().setAttribute("x",
      x+xModificator-halfWindowWidth);
  modified = true;

}


void 
PolicyDocumentClass::setY(const QString& patternName, int y)
{
  getPatternNode(patternName).toElement().setAttribute("y",
      y+yModificator-halfWindowHeight);
  modified = true;

}

void 
PolicyDocumentClass::setxModificator(int value)
{
  xModificator=value;
}
    

int 
PolicyDocumentClass::getxModificator()
{
  return xModificator;
}

void
PolicyDocumentClass::setyModificator(int value)
{
  yModificator=value;
}
    

int 
PolicyDocumentClass::getyModificator()
{
  return yModificator;
}

void
PolicyDocumentClass::setStartPattern(const QString& patternName)
{
  //  for all patterns: set "start" attributes to "false" //
  QDomElement docElem = document_->documentElement();

  // check for each child: if it is a pattern //
  QDomNode node = docElem.firstChild();
  while(!node.isNull()) {

    // if tagName == "pattern" -> set attribute //
    if (node.toElement().tagName() == XML_TAG_PATTERN) {
      node.toElement().setAttribute("start", "false");
    }

    node = node.nextSibling();
  }


  // for given pattern: set start attribute to "true" //
  getPatternNode(patternName).toElement().setAttribute("start", "true");

  modified = true;
}


bool 
PolicyDocumentClass::isStartPattern(const QString& patternName) const
{
  return 
    (getPatternNode(patternName).toElement().attribute("start") == "true");
}

//---------------------- behaviour methods ---------------------------//


void
PolicyDocumentClass::addBehaviour(const QString& patternName, const QString& behaviourName)
{
  // get DOM node of the given pattern //
  QDomNode patternNode = getPatternNode(patternName);

  // create new DOM element for the behaviour //
  QDomElement element = document_->createElement(XML_TAG_BEHAVIOUR);

  // set attributes //
  element.setAttribute("name", behaviourName);

  // append element to pattern node //
  patternNode.appendChild(element);

  modified = true;
}

bool 
PolicyDocumentClass::hasBehaviour(const QString& patternName, 
				  const QString& behaviourName)
{
 QDomNode patternNode = getPatternNode(patternName);
 QDomNode node = patternNode.firstChild();

 while (!node.isNull()) {
   if (node.toElement().tagName()==XML_TAG_BEHAVIOUR) {
     if (node.toElement().attribute("name")==behaviourName) 
       return true;
   }
   node = node.nextSibling();
 }

 return false;
}

// deletes a given behaviour //
void 
PolicyDocumentClass::delBehaviour(const QString& patternName, 
				       const QString& behaviourName)
{
  // get DOM node of the given pattern and behaviour //
  QDomNode patternNode   = getPatternNode(patternName);
  QDomNode behaviourNode = getBehaviourNode(patternName, behaviourName);
  
  // remove DOM node //
  patternNode.removeChild(behaviourNode);

  modified = true;
}


int 
PolicyDocumentClass::getNumBehaviours(const QString& patternName) const
{
  // get DOM node of the given pattern //
  QDomNode patternNode = getPatternNode(patternName);

  int behaviourCounter = 0;

  // check for each child: if it is a behaviour -> increase counter //
  QDomNode node = patternNode.firstChild();
  while(!node.isNull()) {

    // if tagName == "behaviour" -> increase counter //
    if (node.toElement().tagName() == XML_TAG_BEHAVIOUR) {
      behaviourCounter++;
    }

    node = node.nextSibling();
  }
  
  return behaviourCounter;
}


QStringList 
PolicyDocumentClass::getBehaviourNames(const QString& patternName) const
{
  QStringList list;

  // get DOM node of the given pattern //
  QDomNode patternNode = getPatternNode(patternName);

  // check for each child: if it is a behaviour -> add to list //
  QDomNode node = patternNode.firstChild();
  while(!node.isNull()) {

    // if tagName == "behaviour" -> add behaviour name to string list //
    if (node.toElement().tagName() == XML_TAG_BEHAVIOUR) {
      list += node.toElement().attribute("name");
    }

    node = node.nextSibling();
  }
  
  return list;
}


void
PolicyDocumentClass::behaviourUp(const QString& patternName, 
				      const QString& behaviourName)
{
  // get DOM node of the given pattern and behaviour //
  QDomNode patternNode       = getPatternNode(patternName);
  QDomNode behaviourNode     = getBehaviourNode(patternName, behaviourName);
  QDomNode prevBehaviourNode = getPrevBehaviourNode(patternName,behaviourName);

  if (prevBehaviourNode.isNull()) 
    return;

  // remove DOM node //
  patternNode.insertBefore(behaviourNode, prevBehaviourNode);

  modified = true;
}


void
PolicyDocumentClass::behaviourDown(const QString& patternName, 
				      const QString& behaviourName)
{
  // get DOM node of the given pattern and behaviour //
  QDomNode patternNode       = getPatternNode(patternName);
  QDomNode behaviourNode     = getBehaviourNode(patternName, behaviourName);
  QDomNode nextBehaviourNode = getNextBehaviourNode(patternName,behaviourName);

  if (nextBehaviourNode.isNull()) 
    return;

  // remove DOM node //
  patternNode.insertAfter(behaviourNode, nextBehaviourNode);

  modified = true;
}



//---------------------- transition functions ---------------------------//

TransitionList 
PolicyDocumentClass::getTransitions(const QString& patternName) const
{
  TransitionList list;

  // get DOM node of the given pattern //
  QDomNode patternNode = getPatternNode(patternName);

  // check for each child: if it is a transition -> add to list //
  QDomNode node = patternNode.firstChild();
  while(!node.isNull()) {

    // if tagName == "transition" -> add a transition object to the list //
    if (node.toElement().tagName() == XML_TAG_TRANSITION) {

      QString message = node.toElement().attribute("message");
      QString target  = node.toElement().attribute("target");

      Transition transition(message, target);
      list.push_back(transition);
    }

    node = node.nextSibling();
  }

  return list;
}


bool 
PolicyDocumentClass::addTransition(const QString& patternName, 
				   const QString& message,  
				   const QString& target)
{
  bool rc = true;

  // get DOM node of the given pattern //
  QDomNode patternNode = getPatternNode(patternName);

  // create new DOM element for the behaviour //
  QDomElement element = document_->createElement(XML_TAG_TRANSITION);

  // set attributes //
  element.setAttribute("message", message);
  element.setAttribute("target",  target);

  // append element to pattern node //
  patternNode.appendChild(element);

  modified = true;

  return rc;
}

void 
PolicyDocumentClass::delTransitionsFrom(const QString& name)
{
  /// ???

  QDomNode patternNode=getPatternNode(name);
  if (!patternNode.isNull()) {
    QDomNode node=patternNode.firstChild();
    while(!node.isNull()) {
      if (node.toElement().tagName() == XML_TAG_TRANSITION) {
	patternNode.removeChild(node);
	node = patternNode.firstChild();
      }
      else {
        node = node.nextSibling();
      }
    }
  }
}


void 
PolicyDocumentClass::delTransitionsTo(const QString& name)
{
  QDomElement docElem = document_->documentElement();

  // for all actionPatterns ... //
  QDomNode patternNode = docElem.firstChild();
  while(!patternNode.isNull()) {
    
    // for all children (behaviours, transitions, arbiters) ... //
    QDomNode node = patternNode.firstChild();
    while(!node.isNull()) {

      // if tagName == "transition" -> check target //
      if (node.toElement().tagName() == XML_TAG_TRANSITION) {
	
	QDomElement element = node.toElement();
	// if target == pattern name -> delete transition //
	if (element.attribute("target") == name) {
	  patternNode.removeChild(node);
	  node = patternNode.firstChild();	// TODO: improve!
	  modified = true;
	}
      }
      
      // TODO: check if this does the right thing when a node is deleted!
      node = node.nextSibling();   
    }

    patternNode = patternNode.nextSibling();
  }
}


//---------------------- arbiter functions ---------------------------//


void 
PolicyDocumentClass::setArbiter(const QString& patternName, 
				const QString& arbiter)
{
  // deletes current arbiter
  delArbiter(patternName);

  // get DOM node of the given pattern
  QDomNode patternNode = getPatternNode(patternName);

  // create new DOM element for the behaviour
  QDomElement element = document_->createElement(XML_TAG_ARBITER);

  // set attributes
  element.setAttribute("name", arbiter);

  // append element to pattern node
  patternNode.appendChild(element);

  modified = true;
}


QString 
PolicyDocumentClass::getArbiter(const QString& patternName) const
{
  // get DOM node of the given pattern //
  QDomNode patternNode = getPatternNode(patternName);

  // check for each child: if it is an arbiter //
  QDomNode node = patternNode.firstChild();
  while(!node.isNull()) {

    // if tagName == "arbiter" -> return arbiter name //
    if (node.toElement().tagName() == XML_TAG_ARBITER) {
      return node.toElement().attribute("name", "");
    }

    node = node.nextSibling();
  }

  return QString("");
}


void 
PolicyDocumentClass::delArbiter(const QString& patternName)
{
  // COMMENT: this removes ALL arbiters (although only one should be set!)

  // get DOM node of the given pattern //
  QDomNode patternNode = getPatternNode(patternName);

  // check for each child: if it is an arbiter //
  QDomNode node = patternNode.firstChild();
  while(!node.isNull()) {

    // if tagName == "arbiter" -> remove node //
    if (node.toElement().tagName() == XML_TAG_ARBITER) {
      patternNode.removeChild(node);
      node = patternNode.firstChild();  // TODO: improve
    }

    node = node.nextSibling();
  }
}


//---------------------- parameter methods ---------------------------//

// returns a stringmap with all parameters of the given behaviour in
// the given pattern.
QMap<QString, QString>
PolicyDocumentClass::getParameters(const QString& patternName,
				   const QString& behaviourName) const
{
  QMap<QString, QString> stringMap;

  // get DOM node of the given behaviour in the given pattern //
  QDomNode behaviourNode = getBehaviourNode(patternName, behaviourName);

  // check for each child: if it is a parameter //
  QDomNode node = behaviourNode.firstChild();
  while(!node.isNull()) {

    // if tagName == "parameter" -> add to stringMap //
    QString tagName = node.toElement().tagName();
    if (tagName == XML_TAG_PARAMETER) {
    
      QString name  = node.toElement().attribute(XML_PARAM_KEY,   "");
      QString value = node.toElement().attribute(XML_PARAM_VALUE, "");

      stringMap.insert(name, value);
    }

    node = node.nextSibling();
  }
  
  return stringMap;
}


// sets a new set of parameters for the given behaviour in the given //
// pattern. All existing parameters are deleted!                     //
void 
PolicyDocumentClass::setParameters(const QString& patternName,
				   const QString& behaviourName, 
				   const QMap<QString, QString>& paramMap)
{
  // delete current parameters //
  delParameters(patternName, behaviourName);

  // add all parameters in the paramMap //
  QMap<QString, QString>::ConstIterator iter;
  for (iter=paramMap.begin(); iter!=paramMap.end(); ++iter) {
    addParameter(patternName, behaviourName, iter.key(), iter.data());
  }
}


// adds a parameter to the given behaviour in the given pattern //
void 
PolicyDocumentClass::addParameter(const QString& patternName, 
				  const QString& behaviourName,
				  const QString& key,
				  const QString& value)
{
  // get DOM node of the given behaviour //
  QDomNode behaviourNode = getBehaviourNode(patternName, behaviourName);

  // create new DOM element //
  QDomElement element = document_->createElement(XML_TAG_PARAMETER);

  // set attributes //
  element.setAttribute(XML_PARAM_KEY,   key);
  element.setAttribute(XML_PARAM_VALUE, value);

  // append element to main node //
  behaviourNode.appendChild(element);
}


// deletes all parameters of the given behaviour in the given pattern //
void
PolicyDocumentClass::delParameters(const QString& patternName, 
				   const QString& behaviourName)
{
  // ???

  // get DOM node of the given behaviour //
  QDomNode behaviourNode = getBehaviourNode(patternName, behaviourName);

  // check for each child: if it is a parameter //
  QDomNode node = behaviourNode.firstChild();
  while(!node.isNull()) {

    // if tagName == "parameter" -> remove node //
    if (node.toElement().tagName() == XML_TAG_PARAMETER) {

      behaviourNode.removeChild(node);
      node = behaviourNode.firstChild();  // TODO: improve
      continue;
    }

    node = node.nextSibling();
  }
}
