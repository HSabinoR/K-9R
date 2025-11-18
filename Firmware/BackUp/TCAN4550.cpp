#include "TCAN4550.h"
#include <TCAN4550_SPI.h>

bool Device_Configure(TCAN4x5x_DEV_CONFIG *deviceConfig) {
    

}

/**
 * @brief Reads the device configuration register and returns its value.
 *
 * @return A TCAN4x5x_DEV_CONFIG struct containing the current device configuration.
 */
TCAN4x5x_DEV_CONFIG Device_ReadConfig() {
     TCAN4x5x_DEV_CONFIG deviceConfig;

    // Read the 32-bit register value via SPI
    deviceConfig.word = TCAN4550_ReadRegister(REG_DEV_MODES_AND_PINS);

    // Return the filled struct
    return deviceConfig;
}

/**
 * @brief Sets the TCAN4550 device mode
 *
 * Sets the TCAN4550 device mode based on the input @c mode enum
 *
 * @param MODE_NORMAL Sets device mode to normal
 * @param MODE_STANDBY Sets device mode to standby
 * @param MODE_SLEEP Puts the device to sleep
 *
 * @return @c true if configuration successfully done, @c false if not
 */
bool Device_SetMode(Device_Mode mode) {
    TCAN4550_ModifyRegister(REG_DEV_MODES_AND_PINS, REG_BITS_DEVICE_MODE_DEVICEMODE_MASK, mode);

#ifdef TCAN4550_DEVICE_VERIFY_CONFIGURATION_WRITES
    uint32_t verify = TCAN4550_ReadRegister(REG_DEV_MODES_AND_PINS);
    if ((verify & REG_BITS_DEVICE_MODE_DEVICEMODE_MASK)
        != mode)
        return false;
#endif

    return true;
}

/**
 * @brief Enables the TCAN4550 device test mode
 *
 * @return @c true if enabling test mode was successful, @c false if not
 */
bool Device_EnableTestMode() {
    TCAN4550_ModifyRegister(REG_DEV_MODES_AND_PINS, REG_BITS_DEVICE_MODE_TESTMODE_ENMASK, REG_BITS_DEVICE_MODE_TESTMODE_EN);

#ifdef TCAN4550_DEVICE_VERIFY_CONFIGURATION_WRITES
    uint32_t verify = TCAN4550_ReadRegister(REG_DEV_MODES_AND_PINS);
    if ((verify & REG_BITS_DEVICE_MODE_TESTMODE_ENMASK)
        != REG_BITS_DEVICE_MODE_TESTMODE_EN)
        return false;
#endif

    return true;
}

/**
 * @brief Disables the TCAN4550 device test mode
 *
 * @return @c true if disabling test mode was successful, @c false if not
 */
bool Device_DisableTestMode() {
    TCAN4550_ModifyRegister(REG_DEV_MODES_AND_PINS, REG_BITS_DEVICE_MODE_TESTMODE_ENMASK, REG_BITS_DEVICE_MODE_TESTMODE_DIS);

#ifdef TCAN4550_DEVICE_VERIFY_CONFIGURATION_WRITES
    uint32_t verify = TCAN4550_ReadRegister(REG_DEV_MODES_AND_PINS);
    if ((verify & REG_BITS_DEVICE_MODE_TESTMODE_ENMASK)
        != REG_BITS_DEVICE_MODE_TESTMODE_DIS)
        return false;
#endif

    return true;
}

/** 
 * @brief Reads the TCAN4550 device mode
 * 
 * @return The device's current Mode of Operation 
*/
Device_Mode Read_DeviceMode() {
    uint32_t mode = (TCAN4550_ReadRegister(REG_DEV_MODES_AND_PINS) & REG_BITS_DEVICE_MODE_DEVICEMODE_MASK);

    switch (mode) {
        case REG_BITS_DEVICE_MODE_DEVICEMODE_NORMAL:
            return MODE_NORMAL;

        case REG_BITS_DEVICE_MODE_DEVICEMODE_SLEEP:
            return MODE_SLEEP;

        case REG_BITS_DEVICE_MODE_DEVICEMODE_STANDBY:
            return MODE_STANDBY;

        default:
            return MODE_STANDBY;
    }
}


/**
 * @brief Configure the WatchDog Timer settings.
 *
 * This function configures the WatchDog Timer (WDT) with the specified timeout value,
 * timeout action, and clock reference.
 *
 * @param timeout_value  The desired watchdog timeout period. See ::WDT_Timeout_Value:
 *                       - WDT_60MS   (60 ms)
 *                       - WDT_600MS  (600 ms)
 *                       - WDT_3S     (3 seconds)
 *                       - WDT_6S     (6 seconds)
 * @param timeout_action The action to take on watchdog timeout. See ::WDT_Timout_Action:
 *                       - ACTION_INTERRUPT
 *                       - ACTION_INTERRUPT_INH_PULSE
 *                       - ACTION_WDT_PULSE
 * @param clock_ref      The clock reference for the watchdog timer. See ::WDT_Clock_Ref:
 *                       - CLK_40MHZ
 *                       - CLK_20MHZ
 *
 * @return @c true if successfully configured, or @c false otherwise.
 */
bool WDT_Configure(WDT_Timeout_Value timeout_value, WDT_Timout_Action timeout_action, WDT_Clock_Ref clock_ref) {
    uint32_t regVal = TCAN4550_ReadRegister(REG_DEV_MODES_AND_PINS);

    // clear all relevant bits first
    regVal &= ~(REG_BITS_DEVICE_MODE_WDT_MASK     |
                REG_BITS_DEVICE_MODE_WDT_ACTION_MASK |
                REG_BITS_DEVICE_MODE_WD_CLK_MASK);

    // set new values
    regVal |= (timeout_value << 28);
    regVal |= (timeout_action << 16);
    regVal |= (clock_ref << 27);

    TCAN4550_WriteRegister(REG_DEV_MODES_AND_PINS, regVal);

#ifdef TCAN4550_DEVICE_VERIFY_CONFIGURATION_WRITES
    uint32_t verify = TCAN4550_ReadRegister(REG_DEV_MODES_AND_PINS);
    if ((verify & (REG_BITS_DEVICE_MODE_WDT_MASK |
                   REG_BITS_DEVICE_MODE_WDT_ACTION_MASK |
                   REG_BITS_DEVICE_MODE_WD_CLK_MASK))
        != (regVal & (REG_BITS_DEVICE_MODE_WDT_MASK |
                      REG_BITS_DEVICE_MODE_WDT_ACTION_MASK |
                      REG_BITS_DEVICE_MODE_WD_CLK_MASK)))
        return false;
#endif

    return true;
}

/**
 * @brief Enable the watchdog timer
 *
 * @return @c true if successfully enabled, or @c false otherwise
 */
bool WDT_Enable() {
    TCAN4550_ModifyRegister(REG_DEV_MODES_AND_PINS, 
                            REG_BITS_DEVICE_MODE_WDT_MASK, 
                            REG_BITS_DEVICE_MODE_WDT_EN);

#ifdef TCAN4550_DEVICE_VERIFY_CONFIGURATION_WRITES
    uint32_t verify = TCAN4550_ReadRegister(REG_DEV_MODES_AND_PINS);
    if ((verify & REG_BITS_DEVICE_MODE_WDT_MASK) != REG_BITS_DEVICE_MODE_WDT_EN)
        return false;
#endif
    return true;
}

/**
 * @brief Disable the watchdog timer
 *
 * @return @c true if successfully disabled, or @c false otherwise
 */
bool WDT_Disable() {
    TCAN4550_ModifyRegister(REG_DEV_MODES_AND_PINS, 
                            REG_BITS_DEVICE_MODE_WDT_MASK, 
                            REG_BITS_DEVICE_MODE_WDT_DIS);

#ifdef TCAN4550_DEVICE_VERIFY_CONFIGURATION_WRITES
    uint32_t verify = TCAN4550_ReadRegister(REG_DEV_MODES_AND_PINS);
    if ((verify & REG_BITS_DEVICE_MODE_WDT_MASK) != REG_BITS_DEVICE_MODE_WDT_DIS)
        return false;
#endif
    return true;
}

/**
 * @brief Reset the watchdog timer
 */
bool WDT_Reset() {
    TCAN4550_ModifyRegister(REG_DEV_MODES_AND_PINS, 
                            REG_BITS_DEVICE_MODE_WDT_RESET_BIT, 
                            REG_BITS_DEVICE_MODE_WDT_RESET_BIT);

#ifdef TCAN4550_DEVICE_VERIFY_CONFIGURATION_WRITES
    uint32_t verify = TCAN4550_ReadRegister(REG_DEV_MODES_AND_PINS);
    if ((verify & REG_BITS_DEVICE_MODE_WDT_MASK) != REG_BITS_DEVICE_MODE_WDT_RESET_BIT)
        return false;
#endif
    return true;
}
