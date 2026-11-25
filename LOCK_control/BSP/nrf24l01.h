#ifndef __NRF24L01_H
#define __NRF24L01_H

#include "stm32f1xx_hal.h"

/* nRF24L01 寄存器地址定义 */
#define NRF_CONFIG      0x00  // 配置寄存器
#define EN_AA          0x01  // 使能自动应答
#define EN_RXADDR      0x02  // 接收地址允许
#define SETUP_AW       0x03  // 地址宽度设置
#define SETUP_RETR     0x04  // 自动重发设置
#define RF_CH          0x05  // 射频通道
#define RF_SETUP       0x06  // 射频设置
#define STATUS         0x07  // 状态
#define OBSERVE_TX     0x08  // 发送监测
#define CD             0x09  // 载波检测
#define RX_ADDR_P0     0x0A  // 数据通道0接收地址
#define RX_ADDR_P1     0x0B  // 数据通道1接收地址
#define RX_ADDR_P2     0x0C  // 数据通道2接收地址
#define RX_ADDR_P3     0x0D  // 数据通道3接收地址
#define RX_ADDR_P4     0x0E  // 数据通道4接收地址
#define RX_ADDR_P5     0x0F  // 数据通道5接收地址
#define TX_ADDR        0x10  // 发送地址
#define RX_PW_P0       0x11  // 接收数据通道0有效数据宽度
#define RX_PW_P1       0x12  // 接收数据通道1有效数据宽度
#define RX_PW_P2       0x13  // 接收数据通道2有效数据宽度
#define RX_PW_P3       0x14  // 接收数据通道3有效数据宽度
#define RX_PW_P4       0x15  // 接收数据通道4有效数据宽度
#define RX_PW_P5       0x16  // 接收数据通道5有效数据宽度
#define FIFO_STATUS    0x17  // FIFO状态寄存器

/* 操作命令 */
#define NRF_READ_REG    0x00  // 读寄存器
#define NRF_WRITE_REG   0x20  // 写寄存器
#define RX_PAYLOAD     0x61  // 读取接收数据
#define TX_PAYLOAD     0xA0  // 写入发送数据
#define FLUSH_TX       0xE1  // 清空发送FIFO
#define FLUSH_RX       0xE2  // 清空接收FIFO
#define REUSE_TX_PL    0xE3  // 重用上一包数据
#define NOP            0xFF  // 空操作,可以用来读状态寄存器

/* 引脚定义 */
#define NRF_CSN_PIN    GPIO_PIN_8
#define NRF_CSN_PORT   GPIOA
#define NRF_CE_PIN     GPIO_PIN_9
#define NRF_CE_PORT    GPIOA

/* 引脚操作宏定义 */
#define NRF_CSN_LOW()    HAL_GPIO_WritePin(NRF_CSN_PORT, NRF_CSN_PIN, GPIO_PIN_RESET)
#define NRF_CSN_HIGH()   HAL_GPIO_WritePin(NRF_CSN_PORT, NRF_CSN_PIN, GPIO_PIN_SET)
#define NRF_CE_LOW()     HAL_GPIO_WritePin(NRF_CE_PORT, NRF_CE_PIN, GPIO_PIN_RESET)
#define NRF_CE_HIGH()    HAL_GPIO_WritePin(NRF_CE_PORT, NRF_CE_PIN, GPIO_PIN_SET)

extern uint8_t Unite_ADDRESS[5];



/* 函数声明 */
void NRF24L01_Init(void);
uint8_t NRF24L01_Check(void);
void NRF24L01_TX_Mode(uint8_t *addr);
void NRF24L01_RX_Mode(uint8_t *addr);
uint8_t NRF24L01_TxPacket(uint8_t *txbuf);
uint8_t NRF24L01_RxPacket(uint8_t *rxbuf);



#endif 


