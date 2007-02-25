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
    std::string videoBroker("VideoBroker");
    if (argc > 1) {
      videoBroker = argv[argc-1];
      std::cout << "using video broker name: " << videoBroker << endl;
    }
    Miro::VideoBroker_var broker = client.resolveName<Miro::VideoBroker>(videoBroker.c_str());
    
    char c;
    std::string in;
    do {
      std::cout << "Possible Commands:" << std::endl << std::endl
		<< " a - Acquire (and release) images from" << std::endl
		<< "     all video interfaces." << std::endl
		<< " s - Print filter tree status." << std:: endl
		<< " q - Quit" << std::endl 
		<< std::endl;
      
      
      while (std::cin >> c) {
      switch (c) {
      case 'a': {
	Miro::FilterTreeIDL_var stats = broker->filterTreeStats();
	Miro::ConnectionSetIDL connections;
	connectToFilters(stats.in(), connections);
	Miro::BufferSetIDL_var buffers = new Miro::BufferSetIDL;
	Miro::TimeIDL tC = broker->acquireNextImageSet(connections, buffers);
	ACE_Time_Value tA;
	Miro::timeC2A(tC, tA);
	std::cout << "Image time: " << tA << std::endl;
	broker->releaseImageSet(connections, buffers.in());
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

      if (c == 'q')
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
