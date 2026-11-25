#ifndef KEY_TASK_H
#define KEY_TASK_H
#include "main.h"
#include "OLED.h"
#include "stm32f1xx_hal.h"
#include <stdio.h>
#include <string.h>
#include "Screen_task.h"
#include "device_data.h"
#include <stdlib.h> 
#include "nrf24l01.h"
#include "tim.h"

extern uint8_t rxbuf[32];
extern char txbuf[32];
extern uint16_t Time_Up;

void key_task(void);
int key_scan(void);
int CompareStr(const char *arrayString, const char *targetString);
void menu_to_device(void);
void device_to_control(void);
void Last_control(void);
void Next_control(void);
void Key_TX_Data(void);
void Key_TX_Data_Check(void);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
#endif


