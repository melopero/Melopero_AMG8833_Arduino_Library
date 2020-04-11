//Author: Leonardo La Rocca
#ifndef MP_AMG8833_H_INCLUDED
#define MP_AMG8833_H_INCLUDED

#define I2C_ADDRESS_A 0x68
#define I2C_ADDRESS_B 0x69

#define FPS_MODE_REGISTER 0x02
#define THERMISTOR_REGISTER 0x0E
#define FIRST_PIXEL_REGISTER = 0x80
#define LAST_PIXEL_REGISTER = 0xFE

enum class FPS_MODE : uint8_t {
    FPS_10_MODE = 0,
    FPS_1_MODE = 1
}

enum class MP_AMG8833_ERROR_CODE : int {
    NO_ERROR = 0,
    ERROR_READING = -1,
    ERROR_WRITING = -2
}

class MP_AMG8833 {
    //Members
    public:
        uint8_t i2cAddress;
        uint8_t bus;

        float pixelMatrix[8][8];
        float thermistorTemperature;

    //Methods
    public:
        MP_AMG8833(uint8_t i2cAddr = I2C_ADDRESS_A);

        int setFPSMode(FPS_MODE mode);
        int getFPSMode();

        int updatePixelMatrix();
        int updateThermistorTemperature();

        int readByte(uint8_t registerAddress)
        int writeByte(uint8_t registerAddress, uint8_t value);

        String getErrorDescription(int errorCode);

    private:
        float parsePixel(uint8_t lsb, uint8_t msb);
}

#endif // MP_AMG8833_H_INCLUDED
