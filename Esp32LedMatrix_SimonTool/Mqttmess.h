
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <ArduinoJWT.h>

WiFiClient espClient;
PubSubClient client(espClient);

#define ResetPin D13

// {"key":"lvJvDWKiv0","mac":"asdsaffsasf"}
bool MQTT_STT = false;

// MQTT SERVER
const char *mqtt_server = "broker.hivemq.com";
char *key = "lvJvDWKiv0";

String MAC = "";
String   topic_in = "/v1/ledmatrix/" + MAC + "/in";
String topic_out = "/v1/ledmatrix/" + MAC + "/out/";

//StaticJsonBuffer<200> jsonBuffer;

//========================= FUnction ============================
void UpdateStt(String &topicOut)
{
  String msg = "{\"update_stt\":\"1\"}";
  //String EncodeJWT(msg);
  char charBuf_out[50];
  topicOut.toCharArray(charBuf_out, 50);
  //  client.publish(charBuf_out, msg);
}

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
  Serial.println();
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject &msg = jsonBuffer.parseObject(json_str);
   Serial.printf(msg));
  if (!msg.success())
  {
    Serial.println("parseObject() failed");
    //return false;
  }
  else
  {
    Serial.print("ParseObject() Okie ");
  }

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
    String clientId = "clientId-jgJEbXcFr1";
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
      //      client.publish("outTopic", "hello world");
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
      delay(5000);
    }
  }
}
// Check button reset
int reset_stt_old = 1;
int reset_stt_new = 1;

void Check_reset()
{
  //  Serial.println("Check_reset");
  reset_stt_new = digitalRead(ResetPin);
  //  Serial.println(reset_stt_new);
  if (reset_stt_new == 0 && reset_stt_old == 1)
  {
    Serial.println("//============== > RESET MODE < ============//");
    MQTT_STT = false;

    delay(500);
    if (reset_stt_new == 0 && reset_stt_old == 1)
    {
      delay(500);
      reset_stt_new = digitalRead(ResetPin);
      Serial.println(reset_stt_new);
      if (reset_stt_new == 0 && reset_stt_old == 1)
      {
        delay(500);
        reset_stt_new = digitalRead(ResetPin);
        Serial.println(reset_stt_new);
        if (reset_stt_new == 0 && reset_stt_old == 1)
        {
          delay(500);
          reset_stt_new = digitalRead(ResetPin);
          Serial.println(reset_stt_new);
          if (reset_stt_new == 0 && reset_stt_old == 1)
          {
            delay(500);
            reset_stt_new = digitalRead(ResetPin);
            Serial.println(reset_stt_new);
            if (reset_stt_new == 0 && reset_stt_old == 1)
            {
              wifiManager.resetSettings();
              reset_stt_old = reset_stt_new;
              Serial.println("//==============< RESET DONE >============//");
              ESP.restart();
            } else {
              MQTT_STT = true;
            }
          } else {
            MQTT_STT = true;;
          }
        } else {
          MQTT_STT = true;;
        }
      } else {
        MQTT_STT = true;;
      }
    } else {
      MQTT_STT = true;;
    }
  }
}



void Initmqtt()
{
  //   WIFI
  Serial.print("MAC: ");
  Serial.println(WiFi.macAddress());
  MAC = WiFi.macAddress();
  MAC.replace(":", "");
  topic_in = "/v1/ledmatrix/" + MAC + "/in";
  topic_out = "/v1/ledmatrix/" + MAC + "/out/";
  Serial.println(MAC);
  Serial.println("Cac thong so cua wifi sau khi ket noi");
  Serial.println(WiFi.status());
  Serial.println("-------------------");
  if (MQTT_STT== true)
  {
   client.setServer("broker.hivemq.com", 1883);
   client.setCallback(mqttCallback);
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
  Check_reset(); // Check button RESET
  yield();
}
