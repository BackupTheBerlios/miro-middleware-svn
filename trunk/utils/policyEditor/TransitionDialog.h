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
#ifndef TransitionDialog_h
#define TransitionDialog_h

#include "utils/widgets/OkCancelDialog.h"

#include <vector>

// Forward declarations
class QComboBox;

//! Transition edit dialogs of the configuration framework.
/** 
 * As every parameter type in the framework is a compound parameter,
 * this dialog is actually a compound parameter dialog.
 */
class TransitionDialog : public OkCancelDialog
{
  Q_OBJECT

  //----------------------------------------------------------------------------
  // private types
  //----------------------------------------------------------------------------

  //! Base class type.
  typedef OkCancelDialog Super;

public:
  //----------------------------------------------------------------------------
  // public methods
  //----------------------------------------------------------------------------

  //! Initializing constructor.
  TransitionDialog(QStringList const& _messages,
		   QStringList const& _targets,
		   QWidget * _parent, const char * _name);

  QString getMessage() const;
  QString getTarget() const;
  
private:
  //----------------------------------------------------------------------------
  // private data
  //----------------------------------------------------------------------------

  //! List of transition messages.
  QComboBox * message_;
  //! List of target patterns.
  QComboBox * target_;
};

#endif
