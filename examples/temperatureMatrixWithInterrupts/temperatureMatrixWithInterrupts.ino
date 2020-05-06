#include <Melopero_AMG8833.h>

Melopero_AMG8833 sensor;

float highThreshold = 26.5;
float lowThreshold = 10;

bool interruptOccurred = false;
const byte interruptPin = 2;

void onInterrupt(){
  interruptOccurred = true;
}

void setup() {
  Serial.begin(9600);
  Serial.print("Resetting sensor ... ");
  int statusCode = sensor.resetFlagsAndSettings();
  Serial.println(sensor.getErrorDescription(statusCode));

  Serial.print("Setting FPS ... ");
  statusCode = sensor.setFPSMode(FPS_MODE::FPS_10);
  Serial.println(sensor.getErrorDescription(statusCode));

  Serial.print("Setting threshold values ... ");
  statusCode = sensor.setInterruptThreshold(lowThreshold, highThreshold);
  Serial.println(sensor.getErrorDescription(statusCode));
  Serial.print("low threshold : ");
  Serial.print(lowThreshold);
  Serial.print(" high threshold : ");
  Serial.println(highThreshold);

  Serial.print("Enabling interrupt ... ");
  statusCode = sensor.enableInterrupt();
  Serial.println(sensor.getErrorDescription(statusCode));

  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), onInterrupt, CHANGE);
}

void loop() {
  Serial.print("Updating thermistor temperature ... ");
  int statusCode = sensor.updateThermistorTemperature();
  Serial.println(sensor.getErrorDescription(statusCode));

  Serial.print("Thermistor temp: ");
  Serial.print(sensor.thermistorTemperature);
  Serial.println("°C");

  

  if (interruptOccurred){
    interruptOccurred = false;
    Serial.println("Interrupt triggered!");
    
    Serial.print("Updating interrupt matrix ... ");
    statusCode = sensor.updateInterruptMatrix();
    Serial.println(sensor.getErrorDescription(statusCode));

    Serial.println("Interrupt Matrix: ");
    for (int x = 0; x < 8; x++){
      for (int y = 0; y < 8; y++){
        Serial.print(sensor.interruptMatrix[y][x]);
        Serial.print(" ");
      }
      Serial.println();
    }
  }

  delay(1000);
}
