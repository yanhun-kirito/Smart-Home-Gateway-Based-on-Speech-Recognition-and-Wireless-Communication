#include "device_data.h"


// 风扇数据
char fan_name[10] = "Fan";
char fan_state[10] = "OFF";
char fan_state_mark[4] = "[ ]";
int fan_gear = 0;
char fan_gear_mark[4] = "[ ]";
char fan_mark[4] = "[ ]";

device_data fan_state_data = {fan_state_mark,fan_state,NULL};
device_data fan_gear_data = {fan_gear_mark,NULL,&fan_gear};

// 台灯数据
char light_name[10] = "Light";
char light_state[10] = "OFF";
char light_state_mark[4] = "[ ]";
int light_gear = 0;
char light_gear_mark[4] = "[ ]";
char light_mark[4] = "[ ]";

device_data light_state_data = {light_state_mark,light_state,NULL};
device_data light_gear_data = {light_gear_mark,NULL,&light_gear};

// 空调数据
char air_name[10] = "Air";
char air_state[10] = "OFF";
char air_state_mark[4] = "[ ]";
int air_gear = 16;
char air_gear_mark[4] = "[ ]";
char air_mark[4] = "[ ]";

device_data air_state_data = {air_state_mark,air_state,NULL};
device_data air_gear_data = {air_gear_mark,NULL,&air_gear};

// 智能锁数据
char lock_name[10] = "Lock";
char lock_state[10] = "OFF";
char lock_state_mark[4] = "[ ]";
int lock_gear = 0;
char lock_gear_mark[4] = "[ ]";
char lock_mark[4] = "[ ]";

device_data lock_state_data = {lock_state_mark,lock_state,NULL};
device_data lock_gear_data = {lock_gear_mark,NULL,&lock_gear};

//返回选项数据
char Back_mark[4] = "<";
device_data device_Back = {Back_mark,NULL,NULL};

void mark_init(void)
{
    writeStr(fan_state_mark, "[ ]");
    writeStr(fan_gear_mark, "[ ]");
    writeStr(fan_mark, "[ ]");
    writeStr(light_state_mark, "[ ]");
    writeStr(light_gear_mark, "[ ]");
    writeStr(light_mark, "[ ]");
    writeStr(air_state_mark, "[ ]");
    writeStr(air_gear_mark, "[ ]");
    writeStr(air_mark, "[ ]");
    writeStr(lock_state_mark, "[ ]");
    writeStr(lock_gear_mark, "[ ]");
    writeStr(lock_mark, "[ ]");
    writeStr(Back_mark, "<");
}


void writeStr(char *strArray, const char *str)
{
    // 清空字符串数组
    memset(strArray, 0, strlen(strArray));  // 将数组内容清空，可以也直接通过指针赋值
    // 将新字符串赋值到数组
    strcpy(strArray, str);  // 使用 strcpy 将字符串复制到数组
}


void device_data_init(void)
{
    
    writeStr(fan_state, "OFF");
    fan_gear = 0;

    writeStr(light_state, "OFF");
    light_gear = 0;

    writeStr(air_state, "OFF");
    air_gear = 16;

    writeStr(lock_state, "OFF");
    lock_gear = 0;
}


// 通用串口发送函数
void print(UART_HandleTypeDef *huart, const char *format, ...)
{
    char buffer[128];  // 发送缓冲区，适当调整大小
    va_list args;
    
    // 初始化变长参数列表
    va_start(args, format);
    
    // 格式化数据写入缓冲区
    vsnprintf(buffer, sizeof(buffer), format, args);
    
    // 结束变长参数列表
    va_end(args);
    
    // 通过串口发送字符串
    HAL_UART_Transmit(huart, (uint8_t *)buffer, strlen(buffer), HAL_MAX_DELAY);
}

// 主控初始化
void Master_init(void)
{
   // 检测模块是否正常
  if(NRF24L01_Check() == 0)
   { 
    // 设置为接收模式
    NRF24L01_RX_Mode(Unite_ADDRESS);

   }
   else
   {
    //错误处理


   }
}



