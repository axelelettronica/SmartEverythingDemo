/*
 * IncFile1.h
 *
 * Created: 02/01/2015 20:50:30
 *  Author: mfontane
 */ 


#ifndef HTS221_H_
#define HTS221_H_

#include <stdbool.h>

bool HTS221nit(void);
bool HTS221getCalibration(void);
bool HTS221getValues(char *buffer);



#endif /* INCFILE1_H_ */