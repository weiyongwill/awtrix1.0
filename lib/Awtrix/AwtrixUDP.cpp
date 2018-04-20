#include <AwtrixUDP.h>

int localUdpPort = 52829;
char inputBuffer[512];

void AwtrixUDP::setup(){
     Udp.begin(localUdpPort);
}

void AwtrixUDP::sendMSG(String msg){
  Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
  Udp.print(msg);
  Udp.endPacket();
}

void AwtrixUDP::loop() {
    int packetSize = Udp.parsePacket();
    if (packetSize) {
      Udp.read(inputBuffer, 512);
      sendMSG("ACK");
      if (packetSize > 0)
        {
          inputBuffer[packetSize] = 0;
        }
        String dat;
        dat = String(inputBuffer);
        String command = dat.substring(0,dat.indexOf("%"));
        String payload = dat.substring(dat.indexOf("%")+1,dat.length());

      if (command == "bri"){   
        BRIGHTNESS=payload.toInt();
        DisplayManager::getInstance().setBrightness(payload.toInt());
        return; 
      }

      if (command== "save"){
          AwtrixSettings::getInstance().saveSettings();
          return;
      }

      if (command== "reset"){
          ESP.reset();
          return;
      }

      if (command== "mood"){
          PET_MOOD=payload.toInt();
          return;
      }

      if (command== "pong"){
         PONG_ACTIVE=payload.toInt();
         return;
      }

      if (command== "snake"){
         SNAKE_ACTIVE=payload.toInt();
         return;
      }

      if (command== "pongmove"){
         paddle=payload.toInt();
         return;
      }

      if (command== "snakemove"){
         direction=payload.toInt();
         return;
      }

      if (command== "text"){
          DisplayManager::getInstance().scrollText(payload);
          return;
      }

      if (command== "next"){
         //ApplicationManager::getInstance().nextApplication() ;
         return;
      }

      if (command== "game"){
         game=payload.toInt();
         return;
      }

            if (command== "gamemode"){
         gamemode=payload.toInt();
         return;
      }

     
      if (command == "color"){
        long number = (long) strtol( &payload[1], NULL, 16);
        TEXT_COLOR_R = number >> 16;
        TEXT_COLOR_G = number >> 8 & 0xff;
        TEXT_COLOR_B = number & 0xff;
        DisplayManager::getInstance().setColor({TEXT_COLOR_R,TEXT_COLOR_G,TEXT_COLOR_B});
        return;
      }


      if (command == "settings"){
        if (payload=="get"){
           sendMSG(AwtrixSettings::getInstance().loadSettings()); 
           
        }else{
          int str_len = payload.length() + 1; 
          char char_array[str_len];
          payload.toCharArray(char_array, str_len);
          AwtrixSettings::getInstance().parseSettings(char_array);
        }
        return;
      }
  sendMSG("ACK");
    }
    delay(10);
}