int startup[]       = {NOTE_C6, NOTE_D6, NOTE_E6},
    button[]        = {NOTE_D5},
    trimmUp[]       = {NOTE_B3},
    trimmDown[]     = {NOTE_B2},
    battLow[]       = {NOTE_B7, NOTE_B7, NOTE_B7},
    packetsLow[]    = {NOTE_E6, NOTE_B7},
    thtEngaged[]    = {NOTE_F6, NOTE_E7},
    thtDisengaged[] = {NOTE_E7, NOTE_F6};

int startup_tempo[]       = {8, 8, 8},
    button_tempo[]        = {8},
    trimmUp_tempo[]       = {8},
    trimmDown_tempo[]     = {8},
    ac_battLow_tempo[]    = {2, 2, 2},
    ct_battLow_tempo[]    = {4, 4, 4},
    wr_packetsLow_tempo[] = {2, 2},
    er_packetsLow_tempo[] = {8, 8},
    thtEngaged_tempo[]    = {4, 4},
    thtDisengaged_tempo[] = {4, 4};

//--------------------------------------------------------------------------------------------------------

bool isBuzzerIdle(){
  return buzzer.getState() == BUZZER_IDLE;
}

//--------------------------------------------------------------------------------------------------------

void startup_tone(){
  if(!isBuzzerIdle)
    return;
    
  buzzer.playMelody(startup, startup_tempo, sizeof(startup_tempo) / sizeof(int));
}

//--------------------------------------------------------------------------------------------------------

void button_tone(){
  if(!isBuzzerIdle)
    return;
    
  buzzer.playMelody(button, button_tempo, sizeof(button_tempo) / sizeof(int));
}

//--------------------------------------------------------------------------------------------------------

void trimmUp_tone(){
  if(!isBuzzerIdle)
    return;
    
  buzzer.playMelody(trimmUp, trimmUp_tempo, sizeof(trimmUp_tempo) / sizeof(int));
}

//--------------------------------------------------------------------------------------------------------

void trimmDown_tone(){
  if(!isBuzzerIdle)
    return;
    
  buzzer.playMelody(trimmDown, trimmDown_tempo, sizeof(trimmDown_tempo) / sizeof(int));
}

//--------------------------------------------------------------------------------------------------------

void ac_battLow_tone(){
  if(!isBuzzerIdle)
    return;
    
  buzzer.playMelody(battLow, ac_battLow_tempo, sizeof(ac_battLow_tempo) / sizeof(int));
}

//--------------------------------------------------------------------------------------------------------

void ct_battLow_tone(){
  if(!isBuzzerIdle)
    return;
    
  buzzer.playMelody(battLow, ct_battLow_tempo, sizeof(ct_battLow_tempo) / sizeof(int));
}

//--------------------------------------------------------------------------------------------------------

void wr_packetsLow_tone(){
  if(!isBuzzerIdle)
    return;
    
  buzzer.playMelody(packetsLow, wr_packetsLow_tempo, sizeof(wr_packetsLow_tempo) / sizeof(int));
}

//--------------------------------------------------------------------------------------------------------

void er_packetsLow_tone(){
  if(!isBuzzerIdle)
    return;
    
  buzzer.playMelody(packetsLow, er_packetsLow_tempo, sizeof(er_packetsLow_tempo) / sizeof(int));
}

//--------------------------------------------------------------------------------------------------------

void thtEngaged_tone(){
  if(!isBuzzerIdle)
    return;
    
  buzzer.playMelody(thtEngaged, thtEngaged_tempo, sizeof(thtEngaged_tempo) / sizeof(int));
}

//--------------------------------------------------------------------------------------------------------

void thtDisengaged_tone(){
  if(!isBuzzerIdle)
    return;
    
  buzzer.playMelody(thtDisengaged, thtDisengaged_tempo, sizeof(thtDisengaged_tempo) / sizeof(int));
}

//--------------------------------------------------------------------------------------------------------
