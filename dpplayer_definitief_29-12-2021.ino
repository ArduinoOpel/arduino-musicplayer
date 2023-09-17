#include <Ethernet.h>
#include <SPI.h>
#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include <EEPROM.h>

byte mac[] = { 0x56, 0xAD, 0xBE, 0xEF, 0xFE, 0xAD };
IPAddress serverD(192, 168, 0, 100); // domoticz

IPAddress ip(192, 168, 0, 200);
EthernetClient client;
EthernetServer server(80);
int port = 8080;

SoftwareSerial mySoftwareSerial(4, 5); // RX, TX 
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);
void zend();
void zendV();

String readString;

bool play = false;
int vol = 15;

int value;

int martijngespeeld;
int sitskegespeeld;
int marliegespeeld;

int martijnmax;
int sitskemax;
int marliemax;

String ontvangen;
String Map;
String nummer;

int index;
int oldvalue;

char c;
String d;
int E;
bool f;

unsigned int lastStringLength = d.length();

bool martijnplay;
bool sitskeplay;
bool marlieplay;

void setup() {
  Ethernet.begin(mac, ip);
  EthernetClient client;
  server.begin();
  Serial.begin(115200);
  mySoftwareSerial.begin(9600);

  martijnmax = EEPROM.read(10);
  sitskemax = EEPROM.read(11);
  marliemax = EEPROM.read(12);

  martijngespeeld = EEPROM.read(1);
  sitskegespeeld = EEPROM.read(2);
  marliegespeeld = EEPROM.read(3);

  vol = EEPROM.read(15);

  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    while (true);
  }
  myDFPlayer.setTimeOut(1500);
  myDFPlayer.enableDAC();
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
  myDFPlayer.volume(vol);  //Set volume value. From 0 to 30

  if (client.connect(serverD, 8080)) {
    client.print("GET /json.htm?type=command&param=udevice&idx=698&nvalue=0&svalue=");
    client.print("Stable");
    client.println(" HTTP/1.1"); // Part of the GET request
    client.println("Host: 192.168.0.121");
    client.println("Connection: close");
    client.println(); // Empty linen
    client.println(); // Empty line
    client.stop();    // Closing connection to server
  }

  zendV();
  zend();
}
void loop() {

  if (value == -1) {
    printDetail(myDFPlayer.readType(), myDFPlayer.read());
  }
  if (myDFPlayer.available()) {
    printDetail(myDFPlayer.readType(), myDFPlayer.read()); //Print the detail message from DFPlayer to handle different errors and states.
  }

  Serial.println("Martijn:");
  Serial.println(martijngespeeld);
  Serial.println(martijnmax);
  Serial.println("sitske:");
  Serial.println(sitskegespeeld);
  Serial.println(sitskemax);
  Serial.println("marlie:");
  Serial.println(marliegespeeld);
  Serial.println(marliemax);


  EthernetClient client = server.available();

  if (client) {
    while (client.connected()) {
      if (client.available()) {
        c = client.read();
        if (readString.length() < 100) {
          readString += c;
          //Serial.print(c);
          ontvangen = c;
          index = ontvangen.indexOf('v');
          f = true;
          if (index != -1) {
            Map = ontvangen.substring(index, '\n');
          }
          index = ontvangen.indexOf('w');
          f = true;
          if (index != -1) {
            Map = ontvangen.substring(index, '\n');
          }
          index = ontvangen.indexOf('x');
          f = true;
          if (index != -1) {
            Map = ontvangen.substring(index, '\n');
          }
          index = ontvangen.indexOf('y');
          f = true;
          if (index != -1) {
            Map = ontvangen.substring(index, '\n');
          }
          index = ontvangen.indexOf('z');
          f = true;
          if (index != -1) {
            Map = ontvangen.substring(index, '\n');
          }

          index = ontvangen.indexOf('$');
          if (index != -1) {
            nummer = ontvangen.substring(index, '\n');
            c = client.read();
            readString += c;
            d = c;
            c = client.read();
            readString += c;
            d = d + c;
            c = client.read();
            readString += c;
            d = d + c;
            Serial.println(d);
          }
          if (f == true) {
            E = d.toInt();
            if (Map == "v") {
              EEPROM.write(10, E);
              EEPROM.write(11, E);
              EEPROM.write(12, E);
            }
            if (Map == "w")
              EEPROM.write(15, E);
          }
          if (Map == "x") {
            EEPROM.write(10, E);
            martijnmax = E;
          }
          if (Map == "y") {
            EEPROM.write(11, E);
            sitskemax = E;
          }
          if (Map == "z") {
            EEPROM.write(12, E);
            marliemax = E;
          }
          martijngespeeld = EEPROM.read(1);
          sitskegespeeld = EEPROM.read(2);
          marliegespeeld = EEPROM.read(3);
          f = false;
        }
      }

      if (c == '\n') {
        delay(1);
        client.stop();
        if (readString.indexOf("Martijn_uit") > 0) {
          if (martijnplay == true) {
            myDFPlayer.pause();
            play = false;
            if (value >= martijnmax) {
              value = 1;
            }
            EEPROM.write(1, value);
            martijnplay = false;
            zend();
          }
        }
        if (readString.indexOf("Sitske_uit") > 0) {
          if (sitskeplay == true) {
            myDFPlayer.pause();
            play = false;
            if (value >= sitskemax) {
              value = 1;
            }
            EEPROM.write(2, value);
            sitskeplay = false;
            zend();
          }
        }
        if (readString.indexOf("Marlie_uit") > 0) {
          if (marlieplay == true) {
            myDFPlayer.pause();
            play = false;
            if (value >= marliemax) {
              value = 1;
            }
            EEPROM.write(3, value);
            marlieplay = false;
            zend();
          }
        }
        if (readString.indexOf("Harder") > 0) {
          if ((martijnplay == true) || (sitskeplay == true) || (marlieplay == true)) {
            if (vol <= 28) {
              vol = vol + 2;
            }
            myDFPlayer.volume(vol);  //Set volume value. From 0 to 30
            zendV();
          }
        }
        if (readString.indexOf("Zachter") > 0) {
          if ((martijnplay == true) || (sitskeplay == true) || (marlieplay == true)) {
            if (vol >= 4) {
              vol = vol - 2;
            }
            myDFPlayer.volume(vol);  //Set volume value. From 0 to 30
            zendV();
          }
        }

        if (readString.indexOf("Martijn_aan") > 0) {
          if (martijnplay == false) {
            value = EEPROM.read(1);
            value = value + 1;
            martijnplay = true;
            sitskeplay = false;
            marlieplay = false;
            zend();
            myDFPlayer.playLargeFolder(1, value);
          } else {
            if (value >= martijnmax) {
              value = 1;
              zend();
              myDFPlayer.playLargeFolder(1, value);
            } else {
              value = value + 1;
              zend();
              myDFPlayer.playLargeFolder(1, value);
            }
          }
        }
        if (readString.indexOf("Sitske_aan") > 0) {
          if (sitskeplay == false) {
            value = EEPROM.read(2);
            value = value + 1;
            martijnplay = false;
            sitskeplay = true;
            marlieplay = false;
            zend();
            myDFPlayer.playLargeFolder(2, value);
          } else {
            if (value >= sitskemax) {
              value = 1;
              myDFPlayer.playLargeFolder(2, value);
              zend();
            } else {
              value = value + 1;
              myDFPlayer.playLargeFolder(2, value);
              zend();
            }
          }
        }
        if (readString.indexOf("Marlie_aan") > 0) {
          if (marlieplay == false) {
            value = EEPROM.read(3);
            value = value + 1;
            martijnplay = false;
            sitskeplay = false;
            marlieplay = true;
            zend();
            myDFPlayer.playLargeFolder(3, value);
          } else {
            myDFPlayer.next();
            if (value >= marliemax) {
              value = 1;
              myDFPlayer.playLargeFolder(3, value);
              zend();
            } else {
              value = value + 1;
              myDFPlayer.playLargeFolder(3, value);
              zend();
            }
          }
        }
        readString = "";
      }
    }
  }
}

void zend() {
  if (client.connect(serverD, 8080)) {
    client.print("GET /json.htm?type=command&param=udevice&idx=683&nvalue=0&svalue=");
    if (martijnplay == true) {
      client.print("Martijn_speelt_nr:_");
      client.print(value);
      client.print("_van:_");
      client.print(martijnmax);
    }
    if (sitskeplay == true) {
      client.print("Sitske_speelt_nr:_");
      client.print(value);
      client.print("_van:_");
      client.print(sitskemax);
    }
    if (marlieplay == true) {
      client.print("Marlie_speelt_nr:_");
      client.print(value);
      client.print("_van:_");
      client.print(marliemax);
    }
    if (martijnplay == false && sitskeplay == false && marlieplay == false) {
      client.print("Uitgeschakelt");
    }
    client.println(" HTTP/1.1"); // Part of the GET request
    client.println("Host: 192.168.0.121");
    client.println("Connection: close");
    client.println(); // Empty linen
    client.println(); // Empty line
    client.stop();    // Closing connection to server
  }

}
void zendV() {
  if (client.connect(serverD, 8080)) {
    client.print("GET /json.htm?type=command&param=udevice&idx=686&nvalue=0&svalue=");
    client.print("Vol:_");
    client.print(vol);
    client.print("__min=2_max=30");
    client.println(" HTTP/1.1"); // Part of the GET request
    client.println("Host: 192.168.0.121");
    client.println("Connection: close");
    client.println(); // Empty linen
    client.println(); // Empty line
    client.stop();    // Closing connection to server
  }
}

void printDetail(uint8_t type, int value) {
  switch (type) {
    case DFPlayerPlayFinished:
      if (martijnplay == true) {
        if (value >= martijnmax) {
          value = 1;
        } else {
          value = value + 1;
        }
        myDFPlayer.playLargeFolder(1, value);
        zend();
      }
      if (sitskeplay == true) {
        if (value >= sitskemax) {
          value = 1;
        } else {
          value = value + 1;
        }
        myDFPlayer.playLargeFolder(2, value);
        zend();
      }
      if (marlieplay == true) {
        if (value >= marliemax) {
          value = 1;
        } else {
          value = value + 1;
        }
        myDFPlayer.playLargeFolder(3, value);
        zend();
      }
      //////////////////////////

      ;
    case TimeOut:
      if (client.connect(serverD, 8080)) {
        client.print("GET /json.htm?type=command&param=udevice&idx=698&nvalue=0&svalue=");
        client.print("Time_Out!");
        client.println(" HTTP/1.1"); // Part of the GET request
        client.println("Host: 192.168.0.121");
        client.println("Connection: close");
        client.println(); // Empty linen
        client.println(); // Empty line
        client.stop();    // Closing connection to server
      }
      break;
    case WrongStack:
      if (client.connect(serverD, 8080)) {
        client.print("GET /json.htm?type=command&param=udevice&idx=698&nvalue=0&svalue=");
        client.print("Stack_Wrong!");
        client.println(" HTTP/1.1"); // Part of the GET request
        client.println("Host: 192.168.0.121");
        client.println("Connection: close");
        client.println(); // Empty linen
        client.println(); // Empty line
        client.stop();    // Closing connection to server
      }
      break;
    case DFPlayerCardInserted:
      if (client.connect(serverD, 8080)) {
        client.print("GET /json.htm?type=command&param=udevice&idx=698&nvalue=0&svalue=");
        client.print("Card_Inserted!");
        client.println(" HTTP/1.1"); // Part of the GET request
        client.println("Host: 192.168.0.121");
        client.println("Connection: close");
        client.println(); // Empty linen
        client.println(); // Empty line
        client.stop();    // Closing connection to server
      }
      break;
    case DFPlayerCardRemoved:
      if (client.connect(serverD, 8080)) {
        client.print("GET /json.htm?type=command&param=udevice&idx=698&nvalue=0&svalue=");
        client.print("Card_Removed!");
        client.println(" HTTP/1.1"); // Part of the GET request
        client.println("Host: 192.168.0.121");
        client.println("Connection: close");
        client.println(); // Empty linen
        client.println(); // Empty line
        client.stop();    // Closing connection to server
      }
      break;
    case DFPlayerCardOnline:
      if (client.connect(serverD, 8080)) {
        client.print("GET /json.htm?type=command&param=udevice&idx=698&nvalue=0&svalue=");
        client.print("Card_Online!");
        client.println(" HTTP/1.1"); // Part of the GET request
        client.println("Host: 192.168.0.121");
        client.println("Connection: close");
        client.println(); // Empty linen
        client.println(); // Empty line
        client.stop();    // Closing connection to server
      }
      break;
    case DFPlayerError:
      if (client.connect(serverD, 8080)) {
        client.print("GET /json.htm?type=command&param=udevice&idx=698&nvalue=0&svalue=");
        client.print("DFPlayerError:");
        client.println(" HTTP/1.1"); // Part of the GET request
        client.println("Host: 192.168.0.121");
        client.println("Connection: close");
        client.println(); // Empty linen
        client.println(); // Empty line
        client.stop();    // Closing connection to server
      }
      switch (value) {
        case Busy:
          if (client.connect(serverD, 8080)) {
            client.print("GET /json.htm?type=command&param=udevice&idx=698&nvalue=0&svalue=");
            client.print("Card_not_found");
            client.println(" HTTP/1.1"); // Part of the GET request
            client.println("Host: 192.168.0.121");
            client.println("Connection: close");
            client.println(); // Empty linen
            client.println(); // Empty line
            client.stop();    // Closing connection to server
          }
          break;
        case Sleeping:
          if (client.connect(serverD, 8080)) {
            client.print("GET /json.htm?type=command&param=udevice&idx=698&nvalue=0&svalue=");
            client.print("Sleeping");
            client.println(" HTTP/1.1"); // Part of the GET request
            client.println("Host: 192.168.0.121");
            client.println("Connection: close");
            client.println(); // Empty linen
            client.println(); // Empty line
            client.stop();    // Closing connection to server
          }
          break;
        case SerialWrongStack:
          if (client.connect(serverD, 8080)) {
            client.print("GET /json.htm?type=command&param=udevice&idx=698&nvalue=0&svalue=");
            client.print("Get_Wrong_Stack");
            client.println(" HTTP/1.1"); // Part of the GET request
            client.println("Host: 192.168.0.121");
            client.println("Connection: close");
            client.println(); // Empty linen
            client.println(); // Empty line
            client.stop();    // Closing connection to server
          }
          break;
        case CheckSumNotMatch:
          if (client.connect(serverD, 8080)) {
            client.print("GET /json.htm?type=command&param=udevice&idx=698&nvalue=0&svalue=");
            client.print("Check_Sum_Not_Match");
            client.println(" HTTP/1.1"); // Part of the GET request
            client.println("Host: 192.168.0.121");
            client.println("Connection: close");
            client.println(); // Empty linen
            client.println(); // Empty line
            client.stop();    // Closing connection to server
          }
          break;
        case FileIndexOut:
          if (client.connect(serverD, 8080)) {
            client.print("GET /json.htm?type=command&param=udevice&idx=698&nvalue=0&svalue=");
            client.print("File_Index_Out_of_Bound");
            client.println(" HTTP/1.1"); // Part of the GET request
            client.println("Host: 192.168.0.121");
            client.println("Connection: close");
            client.println(); // Empty linen
            client.println(); // Empty line
            client.stop();    // Closing connection to server
          }
          break;
        case FileMismatch:
          if (client.connect(serverD, 8080)) {
            client.print("GET /json.htm?type=command&param=udevice&idx=698&nvalue=0&svalue=");
            client.print("Cannot_Find_File");
            client.println(" HTTP/1.1"); // Part of the GET request
            client.println("Host: 192.168.0.121");
            client.println("Connection: close");
            client.println(); // Empty linen
            client.println(); // Empty line
            client.stop();    // Closing connection to server
          }
          break;
        default:
          break;
      }
      break;
    default:
      break;

  }
}
