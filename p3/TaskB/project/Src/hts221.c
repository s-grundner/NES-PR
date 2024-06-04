#include "hts221.h"

#define I2C_TIMEOUT HAL_MAX_DELAY

// --- HTS221 Functions ---
// Initialize

HAL_StatusTypeDef HTS221_Init(HTS221_HandleTypeDef* hhts221, I2C_HandleTypeDef* hi2c, uint8_t address)
{
	hhts221->hi2c = hi2c;
	hhts221->address = address;
	hhts221->status = 0;
	hhts221->temperature = 0;
	hhts221->humidity = 0;

	// Check if Sensor is connected
	uint8_t who_am_i;
	if (HTS221_ReadRegister(hhts221, HTS221_WHO_AM_I, &who_am_i)) return HAL_ERROR;
	if (who_am_i != HTS221_WHO_AM_I_VAL) return HAL_ERROR;

	// Configure Sensor
	if (HTS221_WriteRegister(hhts221, HTS221_CTRL_REG1, ODR0 | PD) != HAL_OK) return HAL_ERROR;

	// Read Calibration Values
	uint8_t i2c_buf[4], t_msb; // Temporary buffer for I2C data

	// i2c_buf[0:3] <- 0x30 - 0x33 (H0_rH_x2, H1_rH_x2, T0_degC_x8, T1_degC_x8)
	// t_msb <- 0x35 (T1_T0_MSB)
	if (HTS221_ReadRegisters(hhts221, HTS221_H0_rH_x2, i2c_buf, 4) != HAL_OK) return HAL_ERROR;
	if (HTS221_ReadRegister(hhts221, HTS221_T1_T0_MSB, &t_msb) != HAL_OK) return HAL_ERROR;
	hhts221->cal.H0_rH = i2c_buf[0] >> 1; // Divide by 2
	hhts221->cal.H1_rH = i2c_buf[1] >> 1;
	hhts221->cal.T0_degC = (((uint16_t)(t_msb & 0x03) << 8) | i2c_buf[2]) >> 3; // Prepend MSB and Divide by 8
	hhts221->cal.T1_degC = (((uint16_t)(t_msb & 0x0C) << 6) | i2c_buf[3]) >> 3;

	// i2c_buf[0:1] <- 0x36 - 0x37 (H0_T0_OUT)
	// i2c_buf[2:3] <- 0x3A - 0x3B (H1_T0_OUT)
	if (HTS221_ReadRegisters(hhts221, HTS221_H0_T0_OUT_L, i2c_buf, 2) != HAL_OK) return HAL_ERROR;
	if (HTS221_ReadRegisters(hhts221, HTS221_H1_T0_OUT_L, i2c_buf + 2, 2) != HAL_OK) return HAL_ERROR;
	hhts221->cal.H0_T0_out = ((uint16_t)i2c_buf[1] << 8 | i2c_buf[0]);
	hhts221->cal.H1_T0_out = ((uint16_t)i2c_buf[3] << 8 | i2c_buf[2]);

	// i2c_buf[0:3] <- 0x3C - 0x3E, (T0_OUT, T1_OUT)
	if (HTS221_ReadRegisters(hhts221, HTS221_T0_OUT_L, i2c_buf, 4) != HAL_OK) return HAL_ERROR;
	hhts221->cal.T0_out = ((uint16_t)i2c_buf[1] << 8 | i2c_buf[0]);
	hhts221->cal.T1_out = ((uint16_t)i2c_buf[3] << 8 | i2c_buf[2]);

	return HAL_OK;
}

// Read / Write General Purpose Registers (Wrapper Functions)

HAL_StatusTypeDef HTS221_ReadRegister(HTS221_HandleTypeDef* hhts221, uint8_t reg, uint8_t* data)
{
	return HAL_I2C_Mem_Read(hhts221->hi2c, hhts221->address, reg, I2C_MEMADD_SIZE_8BIT, data, 1, I2C_TIMEOUT);
}

HAL_StatusTypeDef HTS221_ReadRegisters(HTS221_HandleTypeDef* hhts221, uint8_t reg, uint8_t* data, uint16_t size)
{
	// Set MSB to 1 for auto-increment
	return HAL_I2C_Mem_Read(hhts221->hi2c, hhts221->address, 0x80 | reg, I2C_MEMADD_SIZE_8BIT, data, size, I2C_TIMEOUT);
}

HAL_StatusTypeDef HTS221_WriteRegister(HTS221_HandleTypeDef* hhts221, uint8_t reg, uint8_t data)
{
	return HAL_I2C_Mem_Write(hhts221->hi2c, hhts221->address, reg, I2C_MEMADD_SIZE_8BIT, &data, 1, I2C_TIMEOUT);
}

// Read / Write Specific Registers

HAL_StatusTypeDef HTS221_ReadStatus(HTS221_HandleTypeDef* hhts221)
{
	return HTS221_ReadRegister(hhts221, HTS221_STATUS_REG, &hhts221->status);
}

HAL_StatusTypeDef HTS221_ReadTemperature(HTS221_HandleTypeDef* hhts221)
{
	uint8_t data[2] = { 0, 0 };

	// Burst read temperature registers (Status Register is cleared automatically)
	if (HTS221_ReadRegisters(hhts221, HTS221_TEMP_OUT_L, data, 2) != HAL_OK) return HAL_ERROR;
	uint16_t raw_temp = ((int16_t)data[1] << 8 | (int16_t)data[0]);

	// Interpolate temperature using formula (2) from the Technical Note [TN1218, p. 6]
	// Multiply by 10 to get one decimal place
	uint32_t tmp32 = ((int32_t)(raw_temp - hhts221->cal.T0_out)) * ((int32_t)(hhts221->cal.T1_degC - hhts221->cal.T0_degC)) * 10;
	hhts221->temperature = tmp32 / (hhts221->cal.T1_out - hhts221->cal.T0_out) + hhts221->cal.T0_degC * 10;

	return HAL_OK;
}

HAL_StatusTypeDef HTS221_ReadHumidity(HTS221_HandleTypeDef* hhts221)
{
	uint8_t data[2] = { 0, 0 };

	// Burst read humidity registers (Status Register is cleared automatically)
	if (HTS221_ReadRegisters(hhts221, HTS221_TEMP_OUT_L, data, 2) != HAL_OK) return HAL_ERROR;
	int16_t raw_hum = (int16_t)((uint16_t)data[1] << 8 | (uint16_t)data[0]);

	// Interpolate humidity using formula (1) from the Technical Note [TN1218, p. 2]
	int32_t tmp32 = (int32_t)(raw_hum - hhts221->cal.H0_T0_out) * (int32_t)(hhts221->cal.H1_rH - hhts221->cal.H0_rH) * 10;
	hhts221->humidity = (tmp32 / (hhts221->cal.H1_T0_out - hhts221->cal.H0_T0_out)) + hhts221->cal.H0_rH * 10;

	if (hhts221->humidity > 999) hhts221->humidity = 999; // Clamp to 99.9%

	return HAL_OK;
}