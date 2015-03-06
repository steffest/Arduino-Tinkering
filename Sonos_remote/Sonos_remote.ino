/**/

#include <WiServer.h>

bool useSerial = true;
bool useWifi = true;

byte volumePin = 2;    // analog input pin for the volume potentio
byte playPin = 3;  
byte presetPin = 9;
byte preset2Pin = 10;
byte preset3Pin = 4;
byte preset4Pin = 5;

byte redPin = 17;
byte greenPin = 18;
byte bluePin = 19;

int volumeSensorValue = 0;    
byte playSensorValue = 0;
byte presetSensorValue = 0;  
byte preset2SensorValue = 0;
byte preset3SensorValue = 0;
byte preset4SensorValue = 0;

byte canHttp = 1;
byte buttonTreshold = 30; // old buttons sometimes give false postives -> average input over x readings
bool isSleeping = false;

boolean RED[] = {LOW, HIGH, HIGH};
boolean GREEN[] = {HIGH, LOW, HIGH};
boolean BLUE[] = {HIGH, HIGH, LOW};
boolean OFF[] = {HIGH, HIGH, HIGH};

boolean color[] = {HIGH, HIGH, HIGH};

// Wireless configuration parameters ----------------------------------------
unsigned char local_ip[]    = {192,168,0,120};   // IP address of WiShield
unsigned char gateway_ip[]  = {192,168,0,1};   // router or gateway IP address
unsigned char subnet_mask[] = {255,255,255,0}; // subnet mask for the local network
char ssid[]                 = {"Steffest Home"};   // max 32 bytes
unsigned char security_type = 3;               // 0 - open; 1 - WEP; 2 - WPA; 3 - WPA2

// WPA/WPA2 passphrase
const prog_char security_passphrase[] PROGMEM = {"wiefiewoefie"};	// max 64 characters

// WEP 128-bit keys
unsigned char wep_keys[] = {};

// setup the wireless mode; infrastructure - connect to AP; adhoc - connect to another WiFi device
#define WIRELESS_MODE_INFRA	1
#define WIRELESS_MODE_ADHOC	2
unsigned char wireless_mode = WIRELESS_MODE_INFRA;
unsigned char ssid_len;
unsigned char security_passphrase_len;
// End of wireless configuration parameters ----------------------------------------

// Function that prints data from the server
void printData(char* data, int len) {
  
  // Print the data returned by the server
  // Note that the data is not null-terminated, may be broken up into smaller packets, and 
  // includes the HTTP header. 
  if (useSerial){
    while (len-- > 0) {
      Serial.print(*(data++));
    } 
  }
  canHttp = 1;
  setColor(GREEN);
}

// IP Address for steffest Home server  
uint8 ip[] = {192,168,0,140};

String urlCommand = "";
char ch_urlCommand[44] = "/groupVolume/10";

// The GET request
GETrequest httpGet(ip, 4044, "home.stef.be", ch_urlCommand);


void setColor(boolean* newColor) {
  color[0] = newColor[0];
  color[1] = newColor[1];
  color[2] = newColor[2];
}

void setLED(){
  digitalWrite(redPin, color[0]);
  digitalWrite(greenPin, color[1]);
  digitalWrite(bluePin, color[2]);
}

void setup() {
  
  // use the internal resistor of the "button" pins
  pinMode(playPin,INPUT_PULLUP);
  pinMode(presetPin,INPUT_PULLUP);
  pinMode(preset2Pin,INPUT_PULLUP);
  pinMode(preset3Pin,INPUT_PULLUP);
  pinMode(preset4Pin,INPUT_PULLUP);
  
  pinMode(redPin,OUTPUT);
  pinMode(greenPin,OUTPUT);
  pinMode(bluePin,OUTPUT);
     
  setColor(RED);
  setLED();
  
  if (useSerial){
    Serial.begin(57600);
    Serial.println("Connecting to Wifi ...");
  }
  
   // Initialize WiServer (we'll pass NULL for the page serving function since we don't need to serve web pages) 
  if (useWifi){
     WiServer.init(NULL);
     WiServer.enableVerboseMode(useSerial);
  }
 
  if (useSerial) Serial.println("Connection done");
  setColor(GREEN);
  setLED();

  // Have the processData function called when data is returned by the server
  httpGet.setReturnFunc(printData);
  
  delay(500);
}

byte currentVolume = 0;
byte currentPreset = 0;
int newVolume = 0;
byte changeVolume = 0;

byte isPlaying = 1;

byte initDone = 0;
int actionDelay = 0;

byte hasAction = 0;

byte counter = 0;

void loop(){
  
  if (isSleeping){
    setColor(OFF);
  }
  setLED();
  
  if (initDone == 0){
    if (useSerial) Serial.println("Init Done");
    initDone = 1;
  }
  
  volumeSensorValue = analogRead(volumePin);
  
  if (digitalRead(playPin) == 0){
    if (playSensorValue>0) playSensorValue--;
  }else{
    if (playSensorValue<buttonTreshold) playSensorValue++;
  }
  
  if (digitalRead(presetPin) == 0){
    if (presetSensorValue>0) presetSensorValue--;
  }else{
    if (presetSensorValue<buttonTreshold) presetSensorValue++;
  }
  
  if (digitalRead(preset2Pin) == 0){
    if (preset2SensorValue>0) preset2SensorValue--;
  }else{
    if (preset2SensorValue<buttonTreshold) preset2SensorValue++;
  }
  
  if (digitalRead(preset3Pin) == 0){
    if (preset3SensorValue>0) preset3SensorValue--;
  }else{
    if (preset3SensorValue<buttonTreshold) preset3SensorValue++;
  }
  
  if (digitalRead(preset4Pin) == 0){
    if (preset4SensorValue>0) preset4SensorValue--;
  }else{
    if (preset4SensorValue<buttonTreshold) preset4SensorValue++;
  }
   
  // use average to dampen spikes
  newVolume = (byte) (newVolume + (volumeSensorValue/10))/2;
  
  changeVolume = (newVolume-currentVolume);
  if (changeVolume<0)changeVolume = 0-changeVolume;
  
  
  if (actionDelay == 0){
    // no action queued
    if (changeVolume>6){
      // trigger action with a small delay to get more relevant current data (turning a knob takes some time)
      actionDelay=1;
    }else{
      
      if (isPlaying == 1 && playSensorValue == buttonTreshold){
        // pause button is pressed
        hasAction = 1;
        isPlaying = 0;
        urlCommand = "/Pause";
      }
      
      if (hasAction == 0 && isPlaying == 0 && playSensorValue == 0){
        // pause button is not pressed
        hasAction = 1;
        isPlaying = 1;
        urlCommand = "/Play";
      }
      
      if (hasAction == 0 && currentPreset != 1 && presetSensorValue == buttonTreshold){
        // preset button is pressed
        hasAction = 1;
        currentPreset = 1;
        urlCommand = "/favorite/StuBru";
        //stringOne.toCharArray(urlCommand, 44); 
      }
      
      if (hasAction == 0 && currentPreset != 2 && preset2SensorValue == buttonTreshold){
        // preset button is pressed
        hasAction = 1;
        currentPreset = 2;
        urlCommand = "/favorite/Nostalgie";
        //stringOne.toCharArray(urlCommand, 44); 
      }
      
      if (hasAction == 0 && currentPreset != 3 && preset3SensorValue == buttonTreshold){
        // preset button is pressed
        hasAction = 1;
        currentPreset = 3;
        urlCommand = "/favorite/Sterrekes";
        //stringOne.toCharArray(urlCommand, 44); 
      }
      
      if (hasAction == 0 && currentPreset != 4 && preset4SensorValue == buttonTreshold){
        // preset button is pressed
        // no action, just activate the pause/volume keys
        currentPreset = 4;
      }
      
      if (presetSensorValue == 0 && preset2SensorValue == 0 && preset3SensorValue == 0 && preset4SensorValue == 0){
        // no button pressed
        // set to off
        currentPreset = 0;
      }
      
    }
    
    if (currentPreset == 0){
        isSleeping = true;
    }else{
        if (isSleeping){
          // waking up
          setColor(GREEN);
        }
        isSleeping = false;
    }
    
  }else{
    actionDelay = actionDelay+1;
    
    if (actionDelay>30){
      // trigger action with the most current value
      actionDelay = 0;
        
      if (hasAction == 0 && changeVolume>6){
        if (newVolume>99) newVolume=99;
        currentVolume = newVolume;
        if (useSerial){
          Serial.print("Setting Volume to ");
          Serial.print(currentVolume);
          Serial.println();
        }
          
        urlCommand = "/groupVolume/";
        urlCommand = urlCommand + currentVolume;
        hasAction = 1;
      }
    }
  }
  
  if (hasAction == 1){
      if (canHttp == 0){
          if (useSerial){
             Serial.println("HTTP busy");
             Serial.println(urlCommand);
          }
          hasAction = 0; // maybe set to 0 to drop the last action?
          setColor(RED);
      }else{
          if (useSerial){
            Serial.println("Sending HTTP");
            Serial.println(urlCommand);
          }
         hasAction = 0;
         setColor(BLUE);
        
        if (useWifi && !isSleeping){
            canHttp = 0;
            urlCommand.toCharArray(ch_urlCommand, 44);  
            httpGet.submit();
        }
      } 
  }
  
  counter = counter+1;
  if (counter>200){
    if (useSerial){
      Serial.print(": ");
      Serial.print(newVolume);
      Serial.print(",");
      Serial.print(playSensorValue);
      Serial.print(",");
      Serial.print(presetSensorValue);
      Serial.print(",");
      Serial.print(preset2SensorValue);
      Serial.print(",");
      Serial.print(preset3SensorValue);
      Serial.print(",");
      Serial.print(preset4SensorValue);
      Serial.println();
    }
    counter = 0;
  }
   
  // Run WiServer
  if (useWifi){
       WiServer.server_task();
  }
  
  delay(10);
}
