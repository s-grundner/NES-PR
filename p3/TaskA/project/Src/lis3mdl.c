#include "lis3mdl.h"

#define I2C_TIMEOUT 500

HAL_StatusTypeDef LIS3MDL_Init(LIS3MDL_HandleTypeDef *hlis3mdl, I2C_HandleTypeDef *hi2c, uint8_t address)
{
	hlis3mdl->hi2c = hi2c;
	hlis3mdl->address = address;
	hlis3mdl->x = 0;
	hlis3mdl->y = 0;
	hlis3mdl->z = 0;

	LIS3MDL_WriteRegister(hlis3mdl, LIS3MDL_CTRL_REG1, DO_80HZ | OM_UHP);
	LIS3MDL_WriteRegister(hlis3mdl, LIS3MDL_CTRL_REG2, FS_12GAUSS);
	LIS3MDL_WriteRegister(hlis3mdl, LIS3MDL_CTRL_REG4, OMZ_UHP);

	return HAL_OK;
}

// Read / Write General Purpose Registers

HAL_StatusTypeDef LIS3MDL_ReadRegister(LIS3MDL_HandleTypeDef *hlis3mdl, uint8_t reg, uint8_t *data)
{
	return HAL_I2C_Mem_Read(hlis3mdl->hi2c, hlis3mdl->address, reg, I2C_MEMADD_SIZE_8BIT, data, 1, I2C_TIMEOUT);
}

HAL_StatusTypeDef LIS3MDL_ReadRegisters(LIS3MDL_HandleTypeDef *hlis3mdl, uint8_t reg, uint8_t *data, uint16_t size)
{
	return HAL_I2C_Mem_Read(hlis3mdl->hi2c, hlis3mdl->address, reg, I2C_MEMADD_SIZE_8BIT, data, size, I2C_TIMEOUT);
}

HAL_StatusTypeDef LIS3MDL_WriteRegister(LIS3MDL_HandleTypeDef *hlis3mdl, uint8_t reg, uint8_t data)
{
	return HAL_I2C_Mem_Write(hlis3mdl->hi2c, hlis3mdl->address, reg, I2C_MEMADD_SIZE_8BIT, &data, 1, I2C_TIMEOUT);
}

// Read / Write Specific Registers

HAL_StatusTypeDef LIS3MDL_WriteStatus(LIS3MDL_HandleTypeDef *hlis3mdl, uint8_t status)
{
	return LIS3MDL_WriteRegister(hlis3mdl, LIS3MDL_STATUS_REG, status);
}

HAL_StatusTypeDef LIS3MDL_ReadXYZ(LIS3MDL_HandleTypeDef *hlis3mdl)
{
	// Burst read 6 bytes starting from OUT_X_L
	uint8_t data[6] = {0};
	HAL_StatusTypeDef err = LIS3MDL_ReadRegisters(hlis3mdl, LIS3MDL_OUT_X_L, data, 6);

	if (err != HAL_OK)
	{
		return err;
	}

	// Combine the 8-bit high and low bytes into a 16-bit value
	hlis3mdl->x = (data[1] << 8) | data[0];
	hlis3mdl->y = (data[3] << 8) | data[2];
	hlis3mdl->z = (data[5] << 8) | data[4];

	return HAL_OK;
}
