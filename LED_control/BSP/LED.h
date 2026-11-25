#ifndef LED_H
#define LED_H  // 添加头文件保护宏

#include "stdio.h"
#include "string.h"
#include "stm32f1xx_hal.h"
#include "main.h"               
#include "tim.h"
#include "nrf24l01.h"
#include "RX_DataHandle.h"
#include "usart.h"
#include "stdint.h"

extern uint8_t rxbuf[32];
extern char Data_source[10];
extern char Receive_name[10];
extern char Receive_state[10];
extern int Receive_gear;

extern char light_name[10];
extern char light_state[10];
extern int light_gear;

typedef struct {
    char source[20];  // 源
    char device[20];  // 设备
    char state[20];   // 状态
    int gear;         // 档位
} ReceivedData;


void KEY_task(void);
int KEY_scan(void);
void LED_Init(void);
void LED_handle(void);
int CompareStr(const char *arrayString, const char *targetString);
void writeStr(char *targetString, const char *sourceString);
void Light_control(void);

// 保持此处空白行以确保文件结尾换行
#endif


