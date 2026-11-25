#ifndef __RX_HANDLE_H__
#define __RX_HANDLE_H__
#include "RX_DataHandle.h"
#include "main.h"
#include "usart.h"
#include "stdint.h"
#include "nrf24l01.h"
#include "Key_Task.h"
#include "Screen_task.h"

extern DMA_HandleTypeDef hdma_usart2_rx;  

extern uint8_t rx_data[32];  
extern char Data_source[10];
extern char Receive_name[10];
extern char Receive_state[10];
extern int Receive_gear;

void RX_Init(void);
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart,uint16_t Size);










#endif


