/*
lib_main.c - главный модуль библиотеки. 

Бабурин Дмитрий Сергеевич
МК-101
*/

#include "lib_main.h"
#include <string.h>
#include <ctype.h>

static int hex_char_to_val(char c) {
	if (c >= '0' && c <= '9') return c - '0';
	if (c >= 'a' && c <= 'f') return c - 'a' + 10;
	if (c >= 'A' && c <= 'F') return c - 'A' + 10;
	return -1;
}

int hex_to_bytes(const char *hex, uint8_t *out_bytes, size_t *out_len) {
	if (hex == NULL || out_bytes == NULL || out_len == NULL) {
		return -1;
	}

	size_t len = strlen(hex);
	if (len % 2 != 0) {
		return -1; 
	}

	size_t byte_idx = 0;
	for (size_t i = 0; i < len; i += 2) {
		int high = hex_char_to_val(hex[i]);
		int low = hex_char_to_val(hex[i + 1]);

		if (high == -1 || low == -1) {
			return -1; 
		}
		out_bytes[byte_idx++] = (uint8_t)((high << 4) | low);
	}

	*out_len = byte_idx;
	return 0;
}