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
#ifndef notifyObserver_EventChannelFactory_h
#define notifyObserver_EventChannelFactory_h

#include "utils/widgets/Item.h"

#include <orbsvcs/CosNotifyChannelAdminS.h>

#include <vector>

// forward declarations
class QListView;

class EventChannelFactory : public Item
{
  Q_OBJECT
  
  typedef Item Super;

public:
  typedef std::vector<CosNotifyChannelAdmin::ChannelID> ChannelIDVector;

  EventChannelFactory(CosNotifyChannelAdmin::EventChannelFactory_ptr _notifyFactory,
		      QListView * _listView,
		      QObject * _parent = 0, const char * _name = 0);

  virtual void update();
  virtual void contextMenu(QPopupMenu& _menu);


public slots:
  void updateFactory();

protected slots:
  void createChannel();

protected:
  CosNotifyChannelAdmin::EventChannel_ptr getChannel(CosNotifyChannelAdmin::ChannelID _id);
  
  //! Channel factory.
  CosNotifyChannelAdmin::EventChannelFactory_var notifyFactory_;
};

#endif // notifyObserver_EventChannelFactory_h
