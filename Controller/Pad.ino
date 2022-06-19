
//-------------------------------------------------------------------------------------

void padInit() {
  config.kRightX = config.kRightY = config.kLeftX = 0.2;
  pinMode(RIGHT_TRIGGER_UP, INPUT_PULLUP);
  pinMode(RIGHT_TRIGGER_DW, INPUT_PULLUP);
  pinMode(LEFT_TRIGGER_UP,  INPUT_PULLUP);
  pinMode(LEFT_TRIGGER_DW,  INPUT_PULLUP);
  pinMode(RIGHT_JOY_BUTTON, INPUT_PULLUP);
  pinMode(LEFT_JOY_BUTTON,  INPUT_PULLUP);
  pinMode(ACTION1_BUTTON,   INPUT_PULLUP);
  pinMode(ACTION2_BUTTON,   INPUT_PULLUP);
  pinMode(ACTION3_BUTTON,   INPUT_PULLUP);
  pinMode(ACTION4_BUTTON,   INPUT_PULLUP);
  pinMode(ENCODER_CLK,      INPUT_PULLUP);
  pinMode(ENCODER_DATA,     INPUT_PULLUP);
  pinMode(SELECT_BUTTON,    INPUT_PULLUP);

  //attachInterrupt(digitalPinToInterrupt(ENCODER_CLK), encoderISR, RISING);  For the mouse whell encoder that is not working on this build
}

//-------------------------------------------------------------------------------------

void padSample() {
  buttons.select  = digitalRead(SELECT_BUTTON);
  buttons.action1 = digitalRead(ACTION1_BUTTON);
  buttons.action2 = digitalRead(ACTION2_BUTTON);
  buttons.action3 = digitalRead(ACTION3_BUTTON);
  buttons.action4 = digitalRead(ACTION4_BUTTON);

  buttons.rightTriggerUp   = digitalRead(RIGHT_TRIGGER_UP);
  buttons.rightTriggerDown = digitalRead(RIGHT_TRIGGER_DW);
  buttons.leftTriggerUp    = digitalRead(LEFT_TRIGGER_UP);
  buttons.leftTriggerDown  = digitalRead(LEFT_TRIGGER_DW);

  joystick.rightButton = digitalRead(RIGHT_JOY_BUTTON);
  joystick.leftButton  = digitalRead(LEFT_JOY_BUTTON);

  joystick.rightX = analogRead(RIGHT_JOY_X);
  joystick.rightY = analogRead(RIGHT_JOY_Y);
  joystick.leftX  = analogRead(LEFT_JOY_X);
  joystick.leftY  = analogRead(LEFT_JOY_Y);

  if (rx_inverted)
    joystick.rightX = map(joystick.rightX, 0, ADC_MAX, ADC_MAX, 0);

  if (ry_inverted)
    joystick.rightY = map(joystick.rightY, 0, ADC_MAX, ADC_MAX, 0);

  if (lx_inverted)
    joystick.leftX  = map(joystick.leftX,  0, ADC_MAX, ADC_MAX, 0);

  transmitPrep();
  events();
}

//-------------------------------------------------------------------------------------

void events() {
  if (buttons.leftTriggerUp    != buttonsOld.leftTriggerUp)    leftTriggerUpEvent();
  if (buttons.leftTriggerDown  != buttonsOld.leftTriggerDown)  leftTriggerDownEvent();
  if (buttons.rightTriggerUp   != buttonsOld.rightTriggerUp)   rightTriggerUpEvent();
  if (buttons.rightTriggerDown != buttonsOld.rightTriggerDown) rightTriggerDownEvent();
  if (buttons.action1      != buttonsOld.action1)      buttonAction1Event();
  if (buttons.action2      != buttonsOld.action2)      buttonAction2Event();
  if (buttons.action3      != buttonsOld.action3)      buttonAction3Event();
  if (buttons.action4      != buttonsOld.action4)      buttonAction4Event();
  if (joystick.leftButton  != joystickOld.leftButton)  joyLeftButtonEvent();
  if (joystick.rightButton != joystickOld.rightButton) joyRightButtonEvent();
  if (joystick.rightX      != joystickOld.rightX) joyRightXEvent();
  if (joystick.rightY      != joystickOld.rightY) joyRightYEvent();
  if (joystick.leftX       != joystickOld.leftX)  joyLeftXEvent();
  if (joystick.leftY       != joystickOld.leftY)  joyLeftYEvent();
}

//-------------------------------------------------------------------------------------

void leftTriggerDownEvent() {
  buttonsOld.leftTriggerDown = buttons.leftTriggerDown;

  if (buttons.leftTriggerDown  == 1) return;

  switch (menu) {
    case HOME:
      trimmDown_tone();

      if (config.trimRightY > -50) {
        config.trimRightY -= 2;
        refresh = true;
      }
      break;
    case MENU_OPTIONS:
      button_tone();
      menu    = HOME;
      refresh = true;
      break;
    case MENU_LIMIT:
    case MENU_CURVE:
    case MENU_MODE:
    case MENU_STATS:
      button_tone();
      selectedOption = 0;
      menu    = MENU_OPTIONS;
      refresh = true;
      break;
  }
}

//-------------------------------------------------------------------------------------

void leftTriggerUpEvent() {
  buttonsOld.leftTriggerUp = buttons.leftTriggerUp;

  if (buttons.leftTriggerUp  == 1) return;

  switch (menu) {
    case HOME:
      trimmUp_tone();

      if (config.trimRightY < 50) {
        config.trimRightY += 2;
        refresh = true;
      }
      break;
    case MENU_OPTIONS:
      button_tone();
      menu    = HOME;
      refresh = true;
      break;
    case MENU_LIMIT:
      button_tone();
      selectedOption = 0;
      menu    = MENU_OPTIONS;
      refresh = true;
      break;
    case MENU_CURVE:
      button_tone();
      menu    = MENU_OPTIONS;
      refresh = true;
      break;
    case MENU_MODE:
      button_tone();
      menu    = MENU_OPTIONS;
      refresh = true;
      break;
  }
}

//-------------------------------------------------------------------------------------

void rightTriggerDownEvent() {
  buttonsOld.rightTriggerDown = buttons.rightTriggerDown;

  if (buttons.rightTriggerDown  == 1) return;

  switch (menu) {
    case HOME:

      break;
    case MENU_OPTIONS:

      break;
    case MENU_LIMIT:
      trimmDown_tone();

      switch (selectedOption) {
        case 0:
          config.throwRightX -= config.throwRightX == THROW_MIN ? 0 : THROW_INCREMENT;
          break;
        case 1:
          config.throwRightY -= config.throwRightY == THROW_MIN ? 0 : THROW_INCREMENT;
          break;
        case 2:
          config.throwLeftX  -= config.throwLeftX  == THROW_MIN ? 0 : THROW_INCREMENT;
          break;
      }

      refresh = true;
      break;
    case MENU_CURVE:
      trimmDown_tone();

      if (selectedOption == 0)
        config.kRightX += config.kRightX == K_CURVE_MAX ? 0 : CURVE_INCREMENT;
      else if (selectedOption == 1)
        config.kRightY += config.kRightY == K_CURVE_MAX ? 0 : CURVE_INCREMENT;
      else
        config.kLeftX  += config.kLeftX  == K_CURVE_MAX ? 0 : CURVE_INCREMENT;

      refresh = true;
      break;
    case MENU_MODE:

      break;
  }
}

//-------------------------------------------------------------------------------------

void rightTriggerUpEvent() {
  buttonsOld.rightTriggerUp = buttons.rightTriggerUp;

  if (buttons.rightTriggerUp  == 1) return;

  switch (menu) {
    case HOME:

      break;
    case MENU_OPTIONS:

      break;
    case MENU_LIMIT:
      trimmUp_tone();

      switch (selectedOption) {
        case 0:
          config.throwRightX += config.throwRightX == THROW_MAX ? 0 : THROW_INCREMENT;
          break;
        case 1:
          config.throwRightY += config.throwRightY == THROW_MAX ? 0 : THROW_INCREMENT;
          break;
        case 2:
          config.throwLeftX  += config.throwLeftX  == THROW_MAX ? 0 : THROW_INCREMENT;
          break;
      }

      refresh = true;
      break;
    case MENU_CURVE:
      trimmUp_tone();

      if (selectedOption == 0)
        config.kRightX -= config.kRightX == K_CURVE_MIN ? 0 : CURVE_INCREMENT;
      else if (selectedOption == 1)
        config.kRightY -= config.kRightY == K_CURVE_MIN ? 0 : CURVE_INCREMENT;
      else
        config.kLeftX  -= config.kLeftX  == K_CURVE_MIN ? 0 : CURVE_INCREMENT;

      refresh = true;
      break;
    case MENU_MODE:

      break;
  }
}

//-------------------------------------------------------------------------------------

void buttonAction1Event() {
  buttonsOld.action1 = buttons.action1;

  if (buttons.action1  == 1) return;

  button_tone();

  switch (menu) {
    case MENU_OPTIONS:
      menu    = MENU_LIMIT;
      refresh = true;
      break;
    case MENU_MODE:
      refresh = config.mode = true;
      break;
    case MENU_LIMIT:
    case MENU_CURVE:
      refresh        = true;
      selectedOption = 0;
      break;
  }
}

//-------------------------------------------------------------------------------------

void buttonAction2Event() {
  buttonsOld.action2 = buttons.action2;

  if (buttons.action2  == 1) return;

  button_tone();

  switch (menu) {
    case MENU_OPTIONS:
      menu    = MENU_CURVE;
      refresh = true;
      break;
    case MENU_MODE:
      config.mode = false;
      refresh    = true;
      break;
    case MENU_LIMIT:
    case MENU_CURVE:
      refresh        = true;
      selectedOption = 1;
      break;
  }
}

//-------------------------------------------------------------------------------------

void buttonAction3Event() {
  buttonsOld.action3 = buttons.action3;

  if (buttons.action3  == 1) return;

  button_tone();

  switch (menu) {
    case MENU_OPTIONS:
      menu    = MENU_MODE;
      refresh = true;
      break;
    case MENU_LIMIT:
    case MENU_CURVE:
      refresh        = true;
      selectedOption = 2;
      break;
  }
}

//-------------------------------------------------------------------------------------

void buttonAction4Event() {
  buttonsOld.action4 = buttons.action4;

  if (buttons.action4  == 1) return;

  button_tone();

  switch (menu) {
    case HOME:
      menu    = MENU_OPTIONS;
      refresh = true;
      break;
    case MENU_LIMIT:
      upper   = !upper;
      refresh = true;
      break;
    case MENU_OPTIONS:
      menu    = MENU_STATS;
      refresh = true;
      break;
    case MENU_CURVE:

      switch (selectedOption) {
        case 0:
          rx_inverted = !rx_inverted;
          break;
        case 1:
          ry_inverted = !ry_inverted;
          break;
        case 2:
          lx_inverted = !lx_inverted;
          break;
      }

      refresh = true;
      break;
  }
}

//-------------------------------------------------------------------------------------

void joyLeftButtonEvent() {
  joystickOld.leftButton = joystick.leftButton;

  if (joystick.leftButton  == 1) return;

  button_tone();
}

//-------------------------------------------------------------------------------------

void joyRightButtonEvent() {
  joystickOld.rightButton = joystick.rightButton;

  if (joystick.rightButton  == 1) return;

  config.throttle = !config.throttle;

  if (config.throttle)
    thtEngaged_tone();
  else
    thtDisengaged_tone();
}

//-------------------------------------------------------------------------------------

void joyRightXEvent() {
  joystickOld.rightX = joystick.rightX;

  if (menu == MENU_LIMIT || menu == MENU_CURVE)
    refresh = true;
}

//-------------------------------------------------------------------------------------

void joyRightYEvent() {
  joystickOld.rightY = joystick.rightY;

  if (menu == MENU_LIMIT || menu == MENU_CURVE)
    refresh = true;
}

//-------------------------------------------------------------------------------------

void joyLeftXEvent() {
  joystickOld.leftX = joystick.leftX;

  if (menu == MENU_LIMIT || menu == MENU_CURVE)
    refresh = true;
}

//-------------------------------------------------------------------------------------

void joyLeftYEvent() {
  joystick.leftY = joystick.leftY;
}

//-------------------------------------------------------------------------------------

void transmitPrep() {
  float rjx = map(joystick.rightX, 0, ADC_MAX, 0, 180);
  float ljx = map(joystick.leftX,  0, ADC_MAX, 0, 180);
  float rjy = map(joystick.rightY, 0, ADC_MAX, 0, 180) + config.trimRightY;

  rjx = mapFloat(rjx, 0.0, 180.0, -1.0, 1.0);
  ljx = mapFloat(ljx, 0.0, 180.0, -1.0, 1.0);
  rjy = mapFloat(rjy, 0.0, 180.0, -1.0, 1.0);

  rjx = (rjx - (config.kRightX) * rjx) / ((config.kRightX) - 2.0 * (config.kRightX) * abs(rjx) + 1.0);
  ljx = (ljx - (config.kLeftX)  * ljx) / ((config.kLeftX)  - 2.0 * (config.kLeftX)  * abs(ljx) + 1.0);
  rjy = (rjy - (config.kRightY) * rjy) / ((config.kRightY) - 2.0 * (config.kRightY) * abs(rjy) + 1.0);

  rjx = mapFloat(rjx, -1.0, 1.0, 0.0, 180.0);
  ljx = mapFloat(ljx, -1.0, 1.0, 0.0, 180.0);
  rjy = mapFloat(rjy, -1.0, 1.0, 0.0, 180.0);

  rjx = map((int16_t) rjx, 0, 180, config.throwRightX, 180 - config.throwRightX);
  ljx = map((int16_t) ljx, 0, 180, config.throwLeftX,  180 - config.throwLeftX);
  rjy = map((int16_t) rjy, 0, 180, config.throwRightY, 180 - config.throwRightY);

  transmit.aileronLeft  = rjx;
  transmit.aileronRight = rjx;
  transmit.rudder       = ljx;
  transmit.elevator     = rjy;

  if (!config.mode) {
    int roll  = map(transmit.aileronLeft, 0, 180, -90, 90);
    int pitch = map(transmit.elevator,    0, 180, -90, 90);

    transmit.aileronLeft   = map( pitch + roll, -180, 180, 0, 180);
    transmit.aileronRight  = map(-pitch + roll, -180, 180, 0, 180);
  }

  transmit.throttle = config.throttle ? map(joystick.leftY, 0, ADC_MAX, 180, 0) : 0;
}

//-------------------------------------------------------------------------------------

float mapFloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

//-------------------------------------------------------------------------------------

int limit(int16_t value, int16_t upperLimit, int16_t lowerLimit) {
  if (value > 90 && value > 180 - upperLimit)
    return value - upperLimit;

  if (value < lowerLimit)
    return value + lowerLimit;

  return value;
}