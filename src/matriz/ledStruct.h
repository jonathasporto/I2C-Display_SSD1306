#ifndef LEDSTRUCT_H
#define LEDSTRUCT_H

#include <stdint.h>

// Definição de pixel GRB
struct pixel_t
{
  uint8_t R, G, B; // Três valores de 8-bits compõem um pixel.
};
typedef struct pixel_t pixel_t;
typedef pixel_t npLED_t; // Mudança de nome de "struct pixel_t" para "npLED_t" por clareza.

#endif