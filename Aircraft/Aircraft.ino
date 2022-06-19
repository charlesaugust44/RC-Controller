#define RF_CSN 8
#define RF_CE  7
#define SG_LEFT_AILERON  5
#define SG_RIGHT_AILERON 6
#define SG_ELEVATOR      9
#define SG_RUDDER        10
#define SG_THROTTLE      3

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include <Wire.h>
#include <MPU6050.h>
#include <KalmanFilter.h>
#include <Adafruit_BMP085.h>

//#define BENCHMARK

#ifdef BENCHMARK
unsigned long l1, l2, l3, l4;
#endif

Servo throttle;
Servo left_aileron;
Servo right_aileron;
Servo rudder;
Servo elevator;

typedef struct {
  uint8_t aileronRight, aileronLeft, rudder, elevator, throttle;
} Receive_packet;

typedef struct {
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
} Transmit_packet;

Transmit_packet transmit;
Receive_packet  received;

unsigned long  last_received = 0,
               last_print    = 0,
               last_pps      = 0,
               last_bmp      = 0;

RF24           radio(RF_CE, RF_CSN);
TinyGPS        gps;

MPU6050 mpu;
Adafruit_BMP085  bmp;

KalmanFilter kalmanX(0.001, 0.003, 0.03);
KalmanFilter kalmanY(0.001, 0.003, 0.03);

bool  receiving = false,
      mpuActive = false,
      bmpActive = false;
uint16_t packets = 0;
float    qnh     = 102700.0;
float    altZero = 0;

const byte ac_address[5] = {0xAA, 0xBB, 0xCC, 0xDD, 0xDD};
const byte ct_address[5] = {0x00, 0xBB, 0xCC, 0xDD, 0xEE};

//--------------------------------------------------------------------------------------------------

void setup() {

  Serial.begin(115200);
  Serial.println("Starting Aircraft...");

  transmit.pps = 0;
  transmit.lat = 99.99;
  transmit.lon = 99.99;
  transmit.age = 99999;

  throttle.attach(SG_THROTTLE, 1000, 2000);
  right_aileron.attach(SG_RIGHT_AILERON);
  left_aileron.attach(SG_LEFT_AILERON);
  rudder.attach(SG_RUDDER);
  elevator.attach(SG_ELEVATOR);

  reset_controls();
  
  mpuActive = mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G);
  //bmpActive = bmp.begin();

  if (!mpuActive)
    Serial.println("MPU FAIL");
  else {
    Serial.println("MPU OK");
    mpu.calibrateGyro();
  }

  if (!bmpActive)
    Serial.println("BMP FAIL");
  else
    Serial.println("BMP OK");

  radio.begin();
  radio.openWritingPipe(ac_address);
  radio.openReadingPipe(1, ct_address);
  radio.setPALevel(RF24_PA_MAX);
  radio.setAutoAck(false);
  radio.startListening();
}

//--------------------------------------------------------------------------------------------------

void loop() {
#ifdef BENCHMARK
  l1 = millis();
#endif
  if (radio.available())
    receive();

  if (millis() - last_received > 800)
    reset_controls();

  if (millis() - last_pps > 500) {    
    transmit.pps = (packets * 1000) / (millis() - last_pps);
    packets = 0;
    last_pps = millis();
  }


#ifdef BENCHMARK
  l2 = millis();
#endif
  transmit.bat = analogRead(A1) + 33;



#ifdef BENCHMARK
  l3 = millis();
#endif
  if (mpuActive)
    mpu_read();

  if (bmpActive && millis() - last_bmp > 70)
  {
    transmit.alt = (44330 * (1.0 - pow(bmp.readPressure() / qnh, 0.1903))) - altZero;
    transmit.alt = transmit.alt  < 0 ? 0 : transmit.alt;

    if (altZero == 0)
      altZero = transmit.alt;
  }



#ifdef BENCHMARK
  log("ddd", (int)(l2 - l1), (int)(l3 - l2), (int)(millis() - l3));
#endif
}

//--------------------------------------------------------------------------------------------------

void log(char* format, ...) {
  va_list args;
  va_start(args, format);

  while (*format != '\0') {
    int    d;
    char   c;
    float  f;
    String s;

    switch (*format) {
      case 'd':
        d = va_arg(args, int);
        Serial.print(d);
        break;
      case 'c':
        c = va_arg(args, char);
        Serial.print(c);
        break;
      case 'f':
        f = va_arg(args, double);
        Serial.print(f);
        break;
      case 's':
        do {
          s = va_arg(args, char);
          Serial.print(s);
        } while (s != '\0');
        break;
    }
    Serial.print(" ");
    format++;
  }
  Serial.println();
  va_end(args);
}

//--------------------------------------------------------------------------------------------------

void receive()
{
  radio.read(&received, sizeof(received));
  packets++;

  delay(2);

  radio.stopListening();
  radio.write(&transmit, sizeof(transmit));
  radio.startListening();

  apply_controls();
  last_received = millis();
  receiving = true;
}

//--------------------------------------------------------------------------------------------------

void apply_controls() {
  throttle.write(received.throttle);
  left_aileron.write(received.aileronLeft);
  right_aileron.write(received.aileronRight);
  rudder.write(received.rudder);
  elevator.write(received.elevator);
}

//--------------------------------------------------------------------------------------------------

void reset_controls() {
  receiving = false;

  received.throttle     = 0;
  received.aileronLeft  = map(received.aileronLeft,   0, 180, 180, 0);
  received.aileronRight = map(received.aileronRight,  0, 180, 180, 0);
  received.rudder       = map(received.rudder,        0, 180, 180, 0);
  received.elevator     = map(received.elevator,      0, 180, 180, 0);

  apply_controls();
  delay(800);

  received.aileronLeft  = 90;
  received.aileronRight = 90;
  received.rudder       = 90;
  received.elevator     = 90;

  apply_controls();
}

//--------------------------------------------------------------------------------------------------

void mpu_read() {
  Vector acc = mpu.readNormalizeAccel();
  Vector gyr = mpu.readNormalizeGyro();

  float accPitch = -(atan2(acc.XAxis, sqrt(acc.YAxis * acc.YAxis + acc.ZAxis * acc.ZAxis)) * 180.0) / M_PI;
  float accRoll  =  (atan2(acc.YAxis, acc.ZAxis) * 180.0) / M_PI;

  transmit.pitch = kalmanY.update(accPitch, gyr.YAxis);
  transmit.roll  = kalmanX.update(accRoll, gyr.XAxis);
}

//--------------------------------------------------------------------------------------------------
/*
void gps_read() {
  if (!Serial.available())
    return;

  bool newData = false;

  while (Serial.available()) {
    if (gps.encode(Serial.read()))
      newData = true;
  }

  if (newData) {
    gps.f_get_position(&transmit.lat, &transmit.lon, &transmit.age);
    transmit.alt = gps.f_altitude();
    transmit.crs = gps.f_course();
    transmit.spd = gps.f_speed_kmph();
  }
}*/
