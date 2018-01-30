#include <stdio.h>
#include <stdlib.h>
//#include <osapi.h>
//#include <c_types.h>

#define PWM2 1

#ifdef PWM2
#include <pwm.h>
#else
#include <new_pwm.h>
#endif

#include <esp8266.h>

#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>

#include "esp_motors.h"

static int16_t esp_motors_status[] = {0, 0};
static uint32_t *esp_motors_status_p = (uint32_t*)esp_motors_status;

#ifdef PWM2
static uint8_t pwm_pins[] = {
    ESP_MOTOR_A_SPEED,
    ESP_MOTOR_B_SPEED
};
#else
static uint32_t pwm_pins[2][3] = {
    {0, 0, ESP_MOTOR_A_SPEED},
    {0, 0, ESP_MOTOR_B_SPEED}
};
uint32_t pwm_duty_init[2] = {0};
#endif

//static SemaphoreHandle_t xMutex;
static TaskHandle_t xTaskToNotify = NULL;

void esp_motors_set_single(uint8_t motor_id, uint8_t motor_speed_pin, int16_t motor_speed, uint8_t motor_dir_pin) {
    //motor_speed = motor_speed < 0 ? -(256 + motor_speed) : motor_speed;
#ifdef PWM2
    uint32_t motor_speed_new = abs(motor_speed) * (PWM_MAX_PERIOD/ 256);
    pwm_set_duty(motor_id, motor_speed_new);
#else
    uint32_t motor_speed_new = abs(motor_speed) * (ESP_MOTORS_PERIOD / 256);
    pwm_set_duty(motor_speed_new, motor_id);
#endif

    gpio_write(motor_dir_pin, motor_speed < 0 ? 1 : 0);
}

void esp_motors_set(int16_t motor_a, int16_t motor_b) {
#ifdef PWM2
    pwm_stop();
#else
#endif
    esp_motors_set_single(0, ESP_MOTOR_A_SPEED, motor_a, ESP_MOTOR_A_DIR);
    esp_motors_set_single(1, ESP_MOTOR_B_SPEED, motor_b, ESP_MOTOR_B_DIR);
#ifdef PWM2
    pwm_start();
#else
    pwm_start();
#endif
}

static void esp_motors_task(void *pvParameters) {
    /*
    printf("Motor A (Speed) GPIO: %u\n", ESP_MOTOR_A_SPEED);
    printf("Motor A (Dir) GPIO: %u\n", ESP_MOTOR_A_DIR);
    printf("Motor B (Speed) GPIO: %u\n", ESP_MOTOR_B_SPEED);
    printf("Motor B (Dir) GPIO: %u\n", ESP_MOTOR_B_DIR);
    */

#ifdef PWM2
    pwm_init(3, pwm_pins, false);
    pwm_set_freq(2000);
#else
    pwm_init(ESP_MOTORS_PERIOD, pwm_duty_init, 2, pwm_pins);
#endif

    esp_motors_set(0, 0);
    pwm_start();

    printf("Motors inited\n");

    const TickType_t xMaxBlockTime = pdMS_TO_TICKS(500);
    while (1) {
        uint32_t ulNotificationValue = ulTaskNotifyTake(pdTRUE, xMaxBlockTime);
        //uint32_t ulNotificationValue = xTaskNotifyWait(0, 0, esp_motors_status_p, xMaxBlockTime);

        if (ulNotificationValue != 0) {
            printf("Motor: L:%d R:%d\n", esp_motors_status[0], esp_motors_status[1]);
            //pwm_stop();
            //vTaskDelay(30);
            //taskENTER_CRITICAL();
            //xSemaphoreTake(xMutex, portMAX_DELAY);
            esp_motors_set(esp_motors_status[0], esp_motors_status[1]);
            //xSemaphoreGive(xMutex);
            //taskEXIT_CRITICAL();
            //pwm_start();
        }
        vTaskDelay(1);
    };
}

void esp_motors_save(int16_t motor_a, int16_t motor_b) {
    //xSemaphoreTake(xMutex, portMAX_DELAY);
    //esp_motors_status[0] = motor_a;
    //esp_motors_status[1] = motor_b;
    *esp_motors_status_p = ((uint16_t)motor_b << 16) | (uint16_t)motor_a;
    //uint32_t task_value = (motor_b << 16) | motor_a;
    //xSemaphoreGive(xMutex);

    xTaskNotifyGive(xTaskToNotify);
    //xTaskNotify(xTaskToNotify, task_value, eSetValueWithoutOverwrite);
}

void esp_motors_init() {
    //xMutex = xSemaphoreCreateMutex();
    gpio_enable(ESP_MOTOR_A_DIR, GPIO_OUTPUT);
    gpio_enable(ESP_MOTOR_B_DIR, GPIO_OUTPUT);
    xTaskCreate(&esp_motors_task, "esp_motors", 256, NULL, 1, &xTaskToNotify);
}

