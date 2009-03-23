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
#ifndef EventTypeDialog_h
#define EventTypeDialog_h

#include <qdialog.h>

#include <orbsvcs/CosNotificationC.h>

// forward declarations
class QLineEdit;

class EventTypeDialog : public QDialog
{
  Q_OBJECT;

  typedef QDialog Super;

public:
  EventTypeDialog(CosNotification::EventType const& _eventType,
		  QWidget * _parent = NULL, const char * _name = NULL);

  CosNotification::EventType result() const;

protected:
  QLineEdit * domainName_;
  QLineEdit * typeName_;
};

#endif // EventTypeDialog_h
