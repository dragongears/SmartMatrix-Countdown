/*
 * SmartMatrix Countdown
 * Version 3.0.0
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

#include <Arduino.h>
#include "draw.h"
#include "network-time.h"
#include "gencon.h"

// Change these variables for a new countdown
tm eventDate = {0, 0, 0, 29, 7 - 1, 2025 - 1900, 0, 0, 0};
char eventYear[] = "2O25";

rgb24 textColor = {0xff, 0x3b, 0xe2};

time_t eventTime = mktime(&eventDate);

Draw draw;
NetworkTime nt;

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Countdown");

  nt.getNetworkTime();

  draw.init(eventYear, textColor);
}

void loop() {
  int16_t d = 0;

  if(!nt.getLocalTime()){
    Serial.println("Failed to obtain time");
  } else {
    // Get the number of days until the event
    d = nt.getDaysUntilEvent(eventTime);
    draw.drawCountdown(d, textColor);

    // Wait before updating display
    delay(1000UL);
  }
}
