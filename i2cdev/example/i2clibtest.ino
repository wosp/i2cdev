/* 
* Author :  W.Spiess
* Date   :  24012018
* Version:  1.0
* File   :  i2ctest.ino
* ------------
*
* I2C Port Expander MCP23017.
*
*/

#define TRUE !0
#define FALSE 0

//#include "myHeader.h"
#include <wire.h>
#include "i2cdev.h"
//#include <Adafruit_MCP23017.h>  
//#include <Adafruit_RGBLCDShield.h>
i2cdev mcp23017;				// create a common object from user

int ledPin = 13;                 // LED connected to digital pin 13

void setup()
{
	Wire.begin();            //creates a Wire object
	Serial.begin(9600);      // open the serial port at 9600 bps:    
	while (!Serial);
	mcp23017.begin(0x20);    // MCP23017 Device Address
	pinMode(ledPin, OUTPUT);      // sets the digital pin as output
	Serial.println("\nalle Ports A/B als Ausgang");

}
// http://tronixstuff.com/2011/08/26/tutorial-maximising-your-arduinos-io-ports/

unsigned char cnt = 1;
void loop() {

	mcp23017.wr(0x15, cnt);
	cnt = cnt << 1;
	if (cnt == 0x0) cnt = 0x1;
	/* add main program code here */
	digitalWrite(ledPin, HIGH);   // sets the LED on
	delay(500);                  // waits for a second
	digitalWrite(ledPin, LOW);    // sets the LED off
	delay(500);
}
