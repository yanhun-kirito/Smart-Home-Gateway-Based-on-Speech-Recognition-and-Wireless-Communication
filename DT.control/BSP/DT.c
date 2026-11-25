#include "DT.h"
#include "main.h"
#include "string.h"

int Air_gear=16;
char Air_state[10] = "OFF";
char Air_name[10] = "Air";

uint8_t rxbuf[32] = {0};
uint8_t pre_rxbuf[32] = {0};
char Data_source[10] = " ";
char Receive_name[10] = " ";
char Receive_state[10] = " ";
int Receive_gear = 0;


void KEY_scan(void)
{
  // 增加按键释放检测
  if(HAL_GPIO_ReadPin(switch_GPIO_Port, switch_Pin) == 0)
    {
     HAL_Delay(20);
     if(HAL_GPIO_ReadPin(switch_GPIO_Port, switch_Pin) == 0)
        {
         while(HAL_GPIO_ReadPin(switch_GPIO_Port, switch_Pin) == 0); // 等待松开
         if (strcmp(Air_state,"OFF") == 0)
         {
            strcpy(Air_state,"ON");
         }
         else if (strcmp(Air_state,"ON") == 0)
         {
            strcpy(Air_state,"OFF");
         }
        }    
    }
  if(HAL_GPIO_ReadPin(switch_up_GPIO_Port,switch_up_Pin) == 0)
   {
    HAL_Delay(20);
    if(HAL_GPIO_ReadPin(switch_up_GPIO_Port,switch_up_Pin) == 0)
       {
        while(HAL_GPIO_ReadPin(switch_up_GPIO_Port,switch_up_Pin) == 0);
        if(strcmp(Air_state,"ON") == 0)
        {Air_gear = (Air_gear < 32)? (Air_gear+1) : (32);}
       } 
   }
  if(HAL_GPIO_ReadPin(switch_down_GPIO_Port,switch_down_Pin) == 0)
   {
    HAL_Delay(20);
    if(HAL_GPIO_ReadPin(switch_down_GPIO_Port,switch_down_Pin) == 0)
       {
        while (HAL_GPIO_ReadPin(switch_down_GPIO_Port,switch_down_Pin) == 0);
        if(strcmp(Air_state,"ON") == 0)
        {Air_gear = (Air_gear > 16)? (Air_gear-1) : (16);}
       } 
   }
}

void KEY_task(void)
{
  DT();
}

void DT(void)
{
  KEY_scan();
  if (strcmp(Air_state,"OFF") == 0)
  {
    Air_control();
  }
  else if (strcmp(Air_state,"ON") == 0)
  {
    Air_control();
  }
}

void Data_init(int one_2,int ten_2)
{
   LED[0] = 17;
   LED[1] = 17;
   LED[2] = one_2;
   LED[3] = ten_2;
   LED4_Display();
}

void Air_control(void)
{
    int one = Air_gear % 10;
    int ten = Air_gear / 10;
    if(strcmp(Air_state,"ON") == 0)
    {
         switch (Air_gear)
         { 
         case 16:Data_init(one,ten);break;
         case 17:Data_init(one,ten);break;
         case 18:Data_init(one,ten);break;
         case 19:Data_init(one,ten);break;
         case 20:Data_init(one,ten);break;
         case 21:Data_init(one,ten);break;
         case 22:Data_init(one,ten);break;
         case 23:Data_init(one,ten);break;
         case 24:Data_init(one,ten);break;
         case 25:Data_init(one,ten);break;
         case 26:Data_init(one,ten);break;
         case 27:Data_init(one,ten);break;
         case 28:Data_init(one,ten);break;
         case 29:Data_init(one,ten);break;
         case 30:Data_init(one,ten);break;
         case 31:Data_init(one,ten);break;
         case 32:Data_init(one,ten);break;
        default:   break;
         } 
    }
    else if (strcmp(Air_state,"OFF") == 0)
    {
        for (int i = 0; i < 4; i++)
        {
            LED[i] = 17;
        }
        LED4_Display();
    }
  }

void Air_Init(void)
{
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

void Air_handle(void)
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
            if(CompareStr(Receive_name,"Air") == 1)
            {
              writeStr(Air_state,Receive_state);
              Air_gear = Receive_gear;
              Air_control();
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

