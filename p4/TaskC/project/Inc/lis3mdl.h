#ifndef LIS2MDL
#define LIS3MDL

/**
 * @brief Basic LIS3MDL Magnetometer driver
 *
 */

#include "stm32h7xx_hal.h"
#include "i2c.h"
#include "lis3mdl_registers.h"

typedef struct
{
	I2C_HandleTypeDef *hi2c;
	uint8_t address;
	uint8_t status;
	int16_t sensitivity;
	int16_t x; // x-axes magnetic field strength in mGauss
	int16_t y; // y-axes magnetic field strength in mGauss
	int16_t z; // z-axes magnetic field strength in mGauss
} LIS3MDL_HandleTypeDef;

// --- LIS3MDL Functions ---
// Initialize

HAL_StatusTypeDef LIS3MDL_Init(LIS3MDL_HandleTypeDef *hlis3mdl, I2C_HandleTypeDef *hi2c, uint8_t address);

HAL_StatusTypeDef LIS3MDL_ReadRegister(LIS3MDL_HandleTypeDef *hlis3mdl, uint8_t reg, uint8_t *data);
HAL_StatusTypeDef LIS3MDL_ReadRegisters(LIS3MDL_HandleTypeDef *hlis3mdl, uint8_t reg, uint8_t *data, uint16_t size);

HAL_StatusTypeDef LIS3MDL_WriteRegister(LIS3MDL_HandleTypeDef *hlis3mdl, uint8_t reg, uint8_t data);
HAL_StatusTypeDef LIS3MDL_WriteRegisters(LIS3MDL_HandleTypeDef *hlis3mdl, uint8_t reg, uint8_t *data, uint16_t size);

HAL_StatusTypeDef LIS3MDL_ReadXYZ(LIS3MDL_HandleTypeDef *hlis3mdl);
HAL_StatusTypeDef LIS3MDL_ReadStatus(LIS3MDL_HandleTypeDef *hlis3mdl);

HAL_StatusTypeDef LIS3MDL_ReadXYZ_IT(LIS3MDL_HandleTypeDef *hlis3mdl);
HAL_StatusTypeDef LIS3MDL_ReadStatus_IT(LIS3MDL_HandleTypeDef *hlis3mdl);

#endif // #ifndef LIS3MDL