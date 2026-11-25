#ifndef SCREEN_TASK_H
#define SCREEN_TASK_H
#include "OLED.h"
#include "device_data.h"
#include "main.h"
#include "key_task.h"

// 设备结构体定义
typedef struct device
{     
    char* name;   // 设备名称
    device_data* state;  // 设备状态
    device_data* gear;    // 设备档位  
    char* mark;   // 移动标号
} device;

// 菜单结构体定义，包含4个设备指针
typedef struct menu
{
    device* dev1;  // 设备1
    device* dev2;  // 设备2
    device* dev3;  // 设备3
    device* dev4;  // 设备4
} menu;

//页面指示
extern char page_mark[10];

// 设备实例
extern device fan;
extern device light;
extern device air;
extern device lock;

// 菜单实例
extern menu page1;

// 菜单标记结构体定义
typedef struct menu_mark
{   
    device* dev;
    struct menu_mark* prev;
    struct menu_mark* next;
}menu_mark;

// 设备标记结构体定义
typedef struct device_mark
{
    device_data* dev_data;
    struct device_mark* prev;
    struct device_mark* next;
}device_mark;

extern menu_mark fan_menu_mark;
extern menu_mark light_menu_mark;
extern menu_mark air_menu_mark;
extern menu_mark lock_menu_mark;


extern menu_mark* menu_PrevMark;
extern menu_mark* menu_NowMark;
extern menu_mark* menu_NextMark;

extern device_mark device_Back_mark;
extern device_mark fan_state_list_mark;
extern device_mark fan_gear_list_mark;
extern device_mark light_state_list_mark;
extern device_mark light_gear_list_mark;
extern device_mark air_state_list_mark;
extern device_mark air_gear_list_mark;
extern device_mark lock_state_list_mark;
extern device_mark lock_gear_list_mark;

extern device_mark* device_PrevMark;
extern device_mark* device_NowMark;
extern device_mark* device_NextMark;

// 函数声明
void show_menu(menu* page);
void menu_init(void);
void menu_mark_Handle(void);
void device_mark_Handle(void);
void show_device(void);
#endif

