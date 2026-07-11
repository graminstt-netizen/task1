/*
main.c - главный модуль программы. 

Бабурин Дмитрий Сергеевич
МК-101
*/

#include "lib_main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    // Проверяем, что передали ровно 4 аргумента запуска
    if (argc != 5) {
        fprintf(stderr, "Usage: %s <input_file> <output_file> <search_hex> <replace_hex>\n", argv[0]);
        fprintf(stderr, "Example: %s test.bin out.bin 4142 3132\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Названия файлов и шестнадцатеричные шаблоны из консоли
    const char *path_in = argv[1];
    const char *path_out = argv[2];
    const char *hex_find_str = argv[3];
    const char *hex_replace_str = argv[4];

    size_t hex_find_len = strlen(hex_find_str);
    size_t hex_replace_len = strlen(hex_replace_str);

    // Выделяем память под будущие массивы байт
    uint8_t *bytes_to_find = malloc(hex_find_len / 2 + 1);
    uint8_t *bytes_to_replace = malloc(hex_replace_len / 2 + 1);

    // Если система не выделила память, то выходим
    if (bytes_to_find == NULL || bytes_to_replace == NULL) {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        free(bytes_to_find);
        free(bytes_to_replace);
        return EXIT_FAILURE;
    }

    size_t find_len = 0;
    size_t rep_len = 0;

    // Конвертируем строку поиска в байты
    if (convert_hex_to_bin(hex_find_str, bytes_to_find, &find_len) != 0) {
        fprintf(stderr, "Error: Invalid search hex string format.\n");
        free(bytes_to_find);
        free(bytes_to_replace);
        return EXIT_FAILURE;
    }

    // Конвертируем строку замены в байты
    if (convert_hex_to_bin(hex_replace_str, bytes_to_replace, &rep_len) != 0) {
        fprintf(stderr, "Error: Invalid replace hex string format.\n");
        free(bytes_to_find);
        free(bytes_to_replace);
        return EXIT_FAILURE;
    }

    // Открываем файл для чтения в бинарном режиме
    FILE *file_in = fopen(path_in, "rb");
    if (file_in == NULL) {
        fprintf(stderr, "Error: Cannot open input file %s\n", path_in);
        free(bytes_to_find);
        free(bytes_to_replace);
        return EXIT_FAILURE;
    }

    // Создаем файл для записи в бинарном режиме
    FILE *file_out = fopen(path_out, "wb");
    if (file_out == NULL) {
        fprintf(stderr, "Error: Cannot create output file %s\n", path_out);
        fclose(file_in);
        free(bytes_to_find);
        free(bytes_to_replace);
        return EXIT_FAILURE;
    }

    // Запускаем основной процесс замены байт в файле
    int status = replace_file_bytes(file_in, file_out, bytes_to_find, find_len, bytes_to_replace, rep_len);

    // Закрываем дескрипторы файлов и освобождаем выделенную память
    fclose(file_in);
    fclose(file_out);
    free(bytes_to_find);
    free(bytes_to_replace);

    // Если алгоритм вернул ошибку - пишем об этом
    if (status != 0) {
        fprintf(stderr, "Error: Replacement process failed.\n");
        return EXIT_FAILURE;
    }

    printf("Success: Replacement completed.\n");
    return EXIT_SUCCESS;
}
