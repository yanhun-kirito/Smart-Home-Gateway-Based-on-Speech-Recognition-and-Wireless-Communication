#include "Key_Task.h"

char txbuf[32]="";
uint8_t rxbuf[32] = {0};

int key_scan(void)
{
    if(HAL_GPIO_ReadPin(Button_Last_GPIO_Port,Button_Last_Pin)==GPIO_PIN_RESET)
    {
        HAL_Delay(90);
        if(HAL_GPIO_ReadPin(Button_Last_GPIO_Port,Button_Last_Pin)==GPIO_PIN_RESET)
        {
            return 1;
        }
    }
    else if(HAL_GPIO_ReadPin(Button_OK_GPIO_Port,Button_OK_Pin)==GPIO_PIN_RESET)
    {
        HAL_Delay(90);
        if(HAL_GPIO_ReadPin(Button_OK_GPIO_Port,Button_OK_Pin)==GPIO_PIN_RESET)
        {
            return 2;
        }
    }
    else if(HAL_GPIO_ReadPin(Button_Next_GPIO_Port,Button_Next_Pin)==GPIO_PIN_RESET)
    {
        HAL_Delay(90);
        if(HAL_GPIO_ReadPin(Button_Next_GPIO_Port,Button_Next_Pin)==GPIO_PIN_RESET)
        {
            return 3;
        }
    }
    return 0;
}

void key_task(void)
{
    int key=key_scan();
    if(key==1)
    {
        if(CompareStr(page_mark,"menu"))
        {
            menu_PrevMark = menu_PrevMark->prev;
            menu_NowMark = menu_NowMark->prev;
            menu_NextMark = menu_NextMark->prev;
            menu_mark_Handle();
            show_menu(&page1);
        }
        else if(CompareStr(page_mark,"device"))
        {
            device_PrevMark = device_PrevMark->prev;
            device_NowMark = device_NowMark->prev;
            device_NextMark = device_NextMark->prev;
            device_mark_Handle();
            show_device();
             if (device_NowMark->dev_data == &device_Back)
                OLED_ShowInvertedString(1, 3, "<");
        }
        else if (CompareStr(page_mark,"control"))
        {
            Last_control();
        }
        
    }
    else if(key==2)
    {   
        
        if (CompareStr(page_mark,"menu"))
        {
            menu_to_device();
        }
        else if(CompareStr(page_mark,"device"))
        {
            device_to_control();
        }
        else if (CompareStr(page_mark,"control"))
        {
            device_mark_Handle();
            // 发送并控制设置的设备的状态和档位
            Key_TX_Data();

            writeStr(page_mark, "device");
            OLED_ShowString(3, 14, " ");
            show_device();
        }
        
    }
    else if(key==3)
    {
        if(CompareStr(page_mark,"menu"))
        {
            menu_PrevMark = menu_PrevMark->next;
            menu_NowMark = menu_NowMark->next;
            menu_NextMark = menu_NextMark->next;
            menu_mark_Handle();
            show_menu(&page1);
        }
        else if(CompareStr(page_mark,"device"))
        {
            device_PrevMark = device_PrevMark->next;
            device_NowMark = device_NowMark->next;
            device_NextMark = device_NowMark->next;
            device_mark_Handle();
            show_device();
            if (device_NowMark->dev_data == &device_Back)
                OLED_ShowInvertedString(1,3, "<");
        }
        else if (CompareStr(page_mark,"control"))
        {
            Next_control();
        }
    }
}



int CompareStr(const char *arrayString, const char *targetString) 
{
    return strcmp(arrayString, targetString) == 0;
}


void menu_to_device(void)
{
    if (menu_NowMark->dev == &fan)
            {
                device_NowMark = &fan_state_list_mark;
                device_PrevMark = device_NowMark->prev;
                device_NextMark = device_NowMark->next;
                device_Back_mark.prev = &fan_gear_list_mark;
                device_Back_mark.next = &fan_state_list_mark;
                device_mark_Handle();
                writeStr(page_mark, "device");
                OLED_Clear();
                show_device();
            }
            else if (menu_NowMark->dev == &light)
            {
                device_NowMark = &light_state_list_mark;
                device_PrevMark = device_NowMark->prev;
                device_NextMark = device_NowMark->next;
                device_Back_mark.prev = &light_gear_list_mark;
                device_Back_mark.next = &light_state_list_mark;
                device_mark_Handle();
                writeStr(page_mark, "device");
                OLED_Clear();
                show_device();
            }
            else if (menu_NowMark->dev == &air)
            {
                device_NowMark = &air_state_list_mark;
                device_PrevMark = device_NowMark->prev;
                device_NextMark = device_NowMark->next;
                device_Back_mark.prev = &air_gear_list_mark;
                device_Back_mark.next = &air_state_list_mark;
                device_mark_Handle();
                writeStr(page_mark, "device");
                OLED_Clear();
                show_device();
            }
            else if (menu_NowMark->dev == &lock)
            {
                device_NowMark = &lock_state_list_mark;
                device_PrevMark = device_NowMark->prev;
                device_NextMark = device_NowMark->next;
                device_Back_mark.prev = &lock_gear_list_mark;
                device_Back_mark.next = &lock_state_list_mark;
                device_mark_Handle();
                writeStr(page_mark, "device");
                OLED_Clear();
                show_device();
            }
}

void device_to_control(void)
{
    if (device_NowMark->dev_data == &device_Back)
            {
                menu_init();
            }
            else 
            {
                writeStr(page_mark,"control");
                // writeStr(device_NowMark->dev_data->mark_data,"[ ]");
                show_device();
                if(device_NowMark->dev_data->state_data!=NULL)
                {
                    OLED_ShowInvertedString(3, 12,device_NowMark->dev_data->state_data);
                }
                else OLED_ShowInvertedNum(4, 12, (uint32_t)*(device_NowMark->dev_data->gear_data), 2);
            }
}

void Last_control(void)
{
    if(menu_NowMark->dev==&fan)
    {
        if(device_NowMark->dev_data==&fan_state_data)
        {
            if(CompareStr(fan_state,"OFF"))
            {
                writeStr(fan_state,"ON");
                OLED_ShowString(3, 14, " ");
                OLED_ShowInvertedString(3, 12, fan_state);
            }
            else if(CompareStr(fan_state,"ON"))
            {
                writeStr(fan_state,"OFF");
                OLED_ShowInvertedString(3, 12, fan_state);
            }
            if(CompareStr(fan_state,"OFF")) fan_gear=0;
            else fan_gear=1;
        }
        else if(device_NowMark->dev_data==&fan_gear_data)
        {
            if(fan_gear!=0)
            {
                fan_gear=fan_gear-1;
                OLED_ShowInvertedNum(4, 12, (uint32_t)(fan_gear), 2);
            }
            else 
            {
                fan_gear=4;
                OLED_ShowInvertedNum(4, 12, (uint32_t)(fan_gear), 2);
            }
            if (fan_gear != 0) writeStr(fan_state,"ON");
            else writeStr(fan_state,"OFF");
            
        }
    }
    else if(menu_NowMark->dev==&light)
    {
        if(device_NowMark->dev_data==&light_state_data)
        {
            if(CompareStr(light_state,"OFF"))
            {
                writeStr(light_state,"ON");
                OLED_ShowString(3, 14, " ");
                OLED_ShowInvertedString(3, 12, light_state);
            }
            else if(CompareStr(light_state,"ON"))
            {
                writeStr(light_state,"OFF");
                OLED_ShowInvertedString(3, 12, light_state);
            }
            if(CompareStr(light_state,"OFF")) light_gear=0;
            else light_gear=1;
        }
        else if(device_NowMark->dev_data==&light_gear_data)
        {
            if(light_gear!=0)
            {
                light_gear=light_gear-1;
                OLED_ShowInvertedNum(4, 12, (uint32_t)(light_gear), 2);
            }
            else
            {
                light_gear=5;
                OLED_ShowInvertedNum(4, 12, (uint32_t)(light_gear), 2);
            }
            if (light_gear!= 0) writeStr(light_state,"ON");
            else writeStr(light_state,"OFF");
            
        }
    }
    else if(menu_NowMark->dev==&air)
    {
        if(device_NowMark->dev_data==&air_state_data)
        {
            if(CompareStr(air_state,"OFF"))
            {
                writeStr(air_state,"ON");
                OLED_ShowString(3, 14, " ");
                OLED_ShowInvertedString(3, 12, air_state);
            }
            else if(CompareStr(air_state,"ON"))
            {
                writeStr(air_state,"OFF");
                OLED_ShowInvertedString(3, 12, air_state);
            }
        }
        else if(device_NowMark->dev_data==&air_gear_data)
        {
            if(air_gear!=16)
            {
                air_gear=air_gear-1;
                OLED_ShowInvertedNum(4, 12, (uint32_t)(air_gear), 2);
            }
        }
    }
    else if(menu_NowMark->dev==&lock)
    {
        if(device_NowMark->dev_data==&lock_state_data)
        {
            if(CompareStr(lock_state,"OFF"))
            {
                writeStr(lock_state,"ON");
                OLED_ShowString(3, 14, " ");
                OLED_ShowInvertedString(3, 12, lock_state);
            }
            else if(CompareStr(lock_state,"ON"))
            {
                writeStr(lock_state,"OFF");
                OLED_ShowInvertedString(3, 12, lock_state);
            }
            if(CompareStr(lock_state,"OFF")) lock_gear=0;
            else lock_gear=1;
        }
        else if(device_NowMark->dev_data==&lock_gear_data)
        {
            if(lock_gear!=0)
            {
               lock_gear=lock_gear-1;
                OLED_ShowInvertedNum(4, 12, (uint32_t)(lock_gear), 2);
            }
            else
            {
                lock_gear=1;
                OLED_ShowInvertedNum(4, 12, (uint32_t)(lock_gear), 2);
            }
            if (lock_gear!= 0) writeStr(lock_state,"ON");
            else writeStr(lock_state,"OFF");
        }
    }
}

void Next_control(void)
{
    if(menu_NowMark->dev==&fan)
    {
        if(device_NowMark->dev_data==&fan_state_data)
        {
            if(CompareStr(fan_state,"OFF"))
            {
                writeStr(fan_state,"ON");
                OLED_ShowString(3, 14, " ");
                OLED_ShowInvertedString(3, 12, fan_state);
            }
            else if(CompareStr(fan_state,"ON"))
            {
                writeStr(fan_state,"OFF");
                OLED_ShowInvertedString(3, 12, fan_state);
            }
            if(CompareStr(fan_state,"OFF")) fan_gear=0;
            else fan_gear=1;
        }
        else if(device_NowMark->dev_data==&fan_gear_data)
        {
            if(fan_gear!=4)
            {
                fan_gear=fan_gear+1;
                OLED_ShowInvertedNum(4, 12, (uint32_t)(fan_gear), 2);
            }
            else
            {
                fan_gear=0;
                OLED_ShowInvertedNum(4, 12, (uint32_t)(fan_gear), 2);
            }
            if (fan_gear!= 0) writeStr(fan_state,"ON");
            else writeStr(fan_state,"OFF");
        }
    }
    else if(menu_NowMark->dev==&light)
    {
        if(device_NowMark->dev_data==&light_state_data)
        {
            if(CompareStr(light_state,"OFF"))
            {
                writeStr(light_state,"ON");
                OLED_ShowString(3, 14, " ");
                OLED_ShowInvertedString(3, 12, light_state);
            }
            else if(CompareStr(light_state,"ON"))
            {
                writeStr(light_state,"OFF");
                OLED_ShowInvertedString(3, 12, light_state);
            }
            if(CompareStr(light_state,"OFF")) light_gear=0;
            else light_gear=1;
        }
        else if(device_NowMark->dev_data==&light_gear_data)
        {
            if(light_gear!=5)
            {
                light_gear=light_gear+1;
                OLED_ShowInvertedNum(4, 12, (uint32_t)(light_gear), 2);
            }
            else
            {
                light_gear=0;
                OLED_ShowInvertedNum(4, 12, (uint32_t)(light_gear), 2);
            }
            if (light_gear!= 0) writeStr(light_state,"ON");
            else writeStr(light_state,"OFF");
        }
    }
    else if(menu_NowMark->dev==&air)
    {   
        if(device_NowMark->dev_data==&air_state_data)
        {
            if(CompareStr(air_state,"OFF"))
            {
                writeStr(air_state,"ON");
                OLED_ShowString(3, 14, " ");
                OLED_ShowInvertedString(3, 12, air_state);
            }
            else if(CompareStr(air_state,"ON"))
            {
                writeStr(air_state,"OFF");
                OLED_ShowInvertedString(3, 12, air_state);
            }
        }
        else if(device_NowMark->dev_data==&air_gear_data)
        {
            if(air_gear!=32)
            {
                air_gear=air_gear+1;
                OLED_ShowInvertedNum(4, 12, (uint32_t)(air_gear), 2);
            }
        }
    }
    else if(menu_NowMark->dev==&lock)
    {   
        
        if(device_NowMark->dev_data==&lock_state_data)
        {
            if(CompareStr(lock_state,"OFF"))
            {
                writeStr(lock_state,"ON");
                OLED_ShowString(3, 14, " ");
                OLED_ShowInvertedString(3, 12, lock_state);
            }
            else if(CompareStr(lock_state,"ON"))
            {
                writeStr(lock_state,"OFF");
                OLED_ShowInvertedString(3, 12, lock_state);
            }
            if(CompareStr(lock_state,"OFF")) lock_gear=0;
            else lock_gear=1;
        }
        else if(device_NowMark->dev_data==&lock_gear_data)
        {
            if(lock_gear!=1)
            {
                lock_gear=lock_gear+1;
                OLED_ShowInvertedNum(4, 12, (uint32_t)(lock_gear), 2);
            }
            else
            {
                lock_gear=0;
                OLED_ShowInvertedNum(4, 12, (uint32_t)(lock_gear), 2);
            }
            if (lock_gear!= 0) writeStr(lock_state,"ON");
            else writeStr(lock_state,"OFF");
        }
    }
}


void Key_TX_Data(void)
{
    
    NRF24L01_TX_Mode(Unite_ADDRESS);
    if(menu_NowMark->dev==&fan)
    {   
        sprintf(txbuf,"MAS,%s,%s,%d",fan_name,fan_state,fan_gear);
        Key_TX_Data_Check();
    }
    else if(menu_NowMark->dev==&light)
    {
        sprintf(txbuf,"MAS,%s,%s,%d",light_name,light_state,light_gear);
        Key_TX_Data_Check();
    }
    else if(menu_NowMark->dev==&air)
    {
        sprintf(txbuf,"MAS,%s,%s,%d",air_name,air_state,air_gear);
        Key_TX_Data_Check();
    }
    else if(menu_NowMark->dev==&lock)
    {
        sprintf(txbuf,"MAS,%s,%s,%d",lock_name,lock_state,lock_gear);
        Key_TX_Data_Check();
    }

}

void Key_TX_Data_Check(void)
{
    // if(NRF24L01_TxPacket((uint8_t *)txbuf) == 0)
    // {
        HAL_Delay(1);
        NRF24L01_TxPacket((uint8_t *)txbuf);  
        HAL_Delay(1);
        NRF24L01_TxPacket((uint8_t *)txbuf);
        HAL_Delay(1);
        NRF24L01_TxPacket((uint8_t *)txbuf);  
        HAL_Delay(1);
        NRF24L01_TxPacket((uint8_t *)txbuf);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    
        //发送成功
        NRF24L01_RX_Mode(Unite_ADDRESS);
        HAL_UART_Transmit(&huart2,(uint8_t *)txbuf,strlen(txbuf),HAL_MAX_DELAY);
        writeStr(txbuf," ");
        HAL_TIM_Base_Start_IT(&htim4);
        __HAL_TIM_SET_COUNTER(&htim4, 0);
        while (1)
        {
            if(NRF24L01_RxPacket(rxbuf) == 0)
            {
                if(CompareStr((char*)rxbuf,"OK"))
                {
                    HAL_TIM_Base_Stop_IT(&htim4);
                    OLED_Clear();
                    OLED_ShowString(3, 3, "Send Success!");
                    HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_SET);
                    HAL_Delay(10);
                    HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET);
                    HAL_Delay(1000-10);
                    OLED_Clear();
                    writeStr((char*)rxbuf," ");  
                    break;
                }
                
            }
            else if(Time_Up==1)
                {
                    Time_Up=0;
                    //错误处理
                    OLED_Clear();
                    OLED_ShowString(2, 7, "Error");
                    OLED_ShowString(3, 5, "Send Fail!");
                    HAL_Delay(1000);
                    OLED_Clear();
                    break;
                }
        }  
       
    // }
    // else
    // {
    //    //错误处理
    //    OLED_Clear();
    //    OLED_ShowString(2, 7, "Error");
    //    OLED_ShowString(3, 5, "Send Fail!");
    //    HAL_Delay(1000);
    //    OLED_Clear();
    // }
}

uint16_t Time_Up=0;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim==&htim4) {
       Time_Up=1;
       HAL_TIM_Base_Stop_IT(&htim4);
    }
}


