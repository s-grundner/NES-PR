/**
 *@file lps22hh.h
 * @author @s-grundner
 * @brief Basic LSP22HH Pressure Sensor Driver
 * @date 2024-06-04
 */

#ifndef LPS22HH_H
#define LPS22HH_H

#include "lps22hh_registers.h"
#include "stm32h7xx_hal.h"
#include "i2c.h"

typedef struct {
	I2C_HandleTypeDef* i2c;
	uint8_t address;
	uint8_t status;
	int16_t pressure; // in hPa
	int16_t temp; // in degrees C
} LPS22HH_HandleTypeDef;

// --- LPS22HH Functions ---
// Initialize

HAL_StatusTypeDef LPS22HH_Init(LPS22HH_HandleTypeDef* hlps22hh, I2C_HandleTypeDef* hi2c, uint8_t address);

// Read / Write General Purpose Registers

HAL_StatusTypeDef LPS22HH_ReadRegister(LPS22HH_HandleTypeDef* hlps22hh, uint8_t reg, uint8_t* data);
HAL_StatusTypeDef LPS22HH_ReadRegisters(LPS22HH_HandleTypeDef* hlps22hh, uint8_t reg, uint8_t* data, uint16_t size);
HAL_StatusTypeDef LPS22HH_WriteRegister(LPS22HH_HandleTypeDef* hlps22hh, uint8_t reg, uint8_t data);

// Read / Write Specific Registers

HAL_StatusTypeDef LPS22HH_ReadStatus(LPS22HH_HandleTypeDef* hlps22hh);
HAL_StatusTypeDef LPS22HH_ReadPressure(LPS22HH_HandleTypeDef* hlps22hh);
HAL_StatusTypeDef LPS22HH_ReadTemperature(LPS22HH_HandleTypeDef* hlps22hh);

#endif // LPS22HH_H