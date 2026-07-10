/*
lib_main.h - заголовочный файл модуля lib_main.c. 

Бабурин Дмитрий Сергеевич
МК-101
*/

#ifndef LIB_MAIN_H
#define LIB_MAIN_H

#include <stddef.h>
#include <stdint.h>


int hex_to_bytes(const char *hex, uint8_t *out_bytes, size_t *out_len);

#endif // LIB_MAIN_H
