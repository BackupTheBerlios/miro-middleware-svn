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
#ifndef TransitionWidget_h
#define TransitionWidget_h

#include <qlabel.h>

// forward declarations
class TransitionXML;
class TransitionContainer;

class TransitionWidget : public QLabel
{
  Q_OBJECT;

  typedef QLabel Super;
public:
  TransitionWidget(TransitionXML * _transition,
		   TransitionContainer * _parent, char * _name);
  virtual ~TransitionWidget();

  TransitionXML * transition();
  
protected:
  TransitionXML * transition_;
};

inline
TransitionXML *
TransitionWidget::transition() {
  return transition_;
}

#endif // TransitionWidget_h
