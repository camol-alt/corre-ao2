#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "pico/bootrom.h"
#include "hardware/i2c.h"
#include "hardware/clocks.h"
#include "hardware/pio.h"
#include "inc/ssd1306.h"
#include "inc/font.h"
#include "ws2812.pio.h"

#define IS_RGBW false
#define NUM_PIXELS 25
#define WS2812_PIN 7
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C
#define WIDTH 128
#define HEIGHT 64

#define led_pin_g 11
#define led_pin_b 12
#define led_pin_r 13
#define botaoA 5
#define botaoB 6

uint8_t led_r = 20; // Intensidade do vermelho
uint8_t led_g = 0;  // Intensidade do verde
uint8_t led_b = 0;  // Intensidade do azul

bool led_buffer0[NUM_PIXELS] = {
    0.0, 1.0, 1.0, 1.0, 0.0,
    1.0, 0.0, 0.0, 0.0, 1.0,
    1.0, 0.0, 0.0, 0.0, 1.0,
    1.0, 0.0, 0.0, 0.0, 1.0,
    0.0, 1.0, 1.0, 1.0, 0.0,
};

bool led_buffer1[NUM_PIXELS] = {
    1.0, 1.0, 1.0, 1.0, 1.0,
    0.0, 0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0, 1.0,
    0.0, 1.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0, 0.0,
};

bool led_buffer2[NUM_PIXELS] ={
    1.0, 1.0, 1.0, 1.0, 1.0,
    1.0, 0.0, 0.0, 0.0, 0.0,
    1.0, 1.0, 1.0, 1.0, 1.0,
    0.0, 0.0, 0.0, 0.0, 1.0,
    1.0, 1.0, 1.0, 1.0, 1.0,
};

bool led_buffer3[NUM_PIXELS] ={
    1.0, 1.0, 1.0, 1.0, 1.0,
    0.0, 0.0, 0.0, 0.0, 1.0,
    1.0, 1.0, 1.0, 1.0, 1.0,
    0.0, 0.0, 0.0, 0.0, 1.0,
    1.0, 1.0, 1.0, 1.0, 1.0,
};

bool led_buffer4[NUM_PIXELS] ={
    1.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 1.0,
    1.0, 1.0, 1.0, 1.0, 1.0,
    1.0, 0.0, 0.0, 0.0, 1.0,
    1.0, 0.0, 0.0, 0.0, 1.0,
};

bool led_buffer5[NUM_PIXELS] ={
    1.0, 1.0, 1.0, 1.0, 1.0,
    0.0, 0.0, 0.0, 0.0, 1.0,
    1.0, 1.0, 1.0, 1.0, 1.0,
    1.0, 0.0, 0.0, 0.0, 0.0,
    1.0, 1.0, 1.0, 1.0, 1.0,
};

bool led_buffer6[NUM_PIXELS] ={
    1.0, 1.0, 1.0, 1.0, 1.0,
    1.0, 0.0, 0.0, 0.0, 1.0,
    1.0, 1.0, 1.0, 1.0, 1.0,
    1.0, 0.0, 0.0, 0.0, 0.0,
    1.0, 1.0, 1.0, 1.0, 1.0,
};

bool led_buffer7[NUM_PIXELS] ={
    1.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 1.0,
    1.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 1.0,
    1.0, 1.0, 1.0, 1.0, 1.0,
};

bool led_buffer8[NUM_PIXELS] ={
    1.0, 1.1, 1.1, 1.1, 1.1,
    1.0, 0.0, 0.0, 0.0, 1.0,
    1.0, 1.1, 1.1, 1.1, 1.1,
    1.0, 0.0, 0.0, 0.0, 1.0,
    1.0, 1.1, 1.1, 1.1, 1.1, 
};

bool led_buffer9[NUM_PIXELS] ={
    1.0, 1.1, 1.1, 1.1, 1.1,
    0.0, 0.0, 0.0, 0.0, 1.0,
    1.0, 1.1, 1.1, 1.1, 1.1,
    1.0, 0.0, 0.0, 0.0, 1.0,
    1.0, 1.1, 1.1, 1.1, 1.1,
};

static inline void put_pixel(uint32_t pixel_grb) {
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
    return ((uint32_t)(r) << 8) | ((uint32_t)(g) << 16) | (uint32_t)(b);
}

void set_one_led(bool led_buffer[], uint8_t r, uint8_t g, uint8_t b) {
    uint32_t color = urgb_u32(r, g, b);
    for (int i = 0; i < NUM_PIXELS; i++) {
        if (led_buffer[i]) {
            put_pixel(color); // Liga o LED com um no buffer
        } else {
            put_pixel(0); // Desliga os LEDs com zero no buffer
        }
    }
}

void show_number(int number) {
    switch (number) {
        case 0: set_one_led(led_buffer0, led_r, led_g, led_b); break;
        case 1: set_one_led(led_buffer1, led_r, led_g, led_b); break;
        case 2: set_one_led(led_buffer2, led_r, led_g, led_b); break;
        case 3: set_one_led(led_buffer3, led_r, led_g, led_b); break;
        case 4: set_one_led(led_buffer4, led_r, led_g, led_b); break;
        case 5: set_one_led(led_buffer5, led_r, led_g, led_b); break;
        case 6: set_one_led(led_buffer6, led_r, led_g, led_b); break;
        case 7: set_one_led(led_buffer7, led_r, led_g, led_b); break;
        case 8: set_one_led(led_buffer8, led_r, led_g, led_b); break;
        case 9: set_one_led(led_buffer9, led_r, led_g, led_b); break;
        default: break;
    }
}

ssd1306_t ssd;

// Função para inicializar o display
void init_display() {
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT);
    ssd1306_config(&ssd);
    ssd1306_send_data(&ssd);
}

// Função para exibir mensagem no display
void display_message(const char *message) {
    ssd1306_fill(&ssd, false);
    ssd1306_draw_string(&ssd, message, 10, 10);
    ssd1306_send_data(&ssd);
}

// Função de interrupção do botão
void gpio_irq_handler(uint gpio, uint32_t __event) {
    if (gpio == botaoA) {
        bool led_state = !gpio_get(led_pin_g);
        gpio_put(led_pin_g, led_state);

        char message[50];
        snprintf(message, sizeof(message), "LED verde %s!", led_state ? "ligado" : "desligado");
        printf("%s\n", message);
        display_message(message);
    } else if (gpio == botaoB) {
        bool led_state = !gpio_get(led_pin_b);
        gpio_put(led_pin_b, led_state);

        char message[50];
        snprintf(message, sizeof(message), "LED azul %s!", led_state ? "ligado" : "desligado");
        printf("%s\n", message);
        display_message(message);
    } else {
        reset_usb_boot(0, 0);
    }
}

int main() {
    stdio_init_all();
    init_display();

    gpio_init(botaoA);
    gpio_set_dir(botaoA, GPIO_IN);
    gpio_pull_up(botaoA);
    gpio_set_irq_enabled_with_callback(botaoA, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    gpio_init(botaoB);
    gpio_set_dir(botaoB, GPIO_IN);
    gpio_pull_up(botaoB);
    gpio_set_irq_enabled_with_callback(botaoB, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    gpio_init(led_pin_r);
    gpio_set_dir(led_pin_r, GPIO_OUT);
    gpio_put(led_pin_r, 0);

    gpio_init(led_pin_g);
    gpio_set_dir(led_pin_g, GPIO_OUT);
    gpio_put(led_pin_g, 0);

    gpio_init(led_pin_b);
    gpio_set_dir(led_pin_b, GPIO_OUT);
    gpio_put(led_pin_b, 0);

    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW);

    printf("RP2040 inicializado. Envie 'r', 'g' ou 'b' para alternar os LEDs, ou um dígito ('0' a '9') para mostrar o número.\n");

    while (true) {
 
while (true) {
    if (stdio_usb_connected()) {
        char c;
        if (scanf("%c", &c) == 1) {
            printf("Recebido: '%c'\n", c);

            // Envia o caractere para o display
            char message[2];
            message[0] = c;
            message[1] = '\0';
            display_message(message);

            if (c >= '0' && c <= '9') {
                int number = c - '0';
                show_number(number);
            } else {
                switch (c) {
                    case 'r':
                        gpio_put(led_pin_r, !gpio_get(led_pin_r));
                        printf("LED vermelho alternado!\n");
                        break;
                    case 'g':
                        gpio_put(led_pin_g, !gpio_get(led_pin_g));
                        printf("LED verde alternado!\n");
                        break;
                    case 'b':
                        gpio_put(led_pin_b, !gpio_get(led_pin_b));
                        printf("LED azul alternado!\n");
                        break;
                    default:
                        printf("Comando inválido: '%c'\n", c);
                }
            }
        }
    }
    sleep_ms(40);
}
 }
}





