#define FASTLED_INTERRUPT_RETRY_COUNT 0
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <FastLED.h>
#include "Font.h"

/////////////////////////////////////////////////////////////////////////////////
// Defines & Global variables
/////////////////////////////////////////////////////////////////////////////////
#define DEBUG false
/*
 * TMP2.NET Packet structure values
 * The Packet structure is build up like followed:
 * [0] Packet start byte --> should always be 0xC9 to be a TMP2.NET packet
 * [1] Packet Type --> Can be Data frame, Command or Requested response
 * [2] Frame size high byte
 * [3] Frame size low byte
 * [4] Packet number --> So you can chain multiple packets together
 * [5] Number of packets --> So you know how many to expect
 * [6 ...] Data --> The matrix data, with max length of 1490 byte
 * [x] Packet end byte --> should always be 0x36 to end the packet
 */
#define TMP2_START_BYTE         0x9C
#define TMP2_END_BYTE           0x36
#define TMP2_DATA_FRAME         0xDA
#define TMP2_COMMAND            0xC0
#define TMP2_REQUESTED_RESPONSE 0xAA
#define TMP2_HEADER_SIZE        6
#define TMP2_LISTENING_PORT     65506
// Additional Wifi values
#define WIFINAME                "pixel_network"
#define PASSPHRASE              "pixelMadness+200"
WiFiUDP Udp;

/*
 * Led curtain global variables
 */
#define CURTAIN_LENGTH 20                                // this assumes the curtain is square, if not, sorry!!!
#define CURTAIN_SIZE   (CURTAIN_LENGTH * CURTAIN_LENGTH) // total amount of leds
#define LED_PIN        D4                                // The Pin the curtain is connected to
CRGB    leds[CURTAIN_SIZE];

/*
 * global calculated values for the script
 */
int tmpPacketSize = (CURTAIN_SIZE*3) + TMP2_HEADER_SIZE + 1; // any value lower than 1500 is safe

/*
 * Draw variables
 */
Font font; // class Font for use in drawing numbers and letters 5x5 resolution

/////////////////////////////////////////////////////////////////////////////////
// WIFI & TMP2 & UDP functions
/////////////////////////////////////////////////////////////////////////////////
void receiveTMP2Package(){
   byte packet[tmpPacketSize];
  //test if a packet has been recieved
  if (Udp.parsePacket() > 0) {
    //read-in packet and get length
    int len = Udp.read(packet, tmpPacketSize);
    Udp.flush();  //discard unread bytes
    if(len < 1){ return; } //test for empty packet
    //if(DEBUG){ Serial.println(packet[0], HEX); }
    if(packet[0] == TMP2_START_BYTE && packet[tmpPacketSize-1] == TMP2_END_BYTE){
      int pcnt = TMP2_HEADER_SIZE;
      for(int n = 0; n < CURTAIN_SIZE; n++){
        leds[n] = CRGB(packet[pcnt++], packet[pcnt++], packet[pcnt++]);
      }
      FastLED.show();
    }
  }
}

void StartWifiAndUDP(){
  int status = WL_IDLE_STATUS;
  setWifiStatusLed(status);
  WiFi.begin(WIFINAME, PASSPHRASE);
  while ( status != WL_CONNECTED) {
    delay(1000);
    if(DEBUG){
      Serial.print("Attempting to connect to network, SSID: ");
      Serial.println(WIFINAME); 
    } 
    status = WiFi.status();
    setWifiStatusLed(status);
  }
  if(DEBUG){
    Serial.print("Connection made on IP: "); 
    Serial.println(WiFi.localIP());
  }
  drawIP(); // show IP on the led screen
  Udp.begin(TMP2_LISTENING_PORT);
}

void setWifiStatusLed(int status){
  if(status == WL_IDLE_STATUS){
    leds[1] = CRGB(255,0,0);
  } else if(status ==  WL_CONNECTED){
    leds[1] = CRGB(0,255,0);
  }
  FastLED.show();
}

/////////////////////////////////////////////////////////////////////////////////
// Draw functions
/////////////////////////////////////////////////////////////////////////////////
void drawPoint(uint8_t x, uint8_t y, CRGB color){
  if(x>=0 && y>=0 && x<CURTAIN_LENGTH && y<CURTAIN_LENGTH){
    int location =  400 - (x*CURTAIN_LENGTH) + y;
    leds[location] = color;
  }
}

void drawLetter(uint8_t x, uint8_t y, char letter5, CRGB color){
  uint16_t letter = font.getLetter5(letter5);
  uint8_t  cy;
  uint8_t  cx;
  for(cy=0; cy<5;cy++){
    for(cx=0;cx<3;cx++){
      uint8_t index = 15-((cy*3)+cx+1);
      drawPoint(x+cx,y+cy, (((letter >> index) & 1) == 1)?color:CRGB(0,0,0));
    }
  }
}

void drawNumber(uint8_t num, uint8_t x, uint8_t y){
  String number = String(num);
  while(number.length() < 3){
    number = String("0" + number);
  }
  for(uint8_t i=0;i<number.length();i++){
    drawLetter((i*5)+x, y,number.charAt(i),CRGB(255,255,255));
  }
}

void drawIP(){
  IPAddress ip = WiFi.localIP();
  drawNumber(ip[3], 2, 2);
  FastLED.show();
}

/////////////////////////////////////////////////////////////////////////////////
// ARDUINO LOGIC 
/////////////////////////////////////////////////////////////////////////////////

void setupDebug(){
  if(DEBUG){
    Serial.begin(9600);
    // indicate to first led if Debug is on or off
    leds[0] = CRGB(0,255,0); // green for ON
  } else { 
    leds[0] = CRGB(0,0,255); // blue for OFF
  }
  FastLED.show();
}

void setup() {
  FastLED.addLeds<WS2812, LED_PIN, RGB>(leds, CURTAIN_SIZE);
  FastLED.clear();
  setupDebug();
  StartWifiAndUDP();
}

void loop() {
  receiveTMP2Package();
  delay(1);
}
