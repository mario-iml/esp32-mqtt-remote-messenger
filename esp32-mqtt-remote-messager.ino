#include <WiFi.h>
#include <PubSubClient.h>
#include <LiquidCrystal.h>

// Replace with your network credentials
const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;

// ultrasonic distance
const long sensor_distance = 8;

const int led_pin = 32;
const int sensor_echo_pin = 33;
const int sensor_trig_pin = 25;

// lcd
const int rs = 26,
          en = 27,
          d4 = 14,
          d5 = 21,
          d6 = 13,
          d7 = 23;
const int contrast_pin = 22;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const long interval = 30000;
unsigned long previous_millis = 0;


// mqtt
const char mqtt_broker[] =  MQTT_BROKER;
int        mqtt_port     =  MQTT_PORT;
const char* mqtt_username =  MQTT_USER;
const char* mqtt_password =  MQTT_PASSWORD;
const char client_id[]  =  MQTT_CLIENT_ID;
const char topic[]  =  MQTT_TOPIC;

// clients
WiFiClient espClient;
PubSubClient client(espClient);

// message and status
bool new_msg = false;
String msg = "";

void setup() {
  Serial.begin(9600);

  // led to digital output and sensor to digital input
  pinMode(led_pin, OUTPUT);
  pinMode(sensor_trig_pin, OUTPUT);
  pinMode(sensor_echo_pin, INPUT);

  // lcd
  pinMode(contrast_pin, OUTPUT);
  analogWrite(22, 90);
  lcd.begin(16, 2);

  // connect to wifi
  initWiFi();
}

void loop() {
  if (new_msg){
    digitalWrite(led_pin, HIGH);
  } else {
    digitalWrite(led_pin, LOW);
  }
  client.loop();

  // ultrasonic sensor reading
  digitalWrite(sensor_trig_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(sensor_trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(sensor_trig_pin, LOW);

  long duration = pulseIn(sensor_echo_pin, HIGH);
  long cm = microsecondsToCentimeters(duration);
  // Serial.print(cm);
  // Serial.println("cm");

  if(cm >= sensor_distance && new_msg) {
    Serial.print("Displaying msg: ");
    Serial.println(msg);

    // display on lcd display
    lcd.clear();
    lcd.setCursor(0, 0);
    if(msg.length() > 16) {
      lcd.print(msg.substring(0, 16));
      lcd.setCursor(0, 1);
      lcd.print(msg.substring(16, msg.length()));
    } else {
      lcd.print("New Message:");
      lcd.setCursor(0, 1);
      lcd.print(msg);
    }
    

    previous_millis = millis();
    new_msg = false;
  }

  unsigned long current_millis = millis();

  if(current_millis - previous_millis >= interval) {
    lcd.clear();
  }

  delay(10);
}


void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    digitalWrite (led_pin, LOW);
    delay(500);
    digitalWrite (led_pin, HIGH);
    delay(500);
  }
  // connected
  Serial.println(WiFi.localIP());
  digitalWrite (led_pin, LOW);

  // connect to mqtt
  initMqtt();
}

void initMqtt() {
  // connecting to a mqtt broker
    client.setServer(mqtt_broker, mqtt_port);
    client.setCallback(callback);
    while (!client.connected()) {
        String client_id = "esp32-client-";
        client_id += String(WiFi.macAddress());
        Serial.printf("The client %s connects to the public MQTT broker\n", client_id.c_str());
        if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
            Serial.println("Public EMQX MQTT broker connected");
        } else {
            Serial.print("failed with state ");
            Serial.print(client.state());
            digitalWrite (led_pin, LOW);
            delay(3000);
            digitalWrite (led_pin, HIGH);
            delay(3000);
        }
    }
    client.subscribe(topic);
    digitalWrite (led_pin, LOW);
}

void callback(char *topic, byte *payload, unsigned int length) {
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.print("Message:");

    msg = "";
    new_msg = true;

    for (int i = 0; i < length; i++) {
        msg += (char)payload[i];
    }
    Serial.println(msg);
    Serial.println("-----------------------");
}

long microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2;
}
