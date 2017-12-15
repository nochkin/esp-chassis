#ifndef ESP_CMD_H
#define ESP_CMD_H

#define ESP_CMD_MAX_LEN		64
#define ESP_CMD_PARAMS_MAX	8

void esp_cmd_parse(char *cmd, int len);

#endif
