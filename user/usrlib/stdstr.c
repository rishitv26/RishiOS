#include "stdstr.h"

size_t strlen(void* str) {
    int tor = 0;
    while (*((char*)str + tor) != '\0') tor++;
    return tor;
}
