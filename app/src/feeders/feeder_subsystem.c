//
// Created by Jared Woolston on 3/10/2025.
//

#include "feeder_subsystem.h"

#include <app/drivers/motor.h>

#include <zephyr/drivers/adc.h>
#include <zephyr/drivers/dma.h>

#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(feeders, CONFIG_APP_LOG_LEVEL);

#define INVALID_ADC_VALUE SHRT_MIN
#define BUFFER_COUNT 2

typedef int16_t adc_data_size_t;

#define DT_SPEC_AND_COMMA(node_id, prop, idx) ADC_DT_SPEC_GET_BY_IDX(node_id, idx),
#if DT_NODE_HAS_PROP(DT_PATH(zephyr_user), io_channels)
/* Data of ADC io-channels specified in devicetree. */
static const struct adc_dt_spec adc_channels[] = {
    DT_FOREACH_PROP_ELEM(DT_PATH(zephyr_user), io_channels, DT_SPEC_AND_COMMA)
};

static const int adc_channels_count = ARRAY_SIZE(adc_channels);

#else
#error "Unsupported board."
#endif

static __aligned(32) adc_data_size_t m_sample_buffer[BUFFER_COUNT];

K_THREAD_STACK_DEFINE(adc_thread_stack, 1024);
static struct k_thread adc_thread_data;

static k_tid_t adc_thread_tid;

static const struct device* motor_dev;

static int initialize_feeder_sample_adc(void) {
    LOG_DBG("Initializing ADC.");
    if (!adc_is_ready_dt(&adc_channels[0])) {
        LOG_ERR("ADC device is not ready");
        return -EINVAL;
    }

    for (int i = 0; i < adc_channels_count; i++) {
        const int ret = adc_channel_setup_dt(&adc_channels[i]);
        if (ret) {
            LOG_ERR("Setting up of channel %d failed with code %d", i, ret);
            return ret;
        }
    }

    for (int i = 0; i < adc_channels_count; ++i) {
        m_sample_buffer[i] = INVALID_ADC_VALUE;
    }

    return 0;
}

static int feeder_sensor_sample_init(void*, void*, void*) {
    LOG_DBG("Initializing feeder sensor sample task");
    struct adc_sequence_options options = {
        .extra_samplings = 0,
        .interval_us = 0
    };

    struct adc_sequence sequence = {
        .options = &options,
        .buffer = m_sample_buffer,
        .buffer_size = sizeof(m_sample_buffer),
        .resolution = ADC_RESOLUTION_12B,
    };

    initialize_feeder_sample_adc();
    adc_sequence_init_dt(&adc_channels[0], &sequence);

    if (adc_channels_count > 1) {
        sequence.channels |= BIT(adc_channels[1].channel_id);
    }

    while (1) {
        LOG_DBG("Triggering ADC Read.");
        int ret = adc_read_dt(&adc_channels[0], &sequence);
        if (ret) {
            LOG_ERR("adc_read() failed with code %d", ret);
            return ret;
        }

        for (int i = 0; i < adc_channels_count; i++) {
            int32_t sample_value = m_sample_buffer[i];
            ret = adc_raw_to_millivolts(adc_ref_internal(adc_channels[i].dev), adc_channels[i].channel_cfg.gain, sequence.resolution,
                                        &sample_value);

            /* conversion to mV may not be supported, skip if not */
            if ((ret < 0) || adc_channels[i].vref_mv) {
                printf(" (value in mV not available)\n");
            } else {
                LOG_DBG("Channel %d reading: %d mV", i, sample_value);
            }
        }

        k_sleep(K_MSEC(500));
    }
    return 0;
}

int feeder_subsystem_init() {
    LOG_DBG("Searching for motor device.");
    motor_dev = DEVICE_DT_GET(DT_NODELABEL(motors));
    if (!device_is_ready(motor_dev)) {
        LOG_ERR("Motor device not ready");
        return -ENODEV;
    }

    adc_thread_tid = k_thread_create(&adc_thread_data, adc_thread_stack,
                                     K_THREAD_STACK_SIZEOF(adc_thread_stack),
                                     feeder_sensor_sample_init,
                                     NULL, NULL, NULL,
                                     -1, K_ESSENTIAL | K_FP_REGS, K_NO_WAIT);
    return 0;
}