/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */


/******************************************************************************
 * @file     pinmux.c
 * @brief    source file for the pinmux
 * @version  V1.0
 * @date     02. June 2017
 ******************************************************************************/
#include <stdint.h>
#include "pinmux.h"
#include "pin_name.h"
#include <drv_gpio.h>

#define AIM5101_CORE_IO_BASE      0x61001800UL
#define AIM5101_IOCFG(offset)     (*(volatile uint32_t *)(AIM5101_CORE_IO_BASE + (offset)))

#define AIM5101_IOCFG_FUNCSEL_MASK    0x0fUL
#define AIM5101_IOCFG_PULL_DOWN       (1UL << 4)
#define AIM5101_IOCFG_PULL_UP         (1UL << 5)
#define AIM5101_IOCFG_OPEN_DRAIN      (1UL << 17)

static int32_t get_iocfg_offset(port_name_e port, uint8_t offset, uint32_t *iocfg_offset)
{
    if (iocfg_offset == 0 || port != PORTA || offset > 3) {
        return -1;
    }

    *iocfg_offset = (uint32_t)offset * 4UL;
    return 0;
}

int32_t drv_pinmux_config(pin_name_e pin, pin_func_e pin_func)
{
    (void)pin;
    (void)pin_func;
    return 0;
}

int32_t drv_pin_config_mode(port_name_e port, uint8_t offset, gpio_mode_e pin_mode)
{
    uint32_t iocfg_offset;
    uint32_t value;

    if (get_iocfg_offset(port, offset, &iocfg_offset) != 0) {
        return -1;
    }

    value = AIM5101_IOCFG(iocfg_offset);
    value &= ~(AIM5101_IOCFG_FUNCSEL_MASK | AIM5101_IOCFG_PULL_DOWN |
               AIM5101_IOCFG_PULL_UP | AIM5101_IOCFG_OPEN_DRAIN);

    switch (pin_mode) {
        case GPIO_MODE_PULLNONE:
        case GPIO_MODE_PUSH_PULL:
            break;
        case GPIO_MODE_PULLUP:
            value |= AIM5101_IOCFG_PULL_UP;
            break;
        case GPIO_MODE_PULLDOWN:
            value |= AIM5101_IOCFG_PULL_DOWN;
            break;
        case GPIO_MODE_OPEN_DRAIN:
            value |= AIM5101_IOCFG_OPEN_DRAIN;
            break;
        default:
            return -1;
    }

    AIM5101_IOCFG(iocfg_offset) = value;
    return 0;
}
