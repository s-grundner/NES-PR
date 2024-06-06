#include "lis3mdl.h"

#define I2C_TIMEOUT HAL_MAX_DELAY

HAL_StatusTypeDef LIS3MDL_Init(LIS3MDL_HandleTypeDef* hlis3mdl, I2C_HandleTypeDef* hi2c, uint8_t address)
{
	hlis3mdl->hi2c = hi2c;
	hlis3mdl->address = address;
	hlis3mdl->status = 0;
	hlis3mdl->x = 0;
	hlis3mdl->y = 0;
	hlis3mdl->z = 0;

	// Check if the device is connected
	uint8_t whoami = 0;
	if (LIS3MDL_ReadRegister(hlis3mdl, LIS3MDL_WHO_AM_I, &whoami) != HAL_OK) return HAL_ERROR;
	if (whoami != LIS3MDL_WHO_AM_I_VALUE) return HAL_ERROR;

	// Configure device
	if (LIS3MDL_WriteRegister(hlis3mdl, LIS3MDL_CTRL_REG1, DO_80HZ | OM_UHP | TEMP_EN) != HAL_OK) return HAL_ERROR;
	if (LIS3MDL_WriteRegister(hlis3mdl, LIS3MDL_CTRL_REG2, FS_12GAUSS) != HAL_OK) return HAL_ERROR;
	if (LIS3MDL_WriteRegister(hlis3mdl, LIS3MDL_CTRL_REG3, MD_CONTINUOUS) != HAL_OK) return HAL_ERROR;
	if (LIS3MDL_WriteRegister(hlis3mdl, LIS3MDL_CTRL_REG4, OMZ_UHP) != HAL_OK) return HAL_ERROR;

	return HAL_OK;
}

// Read / Write Generic Registers

HAL_StatusTypeDef LIS3MDL_ReadRegister(LIS3MDL_HandleTypeDef* hlis3mdl, uint8_t reg, uint8_t* data)
{
	return HAL_I2C_Mem_Read(hlis3mdl->hi2c, hlis3mdl->address, reg, I2C_MEMADD_SIZE_8BIT, data, 1, I2C_TIMEOUT);
}

HAL_StatusTypeDef LIS3MDL_ReadRegisters(LIS3MDL_HandleTypeDef* hlis3mdl, uint8_t reg, uint8_t* data, uint16_t size)
{
	return HAL_I2C_Mem_Read(hlis3mdl->hi2c, hlis3mdl->address, reg, I2C_MEMADD_SIZE_8BIT, data, size, I2C_TIMEOUT);
}

HAL_StatusTypeDef LIS3MDL_WriteRegister(LIS3MDL_HandleTypeDef* hlis3mdl, uint8_t reg, uint8_t data)
{
	return HAL_I2C_Mem_Write(hlis3mdl->hi2c, hlis3mdl->address, reg, I2C_MEMADD_SIZE_8BIT, &data, 1, I2C_TIMEOUT);
}

// Read / Write Specific Registers

HAL_StatusTypeDef LIS3MDL_ReadStatus(LIS3MDL_HandleTypeDef* hlis3mdl)
{
	return LIS3MDL_ReadRegister(hlis3mdl, LIS3MDL_STATUS_REG, &hlis3mdl->status);
}

HAL_StatusTypeDef LIS3MDL_ReadXYZ(LIS3MDL_HandleTypeDef* hlis3mdl)
{
	// Burst read 6 bytes starting from OUT_X_L
	uint8_t data[6] = { 0 };
	HAL_StatusTypeDef err = LIS3MDL_ReadRegisters(hlis3mdl, LIS3MDL_OUT_X_L, data, 6);

	if (err != HAL_OK) return err;

	// Combine the 8-bit high and low bytes into 16-bit values
	hlis3mdl->x = (data[1] << 8) | data[0];
	hlis3mdl->y = (data[3] << 8) | data[2];
	hlis3mdl->z = (data[5] << 8) | data[4];

	return HAL_OK;
}
