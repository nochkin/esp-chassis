#include <esp/uart.h>

#include "web.h"
#include "esp_motors.h"

void user_init() {
    uart_set_baud(0, 115200);

    web_init();

    esp_motors_init();
}

