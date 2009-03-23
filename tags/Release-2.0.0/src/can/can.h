/****************************************************************/
/* 			Definitions for the SJA1000-CANbus driver	V 1.4 beta    */
/*			PeliCAN mode is supported														    */
/*			Cristiano Brudna  2000, 2001												    */
/****************************************************************/

#include <linux/ioctl.h>

typedef struct {
	unsigned char bt0;
	unsigned char bt1;
} canconfig;

/* Paralel port */
#define BASE_ADDR 0x0278        /* taken from the BIOS setup */ 
#define NUMBER_OF_PORTS 8
#define IRQ 7                   /* taken from the BIOS setup */
#define PP_DATA (BASE_ADDR)
#define PP_STATUS (BASE_ADDR+1)
#define PP_CONTROL (BASE_ADDR+2)
#define EPP_ADDR (BASE_ADDR+3)
#define EPP_DATA (BASE_ADDR+4)

/* Standard operation mode to the (MODE register) */
#define STANDARD_MODE 0x08

/* Operations of the COMMAND register */
#define TRANSMISSION_REQUEST 	0x01
#define ABORT_TRANSMISSION 	0x02
#define RELEASE_RECEIVE_BUFFER 	0X04
#define CLEAR_DATA_OVERRUN 	0x08
#define SLEEP 			0x10

/* State of the controller */
#define ACTIVE 0
#define PASSIVE 1

/* Operations of the ioctl() comand */
#define CAN_MAJOR 120
#define CAN_IOC_MAGIC   'k'
#define CAN_IOCSBAUD	  _IOW(CAN_IOC_MAGIC, 1, 8)  /* set baud rate                    */
#define CAN_IOCSAMASK	  _IOW(CAN_IOC_MAGIC, 2, 8)  /* set acceptance mask              */
#define CAN_IOCSACODE	  _IOW(CAN_IOC_MAGIC, 3, 8)  /* set acceptance code              */
#define CAN_IOCCRBUF    _IOW(CAN_IOC_MAGIC, 4, 8)  /* clear read buffer                */
#define CAN_IOCCWBUF    _IOW(CAN_IOC_MAGIC, 5, 8)  /* clear write buffer               */
#define CAN_IOCRREG     _IOR(CAN_IOC_MAGIC, 6, 8)  /* read register                    */
#define CAN_IOCRTTS     _IOR(CAN_IOC_MAGIC, 7, 8)  /* read last transmit timestamp     */
#define CAN_IOCSACTIVE  _IOW(CAN_IOC_MAGIC, 8, 8)  /* set active mode                  */
#define CAN_IOCSPASSIVE _IOW(CAN_IOC_MAGIC, 9, 8)  /* set passive mode                 */
#define CAN_IOCRAPS     _IOR(CAN_IOC_MAGIC, 10, 8) /* read active/passive status       */
#define CAN_IOCSBTR	    _IOW(CAN_IOC_MAGIC, 11, 8)  /* set bit timing registers 0 and 1 */

#define CAN_IOC_MAXNR 11


/* Types of frames */
#define STANDARD 0
#define EXTENDED 1

/* Definition of possible baudrates for the Can-Bus 	*/
/* Using defines for easy reading			*/
#define B1000	0  /* 1 MBit/s   */
#define B500	1  /* 500 Kbit/s */
#define B250	2  /* 250 Kbit/s */
#define B125	3  /* 125 Kbit/s */
#define B20   4  /* 20 Kbit/s  */

#define BUFFER_SIZE 150
#define MSG_LENGTH sizeof(canmsg)
#define RB_FULL_COUNTER_LIMIT 20

typedef struct CAN_Dev {
	int baud_rate; 						     /* baud rate of the bus            */
	int frame_mode;						     /* normal (0) or extended (1) mode */
	int active_passive_status;     /* current state of the controller */
	int count;                     /* user application counter        */

	int rb_full_counter;           /* read_buffer full counter (protection against too many interrupts */

	canmsg *read_buf;
	int nm_rb;						          /* number of messages in the read buffer */
	int rp_rb;						          /* read position of the read buffer*/	
	int wp_rb;						          /* write position of the read buffer*/

	canmsg *write_buf;
	int nm_wb;                     	/* number of messages in the write buffer */
	int wp_wb;                     	/* write position of the write buffer*/
	int rp_wb;                     	/* read position of the write buffer*/
}CAN_Dev;

typedef struct {
	int baudnr;
	unsigned char bt0;
	unsigned char bt1;		
}bus_param;

enum can_registers{
	MODE			        = 0,
	COMMAND         	= 1,
	STATUS          	= 2,
	INTERRUPT       	= 3,
	INTERRUPT_ENABLE 	= 4,
		
	BUS_TIMING_0    	= 6,
	BUS_TIMING_1    	= 7,
	OUTPUT_CONTROL  	= 8,
	TEST            	= 9,

	ARBITRATION_LOST_CAPTURE = 11,
	ERROR_CODE_CAPTURE 	     = 12,
	ERROR_WARNING_LIMIT 	   = 13,
	RX_ERROR_COUNTER 	       = 14,
	TX_ERROR_COUNTER 	       = 15,
	FRAME_INFORMATION 	     = 16,
	IDENTIFIER 		           = 17,
	
	ACCEPTANCE_MASK		       = 20,
	DATA 			               = 21,
	
	RX_MESSAGE_COUNTER 	     = 29,
	
	CLOCK_DIVIDER   	       = 31,
} can_registers;
