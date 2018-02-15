#define TRUE !0
#define FALSE 0

#include <wire.h>
#include "i2cdev.h"    // using i2cdev lib
//#include "i2cdev.cpp"    // using i2cdev lib

i2cdev mcp23017;				// create a common object from user i2cdev lib
int ledPin = 13;                // LED connected to digital pin 13
unsigned char ret;
char txt[30];
struct watch{
unsigned char sec;
unsigned char min;
unsigned char hour;
};
watch mywatch;


void setup()
{
  /* add setup code here */

Wire.begin();				// intialize a Wire object Standard Speed mode of 100kHz
Wire.setClock(880000);		// Optional - set I2C SCL to High Speed Mode of 400kHz
							// 880000, 800000, 600000, 400000 (hiSp)
// clockFrequency: the value (in Hertz) of desired communication clock. 
// Accepted values are 100000 (standard mode) and 400000 (fast mode). 
// Some processors also support 10000 (low speed mode), 1000000 (fast mode plus) and 
// 3400000 (high speed mode). 
// Please refer to the specific processor documentation to make sure the desired mode 
// is supported.
// https://forum.arduino.cc/index.php?topic=385452.0 
Serial.begin(9600);			// open the serial port at 9600 bps:    
while (!Serial);

mcp23017.begin(0x20);		// MCP23017 Device Address
// mcp23017.detect();
mcp23017.LCDbegin();
mcp23017.LCDprint("        08.02.18");
mcp23017.LCDcursor(1,0);
mcp23017.LCDprint("Wolfi Spiess");
mcp23017.LCDcursor(0,0);
pinMode(ledPin, OUTPUT);	// sets the digital pin as output
Serial.print("\n");
mcp23017.setBit(OLATB,0,1);
mcp23017.setBit(OLATA,6,0);
mcp23017.setBit(OLATA,7,0);
mywatch.hour=mywatch.min=mywatch.sec=0;
}

void loop()
{
  /* add main program code here */
if (mywatch.sec==60) {
mywatch.sec=0;
mywatch.min++;
if (mywatch.min==60) {
    mywatch.hour++;
    mywatch.min=0;
	}
}
// alive blinking LED @ Arduino Port
digitalWrite(ledPin, HIGH);   // sets the LED on
delay(500);						// waits for a second
digitalWrite(ledPin, LOW);    // sets the LED off
delay(500);
mcp23017.LCDcursor(0,0);
sprintf(txt,"%02d:%02d:%02d",mywatch.hour,mywatch.min,mywatch.sec);
mcp23017.LCDprint(txt);
mywatch.sec++;

}
