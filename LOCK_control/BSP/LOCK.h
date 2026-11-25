#ifndef LOCK_H
#define LOCK_H

#include"stdio.h"
#include"stdlib.h"
#include "stdint.h"
#include"string.h"
#include "stm32f1xx_hal.h"
#include "main.h"               
#include "tim.h"
#include "nrf24l01.h"
#include "RX_DataHandle.h"

extern uint8_t rxbuf[32];
extern char Data_source[10];
extern char Receive_name[10];
extern char Receive_state[10];
extern int Receive_gear;
extern char LOCK_state[10];
extern char LOCK_name[10];
extern int LOCK_gear;

typedef struct {
    char source[20];  // 源
    char device[20];  // 设备
    char state[20];   // 状态
    int gear;         // 档位
} ReceivedData;

void KEY_task(void);
void KEY_scan(void);
void AAA(void);
void LOCK_Init(void);
void LOCK_handle(void);
int CompareStr(const char *arrayString, const char *targetString);
void writeStr(char *strArray, const char *str);






#endif




