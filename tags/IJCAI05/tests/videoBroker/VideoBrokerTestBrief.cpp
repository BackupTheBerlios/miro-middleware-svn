// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "idl/VideoC.h"
#include "miro/Client.h"
#include "miro/TimeHelper.h"

#include <iostream>

using std::cout;
using std::cerr;
using std::endl;
using std::flush;
using std::cin;

unsigned int indent = 0;

std::ostream& operator << (std::ostream& _ostr, const Miro::TimeStatsIDL& _rhs)
{
   std::string in;
  for (unsigned int i = 0; i < indent; ++i)
    in += " ";

  ACE_Time_Value mi, ma, me, va;
  Miro::timeC2A(_rhs.min, mi);
  Miro::timeC2A(_rhs.max, ma);
  Miro::timeC2A(_rhs.mean, me);
  Miro::timeC2A(_rhs.var, va);

  _ostr << in << "mean: " << me;

  return _ostr;
}

std::ostream& operator << (std::ostream& _ostr, const Miro::FilterTreeIDL& _rhs)
{
  std::string in;
  for (unsigned int i = 0; i < indent; ++i)
    in += " ";

  _ostr << in << "name: " << _rhs.name << _rhs.timeFilter << std::endl;

  indent += 2;
  for (unsigned int i = 0; i < _rhs.successors.length(); ++i)
    _ostr << _rhs.successors[i];
  indent -= 2;

  return _ostr;
}

void connectToFilters(Miro::FilterTreeIDL const& _tree, 
		      Miro::ConnectionSetIDL& _connections)
{
  if (!CORBA::is_nil(_tree.videoInterface.ior )) {
    CORBA::ULong index = _connections.length();
    _connections.length(index + 1);
    _tree.videoInterface.ior->connect(_connections[index].id);
    _connections[index].filter = CORBA::string_dup(_tree.videoInterface.name);
  }

  for (CORBA::ULong i = 0; i < _tree.successors.length(); ++i) {
    connectToFilters(_tree.successors[i], _connections);
  }
}

void disconnectFromFilters(Miro::Client& _client, 
			   Miro::ConnectionSetIDL& _connections)
{
  for (CORBA::ULong i = 0; i < _connections.length(); ++i) {
    Miro::Video_var video = _client.resolveName<Miro::Video>(_connections[i].filter);
    video->disconnect(_connections[i].id);
  }
}

int main (int argc, char * argv[])
{
  Miro::Client client(argc, argv);
  try {
    std::string videoBroker("VideoBroker");
    if (argc > 1) {
      videoBroker = argv[argc-1];
      std::cout << "using video broker name: " << videoBroker << endl;
    }
    Miro::VideoBroker_var broker = client.resolveName<Miro::VideoBroker>(videoBroker.c_str());
    
    Miro::FilterTreeIDL_var stats = broker->filterTreeStats();
    std::cout << "Filter tree status: " << std::endl
	      << stats.in() << std::endl;
  }
  catch (const CORBA::Exception& e) {
    std::cerr << "CORBA exception occured: " << std::endl
	      << e << std::endl;
    return 1;
  }

  return 0;
}
