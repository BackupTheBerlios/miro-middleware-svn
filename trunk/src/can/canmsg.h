#ifndef CANMSG_H
#define CANMSG_H
// -*- c++ -*-
//
// canmsg.h 
//
// most code contributed by Michael Wallner and Andre Mueller, ported for this hardware by
// Stefan Sablatnoeg
//
// (C) 1999 Stefan Sablatnoeg
// 

typedef unsigned long CanId;

#define NUM_MSG		14			/* number of message objects            */
#define XTID		0x80000000L		/* extended identifier 			*/
#define XTID_MASK	0x1fffffffL		/* extended identifier mask (29 bits)	*/
#define STID_MASK	0x000007ffL		/* standard identifier mask (11 bits)	*/

/* can message */
typedef struct 
{
  CanId id;
  int rtr;					/* remote transmission	*/
  int len;					/* data length 0..8 	*/
  unsigned char d[8];
  unsigned long timestamp;
} canmsg;

/* controller configuration structure */
typedef struct
{
  unsigned char TSEG1,TSEG2,SJW,BRP;	        /* baudrate configuration */
  int in_invert,out_invert;			/* bus configuration */
  CanId gmask;					/* global mask */
  CanId mask15;					/* mask 15 */
  CanId id15;					/* identifier for message object 15 */
} CanConfig;

/* message mask for receving objects */
typedef struct {
  int transmit,			                /* message object for transmitting	*/
    rtr;				        /* message object for remote transmission	*/
  CanId id;				        /* can identifier	  		*/
} MsgConf;

/* can status */
typedef struct {	
  unsigned long st_intr;		        /* number of interrupts */
	
  unsigned long st_rx;	                        /* rx ready			*/
  unsigned long st_tx;		                /* tx ready			*/

  /* controller errors */
  unsigned long st_busoff;
  unsigned long st_warn;
  unsigned long st_wakeup;

  unsigned long st_stuff;
  unsigned long st_form;
  unsigned long st_ack;
  unsigned long st_bit1;
  unsigned long st_bit0;
  unsigned long st_crc;

  unsigned long st_msglost;

  /* ringbuffers */
  unsigned long st_rxoverrun;
} CanStatus;

#endif
