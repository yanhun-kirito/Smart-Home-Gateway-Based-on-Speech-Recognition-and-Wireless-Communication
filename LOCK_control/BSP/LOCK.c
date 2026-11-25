#include "LOCK.h"


uint8_t rxbuf[32] = {0};
uint8_t pre_rxbuf[32] = {0};
char Data_source[10] = " ";
char Receive_name[10] = " ";
char Receive_state[10] = " ";
int Receive_gear = 0;

char LOCK_state[10] = "OFF";
char LOCK_name[10] = "Lock";
int LOCK_gear = 0;

void KEY_scan(void)
{
	if(HAL_GPIO_ReadPin(switch_ccw_GPIO_Port, switch_ccw_Pin) == 0)
	{
		HAL_Delay(20);
		if(HAL_GPIO_ReadPin(switch_ccw_GPIO_Port, switch_ccw_Pin) == 0)
		{
      if(strcmp(LOCK_state,"OFF") == 0)
      {
        strcpy(LOCK_state,"ON");
        LOCK_gear = 1;
      }
      else if(strcmp(LOCK_state,"ON") == 0)
      {
        strcpy(LOCK_state,"OFF");
        LOCK_gear = 0;
      }
		}	
	}	
}
void KEY_task(void)
{
 AAA();
}

void AAA(void)
{
  int rot = 0;
 static int stot = 150;
 KEY_scan();
 if(strcmp(LOCK_state,"OFF") == 0)  //逆时针转动
 {
  rot = 180;
  __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1, rot);
  HAL_Delay(2100);
 }
 else if(strcmp(LOCK_state,"ON") == 0) //顺时针转动
 {
  rot = 120;
  __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1, rot);
  HAL_Delay(2100);
 } 
 __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1, stot);
}


void LOCK_Init(void)
{
  HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
  AAA();
  // 检测模块是否正常
  if(NRF24L01_Check() == 0)
  { 
    // 设置为接收模式
    NRF24L01_RX_Mode(Unite_ADDRESS);
  }

}

void LOCK_handle(void)
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
            if(CompareStr(Receive_name,LOCK_name) == 1)
            {
              writeStr(LOCK_state,Receive_state);
              LOCK_gear = Receive_gear;
              AAA();
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



