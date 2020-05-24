/*
 * SmartMatrix Countdown
 * Version 0.1.2
 * Copyright (c) 2014 Art Dahm (art@dahm.com)
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
#include <Arduino.h>
#include <Time.h>
#include <SmartMatrix_32x32.h>
#include "bitmap.c"
//#include "bitmap_sw.c"

// Change these variables for a new countdown
tmElements_t eventDate = {0, 0, 0, 0, 3, 8, CalendarYrToTm(2021)};
char eventYear[] = "2021";

rgb24 textColor = {0xfe, 0xd7, 0x1e};
//rgb24 textColor = {0xf9, 0xff, 0xff};

time_t eventTime = makeTime(eventDate);

SmartMatrix matrix;

void setup() {
    setSyncProvider(getTeensy3Time);

    matrix.begin();
    matrix.setBrightness(128);

    int i;
    rgb24 *buffer;

    // Get the back buffer for the image
    buffer = matrix.backBuffer();

    // Copy the image into the buffer
    if ((bitmap_image.width <= matrix.getScreenWidth()) &&
    (bitmap_image.height <= matrix.getScreenHeight()))
        for (i = 0; i < matrix.getScreenWidth() * matrix.getScreenHeight(); i++) {
            buffer[i].red = bitmap_image.pixel_data[i * 3 + 0];
            buffer[i].green = bitmap_image.pixel_data[i * 3 + 1];
            buffer[i].blue = bitmap_image.pixel_data[i * 3 + 2];
        }

    // Show the image (swapBuffers is really a copy unless false is passed in)
    matrix.swapBuffers(true);
}

void loop() {
    int16_t d = 0;
    char date[] = "xxx";
    char days[] = "Days";

    // Get the number of days until the event
    d = elapsedDays(eventTime) - elapsedDays(now());

    // Clear the countdown area
    matrix.fillRectangle(0, 21, 31, 31, {0x00, 0x00, 0x00});

    if (d > 0) {
        // Fill in the date string withe the number of days
        date[0] = '0' + d / 100;
        date[1] = '0' + (d / 10) % 10;
        date[2] = '0' + d % 10;

        // Choose the font size based on the number of digits
        if (d == 1) {
		        // Erase the 's' in 'Days' if only one day left
            days[3] = 0x00;

            matrix.setFont(font8x13);
            matrix.drawString(5, 20, textColor, &date[2]);

	          // Draw the word 'Day'
            matrix.setFont(font3x5);
            matrix.drawString(15, 26, textColor, days);
        } else if (d <= 9) {
            matrix.setFont(font8x13);
            matrix.drawString(3, 20, textColor, &date[2]);

		        // Draw the word 'Days'
		        matrix.setFont(font3x5);
		        matrix.drawString(13, 26, textColor, days);
        } else if (d <= 99) {
            matrix.setFont(font6x10);
            matrix.drawString(2, 22, textColor, &date[1]);

		        // Draw the word 'Days'
		        matrix.setFont(font3x5);
		        matrix.drawString(15, 25, textColor, days);
        } else {
            matrix.setFont(font5x7);
            matrix.drawString(0, 24, textColor, date);

		        // Draw the word 'Days'
            matrix.setFont(font3x5);
		        matrix.drawString(17, 25, textColor, days);
        }
    } else {
        // Past the event date. Show the year in the countdown area
        matrix.setFont(font8x13);
        matrix.drawString(0, 20, textColor, eventYear);
    }

    // Show the updated display
    matrix.swapBuffers(false);


    // Wait before updating display
    delay(10000UL);

}

time_t getTeensy3Time() {
    return Teensy3Clock.get();
}
