#include <stdio.h>
#include <stdlib.h>
//#include <osapi.h>
//#include <c_types.h>
#include <new_pwm.h>

#include <esp8266.h>

#include "esp_motors.h"

// https://github.com/StefanBruens/ESP8266_new_pwm

static uint32_t pwm_pins[2][3] = {
    {0, 0, ESP_MOTOR_A_SPEED},
    {0, 0, ESP_MOTOR_B_SPEED},
};

uint32_t pwm_duty_init[2] = {0};

void esp_motors_set_single(uint8_t motor_id, uint8_t motor_speed_pin, int16_t motor_speed, uint8_t motor_dir_pin) {
    uint32_t motor_speed_new = motor_speed * (ESP_MOTORS_PERIOD / 256);
    pwm_set_duty(abs(motor_speed_new), motor_id);

    gpio_write(motor_dir_pin, motor_speed < 0 ? 1 : 0);
}

void esp_motors_set(int16_t motor_a, int16_t motor_b) {
    esp_motors_set_single(0, ESP_MOTOR_A_SPEED, motor_a, ESP_MOTOR_A_DIR);
    esp_motors_set_single(1, ESP_MOTOR_B_SPEED, motor_b, ESP_MOTOR_B_DIR);
    pwm_start();
}

void esp_motors_init() {
    printf("Motor A (Speed) GPIO: %u\n", ESP_MOTOR_A_SPEED);
    printf("Motor A (Dir) GPIO: %u\n", ESP_MOTOR_A_DIR);
    printf("Motor B (Speed) GPIO: %u\n", ESP_MOTOR_B_SPEED);
    printf("Motor B (Dir) GPIO: %u\n", ESP_MOTOR_B_DIR);

    gpio_enable(ESP_MOTOR_A_DIR, GPIO_OUTPUT);
    gpio_enable(ESP_MOTOR_B_DIR, GPIO_OUTPUT);

    pwm_init(ESP_MOTORS_PERIOD, pwm_duty_init, 2, pwm_pins);

    esp_motors_set(0, 0);

    printf("Motors inited\n");
}
