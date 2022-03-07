//-------------------------------------------------------------------------------------

void screenInit() {
  display = Adafruit_SSD1306(128, 64, &Wire, -1,1190000,1190000);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Allocate 1136 bytes
    while (true) {
      digitalWrite(13, HIGH);
      delay(1000);
      digitalWrite(13, LOW);
      delay(1000);
    }
  }

  display.fillScreen(SSD1306_BLACK);
  display.setTextSize(1);
  display.setTextColor(SSD1306_INVERSE);

  display.drawBitmap(
    (display.width()  - LOGO_WIDTH)  / 2,
    (display.height() - LOGO_HEIGHT) / 2,
    logo, LOGO_WIDTH, LOGO_HEIGHT, 1);

  display.display();
  display.fillRect(0, 0, 128, 64, SSD1306_BLACK);
  delay(2500);
}

//-------------------------------------------------------------------------------------

void screenUpdate() {
  switch (menu) {
    case HOME:
      drawHome();
      break;
    case MENU_OPTIONS:
      drawOptions();
      break;
    case MENU_LIMIT:
      drawThrow();
      break;
    case MENU_CURVE:
      drawCurve();
      break;
    case MENU_MODE:
      drawMode();
      break;
  }
}

//-------------------------------------------------------------------------------------
