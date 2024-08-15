void drawDays(int16_t d) {
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

void drawWeeksDays(int16_t d) {
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

void drawWeeks(int16_t d) {
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

void drawOneWeek() {
  backgroundLayer.setFont(font8x13);
  backgroundLayer.drawString(15, 21, textColor, "1");
  backgroundLayer.setFont(font6x10);
  backgroundLayer.drawString(26, 24, textColor, "WEEK");
}

void drawBitmap(int16_t x1, int16_t y1, const bitmap_t* bm) {
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
