#include "DHT.h"
#include "Adafruit_Sensor.h"
#include "Adafruit_BMP280.h"
#include "ESP8266WiFi.h"
#include "PubSubClient.h" 


DHT dht(14, DHT22); //(pin , type of DHT)
Adafruit_BMP280 bme; // I2C pin

const char* ssid = "kublot wi-fi 2";  // network SSID
const char* wifi_password = "asfalt12"; // network password

const char* mqtt_server = "192.168.0.189";  // IP of the MQTT broker
const char* humidity_topic = "home/bathroom/humidity";
const char* temperature_topic = "home/bathroom/temperature";
const char* pressure_topic = "home/bathroom/pressure";
const char* mqtt_username = "asfalt12"; // MQTT username
const char* mqtt_password = "asfalt12"; // MQTT password
const char* clientID = "Home"; // MQTT client ID

WiFiClient wifiClient; // start WIFI
PubSubClient client(mqtt_server, 1883, wifiClient);  //Creating a fully configured MQTT client instance
  
  //Debugging part
  /*Serial.println("WiFi connected");
  //Serial.print("IP address: ");
  //Serial.println(WiFi.localIP());*/
  

void setup() 
{
  Serial.begin(9600);
  Serial.setTimeout(2000);

  dht.begin(); //initialize communication to dht22 sensor
  bme.begin(0x76);  //initialize communication to bme280 sensor 
}

void loop() 
  {
  WiFi.begin(ssid, wifi_password); //connecting to WiFi
  while (WiFi.status() != WL_CONNECTED) // Waiting for the WiFi connection
  {
    delay(500);
    Serial.print(".");
  }
   //Debugging part
  /*Serial.println("WiFi connected");
  //Serial.print("IP address: ");
  //Serial.println(WiFi.localIP());*/
  
  client.connect(clientID, mqtt_username, mqtt_password);   
  
  float h = dht.readHumidity(); // humidity
  float t = dht.readTemperature(); // temperature in Celsius
  //float c = bme.readTemperature(); bme280 temperature if needed
  float p = bme.readPressure();

     
  //Debugging part
  /*if (isnan(h) || isnan(t)) 
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Serial.print("Humidity: ");Serial.print(h);Serial.print(" %\t");
  Serial.print("Temperature: ");Serial.print(t);Serial.print(" *C ");
  Serial.print("Heat index: ");Serial.print(hic);Serial.print(" *C ");
  Serial.print("Pressure: ");Serial.print(p);Serial.print(" Pa ");
  */
  client.publish(temperature_topic, String(t).c_str()); // MQTT can only transfer sting data type
  client.publish(humidity_topic, String(h).c_str());
  client.publish(pressure_topic, String(p).c_str());
  
  // PUBLISH to the MQTT Broker (topic = Temperature, defined at the beginning)
 /* if (client.publish(temperature_topic, String(t).c_str())) {
    Serial.println("Temperature sent!");
  }
  // Again, client.publish will return a boolean value depending on whether it succeded or not.
  // If the message failed to send, we will try again, as the connection may have broken.
  else {
    Serial.println("Temperature failed to send. Reconnecting to MQTT Broker and trying again");
    client.connect(clientID, mqtt_username, mqtt_password);
    delay(10); // This delay ensures that client.publish doesn't clash with the client.connect call
    client.publish(temperature_topic, String(t).c_str());
    Serial.println("Temperature sent!");
  }

  // PUBLISH to the MQTT Broker (topic = Humidity, defined at the beginning)
  if (client.publish(humidity_topic, String(h).c_str())) {
    Serial.println("Humidity sent!");
  }
  // Again, client.publish will return a boolean value depending on whether it succeded or not.
  // If the message failed to send, we will try again, as the connection may have broken.
  else {
    Serial.println("Humidity failed to send. Reconnecting to MQTT Broker and trying again");
    client.connect(clientID, mqtt_username, mqtt_password);
    delay(10); // This delay ensures that client.publish doesn't clash with the client.connect call
    client.publish(humidity_topic, String(h).c_str());
  }
     // PUBLISH to the MQTT Broker (topic = Temperature, defined at the beginning)
  if (client.publish(pressure_topic, String(p).c_str())) {
    Serial.println("Pressure sent!");
  }
  // Again, client.publish will return a boolean value depending on whether it succeded or not.
  // If the message failed to send, we will try again, as the connection may have broken.
  else {
    Serial.println("Pressure failed to send. Reconnecting to MQTT Broker and trying again");
    client.connect(clientID, mqtt_username, mqtt_password);
    delay(10); // This delay ensures that client.publish doesn't clash with the client.connect call
    client.publish(pressure_topic, String(p).c_str());
  }*/
  client.disconnect();  // disconnect from the MQTT broker
    delay(30000); //taking data every 30 seconds
}
