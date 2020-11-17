#ifndef HALLEFFECTSENSOR_H
#define HALLEFFECTSENSOR_H

#include <msp430.h>
#include <stddef.h>

#define SENSOR_BIT                      BIT1
#define SENSOR_PORT                     P1IN
#define SENSOR_DDR                      P1DIR
#define SET_SENSOR_AS_AN_INPUT         SENSOR_DDR &= ~SENSOR_BIT

#endif
