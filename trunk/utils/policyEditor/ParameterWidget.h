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
#ifndef ParameterWidget_h
#define ParameterWidget_h

#include <qlabel.h>

// forward declarations
class ParameterXML;
class QPopupMenu;

/**
 * This class shows a behaviour name for use in PatternWidget
 */
class ParameterWidget : public QLabel
{
  Q_OBJECT

  typedef QLabel Super;

public:
  ParameterWidget(QColor _cNormal, QColor _cEntered,
		  ParameterXML * _parameter,
		  QWidget * _parent, char const * _name);
  virtual ~ParameterWidget();

protected:
  virtual void mousePressEvent(QMouseEvent*);
  virtual void mouseDoubleClickEvent(QMouseEvent* event);
  virtual void enterEvent(QEvent*);
  virtual void leaveEvent(QEvent*);

  QColor cNormal_;
  QColor cEntered_;
  ParameterXML * parameter_;
};
#endif // ParameterWidget_h
