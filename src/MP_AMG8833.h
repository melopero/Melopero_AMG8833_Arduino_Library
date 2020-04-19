//Author: Leonardo La Rocca
#ifndef MP_AMG8833_H_INCLUDED
#define MP_AMG8833_H_INCLUDED

#define I2C_ADDRESS_A 0x68
#define I2C_ADDRESS_B 0x69

#define MODE_REG_ADDR 0x00
#define RESET_REG_ADDR 0x01
#define FPS_MODE_REGISTER 0x02
#define INTERRUPT_CONTROL_REG_ADDR 0x03
#define STATUS_REG_ADDR 0x04
#define CLEAR_STATUS_REG_ADDR 0x05
#define INT_THR_HIGH_L_REG_ADDRESS 0x08
#define INT_THR_HIGH_H_REG_ADDRESS 0x09
#define INT_THR_LOW_L_REG_ADDRESS 0x0A
#define INT_THR_LOW_H_REG_ADDRESS 0x0B
#define INT_TABLE_FIRST_ROW 0x10
#define INT_TABLE_LAST_ROW 0x17
#define THERMISTOR_REGISTER 0x0E
#define FIRST_PIXEL_REGISTER 0x80
#define LAST_PIXEL_REGISTER 0xFE

#define MIN_THRESHOLD_TEMP -2000
#define MAX_THRESHOLD_TEMP 2000

enum class FPS_MODE : uint8_t {
    FPS_10_MODE = 0,
    FPS_1_MODE = 1
}

enum class INT_MODE : uint8_t {
    DIFFERENCE = 0,
    ABSOLUTE_VALUE = 1
}

enum class MP_AMG8833_ERROR_CODE : int {
    NO_ERROR = 0,
    ERROR_READING = -1,
    ERROR_WRITING = -2,
    ARGUMENT_ERROR = -3
}

class MP_AMG8833 {
    //Members
    public:
        uint8_t i2cAddress;
        uint8_t bus;

        float pixelMatrix[8][8];
        float thermistorTemperature;

        bool interruptTable[8][8];

    //Methods
    public:
        MP_AMG8833(uint8_t i2cAddr = I2C_ADDRESS_A);

        int setFPSMode(FPS_MODE mode);
        int getFPSMode();

        int updatePixelMatrix();
        int updateThermistorTemperature();

        int enableInterrupt(bool enable = true, INT_MODE mode = INT_MODE::ABSOLUTE_VALUE);
        int setInterruptThreshold(float lowThreshold, float highThreshold);
        int updateInterruptTable();

        int readByte(uint8_t registerAddress)
        int writeByte(uint8_t registerAddress, uint8_t value);

        String getErrorDescription(int errorCode);

    private:
        float parsePixel(uint8_t lsb, uint8_t msb);
        uint16_t to12bitFormat(float temp);
}

#endif // MP_AMG8833_H_INCLUDED
