//--------------------------------------------------------------------------------------------------------

void drawMode() {
  if (!refresh)
    return;

  refresh = false;

  display.fillScreen(SSD1306_BLACK);

  display.fillRect(0, 0, 128, 11, SSD1306_WHITE);
  display.fillRect(2, 15, 10 * (config.mode ? 6.4 : 0), 11, SSD1306_WHITE);
  display.fillRect(2, 29, 10 * (config.mode ? 0 : 6.4), 11, SSD1306_WHITE);

  display.setCursor(52, 2);
  display.print("MODE");
  display.setCursor(2, 2);
  display.print("L1");
  display.setCursor(4, 17);
  display.print("1 AIRCRAFT");
  display.setCursor(4, 31);
  display.print("2 WING");
}

//--------------------------------------------------------------------------------------------------------

void drawCurve() {
  if (!refresh)
    return;

  refresh = false;

  display.fillScreen(SSD1306_BLACK);

  display.fillRect(0, 0, 128, 11, SSD1306_WHITE);

  display.setCursor(49, 2);
  display.print("CURVE");
  display.setCursor(2, 2);
  display.print("L1");

  display.drawFastVLine( 75, 13, 50, SSD1306_WHITE);
  display.drawFastVLine(100, 13, 50, SSD1306_WHITE);
  display.drawFastVLine(125, 13, 50, SSD1306_WHITE);

  display.drawFastHLine(75, 13, 50, SSD1306_WHITE);
  display.drawFastHLine(75, 38, 50, SSD1306_WHITE);
  display.drawFastHLine(75, 63, 50, SSD1306_WHITE);

  float k;
  int8_t curvePos;

  switch (selectedOption) {
    case 0: // 1
      k = config.kRightX;
      curvePos = map(joystick.rightX, 0, ADC_MAX, -25, 25);
      display.fillRect(2, 15,  69, 11, SSD1306_WHITE);
      break;
    case 1: // 2
      k = config.kRightY;
      curvePos = map(joystick.rightY, 0, ADC_MAX, -25, 25);
      display.fillRect(2, 29,  69, 11, SSD1306_WHITE);
      break;
    case 2: // 3
      k = config.kLeftX;
      curvePos = map(joystick.leftX, 0, ADC_MAX, -25, 25);
      display.fillRect(2, 43,  69, 11, SSD1306_WHITE);
      break;
  }

  display.setCursor(4, 17);
  display.print("1 ROLL ");
  if (rx_inverted)
    display.print(" I");

  display.setCursor(4, 31);
  display.print("2 PITCH");
  if (ry_inverted)
    display.print(" I");

  display.setCursor(4, 45);
  display.print("3 YAW  ");
  if (lx_inverted)
    display.print(" I");

  display.setCursor(90, 2);
  display.print("k ");
  display.print(k);

  display.drawFastVLine(100 + curvePos, 13, 60, SSD1306_WHITE);

  for (int8_t i = -25; i < 25; i++) {
    float r = i / 25.0;
    float c = (r - k * r) / (k - 2.0 * k * abs(r) + 1.0);
    display.drawPixel(100 + i, 38 - (25 * c), SSD1306_WHITE);
  }
}

//--------------------------------------------------------------------------------------------------------

void drawThrow() {
  if (!refresh)
    return;

  refresh = false;

  display.fillScreen(SSD1306_BLACK);

  display.fillRect(0, 0, 128, 11, SSD1306_WHITE);

  display.setCursor(49, 2);
  display.print("THROW");
  display.setCursor(2, 2);
  display.print("L1");

  display.drawFastVLine( 75, 13, 50, SSD1306_WHITE);
  display.drawFastVLine(100, 36,  5, SSD1306_WHITE);
  display.drawFastVLine(125, 13, 50, SSD1306_WHITE);

  display.drawFastHLine(75, 13, 50, SSD1306_WHITE);
  display.drawFastHLine(75, 38, 50, SSD1306_WHITE);
  display.drawFastHLine(75, 63, 50, SSD1306_WHITE);

  int8_t  axis;
  uint8_t limitPos, limit;

  switch (selectedOption) {
    case 0: // RX
      limit = config.throwRightX;
      axis  = map(joystick.rightX, 0, ADC_MAX, -25, 25);
      display.fillRect(2, 15,  69, 11, SSD1306_WHITE);
      break;
    case 1: // RY
      limit = config.throwRightY;
      axis  = map(joystick.rightY, 0, ADC_MAX, -25, 25);
      display.fillRect(2, 29,  69, 11, SSD1306_WHITE);
      break;
    case 2: // LX
      limit = config.throwLeftX;
      axis  = map(joystick.leftX, 0, ADC_MAX, -25, 25);
      display.fillRect(2, 43,  69, 11, SSD1306_WHITE);
      break;
  }

  display.setCursor(4, 17);
  display.print("1 ROLL  ");
  
  display.setCursor(4, 31);
  display.print("2 PITCH ");
  
  display.setCursor(4, 45);
  display.print("3 YAW   ");
  
  limitPos = map(limit, 0, 90, 0, 25);

  display.setCursor(90, 2);
  display.print("L ");
  display.print(limit);

  display.drawFastVLine(75 + limitPos,  13, 50, SSD1306_WHITE);
  display.drawFastVLine(125 - limitPos, 13, 50, SSD1306_WHITE);
  display.drawFastVLine(100 + axis, 13, 50, SSD1306_WHITE);
}

//--------------------------------------------------------------------------------------------------------

void drawOptions() {
  if (!refresh)
    return;

  refresh = false;

  display.fillScreen(SSD1306_BLACK);

  display.fillRect(0,  0, 128, 11, SSD1306_WHITE);
  display.fillRect(2, 15,  10, 11, SSD1306_WHITE);
  display.fillRect(2, 29,  10, 11, SSD1306_WHITE);
  display.fillRect(2, 43,  10, 11, SSD1306_WHITE);

  display.setCursor(52, 2);
  display.print("MENU");
  display.setCursor(2, 2);
  display.print("L1");
  display.setCursor(4, 17);
  display.print("1 LIMIT");
  display.setCursor(4, 31);
  display.print("2 CURVE");
  display.setCursor(4, 45);
  display.print("3 MODE");
}

//--------------------------------------------------------------------------------------------------------

void drawHome() {
  if (received.pitch >  40) received.pitch =  40;
  if (received.pitch < -40) received.pitch = -40;
  if (received.roll  >  90) received.roll  =  90;
  if (received.roll  < -90) received.roll  = -90;

  int8_t x0, y0, x1, y1, x2, y2, x3, y3;

  if (received.roll > -47 && received.roll < 47) {
    x0 = x3 = 94;
    x1 = x2 = 34;
    y2 = y3 = 64;
    y0 = y1 = (sin(received.roll * 0.01745329252) * 30 / cos(received.roll * 0.01745329252));
    y0 = 32 - y0;
    y1 += 32;
  } else {
    bool isNegative = received.roll < 0;

    x0 = x1 = (cos(received.roll * 0.01745329252) * 32 / sin(received.roll * 0.01745329252));
    x0 = abs(x0) + 64;
    x1 = abs(abs(x1) - 64);
    y0 = y3 = isNegative ? 64 : 0;
    y1 = y2 = isNegative ? 0  : 64;

    x3 = x2 = isNegative ? 34 : 94;
  }

  display.fillScreen(SSD1306_BLACK);

  display.fillTriangle(x0, y0, x1, y1, x3, y3, SSD1306_WHITE);
  display.fillTriangle(x1, y1, x2, y2, x3, y3, SSD1306_WHITE);

  for (int i = 0; i < 8; i++)
    if (i != 3)
      display.drawFastHLine(54, (i + 1) * 8, 20, SSD1306_INVERSE);

  display.fillTriangle(89, 32 + config.trimRightY,
                       93, 29 + config.trimRightY,
                       93, 35 + config.trimRightY,
                       SSD1306_INVERSE);

  display.drawFastVLine(34, 0, 64, SSD1306_WHITE);
  display.drawFastVLine(94, 0, 64, SSD1306_WHITE);

  display.fillRect(59, 27 - (received.pitch * 8 / 10), 10, 10, SSD1306_INVERSE);

  display.setCursor(0, 0);
  display.print("SPD");
  display.setCursor(0, 9);
  printCoalesce(pps != 0, received.spd / 100.0f);

  if (!config.throttle) {
    display.setCursor(2, 22);
    display.fillRect(1, 21, 26, 10, SSD1306_WHITE);
    display.print("!THT");
  }

  display.setCursor(received.crs >= 100 ? 55 : (received.crs == 0 ? 55 : 58), 0);
  printCoalesce(pps != 0, received.crs);

  display.setCursor(0 , 39);
  display.drawFastHLine(0, 47, 34, SSD1306_WHITE);
  display.drawFastVLine(8, 47, 17, SSD1306_WHITE);
  display.print("PPS");

  display.setCursor(0 , 49);
  display.print("C  ");
  display.print(pps);

  display.setCursor(0 , 57);
  display.print("A  ");
  printCoalesce(pps != 0, received.pps);

  display.setCursor(100, 0);
  display.print("ALT");
  display.setCursor(100, 9);
  printCoalesce(pps != 0, received.alt / 100.0f);

  display.setCursor(103, 21);
  display.print("C");
  display.setCursor(118, 21);
  display.print("A");
  display.fillRect(100, 29, 11, 32, SSD1306_WHITE);
  display.fillRect(115, 29, 11, 32, SSD1306_WHITE);

  int c_height = mapFloat(con_v, CON_BAT_MIN, CON_BAT_MAX, 30.0, 0.0);
  int a_height = mapFloat(received.bat / 100.0, AIR_BAT_MIN, AIR_BAT_MAX, 30.0, 0.0);

  display.fillRect(101, 30, 9, c_height, SSD1306_BLACK);

  if (pps == 0 || received.bat < 600) {
    display.setCursor(118, 40);
    display.print("X");
  } else
    display.fillRect(116, 30, 9, a_height, SSD1306_BLACK);

  last_roll  = received.roll;
  last_pitch = received.pitch;
}

//--------------------------------------------------------------------------------------------------------

void printCoalesce(bool check, float val) {
  if (check)
    display.print(val,1);
  else
    display.print("---");
}

//--------------------------------------------------------------------------------------------------------

void printCoalesce(bool check, int val) {
  if (check)
    display.print(val);
  else
    display.print("---");
}

//--------------------------------------------------------------------------------------------------------
