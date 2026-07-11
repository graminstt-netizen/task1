/*
lib_main.c - главный модуль библиотеки.

Бабурин Дмитрий Сергеевич
МК-101
*/

#include "lib_main.h"
#include <string.h>
#include <ctype.h>

#ifndef N
#define N 1024 // по умолчанию (если не задано изначально)
#endif

static int hex_char_to_val(char c)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	if (c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	if (c >= 'A' && c <= 'F')
		return c - 'A' + 10;
	return -1;
}

int hex_to_bytes(const char *hex, uint8_t *out_bytes, size_t *out_len)
{
	if (hex == NULL || out_bytes == NULL || out_len == NULL)
	{
		return -1;
	}

	size_t len = strlen(hex);
	if (len % 2 != 0)
	{
		return -1;
	}

	size_t byte_idx = 0;
	for (size_t i = 0; i < len; i += 2)
	{
		int high = hex_char_to_val(hex[i]);
		int low = hex_char_to_val(hex[i + 1]);

		if (high == -1 || low == -1)
		{
			return -1;
		}
		out_bytes[byte_idx++] = (uint8_t)((high << 4) | low);
	}

	*out_len = byte_idx;
	return 0;
}

int replace_bytes(FILE *in, FILE *out, const uint8_t *search, size_t search_len, const uint8_t *replace, size_t replace_len)
{
	if (in == NULL || out == NULL || search == NULL || replace == NULL || search_len == 0)
	{
		return -1; // Ошибка: недопустимые аргументы
	}

	uint8_t buffer[N];
	size_t match_len = 0; // Длина текущего совпадения

	while (1)
	{
		size_t bytes_read = fread(buffer, 1, N, in);
		if (bytes_read == 0)
		{
			if (ferror(in))
			{
				return -1;
			}
			break;
		}
		for (size_t i = 0; i < bytes_read; ++i)
		{
			uint8_t b = buffer[i];

			if (b == search[match_len])
			{
				match_len++;
				if (match_len == search_len)
				{
					if (fwrite(replace, 1, replace_len, out) != replace_len)
					{
						return -1; // Ошибка записи
					}
					match_len = 0; // Сбросить длину совпадения
				}
			}
			else
			{
				size_t k = 0;
				for (size_t temp_k = match_len; temp_k > 0; --temp_k)
				{
					if (b == search[temp_k - 1])
					{
						int match = 1;
						for (size_t j = 0; j < temp_k - 1; ++j)
						{
							if (search[j] != search[match_len - temp_k + 1 + j])
							{
								match = 0;
								break;
							}
						}
						if (match)
						{
							k = temp_k;
							break;
						}
					}
				}

				size_t write_len = match_len + 1 - k;
				if (write_len <= match_len)
				{
					if (fwrite(search, 1, write_len, out) != write_len)
					{
						return -1; // Ошибка записи
					}
				}
				else
				{
					if (match_len > 0)
					{
						if (fwrite(search, 1, match_len, out) != match_len)
						{
							return -1; // Ошибка записи
						}
					}
					if (fwrite(&b, 1, 1, out) != 1)
					{
						return -1; // Ошибка записи
					}
				}
				match_len = k;
			}
		}
	}
	if (match_len > 0)
	{
		if (fwrite(search, 1, match_len, out) != match_len)
		{
			return -1; // Ошибка записи
		}
	}
	return 0; // Успешное завершение
}	

