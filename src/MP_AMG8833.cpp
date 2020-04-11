//Author: Leonardo La Rocca
#include "MP_AMG8833.h"
#include "Wire.h"

MP_AMG8833::MP_AMG8833(uint8_t i2cAddr){
    this->i2cAddress = i2cAddr;
    Wire.begin();
}

int MP_AMG8833::readByte(uint8_t registerAddress){
    Wire.beginTransmission(this->i2cAddress);
    Wire.write(registerAddress);
    if (Wire.endTransmission(false) != 0)
        return MP_AMG8833_ERROR_CODE::ERROR_READING;

    Wire.requestFrom(this->i2cAddress, 1);
    if (Wire.available())
        return Wire.read();
    else
        return MP_AMG8833_ERROR_CODE::ERROR_READING;
}

int MP_AMG8833::writeByte(uint8_t registerAddress, uint8_t value){
    Wire.beginTransmission(this->i2cAddress);
    if (Wire.endTransmission(false) != 0)
        return MP_AMG8833_ERROR_CODE::ERROR_WRITING;

    Wire.beginTransmission(this->i2cAddress);
    Wire.write(registerAddress);
    Wire.write(value);
    Wire.endTransmission();
    return MP_AMG8833_ERROR_CODE::NO_ERROR;
}

int MP_AMG8833::setFPSMode(FPS_MODE mode){
    return this->writeByte(FPS_MODE_REGISTER, mode);
}

int MP_AMG8833::getFPSMode(){
    int value = this->readByte(FPS_MODE_REGISTER);
    if (value < 0) return value;
    return value & 1;
}

int MP_AMG8833::updatePixelMatrix(){
    Wire.beginTransmission(this->i2cAddress);
    Wire.write(FIRST_PIXEL_REGISTER);
    if (Wire.endTransmission(false) != 0)
        return MP_AMG8833_ERROR_CODE::ERROR_READING;

    int bufferSize = 0;
    int i = 0;
    int j = 0;

    Wire.requestFrom(this->i2cAddress, 32);
    if (Wire.available()){
        uint8_t lsb = Wire.read();
        uint8_t msb = Wire.read();

        pixelMatrix[i][j] = this->parsePixel(lsb, msb);
        j += 1;
        if (j >= 8) {
            j = 0;
            i++;
        }

        bufferSize += 2;
        if (bufferSize >= 32){
            bufferSize = 0;
            Wire.requestFrom(i2cAddress, 32);
        }
    }
    else
        return MP_AMG8833_ERROR_CODE::ERROR_READING;
}

float parsePixel(uint8_t lsb, uint8_t msb){
    int unified_no_sign = ((msb & 7) << 8) | lsb;
    int value = (msb & 8) > 0 ? 0 : - (1 << 11);
    value += unified _no_sign;
    return ((float) value) / 4.0;
}

int MP_AMG8833::updateThermistorTemperature(){
    //retrieve register data
    int msb = this->readByte(THERMISTOR_REGISTER + 1);
    int lsb = this->readByte(THERMISTOR_REGISTER);
    //check everything went right
    if (msb < 0 || lsb < 0) return MP_AMG8833_ERROR_CODE::ERROR_READING;

    //parse data
    int sign = msb & 0x08 > 0 ? -1 : 1;
    int value = ((msb & 0x07) << 4) | (lsb & 0xF0);
    float frac = 1.0 / (float) (lsb & 0x0F);
    this->thermistorTemperature = sign * value + frac;

    return return MP_AMG8833_ERROR_CODE::NO_ERROR;
}

String MP_AMG8833::getErrorDescription(int errorCode){
    if (errorCode >= 0)
        return "No Error";
    if (errorCode == -1)
        return "Error reading from device";
    if (errorCode == -2)
        return "Error writing to device";
}
