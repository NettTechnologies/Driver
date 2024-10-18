ECU platform STM32F205ZET6. Cortex 120MHz

Included interfaces for:
	- EEPROM communication
	- CAN J1939 communication
 	-	RTC
	- SD Card Data saving (Log files)
 	- Pick&Hold driver
	- Analog inputs with error detection
 	-	Frequency input
 	-	Digital output
	-	Analog output
 	-	Access to SD card file system thrue USB connection
	-	Error vector
 	-	Main power input and auxiliary output error detection
	-	IGNSW voltage gontrol and shutdown delay

Pinout:
| dscdcd | dcdcd | dcdc | dc | dc |
|--------|-------|------|----|----|
| dcd    | cdc   | dc   | dc | dc |
| dcdc   | dcd   | dc   | dc | dc |
| dcd    | cd    | dc   | dc | dc |
  Pin			Signal																Range
  M1			GND																		0V
	M2			Digital/Freq output. Mosfet output		0-20A
  M3			Digital/Freq output. Mosfet output		0-20A
  M4			Pick&Hold															0-5A
  L1			+Battery															0-30V
	L2			Reference +5V. 	Max 500mA							4.9-5.1V	
	L3			Reference +12V. Max 250mA							11.8-12.2V	
 	L4			Pick&Hold	GND													0-5A
	K1			Ignition input												0-30V
 	K2			Power relay output. Mosfet output			1A 
	K3			CAN1_High
	K4			CAN1_Low
	J1			Digital output. GND										1A
	J2			Digital output. GND										1A
	J3			CAN2_High		
	J4			CAN2_Low		
	H1			Digital output. GND										1A
	H2			Digital output. GND										1A
	H3			Digital output. GND										1A
	H4			Digital output. GND										1A
	G1			Digital output. GND										2A
	G2			Digital output. GND										2A
	G3			Digital output. GND										2A
	G4			Digital output. GND										2A
	F1			Analog output. Max 50mA 							0-5V
	F2			Analog output. Max 50mA 							0-5V
	F3			Frequency input												5-30V
	F4			Analog input. Current									4-20mA	
	E1			Analog input. NTC sensor							1-340kOm
	E2			Analog input. NTC sensor							1-340kOm
	E3			Analog input. NTC sensor							1-340kOm
	E4			Analog input. NTC sensor							1-340kOm
	D1			Analog input													0-5V
	D2			Analog input													0-5V
	D3			Analog input													0-5V
	D4			Analog input													0-5V
	C1			Analog input													0-5V
	C2			Analog input													0-5V
	C3			Analog input													0-5V
	C4			Analog input													0-5V
	B1			+3.3V Programming purposes. Max 20mA	3.2-3.4V
	B2			USB DP
	B3			USB DN
	B4			Reserved
	A1			SWO.		Programming purposes
	A2			NRST.		Programming purposes
	A3			SWCLK.	Programming purposes
	A4			SWDIO.	Programming purposes

