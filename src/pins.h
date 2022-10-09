#ifndef Pins_Arduino_h
#define Pins_Arduino_h
 
#include "../generic/common.h"
 
#define PIN_WIRE_SDA (4)
#define PIN_WIRE_SCL (5)
 
static const uint8_t SDA = PIN_WIRE_SDA;
static const uint8_t SCL = PIN_WIRE_SCL;
 
static const uint8_t LED_BUILTIN = 16;
static const uint8_t BUILTIN_LED = 16;
 
static const uint8_t D0   = 16;
static const uint8_t D1   = 5;
static const uint8_t D2   = 4;
static const uint8_t D3   = 0;
static const uint8_t D4   = 2;
static const uint8_t D5   = 14;
static const uint8_t D6   = 12;
static const uint8_t D7   = 13;
static const uint8_t D8   = 15;
static const uint8_t D9   = 3;
static const uint8_t D10  = 1;
 
#endif /* Pins_Arduino_h */

#define PIN_SPI_SS   (15)
#define PIN_SPI_MOSI (13)
#define PIN_SPI_MISO (12)
#define PIN_SPI_SCK  (14)
 
static const uint8_t SS    = PIN_SPI_SS;
static const uint8_t MOSI  = PIN_SPI_MOSI;
static const uint8_t MISO  = PIN_SPI_MISO;
static const uint8_t SCK   = PIN_SPI_SCK;
 
#define PIN_A0 (17)
static const uint8_t A0 = PIN_A0;