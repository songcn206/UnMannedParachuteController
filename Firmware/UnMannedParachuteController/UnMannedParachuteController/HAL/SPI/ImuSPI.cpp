/*
 * SPI.cpp
 *
 * Created: 09.01.2020 21:02:56
 *  Author: timot
 */ 

#include "ImuSPI.hpp"

volatile ImuSpi :: SpiState ImuSpi :: state = ImuSpi :: SpiState :: Uninited;
volatile uint8_t ImuSpi :: data[18];
volatile uint8_t ImuSpi :: dataPointer = 0;
volatile uint8_t ImuSpi :: settingsPointer = 0;
volatile bool ImuSpi :: DataWritten;
volatile uint8_t ImuSpi :: magnetometerPointer = 0;
volatile bool ImuSpi :: gotIDFirstTime = false;

volatile uint8_t ImuSpi :: DataAddr[18]= {
45,
46,
47,
48,
49,
50,	
51,
52,
53,
54,
55,
56,
59,
60,
61,
62,
63,
64
};

struct ImuSpi :: ImuSettings s0 = {.addr = 5, .value = 0b01000000}; // LP_GONFIG: I2C master duty cycle
struct ImuSpi :: ImuSettings s1 = {.addr = 6, .value = 0b00000001}; // PWR_MGMT_1: Auto select clock
struct ImuSpi :: ImuSettings s2 = {.addr = 7, .value = 0};			// PWR_MGMT_2: Enable cc and gyro	

struct ImuSpi :: ImuSettings s3 = {.addr = 127, .value = 0b00100000}; // REG_BANK_SEL: Change to 2
struct ImuSpi :: ImuSettings s4 = {.addr = 1, .value = 0b00000011}; // GYRO_CONFIG: +-500dps
struct ImuSpi :: ImuSettings s5 = {.addr = 20, .value = 0b00000001}; // ACCEL_CONFIG: +-2g
struct ImuSpi :: ImuSettings s6 = {.addr = 127, .value = 0};		// REG_BANK_SEL: Change to bank 0
struct ImuSpi :: ImuSettings s7 = {.addr = 3, .value = 0b00110000}; // USER_CTRL: Enable I2C master
struct ImuSpi :: ImuSettings s8 = {.addr = 127, .value = 0b00110000}; // REG_BANK_SEL: Change to bank 3
	
struct ImuSpi :: ImuSettings s9 = {.addr = 1, .value = 0b00000111}; // I2C_MST_CTRL: Set clock register to 7 (345kHz)
struct ImuSpi :: ImuSettings s10 = {.addr = 2, .value = 0b00000001}; // I2C_MST_DELAY_CTRL: slave access delay
struct ImuSpi :: ImuSettings s11 = {.addr = 5, .value = 0b10001000}; // I2C_SLV0_CTRL: slave 0 enable and bytes read from slave

struct ImuSpi :: ImuSettings s12 = {.addr = 3, .value = 0b00001100}; // I2C_SLV0_ADDR: Physical address (0x0c) and write data
struct ImuSpi :: ImuSettings s13 = {.addr = 4, .value = 0x32};		// I2C_SLV0_REG: Slave control2 register
struct ImuSpi :: ImuSettings s14 = {.addr = 6, .value = 0b00000001}; // I2C_SLV0_DO: Restart

struct ImuSpi :: ImuSettings s15 = {.addr = 3, .value = 0b00001100}; // I2C_SLV0_ADDR: Device physical address (0x0c) and write data
struct ImuSpi :: ImuSettings s16 = {.addr = 4, .value = 0x31};		// I2C_SLV0_REG: Slave control1 register
struct ImuSpi :: ImuSettings s17 = {.addr = 6, .value = 0b00000100}; // I2C_SLV0_DO: Continuous mode 2 (20Hz)
	
struct ImuSpi :: ImuSettings s18 = {.addr = 3, .value = 0b10001100}; // I2C_SLV0_ADDR: Device physical address (0x0c) and read data
struct ImuSpi :: ImuSettings s19 = {.addr = 4, .value = 0x11};		// I2C_SLV0_REG: Slave first data register	
struct ImuSpi :: ImuSettings s20 = {.addr = 6, .value = 0b11111111}; // I2C_SLV0_DO: Dummy data
	
struct ImuSpi :: ImuSettings s21 = {.addr = 127, .value = 0};		// REG_BANK_SEL: Change to bank 0
	
volatile ImuSpi :: ImuSettings ImuSpi :: settings[16] = {
s0,
s1,
s3,
s2,
s4,
s5,
s6,
s7,
s8,
s9,
s10,
s11,
//s12,
//s13,
//s14,
s15,
s16,
s17,
//s18,
//s19,
//s20,
s21
};

volatile ImuSpi :: ImuSettings ImuSpi ::magnetometer[5] = {
s8,
s18,
s19,
s20,
s21
};


ISR(SPIE_INT_vect) {
	ImuSpi ::InterruptHandler();
}