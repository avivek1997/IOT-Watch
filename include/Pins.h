#include <Arduino.h>
#include <BlynkSimpleEsp8266.h>
struct Pins
{
    static const uint8_t PulseSensor = A0;
    static const uint8_t TemperatureSensor = D0;
    static const uint8_t VibrationSensor = D2;
}Pins;
struct VirtualPins
{
    int PulseSensor = V1;
    int TemperatureSensor = V2;
    int VibrationSensor = V3;
}VirtualPins;
