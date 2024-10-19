ECU platform STM32F205ZET6. Cortex 120MHz

Included interfaces for:
<ul>
<li>-NO EEPROM communication</li>
<li>CAN J1939 communication</li>
<li>_NO RTC</li>
<li>SD Card Data saving (Log files)</li>
<li>Pick&Hold driver</li>
<li>Analog inputs with error detection</li>
<li>-NO Access to SD card file system thrue USB connection</li>
<li>Error vector</li>
<li>-NO Main power input and auxiliary output error detection</li>
<li>-NO IGSW voltage gontrol and shutdown delay</li>
	

Pinout:
| Pin | Signal                               | Range      | Comment |
|-----|--------------------------------------|------------|---------|
| C1  | GND                                  | 0V         |         |
| C2  | Digital/Freq output. Mosfet output   | 0-10A      |         |
| C3  | Digital/Freq output. Mosfet output   | 0-10A      |         |
| C4  | Pick&Hold                            | 0-5A       |         |
| C5  | +Battery                             | 0-30V      |         |
| C6  | Reference +5V. Max 500mA             | 4.9-5.1V   |         |
| C7  | Reference +12V. Max 250mA            | 11.8-12.2V |         |
| C8  | Pick&Hold                            | 0-5A       |         |
| C9  | Ignition input                       | 0-30V      |         |
| C10 | Power relay output. Mosfet output    | 1A         |         |
| C11 | CAN1_High                            |            |         |
| C12 | CAN1_Low                             |            |         |
|     |                                      |            |         |
| D1  | Digital output. GND                  |  1A        |         |
| D2  | CAN1_High                            |            |         |
| D3  | CAN1_Low                             |            |         |
| D4  | Digital output. GND                  |  1A        |         |
| D5  | Digital output. GND                  |  1A        |         |
| D6  | Digital output. GND                  |  1A        |         |
| D7  | Digital output. GND                  |  1A        |         |
| D8  | Digital output. GND                  |  2A        |         |
| D9  | Digital output. GND                  |  2A        |         |
| D10 | Digital output. GND                  |  2A        |         |
| D11 | Digital output. GND                  |  2A        |         |
| D12 | Analog output. Max 50mA              |  0-5V      |         |
