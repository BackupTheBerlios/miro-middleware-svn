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

#include <unistd.h>
#include <cstdio>
#include <iostream>
#include <sys/signal.h>
#include <sys/types.h>
#include <sys/time.h>
#include <wait.h>
#include <string>
#include <fcntl.h>

#include "Ssh.h"

using std::cout;
using std::cerr;
using std::endl;
using std::flush;
using std::string;

void killZombies(int uselessParameter) 
  // the useless parameter is required on some compilers
{

  union wait status;

  while (wait3(&status, WNOHANG, NULL) >0); 

  uselessParameter++; // useless line; to avoid parameter not used warning

}//killZombies




SSH::SSH(int in, int err):
  pipeID(0),
  pidSSH(0),
  opened(false),
  machine("")
{
  signal(SIGCHLD, killZombies);

  oldStdin=dup(0);
  oldStdout=dup(1);
  oldStderr=dup(2);

  if (in==0) {
    int pipe1[2];
    
    pipe(pipe1);

    input=pipe1[0];
    inputIn=pipe1[1];
  } else {
    inputIn=in;
    input=in;
  }

  if (error==0) {
    int pipe1[2];
    
    pipe(pipe1);

    error=pipe1[0];
    errorIn=pipe1[1];
  } else {
    error=err;
    errorIn=err;
  }
  char host[256];
  if (!gethostname(host,255)) {
    localMachine=""+string(host); //""+ to force copying and not referencing
  } else {
    localMachine=string("");
  }
}

SSH::~SSH()
{
  if (opened) close();

  ::close(0);
  dup(oldStdin);
  ::close(1);
  dup(oldStdout);
  ::close(2);
  dup(oldStderr);

  killZombies();
}

//IMPORTANT: Don't forget to close the conections before ending the program!
//Otherwise it will hang up waiting for the connection to end
void SSH::open(std::string remoteMachine)
{
  string SSHVersion="SSH";

  //if it's already open, exit
  if ((machine==remoteMachine)&&(opened)) return;

  if (remoteMachine=="") 
  {
    close();
    return;
  }

  //if a connection is already open to anohter machine, close it.
  if (opened) close();
  
  machine=remoteMachine;

  int pipeSSH[2];
  pipe(pipeSSH);

  int stdo=inputIn;
  int stde=errorIn;


  if (remoteMachine!=localMachine) {
    //get ssh version
    int pipeVersion[2];
    pipe(pipeVersion);
    if (!fork()) {
      ::close(2);
      dup(pipeVersion[1]);
      
      execlp("ssh","ssh","-V",NULL);
      perror("could not get SSH version");
      _exit(0);
    } else {
      int flags=fcntl(pipeVersion[0],F_GETFL);
      fcntl(pipeVersion[0],F_SETFL,flags | O_NONBLOCK);
      
      int cread=0;
      int timeInit=time(0);
      int timeEnd=2;
      char tmp[512];
      string result="";
      
      do {
	cread=read(pipeVersion[0],tmp,511);
	timeEnd=timeInit+2-time(0);
      } while ((cread<0)&&(timeEnd>0));
      fcntl(pipeVersion[0],F_SETFL,flags);
      if (cread>=0) tmp[cread]='\0';
      else {
	tmp[0]='\0';
	perror("read error:");
      }
      result=tmp;
      if ((int)result.find("OpenSSH")>=0) {
	SSHVersion="OpenSSH";
      }
    }
  } else {
    SSHVersion="OpenSSH"; //do not ask for password
  }
    

  //open ssh connection
  pidSSH=fork();
  if (pidSSH<0) 
  {
    pidSSH=0;
    cerr << "No child process" << endl;
    throw("Could not create child process");
  }

  if (!pidSSH) //child
  {
    if (stdo!=1) 
    { 
      ::close(1);
      if (stdo)
      {
	dup(stdo);
      }
    }
    if (stde!=2) 
    { 
      ::close(2);
      if (stde)
      {
	dup(stde);
      }
    }

    ::close(0);
    dup(pipeSSH[0]);

    if (localMachine==remoteMachine) { //use local shell instead of ssh
      execlp("bash","bash", "-s", NULL);
    } else {
      execlp("ssh","ssh", remoteMachine.c_str(),NULL);
    }
    ::close(2);
    ::open("/dev/console",O_WRONLY);
    cerr << "could not exec ssh" << endl << flush;
    _exit(-1);

  } else {
    if (SSHVersion!="OpenSSH")
    {
      string userName=getenv("USER");
      cout << "Please, enter " << userName << "@" << remoteMachine << "'s password:" << flush;
    }
    
  }  
  sleep(1);
  pipeID=pipeSSH[1];
  opened=true;

}//openSSH

void SSH::close()
{
  if ((opened)&&(pipeID>0)) //to close the local shells
    write(pipeID,"exit\n",5);
  opened=false;
  machine=string("");
  if (pidSSH>0)  ::kill(pidSSH,15);
  sleep(1);
  pidSSH=0;
  killZombies();
} //closeSSH

void SSH::execSilent(std::string command)
{
  if (pipeID==0) opened=false;
  if (!opened) throw("The SSH is not opened");

  command+="\n";

  write(pipeID,command.c_str(),command.length());
}

//the ssh needs to be opened with stdout redirected to input
string SSH::exec(std::string command, int timeout)
{
  string result="";

  char tmp[512];
  tmp[0]='\0';
  

  execSilent(command);
  if (input<=2) throw("The output is not redirected; cannot return the value");

  int flags=fcntl(input,F_GETFL);

  fcntl(input,F_SETFL,flags | O_NONBLOCK);

  int cread=0;
  int timeInit=time(0);
  int timeEnd=timeout;

  do 
  {
    cread=read(input,tmp,511);
    timeEnd=timeInit+timeout-time(0);
  } while ((cread<0)&&(timeEnd>0));
  tmp[cread]='\0';
  result+=tmp;
  fcntl(input,F_SETFL,flags);

  return(result);
}


//the ssh needs to be opened with stdout redirected to input
bool SSH::ping(int timeout)
{

  if (!opened) return(false);

  string execResult=exec("echo alive",timeout);

  bool result=(execResult=="alive\n")||(execResult=="alive");

  if (execResult=="") close();

  return (result);
}


// Returns the pid of the first occurrence of the corresponding process
// or 0 if it is not running
int SSH::isRunning(std::string command)
{
  if (!opened) return 0;

  int result=0;

  char tmp[256];
  tmp[0]='\0';

  if (input<=2) throw("The output is not redirected; cannot return the value");
  execSilent("ps -eo command,pid");


  bool endOfList=false;
  int timeout=1;
  string list="";

  int flags=fcntl(input,F_GETFL);

  fcntl(input,F_SETFL,flags | O_NONBLOCK);

  while (!endOfList)
  {
    int cread=0;
    int timeInit=time(0);
    int timeEnd=timeout;
  
    do 
    {
      cread=read(input,tmp,255);
      timeEnd=timeInit+timeout-time(0);
    } while ((cread<0)&&(timeEnd>0));
    if (cread>=0) tmp[cread]='\0';
    else tmp[0]='\0';
    list+=tmp;
    endOfList=(cread<=0);
  }

  fcntl(input,F_SETFL,flags);
  
  if (list=="") return(0);

  char pname[256];

  while (list.length()>0)
  {
    sscanf(list.c_str(),"%s ",pname);

    string processName=pname;
    list.erase(0,list.find(pname)+strlen(pname)+1); //delete the read part

    int pos=processName.rfind("/");
    if (pos>=0) processName.erase(0,pos);
    pos=command.rfind("/");
    if (pos>=0) command.erase(0,pos);
    pos=processName.find(" ");
    if (pos>0) processName.erase(pos,processName.length());
    pos=command.find(" ");
    if (pos>0) command.erase(pos,command.length());

    if ((int)processName.find(command)>=0)
    {
      while ((sscanf(list.c_str(),"%i",&result)!=1)&&(list.length()>0))
      {
	unsigned int pos=list.find(" ")+1;
	if (pos>list.length()) pos--;
	list.erase(0,pos); //remove the parameters if they are not numbers
      }
      break;
    }
  }

  return(result);

}

void SSH::kill(std::string command,int timeout)
{
  int pid=0;
  int pidOld=0;

  if (!opened) return;

  int timeIn=time(0);

  do 
  {
    char tmp[10];

    pidOld=pid;
    pid=isRunning(command);
    sprintf(tmp,"%i",pid);
    if (time(0)-timeIn<timeout) pidOld=pid+1; //wait timeout secs before killing with -9
    if (pid!=0) execSilent(string("killall")+((pid==pidOld)?" -9 ":" ")+command); else break;
    sleep(1);
    if (pid==pidOld) break; //if the kill -9 didn't work, the process cannot be killed
  } while (pid!=0);
}

void SSH::kill(int pid)
{
  char tmp[10];

  sprintf(tmp,"%i",pid);
  if (pid!=0) execSilent(string("kill ")+tmp);
}

