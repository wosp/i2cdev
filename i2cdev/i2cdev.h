/* 
* Author :  W.Spiess
* Date   :  30012018
* Version:  1.0
* File   :  i2cdev.h
* ------------
*
*
*/


#pragma once
#ifndef I2CDEV_H_
#define I2CDEV_H_

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"    // Serial print and other stuff
#else
  #include "WProgram.h"
#endif

#include <wire.h>

/************** Settings for MCP23017     *******************
/************************************************************
MCP23017 supports 7bit Adressing A[0..7] (=128Byte)
S 0 1 0  0 A2 A1 A0 R/W ACK
  |               |
  +-slave address-+  A[0..2] user defined
                     A[3..7] fixed adress 0x20
Portnumbering depends on the I2C address used:
A0:	A1:	A2: 	Address: 	Port range:
0 	0 	0		0x20		1 - 16
1 	0 	0 		0x21 		17 - 32
0 	1 	0 		0x22 		33 - 48
1 	1 	0 		0x23 		49 - 64
0 	0 	1 		0x24 		65 - 80
1 	0 	1 		0x25 		81 - 96
0 	1 	1 		0x26 		97 - 112
1 	1 	1 		0x27 		113 - 128
**************************************************************/
// #define MPC16
#define MPC08
#define _4bit     // use a 4bit HD44780 interface
//#define _8bit   // use a 8bit HD44780 interface
#ifdef MPC16
#define GPA0  0
#define GPA1  1
#define GPA2  2
#define GPA3  3
#define GPA4  4
#define GPA5  5
#define GPA6  6
#define GPA7  7
#define GPB0  8
#define GPB1  9
#define GPB2 10
#define GPB3 11
#define GPB4 12
#define GPB5 13
#define GPB6 14
#define GPB7 15
#endif

#ifdef MPC08
#define GPA0  0
#define GPA1  1
#define GPA2  2
#define GPA3  3
#define GPA4  4  
#define GPA5  5
#define GPA6  6
#define GPA7  7
#define GPB0  0
#define GPB1  1
#define GPB2  2
#define GPB3  3
#define GPB4  4
#define GPB5  5
#define GPB6  6
#define GPB7  7
#endif

// 4-Bit Interface of  LCD HD44780  
#define EN   GPB5
#define RS   GPB7
#define RW   GPB6
#define DB0  GPB4
#define DB1  GPB3
#define DB2  GPB2
#define DB3  GPB1
#define DB4  GPB4
#define DB5  GPB3
#define DB6  GPB2
#define DB7  GPB1
#define RED  GPA6
#define GRE  GPA7
#define BLU  GPB0

//#define MCP23017 1
// MPC23017 registers
#define IODIRA		0x00
#define IPOLA		0x02
#define GPINTENA	0x04
#define DEFVALA		0x06
#define INTCONA		0x08
#define IOCONA		0x0A
#define GPPUA		0x0C
#define INTFA		0x0E
#define INTCAPA		0x10
#define GPIOA		0x12
#define OLATA		0x14


#define IODIRB		0x01
#define IPOLB		0x03
#define GPINTENB	0x05
#define DEFVALB		0x07
#define INTCONB		0x09
#define IOCONB		0x0B
#define GPPUB		0x0D
#define INTFB		0x0F
#define INTCAPB		0x11
#define GPIOB		0x13
#define OLATB		0x15

#define MCP23017_INT_ERR 255 

/************** Settings for HD44780     **************/
//  LCD DB4-DB7 look @ wiredP[] in this section
//  LCD RS      (RS: 1=Data, 0=Instruction)
#define LCD_RS        GPB7
//  LCD EN      (EN: 1-Impuls für Daten)
#define LCD_EN        GPB5
//  LCD EN      (R/W 1= Read  0= WriteEN: 1-Impuls für Daten)
#define LCD_RW        GPB6

// HD 44780 Registers 
#define LCD_DATA	1
#define LCD_CMD		0
#define LCD_RD		1
#define LCD_WR		0
 
// Clear Display -------------- 0b00000001
#define LCD_CLEAR_DISPLAY       0x01
 
// Cursor Home ---------------- 0b0000001x
#define LCD_CURSOR_HOME         0x02
 
// Set Entry Mode ------------- 0b000001xx
#define LCD_SET_ENTRY           0x04
 
#define LCD_ENTRY_DECREMENT     0x00
#define LCD_ENTRY_INCREMENT	    0x02
#define LCD_ENTRY_NOSCROLL      0x00
#define LCD_ENTRY_SCROLL        0x01
 
// Set Display ---------------- 0b00001xxx
#define LCD_SET_DISPLAY         0x08
 
#define LCD_DISPLAY_OFF         0x00
#define LCD_DISPLAY_ON          0x04
#define LCD_CURSOR_OFF          0x00
#define LCD_CURSOR_ON           0x02
#define LCD_BLINKING_OFF        0x00
#define LCD_BLINKING_ON         0x01
 
// Set Shift ------------------ 0b0001xxxx
#define LCD_SET_SHIFT           0x10
 
#define LCD_CURSOR_MOVE         0x00
#define LCD_DISPLAY_SHIFT       0x08
#define LCD_SHIFT_LEFT          0x00
#define LCD_SHIFT_RIGHT         0x04
 
// Set Function --------------- 0b001xxxxx
#define LCD_SET_FUNCTION        0x20
 
#define LCD_FUNCTION_4BIT       0x00
#define LCD_FUNCTION_8BIT       0x10
#define LCD_FUNCTION_1LINE      0x00
#define LCD_FUNCTION_2LINE      0x08
#define LCD_FUNCTION_5X8        0x00
#define LCD_FUNCTION_5X10       0x04
 
#define LCD_RESET_CMD           0x03
 
// Set CG RAM Address --------- 0b01xxxxxx  (Character Generator RAM)
#define LCD_SET_CGADR           0x40
 
#define LCD_GC_CHAR0            0
#define LCD_GC_CHAR1            1
#define LCD_GC_CHAR2            2
#define LCD_GC_CHAR3            3
#define LCD_GC_CHAR4            4
#define LCD_GC_CHAR5            5
#define LCD_GC_CHAR6            6
#define LCD_GC_CHAR7            7
 
// Set DD RAM Address --------- 0b1xxxxxxx  (Display Data RAM)
#define LCD_SET_DDADR           0x80
#define LCD_DDADR_LINE1         0x00 // Line 1: 0x00, 0x01, 0x02 .. 0x13
#define LCD_DDADR_LINE2         0x40 // Line 2: 0x40, 0x41, 0x42 .. 0x53
#define LCD_DDADR_LINE3         0x10 // Line 3: 0x14, 0x15, 0x16 .. 0x27
#define LCD_DDADR_LINE4         0x50 // Line 4: 0x54, 0x55, 0x56 .. 0x67

#define LCD_RED		GPA6
#define LCD_GREEN	GPA7
#define LCD_BLUE    GPB0

/*********** User settings *************************************************
The library is made for a 4-Bit data interface of HD44780 
MCP23017    HD44780
------+      +-------
 GPB1 +------+ DB7           The wiring between I2C MCP23017 and LCD HD44780      
 GPB2 +------+ DB6           needs a special wrapper. You will find this as
 GPB3 +------+ DB5           a table called <<<wiredP[]>> in the following section
 GPB4 +------+ DB4      
 GPB5 +------+ ENABLE
 GPB6 +------+ RS   
 GPB7 +------+ R/W   
      |      |
------+      +-------
************************************************************/
#define LCD_ROWSIZE 2    // changes for other display types made here
#define LCD_COLSIZE 16   //               ""

class i2cdev {
private:
// changes of other wiring configuration between an HD44780 and a MCP23017 made in
// the followung table. This table maps the wiring between MCP23018 and HD44780 LCD 
// using 4Bit Interface. 
//         MCU databits:          D0    D1    D2    D3    D4    D5    D6    D7
	unsigned char wiredP[8] = { GPB4, GPB3, GPB2, GPB1, GPB4, GPB3, GPB2, GPB1 };
	unsigned char DevAdr;					// device address of the I2C device

    unsigned shadowOLATA;		// written in i2cdev::wr()
    unsigned shadowOLATB;       // written in i2cdev::wr()

public:
	i2cdev(void);							// not used with arduino's c++compiler
	~i2cdev(void);							//					"
	unsigned char begin(unsigned char);		// workaround with arduino, used as a kind of constructor
	unsigned char detect(void);				// automatically detection of I2C Devices
    unsigned char LCDbegin(void);			// initialize a HD44780 display: 4bit Mode (Adafruit's RGB_LCD)
    void LCDdel(void);						// delete Display, setting Cursor to row=0 col=0
    char LCDprint(char*);					// print a string to current cursor position
    void LCDbacklite(unsigned char);
    char LCDcursor(unsigned char,
                   unsigned char);			// write 4 data bits to HD44780
	void setBit(unsigned char, 
				unsigned char,
				unsigned char);
  	unsigned char wr(unsigned char,
                     unsigned char);		// write 8 Bits to a MCP23017 register
  	unsigned char wr_1(unsigned char,
                     unsigned char);		// write 8 Bits to a MCP23017 register
	unsigned char rd(unsigned char);		// read 8 bits from a MCP23017 register
    
private:	
	unsigned char map(unsigned char );		// small wrapper for Adafruits wiring MCP23017 to HD44780
	unsigned char remap(unsigned char );	// small re-wrapper for Adafruits wiring MCP23017 to HD44780
	unsigned char mapMask(void);			// set mask bits of HD44780 databus (still not used) 

	unsigned char LCDdir(unsigned char);	// set direction for read/write HD44780 via MCP23017
    unsigned char LCDin4(unsigned char);	// read 4 databits from LCD
    unsigned char LCDin8(unsigned char);	// read Hi-/Lo-Nibble from HD44780 
    unsigned char LCDout4(unsigned char,
                          unsigned char);	// write 4 data bits to HD44780
	unsigned char LCDout8(unsigned char,
                          unsigned char);	// write Hi-/Lo-Nibble to HD44780 

};

#endif
