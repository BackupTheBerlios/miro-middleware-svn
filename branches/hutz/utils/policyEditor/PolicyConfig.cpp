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

#include "PolicyConfig.h"
#include "PolicyConfigurationDialog.h"

#include <qfile.h>
#include <string>
#include <qmessagebox.h>

#include <fstream>

PolicyConfigClass::PolicyConfigClass()
{
  /*
    char *nixRoot=getenv("NIX_ROOT");
    configFile=QString(nixRoot)+"/etc/PolicyEditor_Nix.xml";
    QFile f(behaviourFile);
    if (!f.open(IO_ReadOnly))
    {
    throw string("PolicyConfigClass(): Error: file not found or access denied!");
    }
    domDocument_behaviours.setContent(&f);
    f.close();
  */
  QString infoText;
  char *home=getenv("HOME");
  configFile=QString(home)+"/.PolicyEditorConfig.xml";
  QFile f(configFile);
  if (!f.open(IO_ReadOnly))
  {
    infoText=QString(".PolicyEditorConfig.xml couldn't be found in your Home-Directory\n") + 
      QString("Try to create the configuration file");
    QMessageBox::information(0, "Policy Editor",  infoText,"OK",0);
    char *nR=getenv("NIX_ROOT");
    QString nixRoot=QString(nR);
    if (nixRoot.compare("")==0){
      infoText=QString("Couldn't create configuration-file '~/.PolicyEditorConfig.xml'!\n")+
	QString("Ensure that NIX_ROOT was set to the correct path of the Nix-Project!\n")+
	QString("Closing PolicyEditor!"); 
      QMessageBox::information(0,"Policy Editor", infoText );
      throw string("PolicyConfigClass(): Error: Couldn't create configuration-file '~/.PolicyEditorConfig.xml'");
    }
    QFile df(nixRoot+"/etc/DefaultPolicyEditorConfig.xml");
    if (!df.open(IO_ReadOnly)){
      infoText=QString("Couldn't create configuration-file '~/.PolicyEditorConfig.xml'") + 
	QString("Default-config-file couldn't be loaded")+
	QString("Closing PolicyEditor!");
      QMessageBox::information(0, "Policy Editor", infoText);
      throw string("PolicyConfigClass(): Error: Couldn't create configuration-file '~/.PolicyEditorConfig.xml'");
    }
    else{
      domDocument_config.setContent(&df);
      df.close();
      ofstream outStream(configFile.ascii());
      outStream << domDocument_config.toString() << endl;
			
    }		
		
  }
  else {
    //Pfad zum behaviour-description-file aus PolicyEditorConfig.xml auslesen
    domDocument_config.setContent(&f);
    f.close();
  }
  behaviourFile="";
  behaviourFile=getBehaviourDescriptionFileName();
  if (behaviourFile.compare("")==0){
    infoText=QString("No Behaviour-Description-File was specified!\n")+
      QString("Opining an empty Behaviour-Description-File.\n")+
      QString("Choose  \"Load Behaviour-Description-Filename...\" in Menu \"Options\"")+
      QString("to specify your own Behaviour-Description-File");
    QMessageBox::information(0, "Policy Editor", infoText);
    char *nR=getenv("NIX_ROOT");
    QString nixRoot=QString(nR);
    if (nixRoot.compare("")==0){
      infoText=QString("Error in loading the empty Behaviour-Description-File!\n")+
	QString("Ensure that NIX_ROOT was set to the correct path of the Nix-Project!\n")+
	QString("Closing PolicyEditor!"); 
      QMessageBox::information(0, "Policy Editor", infoText);
      throw string("PolicyConfigClass(): Error: Couldn't load empty Behaviour-Description-File");
    }
    behaviourFile=nixRoot+"/etc/emptyBehaviourDescriptionFile.xml";
  }
	
  getBehaviourDescription();  
	
}
  
vector<BehaviourDescription>
PolicyConfigClass::getBehaviours() const
{
  vector<BehaviourDescription> vectorBehaviours;
  vectorBehaviours.clear();
  QDomNode n=domDocument_behaviours.documentElement().firstChild();
  QDomNode node;
  while (!n.isNull())
  {
    if (n.toElement().tagName()=="config_group")
    {
      if (n.toElement().attribute("name")=="behaviour")
      {
	node=n.firstChild();
	while (!node.isNull())
	{
	  if (node.toElement().tagName()=="config_item")
	  {
	    vectorBehaviours.push_back(BehaviourDescription(
							    node.toElement().attribute("name")));
	  }
	  node=node.nextSibling();
	}
	return vectorBehaviours;
      }
    }
    n=n.nextSibling();
  }
  return vectorBehaviours;
}
     

vector<QString> 
PolicyConfigClass::getArbiters() const
{
  vector<QString> vectorArbiters;
  vectorArbiters.clear();
  QDomNode n=domDocument_behaviours.documentElement().firstChild();
  QDomNode node;
  while (!n.isNull())
  {
    if (n.toElement().tagName()=="config_group")
    {
      if (n.toElement().attribute("name")=="arbiter")
      {
	node=n.firstChild();
	while (!node.isNull())
	{
	  if (node.toElement().tagName()=="config_item")
	  {
	    vectorArbiters.push_back(QString(
					     node.toElement().attribute("name")));
	  }
	  node=node.nextSibling();
	}
	return vectorArbiters;
      }
    }
    n=n.nextSibling();
  }
  return vectorArbiters;
}

vector<BehaviourParam> 
PolicyConfigClass::getBehaviourParams(const QString& behaviourName) const
{  
  vector<BehaviourParam> vectorBehaviourParams;
  vectorBehaviourParams.clear();
  QDomNode n=domDocument_behaviours.documentElement().firstChild();
  QDomNode node;
  while (!n.isNull())
  {
    if (n.toElement().tagName()=="config_group")
    {
      if (n.toElement().attribute("name")=="behaviour")
      {
	node=n.firstChild();
	while (!node.isNull())
	{
	  if (node.toElement().tagName()=="config_item")
	  {
	    if (node.toElement().attribute("name")==behaviourName) 
	    {
	      QDomNode paramNode=node.firstChild();
	      while (!paramNode.isNull())
	      {
		if (paramNode.toElement().tagName()=="config_parameter")
		{
		  vectorBehaviourParams.push_back(BehaviourParam(
								 paramNode.toElement().attribute("name"),
								 paramNode.toElement().attribute("type"),
								 paramNode.toElement().attribute("measure")));
		}
		paramNode=paramNode.nextSibling();
	      }
	      return vectorBehaviourParams;
	    }
	  }
	  node=node.nextSibling();
	}
	return vectorBehaviourParams;
      }
    }
    n=n.nextSibling();
  }
  return vectorBehaviourParams;
}

void 
PolicyConfigClass::setNewBehaviourDescriptionFileName(const QString& file)
{
  behaviourFile=file;
  cout <<behaviourFile <<endl;
  getBehaviourDescription();
  setBehaviourDescriptionFileName();
} 	


/**
 *if no BehaviourDescriptionFile was loaded (i.e. behaviourFile=="") then
 *the function reads the path of the BehaviourDescriptionFile, which is used last, out of the Config-XML-File. 
 *else it only returns the Name of the loaded BehaviourDescriptionFile (i. e. behaviourFile);
 */
QString PolicyConfigClass::getBehaviourDescriptionFileName() const
{
  if (behaviourFile=="")
  {
    QDomNode n=domDocument_config.documentElement().firstChild();
    QDomNode node;
    while (!n.isNull())
    {
      if (n.toElement().tagName()=="configItem")
      {
	if (n.toElement().attribute("name")=="files")
	{
	  node=n.firstChild();
	  while (!node.isNull())
	  {
	    if (node.toElement().tagName()=="file")
	    {
	      if ((node.toElement().attribute("name"))=="behaviour-description-file"){
		//PolicyEditorConfig.xml auslesen
		return node.toElement().attribute("path");
	      } 	     
	    }
	    node=node.nextSibling();
	  }	
	  throw string("behaviour-description-file not specified in PolicyEditorConfig.xml");
	}
      }
      n=n.nextSibling();
    }
    throw string("behaviour-description-file not specified in PolicyEditorConfig.xml");
  }
  else
  {
    return behaviourFile;
  }
}

void 
PolicyConfigClass::setBehaviourDescriptionFileName()
{
  QDomNode n=domDocument_config.documentElement().firstChild();
  QDomNode node;
  while (!n.isNull())
  {
    if (n.toElement().tagName()=="configItem")
    {
      if (n.toElement().attribute("name")=="files")
      {
        node=n.firstChild();
        while (!node.isNull())
        {
          if (node.toElement().tagName()=="file")
          {
	    if ((node.toElement().attribute("name"))=="behaviour-description-file"){
	      //PolicyEditorConfig.xml auslesen
	      node.toElement().setAttribute("path", behaviourFile);
	      ofstream outStream(configFile.ascii());
	      outStream << domDocument_config.toString() << endl;
	      return;
	    } 	     
          }
	  node=node.nextSibling();
	}	
	throw string("behaviour-description-file not specified in PolicyEditorConfig.xml");
      }
    }
    n=n.nextSibling();
  }
  throw string("behaviour-description-file not specified in PolicyEditorConfig.xml");
}

void
PolicyConfigClass::getBehaviourDescription()
{
  QFile ff(behaviourFile);
  if (!ff.open(IO_ReadOnly))
  {
    ff.close();	
    QString infoText=QString("Can't find/read specified Behaviour-Description-File\n")+
      QString("Opining an empty Behaviour-Description-File.\n")+
      QString("Choose  \"Load Behaviour-Description-Filename...\" in Menu \"Options\"")+
      QString("to specify your own Behaviour-Description-File");
    QMessageBox::information(0, "Policy Editor", infoText);
    char *nR=getenv("NIX_ROOT");
    QString nixRoot=QString(nR);
    if (nixRoot.compare("")==0){
      infoText=QString("Error in loading the empty Behaviour-Description-File!\n")+
	QString("Ensure that NIX_ROOT was set to the correct path of the Nix-Project!\n")+
	QString("Closing PolicyEditor!"); 
      QMessageBox::information(0, "Policy Editor", infoText);
      throw string("[PolicyConfigClass.getBehaviourDescription()]: Error: Couldn't load empty Behaviour-Description-File");
    }
    behaviourFile=nixRoot+"/etc/emptyBehaviourDescriptionFile.xml";
    QFile emptyFile(behaviourFile);
    if (!emptyFile.open(IO_ReadOnly)){
      emptyFile.close();
      throw string("[PolicyConfigClass.getBehaviourDescription()] Error on loading the empty Behaviour-Description-File! \n Exit!");
    }
    cout <<behaviourFile <<endl;
    domDocument_behaviours.setContent(&emptyFile);
    emptyFile.close();
		 
  }
  else {
    cout <<behaviourFile <<endl;
    domDocument_behaviours.setContent(&ff);
    ff.close();
  }
}

/* Not supported yet
   void PolicyConfigClass::setConfiguration(QWidget *parent)
   {
   ConfigurationDialog* configurationDialog;
   configurationDialog=new ConfigurationDialog(parent,this);    
   configurationDialog->exec();
   }*/
