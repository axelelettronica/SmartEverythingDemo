/*
 * controller.h
 *
 * Created: 02/01/2015 01:48:14
 *  Author: speirano
 */ 


#ifndef SME_I2C_TASK_H_
#define SME_I2C_TASK_H_


/*
* Possible command for the I2C sensors.
* some of the command need an extra buffer to be completed
*/
typedef enum {
	allSensorsReadValue,
	sensorReadValue,
	sensorReadRegister,
	sensorWriteRegister
} sensorCommandE;

typedef union {
	struct {
		uint8_t sensorId;
		uint8_t i2cRegister;
		uint8_t r_w;
		uint8_t *data;
	}fields;
	uint8_t message[5];
}messageU;

typedef struct {
	sensorCommandE code; // code of command to execute to the sensor
	messageU   command;     // must be allocated and released by the caller
}i2cQueueS;

//! Queue for incoming execution command
extern xQueueHandle i2cCommandQueue;






#endif /* SME_I2C_H_ */