#include <stdio.h>
#include <stdlib.h>
//#include <osapi.h>
//#include <c_types.h>
#include <pwm.h>

#include <esp8266.h>

#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>

#include "esp_motors.h"

static int16_t esp_motors_status[] = {0, 0};

static uint8_t pwm_pins[] = {
    ESP_MOTOR_A_SPEED,
    ESP_MOTOR_B_SPEED
};

//static SemaphoreHandle_t xMutex;
static TaskHandle_t xTaskToNotify = NULL;

void esp_motors_set_single(uint8_t motor_id, uint8_t motor_speed_pin, int16_t motor_speed, uint8_t motor_dir_pin) {
    uint32_t motor_speed_new = motor_speed * (ESP_MOTORS_PERIOD / 256);
    pwm_set_duty(motor_id, abs(motor_speed_new));

    gpio_write(motor_dir_pin, motor_speed < 0 ? 1 : 0);
}

void esp_motors_set(int16_t motor_a, int16_t motor_b) {
    esp_motors_set_single(0, ESP_MOTOR_A_SPEED, motor_a, ESP_MOTOR_A_DIR);
    esp_motors_set_single(1, ESP_MOTOR_B_SPEED, motor_b, ESP_MOTOR_B_DIR);
    pwm_start();
}

static void esp_motors_task(void *pvParameters) {
    printf("Motor A (Speed) GPIO: %u\n", ESP_MOTOR_A_SPEED);
    printf("Motor A (Dir) GPIO: %u\n", ESP_MOTOR_A_DIR);
    printf("Motor B (Speed) GPIO: %u\n", ESP_MOTOR_B_SPEED);
    printf("Motor B (Dir) GPIO: %u\n", ESP_MOTOR_B_DIR);

    pwm_init(3, pwm_pins, false);

    esp_motors_set(0, 0);

    printf("Motors inited\n");

    const TickType_t xMaxBlockTime = pdMS_TO_TICKS(500);
    while (1) {
        uint32_t ulNotificationValue = ulTaskNotifyTake(pdTRUE, xMaxBlockTime);

        if (ulNotificationValue != 0) {
            taskENTER_CRITICAL();
            //xSemaphoreTake(xMutex, portMAX_DELAY);
            esp_motors_set(esp_motors_status[0], esp_motors_status[1]);
            //xSemaphoreGive(xMutex);
            taskEXIT_CRITICAL();
            vTaskDelay(1);
        }
    };
}

void esp_motors_save(int16_t motor_a, int16_t motor_b) {
    //xSemaphoreTake(xMutex, portMAX_DELAY);
    esp_motors_status[0] = motor_a;
    esp_motors_status[1] = motor_b;
    //xSemaphoreGive(xMutex);

    xTaskNotifyGive(xTaskToNotify);
}

void esp_motors_init() {
    //xMutex = xSemaphoreCreateMutex();
    xTaskCreate(&esp_motors_task, "esp_motors", 256, NULL, 8, &xTaskToNotify);
}
