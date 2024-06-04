#ifndef HTS221_REGISTERS_H
#define HTS221_REGISTERS_H

#define HTS221_WHO_AM_I 0x0F
#define HTS221_WHO_AM_I_VAL 0xBC

#define HTS221_CTRL_REG1 0x20
#define ODR0 (1 << 1) // Output Data Rate
#define PD (1 << 8) // Power Down

#define HTS221_STATUS_REG 0x27
#define T_DA (1 << 0) // Temperature Data Available
#define H_DA (1 << 1) // Humidity Data Available

#define HTS221_HUMIDITY_OUT_L 0x28
#define HTS221_HUMIDITY_OUT_H 0x29

#define HTS221_TEMP_OUT_L 0x2A
#define HTS221_TEMP_OUT_H 0x2B

// Calibration Registers

#define HTS221_H0_rH_x2 0x30
#define HTS221_H1_rH_x2 0x31
#define HTS221_T0_degC_x8 0x32
#define HTS221_T1_degC_x8 0x33

#define HTS221_T1_T0_MSB 0x35

#define HTS221_H0_T0_OUT_L 0x36
#define HTS221_H0_T0_OUT_H 0x37

#define HTS221_H1_T0_OUT_L 0x3A
#define HTS221_H1_T0_OUT_H 0x3B

#define HTS221_T0_OUT_L 0x3C
#define HTS221_T0_OUT_H 0x3D
#define HTS221_T1_OUT_L 0x3E
#define HTS221_T1_OUT_H 0x3F

#endif // HTS221_REGISTERS_H