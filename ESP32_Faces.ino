/***************************************************
Copyright (c) 2020 Luis Llamas
(www.luisllamas.es)

This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses
****************************************************/
/*********************************************************
M5Atom Lite + SSD1306 128x64 OLED Version by @shikarunochi
**********************************************************/

#include "M5AtomDefine.h"

#ifdef M5AtomFaces
#include <M5Atom.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define WIDTH  64
#define HEIGHT 128
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 Buffer(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#else
#include <M5StickC.h>
#define WIDTH  80
#define HEIGHT 168
TFT_eSprite Buffer = TFT_eSprite(&M5.Lcd);
#endif

#include "Face.h"

#define EYE 40

Face face(Buffer, HEIGHT, WIDTH, EYE);

void setup(void) {
	Serial.begin(115200);
#ifdef M5AtomFaces
  M5.begin(true,false,true);
  //Wire.begin(32, 26); //GROVE
  Wire.begin(21, 25); 
    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!Buffer.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }
  delay(1000);
  Buffer.setRotation(3);
  Buffer.clearDisplay();
  Buffer.display();
#else
	Buffer.setColorDepth(8);
 	Buffer.createSprite(WIDTH, HEIGHT);
#endif

	face.Expression.GoTo_Normal();

	face.Behavior.Clear();
	face.Behavior.SetEmotion(eEmotions::Normal, 1.0);

#ifndef M5AtomFaces
	M5.MPU6886.Init();
#endif
}

void loop() {
#ifdef M5AtomFaces
    face.Behavior.Clear();
    face.Behavior.SetEmotion(eEmotions::Normal, 2.0);
    face.Behavior.SetEmotion(eEmotions::Happy, 1.0);
#else
  float accX = 0;
  float accY = 0;
  float accZ = 0;
  M5.MPU6886.getAccelData(&accX, &accY, &accZ);

  if (accZ > 0.8 || accZ < -0.8)
  {
    face.Behavior.Clear();
    face.Behavior.SetEmotion(eEmotions::Sad, 1.0);
  }
  else if (accY > 0.8)
  {
    face.Behavior.Clear();
    face.Behavior.SetEmotion(eEmotions::Angry, 1.0);
  }
  else if (accY < -0.8)
  {
    face.Behavior.Clear();
    face.Behavior.SetEmotion(eEmotions::Furious, 1.0);

  }
  else
  {
    face.Behavior.Clear();
    face.Behavior.SetEmotion(eEmotions::Normal, 2.0);
    face.Behavior.SetEmotion(eEmotions::Happy, 1.0);
  }
#endif
	face.Update();
	return;
}
