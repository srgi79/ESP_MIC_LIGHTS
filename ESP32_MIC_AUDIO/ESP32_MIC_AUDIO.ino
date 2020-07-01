
#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#define micPin D0
#elif defined ESP32
#include <WiFi.h>
#include <HTTPClient.h>
#define micPin 4
#endif
 
const char* ssid = "XXXX";
const char* password =  "XXXX";
String serverName = "http://192.168.0.160/light/fredde/";


unsigned long previousMillis = 0;
unsigned long cMillis = 0;
unsigned long iMillis = 0;
unsigned long sMillis = 0;
const long interval = 50;

bool knock = false;

unsigned long sIntervals[] = {
  0, 400, 800, 1200
};
const unsigned int lKnocks = sizeof(sIntervals)/sizeof(sIntervals[0]);
int s = 0;


void setup() {
  pinMode(micPin,INPUT);
  Serial.begin(115200);
  delay(500);
  Serial.print("lKnocks: "); Serial.println(lKnocks);
  WiFi.begin(ssid, password); 
  Serial.print("Connectant ...");
  while (WiFi.status() != WL_CONNECTED) { //Check for the connection
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("WIFI OK: ");
  Serial.println(ssid);
}

void loop() {
  cMillis = millis();
  
  if ((cMillis - previousMillis >= interval) and not digitalRead(micPin)) {
    previousMillis = cMillis;
    if (s > 0) {
      Serial.print("STATE: "); Serial.println(s); } else {iMillis = millis();}
    knock = true;} 

  if (knock) {
    knock = false;
    s = s + 1;
    sMillis = cMillis - iMillis;
    
//    Serial.print("s:   "); Serial.println(s);    Serial.print("s-1: "); Serial.println(s-1);    Serial.print("sMillis: "); Serial.println(sMillis);
    
    if ((sMillis >= sIntervals[s-1]) and (sMillis <= sIntervals[s])) {
//      Serial.println("KNOCK OK"); 
    } 
    else {
      s = 0;
//      Serial.println("KNOCK FAILED"); 
    }
    }
    
  if (s >= lKnocks) { 
    s = 0;
    Serial.println("//////////// TOGGLING ////////////");
    HTTPClient http;
    String pathOn = serverName + "toggle";
    http.begin(pathOn.c_str());
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.POST("{effect:Flicker,brightness:255,white_value:100,r:255,g:255,b:255}");
    http.end();
    }

  if ((cMillis - previousMillis >= 4*sIntervals[1]) and s !=0) {
    s = 0;
    Serial.println("RESET");}
    
}
/*
#define HTTPC_ERROR_CONNECTION_REFUSED  (-1)
#define HTTPC_ERROR_SEND_HEADER_FAILED  (-2)
#define HTTPC_ERROR_SEND_PAYLOAD_FAILED (-3)
#define HTTPC_ERROR_NOT_CONNECTED       (-4)
#define HTTPC_ERROR_CONNECTION_LOST     (-5)
#define HTTPC_ERROR_NO_STREAM           (-6)
#define HTTPC_ERROR_NO_HTTP_SERVER      (-7)
#define HTTPC_ERROR_TOO_LESS_RAM        (-8)
#define HTTPC_ERROR_ENCODING            (-9)
#define HTTPC_ERROR_STREAM_WRITE        (-10)
#define HTTPC_ERROR_READ_TIMEOUT        (-11)
*/
