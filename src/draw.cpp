#include "draw.h"

int brightness = 60;
uint loopCount = 0;

#define COLOR_DEPTH 24                  // Choose the color depth used for storing pixels in the layers: 24 or 48 (24 is good for most sketches - If the sketch uses type `rgb24` directly, COLOR_DEPTH must be 24)
const uint16_t kMatrixWidth = 64;       // Set to the width of your display, must be a multiple of 8
const uint16_t kMatrixHeight = 32;      // Set to the height of your display
const uint8_t kRefreshDepth = 36;       // Tradeoff of color quality vs refresh rate, max brightness, and RAM usage.  36 is typically good, drop down to 24 if you need to.  On Teensy, multiples of 3, up to 48: 3, 6, 9, 12, 15, 18, 21, 24, 27, 30, 33, 36, 39, 42, 45, 48.  On ESP32: 24, 36, 48
const uint8_t kDmaBufferRows = 4;       // known working: 2-4, use 2 to save RAM, more to keep from dropping frames and automatically lowering refresh rate.  (This isn't used on ESP32, leave as default)
const uint8_t kPanelType = SM_PANELTYPE_HUB75_32ROW_MOD16SCAN;   // Choose the configuration that matches your panels.  See more details in MatrixCommonHub75.h and the docs: https://github.com/pixelmatix/SmartMatrix/wiki
const uint32_t kMatrixOptions = (SM_HUB75_OPTIONS_NONE);        // see docs for options: https://github.com/pixelmatix/SmartMatrix/wiki
const uint8_t kBackgroundLayerOptions = (SM_BACKGROUND_OPTIONS_NONE);
const uint8_t kIndexedLayerOptions = (SM_INDEXED_OPTIONS_NONE);

SMARTMATRIX_ALLOCATE_BUFFERS(matrix, kMatrixWidth, kMatrixHeight, kRefreshDepth, kDmaBufferRows, kPanelType, kMatrixOptions);
SMARTMATRIX_ALLOCATE_BACKGROUND_LAYER(backgroundLayer, kMatrixWidth, kMatrixHeight, COLOR_DEPTH, kBackgroundLayerOptions);
SMARTMATRIX_ALLOCATE_INDEXED_LAYER(indexedLayer, kMatrixWidth, kMatrixHeight, COLOR_DEPTH, kIndexedLayerOptions);

// Draw::Draw() {
// }

void Draw::drawBitmap(int16_t x1, int16_t y1, const bitmap_t* bm){
  int x;
  int y;
  rgb24 *buffer;

  // Get the back buffer for the image
  buffer = backgroundLayer.backBuffer();

  // Copy the image into the buffer
  if ((bm->width <= matrix.getScreenWidth()) &&
  (bm->height <= matrix.getScreenHeight()))

  for (y = 0; y < bm->height; y++) {
    for (x = 0; x < bm->width; x++) {
      buffer[matrix.getScreenWidth() * (y + y1) + x + x1].red = bm->pixel_data[(y * bm->width + x) * 3 + 0];
      buffer[matrix.getScreenWidth() * (y + y1) + x + x1].blue = bm->pixel_data[(y * bm->width + x) * 3 + 1];
      buffer[matrix.getScreenWidth() * (y + y1) + x + x1].green = bm->pixel_data[(y * bm->width + x) * 3 + 2];
    }
  }
}

void Draw::drawCountdown(int16_t d, rgb24 textColor) {
  // Clear the countdown area
  backgroundLayer.fillRectangle(0, 21, 63, 31, {0x00, 0x00, 0x00});

  if (d > 0) {
    if (d < 7 || loopCount / 4 % 2 == 0) {
      drawDays(d, textColor);
    } else if (d == 7) {
      drawOneWeek(textColor);
    } else if (d % 7 == 0) {
      drawWeeks(d, textColor);
    } else {
      drawWeeksDays(d, textColor);
    }
  } else {
    // Past the event date. Show a message in the countdown area
    backgroundLayer.setFont(font5x7);
    backgroundLayer.drawString(2, 24, textColor, "INDIANAPOLIS");
  }

  // Show the updated display
  backgroundLayer.swapBuffers(false);

  loopCount++;
}

void Draw::drawDays(int16_t d, rgb24 textColor) {
  int16_t daysX = 0;
  char date[] = "xxx";
  char days[] = "DAYS";

  // Fill in the date string withe the number of days
  date[0] = '0' + d / 100;
  date[1] = '0' + (d / 10) % 10;
  date[2] = '0' + d % 10;

  backgroundLayer.setFont(font8x13);

  // Choose the font size based on the number of digits
  if (d == 1) {
    // Erase the 's' in 'Days' if only one day left
    days[3] = 0x00;

    backgroundLayer.drawString(15, 21, textColor, &date[2]);

    // Draw the word 'Day'
    daysX = 26;
  } else if (d <= 9) {
    backgroundLayer.drawString(12, 21, textColor, &date[2]);

    // Draw the word 'Days'
    daysX = 22;
  } else if (d <= 99) {
    backgroundLayer.drawString(6, 21, textColor, &date[1]);

    // Draw the word 'Days'
    daysX = 27;
  } else {
    backgroundLayer.drawString(3, 21, textColor, date);

    // Draw the word 'Days'
    daysX = 30;
  }
  // backgroundLayer.setFont(font3x5);
  backgroundLayer.drawString(daysX, 21, textColor, days);
}

void Draw::drawWeeks(int16_t d, rgb24 textColor) {
  int16_t daysX = 0;
  int16_t weeksX = 0;
  int16_t weeks = d / 7;
  char week[] = "WEEKS";
  char date[] = "xx";

  if (weeks < 10) {
    daysX = 3;
    weeksX = -2;
  }

  date[0] = '0' + (weeks / 10) % 10;
  date[1] = '0' + weeks % 10;
  backgroundLayer.setFont(font8x13);

  if (weeks < 10) {
    backgroundLayer.drawString(2 + daysX, 21, textColor, &date[1]);
  } else {
    backgroundLayer.drawString(2 + daysX, 21, textColor, date);
  }

  // backgroundLayer.setFont(font6x10);
  backgroundLayer.drawString(22 + weeksX, 21, textColor, week);
}

void Draw::drawOneWeek(rgb24 textColor) {
  backgroundLayer.setFont(font8x13);
  backgroundLayer.drawString(15, 21, textColor, "1");
  backgroundLayer.setFont(font6x10);
  backgroundLayer.drawString(26, 24, textColor, "WEEK");
}

void Draw::drawWeeksDays(int16_t d, rgb24 textColor) {
  int16_t daysX = 0;
  int16_t days = d % 7;
  int16_t weeks = d / 7;
  char day[] = "D";
  char week[] = "W";
  char date[] = "xx";

  if (weeks < 10) {
    daysX = -4;
  }

  date[0] = '0' + (weeks / 10) % 10;
  date[1] = '0' + weeks % 10;
  backgroundLayer.setFont(font8x13);
  if (weeks < 10) {
    backgroundLayer.drawString(18 + daysX, 21, textColor, &date[1]);
  } else {
    backgroundLayer.drawString(10 + daysX, 21, textColor, date);
  }

  backgroundLayer.setFont(font6x10);
  backgroundLayer.drawString(28 + daysX, 24, textColor, week);

  date[1] = '0' + days % 10;
  backgroundLayer.setFont(font8x13);
  backgroundLayer.drawString(39 + daysX, 21, textColor, &date[1]);
  backgroundLayer.setFont(font6x10);
  backgroundLayer.drawString(49 + daysX, 24, textColor, day);
}

void Draw::init(char* eventYear, rgb24 textColor) {
  matrix.addLayer(&backgroundLayer);
  matrix.addLayer(&indexedLayer);
  matrix.begin();
  matrix.setBrightness(brightness*(255/100));
  indexedLayer.setRotation(rotation90);
  indexedLayer.setIndexedColor(1, textColor);
  indexedLayer.setFont(font5x7);
  indexedLayer.drawString(1, 14, 1, eventYear);
  indexedLayer.swapBuffers();

  drawBitmap(13, 1, &bitmap_image);

  // Show the image (swapBuffers is really a copy unless false is passed in)
  backgroundLayer.swapBuffers(true);
}
