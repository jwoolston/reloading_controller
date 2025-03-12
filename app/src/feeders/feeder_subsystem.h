#ifndef FEEDER_SUBSYSTEM_H
#define FEEDER_SUBSYSTEM_H

#define BRASS 0 /**< The channel corresponding to the brass feeder. ADC and Motor channels line up. */
#define BULLET 1 /**< The channel corresponding to the bullet feeder. ADC and Motor channels line up. */

/**
 * @brief Initialize the feeder subsystem.
 *
 * Upon initialization, the feeder subsystem will begin monitoring the queue full sensors and conditionally applying
 * activation settings based on if the app has enabled or disabled a specific channel.
 *
 * @return 0 on success or errno code on failure.
 */
int feeder_subsystem_init();

/**
 * @brief Activates (starts) a specific feeder.
 *
 * Once activated, a feeder will engage its motor. If queue full sensor triggers, the motor will automatically
 * disengage. If the queue full sensor clears, the motor will automatically re-engage.
 *
 * @param feeder The channel to start. Must be one of BRASS or BULLET
 * @return 0 on success or errno code on failure.
 */
int start_feeder(int feeder);

/**
 * @brief Deactivates (stops) a specific feeder.
 *
 * Once deactivated, a feeder will not engage its motor, no matter the state of its queue full sensor.
 *
 * @param feeder The channel to stop. Must be one of BRASS or BULLET
 * @return 0 on success or errno code on failure.
 */
int stop_feeder(int feeder);

/**
 * @brief Set a specific feeders speed.
 *
 * @param feeder The channel to stop. Must be one of BRASS or BULLET
 * @param speed The speed percentage to set the feeder to. Must be in the range of [0-100]
 * @return 0 on success or errno code on failure.
 */
int set_feeder_speed(int feeder, int speed);

#endif //FEEDER_SUBSYSTEM_H
