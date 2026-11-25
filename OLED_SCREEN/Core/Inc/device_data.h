#ifndef DEVICE_DATA_H
#define DEVICE_DATA_H
#include <string.h>  
#include <stdio.h>
#include <stdarg.h>
#include "usart.h"
#include "gpio.h"
#include "nrf24l01.h"


typedef struct device_data
{
    char* mark_data;
    char* state_data;
    int* gear_data;

} device_data;

extern device_data fan_state_data;
extern device_data fan_gear_data;
extern device_data light_state_data;
extern device_data light_gear_data;
extern device_data air_state_data;
extern device_data air_gear_data;
extern device_data lock_state_data;
extern device_data lock_gear_data;
extern device_data device_Back;


extern char fan_name[];
extern char fan_state[];
extern char fan_state_mark[];
extern int fan_gear;
extern char fan_gear_mark[];
extern char fan_mark[];

extern char light_name[];
extern char light_state[];
extern char light_state_mark[];
extern int light_gear;
extern char light_gear_mark[];
extern char light_mark[];

extern char air_name[];
extern char air_state[];
extern char air_state_mark[];
extern int air_gear;
extern char air_gear_mark[];
extern char air_mark[];

extern char lock_name[];
extern char lock_state[];
extern char lock_state_mark[];
extern int lock_gear;
extern char lock_gear_mark[];
extern char lock_mark[];

extern char Back_mark[];

void device_data_init(void);
void print(UART_HandleTypeDef *huart, const char *format, ...);
void writeStr(char *strArray, const char *str);
void mark_init(void);
void Master_init(void);


#endif

