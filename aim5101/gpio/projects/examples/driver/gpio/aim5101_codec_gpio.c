#include "aim5101_codec_gpio.h"
#include "drv_gpio.h"

typedef struct {
    const aim5101_codec_gpio_pin_t *config;
    gpio_pin_handle_t handle;
} codec_gpio_runtime_t;

const aim5101_codec_gpio_config_t g_aim5101_codec_gpio_default_config = {
    .reset = {
        .pin = PA0,
        .active_high = false,
        .used = true,
    },
    .power_enable = {
        .pin = PA1,
        .active_high = true,
        .used = true,
    },
    .mute = {
        .pin = PA2,
        .active_high = true,
        .used = true,
    },
};

static aim5101_codec_gpio_config_t g_codec_gpio_config;
static codec_gpio_runtime_t g_reset_gpio;
static codec_gpio_runtime_t g_power_gpio;
static codec_gpio_runtime_t g_mute_gpio;

static void codec_delay(uint32_t delay_count)
{
    volatile uint32_t i;

    for (i = 0; i < delay_count; i++) {
        __asm volatile("nop");
    }
}

static int codec_gpio_write(codec_gpio_runtime_t *runtime, bool active)
{
    bool level;

    if (runtime == 0 || runtime->config == 0 || !runtime->config->used) {
        return 0;
    }

    if (runtime->handle == 0) {
        return -1;
    }

    level = runtime->config->active_high ? active : !active;
    return csi_gpio_pin_write(runtime->handle, level);
}

static int codec_gpio_init_one(codec_gpio_runtime_t *runtime,
                               const aim5101_codec_gpio_pin_t *config,
                               bool active)
{
    int ret;

    runtime->config = config;
    runtime->handle = 0;

    if (config == 0 || !config->used) {
        return 0;
    }

    runtime->handle = csi_gpio_pin_initialize(config->pin, 0);

    if (runtime->handle == 0) {
        return -1;
    }

    ret = csi_gpio_pin_config(runtime->handle, GPIO_MODE_PUSH_PULL, GPIO_DIRECTION_OUTPUT);

    if (ret != 0) {
        return ret;
    }

    return codec_gpio_write(runtime, active);
}

int aim5101_codec_gpio_init(const aim5101_codec_gpio_config_t *config)
{
    int ret;

    if (config == 0) {
        config = &g_aim5101_codec_gpio_default_config;
    }

    g_codec_gpio_config = *config;

    ret = codec_gpio_init_one(&g_power_gpio, &g_codec_gpio_config.power_enable, false);

    if (ret != 0) {
        return ret;
    }

    ret = codec_gpio_init_one(&g_reset_gpio, &g_codec_gpio_config.reset, false);

    if (ret != 0) {
        return ret;
    }

    return codec_gpio_init_one(&g_mute_gpio, &g_codec_gpio_config.mute, true);
}

void aim5101_codec_gpio_power(bool enable)
{
    (void)codec_gpio_write(&g_power_gpio, enable);
}

void aim5101_codec_gpio_reset_pulse(uint32_t delay_count)
{
    (void)codec_gpio_write(&g_reset_gpio, true);
    codec_delay(delay_count);
    (void)codec_gpio_write(&g_reset_gpio, false);
    codec_delay(delay_count);
}

void aim5101_codec_gpio_mute(bool enable)
{
    (void)codec_gpio_write(&g_mute_gpio, enable);
}
