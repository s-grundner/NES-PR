/**
 * @file hts221.h
 * @author @s-grundner
 * @brief HTS221 Basic Driver Header File
 * @date 2024-06-03
 */

#ifndef HTS221_H
#define HTS221_H

#include "hts221_registers.h"
#include "stm32h7xx_hal.h"
#include "i2c.h"

typedef struct {
	uint8_t H0_rH;
	uint8_t H1_rH;

	int16_t H0_T0_out;
	int16_t H1_T0_out;

	uint8_t T0_degC;
	uint8_t T1_degC;

	int16_t T0_out;
	int16_t T1_out;
} HTS221_CalibrationTypeDef;

typedef struct {
	I2C_HandleTypeDef* hi2c;
	uint8_t address;
	uint8_t status;
	int16_t temperature; // in °C
	uint16_t humidity; // in % (Relative)
	HTS221_CalibrationTypeDef cal;
} HTS221_HandleTypeDef;

// --- HTS221 Functions ---
// Initialize

HAL_StatusTypeDef HTS221_Init(HTS221_HandleTypeDef* hhts221, I2C_HandleTypeDef* hi2c, uint8_t address);

// Read / Write General Purpose Registers

HAL_StatusTypeDef HTS221_ReadRegister(HTS221_HandleTypeDef* hhts221, uint8_t reg, uint8_t* data);
HAL_StatusTypeDef HTS221_ReadRegisters(HTS221_HandleTypeDef* hhts221, uint8_t reg, uint8_t* data, uint16_t size);
HAL_StatusTypeDef HTS221_WriteRegister(HTS221_HandleTypeDef* hhts221, uint8_t reg, uint8_t data);

// Read / Write Specific Registers

HAL_StatusTypeDef HTS221_ReadStatus(HTS221_HandleTypeDef* hhts221);
HAL_StatusTypeDef HTS221_ReadTemperature(HTS221_HandleTypeDef* hhts221);
HAL_StatusTypeDef HTS221_ReadHumidity(HTS221_HandleTypeDef* hhts221);

#endif // HTS221_H