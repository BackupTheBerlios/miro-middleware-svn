// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 1999-2005
// Department of Neuroinformatics, University of Ulm, Germany
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// $Id$
//
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
