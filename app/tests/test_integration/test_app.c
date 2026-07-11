/*
test_app.c - модуль проверки программы. 

Бабурин Дмитрий Сергеевич
МК-101
*/

#include "lib_main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// Вспомогательная функция для запуска теста над файлами на диске
void execute_file_test(const uint8_t *input_data, size_t input_len,
                       const uint8_t *search_pattern, size_t search_len,
                       const uint8_t *replace_pattern, size_t replace_len,
                       const uint8_t *expected_data, size_t expected_len) {
    const char *input_filename = "temp_in.bin";
    const char *output_filename = "temp_out.bin";

    // 1 Записываем тестовые данные в первый файл
    FILE *temp_in = fopen(input_filename, "wb");
    assert(temp_in != NULL);
    if (input_len > 0) {
        assert(fwrite(input_data, 1, input_len, temp_in) == input_len);
    }
    fclose(temp_in);

    // 2 Открываем дескрипторы для работы библиотечного алгоритма
    FILE *file_in = fopen(input_filename, "rb");
    FILE *file_out = fopen(output_filename, "wb");
    assert(file_in != NULL && file_out != NULL);

    // 3 Запускаем процедуру замены байт
    int status = replace_file_bytes(file_in, file_out, search_pattern, search_len, replace_pattern, replace_len);
    assert(status == 0);

    fclose(file_in);
    fclose(file_out);

    // 4 Считываем результат и побайтово сверяем 
    FILE *temp_out = fopen(output_filename, "rb");
    assert(temp_out != NULL);

    uint8_t *result_buffer = malloc(expected_len + 1);
    size_t bytes_read = fread(result_buffer, 1, expected_len + 1, temp_out);

    assert(bytes_read == expected_len);
    if (expected_len > 0) {
        assert(memcmp(result_buffer, expected_data, expected_len) == 0);
    }

    fclose(temp_out);
    free(result_buffer);

    // 5 Очищаем жесткий диск от тестового мусора
    remove(input_filename);
    remove(output_filename);
}

// Тест 7 Простая замена одного байта
void test_simple_replacement(void) {
    uint8_t input[] = {'a', 'b', 'c'};
    uint8_t search[] = {'b'};
    uint8_t replace[] = {'x'};
    uint8_t expected[] = {'a', 'x', 'c'};
    execute_file_test(input, 3, search, 1, replace, 1, expected, 3);
    printf("Test 7 (Simple Replace) passed.\n");
}

// Тест 8 Шаблон поиска отсутствует в исходных данных
void test_no_matches_found(void) {
    uint8_t input[] = {'a', 'b', 'c'};
    uint8_t search[] = {'z'};
    uint8_t replace[] = {'x'};
    uint8_t expected[] = {'a', 'b', 'c'};
    execute_file_test(input, 3, search, 1, replace, 1, expected, 3);
    printf("Test 8 (No Match) passed.\n");
}

// Тест 9 Неудачное частичное совпадение с последующим откатом
void test_dynamic_fallback_backtrack(void) {
    uint8_t input[] = {'A', 'B', 'A', 'B', 'A', 'C'};
    uint8_t search[] = {'A', 'B', 'A', 'C'};
    uint8_t replace[] = {'X'};
    uint8_t expected[] = {'A', 'B', 'X'};
    execute_file_test(input, 6, search, 4, replace, 1, expected, 3);
    printf("Test 9 (Backtrack Match) passed.\n");
}

// Тест 10 Пустой файл на входе
void test_empty_input_file(void) {
    uint8_t input[] = {0};
    uint8_t search[] = {'a'};
    uint8_t replace[] = {'x'};
    uint8_t expected[] = {0};
    execute_file_test(input, 0, search, 1, replace, 1, expected, 0);
    printf("Test 10 (Empty File) passed.\n");
}

// Тест 11 Работа с бинарными нулями
void test_binary_null_bytes_safety(void) {
    uint8_t input[] = {0x00, 0x01, 0x00, 0x02, 0x00};
    uint8_t search[] = {0x00, 0x02};
    uint8_t replace[] = {0xFF, 0xFF};
    uint8_t expected[] = {0x00, 0x01, 0xFF, 0xFF, 0x00};
    execute_file_test(input, 5, search, 2, replace, 2, expected, 5);
    printf("Test 11 (Binary Null Bytes) passed.\n");
}

int main(void) {
    test_simple_replacement();
    test_no_matches_found();
    test_dynamic_fallback_backtrack();
    test_empty_input_file();
    test_binary_null_bytes_safety();

    printf("All integration tests passed successfully!\n");
    return 0;
}

