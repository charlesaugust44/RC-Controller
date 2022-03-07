#include <Arduino.h>

//-------------------------------------------------------------------------------------

struct Buttons {
  bool action1      = true,
       action2      = true,
       action3      = true,
       action4      = true,
       select       = true,
       rightTriggerUp   = true,
       rightTriggerDown = true,
       leftTriggerUp    = true,
       leftTriggerDown  = true;
       
};

//-------------------------------------------------------------------------------------

struct Joystick {
  uint16_t leftX, leftY, rightX, rightY;
  bool     leftButton  = true,
           rightButton = true;
};

//-------------------------------------------------------------------------------------

struct Config {
  int8_t  trimRightY;
  float   kRightX,
          kRightY,
          kLeftX;
  uint8_t throwLeftX,
          throwRightX,
          throwRightY;
  bool    mode     = true,  // True - aircraft         False - Wing
          throttle = false; // True - Enable Throttle  False - Disable Throttle
};

//-------------------------------------------------------------------------------------

struct TransmitPacket {
  uint8_t aileronRight, aileronLeft, rudder, elevator, throttle;
};

//-------------------------------------------------------------------------------------

struct ReceivePacket {
  int16_t pps   = -1,
          roll  = -1,
          pitch = -1,
          lat   = -1,
          lon   = -1,
          alt   = -1,
          crs   = -1,
          spd   = -1,
          bat   = -1;
  unsigned long age;
};

//-------------------------------------------------------------------------------------
