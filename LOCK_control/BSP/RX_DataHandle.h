#ifndef __RX_DATAHANDLE_H__
#define __RX_DATAHANDLE_H__

#include "stm32f1xx_hal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>

// 定义错误码枚举
typedef enum {
    EXTRACT_SUCCESS,            // 成功
    EXTRACT_INVALID_INPUT,      // 输入指针无效
    EXTRACT_NOT_ENOUGH_PARTS,   // 字段不足4个
    EXTRACT_INVALID_INTEGER     // 第四个字段不是整数
} ExtractResult;

void trim(char *str);

ExtractResult receive_data_handle(
    const char *input,
    char *var1, size_t var1_size,
    char *var2, size_t var2_size,
    char *var3, size_t var3_size,
    int *var4
);








#endif



