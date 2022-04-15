#ifndef _DEBUG_H
#define _DEBUG_H

#include <errno.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h> 
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

void DEBUG(const char *restrict format, ...);
void DEBUG_VECTOR(uint8_t *vector, uint32_t vector_size);
void ACTIVATE_DEBUG();
void deal_error_reading_file(FILE *file);
#endif /* _DEBUG_H */