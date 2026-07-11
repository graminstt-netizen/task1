/*
test_lib.c - модуль проверки библиотеки. 

Бабурин Дмитрий Сергеевич
МК-101
*/

#include "lib_main.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

// Тестирование корректных сценариев конвертации хекс
void test_hex_conversion_valid(void) {
    uint8_t output_buffer[16];
    size_t decoded_length = 0;

    // Тест 1: Простая Hex строка
    assert(convert_hex_to_bin("414243", output_buffer, &decoded_length) == 0);
    assert(decoded_length == 3);
    assert(output_buffer[0] == 0x41 && output_buffer[1] == 0x42 && output_buffer[2] == 0x43);

    // Тест 2: Проверка обработки бинарного нуля внутри строки
    assert(convert_hex_to_bin("410042", output_buffer, &decoded_length) == 0);
    assert(decoded_length == 3);
    assert(output_buffer[0] == 0x41 && output_buffer[1] == 0x00 && output_buffer[2] == 0x42);

    // Тест 3: Смешанный регистр символов 
    assert(convert_hex_to_bin("aBcdEF", output_buffer, &decoded_length) == 0);
    assert(decoded_length == 3);
    assert(output_buffer[0] == 0xAB && output_buffer[1] == 0xCD && output_buffer[2] == 0xEF);
    
    printf("test_hex_conversion_valid passed.\n");
}

// Тестирование обработки ошибок и неверных данных
void test_hex_conversion_invalid(void) {
    uint8_t output_buffer[16];
    size_t decoded_length = 0;

    // Тест 1: Нечетная длина строки (ошибка)
    assert(convert_hex_to_bin("414", output_buffer, &decoded_length) == -1);

    // Тест 2: Недопустимый символ в строке (буква G)
    assert(convert_hex_to_bin("414G", output_buffer, &decoded_length) == -1);

    // Тест 3: Передача нулевого указателя (NULL)
    assert(convert_hex_to_bin(NULL, output_buffer, &decoded_length) == -1);
    
    printf("test_hex_conversion_invalid passed.\n");
}

int main(void) {
    test_hex_conversion_valid();
    test_hex_conversion_invalid();
    
    printf("All initial tests passed!\n");
    return 0;
}
