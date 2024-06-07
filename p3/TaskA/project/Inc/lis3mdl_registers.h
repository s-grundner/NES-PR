/**
 * @file lis3mdl_registers.h
 * @brief Register Map for LIS3MDL Magnetometer
 */

#ifndef LIS3MDL_REGISTERS_H
#define LIS3MDL_REGISTERS_H

#define LIS3MDL_WHO_AM_I 0x0F
#define LIS3MDL_WHO_AM_I_VALUE 0x3D

 // Control register 1
#define LIS3MDL_CTRL_REG1 0x20
// Output data rate Bits
#define DO0 (1 << 2)
#define DO1 (1 << 3)
#define DO2 (1 << 4)
// Operating mode Bits (XY)
#define OM0 (1 << 5)
#define OM1 (1 << 6)
// Temperature sensor enable
#define TEMP_EN (1 << 7) 			

#define OM_UHP (OM0 | OM1) 			// Ultra-high performance mode (XY)
#define DO_5HZ (DO0 | DO1) 				// 5 Hz output data rate
#define DO_80HZ (DO0 | DO1 | DO2) 	// 80 Hz output data rate

// Control register 2
#define LIS3MDL_CTRL_REG2 0x21
// Full scale Bits
#define FS0_BIT 5
// Full scale Masks
#define FS0 (1 << FS0_BIT)
#define FS1 (1 << 6)

#define FS_4GAUSS (0)			// ± 4 Gauss Sensitivity
#define FS_8GAUSS (FS0) 		// ± 8 Gauss Sensitivity
#define FS_12GAUSS (FS1)   		// ±12 Gauss Sensitivity
#define FS_16GAUSS (FS0 | FS1) 	// ±16 Gauss Sensitivity and FS-Mask

// Control register 3
#define LIS3MDL_CTRL_REG3 0x22
// Mode selection Masks
#define MD0 (1 << 0) 
#define MD1 (1 << 1)

#define MD_CONTINUOUS (0)	// Continuous-conversion mode

// Control register 4
#define LIS3MDL_CTRL_REG4 0x23
// Operating mode Masks (Z)
#define OMZ0 (1 << 2)
#define OMZ1 (1 << 3)

#define OMZ_UHP (OMZ0 | OMZ1) // Ultra-high performance mode (Z)

// Status Register
#define LIS3MDL_STATUS_REG 0x27
#define ZYXDA (1 << 3) // ZYX data available
#define ZYXOR (1 << 7) // ZYX data overrun

// Output Registers
#define LIS3MDL_OUT_X_L 0x28
#define LIS3MDL_OUT_X_H 0x29
#define LIS3MDL_OUT_Y_L 0x2A
#define LIS3MDL_OUT_Y_H 0x2B
#define LIS3MDL_OUT_Z_L 0x2C
#define LIS3MDL_OUT_Z_H 0x2D

#endif // #ifndef LIS3MDL_REGISTERS_H