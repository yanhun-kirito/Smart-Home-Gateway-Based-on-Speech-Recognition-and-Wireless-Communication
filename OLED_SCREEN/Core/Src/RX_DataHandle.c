#include "RX_DataHandle.h"


// 辅助函数：去除字符串首尾的空格
void trim(char *str) {
    if (str == NULL) return;

    // 去除前导空格
    char *start = str;
    while (isspace((unsigned char)*start)) start++;

    // 全空格情况
    if (*start == '\0') {
        *str = '\0';
        return;
    }

    // 去除尾部空格
    char *end = start + strlen(start) - 1;
    while (end > start && isspace((unsigned char)*end)) end--;
    *(end + 1) = '\0';

    // 移动内容到起始位置（如果前导有空格）
    if (start != str) {
        memmove(str, start, end - start + 2); // +2 包含结束符 '\0'
    }
}

// 主函数：提取变量
ExtractResult receive_data_handle(
    const char *input,
    char *var1, size_t var1_size,
    char *var2, size_t var2_size,
    char *var3, size_t var3_size,
    int *var4 
) {
    // 检查输入和输出指针有效性
    if (!input || !var1 || !var2 || !var3 || !var4) {
        return EXTRACT_INVALID_INPUT;
    }

    // 复制输入字符串（避免修改原字符串）
    char *input_copy = malloc(strlen(input) + 1);
    if (!input_copy) return EXTRACT_INVALID_INPUT;
    strcpy(input_copy, input);

    // 分割字符串
    char *tokens[4] = {0};
    int count = 0;
    char *token = strtok(input_copy, ",");

    while (token && count < 4) {
        tokens[count++] = token;
        token = strtok(NULL, ",");
    }

    // 检查是否分割出4个字段
    if (count != 4) {
        free(input_copy);
        return EXTRACT_NOT_ENOUGH_PARTS;
    }

    // 去除每个字段首尾空格
    for (int i = 0; i < 4; i++) {
        trim(tokens[i]);
    }

    // 检查并复制前三个字符串
    if (strlen(tokens[0]) >= var1_size ||
        strlen(tokens[1]) >= var2_size ||
        strlen(tokens[2]) >= var3_size) {
        free(input_copy);
        return EXTRACT_INVALID_INPUT; // 缓冲区不足
    }

    strncpy(var1, tokens[0], var1_size);
    var1[var1_size - 1] = '\0';

    strncpy(var2, tokens[1], var2_size);
    var2[var2_size - 1] = '\0';

    strncpy(var3, tokens[2], var3_size);
    var3[var3_size - 1] = '\0';

    // 转换第四个字段为整数
    errno = 0;
    char *endptr;
    long num = strtol(tokens[3], &endptr, 10);

    if (*endptr != '\0' || errno == ERANGE || num < INT_MIN || num > INT_MAX) {
        free(input_copy);
        return EXTRACT_INVALID_INTEGER;
    }
    *var4 = (int)num;

    free(input_copy);
    return EXTRACT_SUCCESS;
}


