#include <stdio.h>
#include <stdlib.h>
#include <c_types.h>
#include <string.h>

#include <jsmn.h>

#include <FreeRTOS.h>
#include <task.h>

#include "esp_motors.h"

#include "esp_cmd.h"

static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
    if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
        strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
            return 0;
    }
    return -1;
}

void esp_cmd_parse(char *cmd, int len) {
    if ((len == 0) || (len >= ESP_CMD_MAX_LEN)) return;

    int r;
    uint8_t ik;
    jsmn_parser p;
    jsmntok_t jsmn_tokens[8];

    jsmn_init(&p);
    r = jsmn_parse(&p, cmd, len, jsmn_tokens, sizeof(jsmn_tokens)/sizeof(jsmn_tokens[0]));
    if (r < 0) {
        printf("Failed parsing JSON: %d\n", r);
        return;
    }
    if (r < 1 || jsmn_tokens[0].type != JSMN_OBJECT) {
        printf("Invalid JSON data\n");
        return;
    }

    int16_t params[ESP_CMD_PARAMS_MAX];
    uint8_t params_s[8];
    for (uint8_t ii=1; ii<r; ii++) {
        if (jsoneq(cmd, &jsmn_tokens[ii], "M") == 0) {
            if (jsmn_tokens[ii+1].type != JSMN_ARRAY) {
                printf("'M' must have its value as an array\n");
                continue;
            }
            for (ik=0; ik<jsmn_tokens[ii+1].size; ik++) {
                jsmntok_t *jsmn_tok = &jsmn_tokens[ii+ik+2];
                uint8_t params_len = jsmn_tok->end - jsmn_tok->start;
                if (params_len <= sizeof(params_s)) {
                    sprintf(params_s, "%.*s\n", jsmn_tok->end - jsmn_tok->start, cmd + jsmn_tok->start);
                    params[ik] = atoi(params_s);
                } else {
                    params[ik] = 0;
                }
            }
            if (ik == 2) {
                printf("Motor: L:%d R:%d\n", params[0], params[1]);
                esp_motors_set(params[0], params[1]);
            }
            ii += jsmn_tokens[ii+1].size + 1;
        }
    }

    //vTaskDelay(1);
}
