/*
test_app.c - модуль проверки программы. 

Бабурин Дмитрий Сергеевич
*/

#include "lib_main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void run_integration_test(const uint8_t *input, size_t input_len, const uint8_t *search, size_t search_len, const uint8_t *replace, size_t replace_len, const uint8_t *expected, size_t expected_len) {
    const char *temp_in_path = "temp_input.bin";
    const char *temp_out_path = "temp_output.bin";

    FILE *temp_in = fopen(temp_in_path, "wb");
    assert(temp_in != NULL);
    if (input_len > 0) {
        assert(fwrite(input, 1, input_len, temp_in) == input_len);
    }
    fclose(temp_in);

    FILE *in = fopen(temp_in_path, "rb");
    FILE *out = fopen(temp_out_path, "wb");
    assert(in != NULL && out != NULL);

    int res = replace_bytes(in, out, search, search_len, replace, replace_len);
    assert(res == 0);

    fclose(in);
    fclose(out);

    FILE *temp_out = fopen(temp_out_path, "rb");
    assert(temp_out != NULL);

    uint8_t *result_buf = malloc(expected_len + 1);
    size_t bytes_read = fread(result_buf, 1, expected_len + 1, temp_out);
    
    assert(bytes_read == expected_len);
if (expected_len > 0) {
        assert(memcmp(result_buf, expected, expected_len) == 0);
    }

    fclose(temp_out);
    free(result_buf);

    remove(temp_in_path);
    remove(temp_out_path);
}

void test_simple_replace(void) {
    uint8_t input[] = {'a', 'b', 'c'};
    uint8_t search[] = {'b'};
    uint8_t replace[] = {'x'};
    uint8_t expected[] = {'a', 'x', 'c'};
    run_integration_test(input, 3, search, 1, replace, 1, expected, 3);
    printf("test_simple_replace passed!.\n");
}

void test_no_match(void) {
    uint8_t input[] = {'a', 'b', 'c'};
    uint8_t search[] = {'z'};
    uint8_t replace[] = {'x'};
    uint8_t expected[] = {'a', 'b', 'c'};
    run_integration_test(input, 3, search, 1, replace, 1, expected, 3);
    printf("Test 8 (No Match) passed.\n");
}

void test_backtrack_match(void) {
    uint8_t input[] = {'A', 'B', 'A', 'B', 'A', 'C'};
    uint8_t search[] = {'A', 'B', 'A', 'C'};
    uint8_t replace[] = {'X'};
    uint8_t expected[] = {'A', 'B', 'X'};
    run_integration_test(input, 6, search, 4, replace, 1, expected, 3);
    printf("Test 9 (Backtrack Match) passed.\n");
}

void test_empty_file(void) {
    uint8_t input[] = {0};
    uint8_t search[] = {'a'};
    uint8_t replace[] = {'x'};
    uint8_t expected[] = {0};
    run_integration_test(input, 0, search, 1, replace, 1, expected, 0);
    printf("Test 10 (Empty File) passed.\n");
}

void test_binary_null_bytes(void) {
    uint8_t input[] = {0x00, 0x01, 0x00, 0x02, 0x00};
    uint8_t search[] = {0x00, 0x02};
    uint8_t replace[] = {0xFF, 0xFF};
    uint8_t expected[] = {0x00, 0x01, 0xFF, 0xFF, 0x00};
    run_integration_test(input, 5, search, 2, replace, 2, expected, 5);
    printf("Test 11 (Binary Null Bytes) passed.\n");
}

int main(void) {
    test_simple_replace();
    test_no_match();
    test_backtrack_match();
    test_empty_file();
    test_binary_null_bytes();

    printf("All integration tests passed successfully!.\n");
    return 0;
}