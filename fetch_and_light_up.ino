
#include <M5StickCPlus.h>
#include <ArduinoJson.h>
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>

WiFiMulti wifiMulti;
HTTPClient http;

const char* ssid = "my ssid";     # UPDATE
const char* password = "my pwd";  # UPDATE

//API_BASE_URL = "https://api.kaiterra.com/v1/"
//API_KEY = "my key"              # UPDATE
//DEVICE = "my device"            # UPDATE

String URL = "https://api.kaiterra.com/v1/lasereggs/DEVICE_HERE?key=KEY_HERE";  # UPDATE

const int RPM10C_MODERATE = 55;
const int RPM10C_UNHEALTHY = 255;
const int RPM10C_VUNHEALTHY = 355;
const int RPM10C_HAZARDOUS = 425;

const int RPM25C_MODERATE = 12;
const int RPM25C_UNHEALTHY = 55;
const int RPM25C_VUNHEALTHY = 150;
const int RPM25C_HAZARDOUS = 250;

const int RTVOC_MODERATE = 220;
const int RTVOC_UNHEALTHY = 2200;
const int RTVOC_VUNHEALTHY = 3300;
const int RTVOC_HAZARDOUS = 4400;
     

void setup() {
    M5.begin();
    M5.Lcd.setRotation(3);              // Rotate the screen.
    WiFi.begin(ssid, password);
    M5.Lcd.print("Connecting");
    while(WiFi.status() != WL_CONNECTED) {
      delay(500);
      M5.Lcd.print(".");
    }
    M5.Lcd.print("!!CONNECTED!!\n");

}

void loop() {

  http.begin(URL.c_str());
    int httpResponseCode = http.GET();

    M5.Lcd.print("Http response code");

    String payload = "";
    
    if (httpResponseCode>0) {
        M5.Lcd.print("HTTP Response code: ");
        M5.Lcd.println(httpResponseCode);
        payload = http.getString();
        M5.Lcd.println(payload);
      }
      else {
        M5.Lcd.print("Error code: ");
        M5.Lcd.println(httpResponseCode);
      }

//    JSON auto-generated from https://arduinojson.org/v6/assistant/#/step1
    StaticJsonDocument<384> doc;

    DeserializationError error = deserializeJson(doc, payload);

    if (error) {
      M5.Lcd.print("deserializeJson() failed: ");
      M5.Lcd.println(error.c_str());
      return;
    }
    
    const char* id = doc["id"]; 

    const char* info_aqi_ts = doc["info.aqi"]["ts"]; // "2023-04-05T16:19:12Z"
    
    JsonObject info_aqi_data = doc["info.aqi"]["data"];
    float info_aqi_data_humidity = info_aqi_data["humidity"]; // 61.18
    int info_aqi_data_km200_rpm10c = info_aqi_data["km200.rpm10c"]; // 3
    int info_aqi_data_km200_rpm25c = info_aqi_data["km200.rpm25c"]; // 3
    int info_aqi_data_km203_rtvoc = info_aqi_data["km203.rtvoc"]; // 103
    int info_aqi_data_rco2_ppm_ = info_aqi_data["rco2 (ppm)"]; // 418
    int info_aqi_data_temp = info_aqi_data["temp"]; // 12

     if (info_aqi_data_km200_rpm10c > RPM10C_HAZARDOUS){
        m5.Lcd.fillScreen(RED);
     }else if (info_aqi_data_km200_rpm25c > RPM25C_HAZARDOUS){
        m5.Lcd.fillScreen(RED);
     }else if (info_aqi_data_km203_rtvoc > RTVOC_HAZARDOUS){
        m5.Lcd.fillScreen(RED);
     }
     else if (info_aqi_data_km200_rpm10c > RPM10C_VUNHEALTHY){
        m5.Lcd.fillScreen(ORANGE);
     }else if (info_aqi_data_km200_rpm25c > RPM25C_VUNHEALTHY){
        m5.Lcd.fillScreen(ORANGE);
     }else if (info_aqi_data_km203_rtvoc > RTVOC_VUNHEALTHY){
        m5.Lcd.fillScreen(ORANGE);
     }
     else if (info_aqi_data_km200_rpm10c > RPM10C_UNHEALTHY){
        m5.Lcd.fillScreen(YELLOW);
     }else if (info_aqi_data_km200_rpm25c > RPM25C_UNHEALTHY){
        m5.Lcd.fillScreen(YELLOW);
     }else if (info_aqi_data_km203_rtvoc > RTVOC_UNHEALTHY){
        m5.Lcd.fillScreen(YELLOW);
     }
     else if (info_aqi_data_km200_rpm10c > RPM10C_MODERATE){
        m5.Lcd.fillScreen(WHITE);
     }else if (info_aqi_data_km200_rpm25c > RPM25C_MODERATE){
        m5.Lcd.fillScreen(WHITE);
     }else if (info_aqi_data_km203_rtvoc > RTVOC_MODERATE){
        m5.Lcd.fillScreen(WHITE);
     }else{
        m5.Lcd.fillScreen(GREEN);
//        M5.Lcd.print(info_aqi_data_humidity);
     }
    
     delay(1000*15);  // wait
  
}
