#ifndef FAN_H__
#define FAN_H__

#include "stdio.h"
#include "stm32f1xx_hal.h"
#include "tim.h"
#include "main.h"
#include "drv8833.h"  
#include "nrf24l01.h"
#include "RX_DataHandle.h"

extern uint8_t rxbuf[32];
extern char Data_source[10];
extern char Receive_name[10];
extern char Receive_state[10];
extern int Receive_gear;

extern int Fan_gear;   // 澹版槑澶栭儴鍙橀噺
extern char Fan_state[10];   // 澹版槑澶栭儴鍙橀噺
extern char Fan_name[10];   // 澹版槑澶栭儴鍙橀噺

typedef struct {
    char source[20];  // 源
    char device[20];  // 设备
    char state[20];   // 状态
    int gear;         // 档位
} ReceivedData;

// 添加函数声明（在文件末尾添加）
void FAN_task(void);
void FAN(void);
int CompareStr(const char *arrayString, const char *targetString);
void writeStr(char *strArray, const char *str);
void FAN_handle(void);
void Fan_Init(void);




#endif


