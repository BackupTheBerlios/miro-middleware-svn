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
#ifndef TransitinoContainer_h
#define TransitinoContainer_h

#include <qvbox.h>

// forward declarations
class PatternWidget;

class TransitionContainer : public QVBox
{
  Q_OBJECT;
  
  typedef QVBox Super;

public:
  TransitionContainer(PatternWidget * _pattern,
		      QWidget * _parent, char * _name);
  virtual ~TransitionContainer();

  virtual  void removeChild(QObject * _obj); 

protected:
  PatternWidget * pattern_;
};
#endif // TransitinoContainer_h
