#include "RX_Handle.h"


uint8_t rx_data[32];  
char Data_source[10] = " ";
char Receive_name[10] = " ";
char Receive_state[10] = " ";
int Receive_gear = 0;



void RX_Init(void)
{
    HAL_UARTEx_ReceiveToIdle_DMA(&huart2,rx_data,32);
    __HAL_DMA_DISABLE_IT(&hdma_usart2_rx,DMA_IT_HT);
}


void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart,uint16_t Size)
{
    if(huart == &huart2)
    {   
       ExtractResult RecieveCheck = receive_data_handle((char*)rx_data,Data_source,10,Receive_name,10,Receive_state,10,&Receive_gear);
        if(RecieveCheck == EXTRACT_SUCCESS)
        {
            NRF24L01_TX_Mode(Unite_ADDRESS);
            sprintf(txbuf,"MAS,%s,%s,%d",Receive_name,Receive_state,Receive_gear);
            Key_TX_Data_Check();
            if(CompareStr(Receive_name,"Fan"))
            {
                writeStr(fan_state,Receive_state);
                fan_gear = Receive_gear;
            }
            else if(CompareStr(Receive_name,"Light"))
            {
                writeStr(light_state,Receive_state);
                light_gear = Receive_gear;
            }
            else if(CompareStr(Receive_name,"Air"))
            {
                writeStr(air_state,Receive_state);
                air_gear = Receive_gear; 
            }
            else if(CompareStr(Receive_name,"Lock"))
            {
                writeStr(lock_state,Receive_state);
                lock_gear = Receive_gear; 
            }
            if (CompareStr(page_mark,"device"))
            {
                OLED_ShowString(3, 14, " ");
                show_device();
            }
            else if(CompareStr(page_mark,"menu"))
            {
                show_menu(&page1);
            }
            
        }
        HAL_UART_Transmit(&huart2,rx_data,32,100);
        writeStr((char*)rx_data," ");
        writeStr((char*)txbuf," ");
        writeStr((char*)Data_source," ");
        writeStr((char*)Receive_name," ");
        writeStr((char*)Receive_state," ");
        Receive_gear = 0;
        HAL_UARTEx_ReceiveToIdle_DMA(&huart2,rx_data,32);
        __HAL_DMA_DISABLE_IT(&hdma_usart2_rx,DMA_IT_HT);
    }

}




