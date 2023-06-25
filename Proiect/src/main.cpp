#include <Arduino.h>
#include <BluetoothSerial.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>
using namespace std;

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#define DHTPIN 33
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define PHOTO_PIN 34
#define btcServerName "ProiectSenzori"
BluetoothSerial SerialBT;
String data = "";
const char *teamId = "B07";
bool stopChart = 0;
bool stopChartDHT = 0;
StaticJsonDocument<4096> jsonBuffer;

void receivedData();
void getValuesFR();
void getValuesDHT();
void getAveragesDHT(String n);
void getSensors();

void chartFR()
{
  int photoValue;
  Serial.println(stopChart);
  while (!stopChart)
  {
    if (SerialBT.available())
    {
      receivedData();
      if (stopChart)
      {
        break;
      }
    }
    photoValue = digitalRead(PHOTO_PIN);
    JsonObject pointsObject = jsonBuffer.createNestedObject("points");

    jsonBuffer["id"] = 2;

    pointsObject["Light"] = photoValue;

    jsonBuffer["teamId"] = teamId;

    String jsonString;
    serializeJson(jsonBuffer, jsonString);

    Serial.println("Sent JSON data:");
    Serial.println(jsonString);
    SerialBT.println(jsonString);
    pointsObject.clear();
    jsonBuffer.clear();
    delay(1000);
    if (stopChart)
    {
      break;
    }
  }
  jsonBuffer["id"] = 2;
  jsonBuffer["stopped"] = stopChart;
  jsonBuffer["teamId"] = teamId;

  String jsonString;
  serializeJson(jsonBuffer, jsonString);

  Serial.println("Sent JSON data:");
  Serial.println(jsonString);
  SerialBT.println(jsonString);
  jsonBuffer.clear();
  return;
}

void chartDHT()
{
  float temp, hum;
  Serial.println(stopChartDHT);
  while (!stopChartDHT)
  {
    if (SerialBT.available())
    {
      receivedData();
      if (stopChartDHT)
      {
        break;
      }
    }
    temp = dht.readTemperature();
    hum = dht.readHumidity();
    JsonObject pointsObject = jsonBuffer.createNestedObject("points");

    jsonBuffer["id"] = 1;

    pointsObject["Temperature"] = temp;
    pointsObject["Humidity"] = hum;

    jsonBuffer["teamId"] = teamId;

    String jsonString;
    serializeJson(jsonBuffer, jsonString);

    Serial.println("Sent JSON data:");
    Serial.println(jsonString);
    SerialBT.println(jsonString);
    pointsObject.clear();
    jsonBuffer.clear();
    delay(1000);
    if (stopChartDHT)
    {
      break;
    }
  }
  jsonBuffer["id"] = 1;
  jsonBuffer["stopped"] = stopChartDHT;
  jsonBuffer["teamId"] = teamId;

  String jsonString;
  serializeJson(jsonBuffer, jsonString);

  Serial.println("Sent JSON data:");
  Serial.println(jsonString);
  SerialBT.println(jsonString);
  jsonBuffer.clear();
  return;
}

void getAveragesDHT(String n)
{
  int i = 0, p = 1, j = 0, nr;
  float tempAverage = 0;
  float humAverage = 0;
  float temp, hum;

  Serial.println(n);
  while (j < n.length())
  {
    i = i * p + n[j] - '0';
    j++;
    p *= 10;
  }
  nr = i;

  while (i > 0)
  {
    i--;
    temp = dht.readTemperature();
    hum = dht.readHumidity();
    tempAverage += temp;
    humAverage += hum;
    Serial.print("T:");
    Serial.print(temp);
    Serial.print(" H:");
    Serial.println(hum);
    delay(500);
  }
  tempAverage /= nr;
  humAverage /= nr;

  jsonBuffer["id"] = 2;

  JsonObject averagesObject = jsonBuffer.createNestedObject("averages");
  averagesObject["Temperature"] = tempAverage;
  averagesObject["Humidity"] = humAverage;

  jsonBuffer["teamId"] = teamId;

  String jsonString;
  serializeJson(jsonBuffer, jsonString);

  Serial.println("Sent JSON data:");
  Serial.println(jsonString);
  SerialBT.println(jsonString);
  jsonBuffer.clear();
}

void getValuesFR()
{
  Serial.println("Citire fotorezistor");
  int photoValue = digitalRead(PHOTO_PIN);

  jsonBuffer["id"] = 2;

  JsonObject valuesObject = jsonBuffer.createNestedObject("values");
  valuesObject["Light"] = photoValue;

  jsonBuffer["teamId"] = teamId;

  String jsonString;
  serializeJson(jsonBuffer, jsonString);

  Serial.println("Sent JSON data:");
  Serial.println(jsonString);
  SerialBT.println(jsonString);
  Serial.println(photoValue);
  jsonBuffer.clear();
}

void getValuesDHT()
{
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity))
  {
    Serial.println("Failed to read data from DHT11 sensor");
    return;
  }

  JsonObject valuesObject = jsonBuffer.createNestedObject("values");
  valuesObject["Temperature"] = temperature;
  valuesObject["Humidity"] = humidity;
  jsonBuffer["id"] = 1;
  jsonBuffer["teamId"] = teamId;

  String jsonString;
  serializeJson(jsonBuffer, jsonString);

  Serial.println("Sent JSON data:");
  Serial.println(jsonString);
  SerialBT.println(jsonString);
  jsonBuffer.clear();
}

void getSensors()
{
  // Create a JSON array for the parameters
  JsonArray parameters = jsonBuffer.createNestedArray("parameters");

  // Add items for Sensor 1
  JsonObject sensor1Params1 = parameters.createNestedObject();
  sensor1Params1["parameter"] = "Temperature";
  sensor1Params1["unit"] = "C";

  JsonObject sensor1Params2 = parameters.createNestedObject();
  sensor1Params2["parameter"] = "Humidity";
  sensor1Params2["unit"] = "%";

  jsonBuffer["type"] = "DHT11";
  jsonBuffer["pin"] = "33";
  jsonBuffer["interface"] = "1-Wire";
  jsonBuffer["id"] = "1";
  jsonBuffer["teamId"] = teamId;

  String jsonString;
  serializeJson(jsonBuffer, jsonString);

  // Send the JSON string over Bluetooth
  SerialBT.println(jsonString);

  // Reset the JSON buffer for the next sensor
  jsonBuffer.clear();
  parameters.clear();

  // Add items for Sensor 2

  // Create a JSON array for the parameters
  JsonArray parameters2 = jsonBuffer.createNestedArray("parameters");

  JsonObject sensor2Params = parameters2.createNestedObject();
  sensor2Params["parameter"] = "Light";
  sensor2Params["unit"] = "";

  jsonBuffer["type"] = "Photoresistor";
  jsonBuffer["pin"] = "34";
  jsonBuffer["interface"] = "Digital";
  jsonBuffer["id"] = "2";
  jsonBuffer["teamId"] = teamId;

  // Serialize the JSON buffer into a string
  jsonString.clear();
  serializeJson(jsonBuffer, jsonString);
  jsonBuffer.clear();
  parameters.clear();

  // Send the JSON string over Bluetooth
  SerialBT.println(jsonString);
}

void receivedData()
{
  while (SerialBT.available())
  {
    data = SerialBT.readStringUntil('\n');
  }

  Serial.println(data);
  DynamicJsonDocument JSONDocument(15000);
  DeserializationError error = deserializeJson(JSONDocument, data);
  if (error)
  {
    Serial.print("Error parsing JSON: ");
    Serial.println(error.c_str());
    return;
  }
  else
  {
    String action = JSONDocument["action"];
    Serial.println(action);
    if (action == "getSensors")
      getSensors();
    if (action == "getValues")
    {
      if (JSONDocument["id"] == "2")
        getValuesFR();
      if (JSONDocument["id"] == "1")
        getValuesDHT();
    }
    if (action == "getAverages")
    {
      String n = JSONDocument["number"];
      if (JSONDocument["id"] == "1")
        getAveragesDHT(n);
    }
    if (action == "startChart")
    {
      if (JSONDocument["id"] == "2")
      {
        stopChart = 0;
        chartFR();
      }
      if (JSONDocument["id"] == "1")
      {
        stopChartDHT = 0;
        chartDHT();
      }
    }
    if (action == "stopChart")
    {
      if (JSONDocument["id"] == "2")
        stopChart = 1;
      if (JSONDocument["id"] == "1")
        stopChartDHT = 1;
    }
  }
  data = "";
  delay(100);
}

void setup()
{
  Serial.begin(115200);
  SerialBT.begin(btcServerName);
  Serial.println("The device started, now you can pair it with bluetooth!");
  pinMode(PHOTO_PIN, INPUT);
  dht.begin();
}

void loop()
{
  if (SerialBT.available())
  {
    receivedData();
  }
}