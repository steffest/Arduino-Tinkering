/*

Controls a (TOY !) bomb ( for a game )

There are 5 coloured wires.
You have to cut 1;
One wire disarmes the bomb.
The other four make it explode.

A counter counts down to 0
When the bomb explodes the light turns red and a servo motor is activated (punching a needle into a balloon)
When the bomb is disarmed, the light turns green and a vicory song is played.

Components used: 
  - a 4 digit 7 segment LED displey (hs410561K-D30) 
  - a piezo buzzer
  - a RGB led
  - some wires



The segment code is base on
http://cdn.sparkfun.com/datasheets/Components/LED/_7Seg_Example.pde
Public Domain
  
 */

 

 #include <Servo.h> 

 Servo arm1;

int digit1 = 11; //PWM Display pin 1
int digit2 = 10; //PWM Display pin 2
int digit3 = 9; //PWM Display pin 6
int digit4 = 8; //PWM Display pin 8


int segA = 47; //Display pin 11
int segB = 48; //Display pin 16
int segC = 49; //Display pin 13
int segD = 50; //Display pin 3
int segE = 51; //Display pin 5
int segF = 52; //Display pin 11
int segG = 53; //Display pin 15

int WIRE1 = A1;
int WIRE2 = A2;
int WIRE3 = A3;
int WIRE4 = A4;
int WIRE5 = A5;

int speakerPin = 32;
int greenPin = 7;
int redPin = 6;
int bluePin = 5;

#define ARMED 1
#define EXPLODE 2
#define FREE 3

int bombState = 1;

int maxCounter = 9999;
int lastCounter = 9999;
int resetCounter = 0;
int hasExploded = 0;
int hasPlayed = 0;

#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978

int melody[] = {NOTE_C5, NOTE_D5,NOTE_E5, NOTE_A5, 0,NOTE_F5, NOTE_A5, 0};
int noteDurations[] = {8, 8, 8, 4,16,8,2,16 };

int noteCount = 10;
int doPlay = 0;

void setup() {                
  pinMode(segA, OUTPUT);
  pinMode(segB, OUTPUT);
  pinMode(segC, OUTPUT);
  pinMode(segD, OUTPUT);
  pinMode(segE, OUTPUT);
  pinMode(segF, OUTPUT);
  pinMode(segG, OUTPUT);

  pinMode(digit1, OUTPUT);
  pinMode(digit2, OUTPUT);
  pinMode(digit3, OUTPUT);
  pinMode(digit4, OUTPUT);
  
  pinMode(13, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  pinMode(speakerPin, OUTPUT);

  arm1.attach(3);  

  Serial.begin(9600);
}

void loop() {

  long startTime = millis();

  long counter = maxCounter - (millis()/100);
  if (counter<0){
    counter=0;
  }

  // beep three times at startup
  if (counter == 9998){digitalWrite(speakerPin,HIGH);}
  if (counter == 9997){digitalWrite(speakerPin,LOW);}
  if (counter == 9996){digitalWrite(speakerPin,HIGH);}
  if (counter == 9995){digitalWrite(speakerPin,LOW);}
  if (counter == 9994){digitalWrite(speakerPin,HIGH);}
  if (counter == 9993){digitalWrite(speakerPin,LOW);}

  int armPosition = 70; // off
  int armPositionActive = 200;

  int treshold = 990;
 
  if (bombState == ARMED){
      doPlay = 0; 
      lastCounter = counter;
      digitalWrite(greenPin, HIGH);
     if ((counter % 10) < 5){
      digitalWrite(bluePin, LOW);
      digitalWrite(redPin, HIGH);
      //digitalWrite(speakerPin,HIGH);
    }else{
      digitalWrite(bluePin, HIGH);
      digitalWrite(redPin, LOW);
      //digitalWrite(speakerPin,LOW);
    }

    if (analogRead(WIRE1) < treshold){bombState = EXPLODE;}
    if (analogRead(WIRE2) < treshold){bombState = EXPLODE;}
    if (analogRead(WIRE3) < treshold){bombState = EXPLODE;}
    if (analogRead(WIRE4) < treshold){bombState = FREE;}
    if (analogRead(WIRE5) < treshold){bombState = EXPLODE;}

  }

  if (bombState == EXPLODE){
      counter=0;
      digitalWrite(redPin, LOW);
      digitalWrite(greenPin, HIGH);
      digitalWrite(bluePin, HIGH);
      armPosition = armPositionActive;
      if (hasExploded == 0){
        hasExploded = 1;
        tone(speakerPin , NOTE_A7,5000);
      }
  }

  if (bombState == FREE){
      counter=lastCounter; 
      digitalWrite(greenPin, LOW);
      digitalWrite(redPin, HIGH);
      digitalWrite(bluePin, HIGH);

      // play victory song
      if (hasPlayed == 0){
        hasPlayed = 1;
        for (int thisNote = 0; thisNote < noteCount; thisNote++) {
            int noteDuration = 1000/noteDurations[thisNote];
            tone(speakerPin , melody[thisNote],noteDuration);
            int pauseBetweenNotes = noteDuration * 1.10;
            delay(pauseBetweenNotes);
            noTone(speakerPin);
        }
     
        delay(100);
        noTone(speakerPin);
      }
  }

  arm1.write(armPosition);
  displayNumber(counter);
  
  delay(8);
 
}

void displayNumber(int toDisplay) {
#define DISPLAY_BRIGHTNESS  600

#define DIGIT_ON  HIGH
#define DIGIT_OFF  LOW

  long beginTime = millis();

  for(int digit = 4 ; digit > 0 ; digit--) {

    //Turn on a digit for a short amount of time
    switch(digit) {
    case 1:
      digitalWrite(digit1, DIGIT_ON);
      break;
    case 2:
      digitalWrite(digit2, DIGIT_ON);
      break;
    case 3:
      digitalWrite(digit3, DIGIT_ON);
      break;
    case 4:
      digitalWrite(digit4, DIGIT_ON);
      break;
    }

    //Turn on the right segments for this digit
    lightNumber(toDisplay % 10);
    //lightNumber(8);
    toDisplay /= 10;

    delayMicroseconds(DISPLAY_BRIGHTNESS); 
    //Display digit for fraction of a second (1us to 5000us, 500 is pretty good)

    //Turn off all segments
    lightNumber(10); 

    //Turn off all digits
    digitalWrite(digit1, DIGIT_OFF);
    digitalWrite(digit2, DIGIT_OFF);
    digitalWrite(digit3, DIGIT_OFF);
    digitalWrite(digit4, DIGIT_OFF);
  }

}

//Given a number, turns on those segments
//If number == 10, then turn off number
void lightNumber(int numberToDisplay) {

#define SEGMENT_ON  LOW
#define SEGMENT_OFF HIGH

  switch (numberToDisplay){

  case 0:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_OFF);
    break;

  case 1:
    digitalWrite(segA, SEGMENT_OFF);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_OFF);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_OFF);
    digitalWrite(segG, SEGMENT_OFF);
    break;

  case 2:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_OFF);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_OFF);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 3:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_OFF);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 4:
    digitalWrite(segA, SEGMENT_OFF);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_OFF);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 5:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_OFF);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 6:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_OFF);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 7:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_OFF);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_OFF);
    digitalWrite(segG, SEGMENT_OFF);
    break;

  case 8:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 9:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 10:
    digitalWrite(segA, SEGMENT_OFF);
    digitalWrite(segB, SEGMENT_OFF);
    digitalWrite(segC, SEGMENT_OFF);
    digitalWrite(segD, SEGMENT_OFF);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_OFF);
    digitalWrite(segG, SEGMENT_OFF);
    break;
  }
}
