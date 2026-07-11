/*
lib_main.c - главный модуль библиотеки.

Бабурин Дмитрий Сергеевич
МК-101
*/

#include "lib_main.h"
#include <string.h>
#include <ctype.h>

#ifndef N
#define N 1024 // Буфер по умолчанию, если не задали при сборке
#endif

// Перевод одного hex символа в числовое значение
static int parse_hex_char(char symbol) {
    if (symbol >= '0' && symbol <= '9') {
        return symbol - '0';
    }
    if (symbol >= 'a' && symbol <= 'f') {
        return symbol - 'a' + 10;
    }
    if (symbol >= 'A' && symbol <= 'F') {
        return symbol - 'A' + 10;
    }
    return -1; // Вернем ошибку, если символ левый
}

// Конвертация всей hex строки в бинарный буфер
int convert_hex_to_bin(const char *hex_str, uint8_t *bin_buffer, size_t *parsed_length) {
    // Простейшая проверка указателей на NULL
    if (hex_str == NULL || bin_buffer == NULL || parsed_length == NULL) {
        return -1;
    }

    size_t string_len = strlen(hex_str);
    if (string_len % 2 != 0) {
        return -1; // хекс строка обязана иметь четную длину
    }

    size_t count_bytes = 0;
    for (size_t i = 0; i < string_len; i += 2) {
        int first_half = parse_hex_char(hex_str[i]);
        int second_half = parse_hex_char(hex_str[i + 1]);

        // Если нашли некорректный символ - выходим с ошибкой
        if (first_half == -1 || second_half == -1) {
            return -1;
        }

        // Склеиваем два полубайта в один целый байт
        bin_buffer[count_bytes] = (uint8_t)((first_half << 4) | second_half);
        count_bytes++;
    }

    *parsed_length = count_bytes;
    return 0; // Успешно декодировали
}

// Поиск и замена последовательностей 
int replace_file_bytes(FILE *input_stream, FILE *output_stream,
                       const uint8_t *search_pattern, size_t search_pattern_len,
                       const uint8_t *replace_pattern, size_t replace_pattern_len) {
    
    // Проверка входящих параметров
    if (input_stream == NULL || output_stream == NULL || 
        search_pattern == NULL || search_pattern_len == 0 || replace_pattern == NULL) {
        return -1;
    }

    uint8_t file_buffer[N];  // Буфер чтения 
    size_t current_match = 0; // Счетчик совпавших байт шаблона

    while (1) {
        // Читаем блок из входного файла
        size_t read_size = fread(file_buffer, 1, N, input_stream);
        if (read_size == 0) {
            if (ferror(input_stream)) {
                return -1; 
            }
            break; // Дошли до конца файла
        }

        // Перебираем прочитанный блок побайтово
        for (size_t i = 0; i < read_size; ++i) {
            uint8_t current_byte = file_buffer[i];

            if (current_byte == search_pattern[current_match]) {
                current_match++;
                // Если совпали полностью - пишем замену
                if (current_match == search_pattern_len) {
                    if (fwrite(replace_pattern, 1, replace_pattern_len, output_stream) != replace_pattern_len) {
                        return -1;
                    }
                    current_match = 0; // Начинаем искать заново
                }
            } else {
                // Ошибка совпадения, делаем динамический откат назад
                size_t fallback_len = 0;
                for (size_t temp_len = current_match; temp_len >= 1; --temp_len) {
                    // Проверяем, совпадает ли конец нашей цепочки с началом шаблона
                    if (current_byte == search_pattern[temp_len - 1]) {
                        int is_matched = 1;
                        for (size_t j = 0; j < temp_len - 1; ++j) {
                            if (search_pattern[j] != search_pattern[current_match - temp_len + 1 + j]) {
                                is_matched = 0;
                                break;
                            }
                        }
                        if (is_matched) {
                            fallback_len = temp_len;
                            break;
                        }
                    }
                }

                // Считаем сколько байт нужно сбросить в файл безвозвратно
                size_t out_length = current_match + 1 - fallback_len;
                if (out_length <= current_match) {
                    // Записываем кусок шаблона
                    if (fwrite(search_pattern, 1, out_length, output_stream) != out_length) {
                        return -1;
                    }
                } else {
                    // Если совпадений нет - сбрасываем весь шаблон и текущий байт
                    if (current_match > 0) {
                        if (fwrite(search_pattern, 1, current_match, output_stream) != current_match) {
                            return -1;
                        }
                    }
                    if (fwrite(&current_byte, 1, 1, output_stream) != 1) {
                        return -1;
                    }
                }

                current_match = fallback_len; // Устанавливаем новое состояние поиска
            }
        }
    }

    // Выгружаем остаток шаблона, если файл кончился на полуслове
    if (current_match > 0) {
        if (fwrite(search_pattern, 1, current_match, output_stream) != current_match) {
            return -1;
        }
    }

    return 0; // Всё ок
}

