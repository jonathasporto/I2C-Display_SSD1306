#ifndef MATRIZLED_H
#define MATRIZLED_H

#include <stdint.h> // to uint work
#include "ledStruct.h"

extern npLED_t numberRGB[10][25]; // Valores de entrada para Matriz_Changenumber
void initMatriz(); // Init the LED matriz pin
void buffer_Clear(); // Clear the matriz buffer
void Matriz_changeNumber(const npLED_t newDraw[]); // Change the number in the LED

#endif