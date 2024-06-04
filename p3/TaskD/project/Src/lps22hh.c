/**
 *@file lps22hh.c
 * @author @s-grundner
 * @brief LPS22HH Driver Source File
 * @date 2024-06-04
 */

#include "lps22hh.h"

#define I2C_TIMEOUT HAL_MAX_DELAY
#define T_LSB 100
#define P_LSB 4096

 // --- LPS22HH Functions ---
 // Initialize

HAL_StatusTypeDef LPS22HH_Init(LPS22HH_HandleTypeDef* hlps22hh, I2C_HandleTypeDef* hi2c, uint8_t address)
{
	hlps22hh->i2c = hi2c;
	hlps22hh->address = address;
	hlps22hh->status = 0.0f;
	hlps22hh->pressure = 0.0f;
	hlps22hh->temp = 0.0f;

	// Check WHO_AM_I Register
	uint8_t who_am_i = 0;
	if (LPS22HH_ReadRegister(hlps22hh, LPS22HH_WHO_AM_I, &who_am_i)) return HAL_ERROR;
	if (who_am_i != LPS22HH_WHO_AM_I_VALUE) return HAL_ERROR;

	// Set Output Data Rate to 1Hz
	if (LPS22HH_WriteRegister(hlps22hh, LPS22HH_CTRL_REG1, ODR_1HZ)) return HAL_ERROR;

	return HAL_OK;
}

// Read / Write General Purpose Registers (Wrapper Functions)

HAL_StatusTypeDef LPS22HH_ReadRegister(LPS22HH_HandleTypeDef* hlps22hh, uint8_t reg, uint8_t* data)
{
	return HAL_I2C_Mem_Read(hlps22hh->i2c, hlps22hh->address, reg, I2C_MEMADD_SIZE_8BIT, data, 1, I2C_TIMEOUT);
}

HAL_StatusTypeDef LPS22HH_ReadRegisters(LPS22HH_HandleTypeDef* hlps22hh, uint8_t reg, uint8_t* data, uint16_t size)
{
	return HAL_I2C_Mem_Read(hlps22hh->i2c, hlps22hh->address, reg, I2C_MEMADD_SIZE_8BIT, data, size, I2C_TIMEOUT);
}

HAL_StatusTypeDef LPS22HH_WriteRegister(LPS22HH_HandleTypeDef* hlps22hh, uint8_t reg, uint8_t data)
{
	return HAL_I2C_Mem_Write(hlps22hh->i2c, hlps22hh->address, reg, I2C_MEMADD_SIZE_8BIT, &data, 1, I2C_TIMEOUT);
}

// Read / Write Specific Registers

HAL_StatusTypeDef LPS22HH_ReadPressure(LPS22HH_HandleTypeDef* hlps22hh)
{
	uint8_t data[3] = { 0, 0, 0 };
	if (LPS22HH_ReadRegisters(hlps22hh, LPS22HH_PRESS_OUT_XL, data, 3)) return HAL_ERROR;

	int32_t raw_press = ((uint32_t)data[2] << 16) | ((uint16_t)data[1] << 8) | data[0];
	hlps22hh->pressure = raw_press / P_LSB;

	return HAL_OK;
}

HAL_StatusTypeDef LPS22HH_ReadTemperature(LPS22HH_HandleTypeDef* hlps22hh)
{
	uint8_t data[2] = { 0, 0 };
	if (LPS22HH_ReadRegisters(hlps22hh, LPS22HH_TEMP_OUT_L, data, 2)) return HAL_ERROR;

	int16_t raw_temp = ((uint16_t)data[1] << 8) | data[0];
	hlps22hh->temp = raw_temp / T_LSB;

	return HAL_OK;
}

HAL_StatusTypeDef LPS22HH_ReadStatus(LPS22HH_HandleTypeDef* hlps22hh)
{
	return LPS22HH_ReadRegister(hlps22hh, LPS22HH_STATUS, &hlps22hh->status);
}