#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "ledStruct.h"
#include "matriz/matrizLed.h"
#include "I2C/display.h"


#define UART_ID uart0    // Seleciona a UART0
#define BAUD_RATE 115200 // Define a taxa de transmissão
#define UART_TX_PIN 0    // Pino GPIO usado para TX
#define UART_RX_PIN 1    // Pino GPIO usado para RX

#define LED_RED 13
#define LED_GREEN 11
#define LED_BLUE 12

#define BUTTON_A 5
#define BUTTON_B 6

#define DEBOUNCE_DELAY 200 // Atraso de 200ms para debouncing

static volatile uint32_t last_interrupt_time = 0; // Variavel que salva ultima interrupção
uint8_t numberLED;                                // Variavel de estado do número

// Variaveis de estado do botão
bool isButtonA = false;
bool isButtonB = false;

/**
 * Função que realiza função do BOTÃO A
 */
void dobuttonA()
{
  isButtonA = false;                     // Atualiza estado do botão
  bool estado_led = gpio_get(LED_GREEN); // captura estado do LED
  gpio_put(LED_GREEN, !estado_led);      // Atualiza LED

  // Descritivo print na interface Serial
  char *estado_str = estado_led ? "false" : "true";
  printf("LED VERDE ALTERADO: %s\n", estado_str);

  // Mensagem no display SSD1306
  setDisplay(estado_led ? " " : "G", 64, 48);
}

/**
 * Função que realiza função do BOTÃO B
 */
void dobuttonB()
{
  isButtonB = false;                       // Atualiza estado do botão
  bool estado_led = gpio_get(LED_BLUE);    // captura estado do LED
  gpio_put(LED_BLUE, !gpio_get(LED_BLUE)); // Atualiza LED

  // Descritivo print na interface Serial
  char *estado_str = estado_led ? "false" : "true";
  printf("LED AZUL ALTERADO: %s\n", estado_str);

  // Mensagem no display SSD1306
  setDisplay(estado_led ? " " : "B", 80, 48);
}

/**
 * Função que organiza os callbacks do sistema
 */
void gpio_callback(uint gpio, uint32_t events)
{
  uint32_t current_time = to_ms_since_boot(get_absolute_time()); // CONTROLE DE DEBOUNCING

  // Verifica se o tempo de debouncing passou
  if (current_time - last_interrupt_time > DEBOUNCE_DELAY)
  {
    if (gpio == BUTTON_A)
    {
      isButtonA = true;
    }
    if (gpio == BUTTON_B)
    {
      isButtonB = true;
    }
    // Atualiza o tempo da última interrupção
    last_interrupt_time = current_time;
  }
}

/**
 * PROCESSO MAIN UTILIZANDO USB
 */
void process_USB()
{
  if (stdio_usb_connected()) // Certifica-se de que o USB está conectado stdio_usb_connected()
  {
    char c;                   // Lê um caractere da USB
    if (scanf("%c", &c) == 1) // Lê caractere da entrada padrão
    {
      // 1) Serial Monitor UART recebe 1 char de cada vez
      printf("Character digitado: %c \n\r", c); // Envia uma mensagem adicional para cada caractere recebido

      // 2) Caso número digitado, sera exibido na matrizLed
      if (isdigit(c))
      {
        Matriz_changeNumber(numberRGB[c - '0']);
      }

      // 3) Exibe os caracters no display SSD1306.
      setDisplay_Char(c);
    }
  }
}

/**
 * PROCESSO MAIN UTILIZANDO UART
 */
void process_UART()
{
  // Recebe Input serial e processa.
  if (uart_is_readable(UART_ID))
  {
    // 1) Serial Monitor UART recebe 1 char de cada vez
    char c = uart_getc(UART_ID);                // Lê um caractere da UART
    uart_puts(UART_ID, "Character digitado: "); // Envia uma mensagem adicional para cada caractere recebido
    uart_putc(UART_ID, c);                      // Envia de volta o caractere lido (eco)
    uart_puts(UART_ID, "\n\r");                 // Pula para inicio da proxima linha

    // 2) Caso número digitado, sera exibido na matrizLed
    if (isdigit(c))
    {
      Matriz_changeNumber(numberRGB[c - '0']);
    }

    // 3) Exibe os caracters no display SSD1306.
    setDisplay_Char(c);
  }
}

int main()
{
  stdio_init_all();

  // Inicia os LEDS
  gpio_init(LED_RED);
  gpio_set_dir(LED_RED, GPIO_OUT);
  gpio_init(LED_GREEN);
  gpio_set_dir(LED_GREEN, GPIO_OUT);
  gpio_init(LED_BLUE);
  gpio_set_dir(LED_BLUE, GPIO_OUT);

  // Inicia os Botões
  gpio_init(BUTTON_A);
  gpio_set_dir(BUTTON_A, GPIO_IN);
  gpio_pull_up(BUTTON_A);
  gpio_init(BUTTON_B);
  gpio_set_dir(BUTTON_B, GPIO_IN);
  gpio_pull_up(BUTTON_B);

  // Inicia Matriz de LEDS com PIO
  initMatriz();

  // Inicializa a UART
  uart_init(UART_ID, BAUD_RATE);

  // Configura os pinos GPIO para a UART
  gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART); // Configura o pino 0 para TX
  gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART); // Configura o pino 1 para R

  // Configuração da interrupção com callback
  gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
  gpio_set_irq_enabled(BUTTON_B, GPIO_IRQ_EDGE_FALL, true);

  // Setando o Numero no LED
  numberLED = 0;

  // Incializa o display I2C
  display_init();

  // Programa Inciado!
  printf("Programa Inciado!\n");

  // Loop Principal do programa
  while (true)
  {
    if (isButtonA) // Detecta mudança de botão A e processa
    {
      dobuttonA();
    }
    else if (isButtonB) // Detecta mudança de botão B e processa
    {
      dobuttonB();
    }
    else // Recebe Input serial e processa.
    {
      process_USB();
    }

    // Reduz processamento
    sleep_ms(10);
  }
}
