#include <stdio.h>
#include <stdlib.h>
#include <c_types.h>
#include <string.h>

#include <FreeRTOS.h>
#include <task.h>

#include "esp_motors.h"

#include "esp_cmd.h"

void esp_cmd_parse(char *cmd, int len) {
    if ((len == 0) || (len >= ESP_CMD_MAX_LEN)) return;

    char *cmd_ptr = cmd;

    char *token = strtok_r(cmd_ptr, ":", &cmd_ptr);
    if (token == NULL) return;

    int16_t params[ESP_CMD_PARAMS_MAX];
    int params_len = 0;
    while((token = strtok_r(NULL, " ", &cmd_ptr))) {
        params[params_len] = atoi(token);
        params_len++;
        if (params_len >= ESP_CMD_PARAMS_MAX) break;
    }

    switch(cmd[0]) {
        case 'M': // M: 255 255
            if (params_len == 2) {
                printf("Motor: L:%d R:%d\n", params[0], params[1]);
                esp_motors_set(params[0], params[1]);
            }
            break;
        case 'L': // L: 0 255 1 255
            for (uint8_t ii=0; ii<(params_len-1); ii++) {
                if (params[ii+1] < 0) {
                    //gpio1_toggle(params[ii]);
                } else {
                    //gpio1_set(params[ii], params[ii+1] > 0 ? 255 : 0);
                }
                ii++;
            }
            break;
        default:
            break;
    }

    //vTaskDelay(1);
}
