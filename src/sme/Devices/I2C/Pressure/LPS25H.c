/*
* HTS221.c
*
* Created: 02/01/2015 20:50:20
*  Author: mfontane
*/
#include "LPS25H.h"
#include "..\I2C.h"

#if NOT_SENSOR
// The SparkFun breakout board defaults to 1, set to 0 if SA0 jumper on the bottom of the board is set
#define LPS25H_ADDRESS 85


//Define a few of the registers that we will be accessing on the LPS25H

#define WHO_AM_I    0x00
#define WHO_AM_I_RETURN 0x04 // WHO_AM_I should always be 0x2A

/*
 * [7] PD: power down control.
 * Default value: 0
 * (0: power-down mode; 1: active mode)
 *
 * [6:4] ODR2, ODR1, ODR0: output data rate selection.
 * Default value: 00
 * 
 * [3] DIFF_EN: Interrupt circuit enable.
 * Default value: 0
 * (0: interrupt generation disabled; 1: interrupt circuit enabled)
 * 
 * [2] BDU: block data update.
 * Default value: 0
 * (0: continuous update; 1: output registers not updated until MSB and LSB reading)
 BDU bit is used to inhibit the output registers update between the reading of upper and
 lower register parts. In default mode (BDU = �0�), the lower and upper register parts are
 updated continuously. If it is not sure to read faster than output data rate, it is recommended
 to set BDU bit to �1�. In this way, after the reading of the lower (upper) register part, the
 content of that output registers is not updated until the upper (lower) part
 * 
 * [1] RESET_AZ: Reset AutoZero function. Reset REF_P reg, set pressure to default value in RPDS
 * register (@0x39/A)
 * (1: Reset. 0: disable)
 * 
 * [0] SIM: SPI Serial Interface Mode selection.
 * Default value: 0
 * (0: 4-wire interface; 1: 3-wire interface)
 */
#define CTRL_REG1   0x20
#define POWER_UP    0x80 
#define BDU_SET     0x04



#define CTRL_REG2   0x21
#define CTRL_REG3   0x22
#define REG_DEFAULT 0x00

#define STATUS_REG  0x0D
#define TEMPERATURE_READY 0xA
#define HUMIDITY_READY    0x20

#define HUMIDITY_L_REG 0x13
#define HUMIDITY_H_REG 0x14
#define TEMP_L_REG     0x13
#define TEMP_H_REG     0x14
/*
* calibration registry should be read for temperature and humidity calculation.
* Before the first calculation of temperature and humidity,
* the master reads out the calibration coefficients.
* will do at init phase
*/
#define CALIB_START        0x10
#define CALIB_END	       0x1F
#else

#define LPS25H_ADDRESS 0x2E

#define WHO_AM_I    0x0F
#define WHO_AM_I_RETURN 0xBD // Contains the device ID, BDh

#define RES_CONF_REG 0x10 // Pressure and Temperature internal average configuration.
#define RES_CONF_DEFAULT 0x05  


/*
 * [7] PD: power down control.
 * Default value: 0
 * (0: power-down mode; 1: active mode)
 *
 * [6:4] ODR2, ODR1, ODR0: output data rate selection.
 * Default value: 00
 * 
 * [3] DIFF_EN: Interrupt circuit enable.
 * Default value: 0
 * (0: interrupt generation disabled; 1: interrupt circuit enabled)
 * 
 * [2] BDU: block data update.
 * Default value: 0
 * (0: continuous update; 1: output registers not updated until MSB and LSB reading)
 BDU bit is used to inhibit the output registers update between the reading of upper and
 lower register parts. In default mode (BDU = �0�), the lower and upper register parts are
 updated continuously. If it is not sure to read faster than output data rate, it is recommended
 to set BDU bit to �1�. In this way, after the reading of the lower (upper) register part, the
 content of that output registers is not updated until the upper (lower) part
 * 
 * [1] RESET_AZ: Reset AutoZero function. Reset REF_P reg, set pressure to default value in RPDS
 * register (@0x39/A)
 * (1: Reset. 0: disable)
 * 
 * [0] SIM: SPI Serial Interface Mode selection.
 * Default value: 0
 * (0: 4-wire interface; 1: 3-wire interface)
 */
#define CTRL_REG1   0x20
#define POWER_UP    0x80 
#define BDU_SET     0x04



#define CTRL_REG2   0x21
#define CTRL_REG3   0x22
#define REG_DEFAULT 0x00

/*
 * This register is updated every ODR cycle, regardless of BDU value in CTRL_REG1.
 *
 * P_DA is set to 1 whenever a new pressure sample is available. 
 * P_DA is cleared when PRESS_OUT_H (2Ah) register is read.
 *
 * T_DA is set to 1 whenever a new temperature sample is available. 
 * T_DA is cleared when TEMP_OUT_H (2Ch) register is read.
 *
 * P_OR bit is set to '1' whenever new pressure data is available and P_DA was set in
 * the previous ODR cycle and not cleared. 
 * P_OR is cleared when PRESS_OUT_H (2Ah) register is read.
 *
 * T_OR is set to �1� whenever new temperature data is available and T_DA was set in
 * the previous ODR cycle and not cleared.
 * T_OR is cleared when TEMP_OUT_H (2Ch) register is read.
 * 
 * [7:6] Reserved
 *
 * [5] P_OR: Pressure data overrun. Default value: 0
 * (0: no overrun has occurred;
 * 1: new data for pressure has overwritten the previous one)
 *
 * [4] T_OR: Temperature data overrun. Default value: 0
 * (0: no overrun has occurred;
 * 1: a new data for temperature has overwritten the previous one)
 *
 * [3:2] Reserved
 *
 * [1] P_DA: Pressure data available. Default value: 0
 * (0: new data for pressure is not yet available;
 * 1: new data for pressure is available)
 *
 * [0] T_DA: Temperature data available. Default value: 0
 * (0: new data for temperature is not yet available;
 * 1: new data for temperature is available)
 */
#define STATUS_REG  0x27
#define TEMPERATURE_READY 0x1
#define PRESSURE_READY    0x2

#define HUMIDITY_L_REG 0x28
#define HUMIDITY_H_REG 0x29
#define TEMP_L_REG     0x2A
#define TEMP_H_REG     0x2B
/*
* calibration registry should be read for temperature and humidity calculation.
* Before the first calculation of temperature and humidity,
* the master reads out the calibration coefficients.
* will do at init phase
*/
#define CALIB_START        0x30
#define CALIB_END	       0x3F
#endif
static inline bool humidityReady(uint8_t data) {
	return (data & 0x02);
}
static inline bool temperatureReady(uint8_t data) {
	return (data & 0x01);
}

static uint8_t h0_rH, h1_rH;
static uint16_t T0_degC, T1_degC, H0_T0, H1_T1, T0, T1;


bool LPS25Hnit(void) {
	uint8_t data;
	if (readRegister(LPS25H_ADDRESS, WHO_AM_I, &data)) {
		if (data == WHO_AM_I_RETURN);
		return LPS25HActivate();
		} else {
		return false;
	}
}

bool LPS25HActivate(void) {
	uint8_t data;
	
	if (readRegister(LPS25H_ADDRESS, CTRL_REG1, &data)) {
		data |= POWER_UP;
		if (writeRegister(LPS25H_ADDRESS, CTRL_REG1, data))
			return true;
	}
	return false;
}

bool LPS25HDeactivate(void) {
	uint8_t data;
	
	if (readRegister(LPS25H_ADDRESS, CTRL_REG1, &data)) {
		data &= ~POWER_UP;
		if (writeRegister(LPS25H_ADDRESS, CTRL_REG1, data))
		return true;
	}
	return false;
}

bool LPS25HgetValues(char *buffer){
	uint8_t data;
	if (readRegister(LPS25H_ADDRESS, STATUS_REG, &data)) {
		if (data & TEMPERATURE_READY) {
			if (readRegister(LPS25H_ADDRESS, TEMP_H_REG, &data)) {
				if (readRegister(LPS25H_ADDRESS, TEMP_L_REG, &data)) {
				}
			}
		}
		if (data & HUMIDITY_READY) {
			if (readRegister(LPS25H_ADDRESS, HUMIDITY_H_REG, &data)) {
				if (readRegister(LPS25H_ADDRESS, HUMIDITY_L_REG, &data)) {
				}
			}
		}
		return true;
	} else
	return false;
}

bool LPS25HBDUActivate(void) {
	uint8_t data;
	if (readRegister(LPS25H_ADDRESS, CTRL_REG1, &data)) {
		data |= BDU_SET;
		if (writeRegister(LPS25H_ADDRESS, CTRL_REG1, data))
		return true;
	}
	return false;
}

bool LPS25HBDUDeactivate(void) {
	uint8_t data;
	
	if (readRegister(LPS25H_ADDRESS, CTRL_REG1, &data)) {
		data &= ~BDU_SET;
		if (writeRegister(LPS25H_ADDRESS, CTRL_REG1, data))
		return true;
	}
	return false;
}
