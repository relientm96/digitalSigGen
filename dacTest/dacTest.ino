#include "audio.h"

// Choose One
#define UNO_VER
//#define NANO_VER  

// UNO Shield prototype
#ifdef UNO_VER
  #define BTN_PIN       9
  #define TRIANGLE_LED  A3
  #define SQUARE_LED    A2
  #define SINE_LED      A1
#endif 

// Nano Prototype
#ifdef NANO_VER
  #define AUDIO_INPUT   A4
  #define BTN_PIN       12
  #define TRIANGLE_LED  A5
  #define SQUARE_LED    A2
  #define SINE_LED      A1
#endif

// Size of waveform data
int sizeOfSine     =  sizeof(sineTable)/sizeof(sineTable[0]);
int sizeOfSquare   =  sizeof(squareTable)/sizeof(squareTable[0]);
int sizeOfTriangle =  sizeof(triangleTable)/sizeof(triangleTable[0]);
int sizeOfAudio    =  sizeof(audio)/sizeof(audio[0]);
int sizeOfSawTooth =  sizeof(sawTooth)/sizeof(sawTooth[0]);
int sizeOfString   =  sizeof(string)/sizeof(string[0]);

// Value of analog read from pot
int val;
// Counter to go through each sample
int k = 0;
// State of button for waveform switching
byte buttonState = 1;
// State of which waveform to display
byte state = 0;
// Data to be outputted to output PORTD
byte data = 0;
// Counts number of times button has been pressed ( for debounce )
int btnPressCount = 0;

// Mapping delays to musical notes
int getToneOutput(int value){
  
  if(value < 30){
    return 20;
  }
  if (value < 40){
    return 25;
  }
  if (value < 50){
    return 30;
  }
  if (value < 60){
    return 35;
  }
  if (value < 70){
    return 40; 
  }
  if (value < 80){
    return 45;
  }
  if (value < 3){
    return 55;
  }
  else {
    return 70;
  }
}

byte flipByte(byte c){
  c = ((c>>1)&0x55)|((c<<1)&0xAA);
  c = ((c>>2)&0x33)|((c<<2)&0xCC);
  c = (c>>4) | (c<<4) ;
  return c;
}

// Returns 1 when button state changed and 0 if not changed ( debounce )
byte checkButtonPress(){
  if (digitalRead(BTN_PIN) != buttonState) { 
    //btnPressCount++;
    delay(200);
    if(digitalRead(BTN_PIN) == buttonState) {
      // Button not pressed long enough to be notify a change
      return 0;
    }
    else {
      // Button pressed long enough ( more than 1500 times ) to signify a change
      btnPressCount = 0;
      return 1;
    }
  }
  else {
    // Button has no state change
    return 0;
  }
}

void setup() {
  
  // LED outputs
  pinMode(TRIANGLE_LED, OUTPUT); 
  pinMode(SQUARE_LED, OUTPUT);
  pinMode(SINE_LED, OUTPUT);
  // Button input 
  pinMode(BTN_PIN, INPUT_PULLUP);
  
  // Set Data Direction As Output from pin 0->7
  for (int i = 0; i < 8 ; i++){
    pinMode(i,OUTPUT);
  }

  pinMode(A0, INPUT);

  // Initialize with largest value ( lowest frequency ) 
  val = 0;
}

void loop() {
  
  val = map(analogRead(A0),0,1023,0,200);
  
  if (checkButtonPress()){
    if ( state > 5 ){
      state = 0;
    }
    else {
      state++;
    }
  }
  
 if ( state == 0 ){
    digitalWrite(TRIANGLE_LED, HIGH);
    digitalWrite(SQUARE_LED, LOW);
    digitalWrite(SINE_LED, LOW);
    for (k = 0 ; k < sizeOfTriangle; k++) {
      PORTD = pgm_read_byte(&(triangleTable[k]));
      delayMicroseconds(val);
    }
  }
  else if ( state == 1 ) {
    digitalWrite(TRIANGLE_LED, LOW);
    digitalWrite(SQUARE_LED, HIGH);
    digitalWrite(SINE_LED, LOW);
    for (k = 0 ; k < sizeOfSquare; k++) {
      PORTD = pgm_read_byte(&(squareTable[k]));
      delayMicroseconds(val);
    }
  }
  else if ( state == 2 ) {
    digitalWrite(TRIANGLE_LED, LOW);
    digitalWrite(SQUARE_LED, LOW);
    digitalWrite(SINE_LED, HIGH);
    for (k = 0 ; k < sizeOfSine; k++) {
      PORTD = pgm_read_byte(&(sineTable[k]));
      delayMicroseconds(val);
    }
  }
  else if ( state == 3 ){
    digitalWrite(TRIANGLE_LED, HIGH);
    digitalWrite(SQUARE_LED, LOW);
    digitalWrite(SINE_LED, HIGH);
    for (k = 0 ; k < sizeOfSawTooth; k++) {
      PORTD = pgm_read_byte(&(sawTooth[k]));
      delayMicroseconds(val);
    }
  }
  else if ( state == 4 ){
    digitalWrite(TRIANGLE_LED, LOW);
    digitalWrite(SQUARE_LED, LOW);
    digitalWrite(SINE_LED, LOW);
    for ( k = 0; k < sizeOfString; k++){
      PORTD = pgm_read_byte(&string[k]);
      delayMicroseconds(val);
    }
  }
  else {
    digitalWrite(TRIANGLE_LED, HIGH);
    digitalWrite(SQUARE_LED, HIGH);
    digitalWrite(SINE_LED, HIGH);
    for (k = 0 ; k < sizeOfAudio; k++) {
      if (checkButtonPress()){
        if ( state > 4 ){
          state = 0;
        }
        else {
          state++;
        }
      }
      PORTD = pgm_read_byte(&(audio[k]));
      delayMicroseconds(val);
    }
  }

}




















