// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// for details copyright, usage and credits to other groups see Miro/COPYRIGHT
// for documentation see Miro/doc
// 
// (c) 1999,2000
// Department of Neural Information Processing, University of Ulm, Germany
//
// Authors: 
//   Stefan Enderle, 
//   Guillem Pages,
//   Stefan Sablatnoeg, 
//   Hans Utz
// 
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
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


void killZombies(int=0);

class SSH
{
public:

  SSH(int=1,int=2);

  ~SSH();

  //IMPORTANT: Don't forget to close the conections before ending the program!
  //Otherwise it will hang up waiting for the connection to end
  void open(std::string);
  
  void close();
  
  void execSilent(std::string);
  
  //the ssh needs to be opened with stdout redirected to input
  std::string exec(std::string, int=30);
  
  //the ssh needs to be opened with stdout redirected to input
  bool ping(int=30);

  int isRunning(std::string);

  void kill(std::string,int=5);
  void kill(int);

  inline std::string getMachine() {return machine;};
  inline std::string getLocalMachine() {return localMachine;};
  inline bool isOpen() {return opened;};

private:
  int pipeID;
  int inputIn, input;
  int errorIn,error;
  int oldStdin, oldStdout, oldStderr;
  int pidSSH;

  bool opened;

  std::string machine;
  std::string localMachine;
  
};

#endif
