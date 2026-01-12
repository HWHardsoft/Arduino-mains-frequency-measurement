# Arduino-mains-frequency-measurement

![Image](https://github.com/user-attachments/assets/f785bfbb-316d-462b-8c5f-fac276cf1696)


## Jumper setting 
On the HAT, the Raspberry Pi's UARTs U0, U3, U4, and U5 can be selected via jumpers. Although UART0 is routed to the correct pins on the ESP32-P4, it is also used for the USB debug interface and therefore cannot be used:
For the other 3 UARTs, only the appropriate line of code in the source code needs to be activated:   
```
  // Initialize the RS485 interface. If you are initializing the RS485 interface
  // manually, then the parameter can be empty.
  //Serial2.setPins(33, 23); // pins of U3
  //Serial2.setPins(2, 36); // pins of U4
  Serial2.setPins(48, 54); // pins of U5
  RS485.begin(9600, SERIAL_8N1);
```

## DIP Switch setting
S1: OFF-ON-ON-OFF

S2: OFF-OFF-ON-ON

S3: ON-OFF-ON-ON



## Hardware
- [Waveshare ESP32-P4 Development Kit](https://www.waveshare.com/esp32-p4-module-dev-kit.htm)
- [Zihatec RS485 HAT for Raspberry Pi](https://www.hwhardsoft.de/english/projects/rs485-shield)


## Essential Libraries
These examples use the libraries:
- [CSE_ArduinoRS485](https://github.com/CIRCUITSTATE/CSE_ArduinoRS485)
- [CSE-ModbusRTU](https://github.com/CIRCUITSTATE/CSE_ModbusRTU)

from Vishnu Mohanan (CIRCUITSTATE Electronics LLP).


## License

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.
This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.
You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

mains frequency measurement with Arduino MKR and ili9143 based display
