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

using Miro::operator <<;

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

  _ostr << in << "min: " << mi << std::endl
	<< in << "max: " << ma << std::endl
	<< in << "mean: " << me << std::endl
	<< in << "var: " << va;

  return _ostr;
}

std::ostream& operator << (std::ostream& _ostr, const Miro::FilterTreeIDL& _rhs)
{
  std::string in;
  for (unsigned int i = 0; i < indent; ++i)
    in += " ";

  _ostr << in << "name: " << _rhs.name << std::endl
	<< in << "connections: " <<_rhs.connections << std::endl
	<< in << "filter processing time:" << std::endl;
  indent += 1;
  _ostr << _rhs.timeFilter << std::endl;
  indent -= 1;
  _ostr << in << "subtree processing time:" << std::endl;
  indent += 1;
  _ostr << _rhs.timeSubtree << std::endl;
  indent -= 1;
  _ostr << in << "instance: " << _rhs.videoInterface.ior  << std::endl;
  if (!CORBA::is_nil(_rhs.videoInterface.ior)) {
    _ostr << in << " name: " << _rhs.videoInterface.name << std::endl
	  << in << " connections: " << _rhs.videoInterface.connections << std::endl;
  }
  _ostr << in << "successor links:" << endl;
  for (unsigned int i = 0; i < _rhs.successorLinks.length(); ++i)
    _ostr << in << " " << _rhs.successorLinks[i] << std::endl;

  _ostr << in << "successor filters:" << endl;
  indent += 2;
  for (unsigned int i = 0; i < _rhs.successors.length(); ++i)
    _ostr << _rhs.successors[i] << std::endl;
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
    Miro::VideoBroker_var broker = client.resolveName<Miro::VideoBroker>("VideoBroker");
    
    char c;
    std::string in;
    do {
      std::cout << "Possible Commands:" << std::endl << std::endl
		<< " a - Acquire (and release) images from" << std::endl
		<< "     all video interfaces." << std::endl
		<< " s - Print filter tree status." << std:: endl
		<< " q - Quit" << std::endl 
		<< std::endl;
      
      std::cin >> in;
      c = in[0];
      
      switch (c) {
      case 'a': {
	Miro::FilterTreeIDL_var stats = broker->filterTreeStats();
	Miro::ConnectionSetIDL connections;
	connectToFilters(stats, connections);
	Miro::BufferSetIDL_var buffers = new Miro::BufferSetIDL;
	broker->acquireNextImageSet(connections, buffers);
	broker->releaseImageSet(connections, buffers);
	disconnectFromFilters(client, connections);
	break;
      }
      case 's': {
	Miro::FilterTreeIDL_var stats = broker->filterTreeStats();
	std::cout << "Filter tree status: " << std::endl
		  << stats.in() << std::endl;
	break;
      }
      default:
	break;
      }
    }
    while(c != 'q');
  }
  catch (const CORBA::Exception& e) {
    std::cerr << "CORBA exception occured: " << std::endl
	      << e << std::endl;
    return 1;
  }

  return 0;
}
