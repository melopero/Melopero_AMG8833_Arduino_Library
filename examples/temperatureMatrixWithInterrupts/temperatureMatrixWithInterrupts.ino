#include <MP_AMG8833.h>

MP_AMG8833 sensor;

float highThreshold = 26.5;
float lowThreshold = 10;

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
}

void loop() {
  Serial.print("Updating thermistor temperature ... ");
  int statusCode = sensor.updateThermistorTemperature();
  Serial.println(sensor.getErrorDescription(statusCode));

  Serial.print("Updating status ... ");
  statusCode = sensor.updateStatus();
  Serial.println(sensor.getErrorDescription(statusCode));

  Serial.print("Thermistor temp: ");
  Serial.print(sensor.thermistorTemperature);
  Serial.println("°C");

  Serial.print("Interrupt triggered : ");
  Serial.println(sensor.interruptTriggered);

  if (sensor.interruptTriggered){
    
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
