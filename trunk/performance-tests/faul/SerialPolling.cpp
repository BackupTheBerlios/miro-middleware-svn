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

#include <ace/DEV_Connector.h>
#include <ace/TTY_IO.h>

#include <iostream>

ACE_TTY_IO ioBuffer;


ACE_TTY_IO::Serial_Params oldParams;

int binInit()
{
  unsigned char value1;
  unsigned char value2;
  unsigned char value3;
  unsigned char value4;
  value1 = 200; //Modus1
  value2 = 200; //Istlage
  value3 = 202; //Modus2
  value4 = 255; //kein zweiter wert
   if (ioBuffer.send_n(&value1, 1) < 0) {
    std::cerr << "write error" << std::endl;
    return 1;
   }
    if (ioBuffer.send_n(&value2, 1) < 0) {
    std::cerr << "write error" << std::endl;
    return 1;
   }
    if (ioBuffer.send_n(&value3, 1) < 0) {
    std::cerr << "write error" << std::endl;
    return 1;
   }

    if (ioBuffer.send_n(&value4, 1) < 0) {
    std::cerr << "write error" << std::endl;
    return 1;
   }
   return 0;
}

int testParcours()
{
  // add your read and write code here
  
  //char * message = "test";
  unsigned char value;
  value = 201;
  if (ioBuffer.send_n(&value, 1) < 0) {
    std::cerr << "write error" << std::endl;
    return 1;
  }

  const int BUFFER_SIZE = 256;
  char buffer[BUFFER_SIZE];
  if (ioBuffer.recv_n(buffer, BUFFER_SIZE) < 0) {
    std::cerr << "read error" << std::endl;

    return 1;
  }
  std::cout << buffer << endl;

  return 0;
}

int saveTtyParams()
{
  // save the old tty configuration
  ACE_TTY_IO::Serial_Params oldParams;
  return ioBuffer.control(ACE_TTY_IO::GETPARAMS, &oldParams);
}

int setTtyParams()
{
  ACE_TTY_IO::Serial_Params ttyParams;

  ttyParams.baudrate = 19220;
  ttyParams.parityenb = 0;
  ttyParams.paritymode = 0;
  ttyParams.databits = 8;
  ttyParams.stopbits = 1; 
  ttyParams.readtimeoutmsec = 0;
  ttyParams.modem = 0;
  ttyParams.rcvenb = 1;
  ttyParams.ctsenb = 0;
  ttyParams.rtsenb = 0;
  ttyParams.xinenb = 0;
  ttyParams.xoutenb = 0;


  // let's initialize the TTY 
  return ioBuffer.control(ACE_TTY_IO::SETPARAMS, &ttyParams);
}

int restoreTtyParams()
{
  ACE_TTY_IO::Serial_Params oldParams;
  return ioBuffer.control(ACE_TTY_IO::SETPARAMS, &oldParams);
}

int main (int argc, char * argv[])
{
  int rc = 0;
  char defaultName[] = "/dev/tty/s0";
  char const * name = defaultName;

  if (argc > 1)
    name = argv[1];

  ACE_DEV_Addr ttyName(name);
  ACE_DEV_Connector connector;

  if (connector.connect(ioBuffer, 
			ttyName,
			0, ACE_Addr::sap_any, 0, O_RDWR) == -1) {
    std::cerr << "Failed to open device: " << name << std::endl;
    return 1;
  }
  rc = binInit();
 // if ((rc = saveTtyParams()) == 0 &&
 //     (rc = setTtyParams()) == 0) {

#if defined (ACE_HAS_TERM_IOCTLS)
#if defined(TCGETS)
    struct termios terminfo;
    ioBuffer.ACE_IO_SAP::control(TCGETS, (void *) &terminfo);
#elif defined(TCGETA)
    struct termio terminfo;
    ioBuffer.ACE_IO_SAP::control(TCGETA, (void *) &terminfo);
#endif
    
#if defined(TCGETS) || defined(TCGETA)
    std::cout << "new tty status:" << std::endl << std::hex
	      << "c_iflag = 0x" << terminfo.c_iflag << std::endl
	      << "c_oflag = 0x" << terminfo.c_oflag << std::endl
	      << "c_cflag = 0x" << terminfo.c_cflag << std::endl
	      << "c_lflag = 0x" << terminfo.c_lflag << std::endl
	      << "c_cc[4/5] = 0x" << (int)terminfo.c_cc[VTIME] 
	      << " " << (int)terminfo.c_cc[VMIN] 
	      << std::dec << std::endl;
#endif 
    
#endif // ACE_HAS_TERM_IOCTLS

    for (int i=0;i<100;i++)
    {
      rc = testParcours();
      std::cout<< i <<": "<< endl;
    }

   // restoreTtyParams();


  ioBuffer.close();
  return rc;
}


