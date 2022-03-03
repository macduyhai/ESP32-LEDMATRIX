#if defined(ESP8266)
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#else
#include <WiFi.h>          //https://github.com/esp8266/Arduino
#endif

//needed for library
#if defined(ESP8266)
#include <ESP8266WebServer.h>
#else
#include <WebServer.h>
#endif
#include <DNSServer.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager
//#include "Mqttmess.h"

#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <ArduinoJWT.h>

#define TRIGGER_PIN 0

WiFiClient espClient;
PubSubClient client(espClient);

//#define ResetPin D13

bool MQTT_STT = false;

// MQTT SERVER
const char *mqtt_server = "test.mosquitto.org";
char key[] = "lvJvDWKiv0";

String MAC = "";
String   topic_in = "/v1/ledmatrix/" + MAC + "/in/";
String topic_out = "/v1/ledmatrix/" + MAC + "/out/";

//StaticJsonBuffer<200> jsonBuffer;
WiFiManager wifiManager;

// Check button reset
int reset_stt_old = 1;
int reset_stt_new = 1;

//========================= FUnction ============================
void UpdateStt(String &topicOut)
{
  String msg = "{\"update_stt\":\"1\"}";
  //String EncodeJWT(msg);
  char charBuf_out[50];
  topicOut.toCharArray(charBuf_out, 50);
  //  client.publish(charBuf_out, msg);
}
//
void mqttCallback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  String json_str = "";
  for (int i = 0; i < length; i++)
  {
    //Serial.print((char)payload[i]);
    json_str += (char)payload[i];
  }
  Serial.println(json_str);
  //  Serial.println();
  //  StaticJsonBuffer<200> jsonBuffer;
  //  JsonObject &msg = jsonBuffer.parseObject(json_str);
  //   Serial.printf(msg));
  //  if (!msg.success())
  //  {
  //    Serial.println("parseObject() failed");
  //    //return false;
  //  }
  //  else
  //  {
  //    Serial.print("ParseObject() Okie ");
  //  }

  //   if (msg["id"] == 1)
  //   {
  //   }
  //   else
  //   {

  //   }
}

void mqttReconnect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "mqtt-explorer-a4704b6d";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    //    const char* id = "ESP";
    //    const char* user = "xqdufvgb";
    //    const char* passw = " 5RNJC91AjlAv";
    if (client.connect(clientId.c_str()))
    {
      //if (client.connect(id, user, passw)) {
      Serial.println("connected");
      Serial.print("Topic in subcriber: ");
      Serial.println(topic_in);
      Serial.print("Topic in publisher: ");
      Serial.println(topic_out);
      // Once connected, publish an announcement...
      client.publish("/v1/ledmatrix/test/", "hello world");
      // ... and resubscribe
      //client.subscribe("TNQ_MQTT");
      char charBuf[50];
      topic_in.toCharArray(charBuf, 50);
      Serial.println(charBuf);
      client.subscribe(charBuf);
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(2000);
    }
  }
}

//
//void Check_reset()
//{
//  //  Serial.println("Check_reset");
//  reset_stt_new = digitalRead(ResetPin);
//  //  Serial.println(reset_stt_new);
//  if (reset_stt_new == 0 && reset_stt_old == 1)
//  {
//    Serial.println("//============== > RESET MODE < ============//");
//    MQTT_STT = false;
//
//    delay(500);
//    if (reset_stt_new == 0 && reset_stt_old == 1)
//    {
//      delay(500);
//      reset_stt_new = digitalRead(ResetPin);
//      Serial.println(reset_stt_new);
//      if (reset_stt_new == 0 && reset_stt_old == 1)
//      {
//        delay(500);
//        reset_stt_new = digitalRead(ResetPin);
//        Serial.println(reset_stt_new);
//        if (reset_stt_new == 0 && reset_stt_old == 1)
//        {
//          delay(500);
//          reset_stt_new = digitalRead(ResetPin);
//          Serial.println(reset_stt_new);
//          if (reset_stt_new == 0 && reset_stt_old == 1)
//          {
//            delay(500);
//            reset_stt_new = digitalRead(ResetPin);
//            Serial.println(reset_stt_new);
//            if (reset_stt_new == 0 && reset_stt_old == 1)
//            {
//              wifiManager.resetSettings();
//              reset_stt_old = reset_stt_new;
//              Serial.println("//==============< RESET DONE >============//");
//              ESP.restart();
//            } else {
//              MQTT_STT = true;
//            }
//          } else {
//            MQTT_STT = true;;
//          }
//        } else {
//          MQTT_STT = true;;
//        }
//      } else {
//        MQTT_STT = true;;
//      }
//    } else {
//      MQTT_STT = true;;
//    }
//  }
//}
//

//
void InitMqtt()
{
  Serial.println("InitMqtt");
  //   WIFI
  Serial.print("MAC: ");
  Serial.println(WiFi.macAddress());
  MAC = WiFi.macAddress();
  MAC.replace(":", "");
  topic_in = "/v1/ledmatrix/" + MAC + "/in";
  topic_out = "/v1/ledmatrix/" + MAC + "/out/";
  //  Serial.println(MAC);
  if (MQTT_STT == true)
  {
    client.setServer("test.mosquitto.org", 1883);
    client.setCallback(mqttCallback);
    Serial.println("Init Done");
  }


}

void Loop_mqtt()
{
  if (MQTT_STT == true)
  {
    if (!client.connected())
    {
      mqttReconnect();
    }
    client.loop();
  }
  //  Check_reset(); // Check button RESET
  yield();
}
//==========================================================
void InitWifi() {
  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around

  //reset settings - for testing
  //wifiManager.resetSettings();

  //sets timeout until configuration portal gets turned off
  //useful to make it all retry or go to sleep
  //in seconds
  wifiManager.setTimeout(180);

  //it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration

  //WITHOUT THIS THE AP DOES NOT SEEM TO WORK PROPERLY WITH SDK 1.5 , update to at least 1.5.1
  //WiFi.mode(WIFI_STA);

  //  if (!wifiManager.startConfigPortal("LedMatrix_SimonTool")) { //autoConnect
  if (!wifiManager.autoConnect("LedMatrix_SimonTool")) { //autoConnect
    Serial.println("failed to connect and hit timeout");
    MQTT_STT = false;
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    ESP.restart();
    delay(5000);
  } else {
    MQTT_STT = true;
    //if you get here you have connected to the WiFi
    Serial.println("connected wifi)");
  }


}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("\n Starting");

  pinMode(TRIGGER_PIN, INPUT);
  //  pinMode(BUILTIN_LED, OUTPUT); // Initialize the BUILTIN_LED pin as an output
  //  pinMode(ResetPin, INPUT_PULLUP);
  InitWifi();
  InitMqtt();
}

void loop() {
  Loop_mqtt();
}
