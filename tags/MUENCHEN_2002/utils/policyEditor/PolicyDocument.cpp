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

#include <iostream>

using std::ofstream;
using std::cout;
using std::endl;

PolicyDocumentClass::PolicyDocumentClass() :
  xModificator(0),
  yModificator(0)
{
}

PolicyDocumentClass::~PolicyDocumentClass() 
{
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
QDomNode PolicyDocumentClass::getPatternNode(const QString& patternName) const
{
  QDomElement docElem = domDocument.documentElement();

  // check for each child: if it is a pattern //
  QDomNode node = docElem.firstChild();
  while(!node.isNull()) {

    // if tagName == "pattern" -> check name //
    if (node.toElement().tagName() == XML_TAG_PATTERN) {
    
      QString name = node.toElement().attribute("name", "");

      if (name == patternName) return node;
    }

    node = node.nextSibling();
  }
  
  // pattern node not found //
  throw std::string("PolicyDocumentClass::getPatternNode: pattern '" + 
		    patternName + "' not found");
}


// returns the DOM node of the given behaviour in a pattern //
QDomNode PolicyDocumentClass::getBehaviourNode(const QString& patternName, 
			const QString& behaviourName) const
{
  QDomNode patternNode = getPatternNode(patternName);

  // check for each child: if it is a behaviour -> add to list //
  QDomNode node = patternNode.firstChild();
  while(!node.isNull()) {

    // if tagName == "behaviour" -> check name //
    if (node.toElement().tagName() == XML_TAG_BEHAVIOUR) {

      QString name = node.toElement().attribute("name", "");
      
      if (name == behaviourName) return node;
    }

    node = node.nextSibling();
  }

  // behaviour node not found //
  throw std::string("PolicyDocumentClass::getBehaviourNode: behaviour '" + 
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
PolicyDocumentClass::loadXML(const QString& filename) 
{
  QFile f(filename);
  if (!f.open(IO_ReadOnly))  
    throw std::string("PolicyDocumentClass::loadXML: Error: file not found!");

  if (!domDocument.setContent(&f)) {
    f.close();
    return;
  }
  f.close();
}


void
PolicyDocumentClass::saveXML(const QString& filename) const
{
  ofstream outStream(filename.ascii());
  outStream << domDocument.toString() << endl;
}


void 
PolicyDocumentClass::loadDatabase(const std::string& /* _filename */ ) 
{ 
  databaseVector_ = policyConfig.getBehaviours();
  arbiterVector = policyConfig.getArbiters();
}

void 
PolicyDocumentClass::setModified(bool wert)
{
  modified=wert;
}

bool
PolicyDocumentClass::getModified()
{
  return modified;
}


void 
PolicyDocumentClass::setNewBehaviourDescriptionFileName(const QString& file)
{
  policyConfig.setNewBehaviourDescriptionFileName(file);
}

QString PolicyDocumentClass::getBehaviourDescriptionFileName()
{
  return policyConfig.getBehaviourDescriptionFileName();
}

//---------------------- pattern methods ---------------------------//

void 
PolicyDocumentClass::addPattern(const QString& name, int x, int y)
{
  // create new DOM element //
  QDomElement element = domDocument.createElement(XML_TAG_PATTERN);

  // set attributes //
  element.setAttribute("name", name);
  //element.setAttribute("x", x+xModificator);
  //element.setAttribute("y", y+yModificator);
  printf("%i\n", xModificator);
  element.setAttribute("x", x+xModificator-halfWindowWidth);
  element.setAttribute("y", y+yModificator-halfWindowHeight);
  cout <<element.attribute("x") <<endl;
  cout <<element.attribute("y") <<endl;
  //element.setAttribute("start", "false");    // default: start="false"
	QDomElement docElem=domDocument.documentElement();
	//This pattern has to be Start-Pattern, if it's the first pattern at all
	QDomNode nnode = docElem.firstChild();
	bool hasPatterns=false;
  while(!nnode.isNull()) {

    // if tagName == "pattern" -> check name //
    if (nnode.toElement().tagName() == XML_TAG_PATTERN) {
			hasPatterns=true;
			break;
		}
		nnode = nnode.nextSibling();
	}
	if (hasPatterns==true) element.setAttribute("start", "false");
	else element.setAttribute("start", "true");
  // append element to main node //
  //domDocument.documentElement().appendChild(element);
	docElem.appendChild(element);
}



void 
PolicyDocumentClass::delPattern(const QString& name)
{
  // get DOM node of the given pattern //
  QDomNode patternNode = getPatternNode(name);
  
  // remove DOM node //
  domDocument.documentElement().removeChild(patternNode);

  delTransitionsTo(name);
}




void 
PolicyDocumentClass::renamePattern(const QString& oldName, const QString& newName)
{
 QDomNode patternNode = getPatternNode(oldName);
 patternNode.toElement().setAttribute("name", newName);
 // get DOM node of the given pattern //
 
 
 QDomElement docElem = domDocument.documentElement();

  QDomNode nnode = docElem.firstChild();
  while(!nnode.isNull()) {

    // if tagName == "pattern" -> check name //
    if (nnode.toElement().tagName() == XML_TAG_PATTERN) {
    
     
      QDomNode node = nnode.firstChild();
      while(!node.isNull()) {

     // if tagName == "transition" -> add a transition object to the list //
        if (node.toElement().tagName() == XML_TAG_TRANSITION) {

          if (node.toElement().attribute("target","-")==oldName)
	  {
	    node.toElement().setAttribute("target", newName);
	    cout <<"change transition: OK" <<endl;
	  }
        }

        node = node.nextSibling();
      }

      
      
    }

    nnode = nnode.nextSibling();
  }
}




QStringList PolicyDocumentClass::getPatternNames() const
{
  QStringList list;

  QDomElement docElem = domDocument.documentElement();

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
}


void 
PolicyDocumentClass::setY(const QString& patternName, int y)
{
  getPatternNode(patternName).toElement().setAttribute("y",
      y+yModificator-halfWindowHeight);
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
  QDomElement docElem = domDocument.documentElement();

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
}


bool 
PolicyDocumentClass::isStartPattern(const QString& patternName) const
{
  return 
    (getPatternNode(patternName).toElement().attribute("start") == "true");
}

//---------------------- behaviour methods ---------------------------//


void
PolicyDocumentClass::addBehaviour(const QString& patternName, int n)
{
  // get DOM node of the given pattern //
  QDomNode patternNode = getPatternNode(patternName);

  // create new DOM element for the behaviour //
  QDomElement element = domDocument.createElement(XML_TAG_BEHAVIOUR);

  // set attributes //
  element.setAttribute("name", databaseVector_[n].getName());

  // append element to pattern node //
  patternNode.appendChild(element);
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


void 
PolicyDocumentClass::addTransition(const QString& patternName, 
				   const QString& message,  
				   const QString& target)
{
  // get DOM node of the given pattern //
  QDomNode patternNode = getPatternNode(patternName);

  // create new DOM element for the behaviour //
  QDomElement element = domDocument.createElement(XML_TAG_TRANSITION);

  // set attributes //
  element.setAttribute("message", message);
  element.setAttribute("target",  target);

  // append element to pattern node //
  patternNode.appendChild(element);
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
  QDomElement docElem = domDocument.documentElement();

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
				int arbiterIndex)
{
  setArbiter(patternName, arbiterVector[arbiterIndex]);
}


void 
PolicyDocumentClass::setArbiter(const QString& patternName, 
				const QString& arbiter)
{
  // deletes current arbiter
  delArbiter(patternName);

  // get DOM node of the given pattern
  QDomNode patternNode = getPatternNode(patternName);

  // create new DOM element for the behaviour
  QDomElement element = domDocument.createElement(XML_TAG_ARBITER);

  // set attributes
  element.setAttribute("name", arbiter);

  // append element to pattern node
  patternNode.appendChild(element);
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
  QDomElement element = domDocument.createElement(XML_TAG_PARAMETER);

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
