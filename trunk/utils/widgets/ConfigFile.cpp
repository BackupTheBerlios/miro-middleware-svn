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

#define QT_ALTERNATE_QTSMANIP

#include "ConfigFile.h"

#include "params/Parser.h"
#include "params/Generator.h"
#include "params/TextErrorHandler.h"

#include <qfile.h>
#include <qdom.h>
#include <qxml.h>
#include <qmessagebox.h>

Miro::Singleton<ConfigFileName> ConfigFileName::instance;
Miro::Singleton<ConfigFile> ConfigFile::instance;

ConfigFileName::ConfigFileName() :
  fileName_(".MiroConfigFile.xml")
{}

ConfigFile::ConfigFile() :
  generator_(NULL),
  handler_(NULL)
{
  char const * const home = getenv("HOME");
  configFile_ = QString(home) + "/" + ConfigFileName::instance()->fileName();

  readConfigFile();  
}
  
ConfigFile::~ConfigFile()
{
  delete generator_;
  delete handler_;
}


void 
ConfigFile::setDescriptionFiles(const QStringList& _descriptions)
{
  descriptionFiles_ = _descriptions;

  writeConfigFile();
} 	


/**
 *if no BehaviourDescriptionFile was loaded (i.e. behaviourFile=="") then
 *the function reads the path of the BehaviourDescriptionFile, which is used last, out of the Config-XML-File. 
 *else it only returns the Name of the loaded BehaviourDescriptionFile (i. e. behaviourFile);
 */
void 
ConfigFile::readConfigFile()
{
  descriptionFiles_.clear();

  // try to open policy editor config file
  QFile file(configFile_);
  if (file.open(IO_ReadOnly)) {
    QDomDocument doc("MiroConfigDocument");
    if (doc.setContent(&file)) {

      QDomNode n = doc.documentElement().firstChild();
      while (!n.isNull()) {
	QDomElement e = n.toElement();
	if (e.tagName() == "config_item" &&
	    e.attribute("name") == "files") {
	  
	  QDomNode node = n.firstChild();
	  while (!node.isNull()) {
	    e = node.toElement();
	    if (e.tagName() == "parameter" &&
		e.attribute("name") == "file") {
	      //add behaviour description file to list
	      descriptionFiles_.append(node.toElement().attribute("value"));
	    } 	     
	    node = node.nextSibling();
	  }	
	}
	n = n.nextSibling();
      }
    }  
    else {
      QString infoText("Error parsing configuration file\n" + 
		       configFile_ +
		       "\nStarting with empty configuration.");
      QMessageBox::information(0, "Policy Editor", infoText);
    }
    file.close();
  }
  else {
    QString infoText("Error opening configuration file\n" + 
		     configFile_ +
		     "\nStarting with empty configuration.");
    QMessageBox::information(0, "Policy Editor", infoText);
    // create a minimal config file for the next run
    writeConfigFile();
  }

  parseDescriptionFiles();
}

void 
ConfigFile::writeConfigFile()
{
  parseDescriptionFiles();

  QDomDocument doc( "MiroConfigDocument" );
  QDomElement root = doc.createElement( "Configuration" );
  doc.appendChild( root );

  QDomElement tag = doc.createElement( "config_item" );
  tag.setAttribute("name", "files");
  root.appendChild(tag);
  for (unsigned int i = 0; i < descriptionFiles_.count(); ++i) {
    QDomElement f = doc.createElement( "parameter" );
    f.setAttribute("name", "file");
    f.setAttribute("value", descriptionFiles_[i]);
    tag.appendChild(f);
  }

  QFile f(configFile_);
  if (!f.open(IO_WriteOnly)) {
    throw QString("PolicyConfig::writeConfigFile: Error: file error!");
  }
  QTextStream ts(&f);
  doc.save(ts, 0);
}

void 
ConfigFile::parseDescriptionFiles()
{
  bool okay = false;
  while (!okay) {
    delete generator_;
    generator_ = new Miro::CFG::Generator;
    delete handler_;
    handler_ = new Miro::CFG::Parser(*generator_);
    
    QStringList::Iterator first, last = descriptionFiles_.end();
    for (first = descriptionFiles_.begin(); first != last ; ++first) {

      // kill previous namespace nesting
      generator_->clearNamespace();

      QString fileName = *first;
      QFile xmlFile( fileName );
      if (!xmlFile.exists()) {
      QString infoText("Error parsing behaviour description file: \n" +
		       fileName + "\n" +
		       "File not found.");
      QMessageBox::information(0, "Policy Editor", infoText);
      descriptionFiles_.remove(first);
      break;
      }
      QXmlInputSource source( xmlFile );
      QXmlSimpleReader reader;
      Miro::CFG::TextErrorHandler errorHandler;
      
      reader.setContentHandler( handler_ );
      reader.setErrorHandler( &errorHandler);

      if (!reader.parse( source )) {
	QString infoText("Error parsing behaviour description file:\n" +
			 fileName + "\n" +
			 errorHandler.errorString());
	QMessageBox::information(0, "Policy Editor", infoText);
	descriptionFiles_.remove(first);
	break;
      }
    }
    // parsing successfull
    okay = true;

    // std::cout << *generator_ << endl;
  }
}
