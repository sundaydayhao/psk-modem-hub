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

#define AIM5101_GPIOA_BASE      0x60050000UL
#define AIM5101_GPIO_DATA       (*(volatile uint32_t *)(AIM5101_GPIOA_BASE + 0x000UL))
#define AIM5101_GPIO_DIR        (*(volatile uint32_t *)(AIM5101_GPIOA_BASE + 0x400UL))
#define AIM5101_GPIO_DATA_EN    (*(volatile uint32_t *)(AIM5101_GPIOA_BASE + 0x424UL))

#define AIM5101_CRG_BASE        0x61000000UL
#define AIM5101_PERI_CRG40      (*(volatile uint32_t *)(AIM5101_CRG_BASE + 0x0A0UL))

#define AIM5101_CORE_IO_BASE    0x61001800UL
#define AIM5101_PA0_IOCFG       (*(volatile uint32_t *)(AIM5101_CORE_IO_BASE + 0x000UL))

#define PA0_BIT                 (1UL << 0)
#define GPIO_TOGGLE_DELAY_COUNT 800000UL

void example_pin_gpio_init(void)
{
    AIM5101_PA0_IOCFG = 0x00000000UL;
}

static void gpio_delay(void)
{
    volatile uint32_t i;

    for (i = 0; i < GPIO_TOGGLE_DELAY_COUNT; i++) {
        __asm volatile("nop");
    }
}

void gpio_output_toggle(void)
{
    example_pin_gpio_init();

    AIM5101_PERI_CRG40 = (AIM5101_PERI_CRG40 | 0x01UL) & ~(1UL << 16);
    AIM5101_GPIO_DATA_EN |= PA0_BIT;
    AIM5101_GPIO_DIR |= PA0_BIT;

    while (1) {
        AIM5101_GPIO_DATA &= ~PA0_BIT;
        gpio_delay();
        AIM5101_GPIO_DATA |= PA0_BIT;
        gpio_delay();
    }
}

/*****************************************************************************
test_gpio: main function of the gpio test

INPUT: NULL

RETURN: NULL

*****************************************************************************/
int example_gpio(pin_name_e gpio_pin)
{
    (void)gpio_pin;
    gpio_output_toggle();
    return 0;
}

int main(void)
{
    return example_gpio(EXAMPLE_GPIO_PIN);
}
