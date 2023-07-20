# Polaroid-shutter-control
An arduino project which gives automatic Polaroid pack cameras manual shutter speeds

This project is specifically made for automatic Polaroid pack film cameras with a bellows and extra apertures. By tapping into the shutter timing switch, once the arduino senses the shutter is open, it times the exposure as chosen by the user, then shuts down the camera when the time is complete. The camera needs to be modified to accomplish this, and a module made to accomodate the arduino, display, and rotary encoder. This modification has not been spec'd for the simple version of pack camera with a plastic lens and only 2 exposure settings, though it should work. 

This project uses the SSD1306ASCII.h library for display since graphics aren't needed and the libraries are smaller. 

Features:

- small footprint: will work with Nano Mega 168 boards
- rotary encoder operation
- shutters speeds from 2 seconds to 1/1000
- operates on same battery power as the Polaroid camera

Requirements:

- Polaroid pack film camera (with a bellows), ie. model 100, 240, 250, etc. 
- Arduino nano (will work with the mega 168)
- SSD1306 oLED display
- rotary enocder
- TLP127 opto-isolator
- 2 switches

Configuration:

- D7: rotary encoder CLK
- D8: rotary encoder DT
- D9: 330 Ohm resistor, then to TLP 127 opto isolator
- D11: to shutter timer switch on Polaroid
- A4: SDA on I2C SSD1306
- A5: SCL on I2C SSD1306
- 3.3V: VCC on I2C SSD1306, Polaroid VCC on cameras that use 3.0 volts
- 5.0v: + on battery source from Polaroid (4.5 or 3.0 volts)
- GND: GND on I2C SSD1306, pin 3 on TLP 127, GND side of shutter timing switch on Polaroid

SSD1306 oLED display:

- SDA: A4 on Nano
- SCL: A5 on Nano
- GND: GND on Nano
- VCC: 3.3v on Nano

TLP127 Opto Isolator
 - Pin 1 (Anode): 330 Ohm resistor, then to D9
 - Pin 3 (Cathode): GND on Nano
 - Pin 4 (Emitter): GND on camera lead
 - Pin 6 (Collector): GND on camera battery

Rotary Encoder:

- GND: GND on Nano
- VCC: 5v on Nano
- CLK: Pin D7 on Nano
- DT: Pin D8 on Nano
- SW: not used

To operate, choose the desired shutter speed, then cock the shutter. Once the shutter is cocked, you cannot change the shutter speeds as the program is dedicated to waiting for the first shutter blade to open. Keep the shutter pressed down during the duration of the exposure, at which time you can choose a different shutter speed. Should you need to change shutter speeds after cocking the shutter, hold down the cocking lever while pressing the shutter and slowly raise the cocking lever back to the top. 

Further documentation on the modifications required on the Polaroid are forthcoming.
