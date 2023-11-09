#include <M5Stack.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>

//#include <iostream>
#include <ctime>
#include "time.h"
/*
#include "time.h"
#include <ArduinoJson.h>
#include <string>
#include <stdio.h>

//#include <string.h> 
*/
const char* ssid = "Mads 2.4g";
const char* password = "123456Admin";
String BaseUrl = "https://raspberrypimads.local/";
// put function declarations here:
int myFunction(int, int);
int GetDB(int adv);
#define M5STACKFIRE_MICROPHONE_PIN 34
int16_t count = 0;
String HtmlGetRequest(String url);
void HtmlPUTRequest(String url);
void printLocalTime();

const char* rootCACertificate = \
"-----BEGIN CERTIFICATE-----\n" \
"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\n" \
"0Q==\n" \
"-----END CERTIFICATE-----\n";

const char* ntpServer = "0.pool.ntp.org"; 
const long gmtOffset_sec  = 3600;
const int daylightOffset_sec = 0;








void setup() {
  // put your setup code here, to run once:
  //int result = myFunction(2, 3);
    int16_t cnt = 0;
    bool timedout = false;
    int16_t maxtrys = 100;
    M5.begin();        // Init M5Core.  初始化 M5Core
    M5.Power.begin();  // Init Power module.  初始化电源模块
    M5.Lcd.fillScreen(WHITE); 
    M5.Lcd.setTextSize(4);
    WiFi.disconnect();
    delay(500);
    WiFi.begin(ssid,password);
    delay(1000);

    //M5.Speaker;

    

    while (WiFi.status()!=WL_CONNECTED and timedout == false )
    {
      
      
      cnt++;
      M5.Lcd.clear(YELLOW);
      M5.Lcd.setCursor(0,0);
      M5.Lcd.println("Venter");
      delay(200);  
      M5.Lcd.println("Venter.");
      delay(200); 
      M5.Lcd.println("Venter..");
      delay(200); 
      M5.Lcd.println("Venter...");
      delay(200); 
      M5.Lcd.println("Venter....");
      delay(200); 
      M5.Lcd.println(cnt);
      delay(500); 
      if (cnt == maxtrys)
      {
        timedout = true;
      }      
    }

    if(timedout == true)
    {
      M5.Lcd.println("Timed Out ");
    }
    else
    {
    M5.Lcd.setTextColor(YELLOW);
    M5.Lcd.clear(BLACK);
    M5.Lcd.setCursor(0,0);
    M5.Lcd.println("Connected: ");
    M5.Lcd.println(WiFi.localIP());
    struct tm timeinfo;
    M5.Lcd.println(timeinfo.tm_hour);
    int micValue = analogRead(M5STACKFIRE_MICROPHONE_PIN);
    M5.Lcd.println(micValue);
    }

   printLocalTime();
   delay(10000); 
}

void loop() {
 

   M5.Lcd.clear(BLACK);

   String Data_Noise_Points = HtmlGetRequest("Data_Noise_Points/GetAll");
   M5.Lcd.setCursor(0,0);

   M5.Lcd.println("Done");
   
   delay(1000);
   double micValue = analogRead(M5STACKFIRE_MICROPHONE_PIN);

   //DateTime.toISOString().c_str();
   M5.Lcd.setCursor(0,0);
   delay(1000);
   M5.Lcd.clear(WHITE);
   printf ("Decimals: %d %ld\n", micValue);

  configTime(gmtOffset_sec, daylightOffset_sec,ntpServer);
 
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {  // Return 1 when the time is successfully
                                     // obtained.  成功获取到时间返回1
        M5.Lcd.println("Failed to obtain time");
        return;
    }
    //"%F %H:%M:%S"
    //M5.Lcd.println(&timeinfo,"%A, %B %d \n%Y %H:%M:%S"); 
    M5.Lcd.println(&timeinfo,"%F %H:%M:%S"); 
    //M5.Lcd.println(&timeinfo);                                      
   String timestring =(&timeinfo,"%F %H:%M:%S");

   char buf[1024];
   int NoiseTime = 1234;
   int EnhedId = 1;
   int Data_Noise_PointsId = 1;
   String Alarm_On_Of = "false";
        //https://raspberrypimads.local/Data_Noise/Create/21,2023-11-01 13:53:46,1,1,true
        //https://localhost:7049/Data_Noise/Create/30,2023-11-01%2013%3A53%3A46,1,1,false' 
   
   
   
  //sprintf(buf,"Data_Noise/Create/%.2f,%d-%d-%d %d:%d:%d,%d,%d,%s",micValue/50,(timeinfo.tm_year+1900),timeinfo.tm_mon,timeinfo.tm_mday,timeinfo.tm_hour,timeinfo.tm_min,timeinfo.tm_sec,EnhedId,Data_Noise_PointsId,Alarm_On_Of);
  sprintf(buf,"Data_Noise/Create/%.2f,%d,%d,%s",micValue/50,EnhedId,Data_Noise_PointsId,Alarm_On_Of);
  

   M5.Lcd.setCursor(0,0);
  //  delay(100000);
   M5.Lcd.clear(BLACK);
   M5.Lcd.println(buf);
   Serial.println(buf);
   delay(5000);

   HtmlPUTRequest(buf);

   delay(6000000);
}

void printLocalTime() {  // Output current time.  输出当前时间
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {  // Return 1 when the time is successfully
                                     // obtained.  成功获取到时间返回1
        M5.Lcd.println("Failed to obtain time");
        return;
    }
    //"%F %H:%M:%S"
    //M5.Lcd.println(&timeinfo,"%A, %B %d \n%Y %H:%M:%S"); 
    M5.Lcd.println(&timeinfo,"%F %H:%M:%S"); 
    //M5.Lcd.println(&timeinfo);                                      
    String timestring =(&timeinfo,"%F %H:%M:%S");
}



String HtmlGetRequest(String url) {
  WiFiClientSecure *client = new WiFiClientSecure;
  String payload = "Na";
  if(client) {
    // set secure client with certificate
    client->setCACert(rootCACertificate);
    //create an HTTPClient instance
    HTTPClient https;
    
    //Initializing an HTTPS communication using the secure client
    M5.Lcd.print("[HTTPS] begin...\n");
    if (https.begin(*client, BaseUrl+url)) {  // HTTPS
      M5.Lcd.print("[HTTPS] GET...\n url: "+BaseUrl+url);
      // start connection and send HTTP header
      int httpCode = https.GET();
      // httpCode will be negative on error
      if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
       M5.Lcd.printf("[HTTPS] GET... code: %d\n", httpCode);
      // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          // print server response payload
          payload = https.getString();
          //M5.Lcd.println(payload);
        }
      }
      else {
        M5.Lcd.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
      }
      https.end();
    }
  }
  else {
    M5.Lcd.printf("[HTTPS] Unable to connect\n");
  }
  M5.Lcd.println();
  M5.Lcd.println("Waiting 2min before the next round...");
  //delay(120000);  
  return payload;
}
void HtmlPUTRequest(String url) {
  M5.Lcd.setCursor(0,0);   
  M5.Lcd.clear(BLACK);
  M5.Lcd.setTextColor(WHITE);

  WiFiClientSecure *client = new WiFiClientSecure;
  if(client) {
    // set secure client with certificate
    client->setCACert(rootCACertificate);
    //create an HTTPClient instance
    HTTPClient https;
    //Initializing an HTTPS communication using the secure client
    //M5.Lcd.print("[HTTPS] begin...\n");
    //  M5.Lcd.println(BaseUrl+url);
    if (https.begin(*client, BaseUrl+url)) {  // HTTPS
      //M5.Lcd.print("[HTTPS] GET...\n");
      // start connection and send HTTP header
      //int httpCode = https.GET();
      int httpCode = https.PUT(BaseUrl+url);
      // httpCode will be negative on error
      if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
       M5.Lcd.printf("[HTTPS] Put... code: %d\n", httpCode);
      // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          // print server response payload
          String payload = https.getString();
          //M5.Lcd.println(payload);
        }
      }
      else {
        M5.Lcd.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
      }
      https.end();
    }
  }
  else {
    M5.Lcd.printf("[HTTPS] Unable to connect\n");
  }
  M5.Lcd.println();
  M5.Lcd.println("Waiting 2min before the next round...");
  delay(120000);  
}
