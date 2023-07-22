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
- rotary encoder (optional)
- TLP127 opto-isolator
- 2 switches

Arduino Nano Configuration:

- D3: button switch
- D4: button switch
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

TLP127 Opto Isolator:

 - Pin 1 (Anode): 330 Ohm resistor, then to D9
 - Pin 3 (Cathode): GND on Nano
 - Pin 4 (Emitter): VCC on camera
 - Pin 6 (Collector): VCC on battery

Rotary Encoder:

- GND: GND on Nano
- VCC: 5v on Nano
- CLK: Pin D7 on Nano
- DT: Pin D8 on Nano
- SW: not used

Two switches are sufficient for choosing shutter speeds, and the button delay can be modified before uploading. A rotary encoder is coded in, but they are somewhat large and don't fit easily into the battery compartment of the camera. 

To operate, choose the desired shutter speed, then cock the shutter. Once the shutter is cocked, you cannot change the shutter speeds as the program is dedicated to waiting for the first shutter blade to open. Keep the shutter pressed down during the duration of the exposure, at which time you can choose a different shutter speed. Should you need to change shutter speeds after cocking the shutter, hold down the cocking lever while pressing the shutter and slowly raise the cocking lever back to the top. 

Integration with a Polaroid Pack film camera:

This project needs access to the shutter timing switch on the Polaroid shutter. Disassemble the shutter body, remove the circuit/shutter from the lens body and flip upside down, with the camera facing you. On the upper right, to the left of the cocking lever, there are 2 solder contacts connected to the shutter timing switch. You will need to desolder the upper terminal (remember, the shutter is upside down) which connects the timing switch to the shutter circuit. The lower terminal connects to ground and stays intact. After desoldering, connect a wire to the circuit ribbon (careful not to melt it), and a wire to the timing switch underneath the ribbon. You should insulate the switch from the ribbon to prevent contact since you want to isolate the shutter circuit from the timing switch. Label the wires connected to the switch ("Timing switch), and to the ribbon cable (Shutter circuit) and route them to the battery compartment of the camera.

If you don't want to retain auto exposure, you can simple connect the cables according to the schematic diagram. Auto exposure will not work. 

If you want to be able to switch between auto and manual shutter control, you will need a 3PDT switch (or a DPDT and an SPDT switch) and wire the camera using the auto/manual schematic. 

Principles of operation:

While the arduino is controlling the Polaroid shutter, the shutter timing switch is always closed on the Polaroid shutter circuit, which discharges the internal capacitors and keeps the shutter open. This is effectively putting the camera in "bulb" mode. The arduino de-energizes the Polaroid shutter circuit and is waiting for the shutter to be cocked (sensed by the shutter timing switch closing) at which point, the arduino energizes the Polaroid shutter by turning on the opto-isolator and effectively connecting battery power to the Polaroid. When the shutter is pressed, a switch energizes the solenoid which holds back the second shutter curtain and opens the first shutter curtain, also opening the shutter timing switch. Once the arduino detects the shutter open (timing switch open), it times the exposure, then cuts off power to the camera, de-energizing the solendoid and closing the shutter. 

Plans: 

Design and print a battery compartment door which will fit all the components of this build and replace the existing door. 
