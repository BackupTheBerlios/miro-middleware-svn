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
#include "CanonMessage.h"
//#include "miro/Synch.h"

#include <iostream>

namespace Canon
{

  const char HEX_VALUES[]="0123456789ABCDEF";

  std::ostream& operator<< (std::ostream& ostr, const Message& rhs)
  {
    ostr.setf(ios::hex, ios::basefield);
    ostr << "command=";
    ostr.width(2);
    ostr.fill('0');
    ostr << rhs.command() << "  msg= 0x ";
    for (int i = 0; i < rhs.length(); ++i) {
      //      ostr.width(2);
      //      ostr.fill('0');
      //      ostr << (int)(rhs.buffer()[i]) << " ";
      char tmp[3];
      ostr << int2str(tmp,rhs.buffer()[i],2) << " ";
    }
    ostr.setf(ios::dec, ios::basefield);

    return ostr;
  }
  std::ostream& operator<< (std::ostream& ostr, const Answer& rhs)
  {
    ostr << "answer= 0x ";
    for (int i = 0; i < rhs.length(); ++i) {

      char tmp[3];
      ostr << int2str(tmp,rhs.buffer()[i],2) << " ";
    }

    return ostr;
  }

  char * int2str(char * buffer, int num, int digits)
  {
    buffer[digits--]='\0';
    for (; digits>=0; digits--) {
      buffer[digits]=HEX_VALUES[num%16]; //convert to Hex ASCII
      num/=16;
    }

    return buffer;
  }

  char * int2str(char * buffer, int num1, int num2, int digits)
  {
    buffer[digits*2]='\0';
    for (int i=digits-1; i>=0; i--) {
      buffer[i]=HEX_VALUES[num1%16]; //convert to Hex ASCII
      num1/=16;
    }
    for (int i=2*digits-1; i>=digits; i--) {
      buffer[i]=HEX_VALUES[num2%16]; //convert to Hex ASCII
      num2/=16;
    }
    return buffer;
  }

  int str2int(const char * buffer, int digits) 
  {
    int result=0;
    
    for (int i=0; i<digits; i++) {
      switch (buffer[i]) {
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9': 
	{
	  result*=16;
	  result+=buffer[i]-'0';
	  break;
	}
      case 'a':
      case 'A':
	{
	  result*=16;
	  result+=10;
	  break;
	}
      case 'b':
      case 'B':
	{
	  result*=16;
	  result+=11;
	  break;
	}
      case 'c':
      case 'C':
	{
	  result*=16;
	  result+=12;
	  break;
	}
      case 'd':
      case 'D':
	{
	  result*=16;
	  result+=13;
	  break;
	}
      case 'e':
      case 'E':
	{
	  result*=16;
	  result+=14;
	  break;
	}
      case 'f':
      case 'F':
	{
	  result*=16;
	  result+=15;
	  break;
	}
      }
    }
    return result;
  }

  void str2int(const char * buffer, int &num1, int &num2, int digits) {
    num1=str2int(buffer,digits);
    num2=str2int(buffer+digits,digits);
  }

  Message::Message(unsigned short cmd, unsigned char byte1, unsigned char byte2, unsigned char byte3)
  {
    int i=0;
    header(COMMAND_HEADER);
    deviceNum(SET_ALL_CAMERAS);
    command(cmd);
    if (byte1!=0) parameter()[i++] = byte1;
    if (byte2!=0) parameter()[i++] = byte2;
    if (byte3!=0) parameter()[i++] = byte3;
    parameter()[i++] = END_MARK;
    length_=5+i;
    parameter()[i]='\0'; //transform to string
  }

  Message::Message(unsigned short cmd, char* str)
  {
    header(COMMAND_HEADER);
    deviceNum(SET_ALL_CAMERAS);
    command(cmd);
    strcpy((char*)parameter(),str);
    parameter()[strlen(str)] = END_MARK;
    length_=6+strlen(str);
    parameter()[length_+1]='\0';
  }

};
