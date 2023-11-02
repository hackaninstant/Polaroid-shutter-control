#include <Wire.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"
#include <EEPROM.h>
#define I2C_ADDRESS 0x3C

// Define proper RST_PIN if required.
#define RST_PIN -1

SSD1306AsciiWire oled;

// Output pins:
#define powerPin                9                        // for opto-isolator, camera power switch

// Input pins:
#define shutterPin              11                      // shutter switch on Polaroid
#define PlusButtonPin           3                       // Plus button pin
#define MinusButtonPin          4                       // Minus button pin
#define rotButtonPin            6                       // rotary encoder switch
#define rotCLKPin               7                       // rotary encoder clock
#define rotDTPin                8                       // rotary encoder DT pin

boolean PlusButtonState;                // "+" button state
boolean MinusButtonState;               // "-" button state
boolean MenuButtonState;                // menu button state
boolean shutter;                        // shutter state
boolean cocked;                         // shutter cocked state
boolean mainmenu = true;                // main menu displayed
boolean adjustmenu = false;             // adjust menu displayed

// EEPROM for memory recording
#define ShutterSpeedAddr        1         // shutter speed
#define ButtonDelayAddr         2        // button repeat delay
#define TimerValuesAddr         3         // for timer values array

// default values if running for the first time

#define defaultButtonDelay      150       // button repeat delay
#define defaultShutterSpeedIndex 10       // default shutter speed
#define MaxShutterIndex         35        // matches number of items in spvalues to loop -1

// load values from EEPROM
uint8_t ShutterSpeedIndex =   EEPROM.read(ShutterSpeedAddr);
uint8_t buttondelay =         EEPROM.read(ButtonDelayAddr);
// unit8_t ShutterCocked =       EEPROM.read(ShutterCockedAddr);

float voltage = 0;                   // for storing voltage of battery

int rotcounter = 0;                  // counter for rotary enocoder
int currentStateCLK;                 // for rotary encoder
int lastStateCLK;                     // for rotary encoder
unsigned long lastButtonPress = 0;    // for rotary encoder
int btnState;                         // for rotary encoder
boolean rotaryencoder = false;        // for rotary encoder

// Shutter speed values in seconds. If you add/delete, modify MaxShutterIndex
// spvalues is the displayed shutter speed. timervalues is the actual amount of time that will be counted down
float spvalues[] =    {.00066667, .000833, .0010,  .00125, .0015625, .002, .0025, .003125,  .004, .0050, .00625, .008, .01, .0125, .0167, .02, .025, .033, .04, .05, .067, .08, .1, .125, .150, .200, .250, .300, .400, .500, .600, .800, 1.000, 1.250, 1.500, 2.000};
float timervalues[] = {.00045, .00065, .00085, .0011, .001420, .00182, .00230, .00290, .00375, .00475, .00623, .008, .01, .0125, .0167, .02, .025, .033, .04, .05, .067, .08, .1, .125, .150, .200, .250, .300, .400, .500, .600, .800, 1.000, 1.250, 1.500, 2.000};

// Store settings
void SaveSettings() {
  EEPROM.write(ShutterSpeedAddr, ShutterSpeedIndex);
}

// read button state
void readButtons() {
  PlusButtonState = digitalRead(PlusButtonPin);
  MinusButtonState = digitalRead(MinusButtonPin);
}

void readShutterState(){
  shutter = digitalRead(shutterPin);
}

// figure out what to do with button presses
void menu() {
  
// on main screen and a plus or minus button is pressed:

    if(PlusButtonState == 0) {
     if(ShutterSpeedIndex >= MaxShutterIndex) {
       ShutterSpeedIndex = 0;
      } else {
       ShutterSpeedIndex++;               // Increase shutter speed time
     }
    refreshShutter();                            // redisplay shutter speed
    SaveSettings();
   }
    if(MinusButtonState == 0) {
     if(ShutterSpeedIndex == 0) {
      ShutterSpeedIndex = MaxShutterIndex;
    } else {
     ShutterSpeedIndex--;                 // decrease shutter speed time
    }
    refreshShutter();                     // display shutter speed
    SaveSettings();
   }  
}


void printdivider(int row) {                // prints a divider
  oled.set1X();
  oled.setCursor(0, row);
  int count = 1;
  while (count < 23) {                      // 22 characters wide
    oled.print(F("-"));                     // choose your divider character...
   count++;
  }
}

// Display main screen
void refresh() {

  // Start main display
  oled.clear();
  oled.set1X();
  oled.setCursor(0, 0);
  oled.print(F("Shutter Speed:"));
  oled.setCursor(100,7);
  oled.print(voltage, 1);
  oled.print(F("v"));
  printdivider(1);
  printdivider(6);
  oled.set1X();
  oled.setCursor(40, 3);
  oled.print(F("|"));
  oled.setCursor(40, 4);
  oled.print(F("|"));
  oled.setCursor(0, 3);
  oled.print(F("Shutter"));
  oled.setCursor(0, 4);
  oled.print(F("Speed"));
  refreshShutter(); 

  delay(buttondelay);
}

void refreshShutter(){                        // display just shutter speed 
  float T = spvalues[ShutterSpeedIndex];      // get shutter speed from array
  uint8_t Tdisplay = 0;                       // Flag for shutter speed display style (fractional, seconds, minutes)
  double  Tfr = 0;                            // value for shutter speed fraction
  float   Tmin = 0;                           // shutter speed in minutes, if over 60 seconds
  if (T >= 60 && T <= 600) {
    Tdisplay = 0;                             // Exposure is in minutes
    Tmin = T / 60;

  } else if (T < 60 && T >= 0.6) {            // speed in seconds
    Tdisplay = 2;                             // Exposure in in seconds

  } else if (T < 0.6 && T >= .0001) {            // speed in fractions
    Tdisplay = 1;                             // Display is in fractional form
    Tfr = round(1 / T);
  }

  oled.set2X();
  oled.setCursor(50, 3);
  oled.print(F("       "));           // clear old shutter speed
  oled.setCursor(50, 3);              // reset cursor 
  if (Tdisplay == 0) {                // display shutter speed
    oled.print(Tmin, 1);              // in minutes
    oled.print(F("m"));
  } 
  if (Tdisplay == 1) {                // or in fractions
   if (T > 0) {
     oled.print(F("1/"));
     oled.print(Tfr, 0);
   }
  }
  if (Tdisplay == 2) {                // or in seconds
    oled.print(T, 1);
    oled.print(F("s"));
  }
  if(!rotaryencoder){         // if a button is being used, delay
  delay(buttondelay);
  }
  rotaryencoder = false;      // clear rotary encoder use flag
}

void setup() {
pinMode(PlusButtonPin, INPUT_PULLUP);         // setup buttons to pins
pinMode(MinusButtonPin, INPUT_PULLUP);
pinMode(shutterPin, INPUT_PULLUP);            // shutter switch on Polaroid
pinMode(powerPin, OUTPUT);                    // to transistor that switches on/off Polaroid
pinMode(rotCLKPin, INPUT);                    // setup rotary encoder
pinMode(rotDTPin, INPUT);
pinMode(rotButtonPin, INPUT_PULLUP);          // setup rotary encoder switch

lastStateCLK = digitalRead(rotCLKPin);        // read last state of CLK on rotary encoder

Wire.begin();
oled.begin(&Adafruit128x64, I2C_ADDRESS);     // setup oled address
oled.setFont(Adafruit5x7);                    // setup oled font

// load defaults if first time running
if (ShutterSpeedIndex > MaxShutterIndex) {
  ShutterSpeedIndex = defaultShutterSpeedIndex;
}
if (buttondelay > 250) {
  buttondelay = defaultButtonDelay;
  EEPROM.write(ButtonDelayAddr, buttondelay);
}

// get voltage
const long InternalReferenceVoltage = 1056L;  // Adjust this value to your boards specific internal BG voltage x1000
ADMUX = (0 << REFS1) | (1 << REFS0) | (0 << ADLAR) | (1 << MUX3) | (1 << MUX2) | (1 << MUX1) | (0 << MUX0);
delay(100);  // Let mux settle a little to get a more stable A/D conversion
ADCSRA |= _BV( ADSC );
while ( ( (ADCSRA & (1 << ADSC)) != 0 ) );
voltage = (((InternalReferenceVoltage * 1024L) / ADC) + 5L) / 10L; // calculates for straight line value
voltage = voltage/100;

digitalWrite(powerPin, LOW);                     // de-energize Polaroid camera

refresh();                                        // display main screen

}

void loop() {

// first thing's first -- check to see whether the shutter is cocked (shutterstate will trip high (on))
readShutterState();
if(!shutter){                                       // if the shutter is cocked (sensed by shutterstate tripping high (on))
 float shutterdelay = timervalues[ShutterSpeedIndex];  // get shutter speed
 shutterdelay = shutterdelay * 1000;                // convert to milliseconds
 digitalWrite(powerPin, HIGH);                      // engergize Polaroid (Polaroid itself will not turn on until shutter is pressed)
 cocked = true;                                     // indicate shutter as cocked
 oled.set1X();
 oled.setCursor(0,7);
 oled.print(F("cocked"));                            // indicate that shutter is cocked
 delay(300);                                        // give the shutterstate switch time to settle low
 int delayspeed = 1;
 if(shutterdelay < 8){
   delayspeed = 2;
 }
 switch(delayspeed){                                // branch depending on how to count delay
 case(1):                                           // shutter higher than 1/125
  while(!shutter){                                   // while the shutter is cocked but not fired
    readShutterState();                              // wait here until shutter is fired
 }    
 delay(shutterdelay);                               // wait until shutter speed time is up
 digitalWrite(powerPin, LOW);                       // de-energize the Polaroid to close shutter
 break;
 case(2):                                           // shutter below 1/125 count in microseconds
 shutterdelay = shutterdelay * 1000;                // convert to microseconds
  while(!shutter){                                   // while the shutter is cocked but not fired
    readShutterState();                              // wait here until shutter is fired
 }    
 delayMicroseconds(shutterdelay);
 digitalWrite(powerPin, LOW);
 break;
}
 refresh();
}

readButtons();                                // get button state

currentStateCLK = digitalRead(rotCLKPin);     // read current state of rotary encoder
if(lastStateCLK == LOW && currentStateCLK == HIGH){
  rotaryencoder = true;
  if(digitalRead(rotDTPin) == HIGH ){
    MinusButtonState = 0;                     // translate it to button state
  } else {
    PlusButtonState = 0;
  }
}
lastStateCLK = currentStateCLK;               // wait until next event from encoder

/*
btnState = digitalRead(rotButtonPin);             // read rotary encoder button
if (btnState == LOW){
  if (millis() - lastButtonPress > 50){
    MenuButtonState = 0;                      // translate encoder switcht to menu push
  }
  lastButtonPress = millis();
}
*/

menu();                                 // do stuff with button press

}
