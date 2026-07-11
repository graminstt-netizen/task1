/*
test_lib.c - модуль проверки библиотеки. 

Бабурин Дмитрий Сергеевич
МК-101
*/

#include "lib_main.h"
#include <assert.h>
#include <stdio.h>
#include <string.h> 

void test_hex_to_bytes_valid(void) {
    uint8_t out[16];
    size_t out_len = 0;

    //test 1 простая строка
    assert(hex_to_bytes("414243", out, &out_len) == 0);
    assert(out_len == 3);
    assert(out[0] == 0x41 && out[1] == 0x42 && out[2] == 0x43);

    //test 2 строка с нулевым байтом посередине
    assert(hex_to_bytes("410042", out, &out_len) == 0);
    assert(out_len == 3);
    assert(out[0] == 0x41 && out[1] == 0x00 && out[2] == 0x42);

    //test 3 разный регистр букв
    assert(hex_to_bytes("aBcdEF", out, &out_len) == 0);
    assert(out_len == 3);
    assert(out[0] == 0xAB && out[1] == 0xCD && out[2] == 0xEF);

    printf("test_hex_to_bytes_valid passed!.\n");
}

void test_hex_to_bytes_invalid(void) {
    uint8_t out[16];
    size_t out_len = 0;

    //test 1 нечетная длина строки
    assert(hex_to_bytes("414", out, &out_len) == -1); 

    //test 2 недопустимый символ
    assert(hex_to_bytes("414G", out, &out_len) == -1);

    //test 3 пустые указатели
    assert(hex_to_bytes(NULL, out, &out_len) == -1);

    printf("test_hex_to_bytes_invalid passed!.\n");
}

int main(void) {
    test_hex_to_bytes_valid();
    test_hex_to_bytes_invalid();

    printf("All initial tests passed!.\n");
    return 0;
}