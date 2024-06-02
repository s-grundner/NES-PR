#include "hts221.h"

#define I2C_TIMEOUT 500

// --- HTS221 Functions ---
// Initialize

HAL_StatusTypeDef HTS221_Init(HTS221_HandleTypeDef *hhts221, I2C_HandleTypeDef *hi2c, uint8_t address)
{
	hhts221->hi2c = hi2c;
	hhts221->address = address;
	hhts221->temperature = 0;
	hhts221->humidity = 0;

	return HAL_OK;
}

// Read / Write General Purpose Registers

HAL_StatusTypeDef HTS221_ReadRegister(HTS221_HandleTypeDef *hhts221, uint8_t reg, uint8_t *data)
{
	return HAL_I2C_Mem_Read(hhts221->hi2c, hhts221->address, reg, I2C_MEMADD_SIZE_8BIT, data, 1, I2C_TIMEOUT);
}

HAL_StatusTypeDef HTS221_ReadRegisters(HTS221_HandleTypeDef *hhts221, uint8_t reg, uint8_t *data, uint16_t size)
{
	return HAL_I2C_Mem_Read(hhts221->hi2c, hhts221->address, reg, I2C_MEMADD_SIZE_8BIT, data, size, I2C_TIMEOUT);
}

HAL_StatusTypeDef HTS221_WriteRegister(HTS221_HandleTypeDef *hhts221, uint8_t reg, uint8_t data)
{
	return HAL_I2C_Mem_Write(hhts221->hi2c, hhts221->address, reg, I2C_MEMADD_SIZE_8BIT, &data, 1, I2C_TIMEOUT);
}

// Read / Write Specific Registers

HAL_StatusTypeDef HTS221_ReadTemperature(HTS221_HandleTypeDef *hhts221)
{
	return HAL_OK;
}

HAL_StatusTypeDef HTS221_ReadHumidity(HTS221_HandleTypeDef *hhts221)
{
	return HAL_OK;
}