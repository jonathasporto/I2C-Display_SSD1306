#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "ledStruct.h"
#include "ws2818b.pio.h"

#define MATRIZ 7
#define NUM_PIXELS 25

npLED_t leds[NUM_PIXELS]; // Declaração do buffer de pixels que formam a matriz.
PIO np_pio;               // Variáveis para uso da máquina PIO.
uint sm;

// Variavel que armazena simbolo dos números
const npLED_t numberRGB[10][NUM_PIXELS] = {
    {{255, 40, 0}, {255, 40, 0}, {255, 40, 0}, {255, 40, 0}, {255, 40, 0}, {255, 40, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 40, 0}, {255, 40, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 40, 0}, {255, 40, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 40, 0}, {255, 40, 0}, {255, 40, 0}, {255, 40, 0}, {255, 40, 0}, {255, 40, 0}},
    {{0, 0, 0}, {255, 40, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 40, 0}, {0, 0, 0}, {0, 0, 0}, {255, 40, 0}, {0, 0, 0}, {255, 40, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 40, 0}, {255, 40, 0}, {0, 0, 0}, {0, 0, 0}, {255, 40, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
    {{255, 40, 0}, {255, 40, 0}, {255, 40, 0}, {255, 40, 0}, {255, 40, 0}, {0, 0, 0}, {255, 40, 0}, {255, 40, 0}, {255, 40, 0}, {255, 40, 0}, {255, 40, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 40, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 40, 0}, {0, 0, 0}, {255, 40, 0}, {255, 40, 0}, {255, 40, 0}, {0, 0, 0}},
    {{255, 40, 0}, {255, 40, 0}, {255, 40, 0}, {255, 40, 0}, {255, 40, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 40, 0}, {255, 40, 0}, {255, 40, 0}, {255, 40, 0}, {255, 40, 0}, {255, 40, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 40, 0}, {255, 40, 0}, {255, 40, 0}, {255, 40, 0}, {255, 40, 0}, {255, 40, 0}},
    {{255, 40, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 40, 0}, {255, 40, 0}, {255, 40, 0}, {255, 40, 0}, {255, 40, 0}, {255, 40, 0}, {255, 40, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 40, 0}, {255, 40, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 40, 0}},
    {{255, 40, 0}, {255, 40, 0}, {255, 40, 0}, {255, 40, 0}, {255, 40, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 40, 0}, {255, 40, 0}, {255, 40, 0}, {255, 40, 0}, {255, 40, 0}, {255, 40, 0}, {255, 40, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 40, 0}, {255, 40, 0}, {255, 40, 0}, {255, 40, 0}, {255, 40, 0}},
    {{255, 40, 0}, {255, 40, 0}, {255, 40, 0}, {255, 40, 0}, {0, 0, 0}, {255, 40, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 40, 0}, {0, 0, 0}, {255, 40, 0}, {255, 40, 0}, {255, 40, 0}, {255, 40, 0}, {0, 0, 0}, {255, 40, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 40, 0}, {255, 40, 0}, {0, 0, 0}, {0, 0, 0}},
    {{255, 40, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 40, 0}, {255, 40, 0}, {255, 40, 0}, {255, 40, 0}, {255, 40, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 40, 0}, {255, 40, 0}, {255, 40, 0}, {255, 40, 0}, {255, 40, 0}, {255, 40, 0}},
    {{255, 40, 0}, {255, 40, 0}, {255, 40, 0}, {255, 40, 0}, {255, 40, 0}, {255, 40, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 40, 0}, {255, 40, 0}, {255, 40, 0}, {255, 40, 0}, {255, 40, 0}, {255, 40, 0}, {255, 40, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 40, 0}, {255, 40, 0}, {255, 40, 0}, {255, 40, 0}, {255, 40, 0}, {255, 40, 0}},
    {{255, 40, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 40, 0}, {255, 40, 0}, {255, 40, 0}, {255, 40, 0}, {255, 40, 0}, {255, 40, 0}, {255, 40, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 40, 0}, {255, 40, 0}, {255, 40, 0}, {255, 40, 0}, {255, 40, 0}, {255, 40, 0}}};

/**
 * Inicializa a máquina PIO para controle da matriz de LEDs.
 */
void initMatriz()
{
  // Cria programa PIO.
  uint offset = pio_add_program(pio0, &ws2818b_program);
  np_pio = pio0;

  // Toma posse de uma máquina PIO.
  sm = pio_claim_unused_sm(np_pio, false);
  if (sm < 0)
  {
    np_pio = pio1;
    sm = pio_claim_unused_sm(np_pio, true); // Se nenhuma máquina estiver livre, panic!
  }

  // Inicia programa na máquina PIO obtida.
  ws2818b_program_init(np_pio, sm, offset, MATRIZ, 800000.f);

  // Limpa buffer de pixels.
  for (uint i = 0; i < NUM_PIXELS; ++i)
  {
    leds[i].R = 0;
    leds[i].G = 0;
    leds[i].B = 0;
  }
}

/**
 * Atribui uma cor RGB a um LED.
 */
void buffer_SetLED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b)
{
  leds[index].R = r;
  leds[index].G = g;
  leds[index].B = b;
}

/**
 * Limpa o buffer de pixels.
 */
void buffer_Clear()
{
  for (uint i = 0; i < NUM_PIXELS; ++i)
    buffer_SetLED(i, 0, 0, 0);
}

/**
 * Atualiza os LEDS com as informções do buffer.
 */
void buffer_WriteLEDs()
{
  // Escreve cada dado de 8-bits dos pixels em sequência no buffer da máquina PIO.
  for (uint i = 0; i < NUM_PIXELS; ++i)
  {
    pio_sm_put_blocking(np_pio, sm, leds[i].G);
    pio_sm_put_blocking(np_pio, sm, leds[i].R);
    pio_sm_put_blocking(np_pio, sm, leds[i].B);
  }
  sleep_us(100); // Espera 100us, sinal de RESET do datasheet.
}

/**
 * Troca o valor exibido na matriz de LEDS por um novo valor.
 */
void Matriz_changeNumber(const npLED_t newDraw[])
{

  // Coloca os novos valores fornecidos no buffer
  for (size_t i = 0; i < NUM_PIXELS; i++)
  {
    buffer_SetLED(i, newDraw[i].R, newDraw[i].G, newDraw[i].B);
  }

  // Escreve nos LEDs
  buffer_WriteLEDs();
}