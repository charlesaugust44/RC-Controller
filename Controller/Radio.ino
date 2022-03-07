//-------------------------------------------------------------------------------------

void radioInit() {
  pps           = 0;
  packets       = 0;
  last_received = 0;
  last_pps      = 0;

  transmit.aileronRight = 90;
  transmit.aileronLeft  = 90;
  transmit.rudder       = 90;
  transmit.elevator     = 90;
  transmit.throttle     = 0;

  radio.begin();
  radio.openWritingPipe(ct_address);
  radio.openReadingPipe(1, ac_address);
#ifdef NO_BATTERY
  radio.setPALevel(RF24_PA_MIN);
#else
  radio.setPALevel(RF24_PA_MAX);
#endif
  radio.setAutoAck(false);
  radio.startListening();
}

//-------------------------------------------------------------------------------------

void radioReceive() {
  if (radio.available()) {
    radio.read(&received, sizeof(received));
    last_received = millis();
    packets++;
  }  

  if (millis() - last_received > 1000)
    pps = 0;

  if (millis() - last_pps > 500) {
    pps = (float)(packets * 1000) / (float)(millis() - last_pps);

    packets = 0;
    last_pps = millis();
  }
}

//-------------------------------------------------------------------------------------

void radioTransmit() {
  radio.stopListening();
  radio.write(&transmit, sizeof(transmit));
  radio.startListening();
}

//-------------------------------------------------------------------------------------
