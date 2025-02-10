#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"

#define UART_ID uart0    // Seleciona a UART0
#define BAUD_RATE 115200 // Define a taxa de transmissão
#define UART_TX_PIN 0    // Pino GPIO usado para TX
#define UART_RX_PIN 1    // Pino GPIO usado para RX

/**
 * Função que inicializa comunicação UART
 */
void uartsInit()
{
  // Inicializa a UART
  uart_init(UART_ID, BAUD_RATE);

  // Configura os pinos GPIO para a UART
  gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART); // Configura o pino 0 para TX
  gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART); // Configura o pino 1 para RX
}

/**
 * Função que realiza Entrada de caracteres via PC e exibição
 * 1) Serial Monitor recebe 1 char de cada vez
 * 2) Exibe os caracters no display SSD1306.
 * 3) Caso número digitado, sera exibido na matrizLed
 */
void uartsCharMonitor()
{
  char c = "";

  // Verifica se há dados disponíveis para leitura
  if (uart_is_readable(UART_ID))
  {
    // Lê um caractere da UART
    c = uart_getc(UART_ID);

    // Envia de volta o caractere lido (eco)
    uart_putc(UART_ID, c);

    // Envia uma mensagem adicional para cada caractere recebido
    uart_puts(UART_ID, " <- Eco do RP2\r\n");
  }
}
