#include "feeder_subsystem.h"

#include <app/drivers/motor.h>

#include <zephyr/drivers/adc.h>
#include <zephyr/drivers/dma.h>

#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(feeders, CONFIG_FEEDER_SUBSYSTEM_LOG_LEVEL);

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

static bool is_valid_feeder(const int feeder) {
    if (feeder != BRASS && feeder != BULLET) {
        LOG_ERR("Invalid feeder index: %d", feeder);
        return false;
    }
    return true;
}

static int initialize_feeder_adc(void) {
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

static int feeder_sensor_init(void*, void*, void*) {
    LOG_DBG("Initializing feeder sensor task");
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

    initialize_feeder_adc();
    adc_sequence_init_dt(&adc_channels[BRASS], &sequence);

    if (adc_channels_count > 1) {
        sequence.channels |= BIT(adc_channels[BULLET].channel_id);
    }

    while (1) {
        int ret = adc_read_dt(&adc_channels[0], &sequence);
        if (ret) {
            LOG_ERR("adc_read_dt() failed with code %d", ret);
            return ret;
        }

        for (int i = 0; i < adc_channels_count; i++) {
            int32_t sample_value = m_sample_buffer[i];
            ret = adc_raw_to_millivolts(adc_ref_internal(adc_channels[i].dev), adc_channels[i].channel_cfg.gain,
                sequence.resolution, &sample_value);

            // Conversion to mV may not be supported, skip if not
            if (ret < 0 || adc_channels[i].vref_mv) {
                LOG_ERR("Value in mV not available. Error: %d", ret);
            } else {
                //LOG_DBG("Channel %d reading: %d mV", i, sample_value);
                if (sample_value > CONFIG_FEEDER_FULL_SENSOR_THRESHOLD_MV) {
                    //LOG_DBG("Channel %d is full", i);
                    motor_off(motor_dev, i);
                } else {
                    motor_on(motor_dev, i);
                }
            }
        }
        // Sleep until we care about checking again
        k_sleep(K_MSEC(500));
    }
    return 0;
}

int feeder_subsystem_init() {
    motor_dev = DEVICE_DT_GET(DT_NODELABEL(motors));
    if (!device_is_ready(motor_dev)) {
        LOG_ERR("Motor device not ready");
        return -ENODEV;
    }
    LOG_DBG("Activating feeders");
    set_feeder_speed(BRASS, 25);
    set_feeder_speed(BULLET, 75);
    start_feeder(BRASS);
    start_feeder(BULLET);

    LOG_DBG("Starting feeder queue monitor thread.");
    adc_thread_tid = k_thread_create(&adc_thread_data, adc_thread_stack,
                                     K_THREAD_STACK_SIZEOF(adc_thread_stack),
                                     (k_thread_entry_t)feeder_sensor_init,
                                     NULL, NULL, NULL,
                                     CONFIG_FEEDER_FULL_SENSOR_THREAD_PRIORITY,
                                     K_ESSENTIAL | K_FP_REGS, K_NO_WAIT);
    return 0;
}

int start_feeder(const int feeder) {
    if (!is_valid_feeder(feeder)) {
        return -EINVAL;
    }
    //LOG_DBG("Starting feeder %d", feeder);
    return motor_on(motor_dev, feeder);
}

int stop_feeder(const int feeder) {
    if (!is_valid_feeder(feeder)) {
        return -EINVAL;
    }
    //LOG_DBG("Stopping feeder %d", feeder);
    return motor_off(motor_dev, feeder);
}

int set_feeder_speed(const int feeder, const int speed) {
    if (!is_valid_feeder(feeder)) {

        return -EINVAL;
    }
    if (speed < 0 || speed > 100) {
        LOG_ERR("Invalid speed value %d. Must be in the range of [0-100].", speed);
        return -EINVAL;
    }
    LOG_DBG("Setting feeder %d speed to %d", feeder, speed);
    return motor_set_speed(motor_dev, feeder, speed);
}