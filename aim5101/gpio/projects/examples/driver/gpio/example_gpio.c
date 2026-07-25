/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */


/******************************************************************************
 * @file     example_gpio.c
 * @brief    the main function for the GPIO driver
 * @version  V1.0
 * @date     02. June 2017
 ******************************************************************************/
#include "soc.h"
#include "pin_name.h"
#include "pin.h"
#include "aim5101_codec_gpio.h"

#define CODEC_RESET_DELAY_COUNT 800000UL

int example_pin_gpio_init(void)
{
    return aim5101_codec_gpio_init(&g_aim5101_codec_gpio_default_config);
}

int gpio_output_toggle(void)
{
    int ret = example_pin_gpio_init();

    if (ret != 0) {
        return ret;
    }

    aim5101_codec_gpio_power(true);
    aim5101_codec_gpio_reset_pulse(CODEC_RESET_DELAY_COUNT);
    aim5101_codec_gpio_mute(false);

    return 0;
}

/*****************************************************************************
test_gpio: main function of the gpio test

INPUT: NULL

RETURN: NULL

*****************************************************************************/
int example_gpio(pin_name_e gpio_pin)
{
    (void)gpio_pin;
    return gpio_output_toggle();
}

int main(void)
{
    return example_gpio(EXAMPLE_GPIO_PIN);
}
