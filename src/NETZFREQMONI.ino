/*
 *  Application note: 50Hz grid frequency monitor
 *  Version 1.0
 *  Copyright (C) 2026  Hartmut Wendt  www.zihatec.de
 *  
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.

 *  Credits: 
 *  Based on code by https://www.youtube.com/watch?v=kffy9bAcYZE
 *
 *  For reference values check https://netzfrequenzmessung.de/


*/   

#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include <Fonts/FreeMono9pt7b.h>
#include "DSEG7_Classic_Mini_Bold_16.h"
#include "DSEG7_Classic_Mini_Bold_36.h"

// pin definitions for Arduino MKR Boards 
#define TFT_CS   A3
#define TFT_DC   0
#define TFT_MOSI 8
#define TFT_CLK  9
#define TFT_MISO 10
#define TFT_LED  A2  
#define HAVE_TOUCHPAD
#define TOUCH_CS A4
#define TOUCH_IRQ 1
  
#define BEEPER 2
/*_______End of definitions______*/

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);


// Pin for measure - connected via 1MOhm resitor
#define pin_in 14      // 50 Hz IN   (TX Pin)

long N = 0;           // count of measures
int D_x = 5;          // Diagram: current x-Position
float Hz_min;         // smallest frequency since program start
float Hz_max;         // highest  frequency since program start
float Hz_old;
float Hz_min_old = 0;
float Hz_max_old = 0;
int Sound_enable = 100; // enables sound for new min/max values after a specified mesures



//---------------------------------------------------------------------------------------------------------------
void setup() {
  // init serial port for debuging
  Serial.begin(115200);

  // additional pin definitions
  pinMode(TFT_LED, OUTPUT);     // define LED pin as output for backlight control
  digitalWrite(TFT_LED, HIGH);  // HIGH to turn backlight off - will hide the display during init  
  pinMode(TOUCH_CS, OUTPUT);    // define Touch cs pin as output 
  digitalWrite(TOUCH_CS, HIGH); // HIGH to disable touch controller

  pinMode(pin_in, INPUT);       // Input Pin for grid frequency measure

  // init tft and touch
  Serial.println("Init TFT and Touch...");
  tft.begin();
  tft.setRotation(1);
  Serial.print("tftx ="); Serial.print(tft.width()); Serial.print(" tfty ="); Serial.println(tft.height());
  // draw frames
  tft.fillScreen(ILI9341_BLUE);
  tft.fillRect(4, 4, 312, 87, ILI9341_BLACK);
  tft.fillRect(4, 95, 312, 141, ILI9341_BLACK);
  // draw text Hz, min, max
  tft.setFont(&FreeMono9pt7b); 
  tft.setTextColor(ILI9341_CYAN);
  tft.setCursor(165, 63);
  tft.println("Hz");
  tft.setCursor(277, 33);
  tft.println("max");
  tft.setCursor(277, 70);
  tft.println("min");
  randomSeed(analogRead(0));
  
  
}



//--------------------------------------------------------------------------------------------------------------
void loop() 
  { 
    long z1;         // time measure start (us)
    long z2;         // time measure end (us)
    float z_diff;    // time measure end difference (us)
    float Per_ms;    // Periodendauer (ms)
    float Hz;        // Frequency
    
    float Hz_diff;   // difference to 50,000 Hz
    int Pix_diff;    // difference in pixel to zero x axis for diagram 
    
    // measure
    pulseIn(pin_in, HIGH, 40000);         // wait for log.0 
    z1 = micros();                        // --- start measure ---
    for (int i = 0; i < 100; i++)               // 
      {                                         //  measure 100 times
        pulseIn(pin_in, HIGH, 40000);           //  -> ca. 2 Sec.
      }                                         //
    z2 = micros();                         // --- end measure ---
    z_diff = z2-z1;
    Per_ms = z_diff/100000 ;     // Dauer einer Periode in ms
    Hz = 1000/Per_ms;            // Freq. in Hz
    Hz_diff = Hz-50;             // Difference to 50,000 Hz
    Pix_diff = Hz_diff*1000;      // 1 Pixel per 1 mHz with  *1000 (+-37 mHz)   1 Pixel per 2 mHz with *500 (+-74mHz) 
    if (Pix_diff < -69) Pix_diff = -70;
    if (Pix_diff > 69) Pix_diff = 70;

    if( N > 2 )                             // ab 3. Messung Min./Max. aktualisieren
    { 
      if (Hz < Hz_min) { Hz_min = Hz; }   // Hz_min aktualisieren
      if (Hz > Hz_max) { Hz_max = Hz; }   // Hz_max     "
    }
    else
    {
      Hz_min = Hz;                        // Min. Startwert zuweisen
      Hz_max = Hz;                        // Max.     "        "
    }



    // serial debug output  
    Serial.print(" N: ");         Serial.print(N);          // --- Ser. Ausgabe ---
    Serial.print("  z1: ");       Serial.print(z1);
    Serial.print("  t_diff: ");   Serial.print(z_diff);
    Serial.print("  Per_ms: ");   Serial.print(Per_ms, 3);
    Serial.print("  Hz: ");       Serial.print(Hz, 3);
    Serial.print("  Hz_min: ");   Serial.print(Hz_min, 3);  
    Serial.print("  Hz_max: ");   Serial.print(Hz_max, 3);
    Serial.print("  Hz_diff: ");  Serial.print(Hz_diff, 3);
    Serial.print("  Pix_diff: "); Serial.print(Pix_diff);
    Serial.println(" ");

    tft.setRotation(1);
    tft.setTextSize(0);
    
    
    //tft.setFont(&DSEG7_Modern_Mini_Bold_32); 
    tft.setFont(&DSEG7_Classic_Mini_Bold_36);   
    
    
    tft.setTextColor(ILI9341_BLACK);
    tft.setCursor(10, 65);
    tft.println(Hz_old,3);
    
    //tft.setTextColor(0xFFFF, 0x0000);
    tft.setTextColor(ILI9341_CYAN);
    tft.setCursor(10, 65);
    tft.println(Hz,3);
    Hz_old = Hz;
    
    
    // min and max
    tft.setFont(&DSEG7_Classic_Mini_Bold_16); 
    if (Hz_min != Hz_min_old) {
      tft.setTextColor(ILI9341_BLACK);
      tft.setCursor(203, 71);
      tft.println(Hz_min_old,3);
      tft.setTextColor(ILI9341_CYAN);
      tft.setCursor(203, 71);
      tft.println(Hz_min,3);
      Hz_min_old = Hz_min;
      if (Sound_enable == 0) tone(BEEPER,1000,100); 
    } 

    if (Hz_max != Hz_max_old) {  
      tft.setTextColor(ILI9341_BLACK);
      tft.setCursor(203, 34);
        tft.println(Hz_max_old,3);
        tft.setTextColor(ILI9341_CYAN);
        tft.setCursor(203, 34);
        tft.println(Hz_max,3);
        Hz_max_old = Hz_max;
        if (Sound_enable == 0) tone(BEEPER,1000,100);
    }

    // draw diagram
    tft.drawFastVLine(D_x + 1, 95, 140, ILI9341_GREEN); // draw marker line
    tft.drawFastVLine(D_x + 2, 95, 140, ILI9341_GREEN);

    tft.drawFastVLine(D_x, 95, 70 - Pix_diff, ILI9341_BLACK);  // draw line from value to bottom
    tft.drawFastVLine(D_x, 165 - Pix_diff, 70 + Pix_diff, ILI9341_MAGENTA);  // draw line from top to value
    tft.drawPixel(D_x, 165 - Pix_diff, ILI9341_WHITE);
    tft.drawPixel(D_x, 166 - Pix_diff, ILI9341_WHITE);

    tft.drawFastHLine(4, 165, 315, ILI9341_GREEN);  // draw zero axis (50.000Hz)
    
    N++;                                        // next measure in diagram
    
    D_x++;                                      // next line
    if (D_x > 315)                              // reached right margin ?
      { 
        tft.drawFastVLine(316, 95, 140, ILI9341_BLUE); // delete draw marker line on right margin
        tft.drawFastVLine(317, 95, 140, ILI9341_BLUE);
        D_x = 5;                                // start on most left position again  
      }

    if (Sound_enable) Sound_enable--;
    
    digitalWrite(TFT_LED, LOW);    // LOW to turn backlight on; 
  
  }
