#include "lis3mdl.h"

#define I2C_TIMEOUT HAL_MAX_DELAY

// === LIS3MDL Functions ===
// --- Private Functions ---

HAL_StatusTypeDef __LIS3MDL_Set_Sensitivity(LIS3MDL_HandleTypeDef *hlis3mdl)
{
	// Set the sensitivity based on the full scale range
	uint8_t fs_data = 0;
	if (LIS3MDL_ReadRegister(hlis3mdl, LIS3MDL_CTRL_REG2, &fs_data) != HAL_OK)
		return HAL_ERROR;

	fs_data &= FS_16GAUSS; // Mask out the FS bits

	switch (fs_data)
	{
	case FS_4GAUSS:
		hlis3mdl->sensitivity = ((1 << 15) - 1) / 4; // LSB/Gauss
		break;
	case FS_8GAUSS:
		hlis3mdl->sensitivity = ((1 << 15) - 1) / 8; // LSB/Gauss
		break;
	case FS_12GAUSS:
		hlis3mdl->sensitivity = ((1 << 15) - 1) / 12; // LSB/Gauss
		break;
	case FS_16GAUSS:
	default:
		hlis3mdl->sensitivity = ((1 << 15) - 1) / 16; // LSB/Gauss
		break;
	}
	return HAL_OK;
}

void __LIS2MDL_ConvertToMGs(uint8_t *data, LIS3MDL_HandleTypeDef *hlis3mdl)
{
	// Combine the 8-bit high and low bytes into 16-bit values
	int16_t raw_x = ((uint16_t)data[1] << 8) | data[0];
	int16_t raw_y = ((uint16_t)data[3] << 8) | data[2];
	int16_t raw_z = ((uint16_t)data[5] << 8) | data[4];

	// Convert to mGs
	hlis3mdl->x = 1000 * raw_x / hlis3mdl->sensitivity;
	hlis3mdl->y = 1000 * raw_y / hlis3mdl->sensitivity;
	hlis3mdl->z = 1000 * raw_z / hlis3mdl->sensitivity;
}

// --- Public Functions ---
// Initialization

HAL_StatusTypeDef LIS3MDL_Init(LIS3MDL_HandleTypeDef *hlis3mdl, I2C_HandleTypeDef *hi2c, uint8_t address)
{
	hlis3mdl->hi2c = hi2c;
	hlis3mdl->address = address;
	hlis3mdl->status = 0;
	hlis3mdl->x = 0;
	hlis3mdl->y = 0;
	hlis3mdl->z = 0;

	// Check if the device is connected
	uint8_t whoami = 0;
	if (LIS3MDL_ReadRegister(hlis3mdl, LIS3MDL_WHO_AM_I, &whoami) != HAL_OK)
		return HAL_ERROR;
	if (whoami != LIS3MDL_WHO_AM_I_VALUE)
		return HAL_ERROR;

	// Configure device
	uint8_t cfg_data[4] = {
			DO_5HZ | OM_UHP | TEMP_EN, // CTRL_REG1
			FS_12GAUSS,								 // CTRL_REG2
			MD_CONTINUOUS,						 // CTRL_REG3
			OMZ_UHP										 // CTRL_REG4
	};

	// Write configuration data to control registers 1 to 4
	if (LIS3MDL_WriteRegisters(hlis3mdl, LIS3MDL_CTRL_REG1, cfg_data, 4) != HAL_OK)
		return HAL_ERROR;
	if (__LIS3MDL_Set_Sensitivity(hlis3mdl) != HAL_OK)
		return HAL_ERROR;

	return HAL_OK;
}

// Read / Write Generic Registers

HAL_StatusTypeDef LIS3MDL_ReadRegister(LIS3MDL_HandleTypeDef *hlis3mdl, uint8_t reg, uint8_t *data)
{
	return HAL_I2C_Mem_Read(hlis3mdl->hi2c, hlis3mdl->address, reg, I2C_MEMADD_SIZE_8BIT, data, 1, I2C_TIMEOUT);
}

HAL_StatusTypeDef LIS3MDL_ReadRegisters(LIS3MDL_HandleTypeDef *hlis3mdl, uint8_t reg, uint8_t *data, uint16_t size)
{
	return HAL_I2C_Mem_Read(hlis3mdl->hi2c, hlis3mdl->address, reg, I2C_MEMADD_SIZE_8BIT, data, size, I2C_TIMEOUT);
}

HAL_StatusTypeDef LIS3MDL_WriteRegisters(LIS3MDL_HandleTypeDef *hlis3mdl, uint8_t reg, uint8_t *data, uint16_t size)
{
	return HAL_I2C_Mem_Write(hlis3mdl->hi2c, hlis3mdl->address, reg, I2C_MEMADD_SIZE_8BIT, data, size, I2C_TIMEOUT);
}

HAL_StatusTypeDef LIS3MDL_WriteRegister(LIS3MDL_HandleTypeDef *hlis3mdl, uint8_t reg, uint8_t data)
{
	return HAL_I2C_Mem_Write(hlis3mdl->hi2c, hlis3mdl->address, reg, I2C_MEMADD_SIZE_8BIT, &data, 1, I2C_TIMEOUT);
}

// Read / Write Registers with Interrupts

HAL_StatusTypeDef LIS3MDL_ReadRegister_IT(LIS3MDL_HandleTypeDef *hlis3mdl, uint8_t reg, uint8_t *data)
{
	return HAL_I2C_Mem_Read_IT(hlis3mdl->hi2c, hlis3mdl->address, reg, I2C_MEMADD_SIZE_8BIT, data, 1);
}

HAL_StatusTypeDef LIS3MDL_ReadRegisters_IT(LIS3MDL_HandleTypeDef *hlis3mdl, uint8_t reg, uint8_t *data, uint16_t size)
{
	return HAL_I2C_Mem_Read_IT(hlis3mdl->hi2c, hlis3mdl->address, reg, I2C_MEMADD_SIZE_8BIT, data, size);
}

// Read / Write Specific Registers

HAL_StatusTypeDef LIS3MDL_ReadStatus(LIS3MDL_HandleTypeDef *hlis3mdl)
{
	return LIS3MDL_ReadRegister(hlis3mdl, LIS3MDL_STATUS_REG, &hlis3mdl->status);
}

HAL_StatusTypeDef LIS3MDL_ReadXYZ(LIS3MDL_HandleTypeDef *hlis3mdl)
{
	// Burst read 6 bytes starting from OUT_X_L
	uint8_t data[6];
	if (LIS3MDL_ReadRegisters(hlis3mdl, LIS3MDL_OUT_X_L, data, 6) != HAL_OK)
		return HAL_ERROR;

	__LIS2MDL_ConvertToMGs(data, hlis3mdl);

	return HAL_OK;
}

// Read / Write Specific Registers with Interrupts

HAL_StatusTypeDef LIS3MDL_ReadXYZ_IT(LIS3MDL_HandleTypeDef *hlis3mdl)
{
	// Burst read 6 bytes starting from OUT_X_L
	static uint8_t data[6]; // needs to be preserved to be used from the callback
	if (LIS3MDL_ReadRegisters_IT(hlis3mdl, LIS3MDL_OUT_X_L, data, 6) != HAL_OK)
		return HAL_ERROR;

	__LIS2MDL_ConvertToMGs(data, hlis3mdl);

	return HAL_OK;
}

HAL_StatusTypeDef LIS3MDL_ReadStatus_IT(LIS3MDL_HandleTypeDef *hlis3mdl)
{
	return LIS3MDL_ReadRegister_IT(hlis3mdl, LIS3MDL_STATUS_REG, &hlis3mdl->status);
}