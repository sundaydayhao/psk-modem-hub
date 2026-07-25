#ifndef AIM5101_CODEC_GPIO_H
#define AIM5101_CODEC_GPIO_H

#include <stdbool.h>
#include <stdint.h>
#include "pin_name.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    pin_name_e pin;
    bool active_high;
    bool used;
} aim5101_codec_gpio_pin_t;

typedef struct {
    aim5101_codec_gpio_pin_t reset;
    aim5101_codec_gpio_pin_t power_enable;
    aim5101_codec_gpio_pin_t mute;
} aim5101_codec_gpio_config_t;

extern const aim5101_codec_gpio_config_t g_aim5101_codec_gpio_default_config;

int aim5101_codec_gpio_init(const aim5101_codec_gpio_config_t *config);
void aim5101_codec_gpio_power(bool enable);
void aim5101_codec_gpio_reset_pulse(uint32_t delay_count);
void aim5101_codec_gpio_mute(bool enable);

#ifdef __cplusplus
}
#endif

#endif
