#include "nrf24l01.h"
#include "spi.h"
#include <string.h>

// 定义发送和接收地址
uint8_t Unite_ADDRESS[5] = {0xA1,0xB2,0xC3,0xD4,0xE5}; 


/* 配置参数 */
#define TX_ADR_WIDTH   5   // 发送地址宽度
#define RX_ADR_WIDTH   5   // 接收地址宽度
#define TX_PLOAD_WIDTH 32  // 发送数据通道有效数据宽度
#define RX_PLOAD_WIDTH 32  // 接收数据通道有效数据宽度



/**
  * @brief  SPI读写一个字节
  * @param  TxData 要写入的字节
  * @retval 读取到的字节
  */
static uint8_t SPI_ReadWriteByte(uint8_t TxData)
{
    uint8_t RxData;
    HAL_StatusTypeDef status;
    status = HAL_SPI_TransmitReceive(&hspi2, &TxData, &RxData, 1, 100);
    return (status == HAL_OK) ? RxData : 0xFF;
}

/**
  * @brief  读取NRF24L01某个寄存器的值
  * @param  reg 寄存器地址
  * @retval 寄存器值
  */
static uint8_t NRF24L01_Read_Reg(uint8_t reg)
{
    uint8_t value;
    NRF_CSN_LOW();
    SPI_ReadWriteByte(reg);
    value = SPI_ReadWriteByte(NOP);
    NRF_CSN_HIGH();
    return value;
}

/**
  * @brief  写入NRF24L01某个寄存器的值
  * @param  reg 寄存器地址
  * @param  value 要写入的值
  * @retval 状态值
  */
static uint8_t NRF24L01_Write_Reg(uint8_t reg, uint8_t value)
{
    uint8_t status;
    NRF_CSN_LOW();
    status = SPI_ReadWriteByte(NRF_WRITE_REG | reg);
    SPI_ReadWriteByte(value);
    NRF_CSN_HIGH();
    return status;
}

/**
  * @brief  读取NRF24L01多个字节
  * @param  reg 寄存器地址
  * @param  pBuf 数据指针
  * @param  len 数据长度
  * @retval 状态值
  */
static uint8_t NRF24L01_Read_Buf(uint8_t reg, uint8_t *pBuf, uint8_t len)
{
    uint8_t status;
    NRF_CSN_LOW();
    status = SPI_ReadWriteByte(reg);
    for(uint8_t i = 0; i < len; i++)
    {
        pBuf[i] = SPI_ReadWriteByte(NOP);
    }
    NRF_CSN_HIGH();
    return status;
}

/**
  * @brief  写入NRF24L01多个字节
  * @param  reg 寄存器地址
  * @param  pBuf 数据指针
  * @param  len 数据长度
  * @retval 状态值
  */
static uint8_t NRF24L01_Write_Buf(uint8_t reg, uint8_t *pBuf, uint8_t len)
{
    uint8_t status;
    NRF_CSN_LOW();
    status = SPI_ReadWriteByte(reg);
    for(uint8_t i = 0; i < len; i++)
    {
        SPI_ReadWriteByte(pBuf[i]);
    }
    NRF_CSN_HIGH();
    return status;
}

/**
  * @brief  NRF24L01初始化
  * @param  无
  * @retval 无
  */
void NRF24L01_Init(void)
{
    NRF_CE_LOW();
    NRF_CSN_HIGH();
    // 设置 5 字节地址宽度
    NRF24L01_Write_Reg(NRF_WRITE_REG | SETUP_AW, 0x03);
    HAL_Delay(100);
}

/**
  * @brief  检测NRF24L01是否存在
  * @param  无
  * @retval 0:成功 1:失败
  */
uint8_t NRF24L01_Check(void)
{
    uint8_t buf[5] = {0xA5,0xA5,0xA5,0xA5,0xA5};
    uint8_t buf2[5];
    
    NRF24L01_Write_Buf(NRF_WRITE_REG + TX_ADDR, buf, 5);
    NRF24L01_Read_Buf(TX_ADDR, buf2, 5);
    
    for(uint8_t i = 0; i < 5; i++)
    {
        if(buf2[i] != 0xA5)
            return 1;
    }
    return 0;
}

/**
  * @brief  设置为发送模式
  * @param  addr 发送地址
  * @retval 无
  */
void NRF24L01_TX_Mode(uint8_t *addr)
{
    NRF_CE_LOW();
    
    // 添加电源控制
    NRF24L01_Write_Reg(NRF_WRITE_REG + NRF_CONFIG, 0x08); // 先下电
    HAL_Delay(1);
    
    // 写入发送地址
    NRF24L01_Write_Buf(NRF_WRITE_REG + TX_ADDR, addr, TX_ADR_WIDTH);
    NRF24L01_Write_Buf(NRF_WRITE_REG + RX_ADDR_P0, addr, RX_ADR_WIDTH);
    
    // 设置基本参数
    NRF24L01_Write_Reg(NRF_WRITE_REG + EN_AA, 0x01);      // 使能通道0自动应答
    NRF24L01_Write_Reg(NRF_WRITE_REG + EN_RXADDR, 0x01);  // 使能通道0接收
    NRF24L01_Write_Reg(NRF_WRITE_REG + SETUP_RETR, 0x1A); // 自动重发延时500us + 86us，重发10次
    NRF24L01_Write_Reg(NRF_WRITE_REG + RF_CH, 40);        // 设置射频通道为40
    NRF24L01_Write_Reg(NRF_WRITE_REG + RF_SETUP, 0x0F);   // 设置发射参数(0db,2Mbps)
    NRF24L01_Write_Reg(NRF_WRITE_REG + NRF_CONFIG, 0x0E);  // 配置基本工作模式(PWR_UP,EN_CRC,16BIT_CRC,发射模式)
    
    NRF_CE_HIGH();
    HAL_Delay(1);
    NRF24L01_Write_Reg(NRF_WRITE_REG + NRF_CONFIG, 0x0E); // 最后上电
    NRF_CE_HIGH();
    HAL_Delay(1);
}

/**
  * @brief  设置为接收模式
  * @param  addr 接收地址
  * @retval 无
  */
void NRF24L01_RX_Mode(uint8_t *addr)
{
    NRF_CE_LOW();
    
    // 写入接收地址
    NRF24L01_Write_Buf(NRF_WRITE_REG + RX_ADDR_P0, addr, RX_ADR_WIDTH);
    
    // 设置基本参数
    NRF24L01_Write_Reg(NRF_WRITE_REG + EN_AA, 0x01);      // 使能通道0自动应答
    NRF24L01_Write_Reg(NRF_WRITE_REG + EN_RXADDR, 0x01);  // 使能通道0接收
    NRF24L01_Write_Reg(NRF_WRITE_REG + RF_CH, 40);        // 设置射频通道为40
    NRF24L01_Write_Reg(NRF_WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH); // 设置通道0接收数据长度
    NRF24L01_Write_Reg(NRF_WRITE_REG + RF_SETUP, 0x0F);   // 设置发射参数(0db,2Mbps)
    NRF24L01_Write_Reg(NRF_WRITE_REG + NRF_CONFIG, 0x0F);  // 配置基本工作模式(PWR_UP,EN_CRC,16BIT_CRC,接收模式)
    
    NRF_CE_HIGH();
    HAL_Delay(1);
}

/**
  * @brief  发送一个数据包
  * @param  txbuf 待发送数据
  * @retval 0:发送完成 1:发送失败
  */
uint8_t NRF24L01_TxPacket(uint8_t *txbuf)
{
    uint8_t status;
    uint32_t timeout = 1000; // 1ms超时
    NRF_CE_LOW();
    
    NRF24L01_Write_Buf(TX_PAYLOAD, txbuf, TX_PLOAD_WIDTH);
    NRF_CE_HIGH();
    
    while(timeout--)
    {
        status = NRF24L01_Read_Reg(STATUS);
        if(status & 0x20)  // TX_DS
        {
            NRF24L01_Write_Reg(NRF_WRITE_REG + STATUS, 0x20); // 清除TX_DS标志
            return 0;
        }
        if(status & 0x10)  // MAX_RT
        {
            NRF24L01_Write_Reg(NRF_WRITE_REG + STATUS, 0x10); // 清除MAX_RT标志
            return 1;
        }
        HAL_Delay(1);
    }
    return 1; // 超时返回失败
}

/**
  * @brief  接收一个数据包
  * @param  rxbuf 接收数据缓存
  * @retval 0:接收完成 1:没有收到数据
  */
uint8_t NRF24L01_RxPacket(uint8_t *rxbuf)
{
    uint8_t status = NRF24L01_Read_Reg(STATUS);
    if (status & 0x40) {  // RX_DR 有数据到来
        // 读出数据
        NRF24L01_Read_Buf(RX_PAYLOAD, rxbuf, RX_PLOAD_WIDTH);

        // 清除 RX_DR 标志（只写 0x40）
        NRF24L01_Write_Reg(NRF_WRITE_REG | STATUS, 0x40);

        // 刷新 RX FIFO，防止残留
        NRF24L01_Write_Reg(FLUSH_RX, 0);

        return 0;
    }
    return 1;
}




