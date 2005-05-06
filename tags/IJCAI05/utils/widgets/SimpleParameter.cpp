// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "SimpleParameter.h"

#include "utils/widgets/ConfigFile.h"
#include "utils/widgets/SimpleParameterEdit.h"
#include "utils/widgets/ParameterDialog.h"
#include "utils/widgets/ParameterListDialog.h"
#include "utils/widgets/SingleParameterDialog.h"

#include "params/Generator.h"

#include "miro/Log.h"

#include <qpopupmenu.h>
#include <qlistview.h>

namespace 
{
  struct Pair {
    char const * name;
    SimpleParameter::Type type;
  };

  Pair typeMap [16] = {
    { "char",           SimpleParameter::CHAR },
    { "bool",           SimpleParameter::BOOL },
    { "short",          SimpleParameter::SHORT },
    { "unsigned short", SimpleParameter::USHORT },
    { "int",            SimpleParameter::INT },
    { "unsigned int",   SimpleParameter::UINT },
    { "long",           SimpleParameter::LONG },
    { "unsigned long",  SimpleParameter::ULONG },
    { "double",         SimpleParameter::DOUBLE },
    { "angle",          SimpleParameter::ANGLE },
    { "Miro::Angle",    SimpleParameter::MIRO_ANGLE },
    { "Angle",          SimpleParameter::MIRO_ANGLE },
    { "std::string",    SimpleParameter::STRING },
    { "string",         SimpleParameter::STRING },
    { "ACE_Time_Value", SimpleParameter::ACE_TIME_VALUE },
    { "ACE_INET_Addr",  SimpleParameter::ACE_INET_ADDR }
  };
}

QString const SimpleParameter::XML_ATTRIBUTE_VALUE ="value";

SimpleParameter::Type 
SimpleParameter::typeFromName(QString const& _type)
{
  for (unsigned int i = 0; i < 16; ++i) {
    if (_type == typeMap[i].name)
      return typeMap[i].type;
  }
  return SimpleParameter::NONE;
}

SimpleParameter::SimpleParameter(Miro::CFG::Parameter const& _param,
				 QDomNode const& _node,
				 QListViewItem * _parentItem,
				 QListViewItem * _pre,
				 QObject * _parent, const char * _name) :
  Super(_node, _parentItem, _pre, _parent, _name),
  param_(_param)
{
  QDomElement e = node().toElement();
  if (listViewItem()->listView()->columns() >= 2)
    listViewItem()->setText(1, e.attribute(XML_ATTRIBUTE_VALUE));
  if (listViewItem()->listView()->columns() >= 3)
    listViewItem()->setText(2, param_.type_);
}

SimpleParameter::SimpleParameter(Miro::CFG::Parameter const& _param,
				 QDomNode const& _node,
				 QListView * _list,
				 QListViewItem * _pre,
				 QObject * _parent, const char * _name) :
  Super(_node, _list, _pre, _parent, _name),
  param_(_param)
{
  QDomElement e = node().toElement();
  if (listViewItem()->listView()->columns() >= 2)
    listViewItem()->setText(1, e.attribute(XML_ATTRIBUTE_VALUE));
  if (listViewItem()->listView()->columns() >= 3)
    listViewItem()->setText(2, param_.type_);
}

void
SimpleParameter::setParameters()
{
  SingleParameterDialog dialog(param_,
			       node_,
			       NULL, this, NULL, name());
  int rc = dialog.exec();
  if (rc == QDialog::Accepted) {
    dialog.setXML();
    if (dialog.modified())
      setModified(true);
  }
}
