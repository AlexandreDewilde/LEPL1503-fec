#ifndef MESSAGE_H
#define MESSAGE_H

#include <stdint.h>
typedef struct
{
    uint64_t length;
    uint8_t *message;
} message_t;

#endif /* MESSAGE_H */