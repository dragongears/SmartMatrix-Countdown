/*
 * Header File for Draw Class - Countdown SmartMatrix draw routines
 *
 * Copyright (c) 2024 Art Dahm (art@dahm.com)
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

#ifndef Draw_h
#define Draw_h

// #include "Arduino.h"
#include <stdint.h>
#include <MatrixHardware_ESP32_V0.h>
#include <SmartMatrix.h>
#include "gencon.h"


class Draw {
  public:
    // Draw();
    void drawBitmap(int16_t x1, int16_t y1, const bitmap_t* bm);
    void drawCountdown(int16_t d, rgb24 textColor);
    void drawDays(int16_t d, rgb24 textColor);
    void drawOneWeek(rgb24 textColor);
    void drawWeeks(int16_t d, rgb24 textColor);
    void drawWeeksDays(int16_t d, rgb24 textColor);
    void init(char* eventYear, rgb24 textColor);
	private:
};

#endif  /* _Draw_h */
