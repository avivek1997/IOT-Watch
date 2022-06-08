#include <Arduino.h>
#include <Messages.h>
#include <Configs.h>
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Pins.h>
#include <OneWire.h>
#include <DallasTemperature.h>
OneWire oneWire(Pins.TemperatureSensor);
DallasTemperature temperatureSensor(&oneWire);
double lastLatitiude ,lastLongitude;
BlynkTimer timer;

#define USE_ARDUINO_INTERRUPTS true    // Set-up low-level interrupts for most acurate BPM math.
#include <PulseSensorPlayground.h>     // Includes the PulseSensorPlayground Library.
PulseSensorPlayground pulseSensor;

void HandlePulseSensor()
{
  int myBPM = pulseSensor.getBeatsPerMinute();
  if (pulseSensor.sawStartOfBeat()) {            // Constantly test to see if "a beat happened". 
 Serial.println("♥  A HeartBeat Happened ! "); // If test is "true", print a message "a heartbeat happened".
 Serial.print("BPM: ");                        // Print phrase "BPM: " 
 Serial.println(myBPM);                        // Print the value inside of myBPM. 
 Blynk.virtualWrite(VirtualPins.PulseSensor, myBPM);
}
  
}
void HandleTemperatureSensor()
{
  temperatureSensor.requestTemperatures(); 
  float temperatureC = temperatureSensor.getTempCByIndex(0);
  float temperatureF = temperatureSensor.getTempFByIndex(0);
  Serial.print(temperatureC);
  Serial.println("ºC");
  Serial.print(temperatureF);
  Serial.println("ºF");
  Blynk.virtualWrite(VirtualPins.TemperatureSensor, temperatureC);
}
void HandleVibrationSensor()
{
  int sensor = digitalRead(Pins.VibrationSensor);
  if(sensor)
  {
    Blynk.notify(Messages.VibrationSensorMessage + String(lastLatitiude) + String(lastLongitude));
  }
}

BLYNK_WRITE(V1) {
  GpsParam gps(param);

  // Print 6 decimal places for Lat, Lon
  Serial.print("Lat: ");
  Serial.println(gps.getLat(), 7);
  lastLatitiude = gps.getLat();

  Serial.print("Lon: ");
  Serial.println(gps.getLon(), 7);
  lastLongitude = gps.getLon();

  // Print 2 decimal places for Alt, Speed
  Serial.print("Altitute: ");
  Serial.println(gps.getAltitude(), 2);

  Serial.print("Speed: ");
  Serial.println(gps.getSpeed(), 2);

  Serial.println();
}

void setup() {
  Serial.begin(9600);
  Blynk.begin(Configs.BlynkAuth, Configs.Wifi_SSID, Configs.WIFI_Password);

  pulseSensor.analogInput(Pins.PulseSensor);   
  pulseSensor.blinkOnPulse(LED_BUILTIN);       //auto-magically blink Arduino's LED with heartbeat.
  pulseSensor.setThreshold(Configs.PulseSensorThreshold);    
  pulseSensor.begin();
  temperatureSensor.begin();
  pinMode(Pins.VibrationSensor,INPUT);
  timer.setInterval(100L, HandlePulseSensor);
  timer.setInterval(5000L, HandleTemperatureSensor);
}

void loop() {
   Blynk.run();
   timer.run();
   HandleVibrationSensor();
}