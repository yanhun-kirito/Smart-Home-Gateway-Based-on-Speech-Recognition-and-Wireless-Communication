#ifndef DT_H
#define DT_H

#include "stdio.h"
#include "string.h"
#include "stm32f1xx_hal.h"
#include "main.h"             
#include "nrf24l01.h"
#include "RX_DataHandle.h"
#include "stdint.h"



void Air_control(void);
void KEY_scan(void);
void DT(void);
void KEY_task(void);
void Data_init(int one_2,int ten_2);

extern int Air_gear;
extern char Air_state[10];
extern char Air_name[10];

extern uint8_t rxbuf[32];
extern char Data_source[10];
extern char Receive_name[10];
extern char Receive_state[10];
extern int Receive_gear;

typedef struct {
    char source[20];  // 源
    char device[20];  // 设备
    char state[20];   // 状态
    int gear;         // 档位
} ReceivedData;

int CompareStr(const char *arrayString, const char *targetString);
void writeStr(char *targetString, const char *sourceString);
void Air_Init(void);
void Air_handle(void);




#endif




