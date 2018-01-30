#ifndef _ESP_MOTORS_H
#define _ESP_MOTORS_H

#include <c_types.h>

#define ESP_MOTORS_PERIOD 5120

#define ESP_MOTOR_A_SPEED    4  // D2
#define ESP_MOTOR_A_DIR      2  // D4
#define ESP_MOTOR_B_SPEED    5  // D1
#define ESP_MOTOR_B_DIR      0  // D3

typedef struct {
    uint8_t func;
    uint32_t mux;
    uint8_t pin;
} gpio_t;

typedef struct {
    gpio_t speed;
    gpio_t direction;
} motor;

void esp_motors_set_single(uint8_t motor_id, int16_t motor_speed);
void esp_motors_set(int16_t motor_a, int16_t motor_b);
void esp_motors_save(int16_t motor_a, int16_t motor_b);
void esp_motors_init();

#endif
