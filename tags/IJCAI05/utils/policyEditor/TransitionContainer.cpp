// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2005
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "TransitionContainer.h"
#include "PatternWidget.h"

#include "miro/Log.h"

#include <qobjectlist.h>
#include <qlayout.h>

TransitionContainer::TransitionContainer(PatternWidget * _pattern,
					 QWidget * _parent, char * _name) :
  Super(_parent, _name),
  pattern_(_pattern)
{
  MIRO_ASSERT(pattern_ != NULL);
  QSizePolicy sp(QSizePolicy::Fixed, QSizePolicy::Fixed);
  setSizePolicy(sp);
  layout()->setResizeMode(QLayout::Fixed);

}

TransitionContainer::~TransitionContainer()
{
  pattern_->removeTransitionContainer();
}

void
TransitionContainer::removeChild(QObject * _obj)
{
  Super::removeChild(_obj);

  // remove automatically, if no childs are left
  if (children() == NULL ||
      children()->count() == 1)
    deleteLater();
}
