/* coaxtester.ino
   Tests coax cable to determine if there is a short or open circuit, or 
   something in between
   The in-between is only accurate if the resistance is about the same value
   as the resistor in the test circuit.
   AA6BD hwd 2023-08-08
   derived from https://www.circuitbasics.com/arduino-ohm-meter/
   https://github.com/bhall66/LED-tester/blob/main/LED_tester/LED_tester.ino
   https://roboticsbackend.com/arduino-push-button-tutorial/
   http://www.gammon.com.au/switches pulldown resistor
   https://randomnerdtutorials.com/esp32-i2c-communication-arduino-ide/ 
   https://www.youtube.com/watch?v=WJXr9LEE0vk Build a Coax Tester
*/
#include <Wire.h>                                  // Arduino I2C library
#include <Adafruit_GFX.h>                          // Adafruit graphics library
//#include <Adafruit_SSD1306.h>                    // Adafruit OLED library for SSD1306
#include <Adafruit_SH110X.h>                       // Library for SH1106 controller

/* Uncomment the initialize the I2C address , uncomment only one, If you get a totally blank screen try the other*/
#define i2c_Address 0x3c //initialize with the I2C addr 0x3C Typically eBay OLED's
//#define i2c_Address 0x3d //initialize with the I2C addr 0x3D Typically Adafruit OLED's

//if you can't find I2C device, use this code to find valid I2C address
//https://randomnerdtutorials.com/esp32-i2c-communication-arduino-ide/

#define SCREEN_WIDTH       128                     // OLED display width, in pixels
#define SCREEN_HEIGHT       64                     // OLED display height, in pixels
#define OLED_RESET -1                              //   QT-PY / XIAO

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
//Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
// Declaration for an SSH1106 display connected to I2C
Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int analogPin = 34; //GPIO34 ADC6 physical5 to measure voltage

int raw = 0;
float Vin = 3.3;      // Vin to coax center pin
float Vout = 0;       // Vout from coax ground
float buffer = 0;
float R1 = 10000;   // R1 for open measuring
float R2 = 0;       // R2 resistance of coax

void setup(){
  Serial.begin(115200);             // initialize serial monitor
  delay(250);                       // wait for the OLED to power up
  //display.begin(SSD1306_SWITCHCAPVCC, i2c_Address);  //initialize SSD1306 display
  display.begin(i2c_Address, true); //initialize the SSH1106 display
  delay(2000);
}

void loop(){
  display.clearDisplay();
  display.setTextSize(1);
  //display.setTextColor(WHITE);        // for SSD1306
  display.setTextColor(SH110X_WHITE);   // for SSH1106
  display.setCursor(20, 0);
  // Display "Coax Tester"
  display.println("Coax Tester");

  raw = analogRead(analogPin);    //read voltage
  Serial.print("Raw:" );          // and display it
  Serial.println(raw);
  display.setTextSize(1);
  display.setCursor(0, 35);
  display.print("Raw: ");
  display.println(raw);

  if (raw>4000){                  // if coax is shorted,
    Serial.println("SHORT");      // display this
    display.setTextSize(2);
    display.setCursor(30,15);
    display.println("SHORT");
  }
  if (raw<100) {                  // if coax is open circuit,
    Serial.println("OPEN");       // display this
    display.setTextSize(2);
    display.setCursor(30,15);
    display.println("OPEN");
  }   
  buffer = raw * Vin;            // compute Vout
  Vout = (buffer)/4095.0;
  buffer = (Vin/Vout) - 1;
  R2= R1 * buffer;
  Serial.print("Vout: ");        // display Vout
  Serial.println(Vout);
  Serial.print("R2: ");
  Serial.println(R2);            // display R2
  display.setTextSize(1);
  display.setCursor(0,45);
  display.print("Vout: ");
  display.print(Vout);
  display.setCursor(0,55);
  display.print("R2=");
  display.print(R2);

  display.display();              //display all of the lines
  delay(1000);                    //wait for a second then go back and do it again
}
