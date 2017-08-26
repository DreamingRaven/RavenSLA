// George.O OTA file
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include "./A4988.h"

const char* ssid = "*********"; // Insert your SSID
const char* password = "*******"; // Insert your password
const char* host = "Phileep";
const unsigned int localPort = 2391;
char packetBuffer[255];
char replyBuffer[] = "acknowledged";
int laserPin = 0;
int laserState = 0;
WiFiUDP udpHandle;
// Array holding all motors && assighning pins
A4988 motor[4]{ {12, 14}, {15, 13}, {2, 16}, {4, 5} };
//motor[0].setPins(14, 12); //etc...

void laserSet(int state_t)
{
  if(state_t == laserState)
  {
    // do nothing
  }
  else
  {
    if(laserState == 0)
    {
      laserState = 1;
      digitalWrite(laserPin, HIGH);
    }
    else // if pin is high
    {
      laserState = 0; // set laser state value to corresponding low
      digitalWrite(laserPin, LOW); // set laser pin to low
    }
  }
}

// function that executes instruction from encoding:
  // 1  //data.push_back("zzzz"); 4 (ab)
  // 2  //data.push_back("uuuu"); 8 up commands
  // 3  //data.push_back("dddd"); 8 down commands
  // 4  //data.push_back("xxxx"); 8 x clockwise
  // 5  //data.push_back("aaaa"); 8 x anti-clockwise
  // 6  //data.push_back("yyyy"); 8 y clockwise
  // 7  //data.push_back("bbbb"); 8 y anti-clockwise
  // 8  //data.push_back("cccc"); 4 x clock x antic
  // 9  //data.push_back("eeee"); 4 x antic x clock
  // 10 //data.push_back("ffff"); 4 y clock y antic
  // 11 //data.push_back("gggg"); 4 y antic y clock
  // 12 //data.push_back("hhhh"); 4 x clock y clock
  // 13 //data.push_back("iiii"); 4 x clock y antic
  // 14 //data.push_back("jjjj"); 4 x antic y clock
  // 15 //data.push_back("kkkk"); 4 y clock x clock
  // 16 //data.push_back("llll"); 4 y clock x antic
  // 17 //data.push_back("mmmm"); 4 y antic x clock
  // 18 //data.push_back("nnnn"); 4 y antic x antic
  // 19 //data.push_back("oooo"); 4 x clock   empty
  // 20 //data.push_back("pppp"); 4 x antic   empty
  // 21 //data.push_back("qqqq"); 4 y clock   empty
  // 22 //data.push_back("rrrr"); 4 y antic   empty
  // 23 //data.push_back("ssss"); 4   down    empty
  // 24 //data.push_back("tttt"); 4   up      empty
  // 25 //data.push_back("wwww"); 8   lasr on
  // 26 //data.push_back("vvvv"); 8   lsr off
// this function 'secretley' compares intagers :D, 'weakly typed'-hype!
void doInstruction(char &charEncoded)
{ // motor [0] & [1] will be x & y, every other motor is considered z
  switch(charEncoded)
  {
    case 'a': // x - 2 antic
      motor[0].step(0); // forced to pass by value by compiler, sorry
      motor[0].step(0);
      break;
    case 'b': // y - 2 antic
      motor[1].step(0);
      motor[1].step(0);
      break;
    case 'c': // x - clock, x - antic
      motor[0].step(1);
      motor[0].step(0);
      break;
    case 'd': // z - 2 down commands for each z axis motor
      motor[2].step(0);
      motor[3].step(0);
      motor[2].step(0);
      motor[3].step(0);
      break;
    case 'e': // x - antic, x - clock
      motor[0].step(0);
      motor[0].step(1);
      break;
    case '.':
    case ' ': // will rearange list later by usage for optimisation
      break;
    case 'f': // y - clock, y - antic
      motor[1].step(1);
      motor[1].step(0);
      break;
    case 'g': // y - antic, y - clock
      motor[1].step(0);
      motor[1].step(1);
      break;
    case 'h': // x - clock, y - clock
      motor[0].step(1);
      motor[1].step(1);
      break;
    case 'i': // x - clock, y - antic
      motor[0].step(1);
      motor[1].step(0);
      break;
    case 'j': // x - antic, y - clock
      motor[0].step(0);
      motor[1].step(1);
      break;
    case 'k': // y - clock, x - clock
      motor[1].step(1);
      motor[0].step(1);
      break;
    case 'l': // y - clock, x - antic
      motor[1].step(1);
      motor[0].step(0);
      break;
    case 'm': // y - antic, x - clock
      motor[1].step(0);
      motor[0].step(1);
      break;
    case 'n': // y - antic, x - antic
      motor[1].step(0);
      motor[0].step(0);
      break;
    case 'o': // x - clock,     empty
      motor[0].step(1);
      break;
    case 'p': // x - antic,     empty
      motor[0].step(0);
      break;
    case 'q': // y - clock,     empty
      motor[1].step(1);
      break;
    case 'r': // y - antic,     empty
      motor[1].step(0);
      break;
    case 's': // z - down,      empty
      motor[2].step(0);
      motor[3].step(0);
      break;
    case 't': // z - up,        empty
      motor[2].step(1);
      motor[3].step(1);
      break;
    case 'u': // z - up,        up
      motor[2].step(1);
      motor[3].step(1);
      motor[2].step(1);
      motor[3].step(1);
      break;
    case 'v': //     lsr off
      laserSet(0);
      break;
    case 'w': //     lasr on
      laserSet(1);
      break;
    case 'x': // x - clock, x - clock
      motor[0].step(1);
      motor[0].step(1);
      break;
    case 'y': // y - clock, y - clock
      motor[1].step(1);
      motor[1].step(1);
      break;
    case 'z': // x - antic, y - antic
      motor[0].step(0);
      motor[1].step(0);
      break;
    default:
    {
      Serial.print("not in encoding: ");
      Serial.println(charEncoded);
    }
    break;
  }
}

void setup() {
    Serial.begin(115200); // /waits for serial
    Serial.println("Booting");

    // init WiFi params
    WiFi.mode(WIFI_STA);
    WiFi.hostname(host);
    WiFi.begin(ssid, password);

    // attempt to connect to WiFi
    while (WiFi.waitForConnectResult() != WL_CONNECTED) {
      delay(3000); // prevent incidental dos-attack
      WiFi.begin(ssid, password);
      Serial.println("Could not connect. Retrying ...");
    }
    Serial.println("Connected");

    // Setting what to do if Arduino undergoes OTA update
    ArduinoOTA.setHostname(host);
    ArduinoOTA.onStart([](){
      // do something at start of OTA update
      Serial.println("Updating");
    });
    ArduinoOTA.onEnd([](){
      // do something at end of OTA update
      Serial.println("Updated");
    });
    ArduinoOTA.onError([](ota_error_t error){
      ESP.restart();
    });
    // start OTA service/ server
    ArduinoOTA.begin();
    udpHandle.begin(localPort);

    Serial.println("Setup Completed");
    Serial.println(WiFi.localIP());


    // Motor Pin Tests
    pinMode(14, OUTPUT); // Direction 200milliS
    pinMode(12, OUTPUT); // Step 1microS
    digitalWrite(12, LOW);
    pinMode(13, OUTPUT); // Direction 200milliS
    pinMode(15, OUTPUT); // Step 1microS
    digitalWrite(15, LOW);
    pinMode(0 , OUTPUT);
}

void loop() {
    // OTA update handler
    ArduinoOTA.handle();
    // Serial.println(".");
    int packetSize = udpHandle.parsePacket();
    if(packetSize)
    {
      // if packet size is not 0
      Serial.print("Packet recieved, size: ");
      Serial.print(packetSize);
      Serial.print(" from: ");
      IPAddress remoteIp = udpHandle.remoteIP();
      int remotePrt = udpHandle.remotePort();
      udpHandle.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);

      Serial.print(remoteIp);
      Serial.print(":");
      Serial.println(remotePrt);
      Serial.print("    Contents: ");
      Serial.println(packetBuffer);

      for(int i = 0; i < packetSize; ++i)
      {
        doInstruction(packetBuffer[i]);
        // handle each instruction set
      }

      // send a reply, to the IP address and port that sent us the packet we received
      udpHandle.beginPacket(udpHandle.remoteIP(), udpHandle.remotePort());
      udpHandle.write(replyBuffer);
      udpHandle.endPacket();
      Serial.print("    Reply:    ");
      Serial.println(replyBuffer);

      // read in packet buffer
    }
}
