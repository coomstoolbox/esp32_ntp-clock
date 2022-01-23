#include <WiFi.h>
#include <time.h>

#include <WiFiManager.h>
#include <strings_en.h>

#include <ESPmDNS.h>
#include <ArduinoOTA.h>

const int TriggerBtn = 0;

const int NixieH1 = 13;
const int NixieH2 = 12;
const int NixieM1 = 14;
const int NixieM2 = 27;

const int D0 = 26;
const int D1 = 25;
const int D2 = 33;
const int D3 = 32;

bool wm_nonblocking = true;

WiFiManager wm;

const char* ntpServer = "ntp.nict.jp";
const long  gmtOffset_sec = 3600*9; // JST
const int   daylightOffset_sec = 0;

struct tm timeinfo;

//SemaphoreHandle_t xMutex = xSemaphoreCreateMutex();

bool gotoDivergencMeterMode = false;

bool test = false;

void printLocalTime(){
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    return;
  }
  Serial.println(&timeinfo, "%Y/%m/%d %H:%M:%S");

  int h1 = timeinfo.tm_hour / 10 % 10;
  Serial.printf("%d\n", h1);
  funcK155ID1(h1);
  if (!test) {
    digitalWrite( NixieH1, HIGH );
  } else if (timeinfo.tm_sec % 4 == 0) {
    digitalWrite( NixieH1, HIGH );
  }
  delay(5);

  digitalWrite( NixieH1, LOW );
  digitalWrite( NixieH2, LOW );
  digitalWrite( NixieM1, LOW );
  digitalWrite( NixieM2, LOW );
  delay(1);
  
  int h2 = timeinfo.tm_hour % 10;
  Serial.printf("%d\n", h2);
  funcK155ID1(h2);
  if (!test) {
    digitalWrite( NixieH2, HIGH );
  } else if (timeinfo.tm_sec % 4 == 1) {
    digitalWrite( NixieH1, HIGH );
  }
  delay(5);

  digitalWrite( NixieH1, LOW );
  digitalWrite( NixieH2, LOW );
  digitalWrite( NixieM1, LOW );
  digitalWrite( NixieM2, LOW );
  delay(1);
  
  int m1 = timeinfo.tm_min / 10 % 10;
  Serial.printf("%d\n", m1);
  funcK155ID1(m1);
  if (!test) {
    digitalWrite( NixieM1, HIGH );
  } else if (timeinfo.tm_sec % 4 == 2) {
    digitalWrite( NixieH1, HIGH );
  }
  delay(5);
  
  digitalWrite( NixieH1, LOW );
  digitalWrite( NixieH2, LOW );
  digitalWrite( NixieM1, LOW );
  digitalWrite( NixieM2, LOW );
  delay(1);

  int m2 = timeinfo.tm_min % 10;
  Serial.printf("%d\n", m2);
  funcK155ID1(m2);
  if (!test) {
    digitalWrite( NixieM2, HIGH );
  } else if (timeinfo.tm_sec % 4 == 3) {
    digitalWrite( NixieH1, HIGH );
  }
  delay(5);
  
  digitalWrite( NixieH1, LOW );
  digitalWrite( NixieH2, LOW );
  digitalWrite( NixieM1, LOW );
  digitalWrite( NixieM2, LOW );
  delay(1);
}

void divergencMeter(){
  int h1 = random(10);
  funcK155ID1(h1);
  if (!test) {
    digitalWrite( NixieH1, HIGH );
  } else if (timeinfo.tm_sec % 4 == 0) {
    digitalWrite( NixieH1, HIGH );
  }
  delay(5);

  digitalWrite( NixieH1, LOW );
  digitalWrite( NixieH2, LOW );
  digitalWrite( NixieM1, LOW );
  digitalWrite( NixieM2, LOW );
  delay(1);
  
  int h2 = random(10);
  funcK155ID1(h2);
  if (!test) {
    digitalWrite( NixieH2, HIGH );
  } else if (timeinfo.tm_sec % 4 == 1) {
    digitalWrite( NixieH1, HIGH );
  }
  delay(5);

  digitalWrite( NixieH1, LOW );
  digitalWrite( NixieH2, LOW );
  digitalWrite( NixieM1, LOW );
  digitalWrite( NixieM2, LOW );
  delay(1);
  
  int m1 = random(10);
  funcK155ID1(m1);
  if (!test) {
    digitalWrite( NixieM1, HIGH );
  } else if (timeinfo.tm_sec % 4 == 2) {
    digitalWrite( NixieH1, HIGH );
  }
  delay(5);
  
  digitalWrite( NixieH1, LOW );
  digitalWrite( NixieH2, LOW );
  digitalWrite( NixieM1, LOW );
  digitalWrite( NixieM2, LOW );
  delay(1);

  int m2 = random(10);
  funcK155ID1(m2);
  if (!test) {
    digitalWrite( NixieM2, HIGH );
  } else if (timeinfo.tm_sec % 4 == 3) {
    digitalWrite( NixieH1, HIGH );
  }
  delay(5);
  
  digitalWrite( NixieH1, LOW );
  digitalWrite( NixieH2, LOW );
  digitalWrite( NixieM1, LOW );
  digitalWrite( NixieM2, LOW );
  delay(1);
}

void initOTA(){
    ArduinoOTA
  .onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";
    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  })
  .onEnd([]() {
    Serial.println("\nEnd");
  })
  .onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  })
  .onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
}

void setup(){
  pinMode( NixieH1, OUTPUT );
  digitalWrite( NixieH1, LOW );
  pinMode( NixieH2, OUTPUT );
  digitalWrite( NixieH2, LOW );
  pinMode( NixieM1, OUTPUT );
  digitalWrite( NixieM1, LOW );
  pinMode( NixieM2, OUTPUT );
  digitalWrite( NixieM2, LOW );

  pinMode( D0, OUTPUT );
  digitalWrite( D0, LOW );
  pinMode( D1, OUTPUT );
  digitalWrite( D1, LOW );
  pinMode( D2, OUTPUT );
  digitalWrite( D2, LOW );
  pinMode( D3, OUTPUT );
  digitalWrite( D3, LOW );

  gotoDivergencMeterMode = true;

  xTaskCreatePinnedToCore(nixieloop, "nixieloopTask", 4096, NULL, 1, NULL, 1);

  pinMode(TriggerBtn, INPUT);

  Serial.begin(115200);
  //Serial.setDebugOutput(true);  
  delay(3000);
  Serial.println("\n Starting");

  WiFi.mode(WIFI_STA);
  wm.setConfigPortalBlocking(false);
  std::vector<const char *> menu = {"wifi","sep","info","restart"};
  wm.setMenu(menu);
  wm.setConfigPortalTimeout(45);
  bool res = wm.autoConnect("NtpClock");
  if(!res) {
    Serial.println("Failed to connect or hit timeout");
  } else {
    Serial.println("connected");
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    gotoDivergencMeterMode = false;
  }

  // arduino OTA
  initOTA();
}

void checkTriggerBtn(){
  // check for button press
  if ( digitalRead(TriggerBtn) == LOW ) {
    // poor mans debounce/press-hold, code not ideal for production
    delay(50);
    if( digitalRead(TriggerBtn) == LOW ){
      Serial.println("Button Pressed");
      gotoDivergencMeterMode = true;
      // still holding button for 3000 ms, reset settings, code not ideaa for production
      delay(3000); // reset delay hold
      if( digitalRead(TriggerBtn) == LOW ){
        Serial.println("Button Held");
        Serial.println("Erasing Config, restarting");
        wm.resetSettings();
        ESP.restart();
      }
      
      // start portal w delay
      Serial.println("Starting config portal");
      wm.setConfigPortalTimeout(120);
      
      if (!wm.startConfigPortal("NtpClock")) {
        Serial.println("failed to connect or hit timeout");
        delay(3000);
      } else {
        Serial.println("connected");
      }
      configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
      gotoDivergencMeterMode = false;
    }
  }
}

void loop(){
  if(wm_nonblocking) wm.process(); // avoid delays() in loop when non-blocking and other long running code  
  checkTriggerBtn();
  
  ArduinoOTA.handle();
  // put your main code here, to run repeatedly:
}

void nixieloop( void* arg ){
  while(1) {
    if (!gotoDivergencMeterMode) {
      printLocalTime();
    } else {
      divergencMeter();
    }
  }
}

//const TickType_t xTicksToWait = 1000UL;

// K155ID1ドライバICのインプットを制御
//
// VIH High-level input Min 2v
// VIL Low-level input Max 0.8v
//
//   8 [|1 U16|] 0
//   9 [|2  15|] 1
//   A [|3  14|] 5
//   D [|4  13|] 4
// Vcc [|5  12|] GND
//   B [|6  11|] 6
//   C [|7  10|] 7
//   2 [|8   9|] 3
//
// INPUT  |OUTPUT 
// D|C|B|A| ON
// L|L|L|L|  0
// L|L|L|H|  1
// L|L|H|L|  2
// L|L|H|H|  3
// L|H|L|L|  4
// L|H|L|H|  5
// L|H|H|L|  6
// L|H|H|H|  7
// H|L|L|L|  8
// H|L|L|H|  9
//
// H - high level, L - low level
//
// D0 connect A
// D1 connect B
// D2 connect C
// D3 connect D
//
void funcK155ID1(int outputnum )
{
  int i = outputnum % 10;
//  xSemaphoreGive(xMutex);
//  BaseType_t xStatus = xSemaphoreTake(xMutex, xTicksToWait);
//  if (xStatus == pdTRUE) {
    if (i >> 0 & 0b0001) {
      digitalWrite( D0, HIGH );    
    } else {
      digitalWrite( D0, LOW );
    }
    if (i >> 1 & 0b0001) {
      digitalWrite( D1, HIGH );    
    } else {
      digitalWrite( D1, LOW );
    }
    if (i >> 2 & 0b0001) {
      digitalWrite( D2, HIGH );    
    } else {
      digitalWrite( D2, LOW );
    }
    if (i >> 3 & 0b0001) {
      digitalWrite( D3, HIGH );    
    } else {
      digitalWrite( D3, LOW );
    }
//  } else {
//    // if error
//  }
//  xSemaphoreGive(xMutex);
  return;
}
