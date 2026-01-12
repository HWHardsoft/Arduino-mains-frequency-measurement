# Mains frequency measurement with Arduino MKR and IlI9341 based display

![Image](https://github.com/user-attachments/assets/f785bfbb-316d-462b-8c5f-fac276cf1696)

An important indicator for assessing the condition of the power grid is the frequency of the grid. In Europe, this frequency is set at 50Hz. If there is too much electricity in the grid, this frequency increases; if the load is too high and there is too little electricity available, the frequency decreases. Anything within a range of 49.80Hz to 50.20Hz is normal. Outside this range, the situation becomes critical. 
In this project, an Arduino MKR is used to measure the frequency of the power grid and then display the frequency on an ILI9341-based display. 


## Hardware
- [Arduino MKR WiFi 1010](https://store.arduino.cc/products/arduino-mkr-wifi-1010)
- [AZ-Touch MKR V2](https://www.hwhardsoft.de/english/projects/arduitouch-mkr/)

To measure 50Hz, all we need is a 20cm piece of wire connected to a free GPIO on the Arduino via a 1MOhm resistor. The inputs are very high impedance, so this ‘antenna’ can be used to easily measure the 50Hz on a cable passing by. The resistor, together with the input capacitances, forms a high-pass filter.
In my software, I use GPIO14 (TxD) of the Arduino because this pin is not used by the AZ-Touch. In principle, however, any other pin can be used with the exception of the I2C pins. 


## Essential Libraries
These examples use the libraries:
- [Adafruit GFX](https://github.com/adafruit/Adafruit-GFX-Library)

## Credits
Youtube User [Elektronik](https://www.youtube.com/watch?v=kffy9bAcYZE)


## License

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.
This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.
You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA


