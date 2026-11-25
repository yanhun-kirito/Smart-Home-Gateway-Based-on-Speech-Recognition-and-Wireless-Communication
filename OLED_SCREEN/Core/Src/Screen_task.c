#include <stdio.h>
#include "Screen_task.h"

//定义页面指示符
char page_mark[10] = {0};

device fan={fan_name,&fan_state_data,&fan_gear_data,fan_mark};
device light={light_name,&light_state_data,&light_gear_data,light_mark};
device air={air_name,&air_state_data,&air_gear_data,air_mark};
device lock={lock_name,&lock_state_data,&light_gear_data,lock_mark};

menu page1={&fan,&light,&air,&lock};

void show_menu(menu* page)
{
    // 显示菜单
    OLED_ShowString(1,2,page->dev1->mark);
    OLED_ShowString(1,8,page->dev1->name);
    OLED_ShowString(2,2,page->dev2->mark);
    OLED_ShowString(2,8,page->dev2->name);
    OLED_ShowString(3,2,page->dev3->mark);
    OLED_ShowString(3,8,page->dev3->name);
    OLED_ShowString(4,2,page->dev4->mark);
    OLED_ShowString(4,8,page->dev4->name);
}

void show_device(void)
{
    //显示设备
    OLED_ShowString(1,3,device_Back.mark_data);
    OLED_ShowString(2,8,menu_NowMark->dev->name);
    OLED_ShowString(3,2,menu_NowMark->dev->state->mark_data);
    OLED_ShowString(3,6,"State:");
    OLED_ShowString(3,12,menu_NowMark->dev->state->state_data); 
    OLED_ShowString(4,2,menu_NowMark->dev->gear->mark_data);
    OLED_ShowString(4,6,"Gear:");
    OLED_ShowNum(4, 12, (uint32_t)*(menu_NowMark->dev->gear->gear_data), 2);
    if(menu_NowMark->dev==&lock)
    {
        OLED_ShowNum(4, 12, (uint32_t)(lock_gear), 2);
    }
    
}


// 定义菜单指针
menu_mark fan_menu_mark={&fan,&lock_menu_mark,&light_menu_mark};
menu_mark light_menu_mark={&light,&fan_menu_mark,&air_menu_mark};
menu_mark air_menu_mark={&air,&light_menu_mark,&lock_menu_mark};
menu_mark lock_menu_mark={&lock,&air_menu_mark,&fan_menu_mark};

// 定义菜单操作指针
menu_mark* menu_NowMark = NULL;
menu_mark* menu_PrevMark = NULL;
menu_mark* menu_NextMark = NULL;

//定义设备指针
device_mark device_Back_mark={&device_Back,NULL,NULL};
device_mark fan_state_list_mark={&fan_state_data,&device_Back_mark,&fan_gear_list_mark};
device_mark fan_gear_list_mark={&fan_gear_data,&fan_state_list_mark,&device_Back_mark};
device_mark light_state_list_mark={&light_state_data,&device_Back_mark,&light_gear_list_mark};
device_mark light_gear_list_mark={&light_gear_data,&light_state_list_mark,&device_Back_mark};
device_mark air_state_list_mark={&air_state_data,&device_Back_mark,&air_gear_list_mark};
device_mark air_gear_list_mark={&air_gear_data,&air_state_list_mark,&device_Back_mark};
device_mark lock_state_list_mark={&lock_state_data,&device_Back_mark,&lock_gear_list_mark};
device_mark lock_gear_list_mark={&lock_gear_data,&lock_state_list_mark,&device_Back_mark};

// 定义设备操作指针
device_mark* device_NowMark = NULL;
device_mark* device_PrevMark = NULL;
device_mark* device_NextMark = NULL;


void menu_mark_Handle(void)
{   
    // // 清空上一个选项的标记
    // menu_PrevMark->dev->mark = "[ ]";
    // // 清空下一个选项的标记
    // menu_NextMark->dev->mark = "[ ]";
    // // 显示当前选项的标记
    // menu_NowMark->dev->mark = "[*]";

    // 清空上一个选项的标记
    writeStr(menu_PrevMark->dev->mark,"[ ]");
    // 清空下一个选项的标记
    writeStr(menu_NextMark->dev->mark,"[ ]");
    // 显示当前选项的标记
    writeStr(menu_NowMark->dev->mark,"[*]");
}


void device_mark_Handle(void)
{
    if(device_PrevMark ->dev_data==&device_Back)
      {
        writeStr(menu_NowMark->dev->state->mark_data,"[*]");
        writeStr(menu_NowMark->dev->gear->mark_data,"[ ]");
      }   
   else if(device_NowMark ->dev_data==&device_Back)
      {
        writeStr(menu_NowMark->dev->state->mark_data,"[ ]");
        writeStr(menu_NowMark->dev->gear->mark_data,"[ ]");
      }
   else if(device_NextMark ->dev_data==&device_Back)
      {
        writeStr(menu_NowMark->dev->state->mark_data,"[ ]");
        writeStr(menu_NowMark->dev->gear->mark_data,"[*]");
      }
}

// 初始化菜单
void menu_init(void)
{     
    mark_init();
    menu_NowMark = &fan_menu_mark;
    menu_PrevMark = menu_NowMark->prev;
    menu_NextMark = menu_NowMark->next;
    menu_mark_Handle();
    writeStr(page_mark, "menu");   
    device_PrevMark = NULL;
    device_NowMark = NULL;
    device_NextMark = NULL;
    device_Back_mark.prev=NULL;
    device_Back_mark.next=NULL;
		OLED_Clear();  
    show_menu(&page1);
}










