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
//
// Authors:
//   Hans Utz
//   Stefan Enderle
//   Stefan Sablatnoeg
//
#ifndef SSH_HH
#define SSH_HH

#include <unistd.h>
#include <cstdio>
#include <iostream>
#include <sys/signal.h>
#include <sys/types.h>
#include <sys/time.h>
#include <wait.h>
#include <string>


void killZombies(int = 0);

class SSH
{
public:

  SSH(int = 1, int = 2);

  ~SSH();

  //IMPORTANT: Don't forget to close the conections before ending the program!
  //Otherwise it will hang up waiting for the connection to end
  void open(std::string);

  void close();

  void execSilent(std::string);

  //the ssh needs to be opened with stdout redirected to input
  std::string exec(std::string, int = 30);

  //the ssh needs to be opened with stdout redirected to input
  bool ping(int = 30);

  int isRunning(std::string);

  void kill(std::string, int = 5);
  void kill(int);

  inline std::string getMachine() {
    return machine;
  };
  inline std::string getLocalMachine() {
    return localMachine;
  };
  inline bool isOpen() {
    return opened;
  };

private:
  int pipeID;
  int inputIn, input;
  int errorIn, error;
  int oldStdin, oldStdout, oldStderr;
  int pidSSH;

  bool opened;

  std::string machine;
  std::string localMachine;

};

#endif
