/**
 * YM2612 test code for AVR.
 * 
 * This program is a simple test code for the YM2612 FM sound chip using an AVR ATmega328p mcu.
 * This program configure the YM2612 to sound like a "grand piano" and play note on / note off in loop.
 * For more informations about wiring please see: http://en.wikipedia.org/wiki/Yamaha_YM2612
 * For more informations about YM2612 registers please see: http://www.smspower.org/maxim/Documents/YM2612
 *
 * @remarks This test code is heavly based on Furrtek's YM2612 test code. Big thanks Furrtek for the help !
 * @warning This test code is made to run on an ATmega328/ATmega168 mcu with a 16MHz external crystal.
 * 
 * @author Fabien Batteix <skywodd@gmail.com>
 * @link http://skyduino.wordpress.com My Blog about electronics
 */
#ifndef __YM2612_H__
#define __YM2612_H__
#include "config.h"

/* Pinmap (Arduino UNO compatible) */
#if (defined ARDUINO_MEGA)
#define YM_IC 			(6) 		// PC5 (= pin A5 for Arduino UNO)
#define YM_CS 			(5) 		// PC4 (= pin A4 for Arduino UNO)
#define YM_WR 			(4) 		// PC3 (= pin A3 for Arduino UNO)
#define YM_RD 			(3) 		// PC2 (= pin A2 for Arduino UNO)
#define YM_A0 			(2) 		// PC1 (= pin A1 for Arduino UNO)
#define YM_A1 			(1) 		// PC0 (= pin A0 for Arduino UNO)
#define YM_CTRL_DDR DDRF
#define YM_CTRL_PORT PORTF
#define YM_DATA_DDR DDRA
#define YM_DATA_PORT PORTA
#endif

const uint8_t YM2612_PINS[] { YM_IC, YM_CS, YM_WR, YM_RD, YM_A0, YM_A1 };

// Private function
void _write_ym(uint8_t data);

void YM2612Reset();

/**
 * Write data into a specific register of the YM2612 channels 1-3
 *
 * @author Andres Mata
 * @param reg Destination register address
 * @param data Data to write
 */
void YM2612WritePort0(uint8_t reg, uint8_t data);

/**
 * Write data into a specific register of the YM2612 channels 4-6
 *
 * @author Andres Mata
 * @param reg Destination register address
 * @param data Data to write
 */
void YM2612WritePort1(uint8_t reg, uint8_t data);

void YM2612Off();

#endif
