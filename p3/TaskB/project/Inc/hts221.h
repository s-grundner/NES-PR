#ifndef HTS221_H
#define HTS221_H

#include "hts221_registers.h"
#include "stm32h7xx_hal.h"
#include "i2c.h"

typedef struct
{
	I2C_HandleTypeDef *hi2c;
	uint8_t address;
	int16_t temperature;
	int16_t humidity;
} HTS221_HandleTypeDef;

// --- HTS221 Functions ---
// Initialize

HAL_StatusTypeDef HTS221_Init(HTS221_HandleTypeDef *hhts221, I2C_HandleTypeDef *hi2c, uint8_t address);

// Read / Write General Purpose Registers

HAL_StatusTypeDef HTS221_ReadRegister(HTS221_HandleTypeDef *hhts221, uint8_t reg, uint8_t *data);
HAL_StatusTypeDef HTS221_ReadRegisters(HTS221_HandleTypeDef *hhts221, uint8_t reg, uint8_t *data, uint16_t size);
HAL_StatusTypeDef HTS221_WriteRegister(HTS221_HandleTypeDef *hhts221, uint8_t reg, uint8_t data);

// Read / Write Specific Registers

HAL_StatusTypeDef HTS221_ReadTemperature(HTS221_HandleTypeDef *hhts221);
HAL_StatusTypeDef HTS221_ReadHumidity(HTS221_HandleTypeDef *hhts221);

#endif // HTS221_H