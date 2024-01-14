# Polaroid-shutter-control
An arduino project which gives automatic Polaroid pack cameras manual shutter speeds

This project is specifically made for automatic Polaroid pack film cameras with a bellows and will work with models with extra apertures/ASA speeds as well as the simple version with just two apertures/ASA settings. It basically gives these fully automatic exposure cameras manual shutter speed capability. 

This project uses the SSD1306ASCII.h library for display since graphics aren't needed and the libraries are smaller. 

# Features:

- small footprint: will work with Nano Mega 168 boards
- push button switch and/or rotary encoder operation
- shutters speeds from 2 seconds to 1/1000 in 1/3 stop increments (can be modified in the code)
- operates on same battery power as the Polaroid camera
- switchable between manual and auto exposure mode

# Requirements:

- Polaroid pack film camera (with a bellows), ie. models 100 - 455 
- Arduino nano (will work with the mega 168)
- SSD1306 oLED display
- rotary encoder (optional)
- TLP127 opto-isolator and 330 Ohm resistor
- 2 momentary button switches

# Arduino Nano Configuration:

- D3: button switch (DOWN)
- D4: button switch (UP)
- D7: rotary encoder CLK (optional)
- D8: rotary encoder DT (optional)
- D9: 330 Ohm resistor, then to TLP 127 opto isolator
- D11: to shutter timer switch on Polaroid
- A4: SDA on I2C SSD1306
- A5: SCL on I2C SSD1306
- 3.3V: VCC on I2C SSD1306
- 5.0v: + on battery source from Polaroid (4.5 or 3.0 volts)
- GND: GND on SSD1306, pin 3 on TLP 127, button switches, GND side of shutter timing switch on Polaroid

# SSD1306 oLED display:

- SDA: A4 on Nano
- SCL: A5 on Nano
- GND: GND on Nano
- VCC: 3.3v on Nano

# TLP127 Opto Isolator:

 - Pin 1 (Anode): 330 Ohm resistor, then to D9
 - Pin 3 (Cathode): GND on Nano
 - Pin 4 (Emitter): VCC on camera
 - Pin 6 (Collector): VCC on battery

   pinouts:

   `6    4`  
   `------`  
   `|    |`  
   `|O   |`  
   `------`  
   ` 1   3`  

# Rotary Encoder (optional):

- GND: GND on Nano
- VCC: 5v on Nano
- CLK: Pin D7 on Nano
- DT: Pin D8 on Nano
- SW: not used

Two momentary switches are sufficient for choosing shutter speeds, and the button delay can be modified before uploading. A rotary encoder is coded in, but they are somewhat large and don't fit easily into the battery compartment of the camera. 

To operate, choose the desired shutter speed, then cock the shutter. Once the shutter is cocked, you cannot change the shutter speed as the program is dedicated to waiting for the first shutter blade to open, and timing is critical for high shutter speeds. Keep the shutter pressed down during the duration of the exposure, at which time you can choose a different shutter speed. Should you need to change shutter speeds after cocking the shutter, keep the arduino power on, and switch the auto/manual switch to auto. The shutter will fire with no power, with the closing shutter blade immediately following the opening blade. 

# Integration with a Polaroid Pack film camera:

Reference the file "Polaroid Modification.jpg". This project needs access to the shutter timing switch on the Polaroid shutter. Disassemble the shutter body, remove the circuit/shutter from the lens body and flip upside down, with the camera facing you. On the upper right, to the left of the cocking lever, there are 2 solder contacts connected to the shutter timing switch. You will need to desolder the upper terminal (remember, the shutter is upside down so it shows as the lower terminal in the photo) which connects the timing switch to the shutter circuit. The lower terminal connects to ground and stays intact. After desoldering, If you want to retain the option to use auto exposure, connect a wire to the desoldered contact on the ribbon cable (careful not to melt it). Connect a wire to the timing switch underneath the ribbon. You should insulate that contact from the ribbon cable to prevent contact since you want to isolate the shutter circuit from the timing switch. Label the wires connected to the switch ("Timing switch), and to the ribbon cable (Shutter circuit) and route them to the battery compartment of the camera. If you do not want to retain auto exposure capability, you need to solder a wire from S2 on the circuit ribbon to GND (the solder terminal next to it). 

If you want to be able to switch between auto and manual shutter control, you will need a DPDT and an SPDT switch (or 3 SPDT switches) and wire the arduino/camera using the Polariod integration schematic using the DPDT switch (or 2 SPDT switches) to configure the camera between auto and manual shutter control. By using a separate SPDT switch to switch power between the arduino and camera, you can shoot with "bulb" by switching this switch to Polaroid (auto), and keeping the DPDT switch on manual. 

A 3D printable file is included to help install the screen and switches onto the battery door of a Polaroid pack film camera. 

# Principles of operation:

While the arduino is controlling the Polaroid shutter, the Polaroid shutter circuit is connected to ground, which discharges the internal capacitors and keeps the shutter open. This effectively puts the camera into "bulb" mode. The arduino de-energizes the Polaroid shutter circuit and is waiting for the shutter to be cocked (sensed by the shutter timing switch closing) at which point, the arduino energizes the Polaroid camera circuit by turning on the opto-isolator and effectively connecting battery power to the Polaroid. Having said this, no power is being used by the Polaroid ciruit until the shutter is pressed. When the shutter is pressed, a switch energizes the solenoid which holds back the second shutter curtain and opens the first shutter curtain, also opening the shutter timing switch. Once the arduino detects the shutter open (timing switch open), it times the exposure, then cuts off power to the camera, de-energizing the solendoid and closing the shutter. 

# The Basics: How to get setup with the Arduino and program it:

If you are just getting started with Arduino, here are some instructions to get going. 

1. Download and install Arduino IDE from here: https://www.arduino.cc/en/software
2. Under tools, manage libraries, search for and install SSD1306ASCII by Bill Greiman (this should be the only external library needed).
3. In Arduino IDE, go to File, New Sketch, and erase everything in the new window.
4. Go to the code from this page (polaroidshutter.ino) and put your cursor inside the code window and press <ctrl>A then <ctrl>C to copy the code
5. Back to Arduino IDE, put your cursor inside the new code window and press <ctrl>V to paste the code
6. Connect an Arduino nano to a USB cable on the computer
7. In the toolbar below the menu, a white box will indicate the arduino devices available. Click on the down arrow to see what's there. Select the COM port that's listed there
8. In the window that opens, choose the Nano. The device should now be selected.
9. Go to tools, processor, and select the processor that your nano is using: ATmega168 = 16K, ATmega328P = genuine arduino nano, ATmega328P (old bootloader) = generic (Chinese) nano.
10. Go to Sketch, Upload (or press <ctrl>U) to compile and upload the code to the nano
