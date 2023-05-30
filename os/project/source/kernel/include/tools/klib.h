#ifndef KLIB_H
#define KLIB_H

#include "common/types.h"

void kernel_strcpy(char* dest, const char* src);
void kernel_strncpy(char* dest, const char* src, int size);
int kernel_strncmp(const char* s1, const char* s2, int size);
int kernel_strlen(const char* str);
void kernel_memcpy(void* dest, void* src, int size);
void kernel_memset(void* dest, uint8_t v, int size);
int kernel_memcmp(void* d1, void* d2, int size);

#endif //KLIB_H