#include "FAN.h"
#include "drv8833.h"
#include "main.h"
#include "stdio.h"
#include "string.h"

uint8_t rxbuf[32] = {0};
uint8_t pre_rxbuf[32] = {0};
char Data_source[10] = " ";
char Receive_name[10] = " ";
char Receive_state[10] = " ";
int Receive_gear = 0;

int Fan_gear = 0;
char Fan_state[10] = "OFF";
char Fan_name[10] = "Fan";

void KEY_scan(void)
{
	if(HAL_GPIO_ReadPin(switch_GPIO_Port, switch_Pin) == 0)
	{
		
		HAL_Delay(20);
		if(HAL_GPIO_ReadPin(switch_GPIO_Port, switch_Pin) == 0)
		{
			while(HAL_GPIO_ReadPin(switch_GPIO_Port, switch_Pin) == 0);
			if(strcmp(Fan_state,"OFF") == 0)
			{
				strcpy(Fan_state,"ON");
			}
			else if(strcmp(Fan_state,"ON") == 0)
			{
				strcpy(Fan_state,"OFF");
	        }
	    }
	}
	if(HAL_GPIO_ReadPin(switch_up_GPIO_Port, switch_up_Pin) == 0)
	{
		HAL_Delay(20);
		if(HAL_GPIO_ReadPin(switch_up_GPIO_Port, switch_up_Pin) == 0)
		{
			while(HAL_GPIO_ReadPin(switch_up_GPIO_Port, switch_up_Pin) == 0);
			if(strcmp(Fan_state,"ON") == 0)
			{
				Fan_gear = (Fan_gear < 4) ? Fan_gear+1 : 4 ;
	        }
		}	
	}
	if(HAL_GPIO_ReadPin(switch_down_GPIO_Port, switch_down_Pin) == 0)
	{
		HAL_Delay(20);
		if(HAL_GPIO_ReadPin(switch_down_GPIO_Port, switch_down_Pin) == 0)
		{
			while(HAL_GPIO_ReadPin(switch_down_GPIO_Port, switch_down_Pin) == 0);
			if(strcmp(Fan_state,"ON") == 0)
			{
				Fan_gear = (Fan_gear > 0) ? Fan_gear-1 : 0 ;
			}
		}	
	}
}

void FAN_task(void)
{
	FAN();
}
void FAN(void)
{
	int t = 0;
	int time[4] = {25,50,75,100};
	KEY_scan();
	if(strcmp(Fan_state,"ON") == 0)
	{
		t = time[Fan_gear];
		DRV8833_Forward(t);
	}
	else if(strcmp(Fan_state,"OFF") == 0)
	{
		t = 0;
		DRV8833_Forward(t);
	}
}

void Fan_Init(void)
{
    DRV8833_Init();
    // 检测模块是否正常
  if(NRF24L01_Check() == 0)
  { 
    // 设置为接收模式
    NRF24L01_RX_Mode(Unite_ADDRESS);
  }
  else
  {
    
  }

}


void FAN_handle(void)
{
    // 检测是否接收到数据
 if(NRF24L01_RxPacket(rxbuf) == 0)
 {
	if(CompareStr((const char*)rxbuf,pre_rxbuf) == 0)
    {
		writeStr(pre_rxbuf,(const char*)rxbuf);
		if(CompareStr((const char*)rxbuf,"OK") == 1)
        {
          NRF24L01_TX_Mode(Unite_ADDRESS);
          NRF24L01_TxPacket(rxbuf);
          HAL_Delay(1);
          NRF24L01_TxPacket(rxbuf);
          HAL_Delay(1);
          NRF24L01_TxPacket(rxbuf);
          HAL_Delay(1);
          NRF24L01_RX_Mode(Unite_ADDRESS);
        }
        else
        {
			ExtractResult ReceiveCheck = receive_data_handle( (const char*)rxbuf,Data_source,10,Receive_name,10,Receive_state,10,&Receive_gear);
			if(ReceiveCheck == EXTRACT_SUCCESS)
			{
				if(CompareStr(Receive_name,Fan_name) == 1)
				{
					writeStr(Fan_state,Receive_state);
					Fan_gear = Receive_gear;
					FAN();					
					NRF24L01_TX_Mode(Unite_ADDRESS);
					NRF24L01_TxPacket("OK");
					HAL_Delay(1);
					NRF24L01_TxPacket("OK");
					HAL_Delay(1);
					NRF24L01_TxPacket("OK");
					HAL_Delay(1);
					NRF24L01_RX_Mode(Unite_ADDRESS);
				}
				else
				{
					NRF24L01_TX_Mode(Unite_ADDRESS);
              		NRF24L01_TxPacket(rxbuf);
					HAL_Delay(1);
					NRF24L01_TxPacket(rxbuf);
					HAL_Delay(1);
					NRF24L01_TxPacket(rxbuf);
					HAL_Delay(1);
					NRF24L01_RX_Mode(Unite_ADDRESS);
				}				
			}
			writeStr((char*)rxbuf," ");   
			writeStr(Receive_name," ");
			writeStr(Receive_state," ");
			Receive_gear = 0;
		}
 	}
 }
 
}

/**
 * @brief  字符串比较函数
 * @param  arrayString: 待比较的字符串数组
 * @param  targetString: 目标比较字符串
 * @retval 如果两个字符串相等返回1，否则返回0
 * @note   使用标准库strcmp函数进行比较
 *         主要用于比较接收到的NRF24L01指令
 */
int CompareStr(const char *arrayString, const char *targetString) 
{
    return strcmp(arrayString, targetString) == 0;
}



void writeStr(char *strArray, const char *str)
{
    // 清空字符串数组
    memset(strArray, 0, strlen(strArray));  // 将数组内容清空，可以也直接通过指针赋值
    // 将新字符串赋值到数组
    strcpy(strArray, str);  // 使用 strcpy 将字符串复制到数组
}







