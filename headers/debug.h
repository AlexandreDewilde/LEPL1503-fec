#ifndef _DEBUG_H
#define _DEBUG_H

void DEBUG(const char *restrict format, ...);
void DEBUG_VECTOR(uint8_t *vector, uint32_t vector_size);
void ACTIVATE_DEBUG();
#endif /* _DEBUG_H */