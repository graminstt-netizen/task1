/*
main.c - главный модуль программы. 

Бабурин Дмитрий Сергеевич
МК-101
*/

#include <lib_main.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main (int argc, char *argv[])
{// Проверяем, что переданы все 4 обязательных аргумента командной строки
	if (argc != 5)
	{
		fprintf(stderr, "Usage: %s <input_file> <output_file> <search_hex> <replace_hex>\n", argv[0]);
		fprintf(stderr, "Example: %s input.bin output.bin 4142 3132\n", argv[0]);
		return EXIT_FAILURE;
	}

	const char *input_path = argv[1];
	const char *output_path = argv[2];
	const char *search_hex = argv[3];
	const char *replace_hex = argv[4];

	size_t search_hex_len = strlen(search_hex);
	size_t replace_hex_len = strlen(replace_hex);

	uint8_t *search_bytes = malloc(search_hex_len / 2 + 1);
	uint8_t *replace_bytes_arr = malloc(replace_hex_len / 2 + 1);

	if (search_bytes == NULL || replace_bytes_arr == NULL)
	{
		fprintf(stderr, "Memory allocation failed\n");
		free(search_bytes);
		free(replace_bytes_arr);
		return EXIT_FAILURE;
	}

	size_t search_len = 0;
	size_t replace_len = 0;

	if (hex_to_bytes(search_hex, search_bytes, &search_len) != 0)
	{
		fprintf(stderr, "Invalid search hex string: %s\n");
		free(search_bytes);
		free(replace_bytes_arr);
		return EXIT_FAILURE;
	}

	if (hex_to_bytes(replace_hex, replace_bytes_arr, &replace_len) != 0)
	{
		fprintf(stderr, "Invalid replace hex string: %s\n");
		free(search_bytes);
		free(replace_bytes_arr);
		return EXIT_FAILURE;
	}

	FILE *in = fopen(input_path, "rb");
	if (in == NULL)
	{
		fprintf(stderr, "Error: Cannot open input file %s\n", input_path);
		free(search_bytes);
		free(replace_bytes_arr);
		return EXIT_FAILURE;
	}

	FILE *out = fopen(output_path, "wb");
	if (out == NULL)
	{
		fprintf(stderr, "Error: Cannot open output file %s\n", output_path);
		fclose(in);
		free(search_bytes);
		free(replace_bytes_arr);
		return EXIT_FAILURE;
	}

	int result = replace_bytes(in, out, search_bytes, search_len, replace_bytes_arr, replace_len);
	fclose(in);
	fclose(out);
	free(search_bytes);
	free(replace_bytes_arr);

	if (result != 0)
	{
		fprintf(stderr, "Error: Failed to replace bytes in the file\n");
		return EXIT_FAILURE;
	}
	printf("Successfully replaced bytes in the file.\n");
	return EXIT_SUCCESS;
}
