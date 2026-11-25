#include "LED.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

uint8_t rxbuf[32] = {0};
uint8_t pre_rxbuf[32] = {0};
char Data_source[10] = " ";
char Receive_name[10] = " ";
char Receive_state[10] = " ";
int Receive_gear = 0;

char light_name[10] = "Light";
char light_state[10]="OFF";
int light_gear = 0;


int KEY_scan(void)
{
  // 修改1：增加按键释放检测
  if(HAL_GPIO_ReadPin(switch_GPIO_Port, switch_Pin) == GPIO_PIN_RESET)
    {
     HAL_Delay(50);
     if(HAL_GPIO_ReadPin(switch_GPIO_Port, switch_Pin) == GPIO_PIN_RESET)
        {
         return 1;
        }
    }
  if(HAL_GPIO_ReadPin(switch_up_GPIO_Port,switch_up_Pin) == GPIO_PIN_RESET)
   {
    HAL_Delay(50);
    if(HAL_GPIO_ReadPin(switch_up_GPIO_Port,switch_up_Pin) == GPIO_PIN_RESET)
       {
        return 2;
       }
   }
  if(HAL_GPIO_ReadPin(switch_down_GPIO_Port,switch_down_Pin) == GPIO_PIN_RESET)
   {
    HAL_Delay(50);
    if(HAL_GPIO_ReadPin(switch_down_GPIO_Port,switch_down_Pin) == GPIO_PIN_RESET)
       {   
        return 3;
       }
   }
 return 0;
}

void KEY_task(void)
{
  static int state = 0;
  int time[5] = {20,40,60,80,99};
  int key = KEY_scan();
    
    // 修改2：调整判断顺序，使用if-else链
  if (key == 1)
    {
      if(CompareStr(light_state,"ON") == 1)
        {
          writeStr(light_state,"OFF");
          light_gear = 0;
          __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
        } else {
            writeStr(light_state,"ON");
            light_gear = 1;
            __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, time[0]);
        }
    }
    // 修改3：增加state有效性检查
  else if (state == 1)  
    {
      if (key == 2) 
        {
          if(light_gear == 5)
          {
            light_gear = 5;
            __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, time[4]);
          }
          else
          {
            light_gear++;
            __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, time[light_gear - 1]);
          }
        }
      else if (key == 3) 
        {
          if(light_gear == 1)
          {
            light_gear = 1;
            __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, time[0]);
          }
          else
          {
            light_gear--;
            __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, time[light_gear - 1]);
          }
        }
    }
}

void LED_Init(void)
{
    HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
    __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,0);  // 初始化PWM通道1为0
    Light_control();
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


void LED_handle(void)
{
  // 检测是否接收到数据
 if(NRF24L01_RxPacket(rxbuf) == 0)
 {
    if(CompareStr((const char*)rxbuf,pre_rxbuf) == 0)
    {
        writeStr(pre_rxbuf,(const char*)rxbuf);
        HAL_UART_Transmit(&huart2,rxbuf,32,100);
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
            if(CompareStr(Receive_name,"Light") == 1)
            {
              writeStr(light_state,Receive_state);
              light_gear = Receive_gear;
              Light_control();
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
          writeStr(Receive_name," ");
          writeStr(Receive_state," ");
          Receive_gear = 0;
          writeStr((char*)rxbuf," ");   
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

void Light_control(void)
{
    if(CompareStr(light_state,"ON") == 1)
    {
        if(light_gear == 1)
        {
            __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,20);
        }
        else if(light_gear == 2)
        {
            __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,40);
        }
        else if(light_gear == 3)
        {
            __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,60);
        }
        else if(light_gear == 4)
        {
            __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,80);
        }
        else if(light_gear == 5)
        {
            __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,99);
        }
    }
    else if(CompareStr(light_state,"OFF") == 1)
    {
        __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,0);
    }
}





