// Sweep
// by BARRAGAN <http://barraganstudio.com> 
// This example code is in the public domain.


#include <Servo.h> 
 
Servo rotator2;  
Servo rotator1;  
Servo shaker; 
Servo rattler;  
Servo bassdrum1;
Servo bassdrum2;
Servo arm1;
Servo arm2;
 
int stepduration = 70; // miliseconds of eacht "beat" 125 = 500 miliseconds for a beat = 120 bpm
                        // 110 bpm: (60000/110)/4 = 136 (110,29 bpm)
                        // 100 bpm: (60000/100)/4 = 150
                        // 130 ms ->  115,38



int inByte; 
int inCount;
int patternstep;
int patternpos;

#define PATTERNLENGTH 32
#define PATTERNSTEPS 16 // must be <= PATTERNLENGTH / 2
char pattern1[PATTERNLENGTH+1];
char pattern2[PATTERNLENGTH+1];
char pattern3[PATTERNLENGTH+1];
char pattern4[PATTERNLENGTH+1];

char b;
char b1;
char b2;
char b3;
char b4;
char b5;
char b6;
char b7;
char b8;

String pattern1codes = "+13579bdf";
String pattern2codes = "+2367abef";
String pattern3codes = "+4567cdef";
String pattern4codes = "+89abcdef";
String bs = "-";
 
void setup() 
{ 
  rotator1.attach(12);   
  shaker.attach(11);
  rotator2.attach(10);  
  bassdrum1.attach(9); 
  bassdrum2.attach(8);    
  arm1.attach(7);  
  arm2.attach(6); 
  rattler.attach(5); 

  Serial.begin(28800);  
  
  patternstep=1;
  patternpos=0;
  
  inCount = 0;
  while (inCount < PATTERNLENGTH){
      pattern1[inCount] = '-';
      inCount++;
   }
  pattern1[inCount] = 0;  // null terminate the string

} 

// pattern scheme (a bit stupid but oh well ...)
//1  0  1  0  1  0  1  0  1  0  1  0  1  0  1  
//0  1  1  0  0  1  1  0  0  1  1  0  0  1  1  
//0  0  0  1  1  1  1  0  0  0  0  1  1  1  1  
//0  0  0  0  0  0  0  1  1  1  1  1  1  1  1  

//1  2  3  4  5  6  7  8  9  a  b  c  d  e  f 
 
 
void loop() 
{ 
  // track 1-2 tristate
  if (Serial.available() > 0) {
    inCount = 0;
    while ((Serial.available()) && (inCount < PATTERNSTEPS)){
       pattern1[inCount] = Serial.read();
       inCount++;
    }
    pattern1[inCount] = 0;  // null terminate the string
  }
  
  // track 3-4 tristate
  if (Serial.available() > 0) {
    inCount = 0;
    while ((Serial.available()) && (inCount < PATTERNSTEPS)){
       pattern2[inCount] = Serial.read();
       inCount++;
    }
    pattern2[inCount] = 0;  // null terminate the string
  }
  
  // track 5-8
  if (Serial.available() > 0) {
    inCount = 0;
    while ((Serial.available()) && (inCount < PATTERNSTEPS)){
       pattern3[inCount] = Serial.read();
       inCount++;
    }
    pattern3[inCount] = 0;  // null terminate the string
  }
  
  patternpos = 0;
  
  while (patternpos < PATTERNSTEPS){
    
    // track 1-2 tristate
    b = pattern1[patternpos];
    bs.setCharAt(0,b);
 
    b1 = '-';
    if (pattern1codes.indexOf(bs)>0){b1 = 'x';}
    if (pattern2codes.indexOf(bs)>0){b1 = 'y';}

    b2 = '-';
    if (pattern3codes.indexOf(bs)>0){b2 = 'x';}
    if (pattern4codes.indexOf(bs)>0){b2 = 'y';}
    
    
    // track 3-4 tristate
    b = pattern2[patternpos];
    bs.setCharAt(0,b);    
    b3 = '-';
    if (pattern1codes.indexOf(bs)>0){b3 = 'x';}
    if (pattern2codes.indexOf(bs)>0){b3 = 'y';}
    
    
    b4 = '-';
    if (pattern3codes.indexOf(bs)>0){b4 = 'x';}
    if (pattern4codes.indexOf(bs)>0){b4 = 'y';}
    
     // track 5-8
     b = pattern3[patternpos];
     bs.setCharAt(0,b);
     
     b5 = '-';
     if (pattern1codes.indexOf(bs)>0){b5 = 'x';}
     b6 = '-';
     if (pattern2codes.indexOf(bs)>0){b6 = 'x';}
     b7 = '-';
     if (pattern3codes.indexOf(bs)>0){b7 = 'x';}

     
     if (pattern4codes.indexOf(bs)>0){
        b8 = 'x';
     }else{
      if ((b8 == 'x')){
        // shaker is shaking
        b8 = 'y';
      }else{
        b8 = '-';
      }
     }
   

    
    if (b1 == 'x') {bassdrum1.write(90);}
    if (b1 == 'y') {bassdrum1.write(110);}
    
    if (b2 == 'x') {bassdrum2.write(90);}
    if (b2 == 'y') {bassdrum2.write(110);}
    
    if (b3 == 'x') {rotator1.write(70);}
    if (b3 == '-') {rotator1.write(85);}
    if (b3 == 'y') {rotator1.write(98);}
    
    if (b4 == 'x') {rotator2.write(80);}
    if (b4 == '-') {rotator2.write(100);}
    if (b4 == 'y') {rotator2.write(120);}
    
    if (b5 == 'x') {arm1.write(70);}
    if (b6 == 'x') {arm2.write(130);}
    
    if (b7 == 'x') {rattler.write(65);}
    if (b7 == '-') {rattler.write(50);}
    
    if (b8 == 'x') {shaker.write(0);}
    if (b8 == 'y') {shaker.write(180);}
    if (b8 == '-') {shaker.write(95);}
        
   
    delay(stepduration);
    
    // reset states for "sticks"
    // shakers and rotators remain in position
      
    if (b1 == 'x') {bassdrum1.write(100);}
    if (b1 == 'y') {bassdrum1.write(100);}
    
    if (b2 == 'x') {bassdrum2.write(100);}
    if (b2 == 'y') {bassdrum2.write(100);}
    
    if (b5 == 'x') {arm1.write(100);}
    if (b6 == 'x') {arm2.write(100);}
    //if (b8 == 'x') {rotator.write(20);}
 
    delay(stepduration);
   
    // clear pattern
    //pattern1[patternpos]=0;
    patternpos++;
  
  }
  
 
} 
