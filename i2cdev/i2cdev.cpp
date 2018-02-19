/* 
* Author :  W.Spiess
* Date   :  30012018
* Version:  1.0
* File   :  i2cdev.cpp
* ------------
*
*
*/
#include "i2cdev.h"

// Standard Constructor
i2cdev::i2cdev(void) {
// constructors will not work with arduino. 
// Call i2cdev::begin() after creation of an object of this type.
}

unsigned char i2cdev::LCDdir(unsigned char mode) {
	Wire.beginTransmission(DevAdr);
	Wire.write(0x01); // IODIRB register
	if (mode== LCD_RD) Wire.write(0b00011110); // Set 4-Bits as input
	else Wire.write(0x00); // set all of port B to outputs
	Wire.endTransmission();
return 0;
}

// Poor man's constructor in Arduino sketch, 'cause Contructors will not work
unsigned char i2cdev::begin(unsigned char address) {
	DevAdr = address;				// set Device Address
	Wire.beginTransmission(DevAdr);
	Wire.write(0x00); // IODIRA register
	Wire.write(0x00); // set all of port A to outputs (0=out, 1=in)
	Wire.endTransmission();
	Wire.beginTransmission(DevAdr);
	Wire.write(0x01); // IODIRB register
	Wire.write(0x00); // set all of port B to outputs  (0=out, 1=in)
	Wire.endTransmission();
	return 0;
}

// Standard Destructor
i2cdev::~i2cdev(void) {
// constructors will not work with arduino
}

//  https://www.arduino.cc/en/Reference/WireEndTransmission
// looking for devices at I2C Bus and print out their addresses 
unsigned char i2cdev::detect(void) {
char outTxt[10];
unsigned char col, row;
unsigned char ret;
//  print header
sprintf(outTxt, "\nI2C: ");
Serial.print(outTxt);

for (int i = 0; i < 16; i++) {
	sprintf(outTxt, "-%X ", i);
	Serial.print(outTxt);
	}

for (row=0;row<(0x80/0x10);row++){
	//print row header
	sprintf(outTxt, "\n-%X : ", row);
	Serial.print(outTxt);
	for (col = 0; col<0x10; col++) {
		Wire.beginTransmission(row*16+col);  // talk to slave
		ret = Wire.endTransmission();
		// 0:success
		// 1 : data too long to fit in transmit buffer
		// 2 : received NACK on transmit of address
		// 3 : received NACK on transmit of data
		// 4 : other error
		if (ret==0) 	sprintf(outTxt, "%X ", row*16+col);
		else if (ret == 2 || ret == 3) sprintf(outTxt, "-- ");
		else if (ret == 1 || ret==4) sprintf(outTxt, "e34 ");
		Serial.print(outTxt);
		}
	}
Serial.print("\n");
return ret;
}

// reads a byte (data) from register (reg) of a MCP23017
unsigned char i2cdev::rd(unsigned char reg) {
int ret;

Wire.beginTransmission(DevAdr); // begins to talk to a slave device
Wire.write(reg);				// selecting register
ret= Wire.endTransmission();
if (ret!=0) 	Serial.print("\nErr=1 in  i2cdev::rd()");
ret=Wire.requestFrom(DevAdr, 1);	// request one byte of data from MCP20317
if (ret<0) 	Serial.print("\nErr=2 in  i2cdev::rd()");
ret=Wire.read();				// store the incoming byte into "inputs"
return ret;
}


// writes a byte (data) to register (reg) of a MCP23017
unsigned char i2cdev::wr(unsigned char reg, unsigned char data) {
int ret;
if (reg==OLATA) shadowOLATA=data;
if (reg==OLATB) shadowOLATB=data;
Wire.beginTransmission(DevAdr); // begins to talk to a slave device
Wire.write(reg);				// selecting register
Wire.write(data);				// setting data into register
ret= Wire.endTransmission();

return ret;
}

// writes a byte (data) to register (reg) of a MCP23017
unsigned char i2cdev::wr_1(unsigned char reg, unsigned char data) {
int ret;
if (reg==OLATB){
// check state of databit 0 (=RB0), because its used for backlight 
if (shadowOLATB & (1 << LCD_BLUE)) data=data |   (1 << LCD_BLUE);
else                               data=data & (~(1 << LCD_BLUE));
}
ret=wr(reg,data);
return ret;
}


/*
getting a single bit of MCP-Rgister i.e. GPIOA and GPIOB
port : register of MCP23017 this version shadows OLATB, OLATA
bitnr: number of bit 0...7
return:  1 or 0
example: 
        unsigned char value=mcp23017.getBit(GPIOA,7);
*/
unsigned char i2cdev::getBit(unsigned char port, unsigned char bitnr){
unsigned char value;
value=rd(port);
value = bitnr >> value;
return;
}

/*
setting a single bit of OLATA and OLATB
port : register of MCP23017 this version shadows OLATB, OLATA
bitnr: number of bit 0...7
value: 1 or 0
example:
mcp23017.setBit(OLATB,7,1);
delay(500);
mcp23017.setBit(OLATB,6,1);
delay(500);
*/
void i2cdev::setBit(unsigned char port, unsigned char bitnr, unsigned char value) {
	unsigned char mask;
	mask = 1 << bitnr;
	if (port == OLATA) {
		if (value >0)	shadowOLATA = shadowOLATA | mask;
		else			shadowOLATA = shadowOLATA & ~mask;
		wr(OLATA, shadowOLATA);
	}

	if (port == OLATB) {
		if (value >0)	shadowOLATB = shadowOLATB | mask;
		else			shadowOLATB = shadowOLATB & ~mask;
		wr(OLATB, shadowOLATB);
	}
	return;
}


// Wrapper for wiring of MCP23017 with HD44780 on Adafruit's RGB_LCD from https://www.adafruit.com/product/714
// Moves 4 data-bits to its mapped positions given table wiredP[]. 
// All other bits left as zeros.  
unsigned char i2cdev::map(unsigned char in4bit) {
/*  unsigned char i,mask, help, out4bit=0;
    for (i = 0; i <= 3; i++) {
	mask = 1 << i;				// set mask to bit[i]
	help = mask & in4bit;		// masking out only bit[i]
	help = help >> i;			// shift to lowest bit[0]
	help = help << wiredP[i];	// shift bit[0] to wired mapped position
	out4bit = out4bit | help;	// set bit in output nibble 
	}
*/
unsigned char i,out4bit=0;
for (i = 0; i <= 3; i++) {
	out4bit = out4bit | ((((1 << i) & in4bit) >> i) << wiredP[i]);	// set bit in output nibble 
	}

return out4bit;
}

// remapping allMoves 4 data-bits to its mapped positions given table wiredP[]. 
// All other bits left as zeros.  
unsigned char i2cdev::remap(unsigned char in4bit) {
/*unsigned char i,mask, help, out4bit=0;
for (i = 0; i <= 3; i++) {
    mask = 1 << wiredP[i];		// set Mask to position=wireP[i]
	help = mask & in4bit;		// masking out only bit[i]
	help = help >> wiredP[i];	// shift to lowest bit[0]
	help = help << i;			// shift bit[0] to its orgin position
	out4bit = out4bit | help;	// set bit in output nibble 
	}
*/
unsigned char i, out4bit=0;
for (i = 0; i <= 3; i++) {
	out4bit = out4bit | ((((1 << wiredP[i]) & in4bit ) >> wiredP[i]) << i );	// set bit in output nibble 
	}

return out4bit;
}

// mask to identify valid data bits for HD 444780 databus
// unfinished function
unsigned char i2cdev::mapMask(void) {
unsigned char i, one,mask;

for (int i = 0; i <= 7; i++){
	one= 1 << wiredP[i];
	mask= mask | one;
	}

return mask;
}


// send 4 Bit Data to LCD
// cmd[0..4]: valid databits 
unsigned char i2cdev::LCDout4(unsigned char value, unsigned char rs) {
int out;

out = value & 0x0F;				// loesche obere cmd-bits
out = map(out);					// verdrahtete bits setzen
if (rs== LCD_DATA) out= out | (1 << LCD_RS);
// RB0
wr_1(OLATB,out);					// RS=1; EN=0, R/W=0, data-bits set to cmd

//out= out |  (1 << LCD_EN) |(1 << LCD_RS);		// RS=1; EN=1, R/W=0
out= out |  (1 << LCD_EN);		// RS=0; EN=1, R/W=0
wr_1(OLATB,out);		
		
__asm__ __volatile__("nop");	// wait 60ns: secure-wait from last access to LCD
delayMicroseconds(1);			// Enable = 1 wait for at least 450ns 
out= out & (~(1 << LCD_EN));	// RS=1; EN=0, R/W=0, databits stayed  
if (rs== LCD_DATA) out= out | (1 << LCD_RS);  // be sure that RS=1
wr_1(OLATB,out);		
delayMicroseconds(1);			// Enable = 0 wait for at least 450ns 

wr_1(OLATB,0);					// RS=0; EN=0, R/W=0, data-bits 0

return 0;
}


// reads 4 Bit Data from a LCD
// parameter rs not used
unsigned char i2cdev::LCDin4(unsigned char rs) {
int out;
int in; 

out=0;
LCDdir(LCD_RD);					// set MCP GPB[1..4] as inputs
out=out | (1<< LCD_RW);
wr(OLATB,out);					// RS=0; EN=0, R/W=1, data-bits set to cmd

//out= out |  (1 << LCD_EN) |(1 << LCD_RS);		// RS=1; EN=1, R/W=0
out= out |  (1 << LCD_EN);		// RS=0; EN=1, R/W=1
wr(OLATB,out);		
delayMicroseconds(1);			// Enable = 1 wait for at least 450ns 
//__asm__ __volatile__("nop");	// wait 60ns: secure-wait from last access to LCD
in=rd(GPIOB);					// read LCD_db from MCP23017
in=remap(in)& 0x0F;				// set bits in order an delete Hi Nibble
out= out & (~(1 << LCD_EN));	// RS=0; EN=0, R/W=1, databits stayed  
wr(OLATB,out);		
delayMicroseconds(1);			// Enable = 0 wait for at least 450ns 

LCDdir(LCD_WR);					// set MCP as outputs
wr(OLATB,0);					// RS=0; EN=0, R/W=0, data-bits 0

return in;
}


// send 8 Bytes in two nibbles to LCD
// first Hi-nibble then Lo-nibble
unsigned char i2cdev::LCDout8(unsigned char data, unsigned char rs) {

LCDout4(data>>4,rs);	// hi Byte of instruction/data
LCDout4(data,rs);		// lo Byte of instruction/data

return 0;
}


// gets 8 Data-Bits from LCD
// first Hi-nibble then Lo-nibble
unsigned char i2cdev::LCDin8(unsigned char rs) {
//char txt[30];
int ret;
unsigned char Hi;
unsigned char Lo;

Hi=LCDin4(rs);	// hi Byte of instruction/data
Lo=LCDin4(rs);	// lo Byte of instruction/data
ret=Hi<<4|Lo;

return ret;
}


// Initialization of a HD44780 LCD Display
// https://www.mikrocontroller.net/articles/AVR-GCC-Tutorial/LCD-Ansteuerung
// http://www.sprut.de/electronic/lcd/#init
unsigned char i2cdev::LCDbegin() {

// Special feature of Adafruits LCD is a Backlite off
wr(OLATA,0x0);				// set PortA D6=1, D7=1
wr(OLATB,0x01);				// set PortB D0=1

delayMicroseconds(15000);   // Wait for 15ms after power on

LCDout4(0x03,LCD_CMD);		// 1. Reset
delayMicroseconds(5000);	// wait min 5ms 

LCDout4(0x03,LCD_CMD);		// 2. Reset
delayMicroseconds(1000);	// Wait for 1ms

LCDout4(0x03,LCD_CMD);		// 3.Reset: set 8Bit Interface
delayMicroseconds(1000);	// Wait for 1ms

LCDout4(0x02,LCD_CMD);		// set 4Bit Interface
delayMicroseconds(5000);	// Wait for 5ms

//LCDout8(0x28,LCD_CMD);		// Function Set: 4Bit Interface, 2-zeilig, 5x8 Charakters
LCDout8(LCD_SET_FUNCTION|LCD_FUNCTION_4BIT|LCD_FUNCTION_2LINE|LCD_FUNCTION_5X8,
        LCD_CMD);			
delayMicroseconds(1);		// Wait for 1us

//LCDout8(0b00001111,LCD_CMD);// Display ein/aus: Dsp ein, Cursor ein, Blinken ein  
LCDout8(LCD_SET_DISPLAY|LCD_DISPLAY_ON|LCD_CURSOR_ON|LCD_BLINKING_ON,
		LCD_CMD);
delayMicroseconds(1);		// Wait for 1us

//LCDout8(0b00000110,LCD_CMD);// Entry Mode: Increment, kein Shift  
LCDout8(LCD_SET_ENTRY|LCD_ENTRY_INCREMENT|LCD_ENTRY_NOSCROLL,
		LCD_CMD);
delayMicroseconds(1);		// Wait for 1us

LCDdel();					// Display Löschen: Cursor Adresse 0
return 0;
}


// set Cursor to row/col beginning with 0
// row : 0...3  col: 0...15 take a look @ LCDrows/LCDcol in LCDbegin() 
char i2cdev::LCDcursor(unsigned char row, unsigned char col) {
unsigned char LCD_DD[4]={LCD_DDADR_LINE1,LCD_DDADR_LINE2,LCD_DDADR_LINE3,LCD_DDADR_LINE4};

if (row >= LCD_ROWSIZE) return -1;
if (col >= LCD_COLSIZE) return -11;

LCDout8(LCD_SET_DDADR|LCD_DD[row]+col,LCD_CMD);	

return 0;
}

// print a string from current cursor position
// return -1 : no  valid string pointer 
//        -2 : string to long
char i2cdev::LCDprint(char* string) {
int i;
if (string==NULL) return -1;
if (strlen(string)> LCD_COLSIZE) return -2;

for (i=0;i<strlen(string);i++) LCDout8(string[i],LCD_DATA);	

return 0;
}

// print a string from current cursor position
// this procedure is a native function of HD44780. It takes 2ms.
void i2cdev::LCDdel(void) {
LCDout8(0x01,LCD_CMD);		// Display Löschen: Cursor Adresse 0
delayMicroseconds(2000);	// Wait for 2ms

return;
}
/*
void i2cdev::void LCDbacklite(unsigned char colored) {
// setting OLATB
// setting OLATB
return;
}
*/
/*
 Parameter:   port		OLATA or OLATB
              bitnr		bit number[0..7]
              value		0 or 1
setBit(OLATB, 7,0);
*/

