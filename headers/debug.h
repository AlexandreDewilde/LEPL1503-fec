#ifndef _DEBUG_H
#define _DEBUG_H

#include <errno.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h> 
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

void verbose_matrix(uint8_t **A, uint32_t size, uint32_t b_size);
void verbose_linear_system(uint8_t **A, uint8_t **B, uint32_t size, uint32_t b_size);
void DEBUG(const char *restrict format, ...);
void DEBUG_VECTOR(uint8_t *vector, uint32_t vector_size);
void DEBUG_VECTOR_BOOLEAN(bool *vector, uint32_t vector_size);
void ACTIVATE_DEBUG();
void deal_error_reading_file(FILE *file);
#endif /* _DEBUG_H */