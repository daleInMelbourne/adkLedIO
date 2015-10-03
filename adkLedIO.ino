#include "Arduino.h"
#include "variant.h"
#include <ADK.h>
//#include "Audio.h"
#include <KeyboardController.h>

ADK L;
USBHost usb;
KeyboardController keyboard(usb);



// ADK1 usb accessory strings
#define ACCESSORY_STRING_VENDOR "Google, Inc."
#define ACCESSORY_STRING_NAME   "DemoKit"
#define ACCESSORY_STRING_LONGNAME "ADK2012 Arduino Due Board"
#define ACCESSORY_STRING_VERSION  "1.0"
#define ACCESSORY_STRING_URL    "http://www.android.com"
#define ACCESSORY_STRING_SERIAL "0000000012345678"

void adkPutchar(char c){Serial.write(c);}
extern "C" void dbgPrintf(const char *, ... );


int led = 13;

struct SendBuf {
  void Reset() { pos = 0; memset(buf, 0, sizeof(buf)); }
  void Append(int val) { buf[pos++] = val; }
  void Append(uint8_t val) { buf[pos++] = val; }
  void Append(uint16_t val) { buf[pos++] = val >> 8; buf[pos++] = val; }
  void Append(uint32_t val) { buf[pos++] = val >> 24; buf[pos++] = val >> 16; buf[pos++] = val >> 8; buf[pos++] = val; }

  int Send() { return L.accessorySend(buf, pos); }

  uint8_t buf[128];
  int pos;
};

// Attach keyboard controller to USB
//KeyboardController keyboard(usb);

// This function intercepts key press
void keyPressed() {
  Serial.print("Pressed:  ");
  printKey();
}

// This function intercepts key release
void keyReleased() {
  Serial.print("Released: ");
  printKey();
}

void printKey() {
  // getOemKey() returns the OEM-code associated with the key
  Serial.print(" key:");
  Serial.print(keyboard.getOemKey());

  // getModifiers() returns a bits field with the modifiers-keys
  int mod = keyboard.getModifiers();
  Serial.print(" mod:");
  Serial.print(mod);

  Serial.print(" => ");

  if (mod & LeftCtrl)
    Serial.print("L-Ctrl ");
  if (mod & LeftShift)
    Serial.print("L-Shift ");
  if (mod & Alt)
    Serial.print("Alt ");
  if (mod & LeftCmd)
    Serial.print("L-Cmd ");
  if (mod & RightCtrl)
    Serial.print("R-Ctrl ");
  if (mod & RightShift)
    Serial.print("R-Shift ");
  if (mod & AltGr)
    Serial.print("AltGr ");
  if (mod & RightCmd)
    Serial.print("R-Cmd ");

  // getKey() returns the ASCII translation of OEM key
  // combined with modifiers.
  Serial.write(keyboard.getKey());
  Serial.println();
}

void setup() {
  
  Serial.begin(115200);

  L.adkSetPutchar(adkPutchar);
  L.adkInit();
  
  // set the old accessory strings
  L.usbSetAccessoryStringVendor(ACCESSORY_STRING_VENDOR);
  L.usbSetAccessoryStringName(ACCESSORY_STRING_NAME);
  L.usbSetAccessoryStringLongname(ACCESSORY_STRING_LONGNAME);
  L.usbSetAccessoryStringVersion(ACCESSORY_STRING_VERSION);
  L.usbSetAccessoryStringUrl(ACCESSORY_STRING_URL);
  L.usbSetAccessoryStringSerial(ACCESSORY_STRING_SERIAL);
  
  L.usbStart();

    
   pinMode(led, OUTPUT);
    //Indicate start of program
    digitalWrite(led, LOW);
    delay(500);
    digitalWrite(led, HIGH);
    for(int i = 0; i <= 10; i++){
        digitalWrite(led, HIGH);
        delay(100);
        digitalWrite(led, LOW);
        delay(100);
    }
}


void loop() {
  char helloWorld[] = "Hello World! \r\n";
    
  uint8_t buf[128];

  static int last_report = 0;
  static uint16_t temp = 0;
  static uint16_t light = 0;
  static uint8_t buttons = 0;
  static uint8_t joyx = 0;
  static uint8_t joyy = 0;
  static uint8_t ledrgb[6][3];

  int now = millis() / 1000;
  usb.Task();
  // see if we need to report our current status
  if (now != last_report) {
    SendBuf buf;
    buf.Reset();
/*
    // temperature
    buf.Append(0x04);
    buf.Append(10);
    temp++;

    // light<
    buf.Append(0x05);
    buf.Append(25);
    light++;
*/
    // buttons
    for (int i = 0; i < 5; i++) {
      buf.Append(0x01);
      buf.Append(i);
      buf.Append(35);
      //buf.Append((uint8_t)(buttons & (1 << i)));
    }
    buttons++;

    // joystick
/*    buf.Append(0x06);
    buf.Append(67);
//    buf.Append((uint8_t)(L.capSenseSlider() - 128));
    buf.Append(0);
    joyx += 2;
    joyy += 3;
*/

printf("xdfgdfxgsdfggggggg   %c", (char)buf.buf[1]);
    buf.Send();
  }

  last_report = now;
  
  /*
  if (L.accessoryConnected()) {
      
    int res = L.accessoryReceive(buf, sizeof(buf));
    
   if (res > 0) {
     
     //L.accessorySend((uint8_t *)helloWorld, strlen(helloWorld));
         
      for (uint32_t i = 0; i < res; ++i)
      {
        printf("%c", (char)buf[i]);
        analogWrite(led, buf[i]);
      }
                   
                                     
      printf("\r\n");
      
    }
    
  }*/
  // put your main code here, to run repeatedly:
L.adkEventProcess(); //let the adk framework do its thing

}
