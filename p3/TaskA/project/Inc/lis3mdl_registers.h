/**
 * @file lis3mdl_registers.h
 * @brief Register Map for LIS3MDL Magnetometer (Only necessary registers are defined)
 */

#ifndef LIS3MDL_REGISTERS_H
#define LIS3MDL_REGISTERS_H

// ------------------------------
// Control register 1
// ------------------------------

#define LIS3MDL_CTRL_REG1 0x20
// Output data rate Bits
#define DO0 0x02
#define DO1 0x03
#define DO2 0x04
// Operating mode Bits (XY)
#define OM0 0x05
#define OM1 0x06

#define OM_UHP (OM0 | OM1)				// Ultra-high performance mode (XY)
#define DO_80HZ (DO0 | DO1 | DO2) // 80 Hz output data rate

// ------------------------------
// Control register 2
// ------------------------------

#define LIS3MDL_CTRL_REG2 0x21
// Full scale Bits
#define FS0 0x00
#define FS1 0x01

#define FS_4GAUSS (0)					 // ± 4 Gauss Sensitivity
#define FS_8GAUSS (FS0)				 // ± 8 Gauss Sensitivity
#define FS_12GAUSS (FS1)			 // ±12 Gauss Sensitivity
#define FS_16GAUSS (FS0 | FS1) // ±16 Gauss Sensitivity

// ------------------------------
// Control register 4
// ------------------------------

#define LIS3MDL_CTRL_REG4 0x23
// Operating mode Bits (Z)
#define OMZ0 0x02
#define OMZ1 0x03

#define OMZ_UHP (OMZ0 | OMZ1) // Ultra-high performance mode (Z)

// ------------------------------
// Status Register
// ------------------------------

#define LIS3MDL_STATUS_REG 0x27
#define ZYXDA 0x03 // ZYX data available

// ------------------------------
// Output Registers
// ------------------------------

#define LIS3MDL_OUT_X_L 0x28
#define LIS3MDL_OUT_X_H 0x29
#define LIS3MDL_OUT_Y_L 0x2A
#define LIS3MDL_OUT_Y_H 0x2B
#define LIS3MDL_OUT_Z_L 0x2C
#define LIS3MDL_OUT_Z_H 0x2D

#endif // #ifndef LIS3MDL_REGISTERS_H