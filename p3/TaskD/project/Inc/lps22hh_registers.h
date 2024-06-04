/**
 * @file lps22hh_registers.h
 * @author @s-grundner
 * @brief LPS22HH Register Definitions
 * @version 0.1
 */

#ifndef LPS22HH_REGISTERS_H
#define LPS22HH_REGISTERS_H

 // --- LPS22HH Registers ---

#define LPS22HH_WHO_AM_I 0x0F
#define LPS22HH_WHO_AM_I_VALUE 0xB3

#define LPS22HH_CTRL_REG1 0x10
#define ODR0 (1 << 4)
#define ODR1 (1 << 5)
#define ODR2 (1 << 6)

#define ODR_1HZ (ODR0)

#define LPS22HH_CTRL_REG2 0x11
#define LPS22HH_CTRL_REG3 0x12

#define LPS22HH_STATUS 0x27
#define T_DA (1 << 1)
#define P_DA (1 << 0)

#define LPS22HH_PRESS_OUT_XL 0x28
#define LPS22HH_PRESS_OUT_L 0x29
#define LPS22HH_PRESS_OUT_H 0x2A

#define LPS22HH_TEMP_OUT_L 0x2B
#define LPS22HH_TEMP_OUT_H 0x2C

#endif // LPS22HH_REGISTERS_H