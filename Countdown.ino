/*
 * SmartMatrix Countdown
 * Version 2.0.0
 * Copyright (c) 2014-2021 Art Dahm (art@dahm.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include <Wire.h>
#include <TimeLib.h>
#include <RTClib.h>
#include <MatrixHardware_ESP32_V0.h>
#include <SmartMatrix.h>
#include "gencon.c"
//#include "bitmap_sw.c"

#define COLOR_DEPTH 24                  // Choose the color depth used for storing pixels in the layers: 24 or 48 (24 is good for most sketches - If the sketch uses type `rgb24` directly, COLOR_DEPTH must be 24)
const uint16_t kMatrixWidth = 32;       // Set to the width of your display, must be a multiple of 8
const uint16_t kMatrixHeight = 32;      // Set to the height of your display
const uint8_t kRefreshDepth = 36;       // Tradeoff of color quality vs refresh rate, max brightness, and RAM usage.  36 is typically good, drop down to 24 if you need to.  On Teensy, multiples of 3, up to 48: 3, 6, 9, 12, 15, 18, 21, 24, 27, 30, 33, 36, 39, 42, 45, 48.  On ESP32: 24, 36, 48
const uint8_t kDmaBufferRows = 4;       // known working: 2-4, use 2 to save RAM, more to keep from dropping frames and automatically lowering refresh rate.  (This isn't used on ESP32, leave as default)
const uint8_t kPanelType = SM_PANELTYPE_HUB75_32ROW_MOD16SCAN;   // Choose the configuration that matches your panels.  See more details in MatrixCommonHub75.h and the docs: https://github.com/pixelmatix/SmartMatrix/wiki
const uint32_t kMatrixOptions = (SM_HUB75_OPTIONS_NONE);        // see docs for options: https://github.com/pixelmatix/SmartMatrix/wiki
const uint8_t kBackgroundLayerOptions = (SM_BACKGROUND_OPTIONS_NONE);

SMARTMATRIX_ALLOCATE_BUFFERS(matrix, kMatrixWidth, kMatrixHeight, kRefreshDepth, kDmaBufferRows, kPanelType, kMatrixOptions);
SMARTMATRIX_ALLOCATE_BACKGROUND_LAYER(backgroundLayer, kMatrixWidth, kMatrixHeight, COLOR_DEPTH, kBackgroundLayerOptions);

int brightness = 60;

RTC_DS1307 rtc;


// Change these variables for a new countdown
tmElements_t eventDate = {0, 0, 0, 0, 2, 8, CalendarYrToTm(2022)};
char eventYear[] = "2022";

rgb24 textColor = {0xff, 0xe2, 0x3b};
//rgb24 textColor = {0xf9, 0xff, 0xff};

time_t eventTime = makeTime(eventDate);

// SmartMatrix matrix;

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Countdown");

    // setSyncProvider(getTeensy3Time);

  Wire.begin(14, 13);
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

  matrix.addLayer(&backgroundLayer);
  matrix.begin();
  matrix.setBrightness(brightness*(255/100));

    int i;
    rgb24 *buffer;

    // Get the back buffer for the image
    buffer = backgroundLayer.backBuffer();

    // Copy the image into the buffer
    if ((bitmap_image.width <= matrix.getScreenWidth()) &&
    (bitmap_image.height <= matrix.getScreenHeight()))
        for (i = 0; i < matrix.getScreenWidth() * matrix.getScreenHeight(); i++) {
            buffer[i].red = bitmap_image.pixel_data[i * 3 + 0];
            buffer[i].green = bitmap_image.pixel_data[i * 3 + 1];
            buffer[i].blue = bitmap_image.pixel_data[i * 3 + 2];
        }

    // Show the image (swapBuffers is really a copy unless false is passed in)
    backgroundLayer.swapBuffers(true);
}

void loop() {
    int16_t d = 0;
    char date[] = "xxx";
    char days[] = "Days";

    // Get the number of days until the event
    DateTime now = rtc.now();
    d = elapsedDays(eventTime) - elapsedDays(now.unixtime());

    // Clear the countdown area
    backgroundLayer.fillRectangle(0, 21, 31, 31, {0x00, 0x00, 0x00});

    if (d > 0) {
        // Fill in the date string withe the number of days
        date[0] = '0' + d / 100;
        date[1] = '0' + (d / 10) % 10;
        date[2] = '0' + d % 10;

        // Choose the font size based on the number of digits
        if (d == 1) {
		        // Erase the 's' in 'Days' if only one day left
            days[3] = 0x00;

            backgroundLayer.setFont(font8x13);
            backgroundLayer.drawString(5, 20, textColor, &date[2]);

	          // Draw the word 'Day'
            backgroundLayer.setFont(font3x5);
            backgroundLayer.drawString(15, 26, textColor, days);
        } else if (d <= 9) {
            backgroundLayer.setFont(font8x13);
            backgroundLayer.drawString(3, 20, textColor, &date[2]);

		        // Draw the word 'Days'
		        backgroundLayer.setFont(font3x5);
		        backgroundLayer.drawString(13, 26, textColor, days);
        } else if (d <= 99) {
            backgroundLayer.setFont(font6x10);
            backgroundLayer.drawString(2, 22, textColor, &date[1]);

		        // Draw the word 'Days'
		        backgroundLayer.setFont(font3x5);
		        backgroundLayer.drawString(15, 25, textColor, days);
        } else {
            backgroundLayer.setFont(font5x7);
            backgroundLayer.drawString(0, 24, textColor, date);

		        // Draw the word 'Days'
            backgroundLayer.setFont(font3x5);
		        backgroundLayer.drawString(17, 25, textColor, days);
        }
    } else {
        // Past the event date. Show the year in the countdown area
        backgroundLayer.setFont(font8x13);
        backgroundLayer.drawString(0, 20, textColor, eventYear);
    }

    // Show the updated display
    backgroundLayer.swapBuffers(false);


    // Wait before updating display
    delay(10000UL);

}
