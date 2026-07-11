/*
lib_main.h - заголовочный файл модуля lib_main.c. 

Бабурин Дмитрий Сергеевич
МК-101
*/

#ifndef LIB_MAIN_H
#define LIB_MAIN_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

// Функция для перевода шестнадцатеричной строки в бинарные байты
int convert_hex_to_bin(const char *hex_str, uint8_t *bin_buffer, size_t *parsed_length);

// Функция для побайтового поиска и замены в открытых файлах
int replace_file_bytes(FILE *input_stream, FILE *output_stream,
                       const uint8_t *search_pattern, size_t search_pattern_len,
                       const uint8_t *replace_pattern, size_t replace_pattern_len);

#endif /* LIB_MAIN_H */
